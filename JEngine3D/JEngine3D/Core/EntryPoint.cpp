#include <internal_use_only/config.hpp>// for project_name, project_version

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/WindowController.hpp"
#include "JEngine3D/Core/InputController.hpp"
#include "JEngine3D/Platform/SDL/SDLPlatform.hpp"
#include "JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContextCreator.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLRendererObjectCreator.hpp"

#include <docopt.h>// for docopt


[[maybe_unused]] static constexpr auto USAGE =
R"(JEngine3D Game Engine.

      Usage:
        JEngine3D_Executable [--test]
        JEngine3D_Executable (-h | --help)
        JEngine3D_Executable --version
        
      Options:
        -h --help     Show this screen.
        --version     Show version.
        --test        Run JEngine3D in test mode
  )";// TODO(JesusKrists) - Implement command line interface for editor to open projects etc.

static constexpr auto MAIN_WINDOW_SIZE = JE::Size2DI{ 1600, 900 };

namespace JE {

    using Backend                = SDLPlatform;
    using GraphicsContextCreator = SDLGLGraphicsContextCreator;

    using RendererObjectCreator = OpenGLRendererObjectCreator;

}// namespace JE

// NOLINTNEXTLINE
int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
    const auto versionString = fmt::format("{} {}", JE::cmake::project_name, JE::cmake::project_version);
    const auto options = docopt::docopt(USAGE, std::vector<std::string>{ std::next(argv), std::next(argv, argc) }, true, versionString);

    bool testMode = [&options]() {
        auto it = options.find("--test");// NOLINT
        return it->second.asBool();
    }();

    JE::MemoryController s_MemoryController;
    JE::LoggerController s_LoggerController;

    JE::Backend                s_PlatformBackend;
    JE::GraphicsContextCreator s_GraphicsContextCreator;
    JE::RendererObjectCreator  s_RendererObjectCreator;

    JE::WindowController s_WindowController;
    JE::InputController  s_InputController;

    JE::Application engine{ versionString, testMode };
    engine.MainWindow().SetSize(MAIN_WINDOW_SIZE);
    engine.MainWindow().SetPosition(JE::IPlatform::WINDOW_CENTER_POSITION);
    engine.Run();
}