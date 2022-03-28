#include "SDLPlatformBackend.hpp"

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Events.hpp"

#include <cstring>// IWYU pragma: keep
#include <SDL_events.h>// for SDL_PollEvent, SDL_Event, SDL_...
#include <SDL_error.h>// for SDL_GetError
#include <SDL_video.h>// for SDL_Window, SDL_CreateWindow, SDL_DestroyWindow
#include <SDL.h>

namespace JE {

static auto s_Initialized = false;// NOLINT

SDLPlatformBackend::~SDLPlatformBackend() { SDL_Quit(); }

auto SDLPlatformBackend::Initialize() -> bool
{
  ASSERT(!s_Initialized, "Backend already initialized");

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    // TODO(JesusKrists): Log SDL error message with a proper logger
    std::cout << "SDL Failed to initialize - " << SDL_GetError() << "\n";
    return false;
  }

  s_Initialized = true;
  return true;
}

auto SDLPlatformBackend::Initialized() -> bool { return s_Initialized; }

auto SDLPlatformBackend::CreateWindow(const std::string_view &title, const Size2D &size) -> NativeWindowHandle
{

  return SDL_CreateWindow(title.data(),
    SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
    SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
    size.Width,
    size.Height,
    SDL_WINDOW_OPENGL);
}

void SDLPlatformBackend::DestroyWindow(NativeWindowHandle handle)
{
  SDL_DestroyWindow(static_cast<SDL_Window *>(handle));
}

auto SDLPlatformBackend::WindowSize(NativeWindowHandle handle) -> Size2D
{
  Size2D size{};
  SDL_GetWindowSize(static_cast<SDL_Window *>(handle), &size.Width, &size.Height);
  return size;
}

void SDLPlatformBackend::SetWindowSize(NativeWindowHandle handle, const Size2D &size)
{
  SDL_SetWindowSize(static_cast<SDL_Window *>(handle), size.Width, size.Height);
}

auto SDLPlatformBackend::WindowTitle(NativeWindowHandle handle) -> std::string_view
{
  return SDL_GetWindowTitle(static_cast<SDL_Window *>(handle));
}

void SDLPlatformBackend::SetWindowTitle(NativeWindowHandle handle, const std::string_view &title)
{
  SDL_SetWindowTitle(static_cast<SDL_Window *>(handle), title.data());
}

void SDLPlatformBackend::PollEvents(IEventProcessor &processor)
{
  SDL_Event nativeEvent;
  while (SDL_PollEvent(&nativeEvent) != 0) {
    if (nativeEvent.type == SDL_EventType::SDL_QUIT) {
      QuitEvent event;
      processor.OnEvent(event);
    }
  }
}

}// namespace JE