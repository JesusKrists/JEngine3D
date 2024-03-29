include_guard(GLOBAL)

find_program(MEMORYCHECK_COMMAND NAMES valgrind)
set(MEMORYCHECK_SUPPRESSIONS_FILE
    "${CMAKE_SOURCE_DIR}/valgrind.supp"
    CACHE FILEPATH "File that contains suppressions for the memory checker")
set(MEMORYCHECK_COMMAND_OPTIONS
    "--leak-check=full --show-leak-kinds=definite,indirect,possible --error-exitcode=1 --errors-for-leak-kinds=definite,indirect,possible --track-origins=yes --keep-debuginfo=yes"
    CACHE STRING "valgrind command line options")

# Not ideal to use this global variable, but necessary to make sure
# that tooling and projects use the same version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_EXTENSIONS ON)

# Change this to false if you want to disable warnings_as_errors in developer mode
set(WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)
set(ENABLE_INCLUDE_WHAT_YOU_USE_DEVELOPER_DEFAULT FALSE)

# Fix MSVC toolchain stuff with this workaround (By default this is set to AMD64, which is incorrect)
if(MSVC)
  set(CMAKE_SYSTEM_PROCESSOR
      ${MSVC_CXX_ARCHITECTURE_ID}
      CACHE STRING "MSVC compiler path stuff" FORCE)
endif()

# Add project_options v0.20.0
# https://github.com/aminya/project_options
# Change the version in the following URL to update the package (watch the releases of the repository for future updates)
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.24.0.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)
include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)

set(GIT_SHA
    "Unknown"
    CACHE STRING "SHA this build was generated from")
string(
  SUBSTRING "${GIT_SHA}"
            0
            8
            GIT_SHORT_SHA)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE
      RelWithDebInfo
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS
             "Debug"
             "Release"
             "MinSizeRel"
             "RelWithDebInfo")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(OPT_ENABLE_SANITIZER_ADDRESS
      FALSE
      CACHE BOOL "Disable ASAN" FORCE)
endif()

# defaulted_project_options sets recommended defaults and provides user and developer
# modes and full GUI support for choosing options at configure time

# for more flexibility, look into project_options() macro

# Any default can be overridden
# set(<feature_name>_DEFAULT <value>) - set default for both user and developer modes
# set(<feature_name>_DEVELOPER_DEFAULT <value>) - set default for developer mode
# set(<feature_name>_USER_DEFAULT <value>) - set default for user mode

# TODO(JesusKrists) - Remove this once dynamic_project_options is fixed
if(OPT_ENABLE_COVERAGE)
  set(ENABLE_COVERAGE_VALUE ENABLE_COVERAGE)
else()
  unset(ENABLE_COVERAGE_VALUE)
endif()

# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment the options to enable them:
dynamic_project_options(
  # ENABLE_CACHE
  # ENABLE_CPPCHECK
  # ENABLE_CLANG_TIDY
  # ENABLE_CONAN
  # ENABLE_INTERPROCEDURAL_OPTIMIZATION
  # ENABLE_NATIVE_OPTIMIZATION
  # ENABLE_DOXYGEN
  ${ENABLE_COVERAGE_VALUE}
  # ENABLE_SANITIZER_ADDRESS
  # ENABLE_SANITIZER_LEAK
  # ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
  # ENABLE_SANITIZER_THREAD
  # ENABLE_SANITIZER_MEMORY
  # ENABLE_PCH
  # Note: PCH is disabled by default in developer mode because these headers become
  # globally included and they can mask other errors
  # PCH_HEADERS
  # <vector>
  # <string> # This is a list of headers to pre-compile, here are some common ones
  # WARNINGS_AS_ERRORS
  # ENABLE_INCLUDE_WHAT_YOU_USE
  # ENABLE_USER_LINKER
  # ENABLE_BUILD_WITH_TIME_TRACE
  # ENABLE_UNITY
  # CONAN_OPTIONS    # Extra options to pass to conan
  # MSVC_WARNINGS    # Override the defaults for the MSVC warnings
  # CLANG_WARNINGS   # Override the defaults for the CLANG warnings
  # GCC_WARNINGS     # Override the defaults for the GCC warnings
  CPPCHECK_OPTIONS # Override the defaults for CppCheck
  --enable=style,performance,warning,portability
  --inline-suppr
  # We cannot act on a bug/missing feature of cppcheck
  --suppress=internalAstError
  # if a file does not have an internalAstError, we get an unmatchedSuppression error
  --suppress=unmatchedSuppression
  --inconclusive
  # Custom suppresses added by me
  --suppress=toomanyconfigs
  --suppress=preprocessorErrorDirective
  --suppress=*:*/_deps/*)

target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
  if(NOT WIN32)
    target_compile_options(project_options INTERFACE -fno-omit-frame-pointer)

    if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*")
      target_compile_options(project_options INTERFACE -mno-omit-leaf-frame-pointer)
    endif()
  endif()
endif()

if(MSVC)
  target_compile_definitions(project_options INTERFACE _DISABLE_VECTOR_ANNOTATION)
endif()
