list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_library(JEngine3D_catch_main OBJECT catch_main.cpp)
target_link_system_libraries(
  JEngine3D_catch_main
  PUBLIC
  Catch2::Catch2
  SDL2-static)
target_link_libraries(
  JEngine3D_catch_main
  PRIVATE project_options project_warnings
  PUBLIC JEngine3D)

# Provide a simple smoke test to make sure that the CLI works and can display a --help message
#add_test(NAME cli.has_help COMMAND JEngine3D_Editor_Executable --help)

# Provide a test to verify that the version being reported from the application
# matches the version given to CMake. This will be important once you package
# your program. Real world shows that this is the kind of simple mistake that is easy
# to make, but also easy to test for.
#add_test(NAME cli.version_matches COMMAND JEngine3D_Editor_Executable --version)
#set_tests_properties(cli.version_matches PROPERTIES PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")

include(UnitTest_Sources.cmake)

add_executable(JEngine3D_tests tests.cpp ${UNITTEST_SOURCES})
target_link_libraries(
  JEngine3D_tests
  PRIVATE project_warnings
          project_options
          Catch2WithMain
          JEngine3D_catch_main)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  JEngine3D_tests
  TEST_PREFIX
  "unittests."
  REPORTER
  XML
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittests."
  OUTPUT_SUFFIX
  .xml)

# Add a file containing a set of constexpr tests
add_executable(JEngine3D_constexpr_tests constexpr_tests.cpp)
target_link_libraries(
  JEngine3D_constexpr_tests
  PRIVATE project_options
          project_warnings
          Catch2WithMain
          JEngine3D_catch_main)

catch_discover_tests(
  JEngine3D_constexpr_tests
  TEST_PREFIX
  "constexpr."
  REPORTER
  XML
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "constexpr."
  OUTPUT_SUFFIX
  .xml)

# Disable the constexpr portion of the test, and build again this allows us to have an executable that we can debug when
# things go wrong with the constexpr testing
add_executable(JEngine3D_relaxed_constexpr_tests constexpr_tests.cpp)
target_link_libraries(
  JEngine3D_relaxed_constexpr_tests
  PRIVATE project_options
          project_warnings
          Catch2WithMain
          JEngine3D_catch_main)
target_compile_definitions(JEngine3D_relaxed_constexpr_tests PRIVATE -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE)

catch_discover_tests(
  JEngine3D_relaxed_constexpr_tests
  TEST_PREFIX
  "relaxed_constexpr."
  REPORTER
  XML
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "relaxed_constexpr."
  OUTPUT_SUFFIX
  .xml)
