include_guard(GLOBAL)

FetchContent_Declare(
  fonticons
  GIT_REPOSITORY https://github.com/juliettef/IconFontCppHeaders.git
  GIT_TAG main)

FetchContent_MakeAvailable(fonticons)

add_library(FontIcons INTERFACE)
target_include_directories(FontIcons INTERFACE ${fonticons_SOURCE_DIR})
