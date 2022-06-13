include_guard(GLOBAL)

include(${CMAKE_SOURCE_DIR}/CMake/DisableStaticAnalysis.cmake)

set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE BOOL "Disable GLFW examples" FORCE)

set(GLFW_BUILD_TESTS
    OFF
    CACHE BOOL "Disable GLFW examples" FORCE)

set(GLFW_BUILD_DOCS
    OFF
    CACHE BOOL "Disable GLFW examples" FORCE)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.7)

FetchContent_MakeAvailable(glfw)
disable_static_analysis(glfw)

FetchContent_Declare(
  capstone
  GIT_REPOSITORY https://github.com/capstone-engine/capstone.git
  GIT_TAG next)

FetchContent_MakeAvailable(capstone)
disable_static_analysis(capstone)

set(TracyServer_SOURCES
    ${tracy_SOURCE_DIR}/profiler/src/HttpRequest.cpp
    ${tracy_SOURCE_DIR}/profiler/src/imgui_impl_glfw.cpp
    ${tracy_SOURCE_DIR}/profiler/src/imgui_impl_opengl3.cpp
    ${tracy_SOURCE_DIR}/profiler/src/main.cpp
    ${tracy_SOURCE_DIR}/profiler/src/NativeWindow.cpp
    ${tracy_SOURCE_DIR}/profiler/src/ResolvService.cpp)

set(TracyServer_HEADERS
    ${tracy_SOURCE_DIR}/profiler/src/HttpRequest.hpp
    ${tracy_SOURCE_DIR}/profiler/src/imgui_impl_glfw.h
    ${tracy_SOURCE_DIR}/profiler/src/imgui_impl_opengl3.h
    ${tracy_SOURCE_DIR}/profiler/src/imgui_impl_opengl3_loader.h
    ${tracy_SOURCE_DIR}/profiler/src/NativeWindow.hpp
    ${tracy_SOURCE_DIR}/profiler/src/ResolvService.hpp
    ${tracy_SOURCE_DIR}/profiler/src/stb_image.h)

set(TracyServer_ImGui
    ${tracy_SOURCE_DIR}/imgui/imgui.cpp
    ${tracy_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${tracy_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${tracy_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${tracy_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${tracy_SOURCE_DIR}/imgui/misc/freetype/imgui_freetype.cpp)

file(GLOB TracyServer_Server ${tracy_SOURCE_DIR}/server/*.cpp)
file(GLOB TracyServer_Common ${tracy_SOURCE_DIR}/common/*.cpp)
file(
  GLOB
  TracyServer_zstd
  ${tracy_SOURCE_DIR}/zstd/common/*.c
  ${tracy_SOURCE_DIR}/zstd/compress/*.c
  ${tracy_SOURCE_DIR}/zstd/decompress/*.c
  ${tracy_SOURCE_DIR}/zstd/dictBuilder/*.c)

add_executable(
  TracyServer
  ${TracyServer_SOURCES}
  ${TracyServer_HEADERS}
  ${TracyServer_ImGui}
  ${TracyServer_Server}
  ${TracyServer_Common}
  ${TracyServer_zstd})

target_compile_definitions(
  TracyServer
  PRIVATE IMGUI_ENABLE_FREETYPE
          DISPLAY_SERVER_X11
          ZSTD_DISABLE_ASM
          TRACY_NO_FILESELECTOR)
target_include_directories(TracyServer PRIVATE /usr/include/freetype2 ${tracy_SOURCE_DIR}/imgui
                                               ${capstone_SOURCE_DIR}/include/capstone)
target_link_libraries(TracyServer PRIVATE freetype glfw capstone)

target_compile_options(TracyServer PRIVATE -O3)
target_compile_options(glfw PRIVATE -O3)
target_compile_options(capstone PRIVATE -O3)
target_compile_definitions(TracyServer PRIVATE NDEBUG)
target_compile_definitions(glfw PRIVATE NDEBUG)
target_compile_definitions(capstone PRIVATE NDEBUG)

disable_static_analysis(TracyServer)
