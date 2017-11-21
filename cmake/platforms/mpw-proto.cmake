SET(RUMBOOT_ARCH mpw-proto)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-dummy.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
)

#Flags for Power PC
macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(CMAKE_DUMP_FLAGS     "-EL")
    SET(CMAKE_C_FLAGS     "-std=gnu99 -m32")
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

set(RUMBOOT_COMMON_FLAGS "-mcpu=476fp -gdwarf-2 -m32 -ffreestanding -O0 -mbig-endian -std=c99 -fno-zero-initialized-in-bss")
set(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -mstrict-align -Wall -fdata-sections -ffunction-sections ")
set(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS "-fno-zero-initialized-in-bss -e boot_image_entry0 -Wl,--oformat=elf32-powerpc -Ttext 0x00040018 -static -nostartfiles -Wl,--gc-sections")
set(CMAKE_DUMP_FLAGS     -S -EB -M476,32)

if (NOT CROSS_COMPILE)
  SET(CROSS_COMPILE powerpc-rcm-elf)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
  if (EXISTS /opt/r42/toolchains/arm-rcm-eabihf/bin/arm-rcm-eabihf-gcc)
    set(CROSS_COMPILE "/opt/r42/toolchains/arm-rcm-eabihf/bin/${CROSS_COMPILE}")
  endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
