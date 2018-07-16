MEMORY {
    IM0 (rwx)   : ORIGIN = 0x80000000, LENGTH = 0x20000
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

        rumboot_platform_heap_start = ABSOLUTE(.);
        . = . + 0x1000; /* 60k heap */
        rumboot_platform_heap_end = ABSOLUTE(.);

        /* This is where stacks start */
        rumboot_platform_stack_area_start = .;
        . = . + 0x7f00; /* 60k stack */

        /* This is where stacks end */
        rumboot_platform_stack_area_end = .;
    } > IM0

    .runtime 0x8001ff00 (NOLOAD): {
        /* Runtime info is at the very top of iram */
        rumboot_platform_runtime_info = .;
        *(.rumboot_platform_runtime_info);
    }

    /DISCARD/ : {
        *(.init.text)
        *(.init.rodata)
        *(*.init.rodata)
        *(.init.start)
    }

}
