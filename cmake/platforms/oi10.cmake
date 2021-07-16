SET(RUMBOOT_ARCH ppc)
set(RUMBOOT_SUPPORTS_SPL_ENDIAN_SWAP Yes)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)

if (NOT RUMBOOT_SOC_BUILD_TYPE STREQUAL "RTL")
  set(IRUN_BOOTM_EXTRA_ARGS +BOOT_NOR=0)
else()
  set(IRUN_BOOTM_EXTRA_ARGS +BOOTMGR_PULLDOWN)
endif()


file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/arch/ppc/exception.c
    ${CMAKE_SOURCE_DIR}/src/arch/ppc/p64.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-mpic128.c
    ${CMAKE_SOURCE_DIR}/src/arch/ppc/ppc_mmu_impl.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/mpic128-timers.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/p64.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/plb6mcif2.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/dma2plb6.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/emi.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/l2inject.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/l2scrub.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/l2c.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/hscb.c
#    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/greth.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/ltrace.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/itrace.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/sp804.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/sp805.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/scrb_lib.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/ppc_mmu_impl.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/utlb_entries.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/ppc_476fp_power_modes.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/hscb_lib.c
)

#Flags for Power PC
macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)


    set(RUMBOOT_COMMON_FLAGS "-DRUMBOOT_HEADER_VERSION=2 -mcpu=476fp -fno-plt -fno-pic -m32 -ffreestanding -std=gnu99 -DRUMBOOT_PLATFORM_NUM_HEAPS=10 ")

    if ( "${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles")
      set(RUMBOOT_COMMON_FLAGS "${RUMBOOT_COMMON_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
    set(_filename_magic )
    else()
      set(RUMBOOT_COMMON_FLAGS "${RUMBOOT_COMMON_FLAGS} -D__FILENAME__=__FILE__")
    endif()


    set(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -mstrict-align -Wall -Wno-error=cpp -fdata-sections -ffunction-sections")
    set(CMAKE_ASM_FLAGS "${RUMBOOT_COMMON_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "-g -nostartfiles -static -Wl,--gc-sections")
    set(CMAKE_DUMP_FLAGS -M476,32)
    if (PRODUCTION_TESTING)
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPRODUCTION_TESTING")
    endif()
    if (RUMBOOT_SOC_O32T)
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRUMBOOT_SOC_O32T")
    endif()
endmacro()


if (RUMBOOT_BUILD_TYPE STREQUAL "Production")
  set(BOOTROM_IFLAGS +GTUBE_ONLY_PRODUCTION_OPCODES)
endif()

if (OI10_IRAM_IM0)
  set(BOOTROM_IFLAGS +GTUBE_ONLY_PRODUCTION_OPCODES +OI10_IRAM_IM0)
  set(IRAM_LDS_FILE oi10/iram_legacy.lds)
  set(STUB_LDS_FILE oi10/bootrom_legacy.lds)
  set(IRAM_SPL_LDS_FILE oi10/iram-spl-im0.lds)
  message(WARNING "Tests will run from IM0. This configuration is legacy and will be removed in future")
else()
  set(IRAM_LDS_FILE oi10/iram.lds)
  set(STUB_LDS_FILE oi10/bootrom.lds )
  set(IRAM_SPL_LDS_FILE oi10/iram-spl.lds)
endif()

rumboot_add_configuration(
    BROM
    DEFAULT
    LDFLAGS -Wl,-erumboot_entry_point
    PREFIX brom
    FEATURES ROMGEN
    FILES ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S
    TIMEOUT_CTEST 0
    LOAD BOOTROM_NOR SELF
    IRUN_FLAGS ${IRUN_BOOTM_EXTRA_ARGS}
    CFLAGS -DRUMBOOT_ONLY_STACK ${CONFIGURATION_ROM_CFLAGS} -DRUMBOOT_MAIN_NORETURN
    LDS ${STUB_LDS_FILE}
    IRUN_FLAGS +BOOTMGR_KEEP_DRIVING=1 ${BOOTROM_IFLAGS}
)

rumboot_add_configuration(
    BAREROM
    LDS oi10/rom.lds
    CFLAGS -DRUMBOOT_ONLY_STACK
    SNAPSHOT default
    PREFIX bare-rom
    TIMEOUT_CTEST 0
    TIMEOUT 50 ms
    LOAD BOOTROM_NOR SELF
    IRUN_FLAGS ${IRUN_BOOTM_EXTRA_ARGS}
    FEATURES EXTRACT_LABELS ROMGEN
)

rumboot_add_configuration(
  LPROBE_CPU
  PREFIX lprobe-cpu
  BOOTROM bootrom-lprobe-stub
  LOAD BOOTROM_NOR bootrom-lprobe-stub
  FEATURES LPROBE
  IRUN_FLAGS +LPROBE_MODE=CPU -input ${CMAKE_SOURCE_DIR}/../scripts/lprobe-helper.tcl
  IRUN_FLAGS ${IRUN_BOOTM_EXTRA_ARGS}
)


rumboot_add_configuration(
    SUPPLEMENTARY
    LDS oi10/rom.lds
    SNAPSHOT default
    PREFIX supplementary
    CFLAGS -DOI10_MINIMAL_INIT
    FEATURES STUB
)

rumboot_add_configuration (
    IRAM
    LDS ${IRAM_LDS_FILE}
    PREFIX iram
    LDFLAGS -Wl,-erumboot_main
    FILES ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
    BOOTROM bootrom-stub
    FEATURES LUA COVERAGE PACKIMAGE BANNER
    LOAD
      IM0BIN SELF
      BOOTROM_NOR bootrom-stub
    TIMEOUT_CTEST 86400
    IRUN_FLAGS ${IRUN_BOOTM_EXTRA_ARGS}
)

rumboot_add_configuration(IRAM_LE
  CONFIGURATION IRAM
  PREFIX iram-le
  LDFLAGS -Wl,-belf32-powerpcle -mlittle-endian -Wl,-emain
  CFLAGS -mlittle-endian
  DUMPFLAGS -EL
)

rumboot_add_configuration (
    IRAM_SPL
    CONFIGURATION IRAM
    LDS ${IRAM_SPL_LDS_FILE}
    PREFIX spl
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

#    expand_target_load(TARGET_LOAD TARGET_LOAD TARGET_LOAD_LIST)
#    list(REMOVE_ITEM TARGET_LOAD_LIST TARGET_LOAD_IM0BIN
#      TARGET_LOAD_SD0_BOOT_IMAGE
#      TARGET_LOAD_NOR_IMAGE
#      TARGET_LOAD_SPI0_CONF
#    )
#
#    foreach(mem ${TARGET_LOAD_LIST})
#      message("${CROSS_COMPILE}-ld -g -r ${PROJECT_BINARY_DIR}/${product} ${PROJECT_BINARY_DIR}/${${mem}} -o ${PROJECT_BINARY_DIR}/${product}.rwelf")
#    endforeach()
#

    list (FIND TARGET_FEATURES "ISS" _index)
    if (${_index} GREATER -1)
    ####################################################################################
    # NOTICE (!) For Victor Strukov                                                    #
    ####################################################################################
    # REMEMBER TO USE find_program to detect if riscwatch is installed.                #
    # If not, either exclude ISS tests from build or disable .gold.bin appending logic #
    # Ditch this notice when done                                                      #
    ####################################################################################
    add_custom_command(
        OUTPUT ${product}.gold.bin ${product}.tmp.bin
        COMMAND ${CROSS_COMPILE}-objcopy ${CMAKE_OBJCOPY_FLAGS} -O binary ${product} ${product}.tmp.bin
        #Replace cp with riscwatch command
        COMMAND cp ${product}.tmp.bin ${product}.gold.bin
        COMMENT "Generating ISS golden file for ${product}"
        DEPENDS ${product} utils
    )
    add_custom_target(
      ${product}.gold ALL
      DEPENDS ${product}.gold.bin
    )

    add_custom_command(
      APPEND
      OUTPUT ${product}.bin
      COMMAND cat ${product}.gold.bin >> ${product}.bin
      COMMAND ${packimage_cmd}
      COMMENT "Appending golden binary to ${product}"
      DEPENDS ${product}.gold.bin
    )

  endif()

endmacro()



include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/spels.cmake)

set(ROM_9600_OPTS +BOOT_SLOWUART=1 +BOOT_FASTUART=0 +UART0_SPEED=9600)
set(ROM_19200_OPTS +BOOT_SLOWUART=1 +BOOT_FASTUART=1 +UART0_SPEED=19200)
set(ROM_115200_OPTS +BOOT_SLOWUART=0 +BOOT_FASTUART=0 +UART0_SPEED=115200)
set(ROM_6500K_OPTS +BOOT_SLOWUART=0 +BOOT_FASTUART=1 +UART0_SPEED=6250000)

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)


    rumboot_spels_arch_tests(
      CONFIGURATION IRAM_SPL
      TESTGROUP hwonly
    )

    rumboot_spels_memory_test(
      CONFIGURATION IRAM_SPL
      START "IM2_BASE"
      END "(IM2_BASE + 0x20000)"
      NAME im2
      TESTGROUP hwonly
    )

    rumboot_spels_memory_test(
      CONFIGURATION IRAM_SPL
      START IM0_BASE
      NAME im0
      TESTGROUP hwonly
    )

    rumboot_spels_memory_test(
      CONFIGURATION IRAM_SPL
      END "(IM1_BASE+0x20000)"
      NAME im1
      TESTGROUP hwonly
    )

    if(NOT OI10_IRAM_IM0)
      # rumboot_bootrom_add_components(BROM IRAM_SPL FALSE FALSE)

      # rumboot_bootrom_add_common_units(
      #     CONFIGURATION BROM
      #     IRUN_FLAGS ${ROM_6500K_OPTS}
      # )

      # rumboot_bootrom_unit_test(
      #     ID 0
      #     CONFIGURATION BROM
      #     TAG sdio
      #     MEMTAG SD0_BOOT_IMAGE
      #     TAGOFFSET 0
      #     IRUN_FLAGS +select_sdio0 +BOOT_SD_CD=0 ${ROM_6500K_OPTS}
      #     ENDIAN big
      # )

      # rumboot_bootrom_unit_test(
      #     ID 1
      #     CONFIGURATION BROM
      #     TAG nor-no-ecc
      #     MEMTAG NOR_IMAGE
      #     TAGOFFSET 0
      #     IRUN_FLAGS +BOOT_EMI_ECC=0 ${ROM_6500K_OPTS}
      #     ENDIAN big
      # )

      # rumboot_bootrom_unit_test(
      #     ID 1
      #     CONFIGURATION BROM
      #     TAG nor-with-ecc
      #     MEMTAG NOR_IMAGE
      #     TAGOFFSET 0
      #     IRUN_FLAGS +BOOT_EMI_ECC=1 ${ROM_6500K_OPTS}
      #     ENDIAN big
      # )

      # rumboot_bootrom_unit_test(
      #     ID 2
      #     CONFIGURATION BROM
      #     TAG spi0_cs0
      #     MEMTAG SPI0_CONF
      #     TAGOFFSET 0
      #     FULL YES
      #     IRUN_FLAGS ${ROM_6500K_OPTS}
      #     ENDIAN big
      # )

    #   rumboot_bootrom_integration_test(BROM
    #       NAME "host-mockup-fallthough"
    #       IRUN_FLAGS ${ROM_6500K_OPTS}
    #       LOAD
    #         SD0_BOOT_IMAGE spl-fail
    #         SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
    #         NOR_IMAGE spl-fail-bad-magic
    #         HOSTMOCK  spl-ok
    #   )

    #   rumboot_bootrom_integration_test(BROM
    #       NAME "host-mockup"
    #       IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS}
    #       LOAD
    #         HOSTMOCK  spl-ok
    #   )

    #   rumboot_bootrom_integration_test(BROM
    #       NAME "host-fallthough-edcl"
    #       IRUN_FLAGS ${ROM_6500K_OPTS} +edcl_loader0
    #       LOAD
    #         SD0_BOOT_IMAGE spl-fail
    #         SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
    #         NOR_IMAGE spl-fail-bad-magic
    #         edcl_image  spl-ok
    #   )

    #   rumboot_bootrom_integration_test(BROM
    #       NAME "host-fallthough-rmap"
    #       IRUN_FLAGS ${ROM_6500K_OPTS} +rmap_master=0
    #       LOAD
    #         SD0_BOOT_IMAGE spl-fail
    #         SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
    #         NOR_IMAGE spl-fail-bad-magic
    #         rmap_file spl-ok
    #   )

    #   rumboot_bootrom_integration_test(BROM
    #       NAME "host-edcl-0"
    #       IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +edcl_loader0
    #       LOAD
    #         edcl_image  spl-ok
    #   )

    #   rumboot_bootrom_integration_test(BROM
    #       NAME "host-edcl-1"
    #       IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +edcl_loader1
    #       LOAD
    #         edcl_image spl-ok
    #   )

    #   rumboot_bootrom_integration_test(BROM
    #       NAME "host-rmap-0"
    #       IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +rmap_master=0
    #       LOAD
    #         rmap_file spl-ok
    #   )

    #   rumboot_bootrom_integration_test(BROM
    #       NAME "host-rmap-1"
    #       IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +rmap_master=1
    #       LOAD
    #         rmap_file spl-ok
    #   )

    #   rumboot_bootrom_integration_test(BROM
    #       NAME "host-rmap-2"
    #       IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +rmap_master=2
    #       LOAD
    #         rmap_file spl-ok
    #   )

    #   rumboot_bootrom_integration_test(BROM
    #       NAME "host-rmap-3"
    #       IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +rmap_master=3
    #       LOAD
    #         rmap_file spl-ok
    #   )

    # if (NOT RUMBOOT_BUILD_TYPE STREQUAL "Debug")
    #     rumboot_bootrom_integration_test(BROM
    #         NAME "host-easter-egg"
    #         IRUN_FLAGS +BOOT_HOST=1 ${ROM_6500K_OPTS} +uart_easter_egg
    #     )

    #     rumboot_bootrom_integration_test(BROM
    #         NAME "host-fallthough-easter-egg"
    #         IRUN_FLAGS ${ROM_6500K_OPTS} +uart_easter_egg
    #         LOAD
    #           SD0_BOOT_IMAGE spl-fail
    #           SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
    #           NOR_IMAGE spl-fail-bad-magic
    #     )
    # endif()

    # rumboot_bootrom_integration_test(BROM
    #     NAME "sdio-ok"
    #     IRUN_FLAGS +BOOT_SD_CD=0 +select_sdio0 ${ROM_6500K_OPTS}
    #     LOAD
    #       SD0_BOOT_IMAGE spl-ok
    #       NOR_IMAGE spl-fail
    #       SPI0_CONF spl-fail,spl-fail
    #       HOSTMOCK  spl-fail
    # )

    # rumboot_bootrom_integration_test(BROM
    # NAME "nor-with-ecc-ok-bypass"
    # IRUN_FLAGS ${ROM_6500K_OPTS} +BOOT_EMI_ECC=1 +BOOT_PLL_BYP=1
    # LOAD
    #   SD0_BOOT_IMAGE spl-fail-bad-magic
    #   SPI0_CONF spl-fail,spl-fail
    #   NOR_IMAGE spl-ok
    #   HOSTMOCK  spl-fail
    # )

    # rumboot_bootrom_integration_test(BROM
    #     NAME "nor-with-ecc-ok"
    #     IRUN_FLAGS ${ROM_6500K_OPTS} +BOOT_EMI_ECC=1
    #     LOAD
    #       SD0_BOOT_IMAGE spl-fail-bad-magic
    #       SPI0_CONF spl-fail,spl-fail
    #       NOR_IMAGE spl-ok
    #       HOSTMOCK  spl-fail
    # )

    # rumboot_bootrom_integration_test(BROM
    #     NAME "nor-with-ecc-with-cpu-ecc-ok"
    #     IRUN_FLAGS ${ROM_6500K_OPTS} +BOOT_EMI_ECC=1 +BOOT_CPU_ECC=1
    #     LOAD
    #       SD0_BOOT_IMAGE spl-fail-bad-magic
    #       SPI0_CONF spl-fail,spl-fail
    #       NOR_IMAGE spl-ok
    #       HOSTMOCK  spl-fail
    # )


    # rumboot_bootrom_integration_test(BROM
    #     NAME "nor-no-ecc-ok"
    #     IRUN_FLAGS +BOOT_EMI_ECC=0 ${ROM_6500K_OPTS}
    #     LOAD
    #       SD0_BOOT_IMAGE spl-fail-bad-magic
    #       SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
    #       NOR_IMAGE spl-ok
    #       HOSTMOCK  spl-fail
    # )

    # rumboot_bootrom_integration_test(BROM
    #     NAME "nor-with-ecc-ok-direct"
    #     IRUN_FLAGS +BOOT_EMI_ECC=1 ${ROM_6500K_OPTS} +BOOT_NOR=0
    #     LOAD
    #       SD0_BOOT_IMAGE spl-fail-bad-magic
    #       SPI0_CONF spl-fail,spl-fail
    #       NOR_IMAGE spl-ok
    #       HOSTMOCK  spl-fail
    #       BOOTROM_NOR bootrom-loader
    # )

    # rumboot_bootrom_integration_test(BROM
    #     NAME "nor-no-ecc-ok-direct"
    #     IRUN_FLAGS +BOOT_EMI_ECC=0 ${ROM_6500K_OPTS} +BOOT_NOR=0
    #     LOAD
    #       SD0_BOOT_IMAGE spl-fail-bad-magic
    #       SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
    #       NOR_IMAGE spl-ok
    #       HOSTMOCK  spl-fail
    #       BOOTROM_NOR bootrom-loader
    # )

    # rumboot_bootrom_integration_test(BROM
    #     NAME "spi-cs0-ok"
    #     IRUN_FLAGS ${ROM_6500K_OPTS}
    #     LOAD
    #       SPI0_CONF spl-ok,spl-fail
    #       HOSTMOCK  spl-fail
    # )

    # rumboot_bootrom_integration_test(BROM
    #     NAME "serial-9600"
    #     IRUN_FLAGS ${ROM_9600_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
    #     TIMEOUT 10 ms
    # )
    # rumboot_bootrom_integration_test(BROM
    #     NAME "serial-19200"
    #     IRUN_FLAGS ${ROM_19200_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
    #     TIMEOUT 10 ms
    # )
    # rumboot_bootrom_integration_test(BROM
    #     NAME "serial-115200"
    #     IRUN_FLAGS ${ROM_115200_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
    #     TIMEOUT 10 ms
    # )
    # rumboot_bootrom_integration_test(BROM
    #     NAME "serial-6500000"
    #     IRUN_FLAGS ${ROM_6500K_OPTS} +UART0_STOP_ON_MATCH +UART0_STOP_ON_MISMATCH
    #     TIMEOUT 10 ms
    # )
    endif()


    #Add lprobe sample scripts
    add_rumboot_target_dir(common/lua/
      CONFIGURATION LPROBE_CPU
      PREFIX lprobe-cpu
      TESTGROUP hwonly
    )

    #Cache scrubbing and error injection framework
    add_rumboot_target_dir(l2scrubber/tests
      CONFIGURATION IRAM
      TESTGROUP hwonly
      PREFIX l2scrub
      FILES l2scrubber/oi10.c tlbdump/ops.S
    )


    add_rumboot_target(
        CONFIGURATION BROM
        FILES common/bootrom-stubs/bootrom-lprobe-stub.c
        PREFIX "bootrom"
        NAME "lprobe-stub"
        FEATURES STUB
      )

    add_rumboot_target(
        CONFIGURATION BROM
        FILES common/bootrom-stubs/bootrom-stub.c
        PREFIX "bootrom"
        NAME "stub"
        FEATURES STUB
    )

    add_rumboot_target(
        CONFIGURATION BROM
        CFLAGS -DUSE_SWINT=132
        FILES common/irq/irq-atomics.c
        PREFIX "irq-rom"
    )

    add_rumboot_target(
      CONFIGURATION IRAM_LE
      CFLAGS -DUSE_SWINT=132
      FILES common/irq/irq-atomics.c
  )

    add_rumboot_target(
        CONFIGURATION BROM
        CFLAGS -DUSE_SWINT=132
        FILES common/irq/irq-defhandler.c
        PREFIX "irq-rom"
    )

    add_rumboot_target(
      CONFIGURATION BROM
      CFLAGS -DUSE_SWINT=132
      FILES common/irq/irq-context.c
      PREFIX "irq-context"
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

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SRAM0_BASE -DSW_ASSERTS_ONLY -DNOR_SELFCHECKING_DISABLE
          PREFIX sram0-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DCSP=CSP_1024 -DSDS=SDS_64M -DCHECK_2_1_3 -DCL=CL_2 -DSW_ASSERTS_ONLY
          IRUN_FLAGS +sdram_col10_row14
          PREFIX sdram-csp1024-cl2-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DCSP=CSP_2048 -DSDS=SDS_64M -DCHECK_2_1_3 -DCL=CL_2 -DSW_ASSERTS_ONLY
          PREFIX sdram-csp2048-cl2-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DCSP=CSP_1024 -DSDS=SDS_64M -DCHECK_2_1_3 -DCL=CL_3 -DSW_ASSERTS_ONLY
          IRUN_FLAGS +sdram_col10_row14
          PREFIX sdram-csp1024-cl3-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DCSP=CSP_2048 -DSDS=SDS_64M -DCHECK_2_1_3 -DCL=CL_3 -DSW_ASSERTS_ONLY
          PREFIX sdram-csp2048-cl3-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DSDS=SDS_64M -DCHECK_2_1_5 -DSW_ASSERTS_ONLY
          PREFIX sdram-sds64M-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DSDS=SDS_128M -DCHECK_2_1_5 -DSW_ASSERTS_ONLY
          IRUN_FLAGS +sdram_col11_row14
          PREFIX sdram-sds128M-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DSDS=SDS_256M -DCHECK_2_1_5 -DSW_ASSERTS_ONLY
          IRUN_FLAGS +sdram_col11_row15
          PREFIX sdram-sds256M-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SSRAM_BASE -DSSRAM_SST=SST_Flow_Through -DSSRAM_TSSOE=TSSOE_1 -DSW_ASSERTS_ONLY
          IRUN_FLAGS +SSRAM_FT=1
          PREFIX ssram-ft-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SSRAM_BASE -DSSRAM_SST=SST_Pipelined  -DSSRAM_TSSOE=TSSOE_2 -DSW_ASSERTS_ONLY
          PREFIX ssram-pipe-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=PIPELINED_BASE -DSW_ASSERTS_ONLY
          PREFIX pipelined-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SRAM1_BASE
          PREFIX sram1-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=NOR_BASE -DSW_ASSERTS_ONLY -DNOR_SELFCHECKING_DISABLE
          PREFIX nor-sw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SRAM0_BASE
          PREFIX sram0-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DCSP=CSP_1024 -DSDS=SDS_64M -DCHECK_2_1_3 -DCL=CL_2
          IRUN_FLAGS +sdram_col10_row14
          PREFIX sdram-csp1024-cl2-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DCSP=CSP_2048 -DSDS=SDS_64M -DCHECK_2_1_3 -DCL=CL_2
          PREFIX sdram-csp2048-cl2-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DCSP=CSP_1024 -DSDS=SDS_64M -DCHECK_2_1_3 -DCL=CL_3
          IRUN_FLAGS +sdram_col10_row14
          PREFIX sdram-csp1024-cl3-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DCSP=CSP_2048 -DSDS=SDS_64M -DCHECK_2_1_3 -DCL=CL_3
          PREFIX sdram-csp2048-cl3-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DSDS=SDS_64M -DCHECK_2_1_5
          PREFIX sdram-sds64M-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DSDS=SDS_128M -DCHECK_2_1_5
          IRUN_FLAGS +sdram_col11_row14
          PREFIX sdram-sds128M-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SDRAM_BASE -DSDS=SDS_256M -DCHECK_2_1_5
          IRUN_FLAGS +sdram_col11_row15
          PREFIX sdram-sds256M-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SSRAM_BASE -DSSRAM_SST=SST_Flow_Through -DSSRAM_TSSOE=TSSOE_1
          IRUN_FLAGS +SSRAM_FT=1
          PREFIX ssram-ft-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SSRAM_BASE -DSSRAM_SST=SST_Pipelined  -DSSRAM_TSSOE=TSSOE_2
          PREFIX ssram-pipe-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=PIPELINED_BASE
          PREFIX pipelined-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=SRAM1_BASE
          PREFIX sram1-sw-hw-asserts
        )

    add_rumboot_target(
          CONFIGURATION IRAM
          FILES test_oi10_em2_201.c
          CFLAGS -DEXT_MEM_BASE=NOR_BASE -DNOR_SELFCHECKING_DISABLE
          PREFIX nor-sw-hw-asserts
        )

    #---GRETH tests start---
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_ppc_regs_im1.c
      PREFIX ppc_bw
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
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DEDCL_TEST_BANK=IM2
      PREFIX greth0-edcl-im2
    )

     add_rumboot_target(
         CONFIGURATION IRAM
         FILES test_oi10_greth.c
         CFLAGS -DGRETH_BASE=GRETH_0_BASE -DEDCL_TEST_BANK=IM1
         PREFIX greth0-edcl-im1
     )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE  -DEDCL_TEST_BANK=SRAM0
      PREFIX greth0-edcl-em2
      TESTGROUP broken      
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DEDCL_TEST_BANK=IM2
      PREFIX greth1-edcl-im2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DEDCL_TEST_BANK=IM1
      PREFIX greth1-edcl-im1
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DEDCL_TEST_BANK=SRAM0
      PREFIX greth1-edcl-em2
      TESTGROUP broken
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_HEAP_NAME="IM1" -DDST_HEAP_NAME="IM1"
      PREFIX greth0-im1-im1
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_HEAP_NAME="IM2" -DDST_HEAP_NAME="IM2"
      PREFIX greth0-im2-im2
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_0_BASE -DSRC_HEAP_NAME="SRAM0" -DDST_HEAP_NAME="SRAM0" -DINIT_EMI
      PREFIX greth0-sram0-sram0
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_HEAP_NAME="IM1" -DDST_HEAP_NAME="IM1"
      PREFIX greth1-im1-im1
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_HEAP_NAME="IM2" -DDST_HEAP_NAME="IM2"
      PREFIX greth1-im2-im2
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_greth.c
      CFLAGS -DGRETH_BASE=GRETH_1_BASE -DSRC_HEAP_NAME="SRAM0" -DDST_HEAP_NAME="SRAM0" -DINIT_EMI
      PREFIX greth1-sram0-sram0
    )
    
