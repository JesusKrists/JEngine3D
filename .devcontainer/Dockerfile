# [Choice] bionic (18.04), focal (20.04)
ARG VARIANT="focal"
FROM ubuntu:${VARIANT}

# Restate the variant to use it later on in the llvm and cmake installations
ARG VARIANT

# Install necessary packages available from standard repos
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    software-properties-common wget apt-utils file zip \
    openssh-client gpg-agent socat rsync \
    make ninja-build git \
    python3 python3-pip pkg-config valgrind unzip

# Install packages required by SDL2
RUN apt-get -y install xorg-dev libsdl2-dev

# By default, anything you run in Docker is done as superuser.
# Conan runs some install commands as superuser, and will prepend `sudo` to
# these commands, unless `CONAN_SYSREQUIRES_SUDO=0` is in your env variables.
ENV CONAN_SYSREQUIRES_SUDO 0
# Some packages request that Conan use the system package manager to install
# a few dependencies. This flag allows Conan to proceed with these installations;
# leaving this flag undefined can cause some installation failures.
ENV CONAN_SYSREQUIRES_MODE enabled

# User-settable versions:
# This Dockerfile should support gcc-[7, 8, 9, 10, 11] and clang-[10, 11, 12, 13]
# Earlier versions of clang will require significant modifications to the IWYU section
ARG GCC_VER="11"
# Add gcc-${GCC_VER}
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    gcc-${GCC_VER} g++-${GCC_VER} gdb

# Set gcc-${GCC_VER} as default gcc
RUN update-alternatives --install /usr/bin/gcc gcc $(which gcc-${GCC_VER}) 100
RUN update-alternatives --install /usr/bin/g++ g++ $(which g++-${GCC_VER}) 100
RUN update-alternatives --install /usr/bin/cpp cpp $(which cpp-${GCC_VER}) 100

ARG LLVM_VER="13"
RUN wget -qO - https://apt.llvm.org/llvm.sh | bash -s ${LLVM_VER}
RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    libclang-${LLVM_VER}-dev clang-tidy-${LLVM_VER} clang-format-${LLVM_VER}


RUN update-alternatives --install /usr/bin/clang-format clang-format $(which clang-format-${LLVM_VER}) 100
# Set the default clang-tidy, so CMake can find it
RUN update-alternatives --install /usr/bin/clang-tidy clang-tidy $(which clang-tidy-${LLVM_VER}) 100
# Set clang-${LLVM_VER} as default clang
RUN update-alternatives --install /usr/bin/clang clang $(which clang-${LLVM_VER}) 100
RUN update-alternatives --install /usr/bin/clang++ clang++ $(which clang++-${LLVM_VER}) 100

# Install cmake-format
RUN python3 -m pip install cmakelang

# Add current cmake/ccmake, from Kitware
ARG CMAKE_URL="https://apt.kitware.com/ubuntu/"
ARG CMAKE_PKG=${VARIANT}
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null \
    | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null && \
    apt-add-repository -y "deb ${CMAKE_URL} ${CMAKE_PKG} main" && \
    apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends cmake cmake-curses-gui

# Install editors
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    neovim emacs nano

# Install optional dependecies
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    doxygen graphviz ccache

# Build cppcheck from source
ENV CPPCHECK /home/cppcheck
ENV CPPCHECK_BUILD ${CPPCHECK}/build
ENV CPPCHECK_SRC ${CPPCHECK}/cppcheck-src
RUN mkdir -p ${CPPCHECK_BUILD}
RUN apt-get install libxml2-utils && apt-get install libz3-dev libtinyxml2-dev && apt-get install libpcre3-dev
RUN git clone --branch 2.7 https://github.com/danmar/cppcheck.git ${CPPCHECK_SRC}
RUN CC=clang-${LLVM_VER} CXX=clang++-${LLVM_VER} cmake -S ${CPPCHECK_SRC} -B ${CPPCHECK_BUILD} -G "Unix Makefiles" -DHAVE_RULES=On -DBUILD_GUI=Off
RUN cmake --build ${CPPCHECK_BUILD} -j
RUN cmake --install ${CPPCHECK_BUILD}

# Install include-what-you-use
ENV IWYU /home/iwyu
ENV IWYU_BUILD ${IWYU}/build
ENV IWYU_SRC ${IWYU}/include-what-you-use
RUN if [ "$LLVM_VER" = "15" ] ; then \
    mkdir -p ${IWYU_BUILD} && \
    git clone https://github.com/include-what-you-use/include-what-you-use.git \
    ${IWYU_SRC} ; \
    else \
    mkdir -p ${IWYU_BUILD} && \
    git clone --branch clang_${LLVM_VER} \
    https://github.com/include-what-you-use/include-what-you-use.git \
    ${IWYU_SRC} ; \
    fi
RUN CC=clang-${LLVM_VER} CXX=clang++-${LLVM_VER} cmake -S ${IWYU_SRC} \
    -B ${IWYU_BUILD} \
    -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-${LLVM_VER} && \
    cmake --build ${IWYU_BUILD} -j && \
    cmake --install ${IWYU_BUILD}

# Per https://github.com/include-what-you-use/include-what-you-use#how-to-install:
# `You need to copy the Clang include directory to the expected location before
#  running (similarly, use include-what-you-use -print-resource-dir to learn
#  exactly where IWYU wants the headers).`
RUN mkdir -p $(include-what-you-use -print-resource-dir 2>/dev/null)
RUN ln -s $(readlink -f /usr/lib/clang/${LLVM_VER}/include) \
    $(include-what-you-use -print-resource-dir 2>/dev/null)/include

## Cleanup cached apt data we don't need anymore
RUN apt-get autoremove -y && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Allow the user to set compiler defaults
ARG USE_CLANG
# if --build-arg USE_CLANG=1, set CC to 'clang' or set to null otherwise.
ENV CC=${USE_CLANG:+"clang"}
ENV CXX=${USE_CLANG:+"clang++"}
# if CC is null, set it to 'gcc' (or leave as is otherwise).
ENV CC=${CC:-"gcc"}
ENV CXX=${CXX:-"g++"}

# Include project
#ADD . /workspaces/cpp_starter_project
#WORKDIR /workspaces/cpp_starter_project

CMD ["/bin/bash"]
