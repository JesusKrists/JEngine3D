FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.0.0-preview4)

FetchContent_MakeAvailable(Catch2)
set_property(TARGET Catch2 PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET Catch2 PROPERTY C_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET Catch2WithMain PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET Catch2WithMain PROPERTY C_INCLUDE_WHAT_YOU_USE "")
#
set_property(TARGET Catch2 PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET Catch2 PROPERTY C_CLANG_TIDY "")
set_property(TARGET Catch2WithMain PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET Catch2WithMain PROPERTY C_CLANG_TIDY "")
#
set_property(TARGET Catch2 PROPERTY CXX_CPPCHECK "")
set_property(TARGET Catch2 PROPERTY C_CPPCHECK "")
set_property(TARGET Catch2WithMain PROPERTY CXX_CPPCHECK "")
set_property(TARGET Catch2WithMain PROPERTY C_CPPCHECK "")
