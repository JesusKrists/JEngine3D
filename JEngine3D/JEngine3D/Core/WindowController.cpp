#include "WindowController.hpp"

#include "JEngine3D/Platform/IGraphicsContext.hpp"
#include "JEngine3D/Platform/IGraphicsContextCreator.hpp"

#include <iterator>// for end

namespace JE {


    Window::Window(const std::string_view&       title,
                   const Size2DI&                size,
                   const Position2DI&            position,
                   const WindowConfiguration&    config,
                   IPlatform::NativeWindowHandle nativeHandle)
        : m_NativeHandle(nativeHandle), m_GraphicsContext(IGraphicsContextCreator::Get().CreateContext(m_NativeHandle)), m_Title(title),
          m_Size(size), m_Position(position), m_Shown(!config.Hidden), m_Minimized(config.Minimized)
    {
        ASSERT(m_Title == IPlatform::Get().WindowTitle(m_NativeHandle), "Window title mismatch with native window title");

        // TODO(JesusKrists) - Make no debugbreak assert to get info about this stuff instead of breaking the tests
        // ASSERT(m_Size == IPlatformBackend::Get().WindowSize(m_NativeHandle), "Window size mismatch with native window
        // size");
        m_Size = IPlatform::Get().WindowSize(m_NativeHandle);

        if (position != IPlatform::WINDOW_CENTER_POSITION) {
            ASSERT(m_Position == IPlatform::Get().WindowPosition(m_NativeHandle), "Window position mismatch with native window position");
        } else {
            m_Position = IPlatform::Get().WindowPosition(m_NativeHandle);
        }
        ASSERT(m_Shown == !IPlatform::Get().WindowHidden(m_NativeHandle), "Window mismatch with native window hidden");
        m_Focused = IPlatform::Get().WindowFocused(m_NativeHandle);
        ASSERT(m_Minimized == IPlatform::Get().WindowMinimized(m_NativeHandle), "Window mismatch with native window minimize");
    }

    Window::~Window() { IPlatform::Get().DestroyWindow(m_NativeHandle); }

    void Window::SetTitle(const std::string_view& title)
    {
        IPlatform::Get().SetWindowTitle(m_NativeHandle, title);
        m_Title = title;
    }

    void Window::SetSize(const Size2DI& size)
    {
        IPlatform::Get().SetWindowSize(m_NativeHandle, size);
        m_Size = size;
    }

    void Window::SetPosition(const Position2DI& position)
    {
        IPlatform::Get().SetWindowPosition(m_NativeHandle, position);
        m_Position = position;
    }

    void Window::Show()
    {
        IPlatform::Get().ShowWindow(m_NativeHandle);
        m_Shown = true;
    }

    void Window::Hide()
    {
        IPlatform::Get().HideWindow(m_NativeHandle);
        m_Shown = false;
    }

    void Window::Focus()
    {
        Show();
        IPlatform::Get().FocusWindow(m_NativeHandle);
        m_Focused = true;
    }

    void Window::Minimize()
    {
        IPlatform::Get().MinimizeWindow(m_NativeHandle);
        m_Minimized = true;
    }

    void Window::Maximize()
    {
        IPlatform::Get().MaximizeWindow(m_NativeHandle);
        m_Minimized = false;
    }

    //////////////////////////////////////////////////////////////////////////////////////

    WindowController* WindowController::s_WindowControllerInstance = nullptr;// NOLINT

    WindowController::WindowController()
    {
        ASSERT(!s_WindowControllerInstance, "WindowController instance already exists");
        ASSERT(IPlatform::Get().Initialized(), "Backend needs to be initialized before using this class");
        s_WindowControllerInstance = this;
    }

    WindowController::~WindowController() { s_WindowControllerInstance = nullptr; }

