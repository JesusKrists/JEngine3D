// This file will be generated automatically when you run the CMake configuration step.
// It creates a namespace called `JEngine3D`.
// You can modify the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

#include <array>// for array
#include <cstdio>// for snprintf
#include <docopt.h>// for docopt
#include <docopt_value.h>// for operator<<, value
#include <iostream>// for operator<<, basic_ostream
#include <iterator>// for next
#include <map>// for map, operator==, _Rb_tree_it...
#include <string>// for string, allocator, operator<<
#include <string_view>// for string_view
#include <utility>// for pair


[[maybe_unused]] static constexpr auto USAGE =
  R"(Naval Fate.

    Usage:
          naval_fate ship new <name>...
          naval_fate ship <name> move <x> <y> [--speed=<kn>]
          naval_fate ship shoot <x> <y>
          naval_fate mine (set|remove) <x> <y> [--moored | --drifting]
          naval_fate (-h | --help)
          naval_fate --version
 Options:
          -h --help     Show this screen.
          --version     Show version.
          --speed=<kn>  Speed in knots [default: 10].
          --moored      Moored (anchored) mine.
          --drifting    Drifting mine.
)";

// NOLINTNEXTLINE
int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
  static constexpr auto VERSION_STRING_BUFFER_SIZE = 32;
  std::array<char, VERSION_STRING_BUFFER_SIZE> versionStr{};
  // NOLINTNEXTLINE
  std::snprintf(versionStr.data(),
    VERSION_STRING_BUFFER_SIZE,
    "%s %s",
    JEditor::cmake::project_name.data(),
    JEditor::cmake::project_version.data());

  [[maybe_unused]] auto args = docopt::docopt(USAGE,
    { std::next(argv), std::next(argv, argc) },
    true,// show help if requested
    { versionStr.data() });// version string, acquired from config.hpp via CMake

  (void)args;
}
