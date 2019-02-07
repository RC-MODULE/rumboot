INCLUDE oi10/memory.lds.h

SECTIONS
{

    
  .rwx : {
      . = 0x0;
      KEEP(*(.text.check_cpu_ppc_038))
    *(.text)
    *(.text.*)

    *(.rodata)
    *(.rodata.*)
    *(.data)
     *(.sbss)
     *(.sbss.*)
    . = 0x1000;
    KEEP(*(.data.test_oi10_cpu_038))
    *(.data.*)
 } > IM1

 INCLUDE oi10/runtime_section.lds.h

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
