#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include "TestPlatformBackendFixture.hpp"

#include <JEngine3D/Core/Types.hpp>// for operator==, char_traits
#include <JEngine3D/Core/Events.hpp>
#include <JEngine3D/Core/WindowController.hpp>
#include <JEngine3D/Core/Application.hpp>


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