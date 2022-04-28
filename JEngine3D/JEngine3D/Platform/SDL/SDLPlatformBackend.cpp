#include "SDLPlatformBackend.hpp"

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Events.hpp"
#include "JEngine3D/Core/LoggerController.hpp"// for Logger
#include "JEngine3D/Core/Base.hpp"// for UNUSED
#include "JEngine3D/Core/Types.hpp"

#include <exception>// for exception
#include <cstring>// IWYU pragma: keep

#include <SDL_mouse.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_events.h>// for SDL_PollEvent, SDL_Event, SDL_...
#include <SDL_error.h>// for SDL_GetError
#include <SDL_video.h>// for SDL_Window, SDL_CreateWindow, SDL_DestroyWindow
#include <SDL_keyboard.h>// for SDL_Keysym
#include <SDL_keycode.h>// for SDL_Keycode
#include <SDL_clipboard.h>// for SDL_GetClipboardText
#include <SDL_hints.h>// for SDL_SetHint, SDL_HINT...
#include <SDL_rect.h>
#include <SDL.h>

namespace JE {

static constexpr auto SDLKeyCodeToJEngine3DKeyCode(SDL_Keycode keyCode) -> KeyCode
{
  return static_cast<KeyCode>(keyCode);
}

static auto s_Initialized = false;// NOLINT

SDLPlatformBackend::SDLPlatformBackend() { JE::UNUSED(Initialize()); }

SDLPlatformBackend::~SDLPlatformBackend()
{
  if (m_ClipboardText != nullptr) {
    SDL_free(m_ClipboardText);// NOLINT(cppcoreguidelines-no-malloc,hicpp-no-malloc,cppcoreguidelines-owning-memory)
  }
  SDL_Quit();
}

auto SDLPlatformBackend::Initialize() -> bool
{
  ASSERT(!s_Initialized, "Backend already initialized");

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    JE::Logger::CoreLogger().error("SDL Failed to initialize - {}", SDL_GetError());
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // ImGui - Required
  // Set SDL hint to receive mouse click events on window focus, otherwise SDL doesn't emit the event.
  // Without this, when clicking to gain focus, our widgets wouldn't activate even though they showed as hovered.
  // (This is unfortunately a global SDL setting, so enabling it might have a side-effect on your application.
  // It is unlikely to make a difference, but if your app absolutely needs to ignore the initial on-focus click:
  // you can ignore SDL_MOUSEBUTTONDOWN events coming right after a SDL_WINDOWEVENT_FOCUS_GAINED)
  SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

  s_Initialized = true;
  return true;
}

auto SDLPlatformBackend::Initialized() -> bool { return s_Initialized; }

auto SDLPlatformBackend::CreateWindow(const std::string_view &title,
  const Size2DI &size,
  const Position2DI &position,
  const WindowConfiguration &config) -> NativeWindowHandle
{
  ASSERT(size.Width > 0 && size.Height > 0, "Window size cannot be zero or negative");

  uint32_t sdlFlags = 0;
  sdlFlags |= SDL_WINDOW_OPENGL;
  sdlFlags |= (config.Decorated ? SDL_WINDOW_RESIZABLE : 0);// NOLINT
  sdlFlags |= (!config.Decorated ? SDL_WINDOW_BORDERLESS : 0);// NOLINT
  sdlFlags |= SDL_WINDOW_ALLOW_HIGHDPI;
  sdlFlags |= (config.Minimized ? 0 : SDL_WINDOW_INPUT_FOCUS);// NOLINT
  sdlFlags |= (config.Minimized ? SDL_WINDOW_MINIMIZED : 0);// NOLINT
  sdlFlags |= (config.Hidden ? SDL_WINDOW_HIDDEN : 0);// NOLINT

  return SDL_CreateWindow(std::string(title).c_str(),
    position != WINDOW_CENTER_POSITION ? position.X
                                       : static_cast<int32_t>(SDL_WINDOWPOS_CENTERED),// NOLINT(hicpp-signed-bitwise)
    position != WINDOW_CENTER_POSITION ? position.Y
                                       : static_cast<int32_t>(SDL_WINDOWPOS_CENTERED),// NOLINT(hicpp-signed-bitwise)
    size.Width,
    size.Height,
    sdlFlags);
}

void SDLPlatformBackend::DestroyWindow(NativeWindowHandle handle)
{
  ASSERT(handle, "Window handle is null");
  SDL_DestroyWindow(static_cast<SDL_Window *>(handle));
}

auto SDLPlatformBackend::ValidWindowHandle(NativeWindowHandle handle) -> bool
{
  ASSERT(handle, "Window handle is null");
  return SDL_GetWindowID(static_cast<SDL_Window *>(handle)) != 0;
}

auto SDLPlatformBackend::WindowSize(NativeWindowHandle handle) -> Size2DI
{
  ASSERT(handle, "Window handle is null");
  Size2DI size{};
  SDL_GetWindowSize(static_cast<SDL_Window *>(handle), &size.Width, &size.Height);
  return size;
}

void SDLPlatformBackend::SetWindowSize(NativeWindowHandle handle, const Size2DI &size)
{
  ASSERT(handle, "Window handle is null");
  ASSERT(size.Width > 0 && size.Height > 0, "Window size cannot be zero or negative");
  SDL_SetWindowSize(static_cast<SDL_Window *>(handle), size.Width, size.Height);
}

auto SDLPlatformBackend::WindowTitle(NativeWindowHandle handle) -> std::string_view
{
  ASSERT(handle, "Window handle is null");
  return SDL_GetWindowTitle(static_cast<SDL_Window *>(handle));
}

void SDLPlatformBackend::SetWindowTitle(NativeWindowHandle handle, const std::string_view &title)
{
  ASSERT(handle, "Window handle is null");
  SDL_SetWindowTitle(static_cast<SDL_Window *>(handle), std::string(title).c_str());
}

auto SDLPlatformBackend::WindowPosition(NativeWindowHandle handle) -> Position2DI
{
  ASSERT(handle, "Window handle is null");
  Position2DI position{};
  SDL_GetWindowPosition(static_cast<SDL_Window *>(handle), &position.X, &position.Y);
  return position;
}

void SDLPlatformBackend::SetWindowPosition(NativeWindowHandle handle, const Position2DI &position)
{
  ASSERT(handle, "Window handle is null");
  SDL_SetWindowPosition(static_cast<SDL_Window *>(handle), position.X, position.Y);
}

auto SDLPlatformBackend::WindowHidden(NativeWindowHandle handle) -> bool
{
  ASSERT(handle, "Window handle is null");
  return (SDL_GetWindowFlags(static_cast<SDL_Window *>(handle)) & SDL_WINDOW_HIDDEN) == SDL_WINDOW_HIDDEN;
}

void SDLPlatformBackend::ShowWindow(NativeWindowHandle handle)
{
  ASSERT(handle, "Window handle is null");
  SDL_ShowWindow(static_cast<SDL_Window *>(handle));
}

void SDLPlatformBackend::HideWindow(NativeWindowHandle handle)
{
  ASSERT(handle, "Window handle is null");
  SDL_HideWindow(static_cast<SDL_Window *>(handle));
}

auto SDLPlatformBackend::WindowFocused(NativeWindowHandle handle) -> bool
{
  ASSERT(handle, "Window handle is null");
  return (SDL_GetWindowFlags(static_cast<SDL_Window *>(handle)) & SDL_WINDOW_INPUT_FOCUS) == SDL_WINDOW_INPUT_FOCUS;
}

void SDLPlatformBackend::FocusWindow(NativeWindowHandle handle)
{
  ASSERT(handle, "Window handle is null");
  SDL_RaiseWindow(static_cast<SDL_Window *>(handle));
  SDL_SetWindowInputFocus(static_cast<SDL_Window *>(handle));
}

auto SDLPlatformBackend::FocusedWindow() -> NativeWindowHandle { return SDL_GetKeyboardFocus(); }

auto SDLPlatformBackend::WindowMinimized(NativeWindowHandle handle) -> bool
{
  ASSERT(handle, "Window handle is null");
  return (SDL_GetWindowFlags(static_cast<SDL_Window *>(handle)) & SDL_WINDOW_MINIMIZED) == SDL_WINDOW_MINIMIZED;
}

void SDLPlatformBackend::MinimizeWindow(NativeWindowHandle handle)
{
  SDL_MinimizeWindow(static_cast<SDL_Window *>(handle));
}

void SDLPlatformBackend::MaximizeWindow(NativeWindowHandle handle)
{
  SDL_RestoreWindow(static_cast<SDL_Window *>(handle));
}

auto SDLPlatformBackend::GetMonitorCount() -> int32_t { return SDL_GetNumVideoDisplays(); }

auto SDLPlatformBackend::GetDisplayBounds(int32_t displayIndex) -> RectangleI
{
  SDL_Rect sdlRect;
  RectangleI rect{};
  SDL_GetDisplayBounds(displayIndex, &sdlRect);
  rect.Position.X = sdlRect.x;
  rect.Position.Y = sdlRect.y;
  rect.Size.Width = sdlRect.w;
  rect.Size.Height = sdlRect.h;
  return rect;
}

auto SDLPlatformBackend::GetDisplayUsableBounds(int32_t displayIndex) -> RectangleI
{
  SDL_Rect sdlRect;
  RectangleI rect{};
  SDL_GetDisplayUsableBounds(displayIndex, &sdlRect);
  rect.Position.X = sdlRect.x;
  rect.Position.Y = sdlRect.y;
  rect.Size.Width = sdlRect.w;
  rect.Size.Height = sdlRect.h;
  return rect;
}

auto SDLPlatformBackend::GetDisplayDPI(int32_t displayIndex) -> float
{
  float dpi = 0;
  SDL_GetDisplayDPI(displayIndex, &dpi, nullptr, nullptr);
  return dpi;
}

void SDLPlatformBackend::CaptureMouse() { SDL_CaptureMouse(SDL_TRUE); }

void SDLPlatformBackend::ReleaseMouse() { SDL_CaptureMouse(SDL_FALSE); }

void SDLPlatformBackend::PollEvents()// NOLINT(readability-function-cognitive-complexity)
{
  auto ProcessWindowResizeEvent = [&](const SDL_Event &nativeEvent) {
    WindowResizeEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID),
      { nativeEvent.window.data1, nativeEvent.window.data2 } };
    EventProcessor().OnEvent(event);
  };

  auto ProcessWindowCloseEvent = [&](const SDL_Event &nativeEvent) {
    WindowCloseEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID) };
    EventProcessor().OnEvent(event);
  };

  auto ProcessWindowMoveEvent = [&](const SDL_Event &nativeEvent) {
    WindowMoveEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID),
      { nativeEvent.window.data1, nativeEvent.window.data2 } };
    EventProcessor().OnEvent(event);
  };

  auto ProcessWindowHideEvent = [&](const SDL_Event &nativeEvent) {
    WindowHideEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID) };
    EventProcessor().OnEvent(event);
  };

  auto ProcessWindowShowEvent = [&](const SDL_Event &nativeEvent) {
    WindowShowEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID) };
    EventProcessor().OnEvent(event);
  };

  auto ProcessWindowFocusGainedEvent = [&](const SDL_Event &nativeEvent) {
    WindowFocusGainedEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID) };
    EventProcessor().OnEvent(event);
  };

  auto ProcessWindowFocusLostEvent = [&](const SDL_Event &nativeEvent) {
    WindowFocusLostEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID) };
    EventProcessor().OnEvent(event);
  };

  auto ProcessWindowMinimizedEvent = [&](const SDL_Event &nativeEvent) {
    WindowMinimizedEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID) };
    EventProcessor().OnEvent(event);
  };

  auto ProcessWindowMaximizedEvent = [&](const SDL_Event &nativeEvent) {
    WindowMaximizedEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID) };
    EventProcessor().OnEvent(event);
  };

  SDL_Event nativeEvent;
  while (SDL_PollEvent(&nativeEvent) != 0) {
    if (nativeEvent.type == SDL_EventType::SDL_QUIT) {
      QuitEvent event;
      EventProcessor().OnEvent(event);
    }

    if (nativeEvent.type == SDL_EventType::SDL_WINDOWEVENT) {

      auto *window = SDL_GetWindowFromID(nativeEvent.window.windowID);
      if (window != nullptr) {

        switch (nativeEvent.window.event) {
        case SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED:
          ProcessWindowResizeEvent(nativeEvent);
          break;

        case SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE:
          ProcessWindowCloseEvent(nativeEvent);
          break;

        case SDL_WindowEventID::SDL_WINDOWEVENT_MOVED:
          ProcessWindowMoveEvent(nativeEvent);
          break;

        case SDL_WindowEventID::SDL_WINDOWEVENT_HIDDEN:
          ProcessWindowHideEvent(nativeEvent);
          break;

        case SDL_WindowEventID::SDL_WINDOWEVENT_SHOWN:
          ProcessWindowShowEvent(nativeEvent);
          break;

        case SDL_WindowEventID::SDL_WINDOWEVENT_FOCUS_GAINED:
          ProcessWindowFocusGainedEvent(nativeEvent);
          break;

        case SDL_WindowEventID::SDL_WINDOWEVENT_FOCUS_LOST:
          ProcessWindowFocusLostEvent(nativeEvent);
          break;

        case SDL_WindowEventID::SDL_WINDOWEVENT_MINIMIZED:
          ProcessWindowMinimizedEvent(nativeEvent);
          break;

        case SDL_WindowEventID::SDL_WINDOWEVENT_MAXIMIZED:
          ProcessWindowMaximizedEvent(nativeEvent);
          break;

        default:
          // Nothing to do for default case
          break;
        }
      }
    }

    if (nativeEvent.type == SDL_EventType::SDL_KEYDOWN) {
      KeyModifiers modifiers = { (nativeEvent.key.keysym.mod & KMOD_CTRL) != 0,
        (nativeEvent.key.keysym.mod & KMOD_SHIFT) != 0,
        (nativeEvent.key.keysym.mod & KMOD_ALT) != 0,
        (nativeEvent.key.keysym.mod & KMOD_GUI) != 0 };

      KeyPressEvent event{ SDL_GetWindowFromID(nativeEvent.key.windowID),
        SDLKeyCodeToJEngine3DKeyCode(nativeEvent.key.keysym.sym),
        modifiers,
        nativeEvent.key.repeat };
      EventProcessor().OnEvent(event);
    }

    if (nativeEvent.type == SDL_EventType::SDL_KEYUP) {
      KeyModifiers modifiers = { (nativeEvent.key.keysym.mod & KMOD_CTRL) != 0,
        (nativeEvent.key.keysym.mod & KMOD_SHIFT) != 0,
        (nativeEvent.key.keysym.mod & KMOD_ALT) != 0,
        (nativeEvent.key.keysym.mod & KMOD_GUI) != 0 };

      KeyReleaseEvent event{ SDL_GetWindowFromID(nativeEvent.key.windowID),
        SDLKeyCodeToJEngine3DKeyCode(nativeEvent.key.keysym.sym),
        modifiers,
        nativeEvent.key.repeat };
      EventProcessor().OnEvent(event);
    }

    if (nativeEvent.type == SDL_EventType::SDL_TEXTINPUT) {
      TextInputEvent event{ SDL_GetWindowFromID(nativeEvent.text.windowID),
        static_cast<char *>(nativeEvent.text.text) };
      EventProcessor().OnEvent(event);
    }

    if (nativeEvent.type == SDL_EventType::SDL_MOUSEBUTTONDOWN) {
      MousePressEvent event{ SDL_GetWindowFromID(nativeEvent.button.windowID),
        { nativeEvent.button.x, nativeEvent.button.y },
        static_cast<MouseButton>(nativeEvent.button.button),
        nativeEvent.button.clicks };
      EventProcessor().OnEvent(event);
    }

    if (nativeEvent.type == SDL_EventType::SDL_MOUSEBUTTONUP) {
      MouseReleaseEvent event{ SDL_GetWindowFromID(nativeEvent.button.windowID),
        { nativeEvent.button.x, nativeEvent.button.y },
        static_cast<MouseButton>(nativeEvent.button.button),
        nativeEvent.button.clicks };
      EventProcessor().OnEvent(event);
    }
    if (nativeEvent.type == SDL_EventType::SDL_MOUSEMOTION) {
      MouseMoveEvent event{ SDL_GetWindowFromID(nativeEvent.motion.windowID),
        { nativeEvent.motion.x, nativeEvent.motion.y },
        { nativeEvent.motion.xrel, nativeEvent.motion.yrel } };
      EventProcessor().OnEvent(event);
    }
    if (nativeEvent.type == SDL_EventType::SDL_MOUSEWHEEL) {
      MouseWheelEvent event{ SDL_GetWindowFromID(nativeEvent.wheel.windowID),
        nativeEvent.wheel.direction == SDL_MOUSEWHEEL_NORMAL ? nativeEvent.wheel.y : nativeEvent.wheel.y * -1 };
      EventProcessor().OnEvent(event);
    }
  }
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void SDLPlatformBackend::PushEvent(IEvent &event)
{
  if (event.Type() == EventType::Quit) {
    SDL_Event quitEvent;
    quitEvent.type = SDL_EventType::SDL_QUIT;
    SDL_PushEvent(&quitEvent);
  }

  if (event.Type() == EventType::WindowResize) {
    const auto &resizeEvent =
      static_cast<const WindowResizeEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto *window = static_cast<SDL_Window *>(resizeEvent.NativeWindowHandle());

    SDL_Event nativeResizeEvent;
    nativeResizeEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeResizeEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED;
    nativeResizeEvent.window.data1 = resizeEvent.Size().Width;
    nativeResizeEvent.window.data2 = resizeEvent.Size().Height;
    nativeResizeEvent.window.windowID = SDL_GetWindowID(window);
    ASSERT(nativeResizeEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeResizeEvent);

    SDL_SetWindowSize(window, resizeEvent.Size().Width, resizeEvent.Size().Height);
  }

  if (event.Type() == EventType::WindowClose) {
    const auto &closeEvent =
      static_cast<const WindowCloseEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeCloseEvent;
    nativeCloseEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeCloseEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE;
    nativeCloseEvent.window.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(closeEvent.NativeWindowHandle()));
    ASSERT(nativeCloseEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeCloseEvent);
  }

  if (event.Type() == EventType::WindowMove) {
    const auto &moveEvent =
      static_cast<const WindowMoveEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto *window = static_cast<SDL_Window *>(moveEvent.NativeWindowHandle());

    SDL_Event nativeMoveEvent;
    nativeMoveEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeMoveEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_MOVED;
    nativeMoveEvent.window.data1 = moveEvent.Position().X;
    nativeMoveEvent.window.data2 = moveEvent.Position().Y;
    nativeMoveEvent.window.windowID = SDL_GetWindowID(window);
    ASSERT(nativeMoveEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeMoveEvent);

    SDL_SetWindowPosition(window, moveEvent.Position().X, moveEvent.Position().Y);
  }

  if (event.Type() == EventType::WindowHide) {
    const auto &hideEvent =
      static_cast<const WindowHideEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto *window = static_cast<SDL_Window *>(hideEvent.NativeWindowHandle());

    SDL_Event nativeHideEvent;
    nativeHideEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeHideEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_HIDDEN;
    nativeHideEvent.window.windowID = SDL_GetWindowID(window);
    ASSERT(nativeHideEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeHideEvent);

    SDL_HideWindow(window);
  }

  if (event.Type() == EventType::WindowShow) {
    const auto &showEvent =
      static_cast<const WindowShowEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto *window = static_cast<SDL_Window *>(showEvent.NativeWindowHandle());

    SDL_Event nativeShowEvent;
    nativeShowEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeShowEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_SHOWN;
    nativeShowEvent.window.windowID = SDL_GetWindowID(window);
    ASSERT(nativeShowEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeShowEvent);

    SDL_ShowWindow(window);
  }

  if (event.Type() == EventType::WindowFocusGained) {
    const auto &focusEvent =
      static_cast<const WindowFocusGainedEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto *window = static_cast<SDL_Window *>(focusEvent.NativeWindowHandle());

    SDL_Event nativeFocusEvent;
    nativeFocusEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeFocusEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_FOCUS_GAINED;
    nativeFocusEvent.window.windowID = SDL_GetWindowID(window);
    ASSERT(nativeFocusEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeFocusEvent);

    SDL_ShowWindow(window);
    SDL_RaiseWindow(window);
    SDL_SetWindowInputFocus(window);
  }

  if (event.Type() == EventType::WindowFocusLost) {
    const auto &focusEvent =
      static_cast<const WindowFocusLostEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeFocusEvent;
    nativeFocusEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeFocusEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_FOCUS_LOST;
    nativeFocusEvent.window.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(focusEvent.NativeWindowHandle()));
    ASSERT(nativeFocusEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeFocusEvent);
  }

  if (event.Type() == EventType::WindowMinimized) {
    const auto &minimizeEvent =
      static_cast<const WindowMinimizedEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto *window = static_cast<SDL_Window *>(minimizeEvent.NativeWindowHandle());

    SDL_Event nativeMinimizeEvent;
    nativeMinimizeEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeMinimizeEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_MINIMIZED;
    nativeMinimizeEvent.window.windowID = SDL_GetWindowID(window);
    ASSERT(nativeMinimizeEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeMinimizeEvent);

    SDL_MinimizeWindow(window);
  }

  if (event.Type() == EventType::WindowMaximized) {
    const auto &maximizeEvent =
      static_cast<const WindowMaximizedEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto *window = static_cast<SDL_Window *>(maximizeEvent.NativeWindowHandle());

    SDL_Event nativeMaximizeEvent;
    nativeMaximizeEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeMaximizeEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_MAXIMIZED;
    nativeMaximizeEvent.window.windowID = SDL_GetWindowID(window);
    ASSERT(nativeMaximizeEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeMaximizeEvent);

    SDL_RestoreWindow(window);
  }

  if (event.Type() == EventType::KeyPress) {
    const auto &keyPressEvent =
      static_cast<const KeyPressEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    uint16_t sdlModifiers = 0;
    sdlModifiers |= keyPressEvent.Modifiers().Ctrl ? static_cast<uint16_t>(KMOD_CTRL) : static_cast<uint16_t>(0);
    sdlModifiers |= keyPressEvent.Modifiers().Shift ? static_cast<uint16_t>(KMOD_SHIFT) : static_cast<uint16_t>(0);
    sdlModifiers |= keyPressEvent.Modifiers().Alt ? static_cast<uint16_t>(KMOD_ALT) : static_cast<uint16_t>(0);
    sdlModifiers |= keyPressEvent.Modifiers().Super ? static_cast<uint16_t>(KMOD_GUI) : static_cast<uint16_t>(0);

    SDL_Event nativeKeyPressEvent;
    nativeKeyPressEvent.type = SDL_EventType::SDL_KEYDOWN;
    nativeKeyPressEvent.key.keysym.sym = static_cast<SDL_Keycode>(keyPressEvent.Key());
    nativeKeyPressEvent.key.keysym.mod = sdlModifiers;
    nativeKeyPressEvent.key.repeat = static_cast<uint8_t>(keyPressEvent.Repeat());
    nativeKeyPressEvent.key.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(keyPressEvent.WindowHandle()));
    SDL_PushEvent(&nativeKeyPressEvent);
  }

  if (event.Type() == EventType::KeyRelease) {
    const auto &keyReleaseEvent =
      static_cast<const KeyReleaseEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)


    uint16_t sdlModifiers = 0;
    sdlModifiers |= keyReleaseEvent.Modifiers().Ctrl ? static_cast<uint16_t>(KMOD_CTRL) : static_cast<uint16_t>(0);
    sdlModifiers |= keyReleaseEvent.Modifiers().Shift ? static_cast<uint16_t>(KMOD_SHIFT) : static_cast<uint16_t>(0);
    sdlModifiers |= keyReleaseEvent.Modifiers().Alt ? static_cast<uint16_t>(KMOD_ALT) : static_cast<uint16_t>(0);
    sdlModifiers |= keyReleaseEvent.Modifiers().Super ? static_cast<uint16_t>(KMOD_GUI) : static_cast<uint16_t>(0);

    SDL_Event nativeKeyReleaseEvent;
    nativeKeyReleaseEvent.type = SDL_EventType::SDL_KEYUP;
    nativeKeyReleaseEvent.key.keysym.sym = static_cast<SDL_Keycode>(keyReleaseEvent.Key());
    nativeKeyReleaseEvent.key.keysym.mod = sdlModifiers;
    nativeKeyReleaseEvent.key.repeat = static_cast<uint8_t>(keyReleaseEvent.Repeat());
    nativeKeyReleaseEvent.key.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(keyReleaseEvent.WindowHandle()));
    SDL_PushEvent(&nativeKeyReleaseEvent);
  }

  if (event.Type() == EventType::TextInput) {
    const auto &textInputEvent =
      static_cast<const TextInputEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeTextInputEvent;
    nativeTextInputEvent.type = SDL_EventType::SDL_TEXTINPUT;
    ASSERT(textInputEvent.Text().length() < sizeof(nativeTextInputEvent.text.text), "Text string too long");
    textInputEvent.Text().copy(
      static_cast<char *>(nativeTextInputEvent.text.text), sizeof(nativeTextInputEvent.text.text));
    nativeTextInputEvent.text.text[textInputEvent.Text().length()] = '\0';// NOLINT
    nativeTextInputEvent.text.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(textInputEvent.WindowHandle()));
    SDL_PushEvent(&nativeTextInputEvent);
  }

  if (event.Type() == EventType::MousePress) {
    const auto &mousePressEvent =
      static_cast<const MousePressEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeMousePressEvent;
    nativeMousePressEvent.type = SDL_EventType::SDL_MOUSEBUTTONDOWN;
    nativeMousePressEvent.button.button = static_cast<uint8_t>(mousePressEvent.Button());
    nativeMousePressEvent.button.clicks = static_cast<uint8_t>(mousePressEvent.Clicks());
    nativeMousePressEvent.button.x = mousePressEvent.Position().X;
    nativeMousePressEvent.button.y = mousePressEvent.Position().Y;
    nativeMousePressEvent.button.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(mousePressEvent.WindowHandle()));
    SDL_PushEvent(&nativeMousePressEvent);
  }

  if (event.Type() == EventType::MouseRelease) {
    const auto &mouseReleaseEvent =
      static_cast<const MouseReleaseEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeMouseReleaseEvent;
    nativeMouseReleaseEvent.type = SDL_EventType::SDL_MOUSEBUTTONUP;
    nativeMouseReleaseEvent.button.button = static_cast<uint8_t>(mouseReleaseEvent.Button());
    nativeMouseReleaseEvent.button.clicks = static_cast<uint8_t>(mouseReleaseEvent.Clicks());
    nativeMouseReleaseEvent.button.x = mouseReleaseEvent.Position().X;
    nativeMouseReleaseEvent.button.y = mouseReleaseEvent.Position().Y;
    nativeMouseReleaseEvent.button.windowID =
      SDL_GetWindowID(static_cast<SDL_Window *>(mouseReleaseEvent.WindowHandle()));
    SDL_PushEvent(&nativeMouseReleaseEvent);
  }

  if (event.Type() == EventType::MouseMove) {
    const auto &mouseMoveEvent =
      static_cast<const MouseMoveEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto *window = static_cast<SDL_Window *>(mouseMoveEvent.WindowHandle());

    SDL_Event nativeMouseMoveEvent;
    nativeMouseMoveEvent.type = SDL_EventType::SDL_MOUSEMOTION;
    nativeMouseMoveEvent.motion.x = mouseMoveEvent.Position().X;
    nativeMouseMoveEvent.motion.y = mouseMoveEvent.Position().Y;
    nativeMouseMoveEvent.motion.xrel = mouseMoveEvent.RelativePosition().X;
    nativeMouseMoveEvent.motion.yrel = mouseMoveEvent.RelativePosition().Y;
    nativeMouseMoveEvent.motion.windowID = SDL_GetWindowID(window);
    SDL_PushEvent(&nativeMouseMoveEvent);

    SDL_WarpMouseInWindow(window, mouseMoveEvent.Position().X, mouseMoveEvent.Position().Y);
  }

  if (event.Type() == EventType::MouseWheel) {
    const auto &mouseWheelEvent =
      static_cast<const MouseWheelEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeMouseWheelEvent;
    nativeMouseWheelEvent.type = SDL_EventType::SDL_MOUSEWHEEL;
    nativeMouseWheelEvent.wheel.x = 0;
    nativeMouseWheelEvent.wheel.y = mouseWheelEvent.ScrollAmount();
    nativeMouseWheelEvent.wheel.preciseX = 0;
    nativeMouseWheelEvent.wheel.preciseY = static_cast<float>(mouseWheelEvent.ScrollAmount());
    nativeMouseWheelEvent.wheel.direction = SDL_MOUSEWHEEL_NORMAL;
    nativeMouseWheelEvent.wheel.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(mouseWheelEvent.WindowHandle()));
    SDL_PushEvent(&nativeMouseWheelEvent);
  }
}

auto SDLPlatformBackend::CurrentTicks() -> uint64_t { return SDL_GetPerformanceCounter(); }

auto SDLPlatformBackend::TickFrequency() -> uint64_t { return SDL_GetPerformanceFrequency(); }

void SDLPlatformBackend::SetClipboardText(const std::string_view &text)
{
  SDL_SetClipboardText(std::string(text).c_str());
}

auto SDLPlatformBackend::ClipboardText() -> const char *
{
  if (m_ClipboardText != nullptr) {
    SDL_free(m_ClipboardText);// NOLINT(cppcoreguidelines-no-malloc,hicpp-no-malloc,cppcoreguidelines-owning-memory)
  }
  m_ClipboardText = SDL_GetClipboardText();
  return m_ClipboardText;
}

void SDLPlatformBackend::Delay(uint32_t milliseconds) { SDL_Delay(milliseconds); }

}// namespace JE