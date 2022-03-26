// This file will be generated automatically when you run the CMake configuration step.
// It creates a namespace called `JEngine3D`.
// You can modify the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>// for project_name, project_version

#include <docopt.h>// for docopt
#include <array>// for array
#include <cstdio>// for snprintf
#include <iterator>// for next
#include <string>// for string, allocator
#include <string_view>// for string_view


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
  static constexpr auto VERSION_STRING_BUFFER_SIZE = 32;
  std::array<char, VERSION_STRING_BUFFER_SIZE> versionStr{};
  // NOLINTNEXTLINE
  std::snprintf(versionStr.data(),
    VERSION_STRING_BUFFER_SIZE,
    "%s %s",
    JEditor::cmake::project_name.data(),
    JEditor::cmake::project_version.data());

  docopt::docopt(USAGE, { std::next(argv), std::next(argv, argc) }, true, { versionStr.data() });
}
