######################### docopt ######################################
FetchContent_Declare(
  docopt
  GIT_REPOSITORY https://github.com/docopt/docopt.cpp.git
  GIT_TAG v0.6.3)

FetchContent_MakeAvailable(docopt)
add_library(docopt::docopt ALIAS docopt)

########################## fmt ########################################
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 8.1.1)

FetchContent_MakeAvailable(fmt)

########################## spdlog ######################################
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.9.2)

FetchContent_MakeAvailable(spdlog)
