MEMORY
{
    IM0 (rwx)       : ORIGIN = 0x80000000, LENGTH = 0x20000
    IM1 (rwx)       : ORIGIN = 0x80020000, LENGTH = 0x20000
    ROM (rx)        : ORIGIN = 0xFFFF0000, LENGTH = 0x10000
}

SECTIONS
{

    
  .rwx : {
      . = 0x0;
      . = ALIGN(32);
      KEEP(*(.text.check_cpu_ppc_038))
    *(.text)
    *(.text.*)
      . = 0x1000;
      . = ALIGN(32);
      KEEP(*(.data.mass))

    *(.rodata)
    *(.rodata.*)
    *(.data)
    *(.data.*)
    . = ALIGN(32);
    post_padding = ALIGN(32);
 } > IM1

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
