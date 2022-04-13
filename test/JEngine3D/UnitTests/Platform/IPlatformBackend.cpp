#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/LoggerController.hpp>// for LoggerController
#include <JEngine3D/Core/MemoryController.hpp>// for MemoryController
#include <JEngine3D/Platform/IPlatformBackend.hpp>// for IPlatformBackend
#include <JEngine3D/Platform/SDLPlatformBackend.hpp>// for SDLPlatformBackend
#include <JEngine3D/Core/Types.hpp>// for Size2DI
#include <JEngine3D/Core/Events.hpp>

template<typename BackendImpl> class IPlatformBackendTestsFixture
{
public:
  static constexpr auto TEST_WINDOW_SIZE = JE::Size2DI{ 800, 600 };
  static constexpr auto TEST_WINDOW_TITLE = std::string_view{ "Test Window" };
  static constexpr auto TEST_WINDOW_POSITION = JE::Position2DI{ 100, 100 };

  static constexpr auto NEW_WINDOW_SIZE = JE::Size2DI{ 640, 480 };
  static constexpr auto NEW_WINDOW_TITLE = std::string_view{ "Test Window - New" };
  static constexpr auto NEW_WINDOW_POSITION = JE::Position2DI{ 150, 150 };

  static constexpr auto CLIPBOARD_TEXT = std::string_view{ "Clipboard TEXT test!" };
  static constexpr auto TEXT_INPUT_TEXT = std::string_view{ "Text input text!" };

  static constexpr auto DELAY_COUNT_MS = 1100;

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
  "JE::IPlatformBackend creates an Window and returns a valid WindowHandle",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  REQUIRE(m_Backend.ValidWindowHandle(windowHandle));
  REQUIRE(m_Backend.WindowPosition(windowHandle) != JE::IPlatformBackend::WINDOW_CENTER_POSITION);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an Window and can query the size",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  REQUIRE(m_Backend.WindowSize(windowHandle) == TEST_WINDOW_SIZE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an Window and can set window size",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  m_Backend.SetWindowSize(windowHandle, NEW_WINDOW_SIZE);

  REQUIRE(m_Backend.WindowSize(windowHandle) == NEW_WINDOW_SIZE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an Window and can query window title",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  REQUIRE(m_Backend.WindowTitle(windowHandle) == TEST_WINDOW_TITLE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an Window and can set window title",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  m_Backend.SetWindowTitle(windowHandle, NEW_WINDOW_TITLE);

  REQUIRE(m_Backend.WindowTitle(windowHandle) == NEW_WINDOW_TITLE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an Window and query window position",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION);

  REQUIRE(m_Backend.WindowPosition(windowHandle) == TEST_WINDOW_POSITION);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an Window and can set window position",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION);

  m_Backend.SetWindowPosition(windowHandle, NEW_WINDOW_POSITION);

  CHECK_NOFAIL(m_Backend.WindowPosition(windowHandle) == NEW_WINDOW_POSITION);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an Window and can show window",
  "[JE::IPlatformBackend]")
{
  JE::WindowConfiguration config{};
  config.Hidden = true;
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION, config);
  REQUIRE(m_Backend.WindowHidden(windowHandle));

  m_Backend.ShowWindow(windowHandle);

  REQUIRE(!m_Backend.WindowHidden(windowHandle));

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend creates an Window and can hide window",
  "[JE::IPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION);
  REQUIRE(!m_Backend.WindowHidden(windowHandle));

  m_Backend.HideWindow(windowHandle);

  REQUIRE(m_Backend.WindowHidden(windowHandle));

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
  m_Backend.SetEventProcessor(&checker);

  JE::QuitEvent event;
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

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
        if (resizeEvent.NativeWindowHandle() == m_Handle && resizeEvent.Size() == NEW_WINDOW_SIZE) {
          m_WindowResizeEventReceived = true;
        }
      }
    }

    [[nodiscard]] inline auto WindowResizeEventReceived() const -> bool { return m_WindowResizeEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_WindowResizeEventReceived = false;
  } checker{ handle };
  m_Backend.SetEventProcessor(&checker);


  JE::WindowResizeEvent event{ handle, NEW_WINDOW_SIZE };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

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
        if (resizeEvent.NativeWindowHandle() == m_Handle) { m_WindowCloseEventReceived = true; }
      }
    }

    [[nodiscard]] inline auto WindowCloseEventReceived() const -> bool { return m_WindowCloseEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_WindowCloseEventReceived = false;
  } checker{ handle };
  m_Backend.SetEventProcessor(&checker);


  JE::WindowCloseEvent event{ handle };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

  REQUIRE(checker.WindowCloseEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Window Move)",
  "[JE::IPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class WindowMoveChecker final : public JE::IEventProcessor
  {
  public:
    explicit WindowMoveChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::WindowMove) {
        const auto &moveEvent =
          static_cast<const JE::WindowMoveEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (moveEvent.NativeWindowHandle() == m_Handle && moveEvent.Position() == NEW_WINDOW_POSITION) {
          m_WindowMoveEventReceived = true;
        }
      }
    }

    [[nodiscard]] inline auto WindowMoveEventReceived() const -> bool { return m_WindowMoveEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_WindowMoveEventReceived = false;
  } checker{ handle };
  m_Backend.SetEventProcessor(&checker);

  m_Backend.PollEvents();

  JE::WindowMoveEvent event{ handle, NEW_WINDOW_POSITION };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

  REQUIRE(checker.WindowMoveEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Window Hide)",
  "[JE::IPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class WindowHideChecker final : public JE::IEventProcessor
  {
  public:
    explicit WindowHideChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::WindowHide) {
        const auto &hideEvent =
          static_cast<const JE::WindowHideEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (hideEvent.NativeWindowHandle() == m_Handle) { m_WindowHideEventReceived = true; }
      }
    }

    [[nodiscard]] inline auto WindowHideEventReceived() const -> bool { return m_WindowHideEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_WindowHideEventReceived = false;
  } checker{ handle };
  m_Backend.SetEventProcessor(&checker);


  JE::WindowHideEvent event{ handle };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

  REQUIRE(checker.WindowHideEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Window Show)",
  "[JE::IPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class WindowShowChecker final : public JE::IEventProcessor
  {
  public:
    explicit WindowShowChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::WindowShow) {
        const auto &showEvent =
          static_cast<const JE::WindowShowEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (showEvent.NativeWindowHandle() == m_Handle) { m_WindowShowEventReceived = true; }
      }
    }

    [[nodiscard]] inline auto WindowShowEventReceived() const -> bool { return m_WindowShowEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_WindowShowEventReceived = false;
  } checker{ handle };
  m_Backend.SetEventProcessor(&checker);


  JE::WindowShowEvent event{ handle };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

  REQUIRE(checker.WindowShowEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Window Focus Gained)",
  "[JE::IPlatformBackend]")
{
  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class WindowFocusGainedChecker final : public JE::IEventProcessor
  {
  public:
    explicit WindowFocusGainedChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::WindowFocusGained) {
        const auto &focusEvent =
          static_cast<const JE::WindowFocusGainedEvent &>(// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            event);
        if (focusEvent.NativeWindowHandle() == m_Handle) { m_WindowFocusGainedEventReceived = true; }
      }
    }

    [[nodiscard]] inline auto WindowFocusGainedEventReceived() const -> bool
    {
      return m_WindowFocusGainedEventReceived;
    }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_WindowFocusGainedEventReceived = false;
  } checker{ handle };
  m_Backend.SetEventProcessor(&checker);


  JE::WindowFocusGainedEvent event{ handle };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

  REQUIRE(checker.WindowFocusGainedEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Window Focus Lost)",
  "[JE::IPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class WindowFocusLostChecker final : public JE::IEventProcessor
  {
  public:
    explicit WindowFocusLostChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::WindowFocusLost) {
        const auto &focusEvent =
          static_cast<const JE::WindowFocusLostEvent &>(// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            event);
        if (focusEvent.NativeWindowHandle() == m_Handle) { m_WindowFocusLostEventReceived = true; }
      }
    }

    [[nodiscard]] inline auto WindowFocusLostEventReceived() const -> bool { return m_WindowFocusLostEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_WindowFocusLostEventReceived = false;
  } checker{ handle };
  m_Backend.SetEventProcessor(&checker);


  JE::WindowFocusLostEvent event{ handle };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

  REQUIRE(checker.WindowFocusLostEventReceived());

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
          if (keyPressEvent.Modifiers().Ctrl && keyPressEvent.Modifiers().Shift && keyPressEvent.Modifiers().Alt
              && !keyPressEvent.Modifiers().Super) {
            m_KeyPressEventReceived = true;
          }
        }
      }
    }

    [[nodiscard]] inline auto KeyPressEventReceived() const -> bool { return m_KeyPressEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_KeyPressEventReceived = false;
  } checker{ handle };
  m_Backend.SetEventProcessor(&checker);

  JE::KeyModifiers modifiers = { true, true, true, false };
  JE::KeyPressEvent event{ handle, JE::KeyCode::Mode, modifiers, 1 };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

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
          if (keyReleaseEvent.Modifiers().Ctrl && !keyReleaseEvent.Modifiers().Shift && keyReleaseEvent.Modifiers().Alt
              && keyReleaseEvent.Modifiers().Super) {
            m_KeyReleaseEventReceived = true;
          }
        }
      }
    }

    [[nodiscard]] inline auto KeyReleaseEventReceived() const -> bool { return m_KeyReleaseEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_KeyReleaseEventReceived = false;
  } checker{ handle };
  m_Backend.SetEventProcessor(&checker);

  JE::KeyModifiers modifiers = { true, false, true, true };
  JE::KeyReleaseEvent event{ handle, JE::KeyCode::KeyPadXOR, modifiers, 0 };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

  REQUIRE(checker.KeyReleaseEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend Polls events to EventProcessor (Fake Text input)",
  "[JE::IPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class TextInputChecker final : public JE::IEventProcessor
  {
  public:
    explicit TextInputChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

    inline void OnEvent(JE::IEvent &event) override
    {
      if (event.Type() == JE::EventType::TextInput) {
        const auto &textInputEvent =
          static_cast<const JE::TextInputEvent &>(event);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        if (textInputEvent.WindowHandle() == m_Handle && textInputEvent.Text() == TEXT_INPUT_TEXT) {
          m_TextInputEventReceived = true;
        }
      }
    }

    [[nodiscard]] inline auto TextInputEventReceived() const -> bool { return m_TextInputEventReceived; }

  private:
    JE::IPlatformBackend::NativeWindowHandle m_Handle;
    bool m_TextInputEventReceived = false;
  } checker{ handle };
  m_Backend.SetEventProcessor(&checker);

  JE::TextInputEvent event{ handle, TEXT_INPUT_TEXT };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

  REQUIRE(checker.TextInputEventReceived());

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
  m_Backend.SetEventProcessor(&checker);

  JE::MousePressEvent event{ handle, CLICK_POSITION, JE::MouseButton::Middle, 1 };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

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
  m_Backend.SetEventProcessor(&checker);

  JE::MouseReleaseEvent event{ handle, CLICK_POSITION, JE::MouseButton::Left, 1 };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

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
  m_Backend.SetEventProcessor(&checker);

  JE::MouseMoveEvent event{
    handle, MOVE_POSITION, JE::Position2DI{ MOVE_POSITION.X - INITIAL_POSITION.X, MOVE_POSITION.Y - INITIAL_POSITION.Y }
  };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

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
  m_Backend.SetEventProcessor(&checker);

  JE::MouseWheelEvent event{ handle, 1 };
  m_Backend.PushEvent(event);

  m_Backend.PollEvents();

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

TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend return clipboard text",
  "[JE::IPlatformBackend]")
{
  m_Backend.SetClipboardText(CLIPBOARD_TEXT);
  REQUIRE(m_Backend.ClipboardText() == CLIPBOARD_TEXT);
}


TEST_CASE_METHOD(IPlatformBackendTestsFixture<Backend>,
  "JE::IPlatformBackend blocks execution by calling delay",
  "[JE::IPlatformBackend]")
{
  auto startTicks = m_Backend.CurrentTicks();
  m_Backend.Delay(DELAY_COUNT_MS);
  auto endTicks = m_Backend.CurrentTicks();

  REQUIRE((endTicks - startTicks) >= m_Backend.TickFrequency());
}