#----- TX-RX --- IM2 --- 
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DTX_GRETH_BASE=GRETH_0_BASE 
             -DRX_GRETH_BASE=GRETH_1_BASE 
             #-DINIT_EMI
             -DDESC_HEAP_NAME="IM2"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="IM2"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=60
      PREFIX tx0-rx1-im2-im2_n8_60B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DTX_GRETH_BASE=GRETH_0_BASE 
             -DRX_GRETH_BASE=GRETH_1_BASE 
             #-DINIT_EMI
             -DDESC_HEAP_NAME="IM2"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="IM2"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=65
      PREFIX tx0-rx1-im2-im2_n8_65B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DTX_GRETH_BASE=GRETH_0_BASE 
             -DRX_GRETH_BASE=GRETH_1_BASE 
             #-DINIT_EMI
             -DDESC_HEAP_NAME="IM2"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="IM2"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=1500
      PREFIX tx0-rx1-im2-im2_n8_1500B
    )

#----- TX-RX --- SRAM0 ---
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DTX_GRETH_BASE=GRETH_0_BASE 
             -DRX_GRETH_BASE=GRETH_1_BASE 
             -DINIT_EMI
             -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="SRAM0"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=60
      PREFIX tx0-rx1-sram0-sram0_n8_60B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DTX_GRETH_BASE=GRETH_0_BASE 
             -DRX_GRETH_BASE=GRETH_1_BASE 
             -DINIT_EMI
             -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="SRAM0"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=65
      PREFIX tx0-rx1-sram0-sram0_n8_65B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DTX_GRETH_BASE=GRETH_0_BASE 
             -DRX_GRETH_BASE=GRETH_1_BASE 
             -DINIT_EMI
             -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="SRAM0"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=1500
      PREFIX tx0-rx1-sram0-sram0_n8_1500B
    )
