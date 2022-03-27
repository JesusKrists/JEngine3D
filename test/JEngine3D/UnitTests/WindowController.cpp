#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include "TestPlatformBackend.hpp"

#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/Types.hpp>// for Size2D
#include <JEngine3D/Core/MemoryController.hpp>// for Size2D
#include <JEngine3D/Core/WindowController.hpp>// for Size2D

#include <iterator>// for end
#include <string_view>// for string_view
#include <vector>// for vector

namespace JE {

// TODO(JesusKrists): Write constexpr tests for JE::FindIf and JE::Size2D operator== and JE::AlignTo and
// JE::MemoryController and JE::Window

}// namespace JE

class WindowControllerTestsFixture
{
public:
  static constexpr auto TEST_WINDOW_TITLE = std::string_view{ "Test Window" };
  static constexpr auto TEST_WINDOW_SIZE = JE::Size2D{ 1280, 720 };

  WindowControllerTestsFixture() { JE::UNUSED(m_Backend.Initialize()); }

protected:
  TestPlatformBackend m_Backend;
  JE::MemoryController m_MemoryController;
  JE::WindowController m_WindowController;
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

  m_WindowController.DeleteAllWindows();
  windowIt = JE::FindIf(windows, windowFindFunction);

  REQUIRE(windowIt == std::end(windows));
}