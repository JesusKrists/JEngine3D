#include <internal_use_only/config.hpp>// for project_name, project_version

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/LoggerController.hpp"
#include "JEngine3D/Core/WindowController.hpp"
#include "JEngine3D/Core/InputController.hpp"
#include "JEngine3D/Platform/SDL/SDLPlatformBackend.hpp"
#include "JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContextCreator.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLRendererObjectCreator.hpp"

#include <Tracy.hpp>


#include <cr.h>

namespace JE {

using Backend = SDLPlatformBackend;
using GraphicsContextCreator = SDLGLGraphicsContextCreator;

using RendererObjectCreator = OpenGLRendererObjectCreator;

inline auto CreateApplication(const std::string_view &title) -> Scope<Application, MemoryTag::App>
{
  ZoneScoped;// NOLINT

  static bool s_EngineInitialized = false;
  ASSERT(!s_EngineInitialized, "Application already created");

  static MemoryController s_MemoryController;
  static LoggerController s_LoggerController;

  static Backend s_PlatformBackend;
  static GraphicsContextCreator s_GraphicsContextCreator;
  static RendererObjectCreator s_RendererObjectCreator;

  static WindowController s_WindowController;
  static InputController s_InputController;

  s_EngineInitialized = true;

  return CreateScope<Application, MemoryTag::App>(title);
}

}// namespace JE


static constexpr auto USAGE =
  R"(JEngine3D Editor.

      Usage:
        None at the moment // TODO(JesusKrists) - Implement command line interface for editor to open projects etc.
        
      Options:
        -h --help     Show this screen.
        --version     Show version.
  )";

static constexpr auto MAIN_WINDOW_SIZE = JE::Size2DI{ 1600, 900 };

// NOLINTNEXTLINE
int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
  const auto versionString = fmt::format("{} {}", JE::cmake::project_name, JE::cmake::project_version);

  auto engine = JE::CreateApplication(versionString);
  engine->MainWindow().SetSize(MAIN_WINDOW_SIZE);
  engine->MainWindow().SetPosition(JE::IPlatformBackend::WINDOW_CENTER_POSITION);

  cr_plugin ctx{};
  // the full path to the live-reloadable application
  cr_plugin_open(ctx, "/workspaces/JEngine3D/out/build/JEngine3D_Editor/libJEngine3D_Editor.so");

  // call the update function at any frequency matters to you, this will give
  // the real application a chance to run
  cr_plugin_update(ctx);


  engine->Run();

  // at the end do not forget to cleanup the plugin context
  cr_plugin_close(ctx);
}