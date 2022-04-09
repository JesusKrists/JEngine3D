#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/LoggerController.hpp>// for LoggerController
#include <JEngine3D/Core/MemoryController.hpp>// for MemoryController
#include <JEngine3D/Platform/IPlatformBackend.hpp>// for IPlatformBackend
#include <JEngine3D/Platform/SDLPlatformBackend.hpp>// for SDLPlatformBackend
#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/Types.hpp>// for Size2DI
#include <JEngine3D/Core/Events.hpp>

template<typename BackendImpl> class IPlatformBackendTestsFixture
{
public:
  static constexpr auto TEST_WINDOW_SIZE = JE::Size2DI{ 800, 600 };
  static constexpr auto TEST_WINDOW_TITLE = std::string_view{ "Test Window" };

  static constexpr auto NEW_WINDOW_SIZE = JE::Size2DI{ 640, 480 };
  static constexpr auto NEW_WINDOW_TITLE = std::string_view{ "Test Window - New" };

  IPlatformBackendTestsFixture() { JE::UNUSED(m_Backend.Initialize()); }

protected:
  JE::MemoryController m_MemoryController;
  JE::LoggerController m_LoggerController;
  BackendImpl m_Backend;
};

using Backend = JE::SDLPlatformBackend;

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates singleton instance which can be accessed",
  "[JE::IPlatformBackend]")
{
  REQUIRE(&JE::IPlatformBackend::Get() == &m_Backend);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an SDLWindow and returns a valid WindowHandle",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  REQUIRE(m_Backend.ValidWindowHandle(windowHandle));

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an SDLWindow and can query the size",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  REQUIRE(m_Backend.WindowSize(windowHandle) == TEST_WINDOW_SIZE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an SDLWindow and can set window size",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  m_Backend.SetWindowSize(windowHandle, NEW_WINDOW_SIZE);

  REQUIRE(m_Backend.WindowSize(windowHandle) == NEW_WINDOW_SIZE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an SDLWindow and can query window title",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  REQUIRE(m_Backend.WindowTitle(windowHandle) == TEST_WINDOW_TITLE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an SDLWindow and can set window title",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  m_Backend.SetWindowTitle(windowHandle, NEW_WINDOW_TITLE);

  REQUIRE(m_Backend.WindowTitle(windowHandle) == NEW_WINDOW_TITLE);

  m_Backend.DestroyWindow(windowHandle);
}


TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Quit)",
  "[JE::IPlatformBackend]")
{

  class QuitChecker final : public JE::IEventProcessor
  {
  public:
    inline void OnEvent(JE::IEvent &event) override { m_QuitEventReceived = (event.Type() == JE::EventType::Quit); }
    [[nodiscard]] inline auto QuitEventReceived() const -> bool { return m_QuitEventReceived; }

  private:
    bool m_QuitEventReceived = false;
  } checker;

  JE::QuitEvent event;
  m_Backend.PushEvent(event);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.QuitEventReceived());
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Window Size changed)",
  "[JE::IPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class WindowResizeChecker final : public JE::IEventProcessor
  {
  public:
    explicit WindowResizeChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::WindowResize) {
        const auto &resizeEvent =
          static_cast<const JE::WindowResizeEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (resizeEvent.WindowHandle() == m_Handle && resizeEvent.Size() == NEW_WINDOW_SIZE) {
          m_WindowResizeEventReceived = true;
        }
      }
    }

    [[nodiscard]] inline auto WindowResizeEventReceived() const -> bool { return m_WindowResizeEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_WindowResizeEventReceived = false;
  } checker{ handle };


  JE::WindowResizeEvent event{ handle, NEW_WINDOW_SIZE };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.WindowResizeEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Window Close)",
  "[JE::IPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class WindowCloseChecker final : public JE::IEventProcessor
  {
  public:
    explicit WindowCloseChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::WindowClose) {
        const auto &resizeEvent =
          static_cast<const JE::WindowCloseEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (resizeEvent.WindowHandle() == m_Handle) { m_WindowCloseEventReceived = true; }
      }
    }

    [[nodiscard]] inline auto WindowCloseEventReceived() const -> bool { return m_WindowCloseEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_WindowCloseEventReceived = false;
  } checker{ handle };


  JE::WindowCloseEvent event{ handle };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.WindowCloseEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Key press)",
  "[JE::IPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class KeyPressChecker final : public JE::IEventProcessor
  {
  public:
    explicit KeyPressChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::KeyPress) {
        const auto &keyPressEvent =
          static_cast<const JE::KeyPressEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (keyPressEvent.WindowHandle() == m_Handle && keyPressEvent.Key() == JE::KeyCode::Mode
            && keyPressEvent.Repeat() == 1) {
          m_KeyPressEventReceived = true;
        }
      }
    }

    [[nodiscard]] inline auto KeyPressEventReceived() const -> bool { return m_KeyPressEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_KeyPressEventReceived = false;
  } checker{ handle };

  JE::KeyPressEvent event{ handle, JE::KeyCode::Mode, 1 };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.KeyPressEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Key release)",
  "[JE::IPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class KeyReleaseChecker final : public JE::IEventProcessor
  {
  public:
    explicit KeyReleaseChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::KeyRelease) {
        const auto &keyReleaseEvent =
          static_cast<const JE::KeyReleaseEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (keyReleaseEvent.WindowHandle() == m_Handle && keyReleaseEvent.Key() == JE::KeyCode::KeyPadXOR
            && keyReleaseEvent.Repeat() == 0) {
          m_KeyReleaseEventReceived = true;
        }
      }
    }

    [[nodiscard]] inline auto KeyReleaseEventReceived() const -> bool { return m_KeyReleaseEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_KeyReleaseEventReceived = false;
  } checker{ handle };

  JE::KeyReleaseEvent event{ handle, JE::KeyCode::KeyPadXOR, 0 };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.KeyReleaseEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Mouse press)",
  "[JE::IPlatformBackend]")
{

  static constexpr auto CLICK_POSITION = JE::Position2DI{ 640, 480 };

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class MousePressChecker final : public JE::IEventProcessor
  {
  public:
    explicit MousePressChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::MousePress) {
        const auto &mousePressEvent =
          static_cast<const JE::MousePressEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (mousePressEvent.WindowHandle() == m_Handle && mousePressEvent.Button() == JE::MouseButton::Middle
            && mousePressEvent.Clicks() == 1 && mousePressEvent.Position() == CLICK_POSITION) {
          m_MousePressEventReceived = true;
        }
      }
    }

    [[nodiscard]] inline auto MousePressEventReceived() const -> bool { return m_MousePressEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_MousePressEventReceived = false;
  } checker{ handle };

  JE::MousePressEvent event{ handle, CLICK_POSITION, JE::MouseButton::Middle, 1 };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.MousePressEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Mouse release)",
  "[JE::IPlatformBackend]")
{

  static constexpr auto CLICK_POSITION = JE::Position2DI{ 640, 480 };

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class MouseReleaseChecker final : public JE::IEventProcessor
  {
  public:
    explicit MouseReleaseChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::MouseRelease) {
        const auto &mouseReleaseEvent =
          static_cast<const JE::MouseReleaseEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (mouseReleaseEvent.WindowHandle() == m_Handle && mouseReleaseEvent.Button() == JE::MouseButton::Left
            && mouseReleaseEvent.Clicks() == 1 && mouseReleaseEvent.Position() == CLICK_POSITION) {
          m_MouseReleaseEventReceived = true;
        }
      }
    }

    [[nodiscard]] inline auto MouseReleaseEventReceived() const -> bool { return m_MouseReleaseEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_MouseReleaseEventReceived = false;
  } checker{ handle };

  JE::MouseReleaseEvent event{ handle, CLICK_POSITION, JE::MouseButton::Left, 1 };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.MouseReleaseEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Mouse move)",
  "[JE::IPlatformBackend]")
{

  static constexpr auto INITIAL_POSITION = JE::Position2DI{ 320, 240 };
  static constexpr auto MOVE_POSITION = JE::Position2DI{ 640, 480 };

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class MouseMoveChecker final : public JE::IEventProcessor
  {
  public:
    explicit MouseMoveChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::MouseMove) {
        const auto &mouseMoveEvent =
          static_cast<const JE::MouseMoveEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (mouseMoveEvent.WindowHandle() == m_Handle && mouseMoveEvent.Position() == MOVE_POSITION
            && mouseMoveEvent.RelativePosition()
                 == JE::Position2DI{ MOVE_POSITION.X - INITIAL_POSITION.X, MOVE_POSITION.Y - INITIAL_POSITION.Y }) {
          m_MouseMoveEventReceived = true;
        }
      }
    }

    [[nodiscard]] inline auto MouseMoveEventReceived() const -> bool { return m_MouseMoveEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_MouseMoveEventReceived = false;
  } checker{ handle };

  JE::MouseMoveEvent event{
    handle, MOVE_POSITION, JE::Position2DI{ MOVE_POSITION.X - INITIAL_POSITION.X, MOVE_POSITION.Y - INITIAL_POSITION.Y }
  };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.MouseMoveEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Mouse wheel)",
  "[JE::IPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class MouseWheelChecker final : public JE::IEventProcessor
  {
  public:
    explicit MouseWheelChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::MouseWheel) {
        const auto &mouseWheelEvent =
          static_cast<const JE::MouseWheelEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (mouseWheelEvent.WindowHandle() == m_Handle && mouseWheelEvent.ScrollAmount() == 1) {
          m_MouseWheelEventReceived = true;
        }
      }
    }

    [[nodiscard]] inline auto MouseWheelEventReceived() const -> bool { return m_MouseWheelEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_MouseWheelEventReceived = false;
  } checker{ handle };

  JE::MouseWheelEvent event{ handle, 1 };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.MouseWheelEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Returns current ticks (not 0) and tick frequency (not 0)",
  "[JE::IPlatformBackend]")
{
  REQUIRE(m_Backend.CurrentTicks() != 0);
  REQUIRE(m_Backend.TickFrequency() != 0);
}