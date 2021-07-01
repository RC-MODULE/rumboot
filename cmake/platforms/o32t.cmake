SET(RUMBOOT_ARCH ppc)
set(RUMBOOT_SUPPORTS_SPL_ENDIAN_SWAP Yes)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)

if (NOT RUMBOOT_SOC_BUILD_TYPE STREQUAL "RTL")
  set(IRUN_BOOTM_EXTRA_ARGS +BOOT_NOR=0)
else()
  set(IRUN_BOOTM_EXTRA_ARGS )
endif()

set(RUMBOOT_HAS_V3_BOOTROM yes)

file(GLOB PLATFORM_SOURCES
  ${CMAKE_SOURCE_DIR}/src/arch/ppc/exception.c
  ${CMAKE_SOURCE_DIR}/src/arch/ppc/p64.S
  ${CMAKE_SOURCE_DIR}/src/arch/ppc/vectors.S
  ${CMAKE_SOURCE_DIR}/src/arch/ppc/ppc_mmu_impl.S
  ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
  ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
  ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/ndma_simple.c
  ${CMAKE_SOURCE_DIR}/src/lib/drivers/com_simple.c
  ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-mpic128.c
  ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/scrb_lib.c
)
#Flags for Power PC
macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
  set(RUMBOOT_COMMON_FLAGS "-mcpu=476fp -fno-plt -fno-pic -m32 -ffreestanding -std=gnu99 -DRUMBOOT_PLATFORM_NUM_HEAPS=10 -DRUMBOOT_SUPPORTS_SPL_ENDIAN_SWAP")
  set(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -mstrict-align -Wall -Wno-error=cpp -fdata-sections -ffunction-sections")
  set(CMAKE_ASM_FLAGS "${RUMBOOT_COMMON_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "-g -nostartfiles -static -Wl,--gc-sections")
  set(CMAKE_DUMP_FLAGS -M476,32)
  if (PRODUCTION_TESTING)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPRODUCTION_TESTING")
  endif()
endmacro()


if (RUMBOOT_BUILD_TYPE STREQUAL "Production")
  set(BOOTROM_IFLAGS +GTUBE_ONLY_PRODUCTION_OPCODES)
endif()


rumboot_add_configuration(
  ROM
  DEFAULT
  LDFLAGS -Wl,-erumboot_entry_point
  PREFIX brom
  FEATURES ROMGEN
  FILES ${CMAKE_SOURCE_DIR}/src/arch/ppc/startup.S
  TIMEOUT_CTEST 0
  LOAD BOOTROM_NOR SELF
  IRUN_FLAGS ${IRUN_BOOTM_EXTRA_ARGS}
  CFLAGS -DRUMBOOT_ONLY_STACK ${CONFIGURATION_ROM_CFLAGS} -DRUMBOOT_MAIN_NORETURN
  LDS o32t/bootrom.lds
  IRUN_FLAGS +BOOTMGR_KEEP_DRIVING=1 ${BOOTROM_IFLAGS}
)

rumboot_add_configuration (
  IRAM_IM1
  LDS o32t/iram.lds
  PREFIX iram
  LDFLAGS -Wl,-erumboot_main
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  BOOTROM bootrom-stub
  FEATURES COVERAGE PACKIMAGE BANNER
  LOAD
    IM0BIN SELF
  BOOTROM_NOR bootrom-stub
  TIMEOUT_CTEST 86400
  IRUN_FLAGS ${IRUN_BOOTM_EXTRA_ARGS}
)

rumboot_add_configuration (
  IRAM_IM0
  CONFIGURATION IRAM_IM1
  LDS o32t/iram_im0.lds
  PACKIMAGE_FLAGS -CiR 0x80000000
)


rumboot_add_configuration (
  IRAM_IM1_WITH_EMI
  CONFIGURATION IRAM_IM1
  LOAD 
    IM0BIN stub-emi_initializer,SELF
 )

 rumboot_add_configuration (
  IRAM_IM0_WITH_EMI
  CONFIGURATION IRAM_IM0
  LOAD 
    IM0BIN stub-emi_initializer,SELF
 )

rumboot_add_configuration (
  IRAM_SPL
  CONFIGURATION IRAM
  LDS o32t/iram-spl.lds
  PREFIX spl
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  LDFLAGS -Wl,-emain
  CFLAGS -DRUMBOOT_NOINIT
  FEATURES COVERAGE PACKIMAGE
)

rumboot_add_configuration(IRAM_SPL_LE
  CONFIGURATION IRAM_IM1
  PREFIX spl
  LDFLAGS -Wl,-belf32-powerpcle -mlittle-endian -Wl,-emain
  CFLAGS -mlittle-endian
  DUMPFLAGS -EL
)

rumboot_add_configuration (
  IRAM_SPL_IM0
  CONFIGURATION IRAM_IM0
  LDS o32t/iram-spl-im0.lds
  PREFIX spl
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  LDFLAGS -Wl,-emain
  CFLAGS -DRUMBOOT_NOINIT
  FEATURES COVERAGE PACKIMAGE
)

macro(rumboot_platform_generate_stuff_for_taget product)
  list (FIND TARGET_FEATURES "ROMGEN" _index)
  if (${_index} GREATER -1)
    add_custom_command(
      OUTPUT ${product}.hex/image_mem64_0.hex
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
      install(DIRECTORY ${PROJECT_BINARY_DIR}/${product}.hex DESTINATION bootrom)
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
  add_rumboot_target(
    CONFIGURATION ROM
    FILES common/bootrom-stubs/bootrom-stub.c
    PREFIX "bootrom"
    NAME "stub"
    FEATURES STUB
  )

  ############## BOOTROM STUFF #########################
  rumboot_bootrom_add_components(ROM IRAM_SPL IRAM_SPL_LE IRAM_SPL_IM0)

  rumboot_bootrom_add_common_units(
    CONFIGURATION ROM
    IRUN_FLAGS ${ROM_6500K_OPTS}
  )

  rumboot_bootrom_unit_test(
    ID 0
    CONFIGURATION ROM
    TAG sdio
    MEMTAG SD0_BOOT_IMAGE
    TAGOFFSET 0
    IRUN_FLAGS +select_sdio0 +BOOT_SD_CD=0 ${ROM_6500K_OPTS}
    ENDIAN big
  )
  rumboot_bootrom_unit_test(
    ID 1
    CONFIGURATION ROM
    TAG nor-no-ecc
    MEMTAG NOR_IMAGE
    TAGOFFSET 0
    IRUN_FLAGS +BOOT_EMI_ECC=0 ${ROM_6500K_OPTS}
    ENDIAN big
  )
  rumboot_bootrom_unit_test(
    ID 1
    CONFIGURATION ROM
    TAG nor-with-ecc
    MEMTAG NOR_IMAGE
    TAGOFFSET 0
    IRUN_FLAGS +BOOT_EMI_ECC=1 ${ROM_6500K_OPTS}
    ENDIAN big
  )
  rumboot_bootrom_unit_test(
    ID 2
    CONFIGURATION ROM
    TAG spi0_cs0
    MEMTAG SPI0_CONF
    TAGOFFSET 0
    FULL YES
    IRUN_FLAGS ${ROM_6500K_OPTS}
    ENDIAN big
  )



  rumboot_bootrom_integration_test(BROM
    NAME "host-mockup"
    IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS}
    LOAD
    HOSTMOCK  spl-ok
  )

  rumboot_bootrom_integration_test(BROM
    NAME "host-xmodem"
    IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS}
    LOAD
      XMODEM0  spl-ok
  )

  rumboot_bootrom_integration_test(BROM
    NAME "host-mockup-little-endian"
    IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS}
    LOAD
    HOSTMOCK  spl-reverse-endian-ok
  )

  rumboot_bootrom_integration_test(BROM
    NAME "host-mockup-relocation"
    IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS}
    LOAD
      HOSTMOCK  spl-relocation-ok
  )

  rumboot_bootrom_integration_test(BROM
    NAME "host-mockup-relocation-and-compression"
    IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS}
    LOAD
      HOSTMOCK  spl-reloc-compressed-ok
  )

  rumboot_bootrom_integration_test(BROM
    NAME "host-mockup-compressed"
    IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS}
    LOAD
      HOSTMOCK  spl-compressed-ok
  )

  rumboot_bootrom_integration_test(BROM
    NAME "host-mockup-v2"
    IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS}
    LOAD
    HOSTMOCK  spl-v2-ok
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
    NAME "nor-with-ecc-ok-bypass"
    IRUN_FLAGS ${ROM_6500K_OPTS} +BOOT_EMI_ECC=1 +BOOT_PLL_BYP=1
    LOAD
    SD0_BOOT_IMAGE spl-fail-bad-magic
    SPI0_CONF spl-fail,spl-fail
    NOR_IMAGE spl-ok
    HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(BROM
    NAME "nor-with-ecc-ok"
    IRUN_FLAGS ${ROM_6500K_OPTS} +BOOT_EMI_ECC=1
    LOAD
    SD0_BOOT_IMAGE spl-fail-bad-magic
    SPI0_CONF spl-fail,spl-fail
    NOR_IMAGE spl-ok
    HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(BROM
    NAME "nor-with-ecc-with-cpu-ecc-ok"
    IRUN_FLAGS ${ROM_6500K_OPTS} +BOOT_EMI_ECC=1 +BOOT_CPU_ECC=1
    LOAD
    SD0_BOOT_IMAGE spl-fail-bad-magic
    SPI0_CONF spl-fail,spl-fail
    NOR_IMAGE spl-ok
    HOSTMOCK  spl-fail
  )


  rumboot_bootrom_integration_test(BROM
    NAME "nor-no-ecc-ok"
    IRUN_FLAGS +BOOT_EMI_ECC=0 ${ROM_6500K_OPTS}
    LOAD
    SD0_BOOT_IMAGE spl-fail-bad-magic
    SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
    NOR_IMAGE spl-ok
    HOSTMOCK  spl-fail
  )

  rumboot_bootrom_integration_test(BROM
    NAME "nor-with-ecc-ok-direct"
    IRUN_FLAGS +BOOT_EMI_ECC=1 ${ROM_6500K_OPTS} +BOOT_NOR=0
    LOAD
    SD0_BOOT_IMAGE spl-fail-bad-magic
    SPI0_CONF spl-fail,spl-fail
    NOR_IMAGE spl-ok
    HOSTMOCK  spl-fail
    BOOTROM_NOR bootrom-loader
  )

  rumboot_bootrom_integration_test(BROM
    NAME "nor-no-ecc-ok-direct"
    IRUN_FLAGS +BOOT_EMI_ECC=0 ${ROM_6500K_OPTS} +BOOT_NOR=0
    LOAD
    SD0_BOOT_IMAGE spl-fail-bad-magic
    SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
    NOR_IMAGE spl-ok
    HOSTMOCK  spl-fail
    BOOTROM_NOR bootrom-loader
  )

  rumboot_bootrom_integration_test(BROM
    NAME "spi-cs0-ok"
    IRUN_FLAGS ${ROM_6500K_OPTS}
    LOAD
    SPI0_CONF spl-ok,spl-fail
    HOSTMOCK  spl-fail
  )

  add_rumboot_target(
    CONFIGURATION IRAM_IM1
    FILES emi_initializer.c
    NAME emi_initializer_notlb_unload
    PREFIX stub
    CFLAGS -DKEEP_TLB
    FEATURES STUB
  )

  rumboot_bootrom_integration_test(BROM
    NAME "nmc-extboot-cp01"
    IRUN_FLAGS ${ROM_6500K_OPTS} +BOOT_HOST=1 +nmc_ext_poll_test
    LOAD
      HOSTMOCK  nmc:stub-cp1-booter,nmc:spl-cp-com1,spl-ok
  )

  rumboot_bootrom_integration_test(BROM
    NAME "nmc-extboot-cp10"
    IRUN_FLAGS ${ROM_6500K_OPTS} +BOOT_HOST=1 +nmc_ext_poll_test
    LOAD
      HOSTMOCK  nmc:stub-cp0-booter,nmc:spl-cp-com0,spl-ok
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

  ######################### END OF BOOTROM STUFF #########################

  add_rumboot_target(
    CONFIGURATION IRAM_IM1
    FILES emi_initializer.c
    PREFIX stub
    FEATURES STUB
  )

  add_rumboot_target(
    CONFIGURATION IRAM_IM1
    FILES common/rcm_cp/rcm_cp_size.c
  )

  add_rumboot_target(
    CONFIGURATION IRAM_IM1_WITH_EMI
    FILES emi_sanity.c
  )
  


  add_rumboot_target_dir(simple-iram/
    CONFIGURATION IRAM_IM1
    PREFIX iram-im1
  )

  add_rumboot_target_dir(simple-iram/
    CONFIGURATION IRAM_IM0
    PREFIX iram-im0
  )

  #  add_rumboot_target(
  #    CONFIGURATION IRAM_IM1
  #    FILES oi10/targets/test_oi10_lscb.c
  #    CFLAGS -DMKIO_BASE=MKIO2_BASE -DCHECK_MKIO_REGS
  #    PREFIX mkio0-regs
#  )


#Don't use, not ready yet.
#add_rumboot_target_dir(simple-iram/
#  CONFIGURATION IRAM_IM0
#  PREFIX iram-im0
#)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-dma/test_ndma_mem.c
  CFLAGS -DRD_TEST_BANK="IM1" -DWR_TEST_BANK="IM1"
  PREFIX rd-im1-wr-im1
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-dma/test_ndma_mem.c
  CFLAGS -DRD_TEST_BANK="IM2" -DWR_TEST_BANK="IM2"
  PREFIX rd-im2-wr-im2
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-dma/test_ndma_le.c
  CFLAGS -DRD_TEST_BANK="IM2" -DWR_TEST_BANK="IM2"
  PREFIX rd-im2-wr-im2
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-dma/test_ndma_mem.c
  CFLAGS -DRD_TEST_BANK="IM3" -DWR_TEST_BANK="IM3"
  PREFIX rd-im3-wr-im3
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-dma/test_ndma_mem.c
  CFLAGS -DRD_TEST_BANK="SRAM0" -DWR_TEST_BANK="SRAM0"
  PREFIX rd-sram0-wr-sram0
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-dma/test_ndma_apb.c
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-dma/test_ndma_denied.c
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-dma/test_ndma_denied_2.c
  CFLAGS -DRD_TEST_BANK="IM1"
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-dma/test_ndma_irq_es.c
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-dma/test_ndma_irq_cpl.c
  CFLAGS -DRD_TEST_BANK="IM1" -DWR_TEST_BANK="IM1"
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_simple.c
  CFLAGS -DCOM_SRC_HEAP="IM1" -DCOM_DST_HEAP="IM1"
  PREFIX com-simple-im1-im1
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_simple.c
  CFLAGS -DCOM_SRC_HEAP="IM2" -DCOM_DST_HEAP="IM2"
  PREFIX com-simple-im2-im2
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_simple.c
  CFLAGS -DCOM_SRC_HEAP="IM3" -DCOM_DST_HEAP="IM3"
  PREFIX com-simple-im3-im3
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_irq.c
  CFLAGS -DCOM_SRC_HEAP="IM1" -DCOM_DST_HEAP="IM1"
  PREFIX com-irq-im1-im1
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_irq.c
  CFLAGS -DCOM_SRC_HEAP="IM2" -DCOM_DST_HEAP="IM2"
  PREFIX com-irq-im2-im2
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_irq.c
  CFLAGS -DCOM_SRC_HEAP="IM3" -DCOM_DST_HEAP="IM3"
  PREFIX com-irq-im3-im3
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_throughput.c
  CFLAGS -DCOM_SRC_HEAP="IM1" -DCOM_DST_HEAP="IM1"
  PREFIX com-thru-im1-im1
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_throughput.c
  CFLAGS -DCOM_SRC_HEAP="IM2" -DCOM_DST_HEAP="IM2"
  PREFIX com-thru-im2-im2
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_throughput.c
  CFLAGS -DCOM_SRC_HEAP="IM3" -DCOM_DST_HEAP="IM3"
  PREFIX com-thru-im3-im3
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1_WITH_EMI
  FILES axi-com/test_com_irq.c
  CFLAGS -DCOM_SRC_HEAP="SRAM0" -DCOM_DST_HEAP="SRAM0"
  PREFIX com-irq-sram0-sram0
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_regs.c  
  PREFIX com-regs
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_irq_es.c
  CFLAGS -DCOM_SRC_HEAP="IM1" -DCOM_DST_HEAP="IM1"
  PREFIX com-irq_es-im1-im1
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_irq_es.c
  CFLAGS -DCOM_SRC_HEAP="IM2" -DCOM_DST_HEAP="IM2"
  PREFIX com-irq_es-im2-im2
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_irq_es.c
  CFLAGS -DCOM_SRC_HEAP="IM3" -DCOM_DST_HEAP="IM3"
  PREFIX com-irq_es-im3-im3
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_simple_msbf.c
  CFLAGS -DCOM_SRC_HEAP="IM1" -DCOM_DST_HEAP="IM1"
  PREFIX com-msbf-im1-im1
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_simple_msbf.c
  CFLAGS -DCOM_SRC_HEAP="IM2" -DCOM_DST_HEAP="IM2"
  PREFIX com-msbf-im2-im2
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_simple_msbf.c
  CFLAGS -DCOM_SRC_HEAP="IM3" -DCOM_DST_HEAP="IM3"
  PREFIX com-msbf-im3-im3
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_xrdy.c
  CFLAGS -DCOM_SRC_HEAP="IM1" -DCOM_DST_HEAP="IM1"
  PREFIX com-xrdy-im1-im1
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_xrdy.c
  CFLAGS -DCOM_SRC_HEAP="IM2" -DCOM_DST_HEAP="IM2"
  PREFIX com-xrdy-im2-im2
)
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES axi-com/test_com_xrdy.c
  CFLAGS -DCOM_SRC_HEAP="IM3" -DCOM_DST_HEAP="IM3"
  PREFIX com-xrdy-im3-im3
)

