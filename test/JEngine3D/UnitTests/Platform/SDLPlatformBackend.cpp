#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/LoggerController.hpp>// for LoggerController
#include <JEngine3D/Core/MemoryController.hpp>// for MemoryController
#include <JEngine3D/Platform/IPlatformBackend.hpp>// for IPlatformBackend
#include <JEngine3D/Platform/SDLPlatformBackend.hpp>// for SDLPlatformBackend
#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/Types.hpp>// for Size2D
#include <JEngine3D/Core/Events.hpp>

#include <SDL_video.h>// for SDL_GetWindowID
#include <SDL_events.h>

class SDLPlatformBackendTestsFixture
{
public:
  static constexpr auto TEST_WINDOW_SIZE = JE::Size2D{ 1280, 720 };
  static constexpr auto TEST_WINDOW_TITLE = std::string_view{ "Test Window" };

  static constexpr auto NEW_WINDOW_SIZE = JE::Size2D{ 640, 480 };
  static constexpr auto NEW_WINDOW_TITLE = std::string_view{ "Test Window - New" };

  SDLPlatformBackendTestsFixture() { JE::UNUSED(m_Backend.Initialize()); }

protected:
  JE::MemoryController m_MemoryController;
  JE::LoggerController m_LoggerController;
  JE::SDLPlatformBackend m_Backend;
};

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::IPlatformBackend creates singleton instance which can be accessed",
  "[JE::IPlatformBackend]")
{
  REQUIRE(&JE::IPlatformBackend::Get() == &m_Backend);
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend creates an SDLWindow and returns a valid WindowHandle",
  "[JE::SDLPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  REQUIRE(SDL_GetWindowID(static_cast<SDL_Window *>(windowHandle)) != 0);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend creates an SDLWindow and can query the size",
  "[JE::SDLPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  REQUIRE(m_Backend.WindowSize(windowHandle) == TEST_WINDOW_SIZE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend creates an SDLWindow and can set window size",
  "[JE::SDLPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  m_Backend.SetWindowSize(windowHandle, NEW_WINDOW_SIZE);

  REQUIRE(m_Backend.WindowSize(windowHandle) == NEW_WINDOW_SIZE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend creates an SDLWindow and can query window title",
  "[JE::SDLPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  REQUIRE(m_Backend.WindowTitle(windowHandle) == TEST_WINDOW_TITLE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend creates an SDLWindow and can set window title",
  "[JE::SDLPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  m_Backend.SetWindowTitle(windowHandle, NEW_WINDOW_TITLE);

  REQUIRE(m_Backend.WindowTitle(windowHandle) == NEW_WINDOW_TITLE);

  m_Backend.DestroyWindow(windowHandle);
}


TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend Polls events to EventProcessor (Fake Quit event)",
  "[JE::SDLPlatformBackend]")
{

  class QuitChecker final : public JE::IEventProcessor
  {
  public:
    inline void OnEvent(JE::IEvent &event) override { m_QuitEventReceived = (event.Type() == JE::EventType::Quit); }
    [[nodiscard]] inline auto QuitEventReceived() const -> bool { return m_QuitEventReceived; }

  private:
    bool m_QuitEventReceived = false;
  } checker;

  SDL_Event quitEvent;
  quitEvent.type = SDL_EventType::SDL_QUIT;
  SDL_PushEvent(&quitEvent);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.QuitEventReceived());
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend Polls events to EventProcessor (Fake Window Size changed event)",
  "[JE::SDLPlatformBackend]")
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

  SDL_Event resizeEvent;
  resizeEvent.type = SDL_EventType::SDL_WINDOWEVENT;
  resizeEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED;
  resizeEvent.window.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(handle));
  resizeEvent.window.data1 = NEW_WINDOW_SIZE.Width;
  resizeEvent.window.data2 = NEW_WINDOW_SIZE.Height;
  SDL_PushEvent(&resizeEvent);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.WindowResizeEventReceived());

  m_Backend.DestroyWindow(handle);
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend Polls events to EventProcessor (Fake Window Close event)",
  "[JE::SDLPlatformBackend]")
{

  auto *handle = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  class WindowResizeChecker final : public JE::IEventProcessor
  {
  public:
    explicit WindowResizeChecker(JE::IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

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

  SDL_Event closeEvent;
  closeEvent.type = SDL_EventType::SDL_WINDOWEVENT;
  closeEvent.window.event = SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE;
  closeEvent.window.windowID = SDL_GetWindowID(static_cast<SDL_Window *>(handle));
  SDL_PushEvent(&closeEvent);

  m_Backend.PollEvents(checker);

  REQUIRE(checker.WindowCloseEventReceived());

  m_Backend.DestroyWindow(handle);
}