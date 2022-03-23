//#include <functional>
//#include <iostream>

//#include <docopt.h>
//#include <spdlog/spdlog.h>

// This file will be generated automatically when you run the CMake configuration step.
// It creates a namespace called `JEngine3D`.
// You can modify the source template at `configured_files/config.hpp.in`.
//#include <internal_use_only/config.hpp>


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
int main_copy([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
  /*try {
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
      { std::next(argv), std::next(argv, argc) },
      true,// show help if requested
      fmt::format("{} {}",
        JEditor::cmake::project_name,
        JEditor::cmake::project_version));// version string, acquired from config.hpp via CMake

    for (auto const &arg : args) { std::cout << arg.first << "=" << arg.second << '\n'; }


    // Use the default logger (stdout, multi-threaded, colored)
    spdlog::info("Hello, {}!", "World");

    fmt::print("Hello, from {}\n", "{fmt}");
  } catch (const std::exception &e) {
    fmt::print("Unhandled exception in main: {}", e.what());
  }
*/
  return 0;
}
