SET(RUMBOOT_ARCH nmc3)
SET(RUMBOOT_PLATFORM nmc3)

set(RUMBOOT_PLATFORM_DEFAULT_LDS nmc3/generic.lds)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)

#These are configurations for our binaries
rumboot_add_configuration(
  IRAM
  DEFAULT
  PREFIX iram
  SNAPSHOT default
  LDS nmc3/generic.lds
  IRUN_FLAGS ${BOOTROM_IFLAGS}
)

rumboot_add_configuration(
  IRAM_SPL
  DEFAULT
  PREFIX iram
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c 
  SNAPSHOT default
  LDS nmc3/generic.lds
  IRUN_FLAGS ${BOOTROM_IFLAGS}
)


include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  add_rumboot_target_dir(iram/
    CONFIGURATION IRAM
  )

  rumboot_bootrom_add_components(
    IRAM_SPL IRAM
    -a 512 -z 512)

  rumboot_bootrom_add_common_units(
        CONFIGURATION IRAM
    )

  rumboot_bootrom_unit_test(
    ID 0
    TAG spi0
    ENDIAN little
)

endmacro()

if (CMAKE_VERILOG_RULES_LOADED)
    return()
endif()

file(GLOB PLATFORM_SOURCES
  ${CMAKE_SOURCE_DIR}/src/platform/nmc3/glue.c
  ${CMAKE_SOURCE_DIR}/src/platform/nmc3/bootglue.c
  ${CMAKE_SOURCE_DIR}/src/platform/nmc3/easynmc-io.c
  ${CMAKE_SOURCE_DIR}/src/platform/nmc3/easynmc-args.S
  ${CMAKE_SOURCE_DIR}/src/platform/nmc3/startup.S
  ${CMAKE_SOURCE_DIR}/src/arch/nmc3/exception.c
  )

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-fnmc-compatible-if-packed -mnmc3 -Xassembler -nmc3 -std=gnu99 -Wno-attributes")
    SET(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -Wall -fdata-sections -ffunction-sections -DRUMBOOT_PLATFORM_NUM_HEAPS=8")
    SET(CMAKE_ASM_FLAGS "-mmas ${RUMBOOT_COMMON_FLAGS}")
    SET(CMAKE_OBJCOPY_FLAGS )
    SET(CMAKE_EXE_LINKER_FLAGS "-e start -nostartfiles -Wl,--gc-sections")
    SET(CMAKE_DUMP_FLAGS     --byte-addr -S)
endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)

endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)


endmacro()


if (NOT CROSS_COMPILE)
  SET(CROSS_COMPILE nmc)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
endif()

set(CMAKE_C_COMPILER_WORKS 1)
