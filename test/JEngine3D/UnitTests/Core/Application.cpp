#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include "TestPlatformBackendFixture.hpp"

#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/Types.hpp>// for operator==, char_traits
#include <JEngine3D/Core/Events.hpp>
#include <JEngine3D/Core/WindowController.hpp>
#include <JEngine3D/Core/Application.hpp>
#include <JEngine3D/Core/InputController.hpp>// for InputController
#include <JEngine3D/Core/ILayer.hpp>// for InputController


class ApplicationTestsFixture : public TestPlatformBackendFixture
{
public:
  static constexpr auto DEFAULT_TITLE = std::string_view{ "Test App" };

  static constexpr auto NEW_WINDOW_TITLE = std::string_view{ "Test App Window" };
  static constexpr auto NEW_WINDOW_SIZE = JE::Size2DI{ 320, 240 };
  static constexpr auto NEW_WINDOW_RESIZE = JE::Size2DI{ 640, 480 };

  ApplicationTestsFixture() : m_App(DEFAULT_TITLE) {}

protected:
  JE::Application m_App;
};

TEST_CASE_METHOD(ApplicationTestsFixture, "JE::Application creates MainWindow", "[JE::Application]")
{
  REQUIRE(m_App.MainWindow().Title() == DEFAULT_TITLE);
  REQUIRE(m_App.MainWindow().Size() == JE::Application::DEFAULT_SIZE);
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
  auto &window = m_WindowController.CreateWindow(NEW_WINDOW_TITLE, NEW_WINDOW_SIZE);

  JE::WindowResizeEvent resizeEvent{ window.NativeHandle(), NEW_WINDOW_RESIZE };
  m_Backend.PushEvent(resizeEvent);

  m_App.Run(1);

  REQUIRE(window.Size() == NEW_WINDOW_RESIZE);
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
  auto &window = m_WindowController.CreateWindow(NEW_WINDOW_TITLE, NEW_WINDOW_SIZE);

  JE::KeyPressEvent keyPressEvent{ window.NativeHandle(), JE::KeyCode::LShift, 0 };
  m_Backend.PushEvent(keyPressEvent);

  m_App.Run(1);

  REQUIRE(m_InputController.KeyPressed(JE::KeyCode::LShift));
  REQUIRE(keyPressEvent.Handled());
}

TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches KeyReleaseEvent to InputController",
  "[JE::Application]")
{
  auto &window = m_WindowController.CreateWindow(NEW_WINDOW_TITLE, NEW_WINDOW_SIZE);

  JE::KeyPressEvent keyPressEvent{ window.NativeHandle(), JE::KeyCode::LShift, 0 };
  JE::KeyReleaseEvent keyReleaseEvent{ window.NativeHandle(), JE::KeyCode::LShift, 0 };
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
  auto &window = m_WindowController.CreateWindow(NEW_WINDOW_TITLE, NEW_WINDOW_SIZE);

  JE::MousePressEvent mousePressEvent{ window.NativeHandle(), { 0, 0 }, JE::MouseButton::Middle, 1 };
  m_Backend.PushEvent(mousePressEvent);

  m_App.Run(1);

  REQUIRE(m_InputController.MousePressed(JE::MouseButton::Middle));
  REQUIRE(mousePressEvent.Handled());
}

TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches MouseReleaseEvent to InputController",
  "[JE::Application]")
{
  auto &window = m_WindowController.CreateWindow(NEW_WINDOW_TITLE, NEW_WINDOW_SIZE);

  JE::MousePressEvent mousePressEvent{ window.NativeHandle(), { 0, 0 }, JE::MouseButton::Middle, 1 };
  JE::MouseReleaseEvent mouseReleaseEvent{ window.NativeHandle(), { 0, 0 }, JE::MouseButton::Middle, 0 };
  m_Backend.PushEvent(mousePressEvent);
  m_Backend.PushEvent(mouseReleaseEvent);

  m_App.Run(1);

  REQUIRE(!m_InputController.MousePressed(JE::MouseButton::Middle));
  REQUIRE(mousePressEvent.Handled());
  REQUIRE(mouseReleaseEvent.Handled());
}

TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches MouseMoveEvent to InputController",
  "[JE::Application]")
{
  static constexpr auto MOUSE_POSITION = JE::Position2DI{ 375, 189 };
  static constexpr auto RELATIVE_MOUSE_POSITION = JE::Position2DI{ 40, 40 };

  auto &window = m_WindowController.CreateWindow(NEW_WINDOW_TITLE, NEW_WINDOW_SIZE);

  JE::MouseMoveEvent mouseMoveEvent{ window.NativeHandle(), MOUSE_POSITION, RELATIVE_MOUSE_POSITION };
  m_Backend.PushEvent(mouseMoveEvent);

  m_App.Run(1);

  REQUIRE(m_InputController.MousePosition() == MOUSE_POSITION);
  REQUIRE(m_InputController.RelativeMousePosition() == RELATIVE_MOUSE_POSITION);
  REQUIRE(mouseMoveEvent.Handled());
}

TEST_CASE_METHOD(ApplicationTestsFixture,
  "JE::Application is run and dispatches MouseWheelEvent to InputController",
  "[JE::Application]")
{
  auto &window = m_WindowController.CreateWindow(NEW_WINDOW_TITLE, NEW_WINDOW_SIZE);

  JE::MouseWheelEvent mouseWheelEvent{ window.NativeHandle(), 3 };
  m_Backend.PushEvent(mouseWheelEvent);

  m_App.Run(1);

  REQUIRE(m_InputController.MouseScrollAmount() == 3);
  REQUIRE(mouseWheelEvent.Handled());
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE_METHOD(ApplicationTestsFixture, "JE::Application can push and pop layers and overlays", "[JE::Application]")
{
  static constexpr auto LAYER_NAME = std::string_view{ "Layer Debug Name" };
  static constexpr auto OVERLAY_NAME = std::string_view{ "Overlay Debug Name" };

  class TestLayer : public JE::ILayer
  {
  public:
    explicit TestLayer(const std::string_view &name) : ILayer(name) {}

    void OnCreate() override { m_OnCreateCalled = true; }
    void OnDestroy() override { m_OnDestroyCalled = true; }

    void OnUpdate() override { m_OnUpdateCalled = true; }
    void OnImGuiRender() override { m_OnImGuiRenderCalled = true; }

    void OnEvent(JE::IEvent &event) override
    {
      JE::UNUSED(event);
      m_OnEventCalled = true;
    }

    [[nodiscard]] inline auto OnCreateCalled() const -> bool { return m_OnCreateCalled; }
    [[nodiscard]] inline auto OnDestroyCalled() const -> bool { return m_OnDestroyCalled; }

    [[nodiscard]] inline auto OnUpdateCalled() const -> bool { return m_OnUpdateCalled; }
    [[nodiscard]] inline auto OnImGuiRenderCalled() const -> bool { return m_OnImGuiRenderCalled; }

    [[nodiscard]] inline auto OnEventCalled() const -> bool { return m_OnEventCalled; }

  private:
    bool m_OnCreateCalled = false;
    bool m_OnDestroyCalled = false;
    bool m_OnUpdateCalled = false;
    bool m_OnImGuiRenderCalled = false;
    bool m_OnEventCalled = false;

  } testLayer{ LAYER_NAME }, testOverlay{ OVERLAY_NAME };

  REQUIRE(testLayer.DebugName() == LAYER_NAME);
  REQUIRE(testOverlay.DebugName() == OVERLAY_NAME);


  auto &window = m_WindowController.CreateWindow(NEW_WINDOW_TITLE, NEW_WINDOW_SIZE);
  JE::MouseWheelEvent mouseWheelEvent{ window.NativeHandle(), 3 };
  m_Backend.PushEvent(mouseWheelEvent);

  m_App.PushLayer(testLayer);
  m_App.PushOverlay(testOverlay);

  m_App.Run(1);

  REQUIRE(testLayer.OnCreateCalled());
  REQUIRE(testOverlay.OnCreateCalled());
  REQUIRE(testLayer.OnUpdateCalled());
  REQUIRE(testOverlay.OnUpdateCalled());
  REQUIRE(testLayer.OnImGuiRenderCalled());
  REQUIRE(testOverlay.OnImGuiRenderCalled());
  REQUIRE(testLayer.OnEventCalled());
  REQUIRE(testOverlay.OnEventCalled());

  m_App.PopLayer(testLayer);
  m_App.PopOverlay(testOverlay);

  REQUIRE(testLayer.OnDestroyCalled());
  REQUIRE(testOverlay.OnDestroyCalled());
}

TEST_CASE_METHOD(ApplicationTestsFixture, "JE::Application updates delta time", "[JE::Application]")
{
  m_App.Run(2);
  REQUIRE(m_App.DeltaTime() != 0);
}