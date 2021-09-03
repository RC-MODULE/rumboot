# This file assumes that you have a linaro abe-based toolchain
# with a raspbian sysroot somewhere inside. This file also
# takes care to trick pkg-config into searching only toolchain's sysroot for
# the libraries

if (NOT CMAKE_LIBRARY_PATH)
    SET(CMAKE_LIBRARY_PATH ${CROSS_COMPILE})
endif()

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

#Hack!
if(WIN32)
  set(TOOL_SUFFIX .exe)
endif()

if (NOT CMAKE_C_COMPILER)
    # HACK: Bare metal compiler confuses cmake
    if (CLANG_ANALYZE)
        set(CMAKE_C_COMPILER_WORKS 1)
        SET(CMAKE_C_COMPILER     /usr/share/clang/scan-build-3.8/libexec/ccc-analyzer)
    else()
        SET(CMAKE_C_COMPILER     "${CROSS_COMPILE}-gcc${TOOL_SUFFIX}")
    endif()
endif()

if (NOT CMAKE_CXX_COMPILER)
    SET(CMAKE_CXX_COMPILER   "${CROSS_COMPILE}-g++${TOOL_SUFFIX}")
endif()
set(CMAKE_CXX_COMPILER_WORKS 1)

# where is the target environment
# This macro should be called once AFTER PROJECT() directive
macro(CROSS_COMPILE_DETECT_SYSROOT)
    find_program(CROSS_TOOLCHAIN_PATH NAMES ${CMAKE_C_COMPILER})
    get_filename_component(CROSS_TOOLCHAIN_PATH "${CROSS_TOOLCHAIN_PATH}" PATH)

    if (EXISTS ${CROSS_TOOLCHAIN_PATH}/../${CROSS_COMPILE}/sysroot)
        SET(CMAKE_FIND_ROOT_PATH  ${CROSS_TOOLCHAIN_PATH}/../${CROSS_COMPILE}/sysroot)
    elseif(EXISTS ${CROSS_TOOLCHAIN_PATH}/../${CROSS_COMPILE}/libc)
        SET(CMAKE_FIND_ROOT_PATH  ${CROSS_TOOLCHAIN_PATH}/../${CROSS_COMPILE}/libc)
    else()
        message(WARNING "Couldn't auto-detect sysroot dir")
    endif()

    # search for programs in the build host directories
    SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    # for libraries and headers in the target directories
    SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endmacro()
