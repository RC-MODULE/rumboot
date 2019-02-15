MEMORY
{
    SRAM0 (rwx)       : ORIGIN = 0x00000000, LENGTH = 0x100000
}

SECTIONS
{
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

    . = 0x1000;
    KEEP(*(.trace1.funcs))

    . = 0x3F000;
    KEEP(*(.trace2.funcs))

    . = 0x40000;

 } > SRAM0

 .runtime 0x8001ff00 (NOLOAD): {
    /* Runtime info is at the very top of iram */
    rumboot_platform_runtime_info = .;
    *(.rumboot_platform_runtime_info);
 }

 /DISCARD/ :
 {
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