#----- TX-RX --- IM2 - SRAM0 ---
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DTX_GRETH_BASE=GRETH_0_BASE 
             -DRX_GRETH_BASE=GRETH_1_BASE 
             -DINIT_EMI
             -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=60
      PREFIX tx0-rx1-im2-sram0_n8_60B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DTX_GRETH_BASE=GRETH_0_BASE 
             -DRX_GRETH_BASE=GRETH_1_BASE 
             -DINIT_EMI
             -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=65
      PREFIX tx0-rx1-im2-sram0_n8_65B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DTX_GRETH_BASE=GRETH_0_BASE 
             -DRX_GRETH_BASE=GRETH_1_BASE 
             -DINIT_EMI
             -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=1500
      PREFIX tx0-rx1-im2-sram0_n8_1500B
    )
#----- 2 ETH --- IM2 ---   
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="IM2"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="IM2"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=60
             -DINIT_EMI
      PREFIX 2_greth_system-im2-im2_n8_60B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="IM2"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="IM2"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=65
             -DINIT_EMI
      PREFIX 2_greth_system-im2-im2_n8_65B
    )
         
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="IM2"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="IM2"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=1500
             -DINIT_EMI
      PREFIX 2_greth_system-im2-im2_n8_1500B
    )
    
#----- 2 ETH --- SRAM0 --- 
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="SRAM0"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=60
             -DINIT_EMI
      PREFIX 2_greth_system-sram0-sram0_n8_60B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="SRAM0"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=65
             -DINIT_EMI
      PREFIX 2_greth_system-sram0-sram0_n8_65B
    )
         
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="SRAM0"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=1500
             -DINIT_EMI
      PREFIX 2_greth_system-sram0-sram0_n8_1500B
    )
        
#----- 2 ETH --- IM2 - SRAM0 --- 
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=60
             -DINIT_EMI
      PREFIX 2_greth_system-desc_sram0-im2-sram0_n8_60B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=65
             -DINIT_EMI
      PREFIX 2_greth_system-desc_sram0-im2-sram0_n8_65B
    )
         
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="SRAM0"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=1500
             -DINIT_EMI
      PREFIX 2_greth_system-desc_sram0-im2-sram0_n8_1500B
    )
         
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="IM2"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=60
             -DINIT_EMI
      PREFIX 2_greth_system-desc_im2-im2-sram0_n8_60B
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="IM2"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=65
             -DINIT_EMI
      PREFIX 2_greth_system-desc_im2-im2-sram0_n8_65B
    )
         
    add_rumboot_target(
      CONFIGURATION IRAM
      TESTGROUP test_greth_system
      FILES test_o32t_2_greth_system.c
      IRUN_FLAGS +enable_IPG_checker
      CFLAGS -DDESC_HEAP_NAME="IM2"
             -DSRC_HEAP_NAME="IM2"
             -DDST_HEAP_NAME="SRAM0"
             -DN_DESC=8
             -DGRETH_TEST_DATA_LEN_BYTES=1500
             -DINIT_EMI
      PREFIX 2_greth_system-desc_im2-im2-sram0_n8_1500B
    )
#---GRETH tests finish---

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_spi.c
      CFLAGS -DGSPI_BASE=GSPI_0_BASE -DGSPI_INT=GSPI0_INT -DGSPI_CHECK_REGS -DGPIO1_X=0x1
      IRUN_FLAGS +spi_irq_test
                 +BOOTMGR_PULLDOWN
      PREFIX gspi0
    )
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_spi.c
      CFLAGS -DGSPI_BASE=GSPI_1_BASE -DGSPI_INT=GSPI1_INT -DGSPI_CHECK_REGS -DGPIO1_X=0x2
      IRUN_FLAGS +spi_irq_test
                 +BOOTMGR_PULLDOWN
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
        CONFIGURATION BROM
    )

    add_rumboot_target_dir(tests/
        CONFIGURATION BROM
    )

    add_rumboot_target_dir(jenkins/
        CONFIGURATION IRAM
        PREFIX jenkins
        TESTGROUP short
    )

    add_rumboot_target_dir(simple-iram/
        CONFIGURATION IRAM
        PREFIX simple-iram
        FEATURES EXTRACT_LABELS
    )

if(RUMBOOT_SOC_O32T)
  if (RUMBOOT_SOC_BUILD_TYPE MATCHES "RTL")
      if (OI10_IRAM_IM0)
          add_rumboot_target(
              CONFIGURATION IRAM
              FILES test_oi10_ctrl_000.c
              CFLAGS -DMBIST_RELOCATE=KMBIST_CHAIN_SF_8_IM0 -DRELOCATE_FROM=IM0_BASE -DRELOCATE_TO=IM2_BASE
          )
      else()
          add_rumboot_target(
              CONFIGURATION IRAM
              FILES test_oi10_ctrl_000.c
              CFLAGS -DMBIST_RELOCATE=KMBIST_CHAIN_SF_6_IM1 -DRELOCATE_FROM=IM1_BASE -DRELOCATE_TO=IM2_BASE
          )
      endif()
  else()
      if (OI10_IRAM_IM0)
          add_rumboot_target(
              CONFIGURATION IRAM
              FILES test_oi10_ctrl_000.c
              CFLAGS -DMBIST_RELOCATE=KMBIST_CHAIN_SF_8_IM0 -DRELOCATE_FROM=IM0_BASE -DRELOCATE_TO=IM2_BASE
          )
          add_rumboot_target(
              CONFIGURATION IRAM
              FILES test_oi10_ctrl_000.c
              CFLAGS -DMBIST_RELOCATE=KMBIST_CHAIN_SF_8_IM0 -DRELOCATE_FROM=IM0_BASE -DRELOCATE_TO=IM2_BASE -DERR_INJ_ENABLED
              NAME "test_oi10_ctrl_000_inj"
          )
      else()
          add_rumboot_target(
              CONFIGURATION IRAM
              FILES test_oi10_ctrl_000.c
              CFLAGS -DMBIST_RELOCATE=KMBIST_CHAIN_SF_6_IM1 -DRELOCATE_FROM=IM1_BASE -DRELOCATE_TO=IM2_BASE
          )
          add_rumboot_target(
              CONFIGURATION IRAM
              FILES test_oi10_ctrl_000.c
              CFLAGS -DMBIST_RELOCATE=KMBIST_CHAIN_SF_6_IM1 -DRELOCATE_FROM=IM1_BASE -DRELOCATE_TO=IM2_BASE -DERR_INJ_ENABLED
          NAME "test_oi10_ctrl_000_inj"
          )
      endif()
  endif()