#  The one test, checking access to closed space
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO0_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM0" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio0-im0-func-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO1_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM0" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio1-im0-func-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO2_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM0" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio2-im0-func-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO3_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM0" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio3-im0-func-a
)


#  Four tests for checking 1553 signals changing polarity for each controller
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES test_oi10_lscb_polarity.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO0_BASE -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  PREFIX mkio0-im1-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES test_oi10_lscb_polarity.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO1_BASE -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  PREFIX mkio1-im1-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES test_oi10_lscb_polarity.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO2_BASE -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  PREFIX mkio2-im1-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES test_oi10_lscb_polarity.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO3_BASE -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  PREFIX mkio3-im1-a
)


#  Four tests for checking 1553 functionality for each controller
add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES test_oi10_lscb_1553_functionality.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO0_BASE -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  PREFIX mkio0-im1-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES test_oi10_lscb_1553_functionality.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO1_BASE -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  PREFIX mkio1-im1-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES test_oi10_lscb_1553_functionality.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO2_BASE -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  PREFIX mkio2-im1-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES test_oi10_lscb_1553_functionality.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO3_BASE -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  PREFIX mkio3-im1-a
)


#  Set of tests for MKIO2 controller
#    - only one test to check B channel - its enough

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO2_BASE -DCHECK_MKIO_REGS -D__FILENAME__=\"wtfisthisiwonder\"
  PREFIX mkio2-regs
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO2_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio2-im1-func-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DDATA_DIRECT_ADDRESS=0xC0060000 -DMKIO_BASE=MKIO2_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio2-im1-func-a-dir_addr
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO2_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_B -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio2-im1-func-b
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO2_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM2" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio2-im2-func-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO2_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM3" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio2-im3-func-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO2_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="SSRAM" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio2-em2-func-a
)


