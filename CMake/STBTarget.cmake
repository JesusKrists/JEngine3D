include_guard(GLOBAL)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master)

FetchContent_MakeAvailable(stb)

add_library(STBTarget INTERFACE)

target_include_directories(STBTarget INTERFACE ${stb_SOURCE_DIR})
target_compile_definitions(STBTarget INTERFACE STB_IMAGE_IMPLEMENTATION)
