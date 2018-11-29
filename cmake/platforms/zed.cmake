SET(RUMBOOT_ARCH arm)
SET(RUMBOOT_PLATFORM zed)

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
  RAM
  DEFAULT
  SNAPSHOT default
  LDS zed/app.lds
  LDFLAGS "-e rumboot_reset_handler"
  IRUN_FLAGS ${BOOTROM_IFLAGS}
)

include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

### Add tests here ###
#WARNING! Full regression automatically includes all tests from the short ones
macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  add_rumboot_target_dir(apps/
      CONFIGURATION RAM
      PREFIX app)

  add_rumboot_target(
        FILES common/irq/irq-atomics.c
        CONFIGURATION RAM
        PREFIX app
      )
endmacro()

if (CMAKE_VERILOG_RULES_LOADED)
    return()
endif()

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-gic.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-mcpu=cortex-a5 -mapcs-frame -marm -ffreestanding -Wno-error=cpp")
    SET(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -Wall -fdata-sections -ffunction-sections -DRUMBOOT_PLATFORM_NUM_HEAPS=8")
    SET(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    SET(CMAKE_OBJCOPY_FLAGS )
    SET(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -static -Wl,--gc-sections")
    SET(CMAKE_DUMP_FLAGS     "-EL")
endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)

endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)


endmacro()


if (NOT CROSS_COMPILE)
  SET(CROSS_COMPILE arm-rcm-eabihf)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
  if (EXISTS /opt/r42/toolchains/arm-rcm-eabihf/bin/arm-rcm-eabihf-gcc)
    set(CROSS_COMPILE "/opt/r42/toolchains/arm-rcm-eabihf/bin/${CROSS_COMPILE}")
  endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
