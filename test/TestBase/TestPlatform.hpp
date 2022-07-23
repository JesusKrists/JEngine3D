#pragma once

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/Types.hpp>
#include <JEngine3D/Platform/IPlatform.hpp>// for IPlatformBackend
#include <JEngine3D/Core/Events.hpp>

class TestPlatform final : public JE::IPlatform
{
private:
    [[nodiscard]] inline auto WindowIterator(NativeWindowHandle handle) -> decltype(auto)
    {
        return JE::FindIf(m_CreatedWindows, [&](const TestWindow& window) {
            return window.ID == reinterpret_cast<decltype(window.ID)>(handle);// NOLINT
        });
    }

public:
    static constexpr auto INVALID_WINDOW_SIZE     = JE::Size2DI{ -1, -1 };
    static constexpr auto INVALID_WINDOW_TITLE    = std::string_view{ "Invalid Window" };
    static constexpr auto INVALID_WINDOW_POSITION = JE::Position2DI{ -1, -1 };

    static constexpr uint64_t TICK_INCREMENT = 1000000;
    static constexpr uint64_t TICK_FREQUENCY = 1000000;

    static constexpr auto  DISPLAY_BOUNDS        = JE::RectangleI{ { 0, 0 }, { 640, 480 } };
    static constexpr auto  DISPLAY_USABLE_BOUNDS = JE::RectangleI{ { 0, 0 }, { 600, 480 } };
    static constexpr float DISPLAY_DPI           = 100;

    [[nodiscard]] inline auto Initialized() -> bool override { return true; }

