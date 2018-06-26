SET(RUMBOOT_ARCH arm)
SET(RUMBOOT_PLATFORM basis)

set(RUMBOOT_PLATFORM_DEFAULT_LDS basis/rom.lds)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)



#These are configurations for our binaries
rumboot_add_configuration(
  ROM
  DEFAULT
  SNAPSHOT default
  LDS basis/rom.lds
  LDFLAGS "-e rumboot_reset_handler"
  CFLAGS -DRUMBOOT_ONLY_STACK -DRUMBOOT_PRINTF_ACCEL
  PREFIX ROM
  FEATURES ROMGEN
)

rumboot_add_configuration(
  DROM
  SNAPSHOT default
  LDS basis/rom-with-data.lds
  LDFLAGS "-e rumboot_reset_handler"
  CFLAGS -DRUMBOOT_PRINTF_ACCEL -DRUMBOOT_DATA_FROM_ROM
  PREFIX DROM
  FEATURES ROMGEN COVERAGE
)

#These are configurations for im0 binaries
rumboot_add_configuration (
  IRAM
  LDS basis/iram.lds
  PREFIX iram
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e rumboot_main"
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  CFLAGS -DRUMBOOT_PRINTF_ACCEL
  BOOTROM bootrom-stub
  FEATURES LUA COVERAGE
)

rumboot_add_configuration (
  IRAM_MIRROR
  LDS basis/iram.lds
  PREFIX iram-mirror
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e rumboot_main"
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  CFLAGS -DRUMBOOT_PRINTF_ACCEL -DRUMBOOT_BASIS_ENABLE_MIRROR -DRUMBOOT_BASIS_ENABLE_DDR
  BOOTROM bootrom-stub-mirror
  FEATURES LUA COVERAGE
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
)


