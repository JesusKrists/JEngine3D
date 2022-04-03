#include "SDLPlatformBackend.hpp"

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Events.hpp"
#include "JEngine3D/Core/LoggerController.hpp"// for Logger

#include <exception>// for exception
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
    JE::Logger::CoreLogger().error("SDL Failed to initialize - {}", SDL_GetError());
    return false;
  }

  s_Initialized = true;
  return true;
}

auto SDLPlatformBackend::Initialized() -> bool { return s_Initialized; }

auto SDLPlatformBackend::CreateWindow(const std::string_view &title, const Size2D &size) -> NativeWindowHandle
{
  ASSERT(size.Width > 0 && size.Height > 0, "Window size cannot be zero or negative");
  return SDL_CreateWindow(title.data(),
    SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
    SDL_WINDOWPOS_CENTERED,// NOLINT(hicpp-signed-bitwise)
    size.Width,
    size.Height,
    SDL_WINDOW_OPENGL);
}

void SDLPlatformBackend::DestroyWindow(NativeWindowHandle handle)
{
  ASSERT(handle, "Window handle is null");
  SDL_DestroyWindow(static_cast<SDL_Window *>(handle));
}

auto SDLPlatformBackend::WindowSize(NativeWindowHandle handle) -> Size2D
{
  ASSERT(handle, "Window handle is null");
  Size2D size{};
  SDL_GetWindowSize(static_cast<SDL_Window *>(handle), &size.Width, &size.Height);
  return size;
}

void SDLPlatformBackend::SetWindowSize(NativeWindowHandle handle, const Size2D &size)
{
  ASSERT(handle, "Window handle is null");
  ASSERT(size.Width > 0 && size.Height > 0, "Window size cannot be zero or negative");
  SDL_SetWindowSize(static_cast<SDL_Window *>(handle), size.Width, size.Height);
}

auto SDLPlatformBackend::WindowTitle(NativeWindowHandle handle) -> std::string_view
{
  ASSERT(handle, "Window handle is null");
  return SDL_GetWindowTitle(static_cast<SDL_Window *>(handle));
}

void SDLPlatformBackend::SetWindowTitle(NativeWindowHandle handle, const std::string_view &title)
{
  ASSERT(handle, "Window handle is null");
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

    if (nativeEvent.type == SDL_EventType::SDL_WINDOWEVENT) {
      switch (nativeEvent.window.event) {
      case SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED:
        WindowResizeEvent event{ SDL_GetWindowFromID(nativeEvent.window.windowID),
          { nativeEvent.window.data1, nativeEvent.window.data2 } };
        processor.OnEvent(event);
        break;
      }
    }
  }
}

}// namespace JE