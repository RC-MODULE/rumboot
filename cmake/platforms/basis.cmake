SET(RUMBOOT_ARCH arm)
SET(RUMBOOT_PLATFORM basis)

set(RUMBOOT_PLATFORM_DEFAULT_LDS basis/rom.lds)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)


if (RUMBOOT_SOC_BUILD_TYPE STREQUAL "RTL")
  set(RTLFLAGS +pcie_mbist_model)
endif()

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
  LDS basis/rom.lds
  LDFLAGS "-e rumboot_reset_handler"
  CFLAGS -DRUMBOOT_ONLY_STACK -DRUMBOOT_MAIN_NORETURN
  PREFIX ROM
  TIMEOUT_CTEST 200000
  TIMEOUT 100 ms
  IRUN_FLAGS ${BOOTROM_IFLAGS}
  FEATURES ROMGEN
)

rumboot_add_configuration(
  DROM
  SNAPSHOT default
  LDS basis/rom-with-data.lds
  LDFLAGS "-e rumboot_reset_handler"
  CFLAGS  -DRUMBOOT_DATA_FROM_ROM
  PREFIX DROM
  TIMEOUT_CTEST 1000
  FEATURES ROMGEN COVERAGE
)

#These are configurations for im0 binaries
rumboot_add_configuration (
  IRAM
  LDS basis/iram.lds
  PREFIX iram
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e rumboot_main"
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c 
  CFLAGS -g
  BOOTROM bootrom-stub
  TIMEOUT_CTEST 200000
  FEATURES LUA COVERAGE PACKIMAGE BANNER
  LOAD IM0BIN SELF
)

rumboot_add_configuration (
    IRAM_SPL
    CONFIGURATION IRAM
    PREFIX spl
    LDS basis/iram-spl.lds
    LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e main"
    CFLAGS  -DRUMBOOT_NOINIT
    FEATURES COVERAGE PACKIMAGE
)

rumboot_add_configuration (
  IRAM_WITH_DDR
  LDS basis/iram.lds
  PREFIX iram_with_ddr
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e rumboot_main"
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  CFLAGS  -DRUMBOOT_BASIS_ENABLE_DDR
  BOOTROM bootrom-stub-mirror
  TIMEOUT_CTEST 200000
  FEATURES LUA COVERAGE PACKIMAGE
  LOAD IM0BIN SELF
)

rumboot_add_configuration (
  IRAM_MIRROR
  LDS basis/iram.lds
  PREFIX iram-mirror
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e rumboot_main"
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  CFLAGS  -DRUMBOOT_BASIS_ENABLE_MIRROR -DRUMBOOT_BASIS_ENABLE_DDR
  BOOTROM bootrom-stub-mirror
  TIMEOUT_CTEST 200000
  FEATURES LUA COVERAGE PACKIMAGE
  LOAD IM0BIN SELF
)

rumboot_add_configuration(
  LPROBE_CPU
  PREFIX lprobe-cpu
  BOOTROM bootrom-lprobe-stub
  FEATURES LPROBE
  IRUN_FLAGS +LPROBE_MODE=CPU -input ${CMAKE_SOURCE_DIR}/../scripts/lprobe-helper.tcl
)

rumboot_add_configuration(
  LPROBE_PCIE
  PREFIX lprobe-pcie
  BOOTROM bootrom-noop-stub
  FEATURES LPROBE
  IRUN_FLAGS +LPROBE_MODE=PCIE -input ${CMAKE_SOURCE_DIR}/../scripts/lprobe-helper.tcl
)

rumboot_add_configuration(
  INTEGRATION
  PREFIX integration
  BOOTROM bootrom-lprobe-stub
  DEPENDS spl-ok spl
  FEATURES PACKIMAGE
)

include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

set(ROM_115200_OPTS +BOOT_FASTUART=0 +UART0_SPEED=115200  +I2C0_SLV_DISABLE)
set(ROM_6500K_OPTS  +BOOT_FASTUART=1 +UART0_SPEED=6250000 +I2C0_SLV_DISABLE)

