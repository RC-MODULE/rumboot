SET(RUMBOOT_ARCH ppc)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/uart_pl011.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/irq-mpic128.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/p64.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/p64.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/plb6mcif2.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/dma2plb6.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/emi.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/gpio_pl061.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/l2c.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/hscb.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/greth.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/ppc_mmu_impl.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/utlb_entries.S
)

#Flags for Power PC
macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    set(RUMBOOT_COMMON_FLAGS "-mcpu=476fp -gdwarf-2 -fno-plt -fno-pic -m32 -ffreestanding -Os -mbig-endian -std=gnu99 -DRUMBOOT_PLATFORM_NUM_HEAPS=8 -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
    set(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -mstrict-align -Wall -Werror -fdata-sections -ffunction-sections")
    set(CMAKE_ASM_FLAGS "${RUMBOOT_COMMON_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -static -Wl,--gc-sections -Wl,--oformat=elf32-powerpc")
    set(CMAKE_DUMP_FLAGS -EB -M476,32)
endmacro()

rumboot_add_configuration(
    ROM
    DEFAULT
    LDS oi10/rom.lds
    CFLAGS -DRUMBOOT_ONLY_STACK ${DEBUG_DEFINES}
    LDFLAGS "-e rumboot_entry_point"
    PREFIX rom
    FEATURES ROMGEN
    FILES ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S
    TIMEOUT_CTEST 0
)

if (RUMBOOT_BUILD_TYPE STREQUAL "Production")
  set(BOOTROM_IFLAGS +GTUBE_ONLY_PRODUCTION_OPCODES)
endif()

#Temporary hack, before we figure out what to do next.
rumboot_add_configuration(
    BROM
    CONFIGURATION ROM
    LDS oi10/bootrom.lds
    IRUN_FLAGS +BOOTMGR_KEEP_DRIVING=1 ${BOOTROM_IFLAGS}
)

rumboot_add_configuration(
    BAREROM
    LDS oi10/rom.lds
    CFLAGS -DRUMBOOT_ONLY_STACK
    SNAPSHOT default
    PREFIX bare-rom
    FEATURES ROMGEN
    TIMEOUT_CTEST 0
    TIMEOUT 10 ms
)

rumboot_add_configuration(
  LPROBE_CPU
  PREFIX lprobe-cpu
  BOOTROM bootrom-lprobe-stub
  FEATURES LPROBE
  IRUN_FLAGS +LPROBE_MODE=CPU -input ${CMAKE_SOURCE_DIR}/../scripts/lprobe-helper.tcl
)


rumboot_add_configuration(
    SUPPLEMENTARY
    LDS oi10/rom.lds
    SNAPSHOT default
    PREFIX supplementary
    FEATURES STUB
)

rumboot_add_configuration (
    IRAM
    LDS oi10/iram.lds
    PREFIX iram
    LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e rumboot_main"
    FILES ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
    CFLAGS ${DEBUG_DEFINES}
    BOOTROM bootrom-stub
    FEATURES LUA COVERAGE PACKIMAGE
    LOAD IM0BIN SELF
    TIMEOUT_CTEST 86400
)

rumboot_add_configuration (
    IRAM_SPL
    CONFIGURATION IRAM
    LDS oi10/iram-spl.lds
    PREFIX spl
    LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e main"
    CFLAGS ${DEBUG_DEFINES} -DRUMBOOT_NOINIT
    FEATURES COVERAGE PACKIMAGE
)

macro(rumboot_platform_generate_stuff_for_taget product)
    list (FIND TARGET_FEATURES "ROMGEN" _index)
    if (${_index} GREATER -1)
        add_custom_command(
            OUTPUT ${product}.hex/image_mem64_0.hex
            COMMAND mkdir -p ${product}.hex
            COMMAND ${CMAKE_BINARY_DIR}/utils/romgen -l oi10_gen -i ${product}.bin -o ${product}.hex
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



include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

set(ROM_9600_OPTS +BOOT_SLOWUART=1 +BOOT_FASTUART=0 +UART0_SPEED=9600)
set(ROM_19200_OPTS +BOOT_SLOWUART=1 +BOOT_FASTUART=1 +UART0_SPEED=19200)
set(ROM_115200_OPTS +BOOT_SLOWUART=0 +BOOT_FASTUART=0 +UART0_SPEED=115200)
set(ROM_6500K_OPTS +BOOT_SLOWUART=0 +BOOT_FASTUART=1 +UART0_SPEED=6250000)

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
#   Example for adding a single bare-rom test
#   add_rumboot_target(
#       CONFIGURATION BAREROM
#       FILES bare-rom/gtube-spr-check.S
#   )

    rumboot_bootrom_add_components(IRAM_SPL BROM
      -a 512 -z 512
    )

    rumboot_bootrom_unit_test(
        ID 0
        CONFIGURATION BROM
        TAG sdio
        MEMTAG SD0_BOOT_IMAGE
        TAGOFFSET 0
        IRUN_FLAGS +select_sdio0 +BOOT_SD_CD=0 ${ROM_6500K_OPTS}
        ENDIAN big
    )

    rumboot_bootrom_unit_test(
        ID 1
        CONFIGURATION BROM
        TAG nor-no-ecc
        MEMTAG NOR_IMAGE
        TAGOFFSET 0
        IRUN_FLAGS +BOOT_EMI_ECC=0 ${ROM_6500K_OPTS}
        ENDIAN big
    )

    rumboot_bootrom_unit_test(
        ID 1
        CONFIGURATION BROM
        TAG nor-with-ecc
        MEMTAG NOR_IMAGE
        TAGOFFSET 0
        IRUN_FLAGS +BOOT_EMI_ECC=1 ${ROM_6500K_OPTS}
        ENDIAN big
    )

    rumboot_bootrom_unit_test(
        ID 2
        CONFIGURATION BROM
        TAG spi0_cs0
        MEMTAG SPI0_CONF
        TAGOFFSET 0
        FULL YES
        IRUN_FLAGS ${ROM_6500K_OPTS}
        ENDIAN big
    )

    rumboot_bootrom_integration_test(BROM
        NAME "host-mockup-fallthough"
        IRUN_FLAGS ${ROM_6500K_OPTS}
        LOAD
          SD0_BOOT_IMAGE spl-fail
          SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
          NOR_IMAGE spl-fail-bad-magic
          HOSTMOCK  spl-ok
    )

    rumboot_bootrom_integration_test(BROM
        NAME "host-mockup"
        IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS}
        LOAD
          HOSTMOCK  spl-ok
    )

    rumboot_bootrom_integration_test(BROM
        NAME "host-fallthough-edcl"
        IRUN_FLAGS ${ROM_6500K_OPTS} +edcl_loader0
        LOAD
          SD0_BOOT_IMAGE spl-fail
          SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
          NOR_IMAGE spl-fail-bad-magic
          edcl_image  spl-ok
    )

    rumboot_bootrom_integration_test(BROM
        NAME "host-fallthough-rmap"
        IRUN_FLAGS ${ROM_6500K_OPTS} +rmap_master=0
        LOAD
          SD0_BOOT_IMAGE spl-fail
          SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
          NOR_IMAGE spl-fail-bad-magic
          rmap_file spl-ok
    )

    rumboot_bootrom_integration_test(BROM
        NAME "host-edcl-0"
        IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +edcl_loader0
        LOAD
          edcl_image  spl-ok
    )

    rumboot_bootrom_integration_test(BROM
        NAME "host-edcl-1"
        IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +edcl_loader1
        LOAD
          edcl_image spl-ok
    )

    rumboot_bootrom_integration_test(BROM
        NAME "host-rmap-0"
        IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +rmap_master=0
        LOAD
          rmap_file spl-ok
    )

    rumboot_bootrom_integration_test(BROM
        NAME "host-rmap-1"
        IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +rmap_master=1
        LOAD
          rmap_file spl-ok
    )

    rumboot_bootrom_integration_test(BROM
        NAME "host-rmap-2"
        IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +rmap_master=2
        LOAD
          rmap_file spl-ok
    )

    rumboot_bootrom_integration_test(BROM
        NAME "host-rmap-3"
        IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +rmap_master=3
        LOAD
          rmap_file spl-ok
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
          NOR_IMAGE spl-fail-bad-magic
    )

endif()

    rumboot_bootrom_integration_test(BROM
        NAME "sdio-ok"
        IRUN_FLAGS +BOOT_SD_CD=0 +select_sdio0 ${ROM_6500K_OPTS}
        LOAD
          SD0_BOOT_IMAGE spl-ok
          NOR_IMAGE spl-fail
          SPI0_CONF spl-fail,spl-fail
          HOSTMOCK  spl-fail
    )

    rumboot_bootrom_integration_test(BROM
        NAME "nor-with-ecc-ok"
        IRUN_FLAGS ${ROM_6500K_OPTS}
        LOAD
          SD0_BOOT_IMAGE spl-fail-bad-magic
          SPI0_CONF spl-fail,spl-fail
          NOR_IMAGE spl-ok
          HOSTMOCK  spl-fail
        IRUN_FLAGS +BOOT_EMI_ECC=1
    )

    rumboot_bootrom_integration_test(BROM
        NAME "nor-no-ecc-ok"
        LOAD
          SD0_BOOT_IMAGE spl-fail-bad-magic
          SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
          NOR_IMAGE spl-ok
          HOSTMOCK  spl-fail
        IRUN_FLAGS +BOOT_EMI_ECC=0 ${ROM_6500K_OPTS}
    )

    rumboot_bootrom_integration_test(BROM
        NAME "spi-cs0-ok"
        IRUN_FLAGS ${ROM_6500K_OPTS}
        LOAD
          SPI0_CONF spl-ok,spl-fail
          HOSTMOCK  spl-fail
    )

    rumboot_bootrom_integration_test(BROM
        NAME "serial-9600"
        IRUN_FLAGS ${ROM_9600_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
        TIMEOUT 10 ms
    )
    rumboot_bootrom_integration_test(BROM
        NAME "serial-19200"
        IRUN_FLAGS ${ROM_19200_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
        TIMEOUT 10 ms
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


    #Add lprobe sample scripts
    add_rumboot_target_dir(common/lua/
      CONFIGURATION LPROBE_CPU
      PREFIX lprobe-cpu
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
        FILES common/bootrom-stubs/bootrom-stub.c
        PREFIX "bootrom"
        NAME "stub"
        FEATURES STUB
    )
    
    add_rumboot_target(
        CONFIGURATION ROM
        CFLAGS -DUSE_SWINT=132
        FILES common/irq/irq-atomics.c
        PREFIX "irq-rom"
    )

    add_rumboot_target(
        CONFIGURATION ROM
        CFLAGS -DUSE_SWINT=132
        FILES common/irq/irq-defhandler.c
        PREFIX "irq-rom"
    )


    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DUSE_SWINT=132
        FILES common/irq/irq-atomics.c
        PREFIX "irq-iram"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DUSE_SWINT=132
        FILES common/irq/irq-defhandler.c
        PREFIX "irq-iram"
    )
    
    #---GRETH tests start---
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DCHECK_RX_COL=1
      PREFIX greth0-rx_col
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DCHECK_RX_COL=1
      PREFIX greth1-rx_col
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DCHECK_RX_ER=1
      PREFIX greth0-rx_er
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DCHECK_RX_ER=1
      PREFIX greth1-rx_er
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DCHECK_REG_AND_MDIO=1
      PREFIX greth0-regs
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DCHECK_REG_AND_MDIO=1
      PREFIX greth1-regs
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DEDCL_TEST_ADDR=EDCL_TEST_ADDR_IM2
      PREFIX greth0-edcl-im2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DEDCL_TEST_ADDR=EDCL_TEST_ADDR_IM1
      PREFIX greth0-edcl-im1
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE  -DEDCL_TEST_ADDR=EDCL_TEST_ADDR_EM2
      PREFIX greth0-edcl-em2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DEDCL_TEST_ADDR=EDCL_TEST_ADDR_IM2
      PREFIX greth1-edcl-im2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DEDCL_TEST_ADDR=EDCL_TEST_ADDR_IM1
      PREFIX greth1-edcl-im1
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DEDCL_TEST_ADDR=EDCL_TEST_ADDR_EM2
      PREFIX greth1-edcl-em2
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_BANK=2 -DDST_BANK=2
      PREFIX greth0-im2-im2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_BANK=2 -DDST_BANK=1
      PREFIX greth0-im2-im1
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_BANK=2 -DDST_BANK=3
      PREFIX greth0-im2-em2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_BANK=1 -DDST_BANK=2
      PREFIX greth0-im1-im2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_BANK=1 -DDST_BANK=1
      PREFIX greth0-im1-im1
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_BANK=1 -DDST_BANK=3
      PREFIX greth0-im1-em2
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_BANK=3 -DDST_BANK=2
      PREFIX greth0-em2-im2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_BANK=3 -DDST_BANK=1
      PREFIX greth0-em2-im1
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_BANK=3 -DDST_BANK=3
      PREFIX greth0-em2-em2
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_BANK=2 -DDST_BANK=2
      PREFIX greth1-im2-im2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_BANK=2 -DDST_BANK=1
      PREFIX greth1-im2-im1
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_BANK=2 -DDST_BANK=3
      PREFIX greth1-im2-em2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_BANK=1 -DDST_BANK=2
      PREFIX greth1-im1-im2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_BANK=1 -DDST_BANK=1
      PREFIX greth1-im1-im1
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_BANK=1 -DDST_BANK=3
      PREFIX greth1-im1-em2
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_BANK=3 -DDST_BANK=2
      PREFIX greth1-em2-im2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_BANK=3 -DDST_BANK=1
      PREFIX greth1-em2-im1
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_BANK=3 -DDST_BANK=3
      PREFIX greth1-em2-em2
    )
    #---GRETH tests finish---

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_spi.c
      CFLAGS -DGSPI_BASE=GSPI_0_BASE -DGSPI_INT=GSPI0_INT -DGSPI_CHECK_REGS -DGPIO1_X=0x1
      IRUN_FLAGS +spi_irq_test
      PREFIX gspi0
    )
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_spi.c
      CFLAGS -DGSPI_BASE=GSPI_1_BASE -DGSPI_INT=GSPI1_INT -DGSPI_CHECK_REGS -DGPIO1_X=0x2
      IRUN_FLAGS +spi_irq_test
      PREFIX gspi1
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_em2_208.c
      IRUN_FLAGS +test_oi10_em2_208
      PREFIX HiZ
    )

    add_rumboot_target_dir(bare-rom/
        CONFIGURATION BAREROM
    )

    add_rumboot_target_dir(rom/
        CONFIGURATION ROM
    )

    add_rumboot_target_dir(tests/
        CONFIGURATION ROM
    )

    add_rumboot_target_dir(jenkins/
        CONFIGURATION IRAM
        PREFIX jenkins
        TESTGROUP short
    )

    add_rumboot_target_dir(simple-iram/
        CONFIGURATION IRAM
        PREFIX simple-iram
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_038_im1.lds
        FILES test_oi10_cpu_038_helper.c
        CFLAGS -DTEST_OI10_CPU_038_ARRAY_SIZE=0x7C00
        NAME "test_oi10_cpu_038_helper_im1"
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_038_im0.lds
        FILES test_oi10_cpu_038_helper.c
        CFLAGS -DTEST_OI10_CPU_038_ARRAY_SIZE=0x7C00
        NAME "test_oi10_cpu_038_helper_im0"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_038.c
        PREFIX simple-iram
        CFLAGS -DIM_BASE=IM1_BASE
        NAME "test_oi10_cpu_038_im1"
        LOAD IM0BIN SELF
             IMBIN supplementary-test_oi10_cpu_038_helper_im1
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_038.c
        PREFIX simple-iram
        CFLAGS -DTEST_OI10_CPU_038_IM0 -DIM_BASE=IM1_BASE
        NAME "test_oi10_cpu_038_im0"
        LOAD IM0BIN SELF
             IMBIN supplementary-test_oi10_cpu_038_helper_im0
    )

    add_rumboot_target_dir(uart_data_logger/
        CONFIGURATION IRAM
        IRUN_FLAGS +use_uart_data_logger
        PREFIX uart_data_logger
    )

    add_rumboot_target_dir(iss-iram/
        CONFIGURATION IRAM
        PREFIX iss-iram
        CHECKCMD ${CMAKE_SOURCE_DIR}/scripts/ISS/dump_gen.csh ${CMAKE_BINARY_DIR} $PRODUCT ${CMAKE_SOURCE_DIR}/scripts/ISS
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DGPIO_X_BASE=GPIO_0_BASE -DGPIO_X_INT=GPIO0_INT -DCHECK_REGS -DGPIO_TIMEOUT=0x200 IRUN_FLAGS +gpio_test
        FILES test_oi10_gpio.c
        PREFIX "gpio_0"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DGPIO_X_BASE=GPIO_1_BASE -DGPIO_X_INT=GPIO1_INT -DCHECK_REGS -DGPIO_TIMEOUT=0x200 IRUN_FLAGS +gpio_test
        FILES test_oi10_gpio.c
        PREFIX "gpio_1"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE 
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE 
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT 
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT 
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT 
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT 
               -DHSCB_SHORT_TEST
               -DTEST_OI10_HSCB_FULL_TRACING
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_0"
        NAME test_oi10_hscb_im1_im1
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE 
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE 
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT 
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT 
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT 
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT 
               -DHSCB_SHORT_TEST
               -DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="IM2"
               -DRX_0_HEAP_NAME="IM2"
               -DRX_1_HEAP_NAME="IM1"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_0"
        NAME test_oi10_hscb_im1_im2
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE 
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE 
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT 
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT 
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT 
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT 
               -DTX_0_HEAP_NAME="SDRAM"
               -DTX_1_HEAP_NAME="SDRAM"
               -DRX_0_HEAP_NAME="SDRAM"
               -DRX_1_HEAP_NAME="SDRAM"
               -DDATA_SIZE_0=32
               -DDATA_SIZE_1=257
               -DHSCB_SHORT_TEST
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_0"
        NAME test_oi10_hscb_sdram_sdram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE 
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE 
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT 
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT 
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT 
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT 
               -DTX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="SDRAM"
               -DRX_0_HEAP_NAME="SSRAM"
               -DRX_1_HEAP_NAME="PLRAM"
               -DDATA_SIZE_0=320
               -DDATA_SIZE_1=257
               -DHSCB_SHORT_TEST
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_0"
        NAME test_oi10_hscb_im1_ssram_plram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE 
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE 
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT 
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT 
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT 
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT 
               -DTEST_OI10_HSCB_FULL_TRACING
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_0"
        NAME test_oi10_hscb_long_sdram_ssram_im1_sdram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DMKIO0_IRQ=MKIO0_INT -DMKIO1_IRQ=MKIO1_INT
        FILES common/mkio/mkio_write_read_test.c
        NAME mkio_write_read_test
      )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DMKIO0_IRQ=MKIO0_INT -DMKIO1_IRQ=MKIO1_INT
        FILES common/mkio/mkio0_irq_test.c
        NAME mkio0_irq_test
      )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DMKIO0_IRQ=MKIO0_INT -DMKIO1_IRQ=MKIO1_INT
        FILES common/mkio/mkio1_irq_test.c
        NAME mkio1_irq_test
      )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DMKIO0_IRQ=MKIO0_INT -DMKIO1_IRQ=MKIO1_INT
        FILES common/mkio/mkio_polarity_test.c
        NAME mkio_polarity_test
      )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO0_BASE -DCHECK_MKIO_REGS
      PREFIX mkio0-regs
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO1_BASE -DCHECK_MKIO_REGS
      PREFIX mkio1-regs
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO0_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio0-im1-func-a
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO1_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio1-im1-func-a
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO0_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_B -DTEST_BANK="IM1"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio0-im1-func-b
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO1_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_B -DTEST_BANK="IM1"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio1-im1-func-b
    )


    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO0_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM2"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio0-im2-func-a
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO1_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM2"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio1-im2-func-a
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO0_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_B -DTEST_BANK="IM2"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio0-im2-func-b
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO1_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_B -DTEST_BANK="IM2"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio1-im2-func-b
    )


    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO0_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="SSRAM"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio0-em2-func-a
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO1_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="SSRAM"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio1-em2-func-a
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO0_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_B -DTEST_BANK="SSRAM"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio0-em2-func-b
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO1_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_B -DTEST_BANK="SSRAM"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio1-em2-func-b
    )


    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_uart_000.c
      CFLAGS -DUARTRX_BASE=UART0_BASE -DUARTTX_BASE=UART1_BASE -DUARTRX_INT=UART0_INT -DUARTTX_INT=UART1_INT -DCHECK_REGISTERS
      PREFIX uart0
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_uart_000.c
      CFLAGS -DUARTRX_BASE=UART1_BASE -DUARTTX_BASE=UART0_BASE -DUARTRX_INT=UART1_INT -DUARTTX_INT=UART0_INT -DCHECK_REGISTERS
      PREFIX uart1
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
