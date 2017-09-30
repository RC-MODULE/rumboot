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
  CFLAGS -DRUMBOOT_ONLY_STACK -DRUMBOOT_PRINTF_ACCEL
  PREFIX ROM
)

#These are configurations for im0 binaries
rumboot_add_configuration(
  IRAM
  LDS basis/iram.lds
  CFLAGS -DRUMBOOT_ONLY_STACK
  LDFLAGS -e main -Ttext 0x00040018
  FEATURES LUA
)

macro(add_directory_with_targets dir)
  file(GLOB RUMBOOT_TARGETS ${RUMBOOT_PLATFORM_TARGET_DIR}/${dir}/*.c)
  foreach(target ${RUMBOOT_TARGETS})
    add_rumboot_target(
        ${ARGN}
        FILES ${target}
    )
  endforeach()
  file(GLOB RUMBOOT_TARGETS ${RUMBOOT_PLATFORM_TARGET_DIR}/${dir}/*.S)
  foreach(target ${RUMBOOT_TARGETS})
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

  add_directory_with_targets(jenkins/
      CONFIGURATION ROM
      PREFIX jenkins
      TESTGROUP short
    )

  add_rumboot_target(
      SNAPSHOT default
      LDS basis/rom.lds
      FILES simple-rom/pcie_dma_eRP_test.c
      NAME pcie_dma_eRP_test
      CFLAGS -Dincrease_test_duration=1
      IRUN_FLAGS +pcie_line_interference
    )

  add_rumboot_target(
      SNAPSHOT default
      LDS basis/rom.lds
      FILES pcie/pcie_at_slv_eRP_test_0.c
      NAME pcie_at_slv_eRP_test_0
      CFLAGS -Dshort_test_for_sim=1
    )

  add_rumboot_target(
      SNAPSHOT default
      LDS basis/rom.lds
      FILES pcie/pcie_legacy_eRP_test.c
      NAME pcie_legacy_eRP_test
      IRUN_FLAGS +pcie_legacy_int_elab
    )

  add_rumboot_target(
      SNAPSHOT default
      LDS basis/rom.lds
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
        CONFIGURATION IRAM
        FILES iram/hello-iram.c
        PREFIX iram
        NAME hello
      )

endmacro()

if (CMAKE_VERILOG_RULES_LOADED)
    return()
endif()

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-gic.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/ddr_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/pcie_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/dma_test_lib.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/dit_lib.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-mcpu=cortex-a5 -marm -ffreestanding -Os -Werror -Wno-error=cpp")
    SET(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -Wall -fdata-sections -ffunction-sections")
    SET(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    SET(CMAKE_OBJCOPY_FLAGS --gap-fill 0xFF --pad-to 0x40000)
    SET(CMAKE_EXE_LINKER_FLAGS "-e rumboot_reset_handler -nostartfiles -static -Wl,--gc-sections")
    SET(CMAKE_DUMP_FLAGS     "-EL")
endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)

endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)
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

  install(DIRECTORY ${CMAKE_BINARY_DIR}/${product}.rcf DESTINATION rumboot/rcf)
endmacro()


if (NOT CROSS_COMPILE)
  SET(CROSS_COMPILE arm-rcm-eabihf)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
  if (EXISTS /opt/r42/toolchains/arm-rcm-eabihf/bin/arm-rcm-eabihf-gcc)
    set(CROSS_COMPILE "/opt/r42/toolchains/arm-rcm-eabihf/bin/${CROSS_COMPILE}")
  endif()
endif()

if (NOT (${CROSS_COMPILE} MATCHES "arm-rcm-eabihf"))
  message(FATAL_ERROR "This platform should be built with arm-rcm-eabihf toolchain")
endif()

set(CMAKE_C_COMPILER_WORKS 1)