endif()

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_038_im2.lds
        FILES test_oi10_cpu_038_helper.c
        CFLAGS -DTEST_OI10_CPU_038_ARRAY_SIZE=0x7C00
        NAME "test_oi10_cpu_038_helper_im2"
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
        LDS oi10/test_oi10_cpu_038_im1.lds
        FILES test_oi10_cpu_038_helper.c
        CFLAGS -DTEST_OI10_CPU_038_ARRAY_SIZE=0x7C00
        NAME "test_oi10_cpu_038_helper_im0"
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_038_sram0.lds
        FILES test_oi10_cpu_038_helper.c
        CFLAGS -DTEST_OI10_CPU_038_ARRAY_SIZE=0xFC00
        NAME "test_oi10_cpu_038_helper_em2"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_038.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=IM2_BASE
        NAME "test_oi10_cpu_038_im2"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_038_helper_im2
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_038.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=IM1_BASE
        NAME "test_oi10_cpu_038_im1"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_038_helper_im1
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_038.c
        PREFIX simple-iram
        CFLAGS -DTEST_OI10_CPU_038_IM0 -DM_BASE=IM1_BASE
        NAME "test_oi10_cpu_038_im0"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_038_helper_im0
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_038.c
        PREFIX simple-iram
        CFLAGS -DEMI_INIT -DM_BASE=SRAM0_BASE
        NAME "test_oi10_cpu_038_em2"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_038_helper_em2
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_039_sram0.lds
        FILES test_oi10_cpu_039_00_helper.c
        CFLAGS -DTEST_OI10_CPU_039_MEM_SIZE=0x80000
        NAME "test_oi10_cpu_039_helper_em2"
    )
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_039_sram0.lds
        FILES test_oi10_cpu_039_00_helper.c
        CFLAGS -DTEST_OI10_CPU_039_MEM_SIZE=0x100
        NAME "test_oi10_cpu_039_helper_em2_short"
    )
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_039_sram0.lds
        FILES test_oi10_cpu_040_00_helper.c
        CFLAGS -DTEST_OI10_CPU_039_MEM_SIZE=0x80000
        NAME "test_oi10_cpu_040_helper_em2"
    )
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_039_sram0.lds
        FILES test_oi10_cpu_040_00_helper.c
        CFLAGS -DTEST_OI10_CPU_039_MEM_SIZE=0x100
        NAME "test_oi10_cpu_040_helper_em2_short"
    )

   add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_039_sram0.lds
        FILES test_oi10_cpu_039_00_helper_im.c
        CFLAGS -DTEST_OI10_CPU_039_MEM_SIZE=0x80000
        NAME "test_oi10_cpu_039_helper_im_em2"
    )
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_039_sram0.lds
        FILES test_oi10_cpu_039_00_helper_im.c
        CFLAGS -DTEST_OI10_CPU_039_MEM_SIZE=0x100
        NAME "test_oi10_cpu_039_helper_im_em2_short"
    )
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_039_sram0.lds
        FILES test_oi10_cpu_040_00_helper_im.c
        CFLAGS -DTEST_OI10_CPU_039_MEM_SIZE=0x80000
        NAME "test_oi10_cpu_040_helper_im_em2"
    )
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_039_sram0.lds
        FILES test_oi10_cpu_040_00_helper_im.c
        CFLAGS -DTEST_OI10_CPU_039_MEM_SIZE=0x100
        NAME "test_oi10_cpu_040_helper_im_em2_short"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=1 -DL2C_IL1D_BIT=1 -DL2C_W_BIT=1
        NAME "test_oi10_cpu_039_00"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_039_helper_em2
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=1 -DL2C_W_BIT=1
        NAME "test_oi10_cpu_039_01"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_039_helper_em2
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=1 -DL2C_IL1D_BIT=0 -DL2C_W_BIT=1
        NAME "test_oi10_cpu_039_10"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_039_helper_em2
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=0 -DL2C_W_BIT=1
        NAME "test_oi10_cpu_039_11"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_039_helper_em2
    )
   add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00_im.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=0 -DL2C_W_BIT=1
        NAME "test_oi10_cpu_039_11_im"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_039_helper_im_em2
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=1 -DL2C_IL1D_BIT=1 -DL2C_W_BIT=0
        NAME "test_oi10_cpu_040_00"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_040_helper_em2
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=1 -DL2C_W_BIT=0
        NAME "test_oi10_cpu_040_01"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_040_helper_em2
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=1 -DL2C_IL1D_BIT=0 -DL2C_W_BIT=0
        NAME "test_oi10_cpu_040_10"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_040_helper_em2
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=0 -DL2C_W_BIT=0
        NAME "test_oi10_cpu_040_11"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_040_helper_em2
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00_im.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=0 -DL2C_W_BIT=0
        NAME "test_oi10_cpu_040_11_im"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_040_helper_im_em2
    )
   add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=0 -DL2C_W_BIT=1
        NAME "test_oi10_cpu_039_11_short"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_039_helper_em2_short
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=0 -DL2C_W_BIT=0
        NAME "test_oi10_cpu_040_11_short"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_040_helper_em2_short
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00_im.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=0 -DL2C_W_BIT=1
        NAME "test_oi10_cpu_039_11_im_short"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_039_helper_im_em2_short
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_039_00_im.c
        PREFIX simple-iram
        CFLAGS -DM_BASE=SRAM0_BASE -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=0 -DL2C_W_BIT=0
        NAME "test_oi10_cpu_040_11_im_short"
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_040_helper_im_em2_short
    )
    add_rumboot_target_dir(iss-iram/
        CONFIGURATION IRAM
        PREFIX iss-iram
        CHECKCMD ${CMAKE_SOURCE_DIR}/scripts/ISS/dump_gen.csh ${CMAKE_BINARY_DIR} $PRODUCT ${CMAKE_SOURCE_DIR}/scripts/ISS rumboot-${RUMBOOT_PLATFORM}-${RUMBOOT_BUILD_TYPE}
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DGPIO_X_BASE=GPIO_0_BASE -DGPIO_X_INT=GPIO0_INT -DCHECK_REGS -DGPIO_TIMEOUT=0x200 IRUN_FLAGS +gpio_test
        FILES test_oi10_gpio.c
        IRUN_FLAGS +BOOTMGR_PULLDOWN
        PREFIX "gpio_0"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DGPIO_X_BASE=GPIO_1_BASE -DGPIO_X_INT=GPIO1_INT -DCHECK_REGS -DGPIO_TIMEOUT=0x200 IRUN_FLAGS +gpio_test
        FILES test_oi10_gpio.c
        IRUN_FLAGS +BOOTMGR_PULLDOWN
        PREFIX "gpio_1"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES chain-start-dummy.c
        PREFIX "simple-iram"
        NAME chain-start-dummy
        TESTGROUP hwonly
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES chain-end-dummy.c
        PREFIX "simple-iram"
        NAME chain-end-dummy
        TESTGROUP hwonly
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="SRAM0"
               -DRX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="IM1"
               -DRX_1_HEAP_NAME="SRAM0"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_sram0_im1_im1_sram0
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="SRAM1"
               -DRX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="IM1"
               -DRX_1_HEAP_NAME="SRAM1"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_sram1_im1_im1_sram1
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="SDRAM"
               -DRX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="IM1"
               -DRX_1_HEAP_NAME="SDRAM"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_sdram_im1_im1_sdram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="SSRAM"
               -DRX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="IM1"
               -DRX_1_HEAP_NAME="SSRAM"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_ssram_im1_im1_ssram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="SSRAM"
               -DRX_0_HEAP_NAME="SRAM1"
               -DTX_1_HEAP_NAME="PLRAM"
               -DRX_1_HEAP_NAME="SDRAM"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_ssram_sram1_plram_sdram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="SDRAM"
               -DRX_0_HEAP_NAME="SSRAM"
               -DTX_1_HEAP_NAME="SRAM1"
               -DRX_1_HEAP_NAME="PLRAM"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_sdram_ssram_sram1_plram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="PLRAM"
               -DRX_0_HEAP_NAME="SDRAM"
               -DTX_1_HEAP_NAME="SSRAM"
               -DRX_1_HEAP_NAME="SRAM1"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_plram_sdram_ssram_sram1
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="SRAM1"
               -DRX_0_HEAP_NAME="PLRAM"
               -DTX_1_HEAP_NAME="SDRAM"
               -DRX_1_HEAP_NAME="SSRAM"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_sram1_plram_sdram_ssram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="PLRAM"
               -DRX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="IM1"
               -DRX_1_HEAP_NAME="PLRAM"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_plram_im1_im1_plram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_0-hscb_1"
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
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="IM2"
               -DRX_0_HEAP_NAME="IM2"
               -DRX_1_HEAP_NAME="IM1"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_0-hscb_1"
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
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
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
               -DTX_0_HEAP_NAME="SSRAM"
               -DTX_1_HEAP_NAME="SSRAM"
               -DRX_0_HEAP_NAME="SSRAM"
               -DRX_1_HEAP_NAME="SSRAM"
               -DDATA_SIZE_0=32
               -DDATA_SIZE_1=257
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_ssram_ssram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DTX_0_HEAP_NAME="PLRAM"
               -DTX_1_HEAP_NAME="PLRAM"
               -DRX_0_HEAP_NAME="PLRAM"
               -DRX_1_HEAP_NAME="PLRAM"
               -DDATA_SIZE_0=32
               -DDATA_SIZE_1=257
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_plram_plram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DTX_0_HEAP_NAME="SRAM0"
               -DTX_1_HEAP_NAME="SRAM0"
               -DRX_0_HEAP_NAME="SRAM0"
               -DRX_1_HEAP_NAME="SRAM0"
               -DDATA_SIZE_0=32
               -DDATA_SIZE_1=257
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_sram0_sram0
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DTX_0_HEAP_NAME="SRAM1"
               -DTX_1_HEAP_NAME="SRAM1"
               -DRX_0_HEAP_NAME="SRAM1"
               -DRX_1_HEAP_NAME="SRAM1"
               -DDATA_SIZE_0=32
               -DDATA_SIZE_1=257
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_sram1_sram1
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="SRAM0"
               -DRX_0_HEAP_NAME="SSRAM"
               -DTX_1_HEAP_NAME="SRAM1"
               -DRX_1_HEAP_NAME="PLRAM"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_em"
        NAME test_oi10_hscb_sram0_ssram_sram1_plram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB1_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB0_BASE
               -DHSCB_UNDER_TEST_INT=SW1_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW1_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW0_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW0_AXI_INT
               -DTX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="SDRAM"
               -DRX_0_HEAP_NAME="SSRAM"
               -DRX_1_HEAP_NAME="PLRAM"
               -DDATA_SIZE_0=320
               -DDATA_SIZE_1=257
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_1-hscb_0"
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
               -DHSCB_FUNC_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_0-hscb_1"
        NAME test_oi10_hscb_long_im2_ssram_im1_im2
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB2_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB3_BASE
               -DHSCB_UNDER_TEST_INT=SW2_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW2_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW3_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW3_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_2-hscb_3"
        NAME test_oi10_hscb_im1_im1
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB2_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB3_BASE
               -DHSCB_UNDER_TEST_INT=SW2_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW2_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW3_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW3_AXI_INT
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DTX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="IM2"
               -DRX_0_HEAP_NAME="IM2"
               -DRX_1_HEAP_NAME="IM1"
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_2-hscb_3"
        NAME test_oi10_hscb_im1_im2
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB2_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB3_BASE
               -DHSCB_UNDER_TEST_INT=SW2_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW2_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW3_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW3_AXI_INT
               -DTX_0_HEAP_NAME="SDRAM"
               -DTX_1_HEAP_NAME="SDRAM"
               -DRX_0_HEAP_NAME="SDRAM"
               -DRX_1_HEAP_NAME="SDRAM"
               -DDATA_SIZE_0=32
               -DDATA_SIZE_1=257
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_2-hscb_3"
        NAME test_oi10_hscb_sdram_sdram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB3_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB2_BASE
               -DHSCB_UNDER_TEST_INT=SW3_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW3_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW2_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW2_AXI_INT
               -DTX_0_HEAP_NAME="IM1"
               -DTX_1_HEAP_NAME="SDRAM"
               -DRX_0_HEAP_NAME="SSRAM"
               -DRX_1_HEAP_NAME="PLRAM"
               -DDATA_SIZE_0=320
               -DDATA_SIZE_1=257
               -DHSCB_FUNC_TEST
               -DHSCB_SHORT_TEST
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_3-hscb_2"
        NAME test_oi10_hscb_im1_ssram_plram
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB2_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB3_BASE
               -DHSCB_UNDER_TEST_INT=SW2_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW2_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW3_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW3_AXI_INT
               -DHSCB_FUNC_TEST
               #-DTEST_OI10_HSCB_FULL_TRACING
              #-DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_2-hscb_3"
        NAME test_oi10_hscb_long_im2_ssram_im1_im2
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_0"
        NAME test_oi10_hscb_reg
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB1_BASE
               -DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_1"
        NAME test_oi10_hscb_reg
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB2_BASE
               -DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_2"
        NAME test_oi10_hscb_reg
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB3_BASE
               -DCHECK_REGS
        FILES test_oi10_hscb.c
        PREFIX "hscb_3"
        NAME test_oi10_hscb_reg
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               #-DTEST_OI10_HSCB_FULL_TRACING
        FILES test_oi10_hscb_rmap.c
        PREFIX "hscb_rmap_0"
        NAME test_oi10_hscb_rmap
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB1_BASE
               -DHSCB_UNDER_TEST_INT=SW0_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW0_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW1_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW1_AXI_INT
               #-DTEST_OI10_HSCB_FULL_TRACING
               -DRMAP_PACKET_LIST=WnV_RI_sram1,RRI_sram1
               -DDATA_SIZE_SRAM1=0x1019
        FILES test_oi10_hscb_rmap.c
        PREFIX "hscb_rmap_0"
        NAME test_oi10_hscb_rmap_only_sram1
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB1_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB0_BASE
               -DHSCB_UNDER_TEST_INT=SW1_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW1_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW0_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW0_AXI_INT
               #-DTEST_OI10_HSCB_FULL_TRACING
        FILES test_oi10_hscb_rmap.c
        PREFIX "hscb_rmap_1"
        NAME test_oi10_hscb_rmap
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB2_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB3_BASE
               -DHSCB_UNDER_TEST_INT=SW2_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW2_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW3_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW3_AXI_INT
               #-DTEST_OI10_HSCB_FULL_TRACING
        FILES test_oi10_hscb_rmap.c
        PREFIX "hscb_rmap_2"
        NAME test_oi10_hscb_rmap
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB3_BASE
               -DHSCB_SUPPLEMENTARY_BASE=HSCB2_BASE
               -DHSCB_UNDER_TEST_INT=SW3_HSCB_INT
               -DHSCB_UNDER_TEST_DMA_INT=SW3_AXI_INT
               -DHSCB_SUPPLEMENTARY_INT=SW2_HSCB_INT
               -DHSCB_SUPPLEMENTARY_DMA_INT=SW2_AXI_INT
               #-DTEST_OI10_HSCB_FULL_TRACING
        FILES test_oi10_hscb_rmap.c
        PREFIX "hscb_rmap_3"
        NAME test_oi10_hscb_rmap
    )

