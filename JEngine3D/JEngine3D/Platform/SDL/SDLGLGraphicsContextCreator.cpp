#include "SDLGLGraphicsContextCreator.hpp"

#include "JEngine3D/Platform/SDL/SDLGLGraphicsContext.hpp"
#include "JEngine3D/Platform/IGraphicsContext.hpp"// for IGraphics...
#include "JEngine3D/Core/LoggerController.hpp"// for Logger

#include <exception>// for exception
#include <cstring>// IWYU pragma: keep
#include <SDL_video.h>// for SDL_GL_Cr...

#include <GL/glew.h>

namespace JE {

static bool s_GLEWInitialized = false;// NOLINT

auto SDLGLGraphicsContextCreator::CreateContext(IPlatformBackend::NativeWindowHandle handle)
  -> Scope<IGraphicsContext, MemoryTag::App>
{
  auto *currentWindow = SDL_GL_GetCurrentWindow();
  auto *currentContext = SDL_GL_GetCurrentContext();

  SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
  auto *context = SDL_GL_CreateContext(static_cast<SDL_Window *>(handle));

  if (!s_GLEWInitialized) {
    glewExperimental = GL_TRUE;
    auto success = glewInit();
    if (success != GLEW_OK) {
      Logger::CoreLogger().error("GLEW failed to initialize - {}",
        reinterpret_cast<const char *>(// NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
          glewGetErrorString(success)));
    }

    s_GLEWInitialized = true;
  }

  SDL_GL_MakeCurrent(currentWindow, currentContext);
  return CreatePolymorphicScope<SDLGLGraphicsContext, MemoryTag::App, IGraphicsContext>(handle, context);
}

}// namespace JE