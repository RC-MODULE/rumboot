INCLUDE oi10/memory.lds.h

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
     *(.sbss)
     *(.sbss.*)
 
    /* ROM sections copy */
    *(*.init.text)
    /* ROM sections copy end */
        
    . = 0x1000;
    KEEP(*(.data.test))
    *(.data.*)
 } > SRAM0

 .runtime 0x8001fe00 (NOLOAD): {
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
