#include "SDLGLGraphicsContextCreator.hpp"

#include "JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContext.hpp"
#include "JEngine3D/Platform/IGraphicsContext.hpp"// for IGraphics...
#include "JEngine3D/Core/LoggerController.hpp"// for Logger

#include <JEngine3D/Core/Base.hpp>
#include <exception>// for exception
#include <cstring>// IWYU pragma: keep
#include <SDL_video.h>// for SDL_GL_Cr...

#include <GL/glew.h>

namespace JE {

static bool s_GLEWInitialized = false;// NOLINT

auto SDLGLGraphicsContextCreator::CreateContext(IPlatformBackend::NativeWindowHandle handle)
  -> Scope<IGraphicsContext, MemoryTag::App>
{
  auto *previousContext = IGraphicsContext::CurrentContext();

  SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, SDL_TRUE);
  auto *glContext = SDL_GL_CreateContext(static_cast<SDL_Window *>(handle));
  if (glContext == nullptr) {
    JE::Logger::CoreLogger().error("SDL Failed to initialize OpenGL Context - {}", SDL_GetError());
    DEBUGBREAK();
  }
  SDL_GL_MakeCurrent(static_cast<SDL_Window *>(handle), glContext);
  SDL_GL_SetSwapInterval(0);

  if (!s_GLEWInitialized) {
    glewExperimental = GL_TRUE;
    auto success = glewInit();
    if (success != GLEW_OK) {
      Logger::CoreLogger().error("GLEW failed to initialize - {}",
        reinterpret_cast<const char *>(// NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
          glewGetErrorString(success)));

      DEBUGBREAK();
    }

    s_GLEWInitialized = true;
  }

  if (previousContext != nullptr) { previousContext->MakeCurrent(); }

  return CreatePolymorphicScope<SDLGLGraphicsContext, MemoryTag::App, IGraphicsContext>(handle, glContext);
}

}// namespace JE