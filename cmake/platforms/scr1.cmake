SET(RUMBOOT_ARCH riscv)
SET(RUMBOOT_PLATFORM scr1)

set(RUMBOOT_PLATFORM_DEFAULT_LDS scr1/rom.lds)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT top)
set(RUMBOOT_NO_SELFTEST true)

if (RUMBOOT_BUILD_TYPE STREQUAL "Production")
  set(BOOTROM_IFLAGS ${RTLFLAGS} +BOOTMGR_KEEP_DRIVING=1 +GTUBE_ONLY_PRODUCTION_OPCODES)
else()
  set(BOOTROM_IFLAGS ${RTLFLAGS})
endif()

#These are configurations for our binaries
rumboot_add_configuration(
  ROM
  DEFAULT
  SNAPSHOT top
  LDS scr1/rom.lds
  LDFLAGS "-e _start"
  CFLAGS -DRUMBOOT_MAIN_NORETURN
  PREFIX ROM
  TIMEOUT_CTEST 200000
  TIMEOUT 1500 ms
  LOAD IRAM_IMAGE SELF
  IRUN_FLAGS ${BOOTROM_IFLAGS}
  FEATURES ROMGEN
)


macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)

    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/tools/print-heaps.c
      )
    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/bootrom/timer.c
    )



endmacro()

if (CMAKE_VERILOG_RULES_LOADED)
    return()
endif()

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
    ${CMAKE_SOURCE_DIR}/src/lib/eventsystem-memory.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-D__RVE_EXT -funroll-loops -fpeel-loops -fgcse-sm -fgcse-las -march=rv32imc -mabi=ilp32")
    SET(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -Wall -fdata-sections -ffunction-sections -DRUMBOOT_PLATFORM_NUM_HEAPS=8")
    SET(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    SET(CMAKE_OBJCOPY_FLAGS --gap-fill 0x00 --pad-to 16384)
    SET(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -static -Wl,--gc-sections")
    SET(CMAKE_DUMP_FLAGS     "-EL")
endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)

endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)

endmacro()


if (NOT CROSS_COMPILE)
  SET(CROSS_COMPILE riscv32-ncrmnt-elf)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
  if (EXISTS /opt/r42/toolchains/riscv32-ncrmnt-elf/bin/riscv32-ncrmnt-elf-gcc)
      set(CROSS_COMPILE "/opt/r42/toolchains/riscv32-ncrmnt-elf/bin/${CROSS_COMPILE}")
  endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
