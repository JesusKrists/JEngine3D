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
    python3 python3-pip pkg-config valgrind unzip curl kcachegrind dbus-x11

# Install packages required by SDL2
RUN apt-get -y install xorg-dev libsdl2-dev

ARG GCC_VER="11"
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    gcc-11 g++-11 gdb \
    gcc-10 g++-10 \
    gcc-9 g++-9 \
    gcc-8 g++-8 \
    gcc-7 g++-7

# Set gcc-${GCC_VER} as default gcc
RUN update-alternatives --install /usr/bin/gcc gcc $(which gcc-${GCC_VER}) 100
RUN update-alternatives --install /usr/bin/g++ g++ $(which g++-${GCC_VER}) 100
RUN update-alternatives --install /usr/bin/cpp cpp $(which cpp-${GCC_VER}) 100

ARG LLVM_VER="13"
RUN wget -qO - https://apt.llvm.org/llvm.sh | bash -s 15
RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    libclang-15-dev clang-tidy-15 clang-format-15

RUN wget -qO - https://apt.llvm.org/llvm.sh | bash -s 14
RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    libclang-14-dev clang-tidy-14 clang-format-14

RUN wget -qO - https://apt.llvm.org/llvm.sh | bash -s 13
RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    libclang-13-dev clang-tidy-13 clang-format-13

RUN wget -qO - https://apt.llvm.org/llvm.sh | bash -s 12
RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    libclang-12-dev clang-tidy-12 clang-format-12

RUN wget -qO - https://apt.llvm.org/llvm.sh | bash -s 11
RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    libclang-11-dev clang-tidy-11 clang-format-11

RUN wget -qO - https://apt.llvm.org/llvm.sh | bash -s 10
RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    libclang-10-dev clang-tidy-10 clang-format-10


RUN update-alternatives --install /usr/bin/clang-format clang-format $(which clang-format-15) 100
# Set the default clang-tidy, so CMake can find it
RUN update-alternatives --install /usr/bin/clang-tidy clang-tidy $(which clang-tidy-15) 100
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
RUN CC=clang-15 CXX=clang++-15 cmake -S ${CPPCHECK_SRC} -B ${CPPCHECK_BUILD} -G "Unix Makefiles" -DHAVE_RULES=On -DBUILD_GUI=Off
RUN cmake --build ${CPPCHECK_BUILD} -j
RUN cmake --install ${CPPCHECK_BUILD}

# Install include-what-you-use
ENV IWYU /home/iwyu
ENV IWYU_BUILD ${IWYU}/build
ENV IWYU_SRC ${IWYU}/include-what-you-use
RUN mkdir -p ${IWYU_BUILD} && \
    git clone --branch clang_14 \
    https://github.com/include-what-you-use/include-what-you-use.git \
    ${IWYU_SRC}
RUN CC=clang-14 CXX=clang++-14 cmake -S ${IWYU_SRC} \
    -B ${IWYU_BUILD} \
    -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-14 && \
    cmake --build ${IWYU_BUILD} -j && \
    cmake --install ${IWYU_BUILD}

# Per https://github.com/include-what-you-use/include-what-you-use#how-to-install:
# `You need to copy the Clang include directory to the expected location before
#  running (similarly, use include-what-you-use -print-resource-dir to learn
#  exactly where IWYU wants the headers).`
RUN mkdir -p $(include-what-you-use -print-resource-dir 2>/dev/null)
RUN ln -s $(readlink -f /usr/lib/clang/14/include) \
    $(include-what-you-use -print-resource-dir 2>/dev/null)/include


RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
    libgtk-3-dev

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
