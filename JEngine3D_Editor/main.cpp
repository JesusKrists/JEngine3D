// This file will be generated automatically when you run the CMake configuration step.
// You can modify the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>// for project_name, project_version

#include <iterator>// for next
#include <string>// for string, allocator
#include <memory>// for unique_ptr
#include <vector>

#include <docopt.h>// for docopt
#include <fmt/core.h>// for format

#include <JEngine3D/Core/EntryPoint.hpp>
#include <JEngine3D/Core/Application.hpp>// for Application
#include <JEngine3D/Core/Base.hpp>// for UNUSED

static constexpr auto USAGE =
  R"(JEngine3D Editor.

      Usage:
        None at the moment // TODO(JesusKrists) - Implement command line interface for editor to open projects etc.
        
      Options:
        -h --help     Show this screen.
        --version     Show version.
  )";

// NOLINTNEXTLINE
int main(int argc, const char **argv)
{
  const auto versionString = fmt::format("{} {}", JEditor::cmake::project_name, JEditor::cmake::project_version);
  const auto args = std::vector<std::string>{ std::next(argv), std::next(argv, argc) };

  if (!args.empty()) {
    const auto options = docopt::docopt(USAGE, args, true, versionString);
    JE::UNUSED(options);
  }

  auto engine = JE::CreateApplication(versionString);
  engine->Run();
}
