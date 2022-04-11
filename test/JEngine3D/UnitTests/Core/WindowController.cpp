#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include "TestPlatformBackendFixture.hpp"


#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/Types.hpp>// for Size2D
#include <JEngine3D/Core/MemoryController.hpp>// for Size2D
#include <JEngine3D/Core/WindowController.hpp>// for Size2D
#include <JEngine3D/Core/Events.hpp>// for WindowCloseEvent, Win...
#include <JEngine3D/Platform/IPlatformBackend.hpp>// for IPlatformBackend

#include <iterator>// for end

class WindowControllerTestsFixture : public TestPlatformBackendFixture
{
public:
  WindowControllerTestsFixture() { m_Backend.SetEventProcessor(&m_WindowController); }

  static constexpr auto TEST_WINDOW_TITLE = std::string_view{ "Test Window" };
  static constexpr auto TEST_WINDOW_SIZE = JE::Size2DI{ 1280, 720 };
  static constexpr auto TEST_WINDOW_POSITION = JE::Position2DI{ 100, 100 };

  static constexpr auto NEW_WINDOW_TITLE = std::string_view{ "New Window Title" };
  static constexpr auto NEW_WINDOW_SIZE = JE::Size2DI{ 640, 480 };
  static constexpr auto NEW_WINDOW_POSITION = JE::Position2DI{ 500, 500 };
};

TEST_CASE_METHOD(WindowControllerTestsFixture,
  "JE::WindowController creates JE::Window and keeps track of it (Has ownership)",
  "[JE::WindowController]")
{
  auto &window = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  REQUIRE(window.Position() == JE::IPlatformBackend::WINDOW_CENTER_POSITION);

  auto windowFindFunction = [&](const JE::Scope<JE::Window, JE::MemoryTag::App> &windowEntry) {
    return windowEntry.get() == &window;
  };

  const auto &windows = m_WindowController.Windows();
  auto windowIt = JE::FindIf(windows, windowFindFunction);

  REQUIRE(windowIt != std::end(windows));

  m_WindowController.DestroyAllWindows();
  windowIt = JE::FindIf(windows, windowFindFunction);

  REQUIRE(windowIt == std::end(windows));
}


TEST_CASE_METHOD(WindowControllerTestsFixture,
  "JE::WindowController creates JE::Window and can destroy single window",
  "[JE::WindowController]")
{
  auto &window = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  auto &window2 = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  JE::UNUSED(window2);

  auto windowFindFunction = [&](const JE::Scope<JE::Window, JE::MemoryTag::App> &windowEntry) {
    return windowEntry.get() == &window;
  };

  const auto &windows = m_WindowController.Windows();
  auto windowIt = JE::FindIf(windows, windowFindFunction);

  REQUIRE(windowIt != std::end(windows));

  m_WindowController.DestroyWindow(window);
  windowIt = JE::FindIf(windows, windowFindFunction);

  REQUIRE(windowIt == std::end(windows));
  REQUIRE(!windows.empty());

  m_WindowController.DestroyAllWindows();
}

TEST_CASE_METHOD(WindowControllerTestsFixture,
  "JE::WindowController processes WindowResizeEvent",
  "[JE::WindowController]")
{
  auto &window = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  JE::WindowResizeEvent resizeEvent{ window.NativeHandle(), NEW_WINDOW_SIZE };
  m_Backend.PushEvent(resizeEvent);
  m_Backend.PollEvents();

  REQUIRE(window.Size() == NEW_WINDOW_SIZE);
  REQUIRE(resizeEvent.Handled());
}

TEST_CASE_METHOD(WindowControllerTestsFixture,
  "JE::WindowController processes WindowCloseEvent",
  "[JE::WindowController]")
{
  auto &window = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  JE::WindowCloseEvent closeEvent{ window.NativeHandle() };
  m_Backend.PushEvent(closeEvent);
  m_Backend.PollEvents();

  REQUIRE(m_WindowController.Windows().empty());
  REQUIRE(closeEvent.Handled());
}