#  Set of tests for MKIO3 controller
#    - only one test to check B channel - its enough

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO3_BASE -DCHECK_MKIO_REGS -D__FILENAME__=\"wtfisthisiwonder\"
  PREFIX mkio3-regs
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO3_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio3-im1-func-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DDATA_DIRECT_ADDRESS=0xC0060000 -DMKIO_BASE=MKIO3_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio3-im1-func-a-dir_addr
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO3_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_B -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio3-im1-func-b
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO3_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM2" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio3-im2-func-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO3_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM3" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio3-im3-func-a
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES oi10/targets/test_oi10_lscb.c
  CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO3_BASE -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="SSRAM" -D__FILENAME__=\"wtfisthisiwonder\"
  IRUN_FLAGS +mkio_signal_test
  PREFIX mkio3-em2-func-a
)
# --- GRETH ---
    add_rumboot_target(
      CONFIGURATION IRAM_IM1
      FILES oi10/targets/test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE 
             -DEDCL_TEST_BANK=IM3
             -DGRETH_CMAKE_O32T
             -DRUMBOOT_SOC_O32T
      PREFIX greth0-edcl-im3
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM_IM1
      FILES oi10/targets/test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE
             -DEDCL_TEST_BANK=IM3
             -DGRETH_CMAKE_O32T
             -DRUMBOOT_SOC_O32T
      PREFIX greth1-edcl-im3
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM_IM1
      FILES oi10/targets/test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE
             -DSRC_HEAP_NAME="IM3"
             -DDST_HEAP_NAME="IM3"
             -DGRETH_CMAKE_O32T
             -DRUMBOOT_SOC_O32T
      PREFIX greth0-im3-im3
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM_IM1
      FILES oi10/targets/test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE
             -DSRC_HEAP_NAME="IM3"
             -DDST_HEAP_NAME="IM3"
             -DGRETH_CMAKE_O32T
             -DRUMBOOT_SOC_O32T
      PREFIX greth1-im3-im3
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM_IM1
      TESTGROUP test_greth_system
      FILES oi10/targets/test_o32t_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DTX_GRETH_BASE=GRETH_0_BASE 
             -DRX_GRETH_BASE=GRETH_1_BASE 
             #-DINIT_EMI
             -DDESC_HEAP_NAME="IM3"
             -DSRC_HEAP_NAME="IM3"
             -DDST_HEAP_NAME="IM3"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=60
             -DGRETH_CMAKE_O32T
             -DRUMBOOT_SOC_O32T
      PREFIX tx0-rx1-im3-im3_n8_60B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM_IM1
      TESTGROUP test_greth_system
      FILES oi10/targets/test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="IM3"
             -DSRC_HEAP_NAME="IM3"
             -DDST_HEAP_NAME="IM3"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=60
             #-DINIT_EMI
             -DGRETH_CMAKE_O32T
             -DRUMBOOT_SOC_O32T
      PREFIX 2_greth_system-im3-im3_n8_60B
    )
#---
add_rumboot_target(
  CONFIGURATION IRAM_IM0
  CFLAGS -DUSE_SWINT=132
  FILES common/irq/irq-atomics.c
)

add_rumboot_target(
  CONFIGURATION IRAM_IM0
  CFLAGS -DUSE_SWINT=132
  FILES common/irq/irq-defhandler.c
)

add_rumboot_target(
  CONFIGURATION IRAM_IM1
  FILES sctl_integration.c
)

add_rumboot_target(
  CONFIGURATION IRAM_IM0
  FILES scrb.c
  IRUN_FLAGS +insert_error_in_im1_im2_mem
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
