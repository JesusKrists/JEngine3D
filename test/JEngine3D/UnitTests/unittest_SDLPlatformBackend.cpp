
#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Platform/IPlatformBackend.hpp>// for IPlatformBackend
#include <JEngine3D/Platform/SDLPlatformBackend.hpp>// for SDLPlatformBackend
#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/Types.hpp>// for Size2D

#include <SDL_video.h>// for SDL_GetWindowID
#include <string>// for char_traits
#include <string_view>// for operator==, bas...

static constexpr auto WINDOW_TITLE = std::string_view{ "Test Window" };
static constexpr auto WINDOW_SIZE = JE::Size2D{ 1280, 720 };

TEST_CASE("JE::IPlatformBackend creates singleton instance which can be accessed", "[JE::IPlatformBackend]")
{
  JE::SDLPlatformBackend backend;
  REQUIRE(&JE::IPlatformBackend::Get() == &backend);
}

TEST_CASE("JE::SDLPlatformBackend creates an SDLWindow and returns a valid WindowHandle", "[JE::SDLPlatformBackend]")
{
  JE::SDLPlatformBackend backend;
  auto success = backend.Initialize();
  JE::UNUSED(success);

  auto *windowHandle = backend.CreateWindow(WINDOW_TITLE, WINDOW_SIZE);


  REQUIRE(SDL_GetWindowID(static_cast<SDL_Window *>(windowHandle)) != 0);

  backend.DestroyWindow(windowHandle);

  REQUIRE(SDL_GetWindowID(static_cast<SDL_Window *>(windowHandle)) == 0);
}

TEST_CASE("JE::SDLPlatformBackend creates an SDLWindow and can query the size", "[JE::SDLPlatformBackend]")
{
  JE::SDLPlatformBackend backend;
  auto success = backend.Initialize();
  JE::UNUSED(success);

  auto *windowHandle = backend.CreateWindow(WINDOW_TITLE, WINDOW_SIZE);

  REQUIRE(backend.WindowSize(windowHandle) == WINDOW_SIZE);

  backend.DestroyWindow(windowHandle);
}

TEST_CASE("JE::SDLPlatformBackend creates an SDLWindow and can set window size", "[JE::SDLPlatformBackend]")
{
  static constexpr auto NEW_WINDOW_SIZE = JE::Size2D{ 640, 480 };

  JE::SDLPlatformBackend backend;
  auto success = backend.Initialize();
  JE::UNUSED(success);

  auto *windowHandle = backend.CreateWindow(WINDOW_TITLE, WINDOW_SIZE);
  backend.SetWindowSize(windowHandle, NEW_WINDOW_SIZE);

  REQUIRE(backend.WindowSize(windowHandle) == NEW_WINDOW_SIZE);

  backend.DestroyWindow(windowHandle);
}

TEST_CASE("JE::SDLPlatformBackend creates an SDLWindow and can query window title", "[JE::SDLPlatformBackend]")
{
  JE::SDLPlatformBackend backend;
  auto success = backend.Initialize();
  JE::UNUSED(success);

  auto *windowHandle = backend.CreateWindow(WINDOW_TITLE, WINDOW_SIZE);

  REQUIRE(backend.WindowTitle(windowHandle) == WINDOW_TITLE);

  backend.DestroyWindow(windowHandle);
}

TEST_CASE("JE::SDLPlatformBackend creates an SDLWindow and can set window title", "[JE::SDLPlatformBackend]")
{
  static constexpr auto NEW_WINDOW_TITLE = std::string_view{ "Test Window - New" };

  JE::SDLPlatformBackend backend;
  auto success = backend.Initialize();
  JE::UNUSED(success);

  auto *windowHandle = backend.CreateWindow(WINDOW_TITLE, WINDOW_SIZE);
  backend.SetWindowTitle(windowHandle, NEW_WINDOW_TITLE);

  REQUIRE(backend.WindowTitle(windowHandle) == NEW_WINDOW_TITLE);

  backend.DestroyWindow(windowHandle);
}