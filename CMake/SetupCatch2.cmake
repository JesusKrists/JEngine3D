include_guard(GLOBAL)

include(${CMAKE_SOURCE_DIR}/CMake/DisableStaticAnalysis.cmake)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.0.0-preview4)

FetchContent_MakeAvailable(Catch2)

disable_static_analysis(Catch2)
disable_static_analysis(Catch2WithMain)
