# configure files based on CMake configuration options
add_subdirectory(configured_files)

add_library(JEngine3D main.cpp)
target_link_libraries(JEngine3D PRIVATE project_options project_warnings)
target_link_system_libraries(
  JEngine3D
  PRIVATE
  docopt::docopt
  spdlog::spdlog)

target_include_directories(JEngine3D PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/configured_files/configured_files/include")
