#!/bin/bash

set -e

export CXX_LD="mold"
export CXX="clang++"
export CC="clang"

ROOT_SRC_DIR="$(cd -P "$(dirname "${BASH_SOURCE[0]}")/.." && pwd )"
BUILD_DIR="$ROOT_SRC_DIR/build"
BUILD_ALL=true

if [ $# -gt 0 ]; then
    BUILD_ALL=false
    for arg in "$@"; do
        case $arg in
            "bench" | "tests" | "examples")
                eval "BUILD_$arg=true"
                ;;
            *)
                echo "Unknown option: $arg"
                exit 1
                ;;
        esac
    done
fi

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR" 
fi

cd "$BUILD_DIR"

if [ "$BUILD_ALL" = true ]; then
    cmake -GNinja -DCMAKE_BUILD_TYPE=Release "$ROOT_SRC_DIR"
    ninja
else
    for arg in "$@"; do
        if [ "$(eval echo \$BUILD_$arg)" = true ]; then
            cmake -GNinja -DBUILD_${arg^^}=ON -DCMAKE_BUILD_TYPE=Release "$ROOT_SRC_DIR"
            ninja run${arg}
        fi
    done
fi
