CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

INCLUDE(cmake/RumBoot.cmake)
find_package(Doxygen)

SET(RUMBOOT_PLATFORM "native" CACHE STRING "RumBoot platform to compile for")

if (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Production" CACHE string "BuildType")
endif()

INCLUDE(cmake/platforms/${RUMBOOT_PLATFORM}.cmake)

if (CROSS_COMPILE)
  SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/cross-compile.cmake")
endif()

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

PROJECT(rumboot C ASM)
SET(PROJECT_VERSION   0.3)
enable_testing()

if (CROSS_COMPILE)
  CROSS_COMPILE_DETECT_SYSROOT()
endif()

RUMBOOT_PLATFORM_SET_COMPILER_FLAGS()

INCLUDE_DIRECTORIES (
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/platform/${RUMBOOT_PLATFORM}/
)

execute_process(COMMAND
  git rev-parse --short HEAD
  OUTPUT_VARIABLE GIT_REVISION
  OUTPUT_STRIP_TRAILING_WHITESPACE
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

execute_process(COMMAND
  git rev-parse --abbrev-ref HEAD
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

find_program(LUA NAMES lua5.1 lua)
if (LUA STREQUAL "LUA-NOTFOUND")
    message(FATAL_ERROR "lua installation not found. Ask admins to install lua 5.1(!)")
endif()

execute_process(COMMAND
  ${LUA} -v
  ERROR_VARIABLE LUA_VERSION
)

if (NOT LUA_VERSION MATCHES "Lua\ 5.1")
  message(FATAL_ERROR "We need lua 5.1, you have: ${LUA_VERSION}")
endif()


SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -g -Wall -fno-omit-frame-pointer")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DROMBOOT_VERSION=\\\"${GIT_BRANCH}-${GIT_REVISION}\\\"")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DROMBOOT_BUILD_TYPE=\\\"${CMAKE_BUILD_TYPE}\\\"")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DROMBOOT_PLATFORM=\\\"${RUMBOOT_PLATFORM}\\\"")

find_program(CCACHE_FOUND ccache)

message("\n\n ==== RumBoot Configuration summary ====")

message("Platform:           ${RUMBOOT_PLATFORM}")
message("Architecture:       ${RUMBOOT_ARCH}")
if (CROSS_COMPILE)
  message("Target triplet:     ${CROSS_COMPILE}")
endif()

if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    message("Compiler Cache:     Yes, using it!")
else()
    message("Compiler Cache:     Not available ;(")
endif()


message("BuildType:          ${CMAKE_BUILD_TYPE}")

if (DOXYGEN_FOUND)
  message("Doxygen Docs:       enabled")
else()
  message("Doxygen Docs:       disabled")
endif()

if (${RUMBOOT_DEBUG})
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DROMBOOT_DEBUG")
  SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -DROMBOOT_DEBUG")
  message("Debugging:          Enabled")
else()
  message("Debugging:          Disabled")
endif()

if (RUMBOOT_TRACE)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRUMBOOT_TRACE=y")
  set(instrument_functions yes)
  message("Function tracing:   Enabled")
else()
  message("Function tracing:   Disabled")
endif()

if (RUMBOOT_PROFILE)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRUMBOOT_PROFILE=y")
  set(instrument_functions yes)
  message("Function profiling:   Enabled")
else()
  message("Function profiling:   Disabled")
endif()

if (instrument_functions)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finstrument-functions")
  ADD_SOURCE("src/lib/trace.c")
endif()

RUMBOOT_PLATFORM_PRINT_SUMMARY()

message("C Flags:            ${CMAKE_C_FLAGS}")
message("ASM Flags:          ${CMAKE_ASM_FLAGS}")

message(" ==== ==== ==== ==== ==== ==== ==== ====\n\n")

#Sanity checking. Production builds need these
if (${CMAKE_BUILD_TYPE} MATCHES "Production")
  if (${RUMBOOT_DEBUG})
    message( FATAL_ERROR "Fatal error: You want to ship DEBUG build to production. Srsly?")
  endif()
endif()


ADD_SOURCE("src/lib/algo/crc32.c")
ADD_SOURCE("src/lib/logo.c")
ADD_SOURCE("src/lib/stdio.c")
ADD_SOURCE("src/lib/printf.c")
ADD_SOURCE("src/lib/testsuite.c")
ADD_SOURCE("src/irq.c")
ADD_SOURCE("src/lib/regpoker.c")
ADD_SOURCE("src/lib/bootsrc/sdio.c")
ADD_SOURCE("src/lib/bootsrc/spiflash.c")
ADD_SOURCE("src/lib/bootsrc/eeprom.c")
ADD_SOURCE("src/lib/algo/shrink.c")
ADD_SOURCE("src/lib/algo/heatshrink_decoder.c")
ADD_SOURCE("src/lib/algo/heatshrink_encoder.c")
ADD_SOURCE("src/lib/bootsource.c")
ADD_SOURCE("src/lib/bootimage.c")
ADD_SOURCE("src/lib/drivers/i2c.c")
ADD_SOURCE("src/lib/drivers/mdma/mdma.c")
ADD_SOURCE("src/lib/drivers/mdma/mdma_transactions.c")
ADD_SOURCE("src/lib/drivers/mdma/descriptor.c")
ADD_SOURCE("src/lib/drivers/muart.c")
# ADD_SOURCE("src/lib/drivers/bisr.c")

if (RUMBOOT_PLATFORM MATCHES native)
    ADD_SOURCE("src/rumboot_main_native.c")
else()
    ADD_SOURCE("src/rumboot_main.c")
    ADD_SOURCE("src/lib/newlib-stubs.c")
    ADD_SOURCE("src/lib/gcov.c")
    ADD_SOURCE("src/lib/gcov-gcc-4.7.c")
endif()

file(GLOB LUA_FILES src/lib/lua-5.1.5/src/*.c)
include_directories(src/lib/lua-5.1.5/src/)

ADD_LIBRARY(lua STATIC ${LUA_FILES})
foreach(c ${RUMBOOT_CONFIGURATIONS})
  ADD_LIBRARY(rumboot-${c} OBJECT ${PLATFORM_SOURCES})
  target_compile_options(rumboot-${c} PUBLIC ${CONFIGURATION_${c}_CFLAGS})
endforeach()

set(RUMBOOT_PLATFORM_TARGET_DIR ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/)
RUMBOOT_PLATFORM_ADD_COMPONENTS()

get_property(idirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(idir ${idirs})
  set(RUMBOOT_INCLUDE_DIRS "${RUMBOOT_INCLUDE_DIRS} ${idir}")
endforeach()

  CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
  )


if (DOXYGEN_FOUND)
  add_custom_target(docs COMMAND doxygen > doxygen.log)
endif()

include(ExternalProject)
ExternalProject_Add(utils
    SOURCE_DIR        "${CMAKE_SOURCE_DIR}/utils"
    BINARY_DIR        "${CMAKE_BINARY_DIR}/utils"
    CONFIGURE_COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}/utils
    BUILD_COMMAND     "make"
    INSTALL_COMMAND   ""
  )

SET(CPACK_GENERATOR "TGZ")
SET(CPACK_PACKAGE_FILE_NAME "rumboot-${CMAKE_BUILD_TYPE}")
SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
INCLUDE(CPack)
