#pragma once

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Platform/IPlatformBackend.hpp"
#include "JEngine3D/Core/Events.hpp"

namespace JE {

class IGraphicsContext;

class Window
{
  friend class WindowController;

public:
  Window(const Window &other) = delete;
  auto operator=(const Window &other) -> Window & = delete;
  Window(Window &&other) = delete;
  auto operator=(Window &&other) -> Window & = delete;

  Window(const std::string_view &title,
    const Size2DI &size,
    const Position2DI &position,
    const WindowConfiguration &config,
    IPlatformBackend::NativeWindowHandle nativeHandle);
  ~Window();

  [[nodiscard]] inline auto NativeHandle() const -> IPlatformBackend::NativeWindowHandle { return m_NativeHandle; }

  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto GraphicsContext() -> IGraphicsContext & { return *m_GraphicsContext; }

  [[nodiscard]] inline auto Title() const -> const std::string & { return m_Title; }
  void SetTitle(const std::string_view &title);

  [[nodiscard]] inline auto Size() const -> const Size2DI & { return m_Size; }
  void SetSize(const Size2DI &size);

  [[nodiscard]] inline auto Position() const -> const Position2DI & { return m_Position; }
  void SetPosition(const Position2DI &position);

  [[nodiscard]] inline auto Shown() const -> bool { return m_Shown; }
  void Show();
  void Hide();

  [[nodiscard]] inline auto Focused() const -> bool { return m_Focused; }
  void Focus();

  [[nodiscard]] inline auto Minimized() const -> bool { return m_Minimized; }
  void Minimize();
  void Maximize();

private:
  IPlatformBackend::NativeWindowHandle m_NativeHandle;
  Scope<IGraphicsContext, MemoryTag::App> m_GraphicsContext;

  std::string m_Title;
  Size2DI m_Size;
  Position2DI m_Position;
  bool m_Shown;
  bool m_Focused = true;
  bool m_Minimized;
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class WindowController final : public IEventProcessor
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

  auto CreateWindow(const std::string_view &title,
    const Size2DI &size,
    const Position2DI &position = IPlatformBackend::WINDOW_CENTER_POSITION,
    const WindowConfiguration &config = WindowConfiguration{}) -> Window &;
  void DestroyWindow(Window &window);
  [[nodiscard]] inline auto Windows() const -> const WindowContainer & { return m_Windows; }
  [[nodiscard]] auto WindowFromNativeHandle(IPlatformBackend::NativeWindowHandle handle) -> Window &;

  inline void DestroyAllWindows() { m_Windows.clear(); }

private:
  void DestroyWindowFromNativeHandle(IPlatformBackend::NativeWindowHandle handle);

  WindowContainer m_Windows;

  static WindowController *s_WindowControllerInstance;// NOLINT
};

}// namespace JE