SET(RUMBOOT_ARCH basis)
SET(RUMBOOT_PLATFORM basis)

set(RUMBOOT_PLATFORM_TARGET_DIR ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/)

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
  FEATURES LUA
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


macro(add_directory_with_targets dir)
  file(GLOB RUMBOOT_TARGETS_C ${RUMBOOT_PLATFORM_TARGET_DIR}/${dir}/*.c)
  file(GLOB RUMBOOT_TARGETS_S ${RUMBOOT_PLATFORM_TARGET_DIR}/${dir}/*.S)
  file(GLOB RUMBOOT_TARGETS_LUA ${RUMBOOT_PLATFORM_TARGET_DIR}/${dir}/*.lua)
  foreach(target ${RUMBOOT_TARGETS_C} ${RUMBOOT_TARGETS_S} ${RUMBOOT_TARGETS_LUA})
    add_rumboot_target(
        ${ARGN}
        FILES ${target}
    )
  endforeach()

endmacro()

### Add tests here ###
#WARNING! Full regression automatically includes all tests from the short ones
macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  add_directory_with_targets(simple-rom/
    CONFIGURATION ROM
    PREFIX simple-rom)

  add_directory_with_targets(simple-iram/
      CONFIGURATION IRAM
      PREFIX simple-iram)

  add_directory_with_targets(jenkins/
      CONFIGURATION ROM
      PREFIX jenkins
      TESTGROUP short
    )

    add_directory_with_targets(irq/
      CONFIGURATION ROM
      PREFIX irq-rom
      TESTGROUP short
    )

    add_directory_with_targets(irq/
      CONFIGURATION IRAM
      PREFIX irq-iram
      TESTGROUP short
    )

    add_directory_with_targets( lua/
      CONFIGURATION IRAM
      PREFIX lua
    )

    add_directory_with_targets(lua/
      CONFIGURATION LPROBE_CPU
      PREFIX lprobe-cpu
    )

    add_directory_with_targets(lua/
      CONFIGURATION LPROBE_PCIE
      PREFIX lprobe-pcie
    )

    add_directory_with_targets(spl-stubs/
      CONFIGURATION IRAM
      PREFIX spl
      FEATURES STUB
    )

    add_rumboot_target(
        CONFIGURATION ROM
        FILES bootrom-stub.c
        PREFIX "bootrom"
        NAME "stub"
        FEATURES STUB
    )

  add_rumboot_target(
          CONFIGURATION ROM
          FILES bootrom-lprobe-stub.c
          PREFIX "bootrom"
          NAME "lprobe-stub"
          FEATURES STUB
    )

    add_rumboot_target(
            CONFIGURATION ROM
            FILES bootrom-noop-stub.c
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
    FILES can-adapter/can_adapter_0_test.c
    TIMEOUT 300 us
    IRUN_FLAGS +can_plus_adapter
  )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION IRAM
      FILES simple-iram/pcie_dma_eRP_test.c
      NAME pcie_dma_eRP_test
      CFLAGS -Dincrease_test_duration=1
      IRUN_FLAGS +pcie_line_interference
    )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION ROM
      FILES pcie/pcie_at_slv_eRP_test_0.c
      NAME pcie_at_slv_eRP_test_0
      CFLAGS -Dshort_test_for_sim=1
    )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION ROM
      FILES pcie/pcie_at_typical_eRP_test_0.c
      NAME pcie_at_typical_eRP_test_0
      CFLAGS -Dshort_test_for_sim=1
    )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION ROM
      FILES pcie/pcie_at_mst_eRP_test_0.c
      NAME pcie_at_mst_eRP_test_0
      CFLAGS -Dshort_test_for_sim=1
    )

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION ROM
      FILES pcie/pcie_legacy_eRP_test.c
      NAME pcie_legacy_eRP_test
      IRUN_FLAGS +pcie_legacy_int_elab
    )

if (CMAKE_BUILD_TYPE MATCHES "RTL")
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION ROM
      FILES pcie/pcie_mbist_test.c
      NAME pcie_mbist_test
      IRUN_FLAGS +pcie_mbist_model
    )
else()
  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION ROM
      FILES pcie/pcie_mbist_test.c
      NAME pcie_mbist_test
    )
endif()

  add_rumboot_target(
      SNAPSHOT default
      CONFIGURATION ROM
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
    ${CMAKE_SOURCE_DIR}/src/lib/eventsystem-memory.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/ddr_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/pcie_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/dma_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/dit_lib.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-mcpu=cortex-a5 -mfpu=vfpv3-d16 -mfloat-abi=hard -marm -ffreestanding -Os -Werror -Wno-error=cpp")
    SET(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -Wall -fdata-sections -ffunction-sections")
    SET(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    SET(CMAKE_OBJCOPY_FLAGS --gap-fill 0x00 --pad-to 32768)
    SET(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -static -Wl,--gc-sections")
    SET(CMAKE_DUMP_FLAGS     "-EL")
endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)

endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)
  if (TARGET_CONFIGURATION STREQUAL "ROM")
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