### Add tests here ###
#WARNING! Full regression automatically includes all tests from the short ones
macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  add_rumboot_target_dir(simple-rom/
    CONFIGURATION ROM
    PREFIX simple-rom)

  add_rumboot_target_dir(simple-iram/
      CONFIGURATION IRAM
      PREFIX simple-iram)

  add_rumboot_target_dir(simple-iram/
      CONFIGURATION IRAM_MIRROR
    PREFIX simple-iram-mirror)


  add_rumboot_target_dir(jenkins/
      CONFIGURATION ROM
      PREFIX jenkins
      TESTGROUP short
    )

    add_rumboot_target_dir(irq/
      CONFIGURATION ROM
      PREFIX irq-rom
      TESTGROUP short
    )

    add_rumboot_target_dir(irq/
      CONFIGURATION IRAM
      PREFIX irq-iram
      TESTGROUP short
    )

    add_rumboot_target_dir(irq/
      CONFIGURATION IRAM_MIRROR
      PREFIX irq-iram-mirror
    )

    add_rumboot_target_dir(lua/
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
    )
	add_rumboot_target_dir(arinc/
      CONFIGURATION IRAM_MIRROR
      PREFIX arinc-mirror
    )
	
    add_rumboot_target_dir(common/spl-stubs/
      CONFIGURATION IRAM
      PREFIX spl
      FEATURES STUB
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
          CONFIGURATION ROM
          FILES loader.c
          PREFIX "bootrom"
          NAME "production"
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
    FILES sdio-spi/sdio_spi.c
    CFLAGS -DSDIO_BASE=SDIO0_BASE  -DGSPI_SDIO_IRQ=GSPI_SDIO0_IRQ
    IRUN_FLAGS +select_sdio0
    PREFIX sdio-spi-0
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES sdio-spi/sdio_spi.c
    CFLAGS -DSDIO_BASE=SDIO1_BASE  -DGSPI_SDIO_IRQ=GSPI_SDIO1_IRQ
    IRUN_FLAGS +select_sdio1
    PREFIX sdio-spi-1
  )

   add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES sdio-spi/sdio_spi.c
    CFLAGS -DSDIO_BASE=SDIO0_BASE   -DGSPI_SDIO_IRQ=GSPI_SDIO0_IRQ
    IRUN_FLAGS +select_sdio0
    PREFIX sdio-spi-0-mirror
    )
   add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES sdio-spi/sdio_spi.c
    CFLAGS -DSDIO_BASE=SDIO1_BASE  -DGSPI_SDIO_IRQ=GSPI_SDIO1_IRQ
    IRUN_FLAGS +select_sdio1
    PREFIX sdio-spi-1-mirror
    )
  add_rumboot_target(
   CONFIGURATION IRAM
   FILES gpio/gpio.c
   TIMEOUT 300 us
   IRUN_FLAGS +gpio_test
  )
 
  add_rumboot_target(
   CONFIGURATION IRAM
   FILES gpio/gpio_irq.c
   TIMEOUT 300 us
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
    TIMEOUT 300 us
    CFLAGS -DMDIO_NUM=1
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
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_test.c
    NAME mdio_test_1
    TIMEOUT 600 us
    CFLAGS -DMDIO_NUM=1
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_test.c
    NAME mdio_test_2
    TIMEOUT 600 us
    CFLAGS -DMDIO_NUM=2
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_test.c
    NAME mdio_test_3
    TIMEOUT 600 us
    CFLAGS -DMDIO_NUM=3
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_0
    TIMEOUT 100 us
    CFLAGS -DMDIO_NUM=0
    IRUN_FLAGS +eth_phy_interrupt
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_1
    TIMEOUT 100 us
    CFLAGS -DMDIO_NUM=1
    IRUN_FLAGS +eth_phy_interrupt
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_2
    TIMEOUT 100 us
    CFLAGS -DMDIO_NUM=2
    IRUN_FLAGS +eth_phy_interrupt
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_3
    TIMEOUT 100 us
    CFLAGS -DMDIO_NUM=3
    IRUN_FLAGS +eth_phy_interrupt
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_0
    TIMEOUT 800 us
    CFLAGS -DMDIO_NUM=0
    IRUN_FLAGS +mirror_eth_phy_interrupt
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_1
    TIMEOUT 800 us
    CFLAGS -DMDIO_NUM=1
    IRUN_FLAGS +mirror_eth_phy_interrupt
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_2
    TIMEOUT 800 us
    CFLAGS -DMDIO_NUM=2
    IRUN_FLAGS +mirror_eth_phy_interrupt
  )

  add_rumboot_target(
    CONFIGURATION IRAM_MIRROR
    FILES mdio/mdio_phy_intrp_test.c
    NAME mdio_phy_intrp_test_3
    TIMEOUT 800 us
    CFLAGS -DMDIO_NUM=3
    IRUN_FLAGS +mirror_eth_phy_interrupt
  )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_dma_eRP_test.c
      NAME pcie_dma_eRP_test
      CFLAGS -Dincrease_test_duration=1
      IRUN_FLAGS +pcie_line_interference
    )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_irq_eRP_test.c
      NAME pcie_irq_eRP_test
    )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_int_msix_for_mirror.c
      NAME pcie_int_msix_for_mirror
    )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_at_slv_eRP_test_0.c
      NAME pcie_at_slv_eRP_test_0
      CFLAGS -Dshort_test_for_sim=1
    )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_at_typical_eRP_test_0.c
      NAME pcie_at_typical_eRP_test_0
      CFLAGS -Dshort_test_for_sim=1
    )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_at_mst_eRP_test_0.c
      NAME pcie_at_mst_eRP_test_0
      CFLAGS -Dshort_test_for_sim=1
    )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_legacy_eRP_test.c
      NAME pcie_legacy_eRP_test
      IRUN_FLAGS +pcie_legacy_int_elab
    )

if (CMAKE_BUILD_TYPE MATCHES "RTL")
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_mbist_test.c
      NAME pcie_mbist_test
      IRUN_FLAGS +pcie_mbist_model
    )
else()
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_mbist_test.c
      NAME pcie_mbist_test
    )
