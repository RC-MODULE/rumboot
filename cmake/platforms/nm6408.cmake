SET(RUMBOOT_ARCH arm)
SET(RUMBOOT_PLATFORM nm6408)

set(RUMBOOT_PLATFORM_DEFAULT_LDS nm6408/rom.lds)
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
  LDS nm6408/rom.lds
  FILES ../header.c
  LDFLAGS "-e rumboot_reset_handler"
  OBJCOPY_FLAGS 
        --change-section-address .ro=0x8 
        --change-section-address .header=0x0 
        --change-section-address .trailer=16380
  CFLAGS  -marm
  PREFIX ROM
  TIMEOUT_CTEST 200000
  TIMEOUT 1500 ms
  IRUN_FLAGS ${BOOTROM_IFLAGS}
  FEATURES PACKIMAGE
)

# This configuration is for rom unit-tests
rumboot_add_configuration(
  UROM
  CONFIGURATION ROM
  CFLAGS -marm -DRUMBOOT_MAIN_NORETURN
)

rumboot_add_configuration (
  IRAM
  LDS nm6408/iram.lds
  PREFIX iram
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e rumboot_main"
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  CFLAGS -marm
  BOOTROM bootrom-stub
  TIMEOUT_CTEST 200000
  FEATURES COVERAGE PACKIMAGE
  LOAD IM0BIN SELF
)


include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/spels.cmake)

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

  rumboot_bootrom_add_components(
    IRAM ROM
    -a 512 -z 512)

    add_rumboot_target(
        CONFIGURATION IRAM
        FILES hello.c
      )

endmacro()

if (CMAKE_VERILOG_RULES_LOADED)
    return()
endif()

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/arch/arm/startup.S
    ${CMAKE_SOURCE_DIR}/src/arch/arm/exception.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*glue.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-gic.c
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
      COMMAND ${CMAKE_BINARY_DIR}/utils/romgen -l nm6408bootemi -i ${product}.bin -o ${product}.hex
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
