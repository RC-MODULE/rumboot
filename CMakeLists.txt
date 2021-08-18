CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

INCLUDE(cmake/RumBoot.cmake)
find_package(Doxygen)
find_package(Git)

SET(RUMBOOT_PLATFORM "native" CACHE STRING "RumBoot platform to compile for")

if (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Production" CACHE string "BuildType")
endif()
set(RUMBOOT_BUILD_TYPE ${CMAKE_BUILD_TYPE})

if (NOT RUMBOOT_SOC_BUILD_TYPE)
    set(RUMBOOT_SOC_BUILD_TYPE POSTLAYOUT)
endif()

INCLUDE(cmake/platforms/${RUMBOOT_PLATFORM}.cmake)

if (CROSS_COMPILE)
  SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/cross-compile.cmake")
endif()

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

PROJECT(rumboot C ASM CXX)
SET(PROJECT_VERSION   0.3)
enable_testing()

if (CROSS_COMPILE)
  CROSS_COMPILE_DETECT_SYSROOT()
endif()

RUMBOOT_PLATFORM_SET_COMPILER_FLAGS()

if (RUMBOOT_LIBC_PATH OR RUMBOOT_LIBC_PATH_INC OR RUMBOOT_LIBC_PATH_LIB)
    set(RUMBOOT_EXTERNAL_LIBC YES)
    if (NOT RUMBOOT_LIBC_PATH_INC)
        set(RUMBOOT_LIBC_PATH_INC ${RUMBOOT_LIBC_PATH}/include)
    endif()
    if (NOT RUMBOOT_LIBC_PATH_LIB)
        set(RUMBOOT_LIBC_PATH_LIB ${RUMBOOT_LIBC_PATH}/static)
    endif()
    set(CMAKE_C_FLAGS "-I${RUMBOOT_LIBC_PATH_INC} ${CMAKE_C_FLAGS}") 
    set(CMAKE_ASM_FLAGS "-I${RUMBOOT_LIBC_PATH_INC} ${CMAKE_ASM_FLAGS}") 
    set(CMAKE_CXX_FLAGS "-I${RUMBOOT_LIBC_PATH_INC} ${CMAKE_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-L${RUMBOOT_LIBC_PATH_LIB} ${CMAKE_EXE_LINKER_FLAGS}")
endif()

if (RUMBOOT_ENABLE_LTO)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
  set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -flto")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
endif()

INCLUDE_DIRECTORIES (
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/platform/${RUMBOOT_PLATFORM}/
    ${CMAKE_SOURCE_DIR}/include/arch/${RUMBOOT_ARCH}/
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


SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wall -fno-omit-frame-pointer")

SET(EXTRA_COMMON_FLAGS "${EXTRA_COMMON_FLAGS} -DRUMBOOT_VERSION=\\\"${GIT_BRANCH}-${GIT_REVISION}\\\"")


string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_DEF)
set(CMAKE_BUILD_TYPE_DEF "CMAKE_BUILD_TYPE_${CMAKE_BUILD_TYPE_DEF}")
SET(EXTRA_COMMON_FLAGS "${EXTRA_COMMON_FLAGS} -D${CMAKE_BUILD_TYPE_DEF}")

SET(EXTRA_COMMON_FLAGS "${EXTRA_COMMON_FLAGS} -DCMAKE_BUILD_TYPE=\\\"${CMAKE_BUILD_TYPE}\\\"")
SET(EXTRA_COMMON_FLAGS "${EXTRA_COMMON_FLAGS} -DRUMBOOT_PLATFORM=\\\"${RUMBOOT_PLATFORM}\\\"")
SET(EXTRA_COMMON_FLAGS "${EXTRA_COMMON_FLAGS} -DRUMBOOT_ARCH=\\\"${RUMBOOT_ARCH}\\\"")

if(NOT RUMBOOT_DISABLE_CCACHE)
  find_program(CCACHE_FOUND ccache)
else()
  set(CCACHE_FOUND No)
endif()


macro(find_rumboot_tools)
  message("rumboot-tools: Updating submodule, please stand by...")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} submodule update --init rumboot-tools 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
  set(RUMBOOT_TOOLS_DIR ${PROJECT_SOURCE_DIR}/rumboot-tools)
  file(WRITE ${PROJECT_BINARY_DIR}/local-rumboot-packimage "#!/bin/bash\n${PYTHON_EXECUTABLE} ${RUMBOOT_TOOLS_DIR}/rumboot_packimage.py $*")
  file(WRITE ${PROJECT_BINARY_DIR}/local-rumboot-xflash    "#!/bin/bash\n${PYTHON_EXECUTABLE} ${RUMBOOT_TOOLS_DIR}/rumboot_xflash.py -z ${PROJECT_BINARY_DIR}/ $*")
  file(WRITE ${PROJECT_BINARY_DIR}/local-rumboot-flashrom "#!/bin/bash\n${PYTHON_EXECUTABLE} ${RUMBOOT_TOOLS_DIR}/rumboot_flashom.py -z ${PROJECT_BINARY_DIR}/ $*")
  file(WRITE ${PROJECT_BINARY_DIR}/local-rumboot-xrun      "#!/bin/bash\n${PYTHON_EXECUTABLE} ${RUMBOOT_TOOLS_DIR}/rumboot_xrun.py $*")
  file(WRITE ${PROJECT_BINARY_DIR}/local-rumboot-combine   "#!/bin/bash\n${PYTHON_EXECUTABLE} ${RUMBOOT_TOOLS_DIR}/rumboot_combine.py $*")
  find_program(CHMOD_PROG chmod)
  if(CHMOD_PROG)
    execute_process(COMMAND ${CHMOD_PROG} +x ${PROJECT_BINARY_DIR}/local-rumboot-packimage)
    execute_process(COMMAND ${CHMOD_PROG} +x ${PROJECT_BINARY_DIR}/local-rumboot-xrun)
    execute_process(COMMAND ${CHMOD_PROG} +x ${PROJECT_BINARY_DIR}/local-rumboot-xflash)
    execute_process(COMMAND ${CHMOD_PROG} +x ${PROJECT_BINARY_DIR}/local-rumboot-combine)
    execute_process(COMMAND ${CHMOD_PROG} +x ${PROJECT_BINARY_DIR}/local-rumboot-flashrom)
  endif()

  find_file(RUMBOOT_PACKIMAGE
    NAMES rumboot_packimage.py
    PATHS ${PROJECT_SOURCE_DIR}/rumboot-tools
          ${CMAKE_SOURCE_DIR}/rumboot-tools
  )  
  find_file(RUMBOOT_COMBINE
    NAMES rumboot_combine.py
    PATHS ${PROJECT_SOURCE_DIR}/rumboot-tools
          ${CMAKE_SOURCE_DIR}/rumboot-tools
  )

  if (NOT RUMBOOT_PACKIMAGE STREQUAL "RUMBOOT_PACKIMAGE-NOTFOUND")
    message("-- Found rumboot-packimage: ${RUMBOOT_PACKIMAGE}")
  else()
    message(FATAL_ERROR "Failed to find rumboot-packimage (Problems with repo access?)")
  endif()

  if (NOT RUMBOOT_COMBINE STREQUAL "RUMBOOT_COMBINE-NOTFOUND")
    message("-- Found rumboot-combine: ${RUMBOOT_COMBINE}")
  else()
    message(FATAL_ERROR "Failed to find rumboot-combine (Problems with repo access?)")
  endif()
