#!/bin/bash

case "$1" in
    "oi10")
        export CCC_CC=powerpc-rcm-elf-gcc
        export CLANG_ANALYZER_TARGET=powerpc-rcm-elf-gcc
    ;;
    "basis")
        export CCC_CC=arm-rcm-eabihf-gcc
        export CLANG_ANALYZER_TARGET=arm-rcm-eabihf
    ;;
    native)
        export CCC_CC=gcc
    ;;
esac

rm -rf build-clang-analyze
mkdir build-clang-analyze
cd build-clang-analyze
cmake .. -DCMAKE_BUILD_TYPE=Production -DRUMBOOT_DISABLE_CCACHE=Yes -DRUMBOOT_PLATFORM=$1 -DCLANG_ANALYZE=Yes
make utils
scan-build make
