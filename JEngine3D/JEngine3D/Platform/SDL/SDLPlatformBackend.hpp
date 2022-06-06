#pragma once

#include "JEngine3D/Platform/IPlatformBackend.hpp"
#include "JEngine3D/Core/Types.hpp"// for Size2D


namespace JE {

class IEvent;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class SDLPlatformBackend final : public IPlatformBackend
{
public:
  SDLPlatformBackend();
  virtual ~SDLPlatformBackend();// NOLINT


  [[nodiscard]] auto Initialized() -> bool override;

  [[nodiscard]] auto CreateWindow(const std::string_view &title,
    const Size2DI &size,
    const Position2DI &position = WINDOW_CENTER_POSITION,
    const WindowConfiguration &config = WindowConfiguration{}) -> NativeWindowHandle override;
  void DestroyWindow(NativeWindowHandle handle) override;
  [[nodiscard]] auto ValidWindowHandle(NativeWindowHandle handle) -> bool override;


  [[nodiscard]] auto WindowSize(NativeWindowHandle handle) -> Size2DI override;
  void SetWindowSize(NativeWindowHandle handle, const Size2DI &size) override;

  [[nodiscard]] auto WindowTitle(NativeWindowHandle handle) -> std::string_view override;
  void SetWindowTitle(NativeWindowHandle handle, const std::string_view &title) override;

  [[nodiscard]] auto WindowPosition(NativeWindowHandle handle) -> Position2DI override;
  void SetWindowPosition(NativeWindowHandle handle, const Position2DI &position) override;

  [[nodiscard]] auto WindowHidden(NativeWindowHandle handle) -> bool override;
  void ShowWindow(NativeWindowHandle handle) override;
  void HideWindow(NativeWindowHandle handle) override;

  [[nodiscard]] auto WindowFocused(NativeWindowHandle handle) -> bool override;
  void FocusWindow(NativeWindowHandle handle) override;
  [[nodiscard]] auto FocusedWindow() -> NativeWindowHandle override;

  [[nodiscard]] auto WindowMinimized(NativeWindowHandle handle) -> bool override;
  void MinimizeWindow(NativeWindowHandle handle) override;
  void MaximizeWindow(NativeWindowHandle handle) override;

  [[nodiscard]] auto MonitorCount() -> int32_t override;
  [[nodiscard]] auto DisplayBounds(int32_t displayIndex) -> RectangleI override;
  [[nodiscard]] auto DisplayUsableBounds(int32_t displayIndex) -> RectangleI override;
  [[nodiscard]] auto DisplayDPI(int32_t displayIndex) -> float override;

  void CaptureMouse() override;
  void ReleaseMouse() override;

  void PollEvents() override;
  void PushEvent(IEvent &event) override;

  [[nodiscard]] auto CurrentTicks() -> uint64_t override;
  [[nodiscard]] auto TickFrequency() -> uint64_t override;

  void SetClipboardText(const std::string_view &text) override;
  [[nodiscard]] auto ClipboardText() -> const char * override;

  void Delay(uint32_t milliseconds) override;

private:
  char *m_ClipboardText = nullptr;
};

}// namespace JE