#include "SDLPlatformBackend.hpp"

#include <cstring>// IWYU pragma: keep
#include <iostream>// for operator<<, basic_ostream, char_traits, cout
#include <SDL_error.h>// for SDL_GetError
#include <SDL_video.h>// for SDL_Window, SDL_CreateWindow, SDL_DestroyWindow
#include <SDL.h>

namespace JE {

SDLPlatformBackend::~SDLPlatformBackend() { SDL_Quit(); }

auto SDLPlatformBackend::Initialize() -> bool
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    // TODO(JesusKrists): Log SDL error message with a proper logger
    std::cout << "SDL Failed to initialize - " << SDL_GetError() << "\n";
    return false;
  }

  return true;
}

auto SDLPlatformBackend::CreateWindow(std::string_view title, const Size2D &size) -> NativeWindowHandle
{

  return SDL_CreateWindow(title.data(),
    SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
    SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
    size.width,
    size.height,
    SDL_WINDOW_OPENGL);
}

void SDLPlatformBackend::DestroyWindow(NativeWindowHandle handle)
{
  SDL_DestroyWindow(static_cast<SDL_Window *>(handle));
}

auto SDLPlatformBackend::WindowSize(NativeWindowHandle handle) -> Size2D
{
  Size2D size{};
  SDL_GetWindowSize(static_cast<SDL_Window *>(handle), &size.width, &size.height);
  return size;
}

void SDLPlatformBackend::SetWindowSize(NativeWindowHandle handle, const Size2D &size)
{
  SDL_SetWindowSize(static_cast<SDL_Window *>(handle), size.width, size.height);
}

auto SDLPlatformBackend::WindowTitle(NativeWindowHandle handle) -> std::string_view
{
  return SDL_GetWindowTitle(static_cast<SDL_Window *>(handle));
}

void SDLPlatformBackend::SetWindowTitle(NativeWindowHandle handle, std::string_view title)
{
  SDL_SetWindowTitle(static_cast<SDL_Window *>(handle), title.data());
}

}// namespace JE