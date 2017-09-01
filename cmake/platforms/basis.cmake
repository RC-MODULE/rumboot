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
  CFLAGS -DRUMBOOT_ONLY_STACK
  PREFIX ROM
)

#These are configurations for im0 binaries
rumboot_add_configuration(
  SECONDARY
  LDS basis/rom.lds
  #TODO
  CFLAGS -DRUMBOOT_ONLY_STACK
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
      PREFIX simple-rom
      TESTGROUP short
    )

  add_rumboot_target(
      CONFIGURATION ROM
      FILES can/can-loopback.c can/loopback.S
      NAME can-loopback
    )
endmacro()

if (CMAKE_VERILOG_RULES_LOADED)
    return()
endif()

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/*.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-mcpu=cortex-a5 -marm -ffreestanding -Os -Werror")
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