#---------------------------- HSCB_NEW    
     add_rumboot_target(
        CONFIGURATION IRAM
        #CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
        FILES test_oi10_hscb_simult_big.c
        PREFIX "hscb_simult"
        NAME test_hscb_big
    )
    
    add_rumboot_target(
        CONFIGURATION IRAM
        #CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
        CFLAGS -DHSCB1_TX_DSCTBL_BASE="IM1"
               -DHSCB1_TX_DATA_BASE="IM1"
               -DHSCB1_RX_DSCTBL_BASE="IM1"
               -DHSCB1_RX_DATA_BASE="IM1"
               -DHSCB2_TX_DSCTBL_BASE="IM1"
               -DHSCB2_TX_DATA_BASE="IM1"
               -DHSCB2_RX_DSCTBL_BASE="IM1"
               -DHSCB2_RX_DATA_BASE="IM1"
               -DHSCB3_TX_DSCTBL_BASE="IM1"
               -DHSCB3_TX_DATA_BASE="IM1"
               -DHSCB3_RX_DSCTBL_BASE="IM1"
               -DHSCB3_RX_DATA_BASE="IM1"
        FILES test_oi10_hscb_simult_big.c
        PREFIX "hscb0_emi_others_im1"
        NAME test_hscb_big
    )
    
        add_rumboot_target(
        CONFIGURATION IRAM
        #CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
        CFLAGS -DHSCB0_TX_DSCTBL_BASE="SSRAM"
               -DHSCB0_TX_DATA_BASE="SSRAM"
               -DHSCB0_RX_DSCTBL_BASE="SSRAM"
               -DHSCB0_RX_DATA_BASE="SSRAM"
               -DHSCB1_TX_DSCTBL_BASE="SSRAM"
               -DHSCB1_TX_DATA_BASE="SSRAM"
               -DHSCB1_RX_DSCTBL_BASE="SSRAM"
               -DHSCB1_RX_DATA_BASE="SSRAM"
               -DHSCB2_TX_DSCTBL_BASE="SSRAM"
               -DHSCB2_TX_DATA_BASE="SSRAM"
               -DHSCB2_RX_DSCTBL_BASE="SSRAM"
               -DHSCB2_RX_DATA_BASE="SSRAM"
               -DHSCB3_TX_DSCTBL_BASE="SSRAM"
               -DHSCB3_TX_DATA_BASE="SSRAM"
               -DHSCB3_RX_DSCTBL_BASE="SSRAM"
               -DHSCB3_RX_DATA_BASE="SSRAM"
               #-DDEBUG_PRINT=1
        FILES test_oi10_hscb_simult_big.c
        PREFIX "hscb_all_ssram"
        NAME test_hscb_big
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        #CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
        CFLAGS -DHSCB0_TX_DSCTBL_BASE="SSRAM"
               -DHSCB0_TX_DATA_BASE="SSRAM"
               -DHSCB0_RX_DSCTBL_BASE="SSRAM"
               -DHSCB0_RX_DATA_BASE="SSRAM"
               -DHSCB1_TX_DSCTBL_BASE="IM1"
               -DHSCB1_TX_DATA_BASE="IM1"
               -DHSCB1_RX_DSCTBL_BASE="IM1"
               -DHSCB1_RX_DATA_BASE="IM1"
               -DHSCB2_TX_DSCTBL_BASE="IM1"
               -DHSCB2_TX_DATA_BASE="IM1"
               -DHSCB2_RX_DSCTBL_BASE="IM1"
               -DHSCB2_RX_DATA_BASE="IM1"
               -DHSCB3_TX_DSCTBL_BASE="IM1"
               -DHSCB3_TX_DATA_BASE="IM1"
               -DHSCB3_RX_DSCTBL_BASE="IM1"
               -DHSCB3_RX_DATA_BASE="IM1"
        FILES test_oi10_hscb_simult_big.c
        PREFIX "hscb0_ssram_others_im1"
        NAME test_hscb_big
    )  

        add_rumboot_target(
        CONFIGURATION IRAM
        #CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
        CFLAGS -DHSCB0_TX_DSCTBL_BASE="IM1"
               -DHSCB0_TX_DATA_BASE="SSRAM"
               -DHSCB0_RX_DSCTBL_BASE="IM1"
               -DHSCB0_RX_DATA_BASE="SSRAM"
               -DHSCB1_TX_DSCTBL_BASE="IM1"
               -DHSCB1_TX_DATA_BASE="SSRAM"
               -DHSCB1_RX_DSCTBL_BASE="IM1"
               -DHSCB1_RX_DATA_BASE="SSRAM"
               -DHSCB2_TX_DSCTBL_BASE="IM1"
               -DHSCB2_TX_DATA_BASE="SSRAM"
               -DHSCB2_RX_DSCTBL_BASE="IM1"
               -DHSCB2_RX_DATA_BASE="SSRAM"
               -DHSCB3_TX_DSCTBL_BASE="IM1"
               -DHSCB3_TX_DATA_BASE="SSRAM"
               -DHSCB3_RX_DSCTBL_BASE="IM1"
               -DHSCB3_RX_DATA_BASE="SSRAM"
               -DSIZE_OF_PACKET=1024
               -DN_OF_PACKETS=2
               #-DDEBUG_PRINT=1
        FILES test_oi10_hscb_simult_big.c
        PREFIX "hscb_desc_im1_data_ssram"
        NAME test_hscb_big
        )

        add_rumboot_target(
        CONFIGURATION IRAM
        #CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
        CFLAGS -DHSCB0_TX_DSCTBL_BASE="IM1"
               -DHSCB0_TX_DATA_BASE="IM1"
               -DHSCB0_RX_DSCTBL_BASE="IM1"
               -DHSCB0_RX_DATA_BASE="SSRAM"
               -DHSCB1_TX_DSCTBL_BASE="IM2"
               -DHSCB1_TX_DATA_BASE="IM2"
               -DHSCB1_RX_DSCTBL_BASE="IM2"
               -DHSCB1_RX_DATA_BASE="SSRAM"
               -DHSCB2_TX_DSCTBL_BASE="IM1"
               -DHSCB2_TX_DATA_BASE="IM1"
               -DHSCB2_RX_DSCTBL_BASE="IM1"
               -DHSCB2_RX_DATA_BASE="SSRAM"
               -DHSCB3_TX_DSCTBL_BASE="IM1"
               -DHSCB3_TX_DATA_BASE="SSRAM"
               -DHSCB3_RX_DSCTBL_BASE="IM1"
               -DHSCB3_RX_DATA_BASE="SSRAM"
               -DSIZE_OF_PACKET=1024
               -DN_OF_PACKETS=2
               #-DDEBUG_PRINT=1
        FILES test_oi10_hscb_simult_big.c
        PREFIX "hscb_desc_tx_data_im1_rx_data_ssram"
        NAME test_hscb_big
        )        
        
        add_rumboot_target(
        CONFIGURATION IRAM
        #CFLAGS -DHSCB_UNDER_TEST_BASE=HSCB0_BASE
        CFLAGS -DHSCB0_TX_DSCTBL_BASE="IM1"
               -DHSCB0_TX_DATA_BASE="SSRAM"
               -DHSCB0_RX_DSCTBL_BASE="IM1"
               -DHSCB0_RX_DATA_BASE="SSRAM"
               -DHSCB1_TX_DSCTBL_BASE="IM1"
               -DHSCB1_TX_DATA_BASE="SSRAM"
               -DHSCB1_RX_DSCTBL_BASE="IM1"
               -DHSCB1_RX_DATA_BASE="SSRAM"
               -DHSCB2_TX_DSCTBL_BASE="IM1"
               -DHSCB2_TX_DATA_BASE="SSRAM"
               -DHSCB2_RX_DSCTBL_BASE="IM1"
               -DHSCB2_RX_DATA_BASE="SSRAM"
               -DHSCB3_TX_DSCTBL_BASE="IM1"
               -DHSCB3_TX_DATA_BASE="SSRAM"
               -DHSCB3_RX_DSCTBL_BASE="IM1"
               -DHSCB3_RX_DATA_BASE="SSRAM"
               -DCOM_SRC_HEAP="SSRAM"
               -DCOM_DST_HEAP="SSRAM"
               -DSIZE_OF_PACKET=4096
               -DN_OF_PACKETS=2               
               #-DDEBUG_PRINT=1
        FILES test_oi10_hscb_com_simult_big.c
        PREFIX "hscb_com_desc_im1_data_ssram"
        NAME test_hscb_com_big
    )    
#-------------------------------------------------------    
    
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
      FILES test_oi10_ctrl_005.c
      NAME "test_oi10_ctrl_005_soft"
      LOAD IM0BIN SELF,SELF,SELF,SELF,SELF
      TESTGROUP broken
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_ctrl_005.c
      NAME "test_oi10_ctrl_005_hard"
      CFLAGS -DTEST_OI10_CTRL_005_HARD
      LOAD IM0BIN SELF,SELF,SELF,SELF,SELF,SELF
      TESTGROUP broken
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
      FILES test_oi10_lscb.c
      CFLAGS -DMKIO_BASE=MKIO0_BASE -DTEST_MODE__BC_TRANSMIT=0 -DCHECK_MKIO_FUNC -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="SSRAM"
      IRUN_FLAGS +mkio_signal_test
      PREFIX mkio0-em2-func-a-read
    )
    
    
