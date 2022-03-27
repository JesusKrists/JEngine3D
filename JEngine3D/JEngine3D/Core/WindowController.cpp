#include "WindowController.hpp"

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT

#include <memory>// for unique_ptr

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

WindowController::WindowController()
{
  ASSERT(IPlatformBackend::Get().Initialized(), "Backend needs to be initialized before using this class");
}

auto WindowController::CreateWindow(const std::string_view &title, const Size2D &size) -> Window &
{
  m_Windows.push_back(
    CreateScope<Window, MemoryTag::App>(title, size, IPlatformBackend::Get().CreateWindow(title, size)));
  return *m_Windows.back();
}

}// namespace JE