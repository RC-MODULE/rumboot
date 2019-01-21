SET(RUMBOOT_ARCH arm)
SET(RUMBOOT_PLATFORM bbp3)

set(RUMBOOT_PLATFORM_DEFAULT_LDS bbp3/rom.lds)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT boot)


if (RUMBOOT_BUILD_TYPE STREQUAL "Production")
  set(BOOTROM_IFLAGS ${RTLFLAGS} +BOOTMGR_KEEP_DRIVING=1 +GTUBE_ONLY_PRODUCTION_OPCODES)
else()
  set(BOOTROM_IFLAGS ${RTLFLAGS})
endif()

#These are configurations for our binaries
rumboot_add_configuration(
  ROM
  DEFAULT
  SNAPSHOT boot
  LDS bbp3/rom.lds
  LDFLAGS "-e rumboot_reset_handler"
  CFLAGS -DRUMBOOT_ONLY_STACK -marm
  PREFIX ROM
  TIMEOUT_CTEST 200000
  TIMEOUT 100 ms
  IRUN_FLAGS ${BOOTROM_IFLAGS}
  FEATURES ROMGEN
)

rumboot_add_configuration (
  IRAM
  LDS bbp3/iram-spl.lds
  PREFIX iram
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e rumboot_main"
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  CFLAGS -marm
  BOOTROM bootrom-stub
  TIMEOUT_CTEST 200000
  FEATURES LUA COVERAGE PACKIMAGE
  LOAD IM0BIN SELF
)

rumboot_add_configuration (
    IRAM_SPL
    CONFIGURATION IRAM
    PREFIX spl
    LDS bbp3/iram-spl.lds
    LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e main"
    CFLAGS  -DRUMBOOT_NOINIT
    FEATURES COVERAGE PACKIMAGE
)


include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

set(ROM_115200_OPTS +BOOT_FASTUART=0 +UART0_SPEED=115200 )
set(ROM_6500K_OPTS  +BOOT_FASTUART=1 +UART0_SPEED=6250000)

### Add tests here ###
#WARNING! Full regression automatically includes all tests from the short ones
macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)

    rumboot_bootrom_add_components(
      IRAM_SPL ROM
      -a 512 -z 512)

    rumboot_bootrom_unit_test(
        ID 0
        CONFIGURATION ROM
        TAG spi0_cs0
        MEMTAG SPI0_CONF
        TAGOFFSET 0
        FULL YES
        IRUN_FLAGS ${ROM_6500K_OPTS}
        ENDIAN little
    )

    rumboot_bootrom_unit_test(
        ID 0
        CONFIGURATION ROM
        TAG nor_cs0
        MEMTAG NOR_IMAGE
        TAGOFFSET 0
        FULL YES
        IRUN_FLAGS ${ROM_6500K_OPTS}
        ENDIAN little
    )

    rumboot_bootrom_integration_test(BROM
        NAME "serial-115200"
        IRUN_FLAGS ${ROM_115200_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
        TIMEOUT 10 ms
    )
    rumboot_bootrom_integration_test(BROM
        NAME "serial-6500000"
        IRUN_FLAGS ${ROM_6500K_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
        TIMEOUT 10 ms
    )


        rumboot_bootrom_integration_test(BROM
            NAME "host"
            IRUN_FLAGS ${ROM_6500K_OPTS}
            LOAD
              SD0_BOOT_IMAGE spl-fail
              SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
              NOR_IMAGE spl-fail-bad-magic
              HOSTMOCK  spl-ok
        )

    add_rumboot_target(
        CONFIGURATION ROM
        FILES hello.c
      )

endmacro()

if (CMAKE_VERILOG_RULES_LOADED)
    return()
endif()

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/arch/arm/startup.S
    ${CMAKE_SOURCE_DIR}/src/arch/arm/exception.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-gic.c
    ${CMAKE_SOURCE_DIR}/src/lib/eventsystem-memory.c
    ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/dit_lib.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-mcpu=cortex-a5 -mapcs-frame -mfpu=vfpv3-d16 -mfloat-abi=hard -ffreestanding -Wno-error=cpp")
    SET(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -Wall -fdata-sections -ffunction-sections -DRUMBOOT_PLATFORM_NUM_HEAPS=8")
    SET(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    SET(CMAKE_OBJCOPY_FLAGS --gap-fill 0x00 --pad-to 32768)
    SET(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -static -Wl,--gc-sections")
    SET(CMAKE_DUMP_FLAGS     "-EL")
endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)

endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)

  list (FIND TARGET_FEATURES "ROMGEN" _index)
  if (${_index} GREATER -1)
    add_custom_command(
      OUTPUT ${product}.hex/image_mem64_0.hex
      COMMAND mkdir -p ${product}.hex
      COMMAND ${CMAKE_BINARY_DIR}/utils/romgen -i ${product}.bin -o ${product}.hex
      COMMENT "Generating HEX memory files for ${product}.bin"
      DEPENDS ${product}.bin utils
    )

    add_custom_target(
      ${product}.hex ALL
      DEPENDS ${product}.hex/image_mem64_0.hex
    )

    list (FIND TARGET_FEATURES "CPACK" _index)
    if (${_index} GREATER -1)
            install(DIRECTORY ${PROJECT_BINARY_DIR}/${product}.hex DESTINATION rumboot)
    endif()

    add_dependencies(${product}.all ${product}.hex)
  endif()


endmacro()


if (NOT CROSS_COMPILE)
  SET(CROSS_COMPILE arm-rcm-eabihf)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
  if (EXISTS /opt/r42/toolchains/arm-rcm-eabihf/bin/arm-rcm-eabihf-gcc)
    set(CROSS_COMPILE "/opt/r42/toolchains/arm-rcm-eabihf/bin/${CROSS_COMPILE}")
  endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
