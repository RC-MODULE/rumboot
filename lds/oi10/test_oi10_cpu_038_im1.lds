MEMORY
{
    IM0 (rwx)       : ORIGIN = 0x80000000, LENGTH = 0x20000
    IM1 (rwx)       : ORIGIN = 0x80020000, LENGTH = 0x20000
    ROM (rx)        : ORIGIN = 0xFFFF0000, LENGTH = 0x10000
}

SECTIONS
{

    
  .rwx : {
      KEEP(*(.text.check))

    *(.text)
    *(.text.*)
    *(.text.check)
    *(.rodata)
    *(.rodata.*)
    *(.data)
    *(.data.*)
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
 }
 
}
