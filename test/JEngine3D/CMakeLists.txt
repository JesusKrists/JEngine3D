list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

include(${CMAKE_SOURCE_DIR}/CMake/DisableStaticAnalysis.cmake)

add_library(JEngine3D_catch_base INTERFACE)
target_link_system_libraries(JEngine3D_catch_base INTERFACE Catch2WithMain)
target_link_libraries(JEngine3D_catch_base INTERFACE JEngine3D)
target_include_directories(JEngine3D_catch_base INTERFACE ${CMAKE_SOURCE_DIR}/test)

include(UnitTest_Sources.cmake)

add_executable(JEngine3D_tests ${UNITTEST_SOURCES})
target_link_libraries(JEngine3D_tests PRIVATE project_warnings project_options JEngine3D_catch_base)
target_compile_definitions(JEngine3D_tests PRIVATE UNITTEST_BUILD)
disable_static_analysis(JEngine3D_tests)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  JEngine3D_tests
  TEST_PREFIX
  "unittests."
  REPORTER
  console)

add_executable(JEngine3D_Platform_tests ${UNITTEST_PLATFORM_SOURCES})
target_link_libraries(JEngine3D_Platform_tests PRIVATE project_warnings project_options JEngine3D_catch_base)
disable_static_analysis(JEngine3D_Platform_tests)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  JEngine3D_Platform_tests
  TEST_PREFIX
  "unittests."
  REPORTER
  console)

# Add a file containing a set of constexpr tests
add_executable(JEngine3D_constexpr_tests ${CONSTEXPR_SOURCES})
target_link_libraries(JEngine3D_constexpr_tests PRIVATE project_options project_warnings JEngine3D_catch_base)
disable_static_analysis(JEngine3D_constexpr_tests)

catch_discover_tests(
  JEngine3D_constexpr_tests
  TEST_PREFIX
  "constexpr."
  REPORTER
  console)

# Disable the constexpr portion of the test, and build again this allows us to have an executable that we can debug when
# things go wrong with the constexpr testing
add_executable(JEngine3D_relaxed_constexpr_tests ${CONSTEXPR_SOURCES})
target_link_libraries(JEngine3D_relaxed_constexpr_tests PRIVATE project_options project_warnings JEngine3D_catch_base)
target_compile_definitions(JEngine3D_relaxed_constexpr_tests PRIVATE -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE)
disable_static_analysis(JEngine3D_relaxed_constexpr_tests)

catch_discover_tests(
  JEngine3D_relaxed_constexpr_tests
  TEST_PREFIX
  "relaxed_constexpr."
  REPORTER
  console)

include(IntegrationTest_Sources.cmake)

add_executable(JEngine3D_Integration_tests ${INTEGRATIONTEST_SOURCES})
target_link_libraries(JEngine3D_Integration_tests PRIVATE project_warnings project_options JEngine3D_catch_base)
target_compile_definitions(JEngine3D_Integration_tests PRIVATE INTEGRATIONTEST_BUILD)
disable_static_analysis(JEngine3D_Integration_tests)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  JEngine3D_Integration_tests
  TEST_PREFIX
  "integrationtests."
  REPORTER
  console)
