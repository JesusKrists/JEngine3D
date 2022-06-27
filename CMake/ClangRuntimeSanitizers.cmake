include_guard()

macro(enable_clang_runtime_sanitizers target)

  if(NOT WIN32 AND NOT APPLE)
    if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*")
      execute_process(COMMAND clang --print-file-name libclang_rt.asan-x86_64.so OUTPUT_VARIABLE san_file_path)
      execute_process(COMMAND dirname ${san_file_path} OUTPUT_VARIABLE san_path)
      string(APPEND san_path " ")

      target_link_options(
        ${target}
        PUBLIC
        -shared-libsan
        -Wl,-rpath=${san_path})
    endif()
  endif()

endmacro()
