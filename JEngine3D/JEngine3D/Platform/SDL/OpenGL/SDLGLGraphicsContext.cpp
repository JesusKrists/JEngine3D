#include "SDLGLGraphicsContext.hpp"

#include "JEngine3D/Core/Application.hpp"

#include <cstring>// IWYU pragma: keep
#include <SDL_video.h>// for SDL_GL_DeleteContext, SDL_GL_MakeCurrent, SDL...

namespace JE {


auto SDLGLGraphicsContext::DrawableSize() -> Size2DI
{
  Size2DI size{};
  SDL_GL_GetDrawableSize(static_cast<SDL_Window *>(NativeWindowHandle()), &size.Width, &size.Height);
  return size;
}

void SDLGLGraphicsContext::SwapBuffers() { SDL_GL_SwapWindow(static_cast<SDL_Window *>(NativeWindowHandle())); }

void SDLGLGraphicsContext::MakeContextCurrent()
{
  if (SDL_GL_MakeCurrent(static_cast<SDL_Window *>(NativeWindowHandle()), NativeContextHandle()) != 0) {
    JE::Logger::CoreLogger().error("Failed to set OpenGL context - {}", SDL_GetError());
    DEBUGBREAK();
  }
  SDL_GL_SetSwapInterval(0);
  JE_APP.RendererAPI().SetViewport({ { 0, 0 }, DrawableSize() });
}

}// namespace JE