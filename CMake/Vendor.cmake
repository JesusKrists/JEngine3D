######################### docopt ######################################
FetchContent_Declare(
  docopt
  GIT_REPOSITORY https://github.com/docopt/docopt.cpp.git
  GIT_TAG v0.6.3)

FetchContent_MakeAvailable(docopt)
add_library(docopt::docopt ALIAS docopt_s)

########################## spdlog ######################################
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.9.2)

FetchContent_MakeAvailable(spdlog)

######################### Remove static analysis tools ##################
set_property(TARGET docopt PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET docopt PROPERTY C_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET docopt_s PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET docopt_s PROPERTY C_INCLUDE_WHAT_YOU_USE "")
if(NOT MSVC AND NOT XCODE)
  set_property(TARGET docopt_o PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
  set_property(TARGET docopt_o PROPERTY C_INCLUDE_WHAT_YOU_USE "")
endif()
set_property(TARGET spdlog PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
set_property(TARGET spdlog PROPERTY C_INCLUDE_WHAT_YOU_USE "")
#
set_property(TARGET docopt PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET docopt PROPERTY C_CLANG_TIDY "")
set_property(TARGET docopt_s PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET docopt_s PROPERTY C_CLANG_TIDY "")
if(NOT MSVC AND NOT XCODE)
  set_property(TARGET docopt_o PROPERTY CXX_CLANG_TIDY "")
  set_property(TARGET docopt_o PROPERTY C_CLANG_TIDY "")
endif()
set_property(TARGET spdlog PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET spdlog PROPERTY C_CLANG_TIDY "")
#
set_property(TARGET docopt PROPERTY CXX_CPPCHECK "")
set_property(TARGET docopt PROPERTY C_CPPCHECK "")
set_property(TARGET docopt_s PROPERTY CXX_CPPCHECK "")
set_property(TARGET docopt_s PROPERTY C_CPPCHECK "")
if(NOT MSVC AND NOT XCODE)
  set_property(TARGET docopt_o PROPERTY CXX_CPPCHECK "")
  set_property(TARGET docopt_o PROPERTY C_CPPCHECK "")
endif()
set_property(TARGET spdlog PROPERTY CXX_CPPCHECK "")
set_property(TARGET spdlog PROPERTY C_CPPCHECK "")
