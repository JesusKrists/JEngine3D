#include <catch2/catch_test_macros.hpp>// for StringRef, TEST_CASE
#include <cstdint>// for int32_t
#include <string>// for allocator, string
#include <string_view>// for string_view
#include <iostream>// for cout

#include <SDL.h>// for SDL_Init, SDL_Quit, SDL_INIT...
#include <SDL_video.h>// for SDL_CreateWindow, SDL_GetWin...
#include <SDL_error.h>// for SDL_GetError


namespace JE {

template<typename T> constexpr void UNUSED(T &&val) { (void)val; }

struct Size2D
{
  int32_t width;// cppcheck-suppress unusedStructMember
  int32_t height;// cppcheck-suppress unusedStructMember
};


class IPlatformBackend// NOLINT(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
{
public:
  using WindowHandle = void *;

  virtual ~IPlatformBackend() = default;

  [[nodiscard]] virtual auto Initialize() -> bool = 0;
  [[nodiscard]] virtual auto CreateWindow(const std::string &title, const Size2D &size) -> WindowHandle = 0;
  virtual void DestroyWindow(WindowHandle handle) = 0;
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class SDLPlatformBackend final : public IPlatformBackend
{
public:
  ~SDLPlatformBackend() override { SDL_Quit(); }

  [[nodiscard]] auto Initialize() -> bool override
  {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      // TODO(JesusKrists): Log SDL error message with a proper logger
      std::cout << "SDL Failed to initialize - " << SDL_GetError() << "\n";
      return false;
    }

    return true;
  }

  [[nodiscard]] auto CreateWindow(const std::string &title, const Size2D &size) -> WindowHandle override
  {

    return SDL_CreateWindow(title.c_str(),
      SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
      SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
      size.width,
      size.height,
      SDL_WINDOW_OPENGL);
  }

  void DestroyWindow(WindowHandle handle) override { SDL_DestroyWindow(static_cast<SDL_Window *>(handle)); }
};

}// namespace JE


static constexpr auto WINDOW_TITLE = "Test Window";
static constexpr auto WINDOW_WIDTH = 1280;
static constexpr auto WINDOW_HEIGHT = 720;

TEST_CASE("JE::SDLPlatformBackend creates an SDLWindow and returns a valid WindowHandle", "[JE::SDLPlatformBackend]")
{
  JE::SDLPlatformBackend backend;
  auto success = backend.Initialize();
  JE::UNUSED(success);

  auto *windowHandle = backend.CreateWindow(WINDOW_TITLE, { WINDOW_WIDTH, WINDOW_HEIGHT });

  REQUIRE(std::string_view{ SDL_GetWindowTitle(static_cast<SDL_Window *>(windowHandle)) } == WINDOW_TITLE);

  backend.DestroyWindow(windowHandle);
}
