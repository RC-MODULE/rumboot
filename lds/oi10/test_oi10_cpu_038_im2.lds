INCLUDE oi10/memory.lds.h

SECTIONS {
    .rwx : {
        . = 0x0;
        KEEP(*(.text.test))

        /* ROM sections copy */
        *(*.init.text)
        *(.text.rom)
        /* ROM sections copy end */

        *(.text)
        *(.text.*)

        *(.rodata)
        *(.rodata.*)
        *(.data)
        *(.sbss)
        *(.sbss.*)

        . = 0x1000;
        KEEP(*(.data.test))
        *(.data.*)
    } > IM2

    INCLUDE oi10/stacks_section.lds.h

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
