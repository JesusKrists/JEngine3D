include_guard(GLOBAL)

FetchContent_Declare(
  cr
  GIT_REPOSITORY https://github.com/JesusKrists/cr.git
  GIT_TAG master)

FetchContent_Populate(cr)

add_library(CRTarget INTERFACE)
target_include_directories(CRTarget INTERFACE ${cr_SOURCE_DIR})
