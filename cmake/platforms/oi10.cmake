SET(RUMBOOT_ARCH ppc)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-dummy.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
)

#Flags for Power PC
macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    set(RUMBOOT_COMMON_FLAGS "-mcpu=476fp -gdwarf-2 -m32 -ffreestanding -Os -mbig-endian -std=gnu99")
    set(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -mstrict-align -Wall -Werror -fdata-sections -ffunction-sections ")
    set(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -static -Wl,--gc-sections -Wl,--oformat=elf32-powerpc")
    set(CMAKE_DUMP_FLAGS -S -EB -M476,32)
endmacro()

rumboot_add_configuration(
  ROM
  LDS oi10/rom.lds
  CFLAGS -DRUMBOOT_ONLY_STACK -DRUMBOOT_PRINTF_ACCEL
  SNAPSHOT default
  PREFIX rom
  FEATURES ROMGEN
)


macro(rumboot_platform_generate_stuff_for_taget product)
    list (FIND TARGET_FEATURES "ROMGEN" _index)
    if (${_index} GREATER -1)
      add_custom_command(
        OUTPUT ${product}.hex/image_mem64_0.hex
        COMMAND mkdir -p ${product}.hex
        COMMAND ${CMAKE_BINARY_DIR}/utils/romgen -l oi10 -i ${product}.bin -o ${product}.hex
        COMMENT "Generating HEX memory files for ${product}.bin"
        DEPENDS ${product}.bin utils
      )

      add_custom_target(
        ${product}.hex ALL
        DEPENDS ${product}.hex/image_mem64_0.hex
      )
    endif()

endmacro()


macro(add_directory_with_targets dir)
  file(GLOB RUMBOOT_TARGETS_C ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/${dir}/*.c)
  file(GLOB RUMBOOT_TARGETS_S ${RUMBOOT_PLATFORM}/targets/${dir}/*.S)
  foreach(target ${RUMBOOT_TARGETS_C} ${RUMBOOT_TARGETS_S} )
    add_rumboot_target(
        ${ARGN}
        FILES ${target}
    )
  endforeach()
endmacro()

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  add_rumboot_target(
      CONFIGURATION ROM
      FILES hello.c
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
