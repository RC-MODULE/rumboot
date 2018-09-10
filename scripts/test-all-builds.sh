#!/bin/bash
PLATFORMS="native mm7705 oi10 basis"
mkdir build-test
cd build-test
for p in ${PLATFORMS}; do
    mkdir $p
    cd $p
    cmake ../../ -DRUMBOOT_PLATFORM=$p -DCMAKE_BUILD_TYPE=Debug
    make || exit 1
    cd ..
done
