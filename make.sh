#!/bin/bash

#colors
W="\033[0;00m"
G="\033[0;92m"
R="\033[0;91m"
Y="\033[0;93m"

BUILD_DIR=$(pwd)/build
ROOT_DIR=$(pwd)
INSTALL_PREFIX=$("pwd")/out
RUN=true

CONFIG=RELEASE
ARCH=x64
TARGET="--target install"
CLEAN=false
LIB_IMG_SHARED=false
LIB_IMG_EXAMPLES=true

CUDA_CPP_HOST_COMPILER="clang++-15"
CUDA_C_HOST_COMPILER="clang-15"
CXX_COMPILER="clang++-15"
C_COMPILER="clnag-15"
CUDA_ROOT_DIR="/usr/local/cuda"
CMAKE_VERBOSE=""
CMAKE_JOBS="-j"

print_help() {
  printf "help:\n"
  printf "  --clean                Remove existing build folder.\n"
  printf "  --no-run               Build but don't run the app.\n"
  printf "  --prefix               Specify install prefix.\n"
  printf "  --config               Specify build type. Options RELEASE, DEBUG ...\n"
  printf "  --cxx-compiler         Specify c++ compiler. Default clang\n"
  printf "  --c-compiler           Specify c compiler. Default clang\n"
  printf "  --cuda-cxx-compiler    Specify CUDA c++ HOST compiler. Default clang\n"
  printf "  --cuda-c-compiler      Specify CUDA c HOST  compiler. Default clang\n"
  printf "  --cuda-path            Specify CUDA toolkit path. Default \"/usr/local/cuda\"\n"
  printf "  --target               NOT USED. specify target.\n"
  printf "  -j | --jobs            Allow N jobs at once\n"
  printf "  -h | --help            this help.\n"
}

opts=("$@")
for ((i = 0; i < $#; i++)); do
  case "${opts[$i]}" in
  --clean)
    CLEAN=true
    ;;
  --no-run)
    RUN=false
    ;;
  --prefix)
    INSTALL_PREFIX=${opts[$((i + 1))]}
    ((i++))
    ;;
  --examples)
    LIB_IMG_EXAMPLES=true
    ;;
  --config)
    CONFIG=${opts[$((i + 1))]}
    ((i++))
    ;;
  --cxx-compiler)
    CXX_COMPILER=${opts[$((i + 1))]}
    ((i++))
    ;;
  --c-compiler)
    C_COMPILER=${opts[$((i + 1))]}
    ((i++))
    ;;
  --cuda-cxx-compiler)
    CUDA_C_HOST_COMPILER=${opts[$((i + 1))]}
    ((i++))
    ;;
  --cuda-c-compiler)
    CUDA_C_COMPILER=${opts[$((i + 1))]}
    ((i++))
    ;;
  --cuda-path)
    CUDA_ROOT_DIR=${opts[$((i + 1))]}
    ((i++))
    ;;
  --arch)
    ARCH=${opts[$((i + 1))]}
    ((i++))
    ;;
  --shared)
    LIB_IMG_SHARED=true
    ;;
  --target)
    TARGET="--target ${opts[$((i + 1))]}"
    ((i++))
    ;;
  -v | --verbose)
    CMAKE_VERBOSE="-v"
    ;;
  -j | --jobs)
    if [[ ${opts[$((i + 1))]} =~ $num_re ]]; then
      CMAKE_JOBS="-j ${opts[$((i + 1))]}"
      ((i++))
    else
      CMAKE_JOBS="-j"
    fi
    ;;
  -h | --help)
    print_help
    exit 0
    ;;
  *)
    printf "\"${opts[$((i))]}\" is invalid option, -h for help.\n"
    print_help
    exit 1
    ;;
  esac
done

if [ $CLEAN == true ]; then
  printf "${Y}-- Removing build folder${W}\n"
  rm -rf $BUILD_DIR
  if [[ $? -eq 1 ]]; then
    printf "${R}-- Clean Error: Build folder not found $BUILD_DIR.${W}\n"
    exit 1
  fi
  printf "${Y}-- Removing installed build${W}\n"
  rm -rf $INSTALL_PREFIX
  if [[ $? -eq 1 ]]; then
    printf "${R}-- Clean Error: install folder not found $BUILD_DIR.${W}\n"
    exit 1
  fi
fi

cmake -S $ROOT_DIR -B $BUILD_DIR \
  -D CMAKE_CXX_COMPILER=$CXX_COMPILER \
  -D CMAKE_BUILD_TYPE:STRING=$CONFIG \
  -D CMAKE_CUDA_HOST_COMPILER=$CUDA_CPP_HOST_COMPILER \
  -D CMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
  -D CMAKE_EXPORT_COMPILE_COMMANDS:BOOL=true \
  -D LIB_IMG_SHARED:BOOL=$LIB_IMG_SHARED \
  -D LIB_IMG_EXAMPLES:BOOL=$LIB_IMG_EXAMPLES
if [[ $? -eq 1 ]]; then
  printf "${R}-- Cmake failed${W}\n" &&
    exit 1
fi

cmake --build $BUILD_DIR $TARGET $CMAKE_JOBS $CMAKE_VERBOSE
if [[ $? -eq 0 ]]; then
  printf "${G}Build successful.${W}\n\n"
  if [[ $RUN = true ]]; then
    printf "${G}-- Running Application.${W}\n\n"
    ./out/img-example
  fi
else
  printf "${R}Build failed.${W}\n"
fi