### Add tests here ###
#WARNING! Full regression automatically includes all tests from the short ones
macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)

  rumboot_bootrom_add_components(ROM IRAM_SPL FALSE FALSE)

  rumboot_bootrom_add_common_units(
        CONFIGURATION ROM
        IRUN_FLAGS ${ROM_6500K_OPTS}
    )

  rumboot_bootrom_unit_test(
      ID 0
      TAG sdio0
      MEMTAG SD0_BOOT_IMAGE
      TAGOFFSET 0
      IRUN_FLAGS +select_sdio0 +BOOT_SD0_CD=0 ${ROM_6500K_OPTS}
      ENDIAN little
  )

  rumboot_bootrom_unit_test(
      ID 1
      TAG spi0_cs0
      MEMTAG SPI0_CONF
      TAGOFFSET 0
      IRUN_FLAGS ${ROM_6500K_OPTS}
      ENDIAN little
  )

  rumboot_bootrom_unit_test(
      ID 2
      TAG spi0_cs1
      MEMTAG SPI0_CONF
      TAGOFFSET 1
      FULL YES
      IRUN_FLAGS ${ROM_6500K_OPTS}
      ENDIAN little
  )

  rumboot_bootrom_unit_test(
      ID 3
      TAG i2c0_50
      MEMTAG I2C0_CONF
      TAGOFFSET 0
      IRUN_FLAGS ${ROM_6500K_OPTS}
      ENDIAN little
  )

  rumboot_bootrom_unit_test(
      ID 4
      TAG i2c0_51
      MEMTAG I2C0_CONF
      TAGOFFSET 1
      IRUN_FLAGS ${ROM_6500K_OPTS}
      ENDIAN little
  )

  rumboot_bootrom_unit_test(
      ID 5
      TAG i2c0_52
      MEMTAG I2C0_CONF
      TAGOFFSET 2
      IRUN_FLAGS ${ROM_6500K_OPTS}
      ENDIAN little
  )

  rumboot_bootrom_unit_test(
      ID 6
      TAG i2c0_53
      MEMTAG I2C0_CONF
      TAGOFFSET 3
      IRUN_FLAGS ${ROM_6500K_OPTS}
      ENDIAN little
  )

  rumboot_bootrom_unit_test(
      ID 7
      TAG sdio1
      MEMTAG SD1_BOOT_IMAGE
      TAGOFFSET 0
      IRUN_FLAGS +select_sdio1 +BOOT_SD1_CD=0 ${ROM_6500K_OPTS}
      ENDIAN little
  )

  rumboot_bootrom_integration_test(ROM
      NAME "sdio0-ok"
      IRUN_FLAGS +BOOT_SD0_CD=0 +select_sdio0 ${ROM_6500K_OPTS}
      LOAD
        SD0_BOOT_IMAGE spl-ok
        SPI0_CONF spl-fail,spl-fail
        I2C0_CONF spl-fail,spl-fail,spl-fail,spl-fail
        SD1_BOOT_IMAGE spl-fail
        HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(ROM
      NAME "spi0-cs0-ok"
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD
        SD0_BOOT_IMAGE spl-fail
        SPI0_CONF spl-ok,spl-fail
        I2C0_CONF spl-fail,spl-fail,spl-fail,spl-fail
        SD1_BOOT_IMAGE spl-fail
        HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(ROM
      NAME "spi0-cs1-ok"
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD
        SD0_BOOT_IMAGE spl-fail
        SPI0_CONF spl-fail-bad-magic,spl-ok
        I2C0_CONF spl-fail,spl-fail,spl-fail,spl-fail
        SD1_BOOT_IMAGE spl-fail
        HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(ROM
      NAME "i2c0-50"
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD
        SD0_BOOT_IMAGE spl-fail
        SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
        I2C0_CONF spl-ok,spl-fail,spl-fail,spl-fail
        SD1_BOOT_IMAGE spl-fail
        HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(ROM
      NAME "i2c0-51"
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD
        SD0_BOOT_IMAGE spl-fail
        SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
        I2C0_CONF spl-fail-bad-magic,spl-ok,spl-fail,spl-fail
        SD1_BOOT_IMAGE spl-fail
        HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(ROM
      NAME "i2c0-52"
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD
        SD0_BOOT_IMAGE spl-fail
        SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
        I2C0_CONF spl-fail-bad-magic,spl-fail-bad-magic,spl-ok,spl-fail
        SD1_BOOT_IMAGE spl-fail
        HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(ROM
      NAME "i2c0-53"
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD
        SD0_BOOT_IMAGE spl-fail
        SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
        I2C0_CONF spl-fail-bad-magic,spl-fail-bad-magic,spl-fail-bad-magic,spl-ok
        SD1_BOOT_IMAGE spl-fail
        HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(ROM
      NAME "sdio1-ok"
      IRUN_FLAGS
      IRUN_FLAGS +BOOT_SD1_CD=0 +select_sdio1 +I2C0_DISABLE ${ROM_6500K_OPTS}
      LOAD
        SD0_BOOT_IMAGE spl-fail
        SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
        I2C0_CONF spl-fail-bad-magic,spl-fail-bad-magic,spl-fail-bad-magic,spl-fail-bad-magic
        SD1_BOOT_IMAGE spl-ok
        HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(BROM
      NAME "host-mockup-fallthough"
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD
        SD0_BOOT_IMAGE spl-fail
        SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
        I2C0_CONF spl-fail-bad-magic,spl-fail-bad-magic,spl-fail-bad-magic,spl-fail-bad-magic
        SD1_BOOT_IMAGE spl-ok
        HOSTMOCK  spl-ok
  )

  rumboot_bootrom_integration_test(BROM
      NAME "host-mockup"
      IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS}
      LOAD
        HOSTMOCK  spl-ok
  )

  rumboot_bootrom_integration_test(BROM
      NAME "serial-115200"
      IRUN_FLAGS +BOOT_HOST=1 ${ROM_115200_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
      TIMEOUT 10 ms
  )
  rumboot_bootrom_integration_test(BROM
      NAME "serial-6500000"
      IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
      TIMEOUT 10 ms
  )

  if (NOT RUMBOOT_BUILD_TYPE STREQUAL "Debug")
      rumboot_bootrom_integration_test(BROM
          NAME "host-easter-egg"
          IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +uart_easter_egg
      )

      rumboot_bootrom_integration_test(BROM
          NAME "host-fallthough-easter-egg"
          IRUN_FLAGS ${ROM_6500K_OPTS} +uart_easter_egg
          LOAD
            SD0_BOOT_IMAGE spl-fail
            SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
            I2C0_CONF spl-fail-bad-magic,spl-fail-bad-magic,spl-fail-bad-magic,spl-fail-bad-magic
            SD1_BOOT_IMAGE spl-ok
      )

  endif()

  add_rumboot_target_dir(simple-rom/
    CONFIGURATION ROM
    PREFIX simple-rom)

  add_rumboot_target_dir(simple-iram/
      CONFIGURATION IRAM
      PREFIX simple-iram)

  add_rumboot_target_dir(simple-iram/
      CONFIGURATION IRAM_MIRROR
    PREFIX simple-iram-mirror)


    add_rumboot_target_dir(sprint/
        CONFIGURATION IRAM
        PREFIX simple-iram
        TESTGROUP sprint
      )

    add_rumboot_target_dir(sprint/
        CONFIGURATION IRAM_MIRROR
        PREFIX simple-iram-mirror
        TESTGROUP sprint
    )


  add_rumboot_target_dir(jenkins/
      CONFIGURATION ROM
      PREFIX jenkins
      TESTGROUP short
    )

    add_rumboot_target_dir(common/irq/
      CONFIGURATION ROM
      PREFIX irq-rom
      TESTGROUP short
    )

    add_rumboot_target_dir(common/irq/
      CONFIGURATION IRAM
      PREFIX irq-iram
      TESTGROUP short
    )

    add_rumboot_target_dir(common/irq/
      CONFIGURATION IRAM_MIRROR
      PREFIX irq-iram-mirror
    )

    add_rumboot_target_dir(common/lua/
      CONFIGURATION LPROBE_CPU
      PREFIX lprobe-cpu
    )

    add_rumboot_target_dir(lua/
      CONFIGURATION LPROBE_PCIE
      PREFIX lprobe-pcie
    )

    add_rumboot_target_dir(i2c/
      CONFIGURATION IRAM
      CFLAGS -DI2C_BASE=I2C0_BASE -DI2C_IRQ=I2C0_IRQ
      PREFIX i2c-0
    )

    add_rumboot_target_dir(i2c/
      CONFIGURATION IRAM
      CFLAGS -DI2C_BASE=I2C1_BASE -DI2C_IRQ=I2C1_IRQ
      PREFIX i2c-1
    )

   add_rumboot_target_dir(i2c/
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DI2C_BASE=I2C0_BASE -DI2C_IRQ=I2C0_IRQ
      PREFIX i2c-0-mirror
    )

    add_rumboot_target_dir(i2c/
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DI2C_BASE=I2C1_BASE -DI2C_IRQ=I2C1_IRQ
      PREFIX i2c-1-mirror
    )

   add_rumboot_target_dir(i2c/multimaster
      CONFIGURATION IRAM
    CFLAGS -DI2C_BASE=I2C0_BASE
    IRUN_FLAGS +i2c_single_bus
      PREFIX multimaster
    )
   add_rumboot_target_dir(i2c/multimaster
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DI2C_BASE=I2C0_BASE
      IRUN_FLAGS +i2c_single_bus
      PREFIX multimaster-mirror
    )
  add_rumboot_target_dir(arinc/
      CONFIGURATION IRAM
      PREFIX arinc
      TESTGROUP sprint
    )

  add_rumboot_target_dir(arinc/
      CONFIGURATION IRAM_MIRROR
      PREFIX arinc-mirror
    )

  add_rumboot_target_dir(arinc/arinc_ddr/
      CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=2 -Dheap_1=0
      PREFIX arinc_ddr_freq_100_ddr0_IM0
    )
  add_rumboot_target_dir(arinc/arinc_ddr/
       CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=2 -Dheap_1=1
      PREFIX arinc_ddr_freq_100_ddr0_IM1
    )
  add_rumboot_target_dir(arinc/arinc_ddr/
       CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=2 -Dheap_1=2
      PREFIX arinc_ddr_freq_100_ddr0_ddr0
    )
  add_rumboot_target_dir(arinc/arinc_ddr/
       CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=2 -Dheap_1=3
      PREFIX arinc_ddr_freq_100_ddr0_ddr1
    )

  add_rumboot_target_dir(arinc/arinc_ddr/
       CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=3 -Dheap_1=0
      PREFIX arinc_ddr_freq_100_ddr1_IM0
    )
  add_rumboot_target_dir(arinc/arinc_ddr/
       CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=3 -Dheap_1=1
      PREFIX arinc_ddr_freq_100_ddr1_IM1
    )
  add_rumboot_target_dir(arinc/arinc_ddr/
       CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=3 -Dheap_1=2
      PREFIX arinc_ddr_freq_100_ddr1_ddr0
    )
  add_rumboot_target_dir(arinc/arinc_ddr/
      CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=3 -Dheap_1=3
      PREFIX arinc_ddr_freq_100_ddr1_ddr1
    )

  add_rumboot_target_dir(arinc/arinc_ddr
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DTX_FREQ=tx_freq_12_5 -DRX_FREQ=rx_freq_12_5 -Dheap_0=4 -Dheap_1=4
      PREFIX arinc-mirror_arinc_ddr_freq_12_5_PCI_0
    )

  add_rumboot_target_dir(arinc/arinc_ddr
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DTX_FREQ=tx_freq_50 -DRX_FREQ=rx_freq_50 -Dheap_0=5 -Dheap_1=5
      PREFIX arinc-mirror_arinc_ddr_freq_50_PCI_1
    )

  add_rumboot_target_dir(arinc/arinc_ddr
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=4 -Dheap_1=5
      PREFIX arinc-mirror_arinc_ddr_freq_100_PCI_0_PCI_1
    )
  add_rumboot_target_dir(arinc/arinc_ddr
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=5 -Dheap_1=4
      PREFIX arinc-mirror_arinc_ddr_freq_100_PCI_1_PCI_0
    )
  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=0 -Dheap_1=0
      PREFIX arinc_freq_100_IM0
    )
  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=0 -Dheap_1=2
      PREFIX arinc_freq_100_IM0_ddr0
    )
  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=0 -Dheap_1=3
      PREFIX arinc_freq_100_IM0_ddr1
    )

  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=1 -Dheap_1=0
      PREFIX arinc_freq_100_IM1_IM0
    )
  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=1 -Dheap_1=2
      PREFIX arinc_freq_100_IM1_ddr0
    )
  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=1 -Dheap_1=3
      PREFIX arinc_freq_100_IM1_ddr1
    )

  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=4 -Dheap_1=4
      PREFIX arinc_freq_100_PCI_0
    )
  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=5 -Dheap_1=5
      PREFIX arinc_freq_100_PCI_1
    )
  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM
      CFLAGS -DTX_FREQ=tx_freq_50 -DRX_FREQ=rx_freq_50 -Dheap_0=1 -Dheap_1=1
      PREFIX arinc_freq_50_IM1
    )

  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_12_5 -DRX_FREQ=rx_freq_12_5 -Dheap_0=1 -Dheap_1=2
      PREFIX arinc_freq_12_5_IM1_ddr0
    )
  add_rumboot_target_dir(arinc/freq/
      CONFIGURATION IRAM_WITH_DDR
      CFLAGS -DTX_FREQ=tx_freq_12_5 -DRX_FREQ=rx_freq_12_5 -Dheap_0=1 -Dheap_1=3
      PREFIX arinc_freq_12_5_IM1_ddr1
    )
  add_rumboot_target_dir(arinc/freq
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DTX_FREQ=tx_freq_12_5 -DRX_FREQ=rx_freq_12_5 -Dheap_0=4 -Dheap_1=4
      PREFIX arinc-mirror_freq_12_5
    )

  add_rumboot_target_dir(arinc/freq
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DTX_FREQ=tx_freq_50 -DRX_FREQ=rx_freq_50 -Dheap_0=5 -Dheap_1=5
      PREFIX arinc-mirror_freq_50_PCI_1
    )

  add_rumboot_target_dir(arinc/freq
      CONFIGURATION IRAM_MIRROR
      CFLAGS -DTX_FREQ=tx_freq_100 -DRX_FREQ=rx_freq_100 -Dheap_0=4 -Dheap_1=5
      PREFIX arinc-mirror_freq_100_PCI_0_PCI_1
    )
  add_rumboot_target_dir(muart/
      CONFIGURATION IRAM
      PREFIX muart-iram
      IRUN_FLAGS +uart_test
    )
  add_rumboot_target_dir(muart/
      CONFIGURATION IRAM_MIRROR
      PREFIX muart-mirror
      IRUN_FLAGS +uart_test
  )
  add_rumboot_target_dir(muart/uart_direct
      CONFIGURATION IRAM
      PREFIX muart-uart_direct
      IRUN_FLAGS +uart_test
  )
  add_rumboot_target_dir(muart/uart_mirror
      CONFIGURATION IRAM_MIRROR
      PREFIX muart-uart_mirror
      IRUN_FLAGS +uart_test
  )
  add_rumboot_target_dir(muart/uart_direct_DDR
      CONFIGURATION IRAM_WITH_DDR
      PREFIX muart-direct_DDR
      IRUN_FLAGS +uart_test
  )
  add_rumboot_target_dir(muart/uart_direct_DDR
      CONFIGURATION IRAM_MIRROR
      PREFIX muart-mirror_DDR
      IRUN_FLAGS +uart_test
  )

