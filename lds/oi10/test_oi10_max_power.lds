INCLUDE oi10/memory.lds.h

SECTIONS {

    . = 0x00000000;

    .rwx : {
        KEEP(*(.text.test))
        *(.text)
        *(.text.*)

        *(.rodata)
        *(.rodata.*)
        *(.data)
        *(.sbss)
        *(.sbss.*)

        /* ROM sections copy */
        *(*.init.text)
        /* ROM sections copy end */

        . = 0x4000;
        KEEP(*(.data.test))
        *(.data.*)
    } > IM0_PWR_CACHE

    INCLUDE oi10/runtime_section.lds.h

    /DISCARD/ : {
        *(.rela.*)
        *(.got2)
        *(.got)
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
        /* Make sure data & bss go to hell */
        *(.data)
        *(.bss)
        *(.bss.*)
        *(COMMON)
        *(.text._init)
        *(.text._fini)
        *(.debug_info)
    }

}
