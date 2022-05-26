include_guard(GLOBAL)

include(${CMAKE_SOURCE_DIR}/CMake/DisableStaticAnalysis.cmake)

######################### docopt ######################################
FetchContent_Declare(
  docopt
  GIT_REPOSITORY https://github.com/docopt/docopt.cpp.git
  GIT_TAG v0.6.3)

FetchContent_MakeAvailable(docopt)
add_library(docopt::docopt ALIAS docopt_s)

disable_static_analysis(docopt)
disable_static_analysis(docopt_s)
if(NOT MSVC AND NOT XCODE)
  disable_static_analysis(docopt_o)
endif()

########################## fmt ########################################
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 8.1.1)

FetchContent_MakeAvailable(fmt)

disable_static_analysis(fmt)

########################## spdlog ######################################
set(SPDLOG_FMT_EXTERNAL
    ON
    CACHE BOOL "Enable external fmtlib" FORCE)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.10.0)

FetchContent_MakeAvailable(spdlog)

disable_static_analysis(spdlog)

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
  GIT_TAG release-2.0.22)

FetchContent_MakeAvailable(SDL2)

disable_static_analysis(SDL2main)
disable_static_analysis(SDL2-static)

############################## ImGui #############################################

include(${CMAKE_SOURCE_DIR}/CMake/ImGuiTarget.cmake)

############################## glew #############################################

FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG glew-cmake-2.2.0)

FetchContent_MakeAvailable(glew)

disable_static_analysis(libglew_static)
disable_static_analysis(libglew_shared)

################################# ImGui Software renderer ####################################################

include(${CMAKE_SOURCE_DIR}/CMake/ImGuiSoftwareRenderer.cmake)

################################# OpenGL Mathematics (glm) #####################################################

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8)

FetchContent_MakeAvailable(glm)

disable_static_analysis(glm)

################################# STB Image #####################################################

include(${CMAKE_SOURCE_DIR}/CMake/STBTarget.cmake)
