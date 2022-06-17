include_guard()

macro(enable_clang_runtime_sanitizers target asan_path)

  if(NOT WIN32)
    if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*")
      target_link_options(
        ${target}
        PUBLIC
        -shared-libsan
        -Wl,-rpath=${asan_path})
    endif()
  endif()

endmacro()
