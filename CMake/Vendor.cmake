######################### docopt ######################################
FetchContent_Declare(
  docopt
  GIT_REPOSITORY https://github.com/docopt/docopt.cpp.git
  GIT_TAG v0.6.3)

FetchContent_MakeAvailable(docopt)
add_library(docopt::docopt ALIAS docopt_s)

########################## spdlog ######################################
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.9.2)

FetchContent_MakeAvailable(spdlog)

########################## fmt ########################################
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 8.1.1)

FetchContent_MakeAvailable(fmt)

########################## SDL2 ######################################
set(SDL_SHARED_ENABLED_BY_DEFAULT
    OFF
    CACHE BOOL "Disable shared build of SDL2" FORCE)
set(SDL_AUDIO
    OFF
    CACHE BOOL "Disable audio subsystem" FORCE)
set(SDL_POWER
    OFF
    CACHE BOOL "Disable power subsystem" FORCE)
set(SDL_FILESYSTEM
    OFF
    CACHE BOOL "Disable filesystem subsystem" FORCE)
set(SDL_SENSOR
    OFF
    CACHE BOOL "Disable sensor subsystem" FORCE)
set(SDL_LOCALE
    OFF
    CACHE BOOL "Disable locale subsystem" FORCE)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.0.20)

FetchContent_MakeAvailable(SDL2)

######################### Remove static analysis tools ##################
set_property(TARGET docopt PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET docopt PROPERTY C_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET docopt_s PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET docopt_s PROPERTY C_INCLUDE_WHAT_YOU_USE "")
if(NOT MSVC AND NOT XCODE)
  set_property(TARGET docopt_o PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
  set_property(TARGET docopt_o PROPERTY C_INCLUDE_WHAT_YOU_USE "")
endif()

set_property(TARGET spdlog PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET spdlog PROPERTY C_INCLUDE_WHAT_YOU_USE "")

set_property(TARGET fmt PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET fmt PROPERTY C_INCLUDE_WHAT_YOU_USE "")

set_property(TARGET SDL2main PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET SDL2main PROPERTY C_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET SDL2-static PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET SDL2-static PROPERTY C_INCLUDE_WHAT_YOU_USE "")
#
set_property(TARGET docopt PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET docopt PROPERTY C_CLANG_TIDY "")
set_property(TARGET docopt_s PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET docopt_s PROPERTY C_CLANG_TIDY "")
if(NOT MSVC AND NOT XCODE)
  set_property(TARGET docopt_o PROPERTY CXX_CLANG_TIDY "")
  set_property(TARGET docopt_o PROPERTY C_CLANG_TIDY "")
endif()

set_property(TARGET spdlog PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET spdlog PROPERTY C_CLANG_TIDY "")

set_property(TARGET fmt PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET fmt PROPERTY C_CLANG_TIDY "")

set_property(TARGET SDL2main PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET SDL2main PROPERTY C_CLANG_TIDY "")
set_property(TARGET SDL2-static PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET SDL2-static PROPERTY C_CLANG_TIDY "")
#
set_property(TARGET docopt PROPERTY CXX_CPPCHECK "")
set_property(TARGET docopt PROPERTY C_CPPCHECK "")
set_property(TARGET docopt_s PROPERTY CXX_CPPCHECK "")
set_property(TARGET docopt_s PROPERTY C_CPPCHECK "")
if(NOT MSVC AND NOT XCODE)
  set_property(TARGET docopt_o PROPERTY CXX_CPPCHECK "")
  set_property(TARGET docopt_o PROPERTY C_CPPCHECK "")
endif()

set_property(TARGET spdlog PROPERTY CXX_CPPCHECK "")
set_property(TARGET spdlog PROPERTY C_CPPCHECK "")

set_property(TARGET fmt PROPERTY CXX_CPPCHECK "")
set_property(TARGET fmt PROPERTY C_CPPCHECK "")

set_property(TARGET SDL2main PROPERTY CXX_CPPCHECK "")
set_property(TARGET SDL2main PROPERTY C_CPPCHECK "")
set_property(TARGET SDL2-static PROPERTY CXX_CPPCHECK "")
set_property(TARGET SDL2-static PROPERTY C_CPPCHECK "")