TEST_CASE_METHOD(WindowControllerTestsFixture,
  "JE::WindowController processes WindowMoveEvent",
  "[JE::WindowController]")
{
  auto &window = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION);

  JE::WindowMoveEvent moveEvent{ window.NativeHandle(), NEW_WINDOW_POSITION };
  m_Backend.PushEvent(moveEvent);
  m_Backend.PollEvents();

  REQUIRE(window.Position() == NEW_WINDOW_POSITION);
  REQUIRE(moveEvent.Handled());
}

TEST_CASE_METHOD(WindowControllerTestsFixture,
  "JE::WindowController processes WindowHideEvent",
  "[JE::WindowController]")
{
  auto &window = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  REQUIRE(window.Shown());

  JE::WindowHideEvent hideEvent{ window.NativeHandle() };
  m_Backend.PushEvent(hideEvent);
  m_Backend.PollEvents();

  REQUIRE(!window.Shown());
  REQUIRE(hideEvent.Handled());
}

TEST_CASE_METHOD(WindowControllerTestsFixture,
  "JE::WindowController processes WindowShowEvent",
  "[JE::WindowController]")
{
  JE::WindowConfiguration config{};
  config.Hidden = true;
  auto &window = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION, config);

  REQUIRE(!window.Shown());

  JE::WindowShowEvent showEvent{ window.NativeHandle() };
  m_Backend.PushEvent(showEvent);
  m_Backend.PollEvents();

  REQUIRE(window.Shown());
  REQUIRE(showEvent.Handled());
}

TEST_CASE_METHOD(WindowControllerTestsFixture, "JE::Window sets title of underlying NativeHandle", "[JE::Window]")
{
  JE::WindowConfiguration config{};
  auto *nativeWindow = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION, config);
  auto window = JE::Window(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION, config, nativeWindow);

  REQUIRE(window.Title() == TEST_WINDOW_TITLE);
  REQUIRE(m_Backend.WindowTitle(window.NativeHandle()) == TEST_WINDOW_TITLE);

  window.SetTitle(NEW_WINDOW_TITLE);

  REQUIRE(window.Title() == NEW_WINDOW_TITLE);
  REQUIRE(m_Backend.WindowTitle(window.NativeHandle()) == NEW_WINDOW_TITLE);
}

TEST_CASE_METHOD(WindowControllerTestsFixture, "JE::Window sets size of underlying NativeHandle", "[JE::Window]")
{
  JE::WindowConfiguration config{};
  auto *nativeWindow = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION, config);
  auto window = JE::Window(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION, config, nativeWindow);

  REQUIRE(window.Size() == TEST_WINDOW_SIZE);
  REQUIRE(m_Backend.WindowSize(window.NativeHandle()) == TEST_WINDOW_SIZE);

  window.SetSize(NEW_WINDOW_SIZE);

  REQUIRE(window.Size() == NEW_WINDOW_SIZE);
  REQUIRE(m_Backend.WindowSize(window.NativeHandle()) == NEW_WINDOW_SIZE);
}

TEST_CASE_METHOD(WindowControllerTestsFixture, "JE::Window sets position of underlying NativeHandle", "[JE::Window]")
{
  JE::WindowConfiguration config{};
  auto *nativeWindow = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION, config);
  auto window = JE::Window(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION, config, nativeWindow);

  REQUIRE(window.Position() == TEST_WINDOW_POSITION);
  REQUIRE(m_Backend.WindowPosition(window.NativeHandle()) == TEST_WINDOW_POSITION);

  window.SetPosition(NEW_WINDOW_POSITION);

  REQUIRE(window.Position() == NEW_WINDOW_POSITION);
  REQUIRE(m_Backend.WindowPosition(window.NativeHandle()) == NEW_WINDOW_POSITION);
}

TEST_CASE_METHOD(WindowControllerTestsFixture, "JE::Window shows and hides underlying NativeHandle", "[JE::Window]")
{
  JE::WindowConfiguration config{};
  auto *nativeWindow = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION, config);
  auto window = JE::Window(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, TEST_WINDOW_POSITION, config, nativeWindow);

  REQUIRE(window.Shown() == true);

  window.Hide();

  REQUIRE(window.Shown() == false);

  window.Show();

  REQUIRE(window.Shown() == true);
}