#    add_rumboot_target(
#       CONFIGURATION IRAM_WITH_DDR
#       FILES muart/muart_AXI_DDR_PCIE.c
#       PREFIX muart_iram_with_DDR
#       NAME muart_AXI_DDR_PCIE
#   )

    add_rumboot_target_dir(mgeth/
        CONFIGURATION IRAM
        PREFIX mgeth-iram
    )
    add_rumboot_target_dir(mgeth/
        CONFIGURATION IRAM_MIRROR
        PREFIX mgeth-mirror
    )
    add_rumboot_target_dir(mgeth/geth_direct
        CONFIGURATION IRAM
        PREFIX mgeth-geth_direct
    )
    add_rumboot_target_dir(mgeth/geth_direct_jumbo_10000byte
        CONFIGURATION IRAM
        PREFIX mgeth-geth_direct_jumbo_10000byte
    )
    add_rumboot_target_dir(mgeth/geth_mirror
        CONFIGURATION IRAM_MIRROR
        PREFIX mgeth-geth_mirror
    )
    add_rumboot_target_dir(mgeth/geth_direct_ddr
        CONFIGURATION IRAM_WITH_DDR
        PREFIX mgeth-geth_direct_ddr
    )

    add_rumboot_target_dir(sctl/
        CONFIGURATION IRAM
        PREFIX sctl-iram
    )
    
    add_rumboot_target_dir(basis_stress_test/
        CONFIGURATION IRAM_MIRROR
        FILES basis_stress_test/stress_test.c
        CFLAGS -DRUMBOOT_BASIS_DIRECT_IRAM -DRUMBOOT_BASIS_DMA_MEM_ACCEL=3
        PREFIX "basis-stress-test"
        NAME "uart-eth-dma"
        IRUN_FLAGS +uart_test
    )

    add_rumboot_target(
        CONFIGURATION DROM
        FILES common/tools/drom-check.c
        NAME "hello-with-data"
    )

    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/bootrom-stubs/bootrom-stub.c
        PREFIX "bootrom"
        NAME "stub"
        FEATURES STUB
    )

    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/bootrom-stubs/bootrom-rvds-stub.c
        PREFIX "bootrom"
        NAME "stub-rvds"
        FEATURES STUB
    )

    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/bootrom-stubs/bootrom-stub.c
        PREFIX "bootrom"
        NAME "stub-mirror"
        CFLAGS -DRUMBOOT_MIRROR_STUB
        FEATURES STUB
    )

  add_rumboot_target(
          CONFIGURATION ROM
          FILES common/bootrom-stubs/bootrom-lprobe-stub.c
          PREFIX "bootrom"
          NAME "lprobe-stub"
          FEATURES STUB
    )

    add_rumboot_target(
            CONFIGURATION ROM
            FILES common/bootrom-stubs/bootrom-noop-stub.c
            PREFIX "bootrom"
            NAME "noop-stub"
            FEATURES STUB
      )

  add_rumboot_target(
          CONFIGURATION IRAM
          FILES common/tools/print-heaps.c
  )

  add_rumboot_target(
          CONFIGURATION IRAM_MIRROR
          FILES common/tools/print-heaps.c
  )

  add_rumboot_target(
          CONFIGURATION ROM
          FILES common/tools/print-heaps.c
  )


  add_rumboot_target(
    CONFIGURATION IRAM
    FILES spi/spi.c
    CFLAGS -DGSPI_BASE=GSPI0_BASE
    PREFIX spi-0
    TESTGROUP sprint
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES spi/spi.c
    CFLAGS -DGSPI_BASE=GSPI1_BASE
    PREFIX spi-1
    TESTGROUP sprint
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES spi/spi.c
    CFLAGS -DGSPI_BASE=GSPI0_BASE
    PREFIX spi-0-mirror
    TESTGROUP sprint
    )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES spi/spi.c
    CFLAGS -DGSPI_BASE=GSPI1_BASE
    PREFIX spi-1-mirror
    TESTGROUP sprint
    )


  add_rumboot_target(
    CONFIGURATION IRAM
    FILES sdio-spi/sdio_spi.c
    CFLAGS -DSDIO_BASE=SDIO0_BASE  -DGSPI_SDIO_IRQ=GSPI_SDIO0_IRQ
    IRUN_FLAGS +select_sdio0
    PREFIX sdio-spi-0
    TESTGROUP sprint
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES sdio-spi/sdio_spi.c
    CFLAGS -DSDIO_BASE=SDIO1_BASE  -DGSPI_SDIO_IRQ=GSPI_SDIO1_IRQ
    IRUN_FLAGS +select_sdio1
    PREFIX sdio-spi-1
    TESTGROUP sprint
  )

   add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES sdio-spi/sdio_spi.c
    CFLAGS -DSDIO_BASE=SDIO0_BASE   -DGSPI_SDIO_IRQ=GSPI_SDIO0_IRQ
    IRUN_FLAGS +select_sdio0
    PREFIX sdio-spi-0-mirror
    TESTGROUP sprint
    )

   add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES sdio-spi/sdio_spi.c
    CFLAGS -DSDIO_BASE=SDIO1_BASE  -DGSPI_SDIO_IRQ=GSPI_SDIO1_IRQ
    IRUN_FLAGS +select_sdio1
    PREFIX sdio-spi-1-mirror
    TESTGROUP sprint
    )

  add_rumboot_target(
   CONFIGURATION IRAM
   FILES gpio/gpio.c
   TIMEOUT 500 us
   IRUN_FLAGS +gpio_test
  )

  add_rumboot_target(
   CONFIGURATION IRAM
   FILES gpio/gpio_irq.c
   TIMEOUT 500 us
   IRUN_FLAGS +gpio_test
  )
  add_rumboot_target(
   CONFIGURATION IRAM_MIRROR
   FILES gpio/gpio.c
   IRUN_FLAGS +gpio_test
   PREFIX gpio_mirror
 )

  add_rumboot_target(
   CONFIGURATION IRAM_MIRROR
   FILES gpio/gpio_irq.c
   IRUN_FLAGS +gpio_test
   PREFIX gpio_irq_mirror
 )

  add_rumboot_target(
   CONFIGURATION IRAM_MIRROR
   FILES gpio/gpio_connect.c
   IRUN_FLAGS +gpio_station_mirror
 )
   add_rumboot_target(
   CONFIGURATION IRAM
   FILES gpio/gpio_connect.c
   IRUN_FLAGS +gpio_station
 )

  add_rumboot_target(
   CONFIGURATION IRAM
   FILES gpio/gpio_connect_random.c
   IRUN_FLAGS +gpio_test
 )


  add_rumboot_target(
   CONFIGURATION IRAM
   FILES can-adapter/can_adapter_0_test.c
   TIMEOUT 300 us
   IRUN_FLAGS +can_plus_adapter
  )

  add_rumboot_target(
   CONFIGURATION IRAM_MIRROR
   FILES can-adapter/can_adapter_0_test.c
   TIMEOUT 600 us
   IRUN_FLAGS +can_plus_adapter
  )


 add_rumboot_target(
   CONFIGURATION IRAM
   FILES can-adapter/can_adapter_1_test.c
   TIMEOUT 300 us
   IRUN_FLAGS +can_plus_adapter
  )

 add_rumboot_target(
   CONFIGURATION IRAM_MIRROR
   FILES can-adapter/can_adapter_1_test.c
   TIMEOUT 700 us
   IRUN_FLAGS +can_plus_adapter
  )

 add_rumboot_target(
   CONFIGURATION IRAM
   FILES can-adapter/can_adapter_2_test.c
   TIMEOUT 300 us
   IRUN_FLAGS +can_plus_adapter
  )

 add_rumboot_target(
   CONFIGURATION IRAM_MIRROR
   FILES can-adapter/can_adapter_2_test.c
   TIMEOUT 700 us
   IRUN_FLAGS +can_plus_adapter
  )

 add_rumboot_target(
    CONFIGURATION IRAM
    FILES bisr/bisr_hard_test.c
    NAME bisr_hard_test_rep
    TIMEOUT 400 us
    IRUN_FLAGS +bisr_error_injection_rep
    CFLAGS -DBISR_TEST_EXPECTED=BISR_MEM_GOOD
  )

 add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES bisr/bisr_hard_test.c
    NAME bisr_hard_test_rep
    TIMEOUT 1000 us
    IRUN_FLAGS +bisr_error_injection_rep
    CFLAGS -DBISR_TEST_EXPECTED=BISR_MEM_GOOD
  )

 add_rumboot_target(
    CONFIGURATION IRAM
    FILES bisr/bisr_hard_test.c
    NAME bisr_hard_test_nonrep
    TIMEOUT 400 us
    IRUN_FLAGS +bisr_error_injection_nonrep
    CFLAGS -DBISR_TEST_EXPECTED=BISR_MEM_FAIL
  )

 add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES bisr/bisr_hard_test.c
    NAME bisr_hard_test_nonrep
    TIMEOUT 1000 us
    IRUN_FLAGS +bisr_error_injection_nonrep
    CFLAGS -DBISR_TEST_EXPECTED=BISR_MEM_FAIL
  )

 add_rumboot_target(
    CONFIGURATION IRAM
    FILES bisr/bisr_hard_test.c
    NAME bisr_hard_test_clear
    TIMEOUT 400 us
    CFLAGS -DBISR_TEST_EXPECTED=BISR_MEM_PERFECT
  )

 add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES bisr/bisr_hard_test.c
    NAME bisr_hard_test_clear
    TIMEOUT 1000 us
    CFLAGS -DBISR_TEST_EXPECTED=BISR_MEM_PERFECT
  )

 add_rumboot_target(
    CONFIGURATION IRAM
    FILES bisr/bisr_program_test.c
    NAME bisr_program_test_rep
    TIMEOUT 500 us
    IRUN_FLAGS +bisr_error_injection_rep
    CFLAGS -DBISR_TEST_EXPECTED=BISR_MEM_GOOD
  )

 add_rumboot_target(
    CONFIGURATION IRAM
    FILES bisr/bisr_program_test.c
    NAME bisr_program_test_nonrep
    TIMEOUT 500 us
    IRUN_FLAGS +bisr_error_injection_nonrep
    CFLAGS -DBISR_TEST_EXPECTED=BISR_MEM_FAIL
  )

 add_rumboot_target(
    CONFIGURATION IRAM
    FILES bisr/bisr_program_test.c
    NAME bisr_program_clear
    TIMEOUT 500 us
    CFLAGS -DBISR_TEST_EXPECTED=BISR_MEM_PERFECT
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_test.c
    NAME mdio_test_0
    TIMEOUT 300 us
    CFLAGS -DMDIO_NUM=0
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_test.c
    NAME mdio_test_1
    IRUN_FLAGS +mdio_test
    TIMEOUT 300 us
    CFLAGS -DMDIO_NUM=1
    TESTGROUP sprint
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_test.c
    NAME mdio_test_2
    TIMEOUT 300 us
    CFLAGS -DMDIO_NUM=2
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_test.c
    NAME mdio_test_3
    TIMEOUT 300 us
    CFLAGS -DMDIO_NUM=3
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_test.c
    NAME mdio_test_0
    TIMEOUT 600 us
    CFLAGS -DMDIO_NUM=0
    TESTGROUP sprint
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_test.c
    NAME mdio_test_1
    IRUN_FLAGS +mdio_test
    TIMEOUT 4000 us
    CFLAGS -DMDIO_NUM=1
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_test.c
    NAME mdio_test_2
    TIMEOUT 4000 us
    CFLAGS -DMDIO_NUM=2
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_test.c
    NAME mdio_test_3
    TIMEOUT 4000 us
    CFLAGS -DMDIO_NUM=3
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_0
    TIMEOUT 100 us
    CFLAGS -DMDIO_NUM=0
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_1
    IRUN_FLAGS +mdio_test
    TIMEOUT 100 us
    CFLAGS -DMDIO_NUM=1
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_2
    TIMEOUT 100 us
    CFLAGS -DMDIO_NUM=2
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_3
    TIMEOUT 100 us
    CFLAGS -DMDIO_NUM=3
   )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_0
    TIMEOUT 800 us
    CFLAGS -DMDIO_NUM=0
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_1
    IRUN_FLAGS +mdio_test
    TIMEOUT 800 us
    CFLAGS -DMDIO_NUM=1
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_2
    TIMEOUT 800 us
    CFLAGS -DMDIO_NUM=2
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_3
    TIMEOUT 800 us
    CFLAGS -DMDIO_NUM=3
   )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES wdt/wdt_crg_iint.c
    NAME wdt_crg_iint_test
    TIMEOUT 1000 us
    TESTGROUP sprint
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES wdt/wdt_crg_eint.c
    NAME wdt_crg_eint_test
    TIMEOUT 1000 us
    IRUN_FLAGS +wdt_crg_eint_test_flag
    TESTGROUP sprint
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES wdt/wdt_crg_iint.c
    NAME wdt_crg_iint_test
    TIMEOUT 1000 us
    TESTGROUP sprint
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES wdt/wdt_crg_eint.c
    NAME wdt_crg_eint_test
    TIMEOUT 1000 us
    IRUN_FLAGS +wdt_crg_eint_test_flag
    TESTGROUP sprint
  )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_dma_eRP_test.c
      NAME pcie_dma_eRP_test
      CFLAGS -Dincrease_test_duration=1
      IRUN_FLAGS +pcie_line_interference
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_board_test_config.c
      NAME pcie_board_test_config
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_board_test_config_rootport.c
      NAME pcie_board_test_config_rootport
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_board_config.c
      NAME pcie_board_config
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_irq_eRP_test.c
      NAME pcie_irq_eRP_test
      TESTGROUP sprint
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_to_pcie_access.c
      NAME pcie_to_pcie_access
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_dma_ddr0_performance_msr.c
      NAME pcie_dma_ddr0_performance_msr
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_dma_ddr1_performance_msr.c
      NAME pcie_dma_ddr1_performance_msr
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_int_msix_for_mirror.c
      NAME pcie_int_msix_for_mirror
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_at_slv_eRP_test_0.c
      NAME pcie_at_slv_eRP_test_0
      CFLAGS -Dshort_test_for_sim=1
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_at_typical_eRP_test_0.c
      NAME pcie_at_typical_eRP_test_0
      CFLAGS -Dshort_test_for_sim=1
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_at_mst_eRP_test_0.c
      NAME pcie_at_mst_eRP_test_0
      CFLAGS -Dshort_test_for_sim=1
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_legacy_eRP_test.c
      NAME pcie_legacy_eRP_test
      IRUN_FLAGS +pcie_legacy_int_elab
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_mbist_test.c
      NAME pcie_mbist_test
      IRUN_FLAGS ${RTLFLAGS}
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/ext_int_test.c
      NAME ext_int_test
      IRUN_FLAGS +ext_int_elab
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/ext_int_gpio_test.c
      NAME ext_int_gpio_test
      IRUN_FLAGS +ext_irq_to_gpio20
    )

  add_rumboot_target(
      CONFIGURATION ROM
      FILES can/can_loopback.c can/can_loopback.S can/int_send.S can/int_receive.S can/mem_config.S can/test_config.S
      NAME can_loopback
    )

  add_rumboot_target(
      CONFIGURATION ROM
      FILES can/can_request.c can/ccan_request.S can/int_send.S can/int_receive.S can/mem_config.S can/test_config.S
      NAME can_request
    )

  add_rumboot_target(
      CONFIGURATION ROM
      FILES can/can_fifo.c can/ccan_fifo.S can/int_send.S can/int_receive.S can/mem_config.S can/test_config.S
      NAME can_fifo
    )

  add_rumboot_target(
      CONFIGURATION ROM
      FILES can/can_full.c can/ccan_full.S can/int_send.S can/int_receive.S can/mem_config.S can/test_config.S
      NAME can_full
    )

  add_rumboot_target(
      CONFIGURATION ROM
      FILES can/can_maskfilter.c can/ccan_mask_filter.S can/int_send.S can/int_receive.S can/mem_config.S can/test_config.S
      NAME can_maskfilter
    )

    add_rumboot_target(
        CONFIGURATION IRAM_MIRROR
        FILES simple-iram/mdma_test.c
        PREFIX "mirror"
        NAME "mdma"
    )

   add_rumboot_target(
       CONFIGURATION IRAM_WITH_DDR
       FILES mdma_gp/test_suite_event.c mdma_gp/test_suite_base.c
       PREFIX "direct-event"
       NAME "mdma_gp_1-1_1-2_1-3_1-4"
       TESTGROUP mdma_gp
   )

   add_rumboot_target(
       CONFIGURATION IRAM
       FILES mdma_gp/test_suite_event.c mdma_gp/test_suite_base.c
       PREFIX "direct-event"
       NAME "mdma_gp_1-1_1-2_1-3_1-4_internal"
       TESTGROUP mdma_gp
  )

   add_rumboot_target (
       CONFIGURATION IRAM
       FILES mdma_gp/test_suite_memory.c mdma_gp/test_suite_base.c
       PREFIX "direct-memory"
       NAME "mdma_gp_2-1_2-2_2-3_internal"
       TESTGROUP mdma_gp
   )

   add_rumboot_target(
       CONFIGURATION IRAM_MIRROR
       FILES mdma_gp/test_suite_event.c mdma_gp/test_suite_base.c
       PREFIX "mirror-event"
       NAME "mdma_gp_1-1_1-2_1-3_1-5"
       TESTGROUP mdma_gp
   )

  add_rumboot_target(
      CONFIGURATION IRAM_MIRROR
      FILES mdma_gp/test_suite_memory.c mdma_gp/test_suite_base.c
      CFLAGS -DRUMBOOT_BASIS_MEM_ACCEL
      PREFIX "mirror-memory"
      NAME "mdma_gp_2-1_2-2_2-3_2-4"
      TESTGROUP mdma_gp
  )

  add_rumboot_target(
      CONFIGURATION IRAM_WITH_DDR
      FILES mdma_gp/test_suite_memory.c mdma_gp/test_suite_base.c
      CFLAGS -DRUMBOOT_BASIS_MEM_ACCEL
      PREFIX "direct-memory"
      NAME "mdma_gp_2-1_2-2_2-3"
      TESTGROUP mdma_gp
  )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES simple-iram/i2c_0_read_test.c
      NAME i2c_0_read_test_interference
      IRUN_FLAGS +i2c_interference_gen
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES simple-iram/i2c_1_read_test.c
      NAME i2c_1_read_test_interference
      IRUN_FLAGS +i2c_interference_gen
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/ext_irq_gen_reg_test.c
      NAME ext_irq_gen_reg_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_ctrl_reg_test.c
      NAME pcie_ctrl_reg_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_phy_bist_prbs31.S
      NAME pcie_phy_bist_prbs31
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_phy_bist_prbs31.c
      NAME pcie_phy_bist_prbs31_c
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_power_consumption.c
      NAME pcie_power_consumption
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_soft_reset_eRP_test.c
      NAME pcie_soft_reset_eRP_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_core_reg_test.c
      NAME pcie_core_reg_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_msix_eRP_test.c
      NAME pcie_msix_eRP_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_phy_reg_test.c
      NAME pcie_phy_reg_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/pcie_turn_on_eRP_test.c
      NAME pcie_turn_on_eRP_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/addr_trans_mst_reg_test.c
      NAME addr_trans_mst_reg_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES pcie/addr_trans_slv_reg_test.c
      NAME addr_trans_slv_reg_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_addr_test.c
      NAME ddr0_addr_test
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr_initializer.c
      CFLAGS -DDDR_HEADER_FILE="platform/ddr_config/ddr__mt41j512m8_187e_8_8_533__bist.h"
      NAME ddr_initializer_bist
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr_initializer.c
      CFLAGS -DDDR_HEADER_FILE="platform/ddr_config/ddr__mt41k256m8_125_8_11_800.h"
      NAME ddr_initializer
    )


  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_phy_bist_test_data.c
      NAME ddr0_phy_bist_test_data
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr1_phy_bist_test_data.c
      NAME ddr1_phy_bist_test_data
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_phy_bist_test_ctrl.c
      NAME ddr0_phy_bist_test_ctrl
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr1_phy_bist_test_ctrl.c
      NAME ddr1_phy_bist_test_ctrl
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_phy_bist_test_clk.c
      NAME ddr0_phy_bist_test_clk
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr1_phy_bist_test_clk.c
      NAME ddr1_phy_bist_test_clk
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_phy_bist_test_dll_data.c
      NAME ddr0_phy_bist_test_dll_data
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr1_phy_bist_test_dll_data.c
      NAME ddr1_phy_bist_test_dll_data
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_phy_bist_test_dll_ctrl.c
      NAME ddr0_phy_bist_test_dll_ctrl
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr1_phy_bist_test_dll_ctrl.c
      NAME ddr1_phy_bist_test_dll_ctrl
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_phy_bist_test_all.c
      NAME ddr0_phy_bist_test_all
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr1_phy_bist_test_all.c
      NAME ddr1_phy_bist_test_all
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr_loop_test/ddr0_loop_test.c ddr/ddr_loop_test/post_memory.c
      NAME ddr0_loop_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr_loop_test/ddr1_loop_test.c ddr/ddr_loop_test/post_memory.c
      NAME ddr1_loop_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr1_addr_test.c
      NAME ddr1_addr_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_reg_test.c
      NAME ddr0_reg_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr1_reg_test.c
      NAME ddr1_reg_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_low_power_test.c
      NAME ddr0_low_power_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr1_low_power_test.c
      NAME ddr1_low_power_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_phy_bist_test.S
      NAME ddr0_phy_bist_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr1_phy_bist_test.S
      NAME ddr1_phy_bist_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_ddr1_byte_test.c
      NAME ddr0_ddr1_byte_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES ddr/ddr0_ddr1_turn_on_test.c
      NAME ddr0_ddr1_turn_on_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES common/mkio/mkio_write_read_test.c
      NAME mkio_write_read_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES common/mkio/mkio0_irq_test.c
      NAME mkio0_irq_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES common/mkio/mkio1_irq_test.c
      NAME mkio1_irq_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES common/mkio/mkio_polarity_test.c
      NAME mkio_polarity_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES common/mkio/mkio0_events_timestamps_test.c
      NAME mkio0_events_timestamps_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES common/mkio/mkio1_events_timestamps_test.c
      NAME mkio1_events_timestamps_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM_MIRROR
      PREFIX "mirror"
      FILES common/mkio/mkio_write_read_test.c
      NAME mkio_write_read_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM_MIRROR
      PREFIX "mirror"
      FILES common/mkio/mkio0_irq_test.c
      NAME mkio0_irq_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM_MIRROR
      PREFIX "mirror"
      FILES common/mkio/mkio1_irq_test.c
      NAME mkio1_irq_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM_MIRROR
      PREFIX "mirror"
      FILES common/mkio/mkio_polarity_test.c
      NAME mkio_polarity_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM_MIRROR
      PREFIX "mirror"
      FILES common/mkio/mkio0_events_timestamps_test.c
      NAME mkio0_events_timestamps_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM_MIRROR
      PREFIX "mirror"
      FILES common/mkio/mkio1_events_timestamps_test.c
      NAME mkio1_events_timestamps_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES common/mkio/mkio_signal_test.c
      NAME mkio_signal_test
      IRUN_FLAGS +mkio_signal_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM_MIRROR
      PREFIX "mirror"
      FILES common/mkio/mkio_signal_test.c
      NAME mkio_signal_test
      IRUN_FLAGS +mkio_signal_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES esram/esram_rnd_space_test.c
      NAME esram_rnd_space_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES esram/esram0_heap_rnd_space_test.c
      NAME esram0_heap_rnd_space_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES esram/esram1_heap_rnd_space_test.c
      NAME esram1_heap_rnd_space_test
    )

  add_rumboot_target(
      CONFIGURATION IRAM
      FILES esram/esram_mia_test.c
      NAME esram_mia_test
    )

  add_rumboot_target(
        CONFIGURATION IRAM_SPL
        FILES configtool/ini_loader.c
        APPEND configtool/hello.ini
        NAME bconfig_test
      )

  add_rumboot_target(
    CONFIGURATION IRAM_SPL
    FILES configtool/ini_loader.c
    NAME bconfig
  )


  add_rumboot_target(
    CONFIGURATION IRAM_SPL
    FILES common/tools/serprog.c
    CFLAGS -DBOOT_ID=2
    PREFIX serprog
    NAME spi0-gpio0_5-cs
)

  add_rumboot_target(
        CONFIGURATION IRAM_SPL
        FILES common/updaters/spiflash-pl022.c
        CFLAGS -DBOOT_ID=1
        PREFIX updater
        NAME spi0-internal-cs
      )

  add_rumboot_target(
        CONFIGURATION IRAM_SPL
        FILES common/updaters/spiflash-pl022.c
        CFLAGS -DBOOT_ID=7
        PREFIX updater
        NAME spi1-internal-cs
  )

  add_rumboot_target(
        CONFIGURATION IRAM_SPL
        FILES common/updaters/spiflash-pl022.c
        CFLAGS -DBOOT_ID=2
        PREFIX updater
        NAME spi0-gpio0_5-cs
  )

  add_rumboot_target(
    CONFIGURATION IRAM_SPL
    FILES common/updaters/i2c.c
    CFLAGS -DI2C_BASE=I2C0_BASE -DEEPROM_ADDRESS=0x50
    PREFIX updater
    NAME i2c0-0x50
    FEATURES STUB
)

