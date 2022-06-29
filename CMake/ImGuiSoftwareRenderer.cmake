include_guard(GLOBAL)

include(${CMAKE_SOURCE_DIR}/CMake/DisableStaticAnalysis.cmake)

FetchContent_Declare(
  ImGui_SW
  GIT_REPOSITORY https://github.com/JesusKrists/imgui_software_renderer.git
  GIT_TAG master
  GIT_SUBMODULES "")

FetchContent_MakeAvailable(ImGui_SW)

set(ImGui_SW_SOURCES ${imgui_sw_SOURCE_DIR}/src/imgui_sw.cpp)

set(ImGui_SW_HEADERS ${imgui_sw_SOURCE_DIR}/src/imgui_sw.hpp)

add_library(ImGui_SWLibrary ${ImGui_SW_SOURCES} ${ImGui_SW_HEADERS})
disable_static_analysis(ImGui_SWLibrary)

target_link_libraries(ImGui_SWLibrary PRIVATE project_options ImGuiLibrary)
target_include_directories(ImGui_SWLibrary PUBLIC ${imgui_sw_SOURCE_DIR}/src)

set_target_properties(ImGui_SWLibrary PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(ImGui_SWLibrary PROPERTIES RELWITHDEBINFO_POSTFIX "d")
