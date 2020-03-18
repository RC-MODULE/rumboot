SET(RUMBOOT_ARCH ppc)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/arch/ppc/exception.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-mpic128.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/uart_pl011.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    set(RUMBOOT_COMMON_FLAGS "-mcpu=476fp -gdwarf-2 -m32 -ffreestanding -mbig-endian -fno-zero-initialized-in-bss")
    set(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -mstrict-align -fdata-sections -ffunction-sections ")
    set(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "-fno-zero-initialized-in-bss -e rumboot_main -Wl,--oformat=elf32-powerpc -static -nostartfiles -Wl,--gc-sections")
    set(CMAKE_DUMP_FLAGS -S -EB -M476,32)

endmacro()

rumboot_add_configuration(
  ROM
  PREFIX rom
  LDS mm7705/rom-shim.lds
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader_legacy.c
  CFLAGS -DRUMBOOT_ONLY_STACK
  #LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group
  FEATURES PACKIMAGE
)

#Add configuration for binaries

rumboot_add_configuration(
  IRAM
  LDS mm7705/spl.lds
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group
  PREFIX spl
  FEATURES PACKIMAGE
)

rumboot_add_configuration(
  SPL
  CONFIGURATION IRAM
  CFLAGS  -DRUMBOOT_NOINIT
)

rumboot_add_configuration(
  LEGACY
  DEFAULT
  LDS mm7705/ram.lds
  PREFIX iram
  CFLAGS
  FEATURES PACKIMAGE
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader_legacy.c
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group
)


macro(rumboot_platform_generate_stuff_for_taget product)

endmacro()


include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  rumboot_bootrom_add_components(SPL ROM)

  add_rumboot_target_dir(tests
    CONFIGURATION IRAM
  )

  add_rumboot_target(
    FILES common/updaters/spiflash-pl022.c
    CONFIGURATION SPL
    CFLAGS -DBOOT_ID=1
    PREFIX updater
    NAME spiflash
    FEATURES STUB
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    CFLAGS -DUSE_SWINT=132
    FILES common/irq/irq-atomics.c
    PREFIX "irq"
)


endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)
endfunction()

if (NOT CROSS_COMPILE)
  SET(CROSS_COMPILE powerpc-rcm-elf)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
  if (EXISTS /opt/r42/toolchains/powerpc-rcm-elf/bin/powerpc-rcm-elf-gcc)
    set(CROSS_COMPILE "/opt/r42/toolchains/powerpc-rcm-elf/bin/${CROSS_COMPILE}")
  endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
