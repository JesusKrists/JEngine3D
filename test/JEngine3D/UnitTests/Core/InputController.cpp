#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include "TestPlatformBackendFixture.hpp"

#include <JEngine3D/Core/Events.hpp>
#include <JEngine3D/Core/InputController.hpp>// for InputController
#include <JEngine3D/Core/Types.hpp>// for Position2DI


class InputControllerTestsFixture : public TestPlatformBackendFixture
{
public:
  static constexpr auto MOUSE_POSITION = JE::Position2DI{ 375, 189 };
  static constexpr auto RELATIVE_MOUSE_POSITION = JE::Position2DI{ 40, 40 };
};

TEST_CASE_METHOD(InputControllerTestsFixture, "JE::InputController processes KeyPress Event", "[JE::InputController]")
{
  JE::KeyPressEvent keyPressEvent{ nullptr, JE::KeyCode::Space, 1 };
  m_Backend.PushEvent(keyPressEvent);
  m_Backend.PollEvents(m_InputController);

  REQUIRE(m_InputController.KeyPressed(JE::KeyCode::Space));
  REQUIRE(keyPressEvent.Handled());
}

TEST_CASE_METHOD(InputControllerTestsFixture, "JE::InputController processes KeyRelease Event", "[JE::InputController]")
{
  JE::KeyPressEvent keyPressEvent{ nullptr, JE::KeyCode::Space, 1 };
  JE::KeyReleaseEvent keyReleaseEvent{ nullptr, JE::KeyCode::Space, 1 };
  m_Backend.PushEvent(keyPressEvent);
  m_Backend.PushEvent(keyReleaseEvent);
  m_Backend.PollEvents(m_InputController);

  REQUIRE(!m_InputController.KeyPressed(JE::KeyCode::Space));
  REQUIRE(keyPressEvent.Handled());
  REQUIRE(keyReleaseEvent.Handled());
}

TEST_CASE_METHOD(InputControllerTestsFixture, "JE::InputController processes MousePress Event", "[JE::InputController]")
{
  JE::MousePressEvent mousePressEvent{ nullptr, { 0, 0 }, JE::MouseButton::Middle, 1 };
  m_Backend.PushEvent(mousePressEvent);
  m_Backend.PollEvents(m_InputController);

  REQUIRE(m_InputController.MousePressed(JE::MouseButton::Middle));
  REQUIRE(mousePressEvent.Handled());
}

TEST_CASE_METHOD(InputControllerTestsFixture,
  "JE::InputController processes MouseRelease Event",
  "[JE::InputController]")
{
  JE::MousePressEvent mousePressEvent{ nullptr, { 0, 0 }, JE::MouseButton::Middle, 1 };
  JE::MouseReleaseEvent mouseReleaseEvent{ nullptr, { 0, 0 }, JE::MouseButton::Middle, 1 };
  m_Backend.PushEvent(mousePressEvent);
  m_Backend.PushEvent(mouseReleaseEvent);
  m_Backend.PollEvents(m_InputController);

  REQUIRE(!m_InputController.MousePressed(JE::MouseButton::Middle));
  REQUIRE(mousePressEvent.Handled());
  REQUIRE(mouseReleaseEvent.Handled());
}

TEST_CASE_METHOD(InputControllerTestsFixture, "JE::InputController processes MouseMove Event", "[JE::InputController]")
{
  JE::MouseMoveEvent mouseMoveEvent{ nullptr, MOUSE_POSITION, RELATIVE_MOUSE_POSITION };
  m_Backend.PushEvent(mouseMoveEvent);
  m_Backend.PollEvents(m_InputController);

  REQUIRE(m_InputController.MousePosition() == MOUSE_POSITION);
  REQUIRE(m_InputController.RelativeMousePosition() == RELATIVE_MOUSE_POSITION);
  REQUIRE(mouseMoveEvent.Handled());
}

TEST_CASE_METHOD(InputControllerTestsFixture, "JE::InputController processes MouseWheel Event", "[JE::InputController]")
{
  JE::MouseWheelEvent mouseWheelEvent{ nullptr, 2 };
  m_Backend.PushEvent(mouseWheelEvent);
  m_Backend.PollEvents(m_InputController);

  REQUIRE(m_InputController.MouseScrollAmount() == 2);
  REQUIRE(mouseWheelEvent.Handled());
}