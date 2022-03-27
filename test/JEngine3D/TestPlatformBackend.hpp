#pragma once

#include <JEngine3D/Platform/IPlatformBackend.hpp>// for IPlatformBackend

class TestPlatformBackend final : public JE::IPlatformBackend
{

private:
  inline auto GetWindowIterator(NativeWindowHandle handle) -> decltype(auto)
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
    auto windowIt = GetWindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { m_CreatedWindows.erase(windowIt); }
  }

  [[nodiscard]] inline auto WindowSize(NativeWindowHandle handle) -> JE::Size2D override
  {
    auto windowIt = GetWindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { return windowIt->Size; }
    return INVALID_WINDOW_SIZE;
  }
  inline void SetWindowSize(NativeWindowHandle handle, const JE::Size2D &size) override
  {
    auto windowIt = GetWindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { windowIt->Size = size; }
  }

  [[nodiscard]] inline auto WindowTitle(NativeWindowHandle handle) -> std::string_view override
  {
    auto windowIt = GetWindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { return windowIt->Title; }
    return INVALID_WINDOW_TITLE;
  }
  inline void SetWindowTitle(NativeWindowHandle handle, const std::string_view &title) override
  {
    auto windowIt = GetWindowIterator(handle);
    if (windowIt != std::end(m_CreatedWindows)) { windowIt->Title = title; }
  }

  inline void PollEvents(JE::IEventProcessor &processor) override { JE::UNUSED(processor); }

private:
  struct TestWindow
  {
    TestWindow(size_t id, const std::string_view &title, const JE::Size2D &size) : ID(id), Title(title), Size(size) {}
    size_t ID;
    std::string Title;
    JE::Size2D Size;
  };

  size_t m_CurrentWindowID = 0;
  std::vector<TestWindow> m_CreatedWindows;
};