#pragma once

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Platform/IPlatformBackend.hpp"
#include "JEngine3D/Core/Events.hpp"

namespace JE {


class Window
{
  friend class WindowController;

public:
  Window(const Window &other) = delete;
  auto operator=(const Window &other) -> Window & = delete;
  Window(Window &&other) = delete;
  auto operator=(Window &&other) -> Window & = delete;

  Window(const std::string_view &title, const Size2D &size, IPlatformBackend::NativeWindowHandle nativeHandle);
  ~Window();

  [[nodiscard]] inline auto NativeHandle() const -> IPlatformBackend::NativeWindowHandle { return m_NativeHandle; }

  [[nodiscard]] inline auto Title() const -> const std::string & { return m_Title; }
  void SetTitle(const std::string_view &title);

  [[nodiscard]] inline auto Size() const -> const Size2D & { return m_Size; }
  void SetSize(const Size2D &size);

private:
  IPlatformBackend::NativeWindowHandle m_NativeHandle;
  std::string m_Title;
  Size2D m_Size;
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class WindowController : public IEventProcessor
{
  using WindowContainer = Vector<Scope<Window, MemoryTag::App>, MemoryTag::App>;

public:
  WindowController();
  ~WindowController() override;

  [[nodiscard]] static inline auto Get() -> WindowController &
  {
    ASSERT(s_WindowControllerInstance, "WindowController instance is null");
    return *s_WindowControllerInstance;
  }

  void OnEvent(IEvent &event) override;

  auto CreateWindow(const std::string_view &title, const Size2D &size) -> Window &;
  [[nodiscard]] inline auto Windows() const -> const WindowContainer & { return m_Windows; }

  inline void DestroyAllWindows() { m_Windows.clear(); }

private:
  auto WindowFromNativeHandle(const IPlatformBackend::NativeWindowHandle handle) -> Window &;
  void DestroyWindowFromNativeHandle(const IPlatformBackend::NativeWindowHandle handle);

  WindowContainer m_Windows;

  static WindowController *s_WindowControllerInstance;// NOLINT
};

}// namespace JE