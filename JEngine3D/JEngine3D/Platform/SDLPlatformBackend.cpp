#include "SDLPlatformBackend.hpp"

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Events.hpp"
#include "JEngine3D/Core/LoggerController.hpp"// for Logger

#include <SDL_mouse.h>
#include <exception>// for exception
#include <cstring>// IWYU pragma: keep
#include <SDL_events.h>// for SDL_PollEvent, SDL_Event, SDL_...
#include <SDL_error.h>// for SDL_GetError
#include <SDL_video.h>// for SDL_Window, SDL_CreateWindow, SDL_DestroyWindow
#include <SDL_keyboard.h>// for SDL_Keysym
#include <SDL_keycode.h>// for SDL_Keycode
#include <SDL.h>

namespace JE {

static auto SDLKeyCodeToJEngine3DKeyCode(SDL_Keycode keyCode) -> KeyCode { return static_cast<KeyCode>(keyCode); }

static auto s_Initialized = false;// NOLINT

SDLPlatformBackend::~SDLPlatformBackend() { SDL_Quit(); }

auto SDLPlatformBackend::Initialize() -> bool
{
  ASSERT(!s_Initialized, "Backend already initialized");

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    JE::Logger::CoreLogger().error("SDL Failed to initialize - {}", SDL_GetError());
    return false;
  }

  s_Initialized = true;
  return true;
}

auto SDLPlatformBackend::Initialized() -> bool { return s_Initialized; }

auto SDLPlatformBackend::CreateWindow(const std::string_view &title, const Size2DI &size) -> NativeWindowHandle
{
  ASSERT(size.Width > 0 && size.Height > 0, "Window size cannot be zero or negative");
  return SDL_CreateWindow(title.data(),
    SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
    SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
    size.Width,
    size.Height,
    SDL_WINDOW_OPENGL);
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
  SDL_SetWindowTitle(static_cast<SDL_Window *>(handle), title.data());
}

