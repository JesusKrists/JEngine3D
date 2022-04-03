#pragma once

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
  static constexpr auto INVALID_WINDOW_SIZE = JE::Size2D{ -1, -1 };
  static constexpr auto INVALID_WINDOW_TITLE = std::string_view{ "Invalid Window" };

  [[nodiscard]] inline auto Initialize() -> bool override { return true; }
  [[nodiscard]] inline auto Initialized() -> bool override { return true; }

  [[nodiscard]] inline auto CreateWindow(const std::string_view &title, const JE::Size2D &size)
    -> NativeWindowHandle override
  {
    auto window = m_CreatedWindows.emplace_back(++m_CurrentWindowID, title, size);
    return reinterpret_cast<NativeWindowHandle>(window.ID);// NOLINT
  }
  inline void DestroyWindow(NativeWindowHandle handle) override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { m_CreatedWindows.erase(windowIt); }
  }

  [[nodiscard]] inline auto WindowSize(NativeWindowHandle handle) -> JE::Size2D override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { return windowIt->Size; }
    return INVALID_WINDOW_SIZE;
  }
  inline void SetWindowSize(NativeWindowHandle handle, const JE::Size2D &size) override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { windowIt->Size = size; }
  }

  [[nodiscard]] inline auto WindowTitle(NativeWindowHandle handle) -> std::string_view override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { return windowIt->Title; }
    return INVALID_WINDOW_TITLE;
  }
  inline void SetWindowTitle(NativeWindowHandle handle, const std::string_view &title) override
  {
    auto windowIt = WindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { windowIt->Title = title; }
  }

  inline void PushEvent(JE::IEvent &event) { m_EventQueue.emplace_back(event); }

  inline void PollEvents(JE::IEventProcessor &processor) override
  {
    for (auto it = std::begin(m_EventQueue); it != std::end(m_EventQueue);) {
      ProcessEvent(*it);

      processor.OnEvent(*it);
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
  }

private:
  struct TestWindow
  {
    TestWindow(size_t windowID, const std::string_view &title, const JE::Size2D &size)
      : ID(windowID), Title(title), Size(size)
    {}
    size_t ID;
    std::string Title;
    JE::Size2D Size;
  };

  size_t m_CurrentWindowID = 0;
  std::vector<TestWindow> m_CreatedWindows;
  std::vector<std::reference_wrapper<JE::IEvent>> m_EventQueue;
};