endif()

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/ext_int_test.c
      NAME ext_int_test
      IRUN_FLAGS +ext_int_elab
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
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES simple-iram/i2c_0_read_test.c
      NAME i2c_0_read_test_interference
      IRUN_FLAGS +i2c_interference_gen
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES simple-iram/i2c_1_read_test.c
      NAME i2c_1_read_test_interference
      IRUN_FLAGS +i2c_interference_gen
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/ext_irq_gen_reg_test.c
      NAME ext_irq_gen_reg_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_ctrl_reg_test.c
      NAME pcie_ctrl_reg_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_phy_bist_prbs31.S
      NAME pcie_phy_bist_prbs31
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_soft_reset_eRP_test.c
      NAME pcie_soft_reset_eRP_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_core_reg_test.c
      NAME pcie_core_reg_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_msix_eRP_test.c
      NAME pcie_msix_eRP_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_phy_reg_test.c
      NAME pcie_phy_reg_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/pcie_turn_on_eRP_test.c
      NAME pcie_turn_on_eRP_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/addr_trans_mst_reg_test.c
      NAME addr_trans_mst_reg_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES pcie/addr_trans_slv_reg_test.c
      NAME addr_trans_slv_reg_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES ddr/ddr0_addr_test.c
      NAME ddr0_addr_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES ddr/ddr1_addr_test.c
      NAME ddr1_addr_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES ddr/ddr0_reg_test.c
      NAME ddr0_reg_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES ddr/ddr1_reg_test.c
      NAME ddr1_reg_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES ddr/ddr0_low_power_test.c
      NAME ddr0_low_power_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES ddr/ddr1_low_power_test.c
      NAME ddr1_low_power_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES ddr/ddr0_phy_bist_test.S
      NAME ddr0_phy_bist_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES ddr/ddr1_phy_bist_test.S
      NAME ddr1_phy_bist_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES ddr/ddr0_ddr1_byte_test.c
      NAME ddr0_ddr1_byte_test
    )
    
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES ddr/ddr0_ddr1_turn_on_test.c
      NAME ddr0_ddr1_turn_on_test
    )
    
  #RumBoot Integration tests
#  add_rumboot_target(
#      CONFIGURATION INTEGRATION
#      NAME selftest_and_boot_sd
#      IRUN_FLAGS
#        +SD_INSERTED
#        +SD_IMAGE=${rumboot_prefix}spl-ok.bin
#    )

#  add_rumboot_target(
#      CONFIGURATION INTEGRATION
#      NAME selftest_and_boot_spi0
#      IRUN_FLAGS
#        +SPI0_0_IMAGE=${rumboot_prefix}spl-ok.bin
#    )

#  add_rumboot_target(
#      CONFIGURATION INTEGRATION
#      NAME selftest_and_boot_spi1
#      IRUN_FLAGS
#        +SPI0_1_IMAGE=${rumboot_prefix}spl-ok.bin
#    )

endmacro()

if (CMAKE_VERILOG_RULES_LOADED)
    return()
endif()

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-gic.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-proxy-gic-cdnpcie.c
    ${CMAKE_SOURCE_DIR}/src/lib/eventsystem-memory.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/ddr_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/pcie_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/dma_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/dit_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/bisr.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/mdio.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/nic400_lib.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-mcpu=cortex-a5 -mfpu=vfpv3-d16 -mfloat-abi=hard -marm -ffreestanding -Os -Werror -Wno-error=cpp")
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

    add_dependencies(${product}.all ${product}.hex)

  endif()

  install(DIRECTORY ${CMAKE_BINARY_DIR}/${product}.rcf DESTINATION rumboot/rcf)
endmacro()


if (NOT CROSS_COMPILE)
  SET(CROSS_COMPILE arm-rcm-eabihf)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
  if (EXISTS /opt/r42/toolchains/arm-rcm-eabihf/bin/arm-rcm-eabihf-gcc)
    set(CROSS_COMPILE "/opt/r42/toolchains/arm-rcm-eabihf/bin/${CROSS_COMPILE}")
  endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
