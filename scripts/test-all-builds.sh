#!/bin/bash
PLATFORMS="native oi10 basis bbp3 o32t"
mkdir build-test
cd build-test

test()
{
    p=$1
    b=$2
    echo $p-$b
    mkdir $p-$b
    cd $p-$b
    cmake ../../ -DRUMBOOT_PLATFORM=$p -DCMAKE_BUILD_TYPE=$b
    make -j16 || exit 1
    cd ..

}


for b in Debug Production PostProduction; do
for p in ${PLATFORMS}; do
	test $p $b
done

test mm7705 PostProduction
test zed PostProduction


done