    void WindowController::OnEvent(IEvent& event)
    {
        if (event.Category() != EventCategory::Window) {
            Logger::CoreLogger().error("WindowController received unknown event type");
            return;
        }
        EventDispatcher dispatcher{ event };

        dispatcher.Dispatch<EventType::WindowResize>([&](const IEvent& evnt) {
            const auto& resizeEvent = static_cast<const WindowResizeEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            auto& window  = WindowFromNativeHandle(resizeEvent.NativeWindowHandle());
            window.m_Size = resizeEvent.Size();

            ASSERT(window.Size() == IPlatform::Get().WindowSize(window.NativeHandle()), "Window size and native window size mismatch");

            window.GraphicsContext().Resize(resizeEvent.Size());

            return true;
        });

        dispatcher.Dispatch<EventType::WindowClose>([&](const IEvent& evnt) {
            const auto& closeEvent = static_cast<const WindowCloseEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            DestroyWindowFromNativeHandle(closeEvent.NativeWindowHandle());

            return true;
        });

        dispatcher.Dispatch<EventType::WindowMove>([&](const IEvent& evnt) {
            const auto& moveEvent = static_cast<const WindowMoveEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            auto& window      = WindowFromNativeHandle(moveEvent.NativeWindowHandle());
            window.m_Position = moveEvent.Position();

            // TODO(JesusKrists): Make a no debugbreak assert since this one is very annoying and no fix is possible really
            // ASSERT(window.Position() == IPlatformBackend::Get().WindowPosition(window.NativeHandle()),
            //   "Window position and native window position mismatch");

            return true;
        });

        dispatcher.Dispatch<EventType::WindowHide>([&](const IEvent& evnt) {
            const auto& hideEvent = static_cast<const WindowHideEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            auto& window   = WindowFromNativeHandle(hideEvent.NativeWindowHandle());
            window.m_Shown = false;

            // TODO(JesusKrists): Make no debugbreak assert
            // ASSERT(IPlatformBackend::Get().WindowHidden(window.NativeHandle()), "Window and Native window hidden mismatch");

            return true;
        });

        dispatcher.Dispatch<EventType::WindowShow>([&](const IEvent& evnt) {
            const auto& showEvent = static_cast<const WindowShowEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            auto& window   = WindowFromNativeHandle(showEvent.NativeWindowHandle());
            window.m_Shown = true;

            ASSERT(!IPlatform::Get().WindowHidden(window.NativeHandle()), "Window and Native window hidden mismatch");

            return true;
        });

        dispatcher.Dispatch<EventType::WindowFocusGained>([&](const IEvent& evnt) {
            const auto& focusEvent =
            static_cast<const WindowFocusGainedEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            auto& window     = WindowFromNativeHandle(focusEvent.NativeWindowHandle());
            window.m_Focused = true;

            // ASSERT(IPlatformBackend::Get().WindowFocused(window.NativeHandle()), "Window and Native window focus mismatch");

            return true;
        });

        dispatcher.Dispatch<EventType::WindowFocusLost>([&](const IEvent& evnt) {
            const auto& focusEvent =
            static_cast<const WindowFocusLostEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            auto& window     = WindowFromNativeHandle(focusEvent.NativeWindowHandle());
            window.m_Focused = false;

            ASSERT(!IPlatform::Get().WindowFocused(window.NativeHandle()), "Window and Native window focus mismatch");

            return true;
        });

        dispatcher.Dispatch<EventType::WindowMinimized>([&](const IEvent& evnt) {
            const auto& minimizeEvent =
            static_cast<const WindowMinimizedEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            auto& window       = WindowFromNativeHandle(minimizeEvent.NativeWindowHandle());
            window.m_Minimized = true;

            // ASSERT(
            //   IPlatformBackend::Get().WindowMinimized(window.NativeHandle()), "Window and Native window minimized mismatch");

            return true;
        });

        dispatcher.Dispatch<EventType::WindowMaximized>([&](const IEvent& evnt) {
            const auto& maximizeEvent =
            static_cast<const WindowMaximizedEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            auto& window       = WindowFromNativeHandle(maximizeEvent.NativeWindowHandle());
            window.m_Minimized = false;

            // ASSERT(
            //   !IPlatformBackend::Get().WindowMinimized(window.NativeHandle()), "Window and Native window minimized
            //   mismatch");

            return true;
        });


        dispatcher.Dispatch<EventType::WindowRestored>([&](const IEvent& evnt) {
            const auto& restoreEvent =
            static_cast<const WindowRestoredEvent&>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

            auto& window       = WindowFromNativeHandle(restoreEvent.NativeWindowHandle());
            window.m_Minimized = false;

            // ASSERT(
            //   !IPlatformBackend::Get().WindowMinimized(window.NativeHandle()), "Window and Native window minimized
            //   mismatch");

            return true;
        });
    }

    auto WindowController::CreateWindow(const std::string_view&    title,
                                        const Size2DI&             size,
                                        const Position2DI&         position,
                                        const WindowConfiguration& config) -> Window&
    {
        m_Windows.push_back(
        CreateScope<Window, MemoryTag::App>(title, size, position, config, IPlatform::Get().CreateWindow(title, size, position, config)));
        return *m_Windows.back();
    }

    void WindowController::DestroyWindow(Window& window)
    {
        auto windowIt = FindIf(m_Windows, [&](const Scope<Window, MemoryTag::App>& windowEntry) { return windowEntry.get() == &window; });
        ASSERT(windowIt != std::end(m_Windows), "Window not found");

        m_Windows.erase(windowIt);
    }

    auto WindowController::WindowFromNativeHandle(IPlatform::NativeWindowHandle handle) -> Window&
    {
        auto windowIt = FindIf(m_Windows, [&](const Scope<Window, MemoryTag::App>& window) { return window->NativeHandle() == handle; });
        ASSERT(windowIt != std::end(m_Windows), "Window not found from native handle");

        return *(*windowIt);
    }

    void WindowController::DestroyWindowFromNativeHandle(IPlatform::NativeWindowHandle handle)
    {
        auto windowIt = FindIf(m_Windows, [&](const Scope<Window, MemoryTag::App>& window) { return window->NativeHandle() == handle; });
        ASSERT(windowIt != std::end(m_Windows), "Window not found from native handle");

        m_Windows.erase(windowIt);
    }

}// namespace JE