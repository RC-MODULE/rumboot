SET(RUMBOOT_ARCH ppc)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-mpic128.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    set(RUMBOOT_COMMON_FLAGS "-DRUMBOOT_NO_IRQ_MACROS -mcpu=476fp -gdwarf-2 -m32 -ffreestanding -O0 -mbig-endian -fno-zero-initialized-in-bss")
    set(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -mstrict-align -Wall -fdata-sections -ffunction-sections ")
    set(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "-fno-zero-initialized-in-bss -e boot_image_entry0 -Wl,--oformat=elf32-powerpc -Ttext 0x00040018 -static -nostartfiles -Wl,--gc-sections")
    set(CMAKE_DUMP_FLAGS -S -EB -M476,32)

endmacro()

rumboot_add_configuration(
  PRIMARY
  LDS mpw-proto/primary.lds
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader_legacy.c
  CFLAGS -DRUMBOOT_ONLY_STACK
  #LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group
  PREFIX PRIMARY
  FEATURES PACKIMAGE
)

#Add configuration for binaries
rumboot_add_configuration(
  SECONDARY
  LDS mpw-proto/secondary.lds
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  CFLAGS -DRUMBOOT_NEWLIB_PRINTF
  LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group
  FEATURES PACKIMAGE SECONDARY
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
