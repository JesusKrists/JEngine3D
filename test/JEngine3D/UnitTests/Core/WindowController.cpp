#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include "TestPlatformBackendFixture.hpp"


#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/Types.hpp>// for Size2D
#include <JEngine3D/Core/MemoryController.hpp>// for Size2D
#include <JEngine3D/Core/WindowController.hpp>// for Size2D
#include "JEngine3D/Core/Events.hpp"// for WindowCloseEvent, Win...

#include <iterator>// for end

class WindowControllerTestsFixture final : public TestPlatformBackendFixture
{
public:
  static constexpr auto TEST_WINDOW_TITLE = std::string_view{ "Test Window" };
  static constexpr auto TEST_WINDOW_SIZE = JE::Size2DI{ 1280, 720 };

  static constexpr auto NEW_WINDOW_TITLE = std::string_view{ "New Window Title" };
  static constexpr auto NEW_WINDOW_SIZE = JE::Size2DI{ 640, 480 };
};

TEST_CASE_METHOD(WindowControllerTestsFixture,
  "JE::WindowController creates JE::Window and keeps track of it (Has ownership)",
  "[JE::WindowController]")
{
  auto &window = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
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

TEST_CASE_METHOD(WindowControllerTestsFixture, "JE::Window sets title of underlying NativeHandle", "[JE::Window]")
{
  auto *nativeWindow = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  auto window = JE::Window(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, nativeWindow);

  REQUIRE(window.Title() == TEST_WINDOW_TITLE);
  REQUIRE(m_Backend.WindowTitle(window.NativeHandle()) == TEST_WINDOW_TITLE);

  window.SetTitle(NEW_WINDOW_TITLE);

  REQUIRE(window.Title() == NEW_WINDOW_TITLE);
  REQUIRE(m_Backend.WindowTitle(window.NativeHandle()) == NEW_WINDOW_TITLE);
}

TEST_CASE_METHOD(WindowControllerTestsFixture, "JE::Window sets size of underlying NativeHandle", "[JE::Window]")
{
  auto *nativeWindow = m_Backend.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  auto window = JE::Window(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE, nativeWindow);

  REQUIRE(window.Size() == TEST_WINDOW_SIZE);
  REQUIRE(m_Backend.WindowSize(window.NativeHandle()) == TEST_WINDOW_SIZE);

  window.SetSize(NEW_WINDOW_SIZE);

  REQUIRE(window.Size() == NEW_WINDOW_SIZE);
  REQUIRE(m_Backend.WindowSize(window.NativeHandle()) == NEW_WINDOW_SIZE);
}

TEST_CASE_METHOD(WindowControllerTestsFixture,
  "JE::WindowController processes WindowResizeEvent",
  "[JE::WindowController]")
{
  auto &window = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);

  JE::WindowResizeEvent resizeEvent{ window.NativeHandle(), NEW_WINDOW_SIZE };
  m_Backend.PushEvent(resizeEvent);
  m_Backend.PollEvents(m_WindowController);

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
  m_Backend.PollEvents(m_WindowController);

  REQUIRE(m_WindowController.Windows().empty());
  REQUIRE(closeEvent.Handled());
}