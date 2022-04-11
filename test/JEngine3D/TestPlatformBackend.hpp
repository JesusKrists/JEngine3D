#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Types.hpp"
#include <JEngine3D/Platform/IPlatformBackend.hpp>// for IPlatformBackend
#include <JEngine3D/Core/Events.hpp>

class TestPlatformBackend final : public JE::IPlatformBackend
{
private:
  [[nodiscard]] inline auto WindowIterator(NativeWindowHandle handle) -> decltype(auto)
  {
    return JE::FindIf(m_CreatedWindows, [&](const TestWindow &window) {
      return window.ID == reinterpret_cast<decltype(window.ID)>(handle);// NOLINT
    });
  }

public:
  static constexpr auto INVALID_WINDOW_SIZE = JE::Size2DI{ -1, -1 };
  static constexpr auto INVALID_WINDOW_TITLE = std::string_view{ "Invalid Window" };
  static constexpr auto INVALID_WINDOW_POSITION = JE::Position2DI{ -1, -1 };

  static constexpr uint64_t TICK_INCREMENT = 1000000;
  static constexpr uint64_t TICK_FREQUENCY = 1000000;

  [[nodiscard]] inline auto Initialize() -> bool override { return true; }
  [[nodiscard]] inline auto Initialized() -> bool override { return true; }

  [[nodiscard]] inline auto CreateWindow(const std::string_view &title,
    const JE::Size2DI &size,
    const JE::Position2DI &position = WINDOW_CENTER_POSITION,
    const JE::WindowConfiguration &config = JE::WindowConfiguration{}) -> NativeWindowHandle override
  {
    const auto window = m_CreatedWindows.emplace_back(++m_CurrentWindowID, title, size, position, config);
    m_WindowInFocus = &window;
    return reinterpret_cast<NativeWindowHandle>(window.ID);// NOLINT
  }
  inline void DestroyWindow(NativeWindowHandle handle) override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { m_CreatedWindows.erase(windowIt); }
  }

  [[nodiscard]] inline auto ValidWindowHandle(NativeWindowHandle handle) -> bool override
  {
    const auto windowIt = WindowIterator(handle);
    return windowIt != std::end(m_CreatedWindows);
  }

  [[nodiscard]] inline auto WindowSize(NativeWindowHandle handle) -> JE::Size2DI override
  {
    const auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { return windowIt->Size; }
    return INVALID_WINDOW_SIZE;
  }
  inline void SetWindowSize(NativeWindowHandle handle, const JE::Size2DI &size) override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { windowIt->Size = size; }
  }

  [[nodiscard]] inline auto WindowTitle(NativeWindowHandle handle) -> std::string_view override
  {
    const auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { return windowIt->Title; }
    return INVALID_WINDOW_TITLE;
  }
  inline void SetWindowTitle(NativeWindowHandle handle, const std::string_view &title) override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { windowIt->Title = title; }
  }

  [[nodiscard]] inline auto WindowPosition(NativeWindowHandle handle) -> JE::Position2DI override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { return windowIt->Position; }
    return INVALID_WINDOW_POSITION;
  }

  inline void SetWindowPosition(NativeWindowHandle handle, const JE::Position2DI &position) override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { windowIt->Position = position; }
  }

  inline auto WindowHidden(NativeWindowHandle handle) -> bool override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { return !windowIt->Shown; }
    return true;
  }

  inline void ShowWindow(NativeWindowHandle handle) override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { windowIt->Shown = true; }
  }

  inline void HideWindow(NativeWindowHandle handle) override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { windowIt->Shown = false; }
  }

  inline void PushEvent(JE::IEvent &event) override { m_EventQueue.emplace_back(event); }

  inline void PollEvents() override
  {
    for (auto it = std::begin(m_EventQueue); it != std::end(m_EventQueue);) {
      ProcessEvent(*it);

      EventProcessor().OnEvent(*it);
      it = m_EventQueue.erase(it);
    }
  }

  inline void ProcessEvent(JE::IEvent &event)
  {
    JE::EventDispatcher dispatcher{ event };
    dispatcher.Dispatch<JE::EventType::WindowResize>([&](const JE::IEvent &evnt) {
      const auto &resizeEvent =
        static_cast<const JE::WindowResizeEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
      auto windowIt = WindowIterator(resizeEvent.WindowHandle());
      if (windowIt != std::end(m_CreatedWindows)) { windowIt->Size = resizeEvent.Size(); }

      return false;
    });

    dispatcher.Dispatch<JE::EventType::WindowMove>([&](const JE::IEvent &evnt) {
      const auto &moveEvent =
        static_cast<const JE::WindowMoveEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
      auto windowIt = WindowIterator(moveEvent.WindowHandle());
      if (windowIt != std::end(m_CreatedWindows)) { windowIt->Position = moveEvent.Position(); }

      return false;
    });

    dispatcher.Dispatch<JE::EventType::WindowHide>([&](const JE::IEvent &evnt) {
      const auto &hideEvent =
        static_cast<const JE::WindowHideEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
      auto windowIt = WindowIterator(hideEvent.WindowHandle());
      if (windowIt != std::end(m_CreatedWindows)) { windowIt->Shown = false; }

      return false;
    });

    dispatcher.Dispatch<JE::EventType::WindowShow>([&](const JE::IEvent &evnt) {
      const auto &showEvent =
        static_cast<const JE::WindowShowEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
      auto windowIt = WindowIterator(showEvent.WindowHandle());
      if (windowIt != std::end(m_CreatedWindows)) { windowIt->Shown = true; }

      return false;
    });
  }

  inline auto CurrentTicks() -> uint64_t override
  {
    m_CurrentTicks += TICK_INCREMENT;
    return m_CurrentTicks;
  }

  inline auto TickFrequency() -> uint64_t override { return TICK_FREQUENCY; }

  inline void SetClipboardText(const std::string_view &text) override { m_ClipboardText = text; }

  [[nodiscard]] inline auto ClipboardText() -> std::string_view override { return m_ClipboardText; }

  inline void Delay(uint32_t milliseconds) override { JE::UNUSED(milliseconds); }

private:
  struct TestWindow
  {
    TestWindow(size_t windowID,
      const std::string_view &title,
      const JE::Size2DI &size,
      const JE::Position2DI &position,
      const JE::WindowConfiguration &config)
      : ID(windowID), Title(title), Size(size), Position(position), Shown(!config.Hidden)
    {}
    size_t ID;
    std::string Title;
    JE::Size2DI Size;
    JE::Position2DI Position;
    bool Shown;
  };

  size_t m_CurrentWindowID = 0;
  std::vector<TestWindow> m_CreatedWindows;
  std::vector<std::reference_wrapper<JE::IEvent>> m_EventQueue;
  const TestWindow *m_WindowInFocus = nullptr;
  uint64_t m_CurrentTicks = 0;

  std::string m_ClipboardText;
};