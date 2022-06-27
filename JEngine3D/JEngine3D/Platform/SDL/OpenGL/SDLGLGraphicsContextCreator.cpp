#include "SDLGLGraphicsContextCreator.hpp"

#include "JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContext.hpp"
#include "JEngine3D/Platform/IGraphicsContext.hpp"// for IGraphics...

#include <exception>// for exception
#include <cstring>// IWYU pragma: keep
#include <SDL_video.h>// for SDL_GL_Cr...
#include <SDL_error.h>
#include <SDL_stdinc.h>

#include <GL/glew.h>

namespace JE {

static bool s_GLEWInitialized = false;// NOLINT

auto SDLGLGraphicsContextCreator::CreateContext(IPlatformBackend::NativeWindowHandle handle)
  -> Scope<IGraphicsContext, MemoryTag::App>
{
  auto *previousContext = IGraphicsContext::CurrentContext();


  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
#if defined(JE_PLATFORM_APPLE)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);// NOLINT
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);// NOLINT
#endif
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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