#    add_rumboot_target(
#        CONFIGURATION IRAM
#        FILES common/mkio/mkio0_events_timestamps_test.c
#        NAME mkio0_events_timestamps_test
#      )
#
#    add_rumboot_target(
#        CONFIGURATION IRAM
#        FILES common/mkio/mkio1_events_timestamps_test.c
#        NAME mkio1_events_timestamps_test
#      )

#    add_rumboot_target(
#        CONFIGURATION IRAM
#        FILES common/mkio/mkio_signal_test.c
#        NAME mkio_signal_test
#        IRUN_FLAGS +mkio_signal_test
#      )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES test_oi10_cpu_021.c
        NAME "test_oi10_cpu_021_wt_100"
        CFLAGS -DL2C_IL1I_BIT=1 -DL2C_IL1D_BIT=1 -DL2C_WRITE_MODE_BIT=1 -DL2C_INHIBIT_BIT=0 -DL2C_MEMORY_COHERENCE_BIT=1
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_021_base.c
        NAME "test_oi10_cpu_021_wt_100"
        LOAD IM0BIN SELF
             SRAM0BIN supplementary-test_oi10_cpu_021_wt_100
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES test_oi10_cpu_021.c
        NAME "test_oi10_cpu_021_wt_101"
        CFLAGS -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=1 -DL2C_WRITE_MODE_BIT=1 -DL2C_INHIBIT_BIT=0 -DL2C_MEMORY_COHERENCE_BIT=1
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_021_base.c
        NAME "test_oi10_cpu_021_wt_101"
        LOAD IM0BIN SELF
             SRAM0BIN supplementary-test_oi10_cpu_021_wt_101
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES test_oi10_cpu_021.c
        NAME "test_oi10_cpu_021_wt_110"
        CFLAGS -DL2C_IL1I_BIT=1 -DL2C_IL1D_BIT=0 -DL2C_WRITE_MODE_BIT=1 -DL2C_INHIBIT_BIT=0 -DL2C_MEMORY_COHERENCE_BIT=1
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_021_base.c
        NAME "test_oi10_cpu_021_wt_110"
        LOAD IM0BIN SELF
             SRAM0BIN supplementary-test_oi10_cpu_021_wt_110
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES test_oi10_cpu_021.c
        NAME "test_oi10_cpu_021_wt_111"
        CFLAGS -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=0 -DL2C_WRITE_MODE_BIT=1 -DL2C_INHIBIT_BIT=0 -DL2C_MEMORY_COHERENCE_BIT=1
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_021_base.c
        NAME "test_oi10_cpu_021_wt_111"
        LOAD IM0BIN SELF
             SRAM0BIN supplementary-test_oi10_cpu_021_wt_111
    )

   add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES test_oi10_cpu_021.c
        NAME "test_oi10_cpu_021_wb_000"
        CFLAGS -DL2C_IL1I_BIT=1 -DL2C_IL1D_BIT=1 -DL2C_WRITE_MODE_BIT=0 -DL2C_INHIBIT_BIT=1 -DL2C_MEMORY_COHERENCE_BIT=0
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_021_base.c
        NAME "test_oi10_cpu_021_wb_000"
        LOAD IM0BIN SELF
             SRAM0BIN supplementary-test_oi10_cpu_021_wb_000
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES test_oi10_cpu_021.c
        NAME "test_oi10_cpu_021_wb_100"
        CFLAGS -DL2C_IL1I_BIT=1 -DL2C_IL1D_BIT=1 -DL2C_WRITE_MODE_BIT=0 -DL2C_INHIBIT_BIT=0 -DL2C_MEMORY_COHERENCE_BIT=1
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_021_base.c
        NAME "test_oi10_cpu_021_wb_100"
        LOAD IM0BIN SELF
             SRAM0BIN supplementary-test_oi10_cpu_021_wb_100
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES test_oi10_cpu_021.c
        NAME "test_oi10_cpu_021_wb_101"
        CFLAGS -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=1 -DL2C_WRITE_MODE_BIT=0 -DL2C_INHIBIT_BIT=0 -DL2C_MEMORY_COHERENCE_BIT=1
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_021_base.c
        NAME "test_oi10_cpu_021_wb_101"
        LOAD IM0BIN SELF
             SRAM0BIN supplementary-test_oi10_cpu_021_wb_101
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES test_oi10_cpu_021.c
        NAME "test_oi10_cpu_021_wb_110"
        CFLAGS -DL2C_IL1I_BIT=1 -DL2C_IL1D_BIT=0 -DL2C_WRITE_MODE_BIT=0 -DL2C_INHIBIT_BIT=0 -DL2C_MEMORY_COHERENCE_BIT=1
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_021_base.c
        NAME "test_oi10_cpu_021_wb_110"
        LOAD IM0BIN SELF
             SRAM0BIN supplementary-test_oi10_cpu_021_wb_110
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES test_oi10_cpu_021.c
        NAME "test_oi10_cpu_021_wb_111"
        CFLAGS -DL2C_IL1I_BIT=0 -DL2C_IL1D_BIT=0 -DL2C_WRITE_MODE_BIT=0 -DL2C_INHIBIT_BIT=0 -DL2C_MEMORY_COHERENCE_BIT=1
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_021_base.c
        NAME "test_oi10_cpu_021_wb_111"
        LOAD IM0BIN SELF
             SRAM0BIN supplementary-test_oi10_cpu_021_wb_111
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
#        CFLAGS -DL2C_CHECK_SIZE=0x100
        LDS oi10/sram0.lds
        FILES test_oi10_cpu_021_wb_cache_size.c
        NAME "test_oi10_cpu_021_wb_cache_size"
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_021_wb_cache_size_base.c
        NAME "test_oi10_cpu_021_wb_cache_size"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_cpu_021_wb_cache_size
    )

#***********************************************************************************************************************************************************************************************************
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_first_exprience.c
        NAME "test_oi10_ppc_memory_bw_exploration_first_exprience"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_first_exprience"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_first_exprience
    )
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_dcbt_dcbf.c
        NAME "test_oi10_ppc_memory_bw_exploration_dcbt_dcbf"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_dcbt_dcbf"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_dcbt_dcbf
    )    
#****PPC_MEMORY_BW_EXPLORATION_RX_CASE*******************************************************************************************************************************************************************************************************
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_rx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DSRAM0_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram0"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram0"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram0
    )   
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_rx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DSRAM1_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram1"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram1"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram1
    )
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_rx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DSSRAM_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_ssram"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_ssram"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_ssram
    )    
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_rx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DIM0_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_im0"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_im0"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_im0
    )  
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_rx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DIM1_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_im1"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_im1"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_im1
    )       
#****PPC_MEMORY_BW_EXPLORATION_TX_CASE*******************************************************************************************************************************************************************************************************
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_tx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DSRAM0_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_sram0"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_sram0"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_sram0
    )   
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_tx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DSRAM1_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_sram1"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_sram1"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_sram1
    )
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_tx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DSSRAM_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_ssram"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_ssram"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_ssram
    )  
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_tx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DIM0_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_im0"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_im0"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_im0
    )  
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_tx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DIM1_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_im1"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_im1"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_im1
    )      
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_tx_case.c
        CFLAGS  -DNUM_OF_BUFS=64
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DSSRAM_CACHE=1
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_full_l2c_simple_crc_ssram"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_full_l2c_simple_crc_ssram"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_tx_case_full_l2c_simple_crc_ssram
    )  
    
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_l2c_im0_scrubber_apb.c
        NAME "test_o32t_l2cache_and_im0_scrubbers_apb"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_im0_double_error.c
        IRUN_FLAGS +enable_IM0_de_insertion
                +set_IM0_to_zero
        NAME "test_im0_double_error"
    )

#********** NACHINE_CHECK disable versions *****************************************************************************************************************************************************    
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_rx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DIM0_CACHE=1
                -DMC_DISABLE=1
                -DNUM_OF_ITERATION=1000
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_im0_mc_disable"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_im0_mc_disable"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_im0_mc_disable
    )  
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_rx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DSRAM0_CACHE=1
                -DMC_DISABLE=1
                -DNUM_OF_ITERATION=1000
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram0_mc_disable"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram0_mc_disable"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram0_mc_disable
    )
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_rx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DSRAM0_CACHE=1
                -DMC_DISABLE=1
                -DNUM_OF_ITERATION=1000
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram0_mc_disable_slow_emi"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        CFLAGS  -DSLOW_SRAM0_CFG=1
        NAME "test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram0_mc_disable_slow_emi"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_rx_case_10_4096_4_simple_crc_sram0_mc_disable_slow_emi
    )
    # -------
    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/sram0.lds
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_tx_case.c
        CFLAGS  -DNUM_OF_BUFS=10
                -DSIZE_OF_BUFS=4096
                -DREAD_SIZE=4
                -DSIMPLE_CRC=1
                -DIM0_CACHE=1
                -DMC_DISABLE=1
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_im0_mc_disable"
        OPTIMIZE -O3
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES ppc_memory_bw_exploration/test_oi10_ppc_memory_bw_exploration_loader.c
        NAME "test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_im0_mc_disable"
        TESTGROUP broken hwonly
        LOAD IM0BIN SELF
             MBIN supplementary-test_oi10_ppc_memory_bw_exploration_tx_case_10_4096_4_simple_crc_im0_mc_disable
    )  
    # -------
