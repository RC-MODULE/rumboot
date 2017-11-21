SET(RUMBOOT_ARCH mpw-proto)

find_program(PACKIMAGE rumboot-packimage REQUIRED)

if (PACKIMAGE-NOTFOUND)
  message(FATAL_ERROR "Missing rumboot-packimage in your path. This ain't gonna work")
endif()


file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-dummy.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
)

#Flags for Power PC
macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    set(RUMBOOT_COMMON_FLAGS "-mcpu=476fp -gdwarf-2 -m32 -ffreestanding -O0 -mbig-endian -std=c99 -fno-zero-initialized-in-bss")
    set(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -mstrict-align -Wall -fdata-sections -ffunction-sections ")
    set(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "-fno-zero-initialized-in-bss -e boot_image_entry0 -Wl,--oformat=elf32-powerpc -Ttext 0x00040018 -static -nostartfiles -Wl,--gc-sections")
    set(CMAKE_DUMP_FLAGS     -S -EB -M476,32)
endmacro()

#Add configuration for binaries
rumboot_add_configuration(
  MPW_PROTO
  DEFAULT
  LDS mpw-proto/ram.lds
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group
  FEATURES LUA
)

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)
endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)

  add_custom_target(
    ${product}.img ALL
    COMMAND ${PACKIMAGE} -t bootimage -i ${product}.bin -o ${product}.img
    COMMENT "Packing image ${product}.img"
    DEPENDS ${product}.bin
  )

  add_custom_target(
    ${product}.edcl ALL
    COMMAND ${PACKIMAGE} -t hostimage -i ${product}.bin -o ${product}.img
    COMMENT "Packing image ${product}.edcl"
    DEPENDS ${product}.bin
  )

  add_custom_target(
    ${product}.sd ALL
    COMMAND dd if=/dev/zero of=${product}.sd bs=1024 count=8;
    COMMAND dd if=${product}.img of=${product}.sd bs=1024 seek=8
    COMMENT "Creating SD card image ${product}.sd"
    DEPENDS ${product}.img
  )

  add_custom_target(
    ${product}.spi ALL
    COMMAND dd status=none if=/dev/zero of=${product}.spi bs=1K count=4096
    COMMAND dd status=none if=${product}.img of=${product}.spi conv=notrunc
    COMMENT "Creating SPI flash image ${product}.spi"
    DEPENDS ${product}.img
  )

  add_dependencies(${product}.all
        ${product}.sd
        ${product}.spi
        ${product}.edcl
        ${product}.img
    )

endmacro()

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  file(GLOB RUMBOOT_TARGETS ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/*.c)
  foreach(target ${RUMBOOT_TARGETS})
    add_rumboot_target(
        SNAPSHOT "null"
        PREFIX "test"
        FILES ${target}
    )
  endforeach()



endmacro()

macro(rumboot_platform_setup_configuration)

endmacro()


if (NOT CROSS_COMPILE)
  SET(CROSS_COMPILE powerpc-rcm-elf)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
  if (EXISTS /opt/r42/toolchains/arm-rcm-eabihf/bin/arm-rcm-eabihf-gcc)
    set(CROSS_COMPILE "/opt/r42/toolchains/arm-rcm-eabihf/bin/${CROSS_COMPILE}")
  endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
