include_guard(GLOBAL)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master)

FetchContent_MakeAvailable(stb)

set(stb_image_headers ${stb_SOURCE_DIR}/stb_image.h)

add_library(STBTarget INTERFACE ${stb_image_headers})
target_include_directories(STBTarget INTERFACE ${stb_SOURCE_DIR})
target_compile_definitions(STBTarget INTERFACE STB_IMAGE_IMPLEMENTATION)
