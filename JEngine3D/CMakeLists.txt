# Set the project name and language
project(
  JEngine3D
  VERSION 0.0.1
  DESCRIPTION "Custom game engine JEngine3D"
  HOMEPAGE_URL "https://github.com/JesusKrists/JEngine3D"
  LANGUAGES CXX C)

# configure files based on CMake configuration options
add_subdirectory(configured_files)

add_library(Jengine3D main.cpp)
target_link_libraries(
  Jengine3D
  PRIVATE project_options
          project_warnings
          docopt::docopt
          fmt::fmt
          spdlog::spdlog)

target_include_directories(Jengine3D PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
