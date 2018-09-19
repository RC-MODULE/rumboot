MEMORY {
    SRAM0 (rwx) : ORIGIN = 0x00000000, LENGTH = 0x100000,
    SDRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x4000000,
    SSRAM (rwx) : ORIGIN = 0x40000000, LENGTH = 0x400000,
    PLRAM (rwx) : ORIGIN = 0x50000000, LENGTH = 0x400000,
    SRAM1 (rwx) : ORIGIN = 0x60000000, LENGTH = 0x400000,
    NOR (rx)    : ORIGIN = 0x70000000, LENGTH = 0x400000,
    IM0 (rwx)   : ORIGIN = 0x80000000, LENGTH = 0x20000
    IM1 (rwx)   : ORIGIN = 0x80020000, LENGTH = 0x20000
    ROM (rx)    : ORIGIN = 0xFFFF0000, LENGTH = 0x10000
}

SECTIONS {

    . = 0x00000000;

    .rwx : {
        KEEP(*(.header))
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        *(.data)
        *(.data.*)

        /* ROM sections copy */
        *(*.init.text)
        /* ROM sections copy end */

        rumboot_platform_bss_start = .;
        *(.bss)
        *(.bss.*)
        rumboot_platform_bss_end = .;

        . = ALIGN(32);

        rumboot_platform_spl_start = .;
        rumboot_platform_spl_end = .;

        . = ALIGN(8);

        rumboot_im0_heap_start = .;
        . = . + 0x4000; /* 16k heap */
        rumboot_im0_heap_end = .;

        /* This is where stacks start */
        rumboot_platform_stack_area_start = .;
        . = . + 0x4000; /* 16k stack */
        /* This is where stacks end */
        rumboot_platform_stack_area_end = .;
    } > IM0

    .runtime 0x8001ff00 (NOLOAD) : {
        /* Runtime info is at the very top of iram */
        rumboot_platform_runtime_info = .;
        *(.rumboot_platform_runtime_info);
    }

    .im1_heap 0x80020000 (NOLOAD) : {
        rumboot_im1_heap_start = .;
        . = . + 0x20000; /* 128k heap */
        rumboot_im1_heap_end = .;
    } > IM1

    .sram0_heap  (NOLOAD) : {
        rumboot_sram0_heap_start = .;
        . = . + 0x20000; /* 128k heap */
        rumboot_sram0_heap_end = .;
    } > SRAM0

    .sdram_heap  (NOLOAD) : {
        rumboot_sdram_heap_start = .;
        . = . + 0x80000; /* 512k heap */
        rumboot_sdram_heap_end = .;
    } > SDRAM

    .ssram_heap  (NOLOAD) : {
        rumboot_ssram_heap_start = .;
        . = . + 0x200000; /* 2М heap */
        rumboot_ssram_heap_end = .;
    } > SSRAM

    .plram_heap  (NOLOAD) : {
        rumboot_plram_heap_start = .;
        . = . + 0x200000; /* 2М heap */
        rumboot_plram_heap_end = .;
    } > PLRAM

    .sram1_heap  (NOLOAD) : {
        rumboot_sram1_heap_start = .;
        . = . + 0x200000; /* 2М heap */
        rumboot_sram1_heap_end = .;
    } > SRAM1

    .nor_heap  (NOLOAD) : {
        rumboot_nor_heap_start = .;
        . = . + 0x200000; /* 2М heap */
        rumboot_nor_heap_end = .;
    } > NOR

    /DISCARD/ : {
        *(.init.text)
        *(.init.rodata)
        *(*.init.rodata)
        *(.init.start)
    }

}
