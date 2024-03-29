# Set the project name and language
project(
  JEngine3D_Editor
  VERSION 0.0.1
  DESCRIPTION "Custom game engine JEngine3D Editor App"
  HOMEPAGE_URL "https://github.com/JesusKrists/JEngine3D"
  LANGUAGES CXX C)

# configure files based on CMake configuration options
add_subdirectory(configured_files)

include(JEngine3D_Editor_Sources.cmake)

add_library(JEngine3D_Editor SHARED ${JEngine3D_Editor_SOURCES} ${JEngine3D_Editor_HEADERS})
target_link_libraries(
  JEngine3D_Editor
  PRIVATE project_options
          project_warnings
          JEngine3D
          FontIcons)

target_include_directories(JEngine3D_Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(JEngine3D_Editor
                           PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/configured_files/configured_files/include")

set_target_properties(JEngine3D_Editor PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(JEngine3D_Editor PROPERTIES RELWITHDEBINFO_POSTFIX "d")

add_custom_command(
  TARGET JEngine3D_Editor
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/JEngine3D_Editor/assets/
          $<TARGET_FILE_DIR:JEngine3D_Executable>/assets/)

#add_custom_command(
#  TARGET JEngine3D_Editor
#  POST_BUILD
#  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:JEngine3D_Editor> $<TARGET_FILE_DIR:JEngine3D_Executable>)