endmacro()

find_rumboot_tools()

message("\n\n ==== RumBoot Configuration summary ====")

message("Platform:           ${RUMBOOT_PLATFORM}")
message("Architecture:       ${RUMBOOT_ARCH}")
message("SoC Build Type:     ${RUMBOOT_SOC_BUILD_TYPE}")
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

if (RUMBOOT_ENABLE_LTO)
  message("GCC LTO:            enabled")
else()
  message("GCC LTO:            disabled")
endif()

if (RUMBOOT_USE_STRICT_DWARF)
  set(EXTRA_COMMON_FLAGS "${EXTRA_COMMON_FLAGS} -g -gdwarf-2 -gstrict-dwarf")
  message("Strict Dwarf:       enabled")
else()
  message("Strict Dwarf:       disabled")
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
  message("Function profiling: Enabled")
else()
  message("Function profiling: Disabled")
endif()

if (instrument_functions)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finstrument-functions")
  ADD_SOURCE("src/lib/trace.c")
endif()

RUMBOOT_PLATFORM_PRINT_SUMMARY()

SET(CMAKE_C_FLAGS "${EXTRA_C_FLAGS} ${CMAKE_C_FLAGS} -DRUMBOOT_SOC_${RUMBOOT_SOC_BUILD_TYPE}")
message("C Flags:            ${CMAKE_C_FLAGS}")
message("ASM Flags:          ${CMAKE_ASM_FLAGS}")
if (RUMBOOT_EXTERNAL_LIBC)
message(STATUS "--------------------------------------------------------------------------------")
message(STATUS "Using external libc. This is a NOT 100% supported use case")
message(STATUS "Dragons ahead! This may result in weird bugs and no support.")
message(STATUS "Proceed at your own risk. You have been warned")
message(STATUS "--------------------------------------------------------------------------------")
  message("External libc:      Yes")
  message("Libc headers path:  ${RUMBOOT_LIBC_PATH_INC}")
  message("Libc lib path:      ${RUMBOOT_LIBC_PATH_LIB}")
