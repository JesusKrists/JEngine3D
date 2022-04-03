#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include "TestPlatformBackendFixture.hpp"

#include <JEngine3D/Core/Assert.hpp>// for ASSERT_, ASSERT
#include <JEngine3D/Core/Types.hpp>// for operator==, char_traits
#include <JEngine3D/Core/Events.hpp>
#include <JEngine3D/Platform/IPlatformBackend.hpp>// for IPlatformBackend
#include <JEngine3D/Core/WindowController.hpp>

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class Application : public IEventProcessor
{
public:
  static constexpr auto DEFAULT_SIZE = Size2D{ 1280, 720 };

  explicit Application(const std::string_view &title)
    : m_MainWindow(WindowController::Get().CreateWindow(title, DEFAULT_SIZE))
  {
    ASSERT(!s_ApplicationInstance, "Application instance already exists");
    s_ApplicationInstance = this;
  }
  ~Application() override { s_ApplicationInstance = nullptr; }

  [[nodiscard]] static inline auto Get() -> Application &
  {
    ASSERT(s_ApplicationInstance, "Application instance is null");
    return *s_ApplicationInstance;
  }

  void OnEvent(IEvent &event) override
  {
    if (event.Category() == EventCategory::Window) {
      WindowController::Get().OnEvent(event);
      if (event.Handled()) { return; }
    }

    EventDispatcher dispatcher{ event };

    dispatcher.Dispatch<EventType::Quit>([&](const IEvent &) {
      m_Running = false;
      return true;
    });
  }

  [[nodiscard]] inline auto MainWindow() -> Window & { return m_MainWindow; }
  [[nodiscard]] inline auto Running() const { return m_Running; }


  void ProcessMainLoop() { IPlatformBackend::Get().PollEvents(*this); }

  void Run(int32_t loopCount = -1)
  {
    ASSERT(loopCount != 0, "Cannot run zero loops");

    m_Running = true;

    if (loopCount < 0) {
      while (m_Running) { ProcessMainLoop(); }
    } else {
      while (m_Running && ((loopCount--) != 0)) { ProcessMainLoop(); }
    }
  }

private:
  Window &m_MainWindow;
  bool m_Running = false;

  static Application *s_ApplicationInstance;// NOLINT
};

Application *Application::s_ApplicationInstance = nullptr;// NOLINT

}// namespace JE

class ApplicationTestsFixture : public TestPlatformBackendFixture
{
public:
  static constexpr auto DEFAULT_TITLE = std::string_view{ "Test App" };
  static constexpr auto NEW_SIZE = JE::Size2D{ 640, 480 };

  ApplicationTestsFixture() : m_App(DEFAULT_TITLE) {}

protected:
  JE::Application m_App;
};

TEST_CASE_METHOD(ApplicationTestsFixture, "JE::Application creates MainWindow", "[JE::Application]")
{
  REQUIRE(m_App.MainWindow().Title() == DEFAULT_TITLE);
}


TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and stops running after receiving QuitEvent",
  "[JE::Application]")
{
  JE::QuitEvent quitEvent;
  m_Backend.PushEvent(quitEvent);

  m_App.Run(1);
  REQUIRE(!m_App.Running());
}

TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches WindowResizeEvent to WindowController",
  "[JE::Application]")
{
  JE::WindowResizeEvent resizeEvent{ m_App.MainWindow().NativeHandle(), NEW_SIZE };
  m_Backend.PushEvent(resizeEvent);

  m_App.Run(1);

  REQUIRE(m_App.MainWindow().Size() == NEW_SIZE);
}