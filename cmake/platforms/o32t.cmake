SET(RUMBOOT_ARCH ppc)
set(RUMBOOT_SUPPORTS_SPL_ENDIAN_SWAP Yes)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)

if (NOT RUMBOOT_SOC_BUILD_TYPE STREQUAL "RTL")
  set(IRUN_BOOTM_EXTRA_ARGS +BOOT_NOR=0)
else()
  set(IRUN_BOOTM_EXTRA_ARGS )
endif()


file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/arch/ppc/exception.c
    ${CMAKE_SOURCE_DIR}/src/arch/ppc/p64.S    
    ${CMAKE_SOURCE_DIR}/src/arch/ppc/vectors.S
    ${CMAKE_SOURCE_DIR}/src/arch/ppc/ppc_mmu_impl.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/ndma_simple.c
	 ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/com_simple.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-mpic128.c)

#Flags for Power PC
macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    set(RUMBOOT_COMMON_FLAGS "-mcpu=476fp -fno-plt -fno-pic -m32 -ffreestanding -std=gnu99 -DRUMBOOT_PLATFORM_NUM_HEAPS=9 ")
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
    LDS oi10/bootrom.lds
    IRUN_FLAGS +BOOTMGR_KEEP_DRIVING=1 ${BOOTROM_IFLAGS}
)

rumboot_add_configuration (
    IRAM_IM1
    LDS oi10/iram.lds
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
    LDS oi10/iram_legacy.lds
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

  add_rumboot_target_dir(simple-iram/
    CONFIGURATION IRAM_IM1
    PREFIX iram-im1
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
        FILES test_oi10_lscb_polarity.c
        CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO0_BASE -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
        PREFIX mkio0-im1-a
    )


    add_rumboot_target(
        CONFIGURATION IRAM_IM1
        FILES test_oi10_lscb_1553_functionality.c
        CFLAGS -DPLATFORM_O32T -DMKIO_BASE=MKIO0_BASE -DMKIO_BUS=MKIO_BUS_A -DTEST_BANK="IM1" -D__FILENAME__=\"wtfisthisiwonder\"
        PREFIX mkio0-im1-a
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
