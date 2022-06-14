cmake_minimum_required(VERSION 3.16)

##############################################################################

set(ENABLE_DEBUG_BREAK
    TRUE
    CACHE BOOL "Enable DEBUGBREAK to break in debugger")

set(ENABLE_TESTING
    TRUE
    CACHE BOOL "Enable the tests")

set(ENABLE_FUZZING
    FALSE
    CACHE BOOL "Enable the fuzz tests")

# Note: by default ENABLE_DEVELOPER_MODE is True
# This means that all analysis (sanitizers, static analysis)
# is enabled and all warnings are treated as errors
set(ENABLE_DEVELOPER_MODE
    TRUE
    CACHE BOOL "Enable 'developer mode'")

####################################################################################

# Set the project name and language
project(
  JEngine3D
  VERSION 0.0.1
  DESCRIPTION "Custom game engine JEngine3D"
  HOMEPAGE_URL "https://github.com/JesusKrists/JEngine3D"
  LANGUAGES CXX C)

######################################################################################
include(CMake/StandardSetup.cmake)
######################################################################################

# Add 3rd party libraries (SDL2, imgui, etc.)
include(CMake/Vendor.cmake)

# Adding the src:
add_subdirectory(JEngine3D)
add_subdirectory(JEngine3D_Editor)

# Adding the tests:
if(ENABLE_TESTING)
  include(CMake/SetupCatch2.cmake)
  enable_testing()

  add_subdirectory(test/JEngine3D)
  #add_subdirectory(test/JEngine3D_Editor)
endif()

if(ENABLE_FUZZING AND ENABLE_TESTING)
  message("Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
  add_subdirectory(fuzz_test)
endif()

# If MSVC is being used, and ASAN is enabled, we need to set the debugger environment
# so that it behaves well with MSVC's debugger, and we can run the target from visual studio
if(MSVC)
  get_all_targets(all_targets)
  set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT JEngine3D_Editor_Executable)

#######################################################################################################################################

# Add other targets that you want installed here, be default we just package the one executable
# we know we want to ship
#package_project(
#  TARGETS
#  JEngine3D
#  JEngine3D_Editor_Executable
#  project_options
#  project_warnings
####################
#  docopt_s)

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
#set(CPACK_PACKAGE_FILE_NAME
#    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
#)

#include(CPack)
