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

void SDLGLGraphicsContext::Destroy() { SDL_GL_DeleteContext(NativeContextHandle()); }

void SDLGLGraphicsContext::MakeContextCurrent()
{
  JE_APP.RendererAPI().SetViewport({ { 0, 0 }, DrawableSize() });
  SDL_GL_MakeCurrent(static_cast<SDL_Window *>(NativeWindowHandle()), NativeContextHandle());
}

}// namespace JE