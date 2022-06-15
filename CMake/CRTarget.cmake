include_guard(GLOBAL)

FetchContent_Declare(
  cr
  GIT_REPOSITORY https://github.com/JesusKrists/cr.git
  GIT_TAG master)

FetchContent_Populate(cr)

set(cr_headers ${cr_SOURCE_DIR}/cr.h)

add_library(CRTarget INTERFACE ${cr_headers})
target_include_directories(CRTarget INTERFACE ${cr_SOURCE_DIR})
