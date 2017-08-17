export CCC_CC=arm-rcm-eabihf-gcc
export CLANG_ANALYZER_TARGET=arm-rcm-eabihf
mkdir build-clang-analyze
cd build-clang-analyze
cmake .. -DRUMBOOT_PLATFORM=basis -DCLANG_ANALYZE=yes
scan-build make
