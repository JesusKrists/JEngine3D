#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include "JEngine3D/Core/KeyCodes.hpp"
#include "TestPlatformBackendFixture.hpp"

#include <JEngine3D/Core/Types.hpp>// for operator==, char_traits
#include <JEngine3D/Core/Events.hpp>
#include <JEngine3D/Core/WindowController.hpp>
#include <JEngine3D/Core/Application.hpp>


class ApplicationTestsFixture : public TestPlatformBackendFixture
{
public:
  static constexpr auto DEFAULT_TITLE = std::string_view{ "Test App" };
  static constexpr auto NEW_SIZE = JE::Size2DI{ 640, 480 };

  static constexpr auto NEW_WINDOW_TITLE = std::string_view{ "Test App Window" };
  static constexpr auto NEW_WINDOW_SIZE = JE::Size2DI{ 320, 240 };

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
  REQUIRE(quitEvent.Handled());
}

TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches WindowResizeEvent to WindowController",
  "[JE::Application]")
{
  JE::WindowResizeEvent resizeEvent{ m_App.MainWindow().NativeHandle(), NEW_SIZE };
  m_Backend.PushEvent(resizeEvent);

  m_App.Run(1);

  REQUIRE(m_App.MainWindow().Size() == NEW_SIZE);
  REQUIRE(resizeEvent.Handled());
}


TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches WindowCloseEvent to WindowController",
  "[JE::Application]")
{
  auto &window = m_WindowController.CreateWindow(NEW_WINDOW_TITLE, NEW_WINDOW_SIZE);

  JE::WindowCloseEvent closeEvent{ window.NativeHandle() };
  m_Backend.PushEvent(closeEvent);

  m_App.Run(1);

  REQUIRE(m_WindowController.Windows().size() == 1);
  REQUIRE(closeEvent.Handled());
}


TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches KeyPressEvent to InputController",
  "[JE::Application]")
{
  JE::KeyPressEvent keyPressEvent{ m_App.MainWindow().NativeHandle(), JE::KeyCode::LShift, 0 };
  m_Backend.PushEvent(keyPressEvent);

  m_App.Run(1);

  REQUIRE(m_InputController.KeyPressed(JE::KeyCode::LShift));
  REQUIRE(keyPressEvent.Handled());
}

TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches KeyReleaseEvent to InputController",
  "[JE::Application]")
{
  JE::KeyPressEvent keyPressEvent{ m_App.MainWindow().NativeHandle(), JE::KeyCode::LShift, 0 };
  JE::KeyReleaseEvent keyReleaseEvent{ m_App.MainWindow().NativeHandle(), JE::KeyCode::LShift, 0 };
  m_Backend.PushEvent(keyPressEvent);
  m_Backend.PushEvent(keyReleaseEvent);

  m_App.Run(1);

  REQUIRE(!m_InputController.KeyPressed(JE::KeyCode::LShift));
  REQUIRE(keyPressEvent.Handled());
  REQUIRE(keyReleaseEvent.Handled());
}

TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches MousePressEvent to InputController",
  "[JE::Application]")
{
  JE::MousePressEvent mousePressEvent{ m_App.MainWindow().NativeHandle(), { 0, 0 }, JE::MouseButton::Middle, 1 };
  m_Backend.PushEvent(mousePressEvent);

  m_App.Run(1);

  REQUIRE(m_InputController.MousePressed(JE::MouseButton::Middle));
  REQUIRE(mousePressEvent.Handled());
}

TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches MouseReleaseEvent to InputController",
  "[JE::Application]")
{
  JE::MousePressEvent mousePressEvent{ m_App.MainWindow().NativeHandle(), { 0, 0 }, JE::MouseButton::Middle, 1 };
  JE::MouseReleaseEvent mouseReleaseEvent{ m_App.MainWindow().NativeHandle(), { 0, 0 }, JE::MouseButton::Middle, 0 };
  m_Backend.PushEvent(mousePressEvent);
  m_Backend.PushEvent(mouseReleaseEvent);

  m_App.Run(1);

  REQUIRE(!m_InputController.MousePressed(JE::KeyCode::Middle));
  REQUIRE(mousePressEvent.Handled());
  REQUIRE(mouseReleaseEvent.Handled());
}

// MouseMoveEvent
// MouseWheelEvent