add_rumboot_target(
  CONFIGURATION IRAM_SPL
  FILES common/updaters/i2c.c
  CFLAGS -DI2C_BASE=I2C0_BASE -DEEPROM_ADDRESS=0x51
  PREFIX updater
  NAME i2c0-0x51
  FEATURES STUB
)

add_rumboot_target(
  CONFIGURATION IRAM_SPL
  FILES common/updaters/i2c.c
  CFLAGS -DI2C_BASE=I2C0_BASE -DEEPROM_ADDRESS=0x52
  PREFIX updater
  NAME i2c0-0x52
  FEATURES STUB
)

add_rumboot_target(
  CONFIGURATION IRAM_SPL
  FILES common/updaters/i2c.c
  CFLAGS -DI2C_BASE=I2C0_BASE -DEEPROM_ADDRESS=0x53
  PREFIX updater
  NAME i2c0-0x53
  FEATURES STUB
)


    foreach(conf IRAM IRAM_MIRROR)
    foreach(dit 0 1 2 3)
      add_rumboot_target(
        CONFIGURATION ${conf}
        NAME sp804-${dit}
        FILES common/sp804/dit.c
        CFLAGS -DDIT_BASE=DIT${dit}_BASE -DDIT_TIMINT1=DIT${dit}_TIMINT1 -DDIT_TIMINT2=DIT${dit}_TIMINT2
      )
    endforeach()
    endforeach()