endif()
message(" ==== ==== ==== ==== ==== ==== ==== ====\n\n")


ADD_SOURCE("src/lib/algo/crc32.c")
ADD_SOURCE("src/lib/algo/aes.c")
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
ADD_SOURCE("src/lib/drivers/mdma_simple_lib.c")
ADD_SOURCE("src/lib/drivers/muart.c")
ADD_SOURCE("src/lib/drivers/eeprom.c")
ADD_SOURCE("src/lib/hexdump.c")
ADD_SOURCE("src/lib/drivers/mgeth.c")
ADD_SOURCE("src/lib/drivers/mdma_chan_lib.c")
ADD_SOURCE("src/lib/drivers/wdt.c")
ADD_SOURCE("src/lib/drivers/mkio.c")
ADD_SOURCE("src/lib/drivers/pl022.c")
ADD_SOURCE("src/lib/drivers/pl022_flash.c")
ADD_SOURCE("src/lib/bootsrc/boilerplate.c")
ADD_SOURCE("src/lib/bootsrc/physmap.c")
ADD_SOURCE("src/lib/malloc.c")
ADD_SOURCE("src/lib/xmodem.c")
ADD_SOURCE("src/lib/eventsystem/offload.c")
ADD_SOURCE("src/lib/ini.c")
ADD_SOURCE("src/lib/drivers/uart_pl011.c")
ADD_SOURCE("src/lib/drivers/greth_edcl.c")
ADD_SOURCE("src/lib/version.c")
ADD_SOURCE("src/lib/bootdbg.c")
ADD_SOURCE("src/lib/memtest.c")
ADD_SOURCE("src/lib/bitswapper.c")
ADD_SOURCE("src/lib/drivers/nfifo.c")
ADD_SOURCE("src/lib/weaklings.c")
ADD_SOURCE("src/lib/drivers/gpio_pl061.c")
ADD_SOURCE("src/lib/drivers/rcm_cp.c")
#ADD_SOURCE("src/lib/drivers/interprocessor_irq_lib.c")

if (RUMBOOT_ARCH MATCHES ppc)
  ADD_SOURCE("src/arch/ppc/exception.c")
  ADD_SOURCE("src/arch/ppc/ppc_mmu_impl.S")
  ADD_SOURCE("src/arch/ppc/p64.S")
  ADD_SOURCE("src/lib/drivers/ugly/plb6mcif2.c")
  ADD_SOURCE("src/lib/drivers/ugly/emi.c")
  ADD_SOURCE("src/lib/drivers/ugly/greth.c")
  ADD_SOURCE("src/arch/ppc/dma2plb6.c")
endif()

if (NOT RUMBOOT_PLATFORM STREQUAL "native")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (RUMBOOT_ARCH MATCHES ppc)
      ADD_SOURCE("src/lib/eventsystem/eventchannel-memory_spr.c")
    else()
      ADD_SOURCE("src/lib/eventsystem/eventchannel-memory.c")
    endif()
    ADD_SOURCE("src/lib/eventsystem/io-eventchannel.c")
    ADD_SOURCE("src/lib/eventsystem/wrappers-simulation.c")
    set(EXTRA_COMMON_FLAGS "${EXTRA_COMMON_FLAGS} -DRUMBOOT_PRINTF_ACCEL")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Production")
    ADD_SOURCE(src/lib/eventsystem/eventchannel-memory.c)
    ADD_SOURCE("src/lib/eventsystem/wrappers-simulation.c")
    elseif(CMAKE_BUILD_TYPE STREQUAL "PostProduction")
    ADD_SOURCE(src/lib/eventsystem/eventchannel-serial.c)
    ADD_SOURCE("src/lib/eventsystem/wrappers-serial.c")
    endif()
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${EXTRA_COMMON_FLAGS}")


if (RUMBOOT_PLATFORM MATCHES native)
    ADD_SOURCE("src/lib/eventsystem/wrappers-simulation.c")
    ADD_SOURCE("src/rumboot_main_native.c")
    ADD_SOURCE("src/lib/bootsrc/file.c")
else()
    ADD_SOURCE("src/rumboot_main.c")
    ADD_SOURCE("src/lib/newlib-stubs.c")
    ADD_SOURCE("src/lib/gcov.c")
endif()

