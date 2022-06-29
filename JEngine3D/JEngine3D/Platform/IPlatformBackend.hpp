#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/Types.hpp"

namespace JE {

class IEventProcessor;
class IEvent;


struct WindowConfiguration
{
  bool Hidden = false;
  bool Minimized = false;
  bool Decorated = true;
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IPlatformBackend
{
public:
  using NativeWindowHandle = void *;
  static constexpr auto WINDOW_CENTER_POSITION = Position2DI{ 100000, 100000 };

  IPlatformBackend()
  {
    ASSERT(!s_PlatformBackendInstance, "Platform backend instance already created");
    s_PlatformBackendInstance = this;
  }

  [[nodiscard]] static inline auto Get() -> IPlatformBackend &
  {
    ASSERT(s_PlatformBackendInstance, "Platform backend instance is null");
    return *s_PlatformBackendInstance;
  }

  inline void SetEventProcessor(IEventProcessor *processor)
  {
    ASSERT(!m_EventProcessor, "Event processor already set");
    m_EventProcessor = processor;
  }
  [[nodiscard]] inline auto EventProcessor() -> IEventProcessor &
  {
    ASSERT(m_EventProcessor, "Event processor is null");
    return *m_EventProcessor;
  }

  virtual ~IPlatformBackend() { s_PlatformBackendInstance = nullptr; };

  [[nodiscard]] virtual auto Initialized() -> bool = 0;

  [[nodiscard]] virtual auto CreateWindow(const std::string_view &title,
    const Size2DI &size,
    const Position2DI &position = WINDOW_CENTER_POSITION,
    const WindowConfiguration &config = WindowConfiguration()) -> NativeWindowHandle = 0;
  virtual void DestroyWindow(NativeWindowHandle handle) = 0;
  [[nodiscard]] virtual auto ValidWindowHandle(NativeWindowHandle handle) -> bool = 0;

  [[nodiscard]] virtual auto WindowSize(NativeWindowHandle handle) -> Size2DI = 0;
  virtual void SetWindowSize(NativeWindowHandle handle, const Size2DI &size) = 0;

  [[nodiscard]] virtual auto WindowTitle(NativeWindowHandle handle) -> std::string_view = 0;
  virtual void SetWindowTitle(NativeWindowHandle handle, const std::string_view &title) = 0;

  [[nodiscard]] virtual auto WindowPosition(NativeWindowHandle handle) -> Position2DI = 0;
  virtual void SetWindowPosition(NativeWindowHandle handle, const Position2DI &position) = 0;

  [[nodiscard]] virtual auto WindowHidden(NativeWindowHandle handle) -> bool = 0;
  virtual void ShowWindow(NativeWindowHandle handle) = 0;
  virtual void HideWindow(NativeWindowHandle handle) = 0;

  [[nodiscard]] virtual auto WindowFocused(NativeWindowHandle handle) -> bool = 0;
  virtual void FocusWindow(NativeWindowHandle handle) = 0;
  [[nodiscard]] virtual auto FocusedWindow() -> NativeWindowHandle = 0;

  [[nodiscard]] virtual auto WindowMinimized(NativeWindowHandle handle) -> bool = 0;
  virtual void MinimizeWindow(NativeWindowHandle handle) = 0;
  virtual void MaximizeWindow(NativeWindowHandle handle) = 0;

  [[nodiscard]] virtual auto MonitorCount() -> int32_t = 0;
  [[nodiscard]] virtual auto DisplayBounds(int32_t displayIndex) -> RectangleI = 0;
  [[nodiscard]] virtual auto DisplayUsableBounds(int32_t displayIndex) -> RectangleI = 0;
  [[nodiscard]] virtual auto DisplayDPI(int32_t displayIndex) -> float = 0;

  virtual void CaptureMouse() = 0;
  virtual void ReleaseMouse() = 0;

  virtual void PollEvents() = 0;
  virtual void PushEvent(IEvent &event) = 0;

  [[nodiscard]] virtual auto CurrentTicks() -> uint64_t = 0;
  [[nodiscard]] virtual auto TickFrequency() -> uint64_t = 0;

  virtual void SetClipboardText(const std::string_view &text) = 0;
  [[nodiscard]] virtual auto ClipboardText() -> const char * = 0;

  virtual void Delay(uint32_t milliseconds) = 0;

private:
  IEventProcessor *m_EventProcessor = nullptr;

  static IPlatformBackend *s_PlatformBackendInstance;// NOLINT
};

}// namespace JE