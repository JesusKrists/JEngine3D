include_guard(GLOBAL)

include(${CMAKE_SOURCE_DIR}/CMake/DisableStaticAnalysis.cmake)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.0.1)

FetchContent_MakeAvailable(Catch2)
disable_static_analysis(Catch2)
disable_static_analysis(Catch2WithMain)

add_library(Catch2JEngine3DMain STATIC ${CMAKE_SOURCE_DIR}/test/TestBase/main.cpp)

target_link_libraries(
  Catch2JEngine3DMain
  PRIVATE project_options
  PUBLIC Catch2::Catch2)

disable_static_analysis(Catch2JEngine3DMain)