    [[nodiscard]] inline auto CreateWindow(const std::string_view&        title,
                                           const JE::Size2DI&             size,
                                           const JE::Position2DI&         position = WINDOW_CENTER_POSITION,
                                           const JE::WindowConfiguration& config = JE::WindowConfiguration{}) -> NativeWindowHandle override
    {
        const auto window = m_CreatedWindows.emplace_back(++m_CurrentWindowID, title, size, position, config);
        if (!config.Hidden) { FocusWindow(window.NativeHandle()); }
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
    inline void SetWindowSize(NativeWindowHandle handle, const JE::Size2DI& size) override
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
    inline void SetWindowTitle(NativeWindowHandle handle, const std::string_view& title) override
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

    inline void SetWindowPosition(NativeWindowHandle handle, const JE::Position2DI& position) override
    {
        auto windowIt = WindowIterator(handle);
        if (windowIt != std::end(m_CreatedWindows)) { windowIt->Position = position; }
    }

    [[nodiscard]] inline auto WindowHidden(NativeWindowHandle handle) -> bool override
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

    [[nodiscard]] inline auto WindowFocused(NativeWindowHandle handle) -> bool override
    {
        auto windowIt = WindowIterator(handle);
        if (windowIt != std::end(m_CreatedWindows)) { return windowIt->Focused; }
        return false;
    }

    inline void FocusWindow(NativeWindowHandle handle) override
    {
        auto windowIt = WindowIterator(handle);
        if (windowIt != std::end(m_CreatedWindows)) { windowIt->Focused = true; }

        JE::ForEach(m_CreatedWindows, [&](auto& window) {
            if (&window != &(*windowIt)) { window.Focused = false; }
        });
    }

    [[nodiscard]] inline auto FocusedWindow() -> NativeWindowHandle override
    {
        for (const auto& window : m_CreatedWindows) {
            if (window.Focused) { return window.NativeHandle(); }
        }

        return nullptr;
    }

    [[nodiscard]] inline auto WindowMinimized(NativeWindowHandle handle) -> bool override
    {
        auto windowIt = WindowIterator(handle);
        if (windowIt != std::end(m_CreatedWindows)) { return windowIt->Minimized; }
        return false;
    }

    inline void MinimizeWindow(NativeWindowHandle handle) override
    {
        auto windowIt = WindowIterator(handle);
        if (windowIt != std::end(m_CreatedWindows)) { windowIt->Minimized = true; }
    }

    inline void MaximizeWindow(NativeWindowHandle handle) override
    {
        auto windowIt = WindowIterator(handle);
        if (windowIt != std::end(m_CreatedWindows)) { windowIt->Minimized = false; }
    }

    [[nodiscard]] inline auto MonitorCount() -> int32_t override { return 1; }

    [[nodiscard]] inline auto DisplayBounds(int32_t displayIndex) -> JE::RectangleI override
    {
        if (displayIndex == 0) { return DISPLAY_BOUNDS; }
        return JE::RectangleI{ { 0, 0 }, { 0, 0 } };
    }

    [[nodiscard]] inline auto DisplayUsableBounds(int32_t displayIndex) -> JE::RectangleI override
    {
        if (displayIndex == 0) { return DISPLAY_USABLE_BOUNDS; }
        return JE::RectangleI{ { 0, 0 }, { 0, 0 } };
    }

    [[nodiscard]] inline auto DisplayDPI(int32_t displayIndex) -> float override
    {
        if (displayIndex == 0) { return DISPLAY_DPI; }
        return 0;
    }

    inline void CaptureMouse() override {}

    inline void ReleaseMouse() override {}

    inline void PushEvent(JE::IEvent& event) override { m_EventQueue.emplace_back(event); }

    inline void PollEvents() override
    {
        JE::ForEach(m_EventQueue, [&](auto& event) {
            ProcessEvent(event);
            EventProcessor().OnEvent(event);
        });

        m_EventQueue.clear();
    }

    inline void ProcessEvent(JE::IEvent& event)
    {
        JE::EventDispatcher dispatcher{ event };
        dispatcher.Dispatch<JE::EventType::WindowResize>([&](const JE::IEvent& evnt) {
            const auto& resizeEvent =
            static_cast<const JE::WindowResizeEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            auto windowIt = WindowIterator(resizeEvent.NativeWindowHandle());
            if (windowIt != std::end(m_CreatedWindows)) { windowIt->Size = resizeEvent.Size(); }

            return false;
        });

        dispatcher.Dispatch<JE::EventType::WindowMove>([&](const JE::IEvent& evnt) {
            const auto& moveEvent = static_cast<const JE::WindowMoveEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            auto        windowIt  = WindowIterator(moveEvent.NativeWindowHandle());
            if (windowIt != std::end(m_CreatedWindows)) { windowIt->Position = moveEvent.Position(); }

            return false;
        });

        dispatcher.Dispatch<JE::EventType::WindowHide>([&](const JE::IEvent& evnt) {
            const auto& hideEvent = static_cast<const JE::WindowHideEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            auto        windowIt  = WindowIterator(hideEvent.NativeWindowHandle());
            if (windowIt != std::end(m_CreatedWindows)) { windowIt->Shown = false; }

            return false;
        });

        dispatcher.Dispatch<JE::EventType::WindowShow>([&](const JE::IEvent& evnt) {
            const auto& showEvent = static_cast<const JE::WindowShowEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            auto        windowIt  = WindowIterator(showEvent.NativeWindowHandle());
            if (windowIt != std::end(m_CreatedWindows)) { windowIt->Shown = true; }

            return false;
        });

        dispatcher.Dispatch<JE::EventType::WindowFocusGained>([&](const JE::IEvent& evnt) {
            const auto& focusEvent =
            static_cast<const JE::WindowFocusGainedEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            FocusWindow(focusEvent.NativeWindowHandle());

            return false;
        });

        dispatcher.Dispatch<JE::EventType::WindowFocusLost>([&](const JE::IEvent& evnt) {
            const auto& focusEvent =
            static_cast<const JE::WindowFocusLostEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            auto windowIt = WindowIterator(focusEvent.NativeWindowHandle());
            if (windowIt != std::end(m_CreatedWindows)) { windowIt->Focused = false; }

            return false;
        });

        dispatcher.Dispatch<JE::EventType::WindowMinimized>([&](const JE::IEvent& evnt) {
            const auto& minimizeEvent =
            static_cast<const JE::WindowMinimizedEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            auto windowIt = WindowIterator(minimizeEvent.NativeWindowHandle());
            if (windowIt != std::end(m_CreatedWindows)) { windowIt->Minimized = true; }

            return false;
        });

        dispatcher.Dispatch<JE::EventType::WindowMaximized>([&](const JE::IEvent& evnt) {
            const auto& maximizeEvent =
            static_cast<const JE::WindowMaximizedEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            auto windowIt = WindowIterator(maximizeEvent.NativeWindowHandle());
            if (windowIt != std::end(m_CreatedWindows)) { windowIt->Minimized = false; }

            return false;
        });

        dispatcher.Dispatch<JE::EventType::WindowRestored>([&](const JE::IEvent& evnt) {
            const auto& restoredEvent =
            static_cast<const JE::WindowRestoredEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            auto windowIt = WindowIterator(restoredEvent.NativeWindowHandle());
            if (windowIt != std::end(m_CreatedWindows)) { windowIt->Minimized = false; }

            return false;
        });
    }

    inline auto CurrentTicks() -> uint64_t override
    {
        m_CurrentTicks += TICK_INCREMENT;
        return m_CurrentTicks;
    }

    [[nodiscard]] inline auto TickFrequency() -> uint64_t override { return TICK_FREQUENCY; }

    inline void SetClipboardText(const std::string_view& text) override { m_ClipboardText = text; }

    [[nodiscard]] inline auto ClipboardText() -> const char* override { return m_ClipboardText.c_str(); }

    inline void Delay(uint32_t milliseconds) override { JE::UNUSED(milliseconds); }

private:
    struct TestWindow
    {
        TestWindow(size_t                         windowID,
                   const std::string_view&        title,
                   const JE::Size2DI&             size,
                   const JE::Position2DI&         position,
                   const JE::WindowConfiguration& config)
            : ID(windowID), Title(title), Size(size), Position(position), Shown(!config.Hidden), Focused(!config.Hidden),
              Minimized(config.Minimized)
        {}

        [[nodiscard]] inline auto NativeHandle() const -> NativeWindowHandle
        {
            return reinterpret_cast<NativeWindowHandle>(ID);// NOLINT
        }

        size_t          ID;
        std::string     Title;
        JE::Size2DI     Size;
        JE::Position2DI Position;
        bool            Shown;
        bool            Focused;
        bool            Minimized;
    };

    size_t                                          m_CurrentWindowID = 0;
    std::vector<TestWindow>                         m_CreatedWindows;
    std::vector<std::reference_wrapper<JE::IEvent>> m_EventQueue;
    uint64_t                                        m_CurrentTicks = 0;

    std::string m_ClipboardText;
};