void SDLPlatformBackend::PollEvents(IEventProcessor &processor)
{
  auto ProcessWindowResizeEvent = [&](SDL_Event &nativeEvent) {
    WindowResizeEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID),
      { nativeEvent.window.data1, nativeEvent.window.data2 } };
    processor.OnEvent(event);
  };

  auto ProcessWindowCloseEvent = [&](SDL_Event &nativeEvent) {
    WindowCloseEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID) };
    processor.OnEvent(event);
  };

  SDL_Event nativeEvent;
  while (SDL_PollEvent(&nativeEvent) != 0) {
    if (nativeEvent.type == SDL_EventType::SDL_QUIT) {
      QuitEvent event;
      processor.OnEvent(event);
    }

    if (nativeEvent.type == SDL_EventType::SDL_WINDOWEVENT) {
      switch (nativeEvent.window.event) {
      case SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED:
        ProcessWindowResizeEvent(nativeEvent);
        break;

      case SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE:
        ProcessWindowCloseEvent(nativeEvent);
        break;

      default:
        // Nothing to do for default case
        break;
      }
    }

    if (nativeEvent.type == SDL_EventType::SDL_KEYDOWN) {
      KeyPressEvent event{ SDL_GetWindowFromID(nativeEvent.key.windowID),
        SDLKeyCodeToJEngine3DKeyCode(nativeEvent.key.keysym.sym),
        nativeEvent.key.repeat };
      processor.OnEvent(event);
    }

    if (nativeEvent.type == SDL_EventType::SDL_KEYUP) {
      KeyReleaseEvent event{ SDL_GetWindowFromID(nativeEvent.key.windowID),
        SDLKeyCodeToJEngine3DKeyCode(nativeEvent.key.keysym.sym),
        nativeEvent.key.repeat };
      processor.OnEvent(event);
    }

    if (nativeEvent.type == SDL_EventType::SDL_MOUSEBUTTONDOWN) {
      MousePressEvent event{ SDL_GetWindowFromID(nativeEvent.button.windowID),
        { nativeEvent.button.x, nativeEvent.button.y },
        static_cast<MouseButton>(nativeEvent.button.button),
        nativeEvent.button.clicks };
      processor.OnEvent(event);
    }

    if (nativeEvent.type == SDL_EventType::SDL_MOUSEBUTTONUP) {
      MouseReleaseEvent event{ SDL_GetWindowFromID(nativeEvent.button.windowID),
        { nativeEvent.button.x, nativeEvent.button.y },
        static_cast<MouseButton>(nativeEvent.button.button),
        nativeEvent.button.clicks };
      processor.OnEvent(event);
    }
    if (nativeEvent.type == SDL_EventType::SDL_MOUSEMOTION) {
      MouseMoveEvent event{ SDL_GetWindowFromID(nativeEvent.motion.windowID),
        { nativeEvent.motion.x, nativeEvent.motion.y },
        { nativeEvent.motion.xrel, nativeEvent.motion.yrel } };
      processor.OnEvent(event);
    }
    if (nativeEvent.type == SDL_EventType::SDL_MOUSEWHEEL) {
      MouseWheelEvent event{ SDL_GetWindowFromID(nativeEvent.wheel.windowID),
        nativeEvent.wheel.direction == SDL_MOUSEWHEEL_NORMAL ? nativeEvent.wheel.y : nativeEvent.wheel.y * -1 };
      processor.OnEvent(event);
    }
  }
}


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

    SDL_Event nativeResizeEvent;
    nativeResizeEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeResizeEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED;
    nativeResizeEvent.window.data1 = resizeEvent.Size().Width;
    nativeResizeEvent.window.data2 = resizeEvent.Size().Height;
    nativeResizeEvent.window.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(resizeEvent.WindowHandle()));
    ASSERT(nativeResizeEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeResizeEvent);
  }

  if (event.Type() == EventType::WindowClose) {
    const auto &closeEvent =
      static_cast<const WindowCloseEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeCloseEvent;
    nativeCloseEvent.type = SDL_EventType::SDL_WINDOWEVENT;
    nativeCloseEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE;
    nativeCloseEvent.window.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(closeEvent.WindowHandle()));
    ASSERT(nativeCloseEvent.window.windowID != 0, "Invalid native window handle passed");
    SDL_PushEvent(&nativeCloseEvent);
  }

  if (event.Type() == EventType::KeyPress) {
    const auto &keyPressEvent =
      static_cast<const KeyPressEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeKeyPressEvent;
    nativeKeyPressEvent.type = SDL_EventType::SDL_KEYDOWN;
    nativeKeyPressEvent.key.keysym.sym = static_cast<SDL_Keycode>(keyPressEvent.Key());
    nativeKeyPressEvent.key.repeat = static_cast<uint8_t>(keyPressEvent.Repeat());
    nativeKeyPressEvent.key.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(keyPressEvent.WindowHandle()));
    SDL_PushEvent(&nativeKeyPressEvent);
  }

  if (event.Type() == EventType::KeyRelease) {
    const auto &keyReleaseEvent =
      static_cast<const KeyReleaseEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeKeyReleaseEvent;
    nativeKeyReleaseEvent.type = SDL_EventType::SDL_KEYUP;
    nativeKeyReleaseEvent.key.keysym.sym = static_cast<SDL_Keycode>(keyReleaseEvent.Key());
    nativeKeyReleaseEvent.key.repeat = static_cast<uint8_t>(keyReleaseEvent.Repeat());
    nativeKeyReleaseEvent.key.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(keyReleaseEvent.WindowHandle()));
    SDL_PushEvent(&nativeKeyReleaseEvent);
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

    SDL_Event nativeMousePressEvent;
    nativeMousePressEvent.type = SDL_EventType::SDL_MOUSEBUTTONUP;
    nativeMousePressEvent.button.button = static_cast<uint8_t>(mouseReleaseEvent.Button());
    nativeMousePressEvent.button.clicks = static_cast<uint8_t>(mouseReleaseEvent.Clicks());
    nativeMousePressEvent.button.x = mouseReleaseEvent.Position().X;
    nativeMousePressEvent.button.y = mouseReleaseEvent.Position().Y;
    nativeMousePressEvent.button.windowID =
      SDL_GetWindowID(static_cast<SDL_Window *>(mouseReleaseEvent.WindowHandle()));
    SDL_PushEvent(&nativeMousePressEvent);
  }

  if (event.Type() == EventType::MouseMove) {
    const auto &mouseMoveEvent =
      static_cast<const MouseMoveEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeMousePressEvent;
    nativeMousePressEvent.type = SDL_EventType::SDL_MOUSEMOTION;
    nativeMousePressEvent.motion.x = mouseMoveEvent.Position().X;
    nativeMousePressEvent.motion.y = mouseMoveEvent.Position().Y;
    nativeMousePressEvent.motion.xrel = mouseMoveEvent.RelativePosition().X;
    nativeMousePressEvent.motion.yrel = mouseMoveEvent.RelativePosition().Y;
    nativeMousePressEvent.motion.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(mouseMoveEvent.WindowHandle()));
    SDL_PushEvent(&nativeMousePressEvent);
  }

  if (event.Type() == EventType::MouseWheel) {
    const auto &mouseWheelEvent =
      static_cast<const MouseWheelEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    SDL_Event nativeMousePressEvent;
    nativeMousePressEvent.type = SDL_EventType::SDL_MOUSEWHEEL;
    nativeMousePressEvent.wheel.x = 0;
    nativeMousePressEvent.wheel.y = mouseWheelEvent.ScrollAmount();
    nativeMousePressEvent.wheel.preciseX = 0;
    nativeMousePressEvent.wheel.preciseY = static_cast<float>(mouseWheelEvent.ScrollAmount());
    nativeMousePressEvent.wheel.direction = SDL_MOUSEWHEEL_NORMAL;
    nativeMousePressEvent.wheel.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(mouseWheelEvent.WindowHandle()));
    SDL_PushEvent(&nativeMousePressEvent);
  }
}

}// namespace JE