#include <catch2/catch_test_macros.hpp>// for StringRef, TEST_CASE
#include <cstdint>// for int32_t
#include <string>// for allocator, string
#include <string_view>// for string_view
#include <iostream>// for cout

#include <SDL.h>// for SDL_Init, SDL_Quit, SDL_INIT...
#include <SDL_video.h>// for SDL_CreateWindow, SDL_GetWin...
#include <SDL_error.h>// for SDL_GetError


namespace JE {

struct Size
{
  int32_t width;
  int32_t height;
};


using WindowHandle = void *;

class IPlatformBackend// NOLINT(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
{
public:
  virtual ~IPlatformBackend() = default;

  virtual auto Initialize() -> bool = 0;
  virtual auto CreateWindow(const std::string &title, const Size &size) -> WindowHandle = 0;
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class SDLPlatformBackend : public IPlatformBackend
{
public:
  auto Initialize() -> bool override
  {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      // TODO(JesusKrists): Log SDL error message with a proper logger
      std::cout << "SDL Failed to initialize - " << SDL_GetError() << "\n";
      return false;// NOLINT(readability-simplify-boolean-expr)
    }

    return true;
  }

  auto CreateWindow(const std::string &title, const Size &size) -> WindowHandle override
  {

    return SDL_CreateWindow(title.c_str(),
      SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
      SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
      size.width,
      size.height,
      SDL_WINDOW_OPENGL);
  }

  ~SDLPlatformBackend() override { SDL_Quit(); }
};

}// namespace JE


static constexpr auto WINDOW_TITLE = "Test Window";
static constexpr auto WINDOW_WIDTH = 1280;
static constexpr auto WINDOW_HEIGHT = 720;

TEST_CASE("JE::SDLPlatformBackend creates an SDLWindow and returns a valid WindowHandle", "[JE::SDLPlatformBackend]")
{
  JE::SDLPlatformBackend backend;
  backend.Initialize();

  auto *windowHandle = backend.CreateWindow(WINDOW_TITLE, { WINDOW_WIDTH, WINDOW_HEIGHT });
  auto sdlWindowTitleView = std::string_view{ SDL_GetWindowTitle(static_cast<SDL_Window *>(windowHandle)) };

  REQUIRE(sdlWindowTitleView == WINDOW_TITLE);

  SDL_DestroyWindow(static_cast<SDL_Window *>(windowHandle));
}
