#include "SDLSoftwareGraphicsContextCreator.hpp"

#include "SDLSoftwareGraphicsContext.hpp"

#include "JEngine3D/Platform/IGraphicsContext.hpp"// for IGraphicsContext

#include <cstring>// IWYU pragma: keep
#include <SDL_render.h>
#include <SDL_video.h>// for SDL_Window

namespace JE {

auto SDLSoftwareGraphicsContextCreator::CreateContext(IPlatformBackend::NativeWindowHandle handle)
  -> Scope<IGraphicsContext, MemoryTag::App>
{
  auto *renderer = SDL_CreateRenderer(static_cast<SDL_Window *>(handle), -1, SDL_RENDERER_ACCELERATED);

  return CreatePolymorphicScope<SDLSoftwareGraphicsContext, MemoryTag::App, IGraphicsContext>(
    handle, static_cast<IGraphicsContextCreator::NativeContextHandle>(renderer));
}

}// namespace JE