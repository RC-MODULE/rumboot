INCLUDE oi10/memory.lds.h

SECTIONS {
    .rwx : {
        . = 0x0;
        KEEP(*(.text.test))
        *(.text)
        *(.text.*)

        *(.rodata)
        *(.rodata.*)
        *(.data)
        *(.data.*)

        /* ROM sections copy */
        *(*.init.text)
        /* ROM sections copy end */

        *(.sbss)
        *(.sbss.*)   
    } > SRAM0

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