#***********************************************************************************************************************************************************************************************************
    
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

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_endianness.c
      NAME "test_oi10_endianness_soft"
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      CFLAGS -DENDIAN_HARD_CHECK
      FILES test_oi10_endianness.c
      NAME "test_oi10_endianness_soft_and_hard"
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      CFLAGS -DUSE_HARDWARE_PARTS
      FILES simple-iram/test_oi10_cpu_025_io.c
      PREFIX "simple-iram"
      NAME "test_oi10_cpu_025_io_hard"
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      CFLAGS -DUSE_HARDWARE_PARTS
      FILES simple-iram/test_oi10_cpu_025_l2c.c
      PREFIX "simple-iram"
      NAME "test_oi10_cpu_025_l2c_hard"
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      CFLAGS -DUSE_HARDWARE_PARTS
      FILES simple-iram/test_oi10_cpu_025_ppc.c
      PREFIX "simple-iram"
      NAME "test_oi10_cpu_025_ppc_hard"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS  -DTEST_OI10_CPU_021_CACHE_OP_LIST=test_ici
                -DL2C_TRACE_DEBUG_MSG
        FILES simple-iram/test_oi10_cpu_021_cache_op.c
        PREFIX "simple-iram"
        NAME test_oi10_cpu_021_cache_op_ici
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      CFLAGS -DUSE_HARDWARE_PART
      FILES simple-iram/test_oi10_cpu_019.c
      PREFIX "simple-iram"
      NAME "test_oi10_cpu_019_hard"
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_cpu_028.c
      IRUN_FLAGS +set_IM0_to_zero 
                 +disable_L2C_se_insertion
                 +disable_IM0_se_insertion
      NAME "test_oi10_cpu_028"
    )
    
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_sdio.c
        CFLAGS -DSDIO_BASE=SDIO_0_BASE  -DGSPI_SDIO_IRQ=GSPI0_INT -DSDIO_HEAP="IM1"
        IRUN_FLAGS +select_sdio0
        PREFIX sdio-spi-0-im1
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_sdio.c
        CFLAGS -DSDIO_BASE=SDIO_1_BASE  -DGSPI_SDIO_IRQ=GSPI1_INT -DSDIO_HEAP="IM1"
        IRUN_FLAGS +select_sdio1
        PREFIX sdio-spi-1-im1
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_sdio.c
        CFLAGS -DSDIO_BASE=SDIO_0_BASE  -DGSPI_SDIO_IRQ=GSPI0_INT -DSDIO_HEAP="IM2"
        IRUN_FLAGS +select_sdio0
        PREFIX sdio-spi-0-im2
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_sdio.c
        CFLAGS -DSDIO_BASE=SDIO_1_BASE  -DGSPI_SDIO_IRQ=GSPI1_INT -DSDIO_HEAP="IM2"
        IRUN_FLAGS +select_sdio1
        PREFIX sdio-spi-1-im2
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_sdio.c
        CFLAGS -DSDIO_BASE=SDIO_0_BASE  -DGSPI_SDIO_IRQ=GSPI0_INT -DSDIO_HEAP="SRAM0"
        IRUN_FLAGS +select_sdio0
        PREFIX sdio-spi-0-sram0
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_sdio.c
        CFLAGS -DSDIO_BASE=SDIO_1_BASE  -DGSPI_SDIO_IRQ=GSPI1_INT -DSDIO_HEAP="SRAM0"
        IRUN_FLAGS +select_sdio1
        PREFIX sdio-spi-1-sram0
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_ctrl_002_2.c
      IRUN_FLAGS +BOOT_PLL_BYP=1 +TEST_OI10_CTRL_002
      LOAD IM0BIN SELF,SELF
      PREFIX crg
      TESTGROUP broken
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_ctrl_002_3.c
      IRUN_FLAGS +TEST_OI10_CTRL_002
      LOAD IM0BIN SELF,SELF,SELF,SELF,SELF
      PREFIX crg
      TESTGROUP broken
      )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_ctrl_002_4.c
      IRUN_FLAGS +TEST_OI10_CTRL_002
      LOAD IM0BIN SELF
      TESTGROUP broken
      PREFIX crg
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_sys_010.lds
        FILES test_oi10_sys_010_func.c
        NAME "test_oi10_sys_010_func"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_sys_010.c
        PREFIX simple-iram
        NAME "test_oi10_sys_010"
        LOAD IM0BIN SELF
             SBIN supplementary-test_oi10_sys_010_func
    )

    add_rumboot_target(
        CONFIGURATION SUPPLEMENTARY
        LDS oi10/test_oi10_cpu_027_sram0.lds
        FILES test_oi10_cpu_027_test.c
        NAME "test_oi10_cpu_027_test"
    )
    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_027.c
        CFLAGS -DTEST_OI10_CPU_027
        NAME "test_oi10_cpu_027"
        LOAD IM0BIN SELF
             SRAM0BIN supplementary-test_oi10_cpu_027_test
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_cpu_mem_022.c test_oi10_cpu_mem_022.S
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      CFLAGS -DBOOT_NOR
      FILES simple-iram/test_oi10_em2_105.c
      PREFIX simple-iram
      NAME "test_oi10_em2_105_bootnor"
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      CFLAGS -DBOOT_NOR
      FILES simple-iram/test_oi10_em2_204.c
      PREFIX simple-iram
      NAME "test_oi10_em2_204_bootnor"
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_cpu_025_wd.c
      LOAD IM0BIN SELF,SELF,SELF,SELF
      TESTGROUP broken
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_cpu_032.c
      LOAD IM0BIN SELF,SELF
      TESTGROUP broken
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES test_oi10_cpu_025_rst.c
        LOAD IM0BIN SELF,SELF,SELF,SELF
        TESTGROUP broken
    )

     add_rumboot_target(
         CONFIGURATION IRAM
         IRUN_FLAGS +TEST_OI10_CPU_025_MPIC
         FILES test_oi10_cpu_025_mpic.c
         LOAD IM0BIN SELF,SELF,SELF,SELF,SELF,SELF,SELF
     )

    add_rumboot_target(
      CONFIGURATION IRAM_SPL
      FILES common/updaters/spiflash-pl022.c
      CFLAGS -DBOOT_ID=2
      PREFIX updater
      TESTGROUP hwonly
      NAME spi-flash-0
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES common/updaters/nor.c
      PREFIX updater
      TESTGROUP hwonly
      NAME nor-mt150.04
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES common/updaters/nor.c
      CFLAGS -DNOR_OFFSET=134152192
      PREFIX updater
      TESTGROUP hwonly
      NAME nor-mt150.04-brom
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS
            -DHSCB_SRC_BASE=HSCB2_BASE
            -DHSCB_DST_BASE=HSCB3_BASE
            -DLOOP_TEST
        FILES hscb_loop.c
        PREFIX "hscb_2_3"
        NAME "loop_test"
        TESTGROUP hwonly
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS
            -DHSCB_SRC_BASE=HSCB0_BASE
            -DHSCB_DST_BASE=HSCB1_BASE
            -DLOOP_TEST
        FILES hscb_loop.c
        PREFIX "hscb_0_1"
        NAME "loop_test"
        TESTGROUP hwonly
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS
            -DHSCB_BASE=HSCB0_BASE
            -DINTERNAL_LOOP_TEST
        FILES hscb_loop.c
        PREFIX "hscb_0"
        NAME "internal_loop_test"
        TESTGROUP hwonly
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS
            -DHSCB_BASE=HSCB0_BASE
            -DBOARD_TEST
#            -DON_BOARD
        FILES hscb_loop.c
        PREFIX "hscb_0"
        NAME "board_test"
        TESTGROUP hwonly
    )

    add_rumboot_target(
      FEATURES NOCODE
      COMBOIMAGE IM0BIN
       LOAD IM0BIN ${SPELS_TEST_CHAIN},simple-iram-oi10_spels_cache_hack
      PREFIX spels
      TESTGROUP chains hwonly
      NAME test-chain
    )

    add_rumboot_target(
      FEATURES NOCODE
      COMBOIMAGE IM0BIN
       LOAD IM0BIN hscb_2_3-loop_test,hscb_0_1-loop_test,simple-iram-oi10_spels_cache_hack,greth0-rx_col-test_oi10_greth,greth1-rx_col-test_oi10_greth,greth0-edcl-im2-test_oi10_greth,greth0-edcl-im1-test_oi10_greth,greth1-edcl-im2-test_oi10_greth,greth1-edcl-im1-test_oi10_greth,greth0-im2-im2-test_oi10_greth,greth0-im1-im1-test_oi10_greth,greth1-im2-im2-test_oi10_greth,greth1-im1-im1-test_oi10_greth,mkio0-regs-test_oi10_lscb,mkio1-regs-test_oi10_lscb,mkio0-im1-func-a-test_oi10_lscb,mkio1-im1-func-a-test_oi10_lscb,mkio0-im1-func-b-test_oi10_lscb,mkio1-im1-func-b-test_oi10_lscb,mkio0-im2-func-a-test_oi10_lscb,mkio1-im2-func-a-test_oi10_lscb,mkio0-im2-func-b-test_oi10_lscb,mkio1-im2-func-b-test_oi10_lscb,simple-iram-test_oi10_cpu_007,simple-iram-test_oi10_cpu_003,iss-iram-test_oi10_cpu_004,iss-iram-test_oi10_cpu_005,simple-iram-test_oi10_cpu_019,${SPELS_TEST_CHAIN},simple-iram-oi10_spels_cache_hack
      PREFIX spels
      TESTGROUP chains hwonly
      NAME functional-test-chain
    )


    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_plb6_axi.c
      CFLAGS -DCHECK_PLB6_AXI_SINGLE
      PREFIX plb6-axi-single
      TESTGROUP broken
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_plb6_axi.c
      CFLAGS -DCHECK_PLB6_AXI_BURST
      PREFIX plb6-axi-burst
      TESTGROUP broken hwonly
      )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_plb6_axi.c
      CFLAGS -DCHECK_AXI_PLB6_SINGLE
      PREFIX axi-plb6-single
      TESTGROUP hwonly
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_plb6_axi.c
      CFLAGS -DCHECK_AXI_PLB6_BURST
      PREFIX axi-plb6-burst
      TESTGROUP hwonly
      )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_mt150_08_sdram.c
      TESTGROUP hwonly
      NAME test_oi10_mt150_08_sdram
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_mt150_07_sram.c
      TESTGROUP hwonly
      NAME test_oi10_mt150_07_sram
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_mt150_06_ssram.c
      TESTGROUP hwonly
      NAME test_oi10_mt150_06_ssram
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES power_hscb.c
      TESTGROUP hwonly
      NAME power_hscb
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES power_dma_and_hscb.c
      TESTGROUP hwonly
      NAME power_dma_and_hscb
      CFLAGS -DPOWER_TIMEOUT=5000000UL
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES power_dma_and_hscb_and_ext_clk.c
      TESTGROUP hwonly
      NAME power_dma_and_hscb_and_ext_clk
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES power_dma_and_hscb_and_gpio_finish.c
      TESTGROUP hwonly
      NAME power_dma_and_hscb_and_gpio_finish
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_progr_crg.c
      TESTGROUP hwonly
      NAME test_oi10_progr_crg
    )

    set(FK_TEST_CHAIN hscb_0_1-loop_test,hscb_2_3-loop_test,greth0-edcl-im1-test_oi10_greth,greth1-edcl-im1-test_oi10_greth,mkio0-im1-func-a-test_oi10_lscb,mkio0-im1-func-b-test_oi10_lscb,mkio1-im1-func-a-test_oi10_lscb,mkio1-im1-func-b-test_oi10_lscb,spels-memory-test-im0,spels-memory-test-im1,simple-iram-test_oi10_cpu_007,simple-iram-test_oi10_cpu_019,irq-iram-irq-atomics,irq-iram-irq-defhandler,spels-math-test,simple-iram-oi10_spels_cache_hack)
    add_rumboot_target(
      FEATURES NOCODE
      COMBOIMAGE IM0BIN
      LOAD IM0BIN simple-iram-chain-start-dummy,${FK_TEST_CHAIN},simple-iram-chain-end-dummy
      TESTGROUP chains hwonly
      NAME fk-test-chain
    )