#iram-ddr0_phy_bist_test,iram-ddr1_phy_bist_test,multimaster-i2c_multimst_write_random_read,,i2c-0-i2c_array4_write_random_read_stop_rpt,i2c-1-i2c_array4_write_random_read_stop_rpt,muart-iram-muart_AXI_GIC_dma
#

add_rumboot_target(
  CONFIGURATION IRAM_SPL
  FILES iram-memtest.c
  NAME micro-iram-spels-test
)

add_rumboot_target(
  CONFIGURATION IRAM_SPL
  FILES iram-memtest2.c
  NAME micro-iram-spels-test2
)

add_rumboot_target(
  CONFIGURATION IRAM
  FILES common/spels/mathtest.c
  NAME spels-mathtest
)

add_rumboot_target(
    CONFIGURATION IRAM
    IRUN_FLAGS +can_plus_adapter +i2c_single_bus
    FEATURES NOCODE
    COMBOIMAGE IM0BIN
     LOAD IM0BIN simple-iram-chain-start-dummy,sctl-iram-pull_up_disabled,spl-micro-iram-spels-test,iram-sp804-0,iram-sp804-1,iram-sp804-2,iram-sp804-3,iram-esram1_heap_rnd_space_test,iram-bisr_hard_test_clear,iram-can_adapter_0_test,mgeth-iram-mgeth_frame_xfer,iram-mkio_write_read_test,arinc_freq_100_IM1_IM0-arinc_loopback_16_freq,simple-iram-nic400,iram-can_adapter_1_test,iram-can_adapter_2_test,direct-event-mdma_gp_1-1_1-2_1-3_1-4_internal,direct-memory-mdma_gp_2-1_2-2_2-3_internal,iram-ddr0_phy_bist_test_all,iram-ddr1_phy_bist_test_all,iram-pcie_phy_bist_prbs31_c,simple-iram-chain-end-dummy
    NAME functional-test-chain
  )

  #muart-iram-muart_APB
  #muart-iram-muart_APB_GIC
  #muart-iram-muart_AXI_GIC_dma

  add_rumboot_target(
    CONFIGURATION IRAM
    IRUN_FLAGS +can_plus_adapter +i2c_single_bus
    FEATURES NOCODE
    COMBOIMAGE IM0BIN
     LOAD IM0BIN simple-iram-chain-start-dummy,iram-ext_int_gpio_test,simple-iram-chain-end-dummy
    NAME chain-test
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    IRUN_FLAGS +can_plus_adapter +i2c_single_bus
    FEATURES NOCODE
    COMBOIMAGE IM0BIN
     LOAD IM0BIN simple-iram-chain-start-dummy,sctl-iram-pull_up_disabled,simple-iram-chain-end-dummy
    NAME chain-test_sctl_pull_up_disabled
  )

  add_rumboot_target(
    CONFIGURATION IRAM_SPL
    FILES simple-iram/boot-spi1-redirect.c
    NAME micro-spi1-redirect
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    CFLAGS 
        -DDDR_RD
    FILES mdma_gp/ddr_mdma_stress_test.c
    PREFIX "ddr_and_mdma"
    NAME "rd_ddr0_wr_im1"
    TESTGROUP mdma_gp
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    CFLAGS 
        -DDDR_WR
    FILES mdma_gp/ddr_mdma_stress_test.c
    PREFIX "ddr_and_mdma"
    NAME "rd_mi1_wr_ddr0"
    TESTGROUP mdma_gp
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    CFLAGS -DGPIO_BASE=GPIO1_BASE
    FILES gpio/gpio_blink.c
    NAME "gpio_blink"
  )  
  
  add_rumboot_target(
      CONFIGURATION IRAM
      CFLAGS -DNO_DDR_INIT_IN_TEST_DDRi_ADDR_TEST
      FILES ddr/ddr0_addr_test.c
      NAME ddr0_addr_test_no_init
  )

  add_rumboot_target(
      CONFIGURATION IRAM
      CFLAGS -DNO_DDR_INIT_IN_TEST_DDRi_ADDR_TEST
      FILES ddr/ddr1_addr_test.c
      NAME ddr1_addr_test_no_init
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    IRUN_FLAGS +can_plus_adapter +i2c_single_bus
    FEATURES NOCODE
    COMBOIMAGE IM0BIN
    LOAD IM0BIN simple-iram-chain-start-dummy,iram-ddr_initializer,iram-ddr0_addr_test_no_init,iram-ddr1_addr_test_no_init,arinc-arinc_loopback_8,iram-mkio_write_read_test,iram-can_adapter_0_test,iram-can_adapter_1_test,iram-can_adapter_2_test,mgeth-geth_direct-mgeth_mem_access,i2c-0-i2c_driver_EEPROM,iram-pcie_board_config
#     LOAD IM0BIN simple-iram-chain-start-dummy,iram-ddr_initializer,iram-ddr0_addr_test
    NAME chain-test_mt14305
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES gpio/IRQ.c
    PREFIX "gpio"
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
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-proxy-gic-cdnpcie.c
    ${CMAKE_SOURCE_DIR}/src/lib/eventsystem-memory.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/ddr_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/pcie_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/pcie_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/dma_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/dit_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/bisr.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/mdio.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/nic400_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/gp_timers.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-DRUMBOOT_HEADER_VERSION=2 -g -mcpu=cortex-a5 -mapcs-frame -fno-omit-frame-pointer -mfpu=vfpv3-d16 -mfloat-abi=hard -marm -ffreestanding -Wno-error=cpp")
    SET(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -Wall -g -fdata-sections -ffunction-sections -DRUMBOOT_PLATFORM_NUM_HEAPS=8")
    if (PRODUCTION_TESTING) 
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPRODUCTION_TESTING")    
    endif()
    SET(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    SET(CMAKE_OBJCOPY_FLAGS --gap-fill 0x00 --pad-to 32768)
    SET(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -g -static -Wl,--gc-sections")
    SET(CMAKE_DUMP_FLAGS     "-EL")
endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)

endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)

  list (FIND TARGET_FEATURES "ROMGEN" _index)
  if (${_index} GREATER -1)
    add_custom_command(
      OUTPUT ${product}.hex/image_mem64_0.hex
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
