INCLUDE oi10/memory.lds.h

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
        rumboot_im0_heap_end = .;
    } > IM0

    .stacks (NOLOAD) : {
        /* This is where stacks start */
        rumboot_platform_stack_area_start = .;
        . = . + 0x4000; /* 16k stack */
        /* This is where stacks end */
        rumboot_platform_stack_area_end = .;
    } > IM0

    .runtime 0x8001fe00 (NOLOAD) : {
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