#ERROR - видимо какой-то из тестов как-то хитро бьет стек и в зависимости от порядка тестов цепочка или падает или проходит    set(FK_TEST_CHAIN_WITHOUT_MKIO hscb_0_1-loop_test,hscb_2_3-loop_test,greth0-edcl-im1-test_oi10_greth,greth1-edcl-im1-test_oi10_greth,spels-memory-test-im0,spels-memory-test-im1,simple-iram-test_oi10_cpu_007,simple-iram-test_oi10_cpu_019,irq-iram-irq-atomics,irq-iram-irq-defhandler,spels-math-test,simple-iram-oi10_spels_cache_hack)
    set(FK_TEST_CHAIN_WITHOUT_MKIO hscb_0_1-loop_test,hscb_2_3-loop_test,greth0-edcl-im1-test_oi10_greth,greth1-edcl-im1-test_oi10_greth,spels-memory-test-im0,spels-memory-test-im1,simple-iram-test_oi10_cpu_007,simple-iram-test_oi10_cpu_019,irq-iram-irq-defhandler,irq-iram-irq-atomics,spels-math-test,simple-iram-oi10_spels_cache_hack)
    add_rumboot_target(
      FEATURES NOCODE
      COMBOIMAGE IM0BIN
      LOAD IM0BIN simple-iram-chain-start-dummy,${FK_TEST_CHAIN_WITHOUT_MKIO},simple-iram-chain-end-dummy
      TESTGROUP chains hwonly
      NAME fk-test-chain_without_mkio
    )
        
    add_rumboot_target(
      FEATURES NOCODE
      COMBOIMAGE IM0BIN
      LOAD IM0BIN ${FK_TEST_CHAIN_WITHOUT_MKIO},simple-iram-chain-end-dummy
      TESTGROUP chains hwonly
      NAME fk-test-chain_tmp
    )
    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES power/loader.c
      CFLAGS -DENABLE_CACHE -DCHAINLOAD_FROM_FLASH
      NAME "flash-im0-loader"
      PREFIX power
      FEATURES STUB
  )

    add_rumboot_target(
      CONFIGURATION IRAM
      LDS oi10/iram_legacy_cached.lds
      FILES power/fpu_power_test_endless.S power/power_endless.c
      NAME "cached-test_endless"
      PREFIX power
      LOAD IM0BIN SELF
      TESTGROUP hwonly
    )


  add_rumboot_target(
    CONFIGURATION IRAM_SPL
    FILES tlbdump/main.c tlbdump/ops.S
    PREFIX "tlbdumper"
    NAME "after-rom"
    TESTGROUP hwonly
  )

  add_rumboot_target(
    CONFIGURATION IRAM
    FILES tlbdump/main.c tlbdump/ops.S
    PREFIX "tlbdumper"
    NAME "with-rumboot-init"
    TESTGROUP hwonly
  )


  add_rumboot_target(
    CONFIGURATION IRAM
    FILES ../archive/test_oi10_cpu_021_dcbtl.c
    PREFIX archive-iram
    TESTGROUP broken hwonly
    )

    add_rumboot_target(
      CONFIGURATION SUPPLEMENTARY
#        CFLAGS -DL2C_CHECK_SIZE=0x100
      LDS oi10/sram0.lds
      PREFIX l2cbug-supp
      FILES l2bug/test_oi10_cpu_021_wb_cache_size.c
      NAME "test_oi10_cpu_021_wb_cache_size"
      TESTGROUP broken hwonly
    )

    add_rumboot_target(
      CONFIGURATION IRAM
      FILES l2bug/test_oi10_cpu_021_wb_cache_size_base.c
      NAME "test_oi10_cpu_021_wb_cache_size"
      PREFIX l2cbug
      TESTGROUP broken hwonly
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD IM0BIN SELF
           MBIN l2cbug-supp-test_oi10_cpu_021_wb_cache_size
  )

  add_rumboot_target(
    CONFIGURATION SUPPLEMENTARY
    PREFIX l2bug
    LDS oi10/l2bug_supp.lds
    CFLAGS -DRUMBOOT_NOINIT
    LDFLAGS -Wl,-emain
    FILES l2bug/multistore.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
    NAME "multistore-supp"
  )

  add_rumboot_target(
    CONFIGURATION IRAM_SPL
    PREFIX tool
    FILES common/tools/trampoline.c
    PACKIMAGE_FLAGS -C -a 1024
    TESTGROUP hwonly
  )

  add_rumboot_target(
    CONFIGURATION IRAM_SPL
    PREFIX l2bug
    FILES l2bug/multistore-loader.c
    NAME "multistore"
    PREFIX l2bug
    CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
    IRUN_FLAGS ${ROM_6500K_OPTS}
    LOAD IM0BIN SELF
         MBIN l2bug-multistore-supp
  )

  add_rumboot_target(
    CONFIGURATION SUPPLEMENTARY
    PREFIX l2bug
    LDS oi10/l2bug_supp-2.lds
    CFLAGS -DRUMBOOT_NOINIT
    LDFLAGS -Wl,-emain
    TESTGROUP hwonly
    FILES l2bug/multistore.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
    NAME "multistore-supp-2"
  )

  add_rumboot_target(
    CONFIGURATION SUPPLEMENTARY
    PREFIX l2bug
    LDS oi10/l2bug_supp-2.lds
    CFLAGS -DRUMBOOT_NOINIT
    LDFLAGS -Wl,-emain
    TESTGROUP hwonly
    FILES l2bug/multistore-msync.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
    NAME "multistore-msync-supp-3"
  )

  add_rumboot_target(
    CONFIGURATION IRAM_SPL
    PREFIX l2bug
    FILES l2bug/multistore-loader-2.c
    NAME "multistore-2"
    TESTGROUP hwonly
    PREFIX l2bug
    CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
#    CFLAGS -DSDRAM_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
    IRUN_FLAGS ${ROM_6500K_OPTS}
    LOAD IM0BIN SELF
        MBIN l2bug-multistore-supp
  )

  add_rumboot_target(
    CONFIGURATION IRAM_SPL
    PREFIX l2bug
    FILES l2bug/multistore-loader-3.c
    TESTGROUP hwonly
    NAME "multistore-3"
    PREFIX l2bug
    CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
#    CFLAGS -DSDRAM_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
    IRUN_FLAGS ${ROM_6500K_OPTS}
    LOAD IM0BIN SELF
        MBIN l2bug-multistore-supp
  )

  add_rumboot_target(
    CONFIGURATION IRAM_SPL
    PREFIX l2bug
    FILES l2bug/multistore-loader-3.c
    TESTGROUP hwonly
    NAME "multistore-msync-3"
    PREFIX l2bug
    CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
#    CFLAGS -DSDRAM_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
    IRUN_FLAGS ${ROM_6500K_OPTS}
    LOAD IM0BIN SELF
        MBIN l2bug-multistore-msync-supp-3
  )

   add_rumboot_target(
    CONFIGURATION IRAM_SPL
    PREFIX l2bug
    FILES l2bug/l1_error_insertion.c
    NAME "l1_error_insertion"
    TESTGROUP hwonly
    CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
    IRUN_FLAGS ${ROM_6500K_OPTS}
  )

      add_rumboot_target(
      FEATURES NOCODE
      COMBOIMAGE IM0BIN
      LOAD IM0BIN simple-iram-chain-start-dummy,iram-power_dma_and_hscb,${FK_TEST_CHAIN},power-flash-im0-loader,power-cached-test_endless
      TESTGROUP chains hwonly
      NAME endless_power_test
      PREFIX chain
    )

    
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES scrb.c
      IRUN_FLAGS +insert_error_in_im1_im2_mem
      TESTGROUP oi10only
    )
#*****************************************************************
    add_rumboot_target(
      CONFIGURATION SUPPLEMENTARY
      PREFIX l2bug
      LDS oi10/l2bug_sram_test.lds
      CFLAGS -DRUMBOOT_NOINIT
      LDFLAGS -Wl,-emain
      FILES l2bug/sram-test.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c l2bug/memcpy_copyed.S
      NAME "sram-test"
    )

    add_rumboot_target(
      CONFIGURATION IRAM_SPL
      PREFIX l2bug
      LDS oi10/iram_legacy.lds
      FILES l2bug/sram-test-loader.c
      NAME "sram-test-loader"
      PREFIX l2bug
      CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD IM0BIN SELF
          MBIN l2bug-sram-test
    )

 add_rumboot_target(
      CONFIGURATION SUPPLEMENTARY
      PREFIX oi10new
      LDS oi10/oi10new_memcopy_write-through.lds
      CFLAGS -DRUMBOOT_NOINIT
      LDFLAGS -Wl,-emain
      FILES oi10new/memcopy.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c oi10new/memcpy_copyed.S
      NAME "memcopy"
    )

    add_rumboot_target(
      CONFIGURATION IRAM_SPL
      PREFIX oi10new
      LDS oi10/iram_legacy.lds
      FILES oi10new/memcopy-spl.c
      NAME "memcopy-spl"
      CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD IM0BIN SELF
          MBIN oi10new-memcopy
    )    

    add_rumboot_target(
      CONFIGURATION SUPPLEMENTARY
      PREFIX oi10new
      LDS oi10/oi10new_dcache.lds
      CFLAGS -DRUMBOOT_NOINIT
      LDFLAGS -Wl,-emain
      FILES oi10new/dcache.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c oi10new/memcpy_copyed.S
      NAME "dcache"
    )

    add_rumboot_target(
      CONFIGURATION IRAM_SPL
      PREFIX oi10new
      LDS oi10/iram_legacy.lds
      FILES oi10new/dcache-spl.c
      NAME "dcache-spl"
      CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD IM0BIN SELF
          MBIN oi10new-dcache
    )

    add_rumboot_target(
      CONFIGURATION SUPPLEMENTARY
      PREFIX oi10new
      LDS oi10/oi10new_memcopy_write-back.lds
      CFLAGS -DRUMBOOT_NOINIT
      LDFLAGS -Wl,-emain
      FILES oi10new/memcopy-wb.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c oi10new/memcpy_copyed.S oi10new/crc.c
      NAME "memcopy-wb"
    )

    add_rumboot_target(
    CONFIGURATION IRAM_SPL
    PREFIX oi10new
    LDS oi10/iram_legacy.lds
    FILES oi10new/memcopy-spl.c
    NAME "memcopy-wb-spl"
    CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
    IRUN_FLAGS ${ROM_6500K_OPTS}
    LOAD IM0BIN SELF
        MBIN oi10new-memcopy-wb
    )    
#***********************************************************************************************************************************************************************************************************
    add_rumboot_target(
      CONFIGURATION SUPPLEMENTARY
      PREFIX l2bug
      LDS oi10/l2bug_sram_test.lds
      CFLAGS -DRUMBOOT_NOINIT
      LDFLAGS -Wl,-emain
      FILES l2bug/sram-test-gc.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c l2bug/memcpy_copyed.S
      NAME "sram-test-gc"
    )

    add_rumboot_target(
      CONFIGURATION IRAM_SPL
      PREFIX l2bug
      LDS oi10/iram_legacy.lds
      FILES l2bug/sram-test-loader.c
      NAME "sram-test-loader-gc"
      PREFIX l2bug
      CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD IM0BIN SELF
          MBIN l2bug-sram-test-gc
    )
    #----------
        add_rumboot_target(
      CONFIGURATION SUPPLEMENTARY
      PREFIX l2bug
      LDS oi10/l2bug_sram_test.lds
      CFLAGS -DRUMBOOT_NOINIT -DONE_MEMCPY
      LDFLAGS -Wl,-emain
      FILES l2bug/sram-test-gc.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c l2bug/memcpy_copyed.S
      NAME "sram-test-one-memcpy-gc"
    )

    add_rumboot_target(
      CONFIGURATION IRAM_SPL
      PREFIX l2bug
      LDS oi10/iram_legacy.lds
      FILES l2bug/sram-test-loader.c
      NAME "sram-test-loader-one-memcpy-gc"
      PREFIX l2bug
      CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD IM0BIN SELF
          MBIN l2bug-sram-test-one-memcpy-gc
    )
    #----------
    add_rumboot_target(
      CONFIGURATION SUPPLEMENTARY
      PREFIX l2bug
      LDS oi10/l2bug_sram_test.lds
      CFLAGS -DRUMBOOT_NOINIT -DMSYNC1
      LDFLAGS -Wl,-emain
      FILES l2bug/sram-test-gc.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c l2bug/memcpy_copyed.S
      NAME "sram-test-msync-gc"
    )

    add_rumboot_target(
      CONFIGURATION IRAM_SPL
      PREFIX l2bug
      LDS oi10/iram_legacy.lds
      FILES l2bug/sram-test-loader.c
      NAME "sram-test-loader-msync-gc"
      PREFIX l2bug
      CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD IM0BIN SELF
          MBIN l2bug-sram-test-msync-gc
    )
  
    
    add_rumboot_target(
      CONFIGURATION SUPPLEMENTARY
      PREFIX oi10new
      LDS oi10/oi10new_dcache.lds
      CFLAGS -DRUMBOOT_NOINIT
      LDFLAGS -Wl,-emain
      FILES oi10new/dcache-gc.c ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c oi10new/memcpy_copyed.S
      NAME "dcache-gc"
    )

    add_rumboot_target(
      CONFIGURATION IRAM_SPL
      PREFIX oi10new
      LDS oi10/iram_legacy.lds
      FILES oi10new/dcache-spl.c
      NAME "dcache-spl-gc"
      CFLAGS -DEMI_INIT -DADD_TLB -DM_BASE=SRAM0_BASE
      IRUN_FLAGS ${ROM_6500K_OPTS}
      LOAD IM0BIN SELF
          MBIN oi10new-dcache-gc
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
