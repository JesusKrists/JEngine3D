include(${CMAKE_SOURCE_DIR}/CMake/ClangRuntimeSanitizers.cmake)

# configure files based on CMake configuration options
add_subdirectory(configured_files)

include(JEngine3D_Sources.cmake)

add_library(JEngine3D SHARED ${JEngine3D_SOURCES} ${JEngine3D_HEADERS})
target_link_libraries(JEngine3D PRIVATE project_options project_warnings)
target_link_system_libraries(
  JEngine3D
  PRIVATE
  SDL2::SDL2
  libglew_static
  lunasvg
  PUBLIC
  docopt::docopt
  fmt::fmt
  spdlog
  ImGuiLibrary
  Tracy::TracyClient
  CRTarget
  STBTarget # Make this a private link when implented stb into JEngine3D properly (currently used in JEngine3D_Editor)
  glm::glm)
target_compile_definitions(JEngine3D PRIVATE JE_EXPORT)

target_include_directories(JEngine3D PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/configured_files/configured_files/include")

target_include_directories(JEngine3D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(JEngine3D_DEFINITIONS ${JEngine3D_DEFINITIONS} TRACY_CALLSTACK=16)
if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
  set(JEngine3D_DEFINITIONS ${JEngine3D_DEFINITIONS} JE_DEBUG)
endif()

if(WIN32)
  set(JEngine3D_DEFINITIONS ${JEngine3D_DEFINITIONS} JE_PLATFORM_WINDOWS)
elseif(UNIX)
  set(JEngine3D_DEFINITIONS ${JEngine3D_DEFINITIONS} JE_PLATFORM_UNIX)
  if(APPLE)
    set(JEngine3D_DEFINITIONS ${JEngine3D_DEFINITIONS} JE_PLATFORM_APPLE)
  endif()
endif()

if(ENABLE_DEBUG_BREAK)
  set(JEngine3D_DEFINITIONS ${JEngine3D_DEFINITIONS} JE_ENABLE_DEBUGBREAK)
endif()

target_compile_definitions(JEngine3D PUBLIC ${JEngine3D_DEFINITIONS})

if(OPT_ENABLE_SANITIZER_ADDRESS OR OPT_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR)
  enable_clang_runtime_sanitizers(JEngine3D)
endif()

set_target_properties(JEngine3D PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(JEngine3D PROPERTIES RELWITHDEBINFO_POSTFIX "d")

add_library(JEngine3D_NEW_OVERRIDE STATIC ${CMAKE_CURRENT_SOURCE_DIR}/JEngine3D/Debug/NewOverride.cpp)
target_include_directories(JEngine3D_NEW_OVERRIDE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_system_libraries(JEngine3D PUBLIC JEngine3D_NEW_OVERRIDE)

###############################################################################################

add_executable(JEngine3D_Executable JEngine3D/Core/EntryPoint.cpp)
target_link_libraries(JEngine3D_Executable PRIVATE project_options project_warnings JEngine3D)
target_include_directories(JEngine3D_Executable
                           PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/configured_files/configured_files/include")

#############################################################################################
#include(${CMAKE_SOURCE_DIR}/CMake/DisableStaticAnalysis.cmake)

#add_executable(ImGui_Reference_App JEngine3D/Core/ImGui/SDL_Opengl_Reference_App.cpp)
#target_link_system_libraries(
#  ImGui_Reference_App
#  PRIVATE
#  project_options
#  SDL2-static
#  libglew_static
#  ImGuiLibrary)
#target_include_directories(ImGui_Reference_App PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#disable_static_analysis(ImGui_Reference_App)