file(GLOB LUA_FILES src/lib/lua-5.1.5/src/*.c)
include_directories(src/lib/lua-5.1.5/src/)

ADD_LIBRARY(lua STATIC ${LUA_FILES})
foreach(c ${RUMBOOT_CONFIGURATIONS})
  list (FIND CONFIGURATION_${c}_FEATURES "NOLIBS" _index)
  if (NOT ${_index} GREATER -1)
    ADD_LIBRARY(rumboot-${c} OBJECT ${PLATFORM_SOURCES})

    target_compile_options(rumboot-${c} PUBLIC
      $<$<COMPILE_LANGUAGE:C>:${CONFIGURATION_${c}_CFLAGS} ${CONFIGURATION_${c}_CCFLAGS} ${CONFIGURATION_${c}_OPTIMIZE}>
      $<$<COMPILE_LANGUAGE:CXX>:${CONFIGURATION_${c}_CFLAGS} ${CONFIGURATION_${c}_CXXFLAGS} ${CONFIGURATION_${c}_OPTIMIZE}>
      $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp ${CONFIGURATION_${c}_CFLAGS} ${CONFIGURATION_${c}_ASFLAGS} ${CONFIGURATION_${c}_OPTIMIZE}>
    )

  endif()
endforeach()

set(RUMBOOT_PLATFORM_TARGET_DIR ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/)
set(RUMBOOT_PLATFORM_COMMON_DIR ${CMAKE_SOURCE_DIR}/src/platform/)
file(REMOVE_RECURSE ${PROJECT_BINARY_DIR}/runners)

RUMBOOT_PLATFORM_ADD_COMPONENTS()

get_property(idirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(idir ${idirs})
  set(RUMBOOT_INCLUDE_DIRS "${RUMBOOT_INCLUDE_DIRS} ${idir}")
endforeach()

  CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
  )

if (RUMBOOT_COVERAGE)
    if (CROSS_COMPILE)
      set(GCOV ${CROSS_COMPILE}-gcov)
    else()
      set(GCOV gcov)
    endif()

    add_custom_target(coverage
      COMMAND lcov --gcov-tool=${GCOV} -c -o coverage.info -d ${PROJECT_BINARY_DIR}
      COMMAND genhtml -o coverage_report coverage.info
    )
endif()

if (DOXYGEN_FOUND)
  add_custom_target(docs COMMAND doxygen > doxygen.log)
endif()

if (NOT RUMBOOT_BUILD_NPE_RM)
  set(RUMBOOT_BUILD_NPE_RM No)
  set(NPE_RM_OPTS "")
else()
  if(DEFINED DUT)
    set(NPE_RM_OPTS "-DDUT=${DUT}")
  else()
    set(NPE_RM_OPTS "")
  endif()
endif()

include(ProcessorCount)
ProcessorCount(NUM_THREADS)
include(ExternalProject)
ExternalProject_Add(utils
    SOURCE_DIR        "${CMAKE_SOURCE_DIR}/utils"
    BINARY_DIR        "${CMAKE_BINARY_DIR}/utils"
    CONFIGURE_COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}/utils -G ${CMAKE_GENERATOR} -DRUMBOOT_BUILD_NPE_RM=${RUMBOOT_BUILD_NPE_RM} ${NPE_RM_OPTS}
#    BUILD_COMMAND     ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}/utils" -- -j ${NUM_THREADS}
    BUILD_COMMAND     "$(MAKE)" -C "${CMAKE_BINARY_DIR}/utils"
    INSTALL_COMMAND   ""
#    Not supported by cmake 2.8.x
#    BUILD_ALWAYS Yes
    )

add_custom_target(utils-check-rebuild
  ALL
  DEPENDS utils
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/utils"
  COMMAND     "$(MAKE)" -C "${CMAKE_BINARY_DIR}/utils"  
)

SET(CPACK_GENERATOR "TGZ")
SET(CPACK_PACKAGE_FILE_NAME "rumboot-${RUMBOOT_PLATFORM}-${CMAKE_BUILD_TYPE}-${GIT_BRANCH}-${GIT_REVISION}")
SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
INCLUDE(CPack)

macro(check_for_optimization_flag var flag)
string(FIND "${${var}}" "${flag}" _opt)
if (NOT _opt STREQUAL "-1")
  message(FATAL_ERROR "${var} contains ${flag}. Please, do not add optimisation flags explicitly, use OPTIMIZE directive. See README.md")
endif()
endmacro()

macro(check_for_optimization_flags var)
  set(_flags "-Os;-O0;-O1;-O2;-O3;-Og")
  foreach(f ${_flags})
    check_for_optimization_flag(${var} "${f}")    
  endforeach()
endmacro()

check_for_optimization_flags(CMAKE_C_FLAGS)
check_for_optimization_flags(CMAKE_ASM_FLAGS)
check_for_optimization_flags(CMAKE_CXX_FLAGS)