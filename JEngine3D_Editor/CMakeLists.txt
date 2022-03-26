# Set the project name and language
project(
  JEngine3D_Editor
  VERSION 0.0.1
  DESCRIPTION "Custom game engine JEngine3D Editor App"
  HOMEPAGE_URL "https://github.com/JesusKrists/JEngine3D"
  LANGUAGES CXX C)

# configure files based on CMake configuration options
add_subdirectory(configured_files)

add_library(JEngine3D_Editor temp.cpp)
target_link_libraries(
  JEngine3D_Editor
  PRIVATE project_options project_warnings
  PUBLIC JEngine3D)

add_executable(JEngine3D_Editor_Executable main.cpp)
target_link_libraries(JEngine3D_Editor_Executable PRIVATE project_options project_warnings JEngine3D_Editor)
target_link_system_libraries(JEngine3D_Editor_Executable PRIVATE docopt::docopt)

target_include_directories(JEngine3D_Editor
                           PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/configured_files/configured_files/include")
target_include_directories(JEngine3D_Editor_Executable
                           PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/configured_files/configured_files/include")
