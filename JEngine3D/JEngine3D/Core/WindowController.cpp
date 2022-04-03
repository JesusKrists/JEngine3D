#include "WindowController.hpp"

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Base.hpp"// for FindIf

#include <iterator>// for end

namespace JE {


Window::Window(const std::string_view &title, const Size2D &size, IPlatformBackend::NativeWindowHandle nativeHandle)
  : m_NativeHandle(nativeHandle), m_Title(title), m_Size(size)
{
  ASSERT(
    m_Title == IPlatformBackend::Get().WindowTitle(m_NativeHandle), "Window title mismatch with native window title");
  ASSERT(m_Size == IPlatformBackend::Get().WindowSize(m_NativeHandle), "Window size mismatch with native window size");
}

Window::~Window() { IPlatformBackend::Get().DestroyWindow(m_NativeHandle); }

void Window::SetTitle(const std::string_view &title)
{
  IPlatformBackend::Get().SetWindowTitle(m_NativeHandle, title);
  m_Title = title;
}

void Window::SetSize(const Size2D &size)
{
  IPlatformBackend::Get().SetWindowSize(m_NativeHandle, size);
  m_Size = size;
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
  if (event.Category() != EventCategory::Window) { return; }
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

  dispatcher.Dispatch<EventType::WindowClose>([&](const IEvent &evnt) {
    const auto &closeEvent =
      static_cast<const WindowCloseEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    DestroyWindowFromNativeHandle(closeEvent.WindowHandle());

    return true;
  });
}

auto WindowController::CreateWindow(const std::string_view &title, const Size2D &size) -> Window &
{
  m_Windows.push_back(
    CreateScope<Window, MemoryTag::App>(title, size, IPlatformBackend::Get().CreateWindow(title, size)));
  return *m_Windows.back();
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