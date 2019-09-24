#!/bin/bash
PLAT=oi10
TARGET_WHITELIST="simple-iram"
SIMPLE_IRAM_WHITELIST="hello.c dit.c oi10_spels_cache_hack.c"

delete_all_but() {
    dir=$1
    shift
    prev=`pwd`
    cd $dir
    ls | while read line; do
        keep=0
        for e in $*; do
            [ "$e" == "$line" ] && keep=1
        done
        if [ "$keep" -eq "0" ]; then
            echo "Removing $line"
            rm -Rfv $line
        else
            echo "Keeping $line"
        fi
    done
    cd $prev
}


delete_all_but src/platform/ native common $PLAT 
delete_all_but src/platform/$PLAT/targets $TARGET_WHITELIST
delete_all_but src/platform/$PLAT/targets/simple-iram $SIMPLE_IRAM_WHITELIST
delete_all_but lds $PLAT 
delete_all_but src/platform/ common $PLAT 
delete_all_but cmake/platforms/ native.cmake $PLAT.cmake
