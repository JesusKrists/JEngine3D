#include "WindowController.hpp"

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Base.hpp"// for FindIf
#include "JEngine3D/Core/LoggerController.hpp"// for Logger

#include <iterator>// for end

namespace JE {


Window::Window(const std::string_view &title,
  const Size2DI &size,
  const Position2DI &position,
  const WindowConfiguration &config,
  IPlatformBackend::NativeWindowHandle nativeHandle)
  : m_NativeHandle(nativeHandle), m_Title(title), m_Size(size), m_Position(position), m_Shown(!config.Hidden)
{
  ASSERT(
    m_Title == IPlatformBackend::Get().WindowTitle(m_NativeHandle), "Window title mismatch with native window title");
  ASSERT(m_Size == IPlatformBackend::Get().WindowSize(m_NativeHandle), "Window size mismatch with native window size");
  if (position != IPlatformBackend::WINDOW_CENTER_POSITION) {
    ASSERT(m_Position == IPlatformBackend::Get().WindowPosition(m_NativeHandle),
      "Window position mismatch with native window position");
  } else {
    m_Position = IPlatformBackend::Get().WindowPosition(m_NativeHandle);
  }
  ASSERT(m_Shown == !IPlatformBackend::Get().WindowHidden(m_NativeHandle), "Window mismatch with native window hidden");
}

Window::~Window() { IPlatformBackend::Get().DestroyWindow(m_NativeHandle); }

void Window::SetTitle(const std::string_view &title)
{
  IPlatformBackend::Get().SetWindowTitle(m_NativeHandle, title);
  m_Title = title;
}

void Window::SetSize(const Size2DI &size)
{
  IPlatformBackend::Get().SetWindowSize(m_NativeHandle, size);
  m_Size = size;
}

void Window::SetPosition(const Position2DI &position)
{
  IPlatformBackend::Get().SetWindowPosition(m_NativeHandle, position);
  m_Position = position;
}

void Window::Show()
{
  IPlatformBackend::Get().ShowWindow(m_NativeHandle);
  m_Shown = true;
}

void Window::Hide()
{
  IPlatformBackend::Get().HideWindow(m_NativeHandle);
  m_Shown = false;
}

//////////////////////////////////////////////////////////////////////////////////////

WindowController *WindowController::s_WindowControllerInstance = nullptr;// NOLINT

WindowController::WindowController()
{
  ASSERT(!s_WindowControllerInstance, "WindowController instance already exists");
  ASSERT(IPlatformBackend::Get().Initialized(), "Backend needs to be initialized before using this class");
  s_WindowControllerInstance = this;
}

WindowController::~WindowController() { s_WindowControllerInstance = nullptr; }

void WindowController::OnEvent(IEvent &event)
{
  if (event.Category() != EventCategory::Window) {
    Logger::CoreLogger().error("WindowController received unknown event type");
    return;
  }
  EventDispatcher dispatcher{ event };

  dispatcher.Dispatch<EventType::WindowResize>([&](const IEvent &evnt) {
    const auto &resizeEvent =
      static_cast<const WindowResizeEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto &window = WindowFromNativeHandle(resizeEvent.WindowHandle());
    window.m_Size = resizeEvent.Size();

    ASSERT(window.Size() == IPlatformBackend::Get().WindowSize(window.NativeHandle()),
      "Window size and native window size mismatch");

    return true;
  });

  dispatcher.Dispatch<EventType::WindowMove>([&](const IEvent &evnt) {
    const auto &moveEvent =
      static_cast<const WindowMoveEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto &window = WindowFromNativeHandle(moveEvent.WindowHandle());
    window.m_Position = moveEvent.Position();

    ASSERT(window.Position() == IPlatformBackend::Get().WindowPosition(window.NativeHandle()),
      "Window position and native window position mismatch");

    return true;
  });

  dispatcher.Dispatch<EventType::WindowHide>([&](const IEvent &evnt) {
    const auto &hideEvent =
      static_cast<const WindowHideEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto &window = WindowFromNativeHandle(hideEvent.WindowHandle());
    window.m_Shown = false;

    ASSERT(IPlatformBackend::Get().WindowHidden(window.NativeHandle()), "Window and Native window hidden mismatch");

    return true;
  });

  dispatcher.Dispatch<EventType::WindowShow>([&](const IEvent &evnt) {
    const auto &showEvent =
      static_cast<const WindowShowEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto &window = WindowFromNativeHandle(showEvent.WindowHandle());
    window.m_Shown = true;

    ASSERT(!IPlatformBackend::Get().WindowHidden(window.NativeHandle()), "Window and Native window hidden mismatch");

    return true;
  });

  dispatcher.Dispatch<EventType::WindowClose>([&](const IEvent &evnt) {
    const auto &closeEvent =
      static_cast<const WindowCloseEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    DestroyWindowFromNativeHandle(closeEvent.WindowHandle());

    return true;
  });
}

auto WindowController::CreateWindow(const std::string_view &title,
  const Size2DI &size,
  const Position2DI &position,
  const WindowConfiguration &config) -> Window &
{
  m_Windows.push_back(CreateScope<Window, MemoryTag::App>(
    title, size, position, config, IPlatformBackend::Get().CreateWindow(title, size, position, config)));
  return *m_Windows.back();
}

void WindowController::DestroyWindow(Window &window)
{
  auto windowIt =
    FindIf(m_Windows, [&](const Scope<Window, MemoryTag::App> &windowEntry) { return windowEntry.get() == &window; });
  ASSERT(windowIt != std::end(m_Windows), "Window not found");

  m_Windows.erase(windowIt);
}

auto WindowController::WindowFromNativeHandle(const IPlatformBackend::NativeWindowHandle handle) -> Window &
{
  auto windowIt =
    FindIf(m_Windows, [&](const Scope<Window, MemoryTag::App> &window) { return window->NativeHandle() == handle; });
  ASSERT(windowIt != std::end(m_Windows), "Window not found from native handle");

  return *(*windowIt);
}

void WindowController::DestroyWindowFromNativeHandle(const IPlatformBackend::NativeWindowHandle handle)
{
  auto windowIt =
    FindIf(m_Windows, [&](const Scope<Window, MemoryTag::App> &window) { return window->NativeHandle() == handle; });
  ASSERT(windowIt != std::end(m_Windows), "Window not found from native handle");

  m_Windows.erase(windowIt);
}

}// namespace JE