# configure files based on CMake configuration options
add_subdirectory(configured_files)

include(JEngine3D_Sources.cmake)

add_library(JEngine3D ${JEngine3D_SOURCES} ${JEngine3D_HEADERS})
target_link_libraries(JEngine3D PRIVATE project_options project_warnings)
target_link_system_libraries(JEngine3D PRIVATE SDL2-static)

target_include_directories(JEngine3D PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/configured_files/configured_files/include")

target_include_directories(JEngine3D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
  set(JEngine3D_DEFINITIONS ${JEngine3D_DEFINITIONS} JE_DEBUG)
endif()

if(WIN32)
  set(JEngine3D_DEFINITIONS ${JEngine3D_DEFINITIONS} JE_PLATFORM_WINDOWS)
elseif(UNIX)
  set(JEngine3D_DEFINITIONS ${JEngine3D_DEFINITIONS} JE_PLATFORM_UNIX)
endif()

target_compile_definitions(JEngine3D PUBLIC ${JEngine3D_DEFINITIONS})
