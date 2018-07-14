SET(RUMBOOT_ARCH ppc)

find_program(PACKIMAGE rumboot-packimage REQUIRED)
find_program (PYTHON NAMES python3 python3.6 REQUIRED)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-dummy.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
)

#Flags for Power PC
macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    set(RUMBOOT_COMMON_FLAGS "-mcpu=476fp -gdwarf-2 -m32 -ffreestanding -O0 -mbig-endian -fno-zero-initialized-in-bss")
    set(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -mstrict-align -Wall -fdata-sections -ffunction-sections ")
    set(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "-fno-zero-initialized-in-bss -e boot_image_entry0 -Wl,--oformat=elf32-powerpc -Ttext 0x00040018 -static -nostartfiles -Wl,--gc-sections")
    set(CMAKE_DUMP_FLAGS -S -EB -M476,32)
endmacro()

rumboot_add_configuration(
  PRIMARY
  LDS mpw-proto/primary.lds
  CFLAGS -DRUMBOOT_ONLY_STACK
  #LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group
  PREFIX PRIMARY
)

#Add configuration for binaries
rumboot_add_configuration(
  SECONDARY
  LDS mpw-proto/secondary.lds
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  CFLAGS -DRUMBOOT_NEWLIB_PRINTF
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group
  FEATURES SECONDARY
)

rumboot_add_configuration(
  MPW-PROTO
  DEFAULT
  LDS mpw-proto/ram.lds
  CFLAGS -DRUMBOOT_NEWLIB_PRINTF
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group
  FEATURES MPW-PROTO
)


macro(rumboot_platform_generate_stuff_for_taget product)

if(${TARGET_CONFIGURATION} STREQUAL "SECONDARY")
  add_custom_target(
    ${product}.img ALL
    COMMAND ${PYTHON} ${CMAKE_SOURCE_DIR}/scripts/rumbootpackimage.py -i ${product}.bin
    COMMENT "Packing image ${product}.img"
    DEPENDS ${product}.bin
  )

  add_dependencies(${product}.all ${product}.img)
else()

  add_custom_target(
    ${product}.img ALL
    COMMAND COMMAND ${PACKIMAGE} -t bootimage -i ${product}.bin -o ${product}.img
    COMMENT "Packing image ${product}.img"
    DEPENDS ${product}.bin
  )

  add_custom_target(
    ${product}.edcl ALL
    COMMAND ${PACKIMAGE} -t hostimage -i ${product}.bin -o ${product}.edcl
    COMMENT "Creating edcl image ${product}.edcl"
    DEPENDS ${product}.bin
  )

  add_dependencies(${product}.all ${product}.img ${product}.edcl)
endif()

  add_custom_target(
    ${product}.sd ALL
    COMMAND dd if=/dev/zero of=${product}.sd bs=1024 count=8;
    COMMAND dd if=${product}.bin of=${product}.sd bs=1024 seek=8
    # COMMAND dd if=${product}.bin of=${product}.sd
    COMMENT "Creating SD card image ${product}.sd"
    DEPENDS ${product}.bin
  )

  add_dependencies(${product}.all ${product}.sd)

endmacro()


macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)

  add_rumboot_target_dir(primary/
    CONFIGURATION PRIMARY
  PREFIX primary)

  add_rumboot_target_dir(secondary/
      CONFIGURATION SECONDARY
    PREFIX secondary)

  foreach(target ${RUMBOOT_TARGETS_C} ${RUMBOOT_TARGETS_S})
    add_rumboot_target(
        SNAPSHOT "null"
        PREFIX "test"
        FILES ${target}
    )
  endforeach()

endmacro()

# macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
#   file(GLOB RUMBOOT_TARGETS ${PROJECT_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/*.c  )
#   foreach(target ${RUMBOOT_TARGETS})
#     add_rumboot_target(
#         SNAPSHOT "null"
#         PREFIX "test"
#         FILES ${target}
#     )
#   endforeach()
# endmacro()

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
