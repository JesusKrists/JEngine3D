// This file will be generated automatically when you run the CMake configuration step.
// You can modify the source template at `configured_files/config.hpp.in`.

/*#include <internal_use_only/config.hpp>// for project_name, project_version

#include <docopt.h>// for docopt
#include <fmt/core.h>// for format

#include <JEngine3D/Core/EntryPoint.hpp>
#include <JEngine3D/Core/Application.hpp>// for Application
#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Platform/IPlatformBackend.hpp>
#include <JEngine3D/Core/Types.hpp>// for string, basic_string
#include <JEngine3D/Core/WindowController.hpp>// for Window

#include "UILayer.hpp"

#include <iterator>// for next
#include <memory>// for unique_ptr
#include <vector>

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
int main(int argc, const char **argv)
{
  const auto versionString = fmt::format("{} {}", JEditor::cmake::project_name, JEditor::cmake::project_version);
  const auto args = std::vector<std::string>{ std::next(argv), std::next(argv, argc) };

  if (!args.empty()) {
    const auto options = docopt::docopt(USAGE, args, true, versionString);
    JE::UNUSED(options);
  } else {
    JEditor::UILayer uiLayer;

    auto engine = JE::CreateApplication(versionString);
    engine->MainWindow().SetSize(MAIN_WINDOW_SIZE);
    engine->MainWindow().SetPosition(JE::IPlatformBackend::WINDOW_CENTER_POSITION);

    engine->PushLayer(uiLayer);
    engine->Run();
  }
}
*/


#include <cr.h>

#include <JEngine3D/Core/Assert.hpp>
#include <JEngine3D/Core/Application.hpp>

#include "UILayer.hpp"

static JEditor::UILayer *s_UILayer = nullptr;// NOLINT

CR_EXPORT int cr_main([[maybe_unused]] cr_plugin *ctx, [[maybe_unused]] cr_op operation)// NOLINT
{
  ASSERT(ctx != nullptr, "No context passed!");

  switch (operation) {
  case CR_LOAD:
    JE::Logger::ClientLogger().debug("CR_LOAD: DLL Application address: {}", fmt::ptr(&JE_APP));
    s_UILayer = &JE_APP.PushLayer<JEditor::UILayer>();
    break;
  case CR_UNLOAD:
    JE::Logger::ClientLogger().debug("CR_UNLOAD: DLL Application address: {}", fmt::ptr(&JE_APP));
    JE_APP.PopLayer(*s_UILayer);
    break;
  case CR_CLOSE:
    JE::Logger::ClientLogger().debug("CR_CLOSE: DLL Application address: {}", fmt::ptr(&JE_APP));
    break;
  default:
    break;
  }

  return 0;
}