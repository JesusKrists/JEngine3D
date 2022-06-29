include_guard(GLOBAL)

include(${CMAKE_SOURCE_DIR}/CMake/DisableStaticAnalysis.cmake)

FetchContent_Declare(
  ImGui
  GIT_REPOSITORY https://github.com/JesusKrists/imgui.git
  GIT_TAG docking)

FetchContent_MakeAvailable(ImGui)

set(ImGui_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp)

set(ImGui_HEADERS
    ${imgui_SOURCE_DIR}/imconfig.h
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imstb_rectpack.h
    ${imgui_SOURCE_DIR}/imstb_textedit.h
    ${imgui_SOURCE_DIR}/imstb_truetype.h)

add_library(ImGuiLibrary ${ImGui_SOURCES} ${ImGui_HEADERS})

target_link_libraries(ImGuiLibrary PRIVATE project_options SDL2-static)
target_include_system_library(ImGuiLibrary PUBLIC fmt::fmt)
target_include_directories(ImGuiLibrary SYSTEM PUBLIC $<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>)

target_include_directories(ImGuiLibrary PUBLIC ${imgui_SOURCE_DIR})
target_include_directories(ImGuiLibrary PUBLIC ${imgui_SOURCE_DIR}/backends)

target_include_directories(ImGuiLibrary PRIVATE ${CMAKE_SOURCE_DIR}/JEngine3D)
target_compile_definitions(ImGuiLibrary PUBLIC "IMGUI_USER_CONFIG=\"JEngine3D/Core/ImGui/config.hpp\"")

disable_static_analysis(ImGuiLibrary)

set_target_properties(ImGuiLibrary PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(ImGuiLibrary PROPERTIES RELWITHDEBINFO_POSTFIX "d")
