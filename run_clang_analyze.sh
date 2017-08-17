#!/bin/bash
export CCC_CC=arm-rcm-eabihf-gcc
export CLANG_ANALYZER_TARGET=arm-rcm-eabihf
rm -rf build-clang-analyze
mkdir build-clang-analyze
cd build-clang-analyze
cmake .. -DRUMBOOT_PLATFORM=basis -DCLANG_ANALYZE=Yes
make utils
scan-build make
