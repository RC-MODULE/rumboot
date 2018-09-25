SET(RUMBOOT_ARCH ppc)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/lib/eventsystem-memory.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/uart_pl011.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/irq-mpic128.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/p64.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/plb6mcif2.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/dma2plb6.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/emi.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/gpio_pl061.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/l2c.c
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
    DEFAULT
    LDS oi10/rom.lds
    CFLAGS -DRUMBOOT_ONLY_STACK -DRUMBOOT_PRINTF_ACCEL
    LDFLAGS "-e rumboot_entry_point"
    PREFIX rom
    FEATURES ROMGEN
    FILES ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S
    TIMEOUT_CTEST 7200
)

rumboot_add_configuration(
    BAREROM
    LDS oi10/rom.lds
    CFLAGS -DRUMBOOT_ONLY_STACK
    SNAPSHOT default
    PREFIX bare-rom
    FEATURES ROMGEN
    TIMEOUT_CTEST 7200
)

rumboot_add_configuration(
  LPROBE_CPU
  PREFIX lprobe-cpu
  BOOTROM bootrom-lprobe-stub
  FEATURES LPROBE
  IRUN_FLAGS +LPROBE_MODE=CPU -input ${CMAKE_SOURCE_DIR}/../scripts/lprobe-helper.tcl
)


rumboot_add_configuration (
    IRAM
    LDS oi10/iram.lds
    PREFIX iram
    LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e rumboot_main"
    FILES ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/startup.S ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
    CFLAGS -DRUMBOOT_PRINTF_ACCEL
    BOOTROM bootrom-stub
    FEATURES LUA COVERAGE PACKIMAGE
    LOAD IM0BIN SELF
    TIMEOUT_CTEST 86400
)

rumboot_add_configuration (
    IRAM_SPL
    CONFIGURATION IRAM
    PREFIX spl
    LDFLAGS -Wl,--start-group -lgcc -lc -lm -Wl,--end-group "-e main"
    CFLAGS -DRUMBOOT_PRINTF_ACCEL -DRUMBOOT_NOINIT
    FEATURES COVERAGE PACKIMAGE
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



include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
#   Example for adding a single bare-rom test
#   add_rumboot_target(
#       CONFIGURATION BAREROM
#       FILES bare-rom/gtube-spr-check.S
#   )

    rumboot_bootrom_add_components(IRAM_SPL ROM)
    #  rumboot_bootrom_unit_test(
    #      ID 0
    #      TAG sdio0
    #      MEMTAG SDIO0_CONF
    #      TAGOFFSET 0
    #  )

    rumboot_bootrom_unit_test(
        ID 0
        TAG spi0_cs0
        MEMTAG SPI0_CONF
        TAGOFFSET 0
    )

    rumboot_bootrom_unit_test(
        ID 1
        TAG spi0_cs1
        MEMTAG SPI0_CONF
        TAGOFFSET 1
    )

    #Add lprobe sample scripts
    add_rumboot_target_dir(common/lua/
      CONFIGURATION LPROBE_CPU
      PREFIX lprobe-cpu
    )

    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/bootrom-stubs/bootrom-lprobe-stub.c
        PREFIX "bootrom"
        NAME "lprobe-stub"
        FEATURES STUB
      )

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
        IRUN_FLAGS +use_uart_data_logger
        PREFIX uart_data_logger
    )

    add_rumboot_target_dir(iss-iram/
        CONFIGURATION IRAM
        PREFIX iss-iram
        CHECKCMD ${CMAKE_SOURCE_DIR}/scripts/ISS/dump_gen.csh ${CMAKE_BINARY_DIR} $PRODUCT
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DGPIO_X_BASE=GPIO_0_BASE -DGPIO_X_INT=GPIO0_INT -DCHECK_REGS -DGPIO_TIMEOUT=0x200 IRUN_FLAGS +gpio_test
        FILES test_oi10_gpio.c
        PREFIX "gpio_0"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DGPIO_X_BASE=GPIO_1_BASE -DGPIO_X_INT=GPIO1_INT -DCHECK_REGS -DGPIO_TIMEOUT=0x200 IRUN_FLAGS +gpio_test
        FILES test_oi10_gpio.c
        PREFIX "gpio_1"
    )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DMKIO0_IRQ=MKIO0_INT -DMKIO1_IRQ=MKIO1_INT
        FILES common/mkio/mkio_write_read_test.c
        NAME mkio_write_read_test
      )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DMKIO0_IRQ=MKIO0_INT -DMKIO1_IRQ=MKIO1_INT
        FILES common/mkio/mkio0_irq_test.c
        NAME mkio0_irq_test
      )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DMKIO0_IRQ=MKIO0_INT -DMKIO1_IRQ=MKIO1_INT
        FILES common/mkio/mkio1_irq_test.c
        NAME mkio1_irq_test
      )

    add_rumboot_target(
        CONFIGURATION IRAM
        CFLAGS -DMKIO0_IRQ=MKIO0_INT -DMKIO1_IRQ=MKIO1_INT
        FILES common/mkio/mkio_polarity_test.c
        NAME mkio_polarity_test
      )

#    add_rumboot_target(
#        CONFIGURATION IRAM
#        FILES common/mkio/mkio0_events_timestamps_test.c
#        NAME mkio0_events_timestamps_test
#      )
#
#    add_rumboot_target(
#        CONFIGURATION IRAM
#        FILES common/mkio/mkio1_events_timestamps_test.c
#        NAME mkio1_events_timestamps_test
#      )

#    add_rumboot_target(
#        CONFIGURATION IRAM
#        FILES common/mkio/mkio_signal_test.c
#        NAME mkio_signal_test
#        IRUN_FLAGS +mkio_signal_test
#      )


    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_uart_000.c
      CFLAGS -DUARTRX_BASE=UART0_BASE -DUARTTX_BASE=UART1_BASE -DUARTRX_INT=UART0_INT -DUARTTX_INT=UART1_INT -DCHECK_REGISTERS
      PREFIX uart0
    )
    add_rumboot_target(
      CONFIGURATION IRAM
      FILES test_oi10_uart_000.c
      CFLAGS -DUARTRX_BASE=UART1_BASE -DUARTTX_BASE=UART0_BASE -DUARTRX_INT=UART1_INT -DUARTTX_INT=UART0_INT -DCHECK_REGISTERS
      PREFIX uart1
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
