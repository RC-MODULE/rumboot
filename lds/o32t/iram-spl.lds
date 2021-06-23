INCLUDE o32t/memory.lds.h

SECTIONS {

    . = 0x00000000;

    .rwx : {
        rumboot_self = .;
        KEEP(*(.header))
        /* ROM sections copy */
        *(.text.irq)
        *(*.init.text)
        /* ROM sections copy end */

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
    } > IM2

    INCLUDE o32t/stacks_section.lds.h

    INCLUDE o32t/runtime_section.lds.h

    /DISCARD/ : {
        *(.init.text)
        *(.init.rodata)
        *(*.init.rodata)
        *(.init.start)
    }

}
