#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Platform/IPlatformBackend.hpp>// for IPlatformBackend
#include <JEngine3D/Platform/SDLPlatformBackend.hpp>// for SDLPlatformBackend
#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/Types.hpp>// for Size2D
#include <JEngine3D/Event/Events.hpp>

#include <SDL_video.h>// for SDL_GetWindowID
#include <SDL_events.h>
#include <string>// for char_traits
#include <string_view>// for operator==, bas...

class SDLPlatformBackendTestsFixture
{
public:
  static constexpr auto WINDOW_SIZE = JE::Size2D{ 1280, 720 };
  static constexpr auto WINDOW_TITLE = std::string_view{ "Test Window" };

  static constexpr auto NEW_WINDOW_SIZE = JE::Size2D{ 640, 480 };
  static constexpr auto NEW_WINDOW_TITLE = std::string_view{ "Test Window - New" };

  SDLPlatformBackendTestsFixture() { JE::UNUSED(m_Backend.Initialize()); }

protected:
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
  auto *windowHandle = m_Backend.CreateWindow(WINDOW_TITLE, WINDOW_SIZE);

  REQUIRE(SDL_GetWindowID(static_cast<SDL_Window *>(windowHandle)) != 0);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend creates an SDLWindow and can query the size",
  "[JE::SDLPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(WINDOW_TITLE, WINDOW_SIZE);

  REQUIRE(m_Backend.WindowSize(windowHandle) == WINDOW_SIZE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend creates an SDLWindow and can set window size",
  "[JE::SDLPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(WINDOW_TITLE, WINDOW_SIZE);
  m_Backend.SetWindowSize(windowHandle, NEW_WINDOW_SIZE);

  REQUIRE(m_Backend.WindowSize(windowHandle) == NEW_WINDOW_SIZE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend creates an SDLWindow and can query window title",
  "[JE::SDLPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(WINDOW_TITLE, WINDOW_SIZE);

  REQUIRE(m_Backend.WindowTitle(windowHandle) == WINDOW_TITLE);

  m_Backend.DestroyWindow(windowHandle);
}

TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend creates an SDLWindow and can set window title",
  "[JE::SDLPlatformBackend]")
{
  auto *windowHandle = m_Backend.CreateWindow(WINDOW_TITLE, WINDOW_SIZE);
  m_Backend.SetWindowTitle(windowHandle, NEW_WINDOW_TITLE);

  REQUIRE(m_Backend.WindowTitle(windowHandle) == NEW_WINDOW_TITLE);

  m_Backend.DestroyWindow(windowHandle);
}


TEST_CASE_METHOD(SDLPlatformBackendTestsFixture,
  "JE::SDLPlatformBackend Polls events to EventProcessor (Fake Quit event)",
  "[JE::SDLPlatformBackend]")
{

  class SDLQUITChecker final : public JE::IEventProcessor
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