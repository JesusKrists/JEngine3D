list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_library(JEngine3D_Editor_catch_base INTERFACE)
target_link_system_libraries(JEngine3D_Editor_catch_base INTERFACE Catch2WithMain)
target_link_libraries(JEngine3D_Editor_catch_base INTERFACE JEngine3D_Editor)
target_include_directories(JEngine3D_Editor_catch_base INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

include(UnitTest_Sources.cmake)

# Provide a simple smoke test to make sure that the CLI works and can display a --help message
add_test(NAME cli.has_help COMMAND JEngine3D_Editor_Executable --help)

# Provide a test to verify that the version being reported from the application
# matches the version given to CMake. This will be important once you package
# your program. Real world shows that this is the kind of simple mistake that is easy
# to make, but also easy to test for.
add_test(NAME cli.version_matches COMMAND JEngine3D_Editor_Executable --version)
set_tests_properties(cli.version_matches PROPERTIES PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")

add_executable(JEngine3D_Editor_tests ${UNITTEST_SOURCES})
target_link_libraries(JEngine3D_Editor_tests PRIVATE project_warnings project_options JEngine3D_Editor_catch_base)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  JEngine3D_Editor_tests
  TEST_PREFIX
  "unittests."
  REPORTER
  console)

# Add a file containing a set of constexpr tests
add_executable(JEngine3D_Editor_constexpr_tests ${CONSTEXPR_SOURCES})
target_link_libraries(JEngine3D_Editor_constexpr_tests PRIVATE project_options project_warnings
                                                               JEngine3D_Editor_catch_base)

catch_discover_tests(
  JEngine3D_Editor_constexpr_tests
  TEST_PREFIX
  "constexpr."
  REPORTER
  console)

# Disable the constexpr portion of the test, and build again this allows us to have an executable that we can debug when
# things go wrong with the constexpr testing
add_executable(JEngine3D_Editor_relaxed_constexpr_tests ${CONSTEXPR_SOURCES})
target_link_libraries(JEngine3D_Editor_relaxed_constexpr_tests PRIVATE project_options project_warnings
                                                                       JEngine3D_Editor_catch_base)
target_compile_definitions(JEngine3D_Editor_relaxed_constexpr_tests PRIVATE -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE)

catch_discover_tests(
  JEngine3D_Editor_relaxed_constexpr_tests
  TEST_PREFIX
  "relaxed_constexpr."
  REPORTER
  console)
