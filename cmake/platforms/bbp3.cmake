SET(RUMBOOT_ARCH arm)
SET(RUMBOOT_PLATFORM bbp3)

set(RUMBOOT_PLATFORM_DEFAULT_LDS bbp3/rom.lds)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)
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
  SNAPSHOT default
  LDS bbp3/rom.lds
  LDFLAGS "-e rumboot_reset_handler"
  CFLAGS -DRUMBOOT_ONLY_STACK -marm
  PREFIX ROM
  TIMEOUT_CTEST 200000
  TIMEOUT 1500 ms
  IRUN_FLAGS ${BOOTROM_IFLAGS}
  FEATURES ROMGEN
)

# This configuration is for 
rumboot_add_configuration(
  UROM
  CONFIGURATION ROM
  CFLAGS -DRUMBOOT_ONLY_STACK -marm -DRUMBOOT_MAIN_NORETURN
)

rumboot_add_configuration (
  IRAM
  LDS bbp3/iram.lds
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
    FEATURES COVERAGE PACKIMAGE SPL
)


include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

set(ROM_115200_OPTS +UART0_SPEED=115200)

### Add tests here ###
#WARNING! Full regression automatically includes all tests from the short ones

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)

  add_rumboot_target(
    CONFIGURATION ROM
    FILES common/bootrom-stubs/bootrom-stub.c
    PREFIX "bootrom"
    NAME "stub"
    FEATURES STUB
  )

  add_rumboot_target_dir(common/irq/
    CONFIGURATION IRAM
    PREFIX iram
    TESTGROUP short
  )

  if (NOT RUMBOOT_BUILD_TYPE STREQUAL "PostProduction")

    rumboot_bootrom_add_components(
      IRAM_SPL ROM
      -a 512 -z 512)

    rumboot_bootrom_add_common_units(
        CONFIGURATION ROM
        IRUN_FLAGS ${ROM_115200_OPTS}
    )

    rumboot_bootrom_unit_test(
        ID 0
        CONFIGURATION UROM
        TAG spi0_cs0
        MEMTAG SPI0_CONF
        TAGOFFSET 0
        FULL YES
        IRUN_FLAGS ${ROM_115200_OPTS}
        ENDIAN little
    )

    rumboot_bootrom_unit_test(
        ID 0
        CONFIGURATION UROM
        TAG nor_cs0
        MEMTAG NOR_IMAGE
        TAGOFFSET 0
        FULL YES
        IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_EMI=1
        ENDIAN little
    )

    rumboot_bootrom_integration_test(ROM
        NAME "serial-115200"
        IRUN_FLAGS ${ROM_115200_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
    )


    rumboot_bootrom_integration_test(ROM
        NAME "host"
        IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_HOST=1
        LOAD
          SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
          NOR_IMAGE spl-fail-bad-magic
          HOSTMOCK  spl-ok
    )

    rumboot_bootrom_integration_test(ROM
    NAME "host-emi"
    IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_HOST=1 +BOOT_EMI_BIS=0 +full_boot_mode +boot_arm
    LOAD
      SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
      NOR_IMAGE spl-fail-bad-magic
      arm_image spl-ok.hex/image_mem64_0.hex
    )

    rumboot_bootrom_integration_test(ROM
    NAME "host-emi-silent"
    IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_HOST=1 +BOOT_SILENT=1 +BOOT_EMI_BIS=0 +full_boot_mode +boot_arm
    LOAD
      SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
      NOR_IMAGE spl-fail-bad-magic
      arm_image spl-ok.hex/image_mem64_0.hex
    )


    rumboot_bootrom_integration_test(ROM
    NAME "host-edcl"
    IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_HOST=1 +BOOT_EMI_BIS=0 +full_boot_mode +boot_arm
    SNAPSHOT edcl
    LOAD
      SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
      NOR_IMAGE spl-fail-bad-magic
      arm_image spl-ok.hex/image_mem64_0.hex
    )    

    rumboot_bootrom_integration_test(ROM
    NAME "host-edcl-silent"
    IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_HOST=1 +BOOT_SILENT=1 +BOOT_EMI_BIS=0 +full_boot_mode +boot_arm
    SNAPSHOT edcl
    LOAD
      SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
      NOR_IMAGE spl-fail-bad-magic
      arm_image spl-ok.hex/image_mem64_0.hex
    )    


if (TEST_XMODEM)
    rumboot_bootrom_integration_test(ROM
        NAME "host-xmodem"
        IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_HOST=1
        LOAD
          SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
          NOR_IMAGE spl-fail-bad-magic
          XMODEM0   spl-ok
    )
endif()

    rumboot_bootrom_integration_test(ROM
        NAME "host-silent"
        IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_SILENT=1 +BOOT_HOST=1
        LOAD
          SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
          NOR_IMAGE spl-fail-bad-magic
          HOSTMOCK  spl-ok
    )

    rumboot_bootrom_integration_test(ROM
        NAME "nor"
        IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_EMI=1
        LOAD
          SPI0_CONF spl-fail,spl-fail
          NOR_IMAGE spl-ok
          HOSTMOCK  spl-fail
    )

    rumboot_bootrom_integration_test(ROM
        NAME "spi"
        IRUN_FLAGS ${ROM_115200_OPTS}
        LOAD
          SPI0_CONF spl-ok,spl-fail
          NOR_IMAGE spl-fail
          HOSTMOCK  spl-fail
    )

    rumboot_bootrom_integration_test(ROM
        NAME "nor-silent"
        IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_EMI=1 +BOOT_SILENT=1
        LOAD
          SPI0_CONF spl-fail,spl-fail
          NOR_IMAGE spl-ok
          HOSTMOCK  spl-fail
    )

    rumboot_bootrom_integration_test(ROM
        NAME "spi-silent"
        IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_SILENT=1
        LOAD
          SPI0_CONF spl-ok,spl-fail
          NOR_IMAGE spl-fail
          HOSTMOCK  spl-fail
    )

    rumboot_bootrom_integration_test(ROM
        NAME "spi-fallthrough-host"
        IRUN_FLAGS ${ROM_115200_OPTS}
        LOAD
          SPI0_CONF spl-fail-bad-magic,spl-fail
          NOR_IMAGE spl-fail
          HOSTMOCK  spl-ok
    )

    rumboot_bootrom_integration_test(ROM
        NAME "nmc-nor"
        IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_EMI=1 +BOOT_NMC=1 +BOOT_EMI_BIS=1
        LOAD
          SPI0_CONF spl-fail,spl-fail
          NOR_IMAGE spl-ok
          HOSTMOCK  spl-fail
    )

    rumboot_bootrom_integration_test(ROM
        NAME "nmc-spi"
        IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_NMC=1 +BOOT_EMI_BIS=1
        LOAD
          SPI0_CONF spl-ok,spl-fail
          NOR_IMAGE spl-fail
          HOSTMOCK  spl-fail
    )

    rumboot_bootrom_integration_test(ROM
        NAME "nmc-host"
        IRUN_FLAGS ${ROM_115200_OPTS} +BOOT_NMC=1 +BOOT_EMI_BIS=0
        LOAD
          SPI0_CONF spl-fail,spl-fail
          NOR_IMAGE spl-fail
          HOSTMOCK  spl-ok
    )

    if (NOT RUMBOOT_BUILD_TYPE STREQUAL "Debug")
        rumboot_bootrom_integration_test(ROM
            NAME "host-easter-egg"
            IRUN_FLAGS +BOOT_HOST=1 ${ROM_115200_OPTS} +uart_easter_egg
        )

        rumboot_bootrom_integration_test(ROM
            NAME "host-fallthough-emi-easter-egg"
            IRUN_FLAGS ${ROM_115200_OPTS} +uart_easter_egg +BOOT_EMI=1
            LOAD
              SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
              NOR_IMAGE spl-fail-bad-magic
        )

        rumboot_bootrom_integration_test(ROM
            NAME "host-fallthough-spi-easter-egg"
            IRUN_FLAGS ${ROM_115200_OPTS} +uart_easter_egg
            LOAD
              SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
              NOR_IMAGE spl-fail-bad-magic
        )
    endif()
  endif()

    add_rumboot_target(
        CONFIGURATION IRAM
        NAME chain-test
        LOAD
          IM0BIN iram-irq-atomics,iram-irq-defhandler
        FEATURES NOCODE
      )

    add_rumboot_target(
        CONFIGURATION UROM
        FILES hello.c
      )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES oi10/targets/test_oi10_uart_000.c
      CFLAGS -DUARTRX_BASE=UART0_Base -DUARTTX_BASE=UART1_Base -DUARTRX_INT=UART0_INT -DUARTTX_INT=UART1_INT -DCHECK_REGISTERS
      PREFIX uart0
    )

    add_rumboot_target(
      CONFIGURATION IRAM_SPL
      FILES common/updaters/spiflash-pl022.c
      CFLAGS -DBOOT_ID=0
      PREFIX updater
      NAME spi0-cs0
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
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/dit_lib.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-mcpu=cortex-a5 -fomit-frame-pointer -mapcs-frame -mfpu=vfpv3-d16 -mfloat-abi=hard -ffreestanding -Wno-error=cpp")
    SET(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -Wall -fdata-sections -ffunction-sections -DRUMBOOT_PLATFORM_NUM_HEAPS=8")
    SET(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    SET(CMAKE_OBJCOPY_FLAGS --gap-fill 0x00 --pad-to 16384)
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

  list (FIND TARGET_FEATURES "SPL" _index)
  if (${_index} GREATER -1)
    add_custom_command(
      OUTPUT ${product}.hex/image_mem64_0.hex
      COMMAND mkdir -p ${product}.hex
      COMMAND ${CMAKE_BINARY_DIR}/utils/romgen -l bbp3bootemi -i ${product}.bin -o ${product}.hex
      COMMENT "Generating Verilog loadable HEX from ${product}.bin"
      DEPENDS ${product}.bin utils
    )

    add_custom_target(
      ${product}.hex ALL
      DEPENDS ${product}.hex/image_mem64_0.hex
    )

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
