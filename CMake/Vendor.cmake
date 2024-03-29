include_guard(GLOBAL)

include(${CMAKE_SOURCE_DIR}/CMake/DisableStaticAnalysis.cmake)

######################### docopt ######################################
FetchContent_Declare(
  docopt
  GIT_REPOSITORY https://github.com/docopt/docopt.cpp.git
  GIT_TAG v0.6.3)

FetchContent_MakeAvailable(docopt)
disable_static_analysis(docopt)
disable_static_analysis(docopt_s)
if(NOT MSVC AND NOT XCODE)
  disable_static_analysis(docopt_o)
endif()

add_library(docopt::docopt ALIAS docopt_s)

########################## fmt ########################################
set(BUILD_SHARED_LIBS_BEFORE ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS ON)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 9.0.0)

FetchContent_MakeAvailable(fmt)
disable_static_analysis(fmt)

set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_BEFORE})

########################## spdlog ######################################
set(SPDLOG_FMT_EXTERNAL
    ON
    CACHE BOOL "Enable external fmtlib" FORCE)
set(SPDLOG_BUILD_SHARED
    ON
    CACHE BOOL "Enable shared library" FORCE)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.10.0)

FetchContent_MakeAvailable(spdlog)
disable_static_analysis(spdlog)

########################## SDL2 ######################################
set(SDL_STATIC_ENABLED_BY_DEFAULT
    OFF
    CACHE BOOL "Disable static build" FORCE)
set(SDL_STATIC
    OFF
    CACHE BOOL "Disable static build" FORCE)
set(SDL2_DISABLE_SDL2MAIN
    ON
    CACHE BOOL "Disable SDL2main build" FORCE)
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
disable_static_analysis(SDL2)

target_compile_definitions(SDL2 PUBLIC SDL_STATIC_LIB)

if(MSVC)
  if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    target_link_libraries(SDL2 PRIVATE msvcrtd ucrtd vcruntimed)
  else()
    target_link_libraries(SDL2 PRIVATE msvcrt ucrt vcruntime)
  endif()
endif()

############################## ImGui #############################################

include(${CMAKE_SOURCE_DIR}/CMake/ImGuiTarget.cmake)

############################## glew #############################################

set(glew-cmake_BUILD_SHARED
    OFF
    CACHE BOOL "Disable shared build of glew" FORCE)

FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG glew-cmake-2.2.0)

FetchContent_MakeAvailable(glew)
disable_static_analysis(libglew_static)

################################# OpenGL Mathematics (glm) #####################################################

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8)

FetchContent_MakeAvailable(glm)
disable_static_analysis(glm)

################################# STB Image #####################################################################

include(${CMAKE_SOURCE_DIR}/CMake/STBTarget.cmake)

################################# Tracy profiler ##################################################################
if(NOT ENABLE_TRACY_PROFILING)
  set(TRACY_ENABLE
      OFF
      CACHE BOOL "Disable inline callstack functions" FORCE)
endif()

set(TRACY_NO_CALLSTACK_INLINES
    ON
    CACHE BOOL "Disable inline callstack functions" FORCE)
#set(TRACY_NO_SYS_TRACE
#    ON
#    CACHE BOOL "Disable system trace sampling" FORCE)
set(TRACY_NO_VSYNC_CAPTURE
    ON
    CACHE BOOL "Disable hw vsync event capture" FORCE)
set(TRACY_NO_FRAME_IMAGE
    ON
    CACHE BOOL "Disable frame image support" FORCE)
#set(TRACY_NO_EXIT
#    ON
#    CACHE BOOL "Disable client exit until all data is sent" FORCE)
set(TRACY_ONLY_IPV4
    ON
    CACHE BOOL "Disable IPv6" FORCE)
set(TRACY_ONLY_LOCALHOST
    ON
    CACHE BOOL "Only localhost network interface" FORCE)

set(BUILD_SHARED_LIBS_BEFORE ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS ON)

FetchContent_Declare(
  tracy
  GIT_REPOSITORY https://github.com/JesusKrists/tracy.git
  GIT_TAG master)

FetchContent_MakeAvailable(tracy)
disable_static_analysis(TracyClient)
#target_compile_definitions(TracyClient PUBLIC TRACY_NO_SYSTEM_TRACING)

set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_BEFORE})

if(ENABLE_TRACY_PROFILING)
  include(${CMAKE_SOURCE_DIR}/CMake/TracyServerTarget.cmake)
endif()

################################ cr (Live reload) ###############################################

include(${CMAKE_SOURCE_DIR}/CMake/CRTarget.cmake)

################################ Font icons headers #############################################

include(${CMAKE_SOURCE_DIR}/CMake/FontIconsTarget.cmake)

################################ SVG Renderer #####################################################

FetchContent_Declare(
  lunasvg
  GIT_REPOSITORY https://github.com/JesusKrists/lunasvg.git
  GIT_TAG master)

FetchContent_MakeAvailable(lunasvg)
disable_static_analysis(lunasvg)

################################# entt (ECS) ###################################################3

FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG v3.10.1)

FetchContent_MakeAvailable(entt)
disable_static_analysis(EnTT)
