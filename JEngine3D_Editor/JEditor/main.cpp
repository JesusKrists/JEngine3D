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

#include "main.h"

#include <JEngine3D/Core/Application.hpp>

#include "JEditorState.hpp"
#include "UILayer.hpp"

namespace JEditor {

EditorState *EditorState::s_StateInstance = nullptr;// NOLINT
static UILayer *s_UILayer = nullptr;// NOLINT

}// namespace JEditor

auto cr_main(cr_plugin *ctx, cr_op operation) -> int
{
  ASSERT(ctx != nullptr, "No context passed!");

  switch (operation) {
  case CR_LOAD:
    JE::Logger::ClientLogger().debug("CR_LOAD: DLL Application address: {}", fmt::ptr(&JE_APP));

    JEditor::EditorState::s_StateInstance = ctx->CreateState<JEditor::EditorState>();
    JEditor::s_UILayer = &JE_APP.PushLayer<JEditor::UILayer>();

    break;
  case CR_UNLOAD:
    JE::Logger::ClientLogger().debug("CR_UNLOAD: DLL Application address: {}", fmt::ptr(&JE_APP));

    JE_APP.PopLayer(*JEditor::s_UILayer);
    JEditor::s_UILayer = nullptr;

    break;
  case CR_CLOSE:
    JE::Logger::ClientLogger().debug("CR_CLOSE: DLL Application address: {}", fmt::ptr(&JE_APP));

    ctx->DeleteState(JEditor::EditorState::s_StateInstance);

    break;
  default:
    break;
  }

  return 0;
}