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
        *(.sdata)
        *(.sdata.*)
        *(.sdata2)
        *(.sdata2.*)

        /* ROM sections copy */
        *(*.init.text)
        /* ROM sections copy end */

        rumboot_platform_bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        *(COMMON)
        rumboot_platform_bss_end = .;

        . = ALIGN(32);

        rumboot_platform_spl_start = .;
        rumboot_platform_spl_end = .;

        . = ALIGN(8);

        rumboot_im0_heap_start = .;
        rumboot_im0_heap_end = .;
    } > IM1

    .stacks (NOLOAD) : {
        /* This is where stacks start */
        rumboot_platform_stack_area_start = .;
        . = . + 0x4000; /* 16k stack */
        /* This is where stacks end */
        rumboot_platform_stack_area_end = .;
    } > IM0

    INCLUDE oi10/runtime_section.lds.h

    /DISCARD/ : {
        *(.init.text)
        *(.init.rodata)
        *(*.init.rodata)
        *(.init.start)
    }

}
