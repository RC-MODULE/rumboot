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
      KEEP(*(.text.check_cpu_ppc_038))
    *(.text)
    *(.text.*)

    *(.rodata)
    *(.rodata.*)
    *(.data)
    *(.rumboot_platform_runtime_info)
     *(.sbss)
     *(.sbss.*)
    . = 0x1000;
    KEEP(*(.data.test_oi10_cpu_038))
    *(.data.*)
 } > IM1
/*    .data.test_oi10_cpu_038 0x80021000 :
    {
        KEEP(*(.data.test_oi10_cpu_038))
    }>IM1
*/
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
