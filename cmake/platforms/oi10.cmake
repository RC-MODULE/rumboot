SET(RUMBOOT_ARCH ppc)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/lib/eventsystem-memory.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/irq-mpic128.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/p64.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/plb6mcif2.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/ppc_mmu_impl.S
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/utlb_entries.S
)

#Flags for Power PC
macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    set(RUMBOOT_COMMON_FLAGS "-mcpu=476fp -gdwarf-2 -fno-plt -fno-pic -m32 -ffreestanding -Os -mbig-endian -std=gnu99")
    set(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -mstrict-align -Wall -Werror -fdata-sections -ffunction-sections ")
    set(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -static -Wl,--gc-sections -Wl,--oformat=elf32-powerpc")
    set(CMAKE_DUMP_FLAGS -S -EB -M476,32)
endmacro()

rumboot_add_configuration(
    ROM
    LDS oi10/rom.lds
    CFLAGS -DRUMBOOT_ONLY_STACK -DRUMBOOT_PRINTF_ACCEL
    LDFLAGS "-e rumboot_entry_point"
    PREFIX rom
    FEATURES ROMGEN
    FILES ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S
)

rumboot_add_configuration(
    BAREROM
    LDS oi10/rom.lds
    CFLAGS -DRUMBOOT_ONLY_STACK
    SNAPSHOT default
    PREFIX bare-rom
    FEATURES ROMGEN
)

rumboot_add_configuration (
    IRAM
    LDS oi10/iram.lds
    PREFIX iram
    LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e rumboot_main"
    FILES ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
    CFLAGS -DRUMBOOT_PRINTF_ACCEL
    BOOTROM bootrom-stub
    FEATURES LUA COVERAGE
    LOAD IM0BIN SELF
)

macro(rumboot_platform_generate_stuff_for_taget product)
    list (FIND TARGET_FEATURES "ROMGEN" _index)
    if (${_index} GREATER -1)
        add_custom_command(
            OUTPUT ${product}.hex/image_mem64_0.hex
            COMMAND mkdir -p ${product}.hex
            COMMAND ${CMAKE_BINARY_DIR}/utils/romgen -l oi10_gen -i ${product}.bin -o ${product}.hex
            COMMENT "Generating HEX memory files for ${product}.bin"
            DEPENDS ${product}.bin utils
        )

        add_custom_target(
            ${product}.hex ALL
            DEPENDS ${product}.hex/image_mem64_0.hex
        )

        add_dependencies(${product}.all ${product}.hex)
    endif()

endmacro()



macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
#   Example for adding a single bare-rom test
#   add_rumboot_target(
#       CONFIGURATION BAREROM
#       FILES bare-rom/gtube-spr-check.S
#   )

    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/bootrom-stubs/bootrom-stub.c
        PREFIX "bootrom"
        NAME "stub"
        FEATURES STUB
    )


    add_rumboot_target(
        CONFIGURATION ROM
        CFLAGS -DUSE_SWINT=132
        FILES common/irq/irq-atomics.c
        PREFIX "irq-rom"
    )

    add_rumboot_target(
        CONFIGURATION ROM
        CFLAGS -DUSE_SWINT=132
        FILES common/irq/irq-defhandler.c
        PREFIX "irq-rom"
    )


    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DUSE_SWINT=132
        FILES common/irq/irq-atomics.c
        PREFIX "irq-iram"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DUSE_SWINT=132
        FILES common/irq/irq-defhandler.c
        PREFIX "irq-iram"
    )

    add_rumboot_target_dir(bare-rom/
        CONFIGURATION BAREROM
    )

    add_rumboot_target_dir(rom/
        CONFIGURATION ROM
    )

    add_rumboot_target_dir(tests/
        CONFIGURATION ROM
    )

    add_rumboot_target_dir(jenkins/
        CONFIGURATION IRAM
        PREFIX jenkins
        TESTGROUP short
    )

    add_rumboot_target_dir(simple-iram/
        CONFIGURATION IRAM
        PREFIX simple-iram
    )

    add_rumboot_target_dir(uart_data_logger/
        CONFIGURATION IRAM
        PREFIX uart_data_logger
    )

    add_rumboot_target_dir(iss-iram/
        CONFIGURATION IRAM
        PREFIX iss-iram
        CHECKCMD ${CMAKE_SOURCE_DIR}/scripts/ISS/dump_gen.csh ${CMAKE_BINARY_DIR} $PRODUCT
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
