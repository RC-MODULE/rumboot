MEMORY
{
    IM1 (rwx)       : ORIGIN = 0x80000000, LENGTH = 0x20000
    IM0 (rwx)       : ORIGIN = 0x80020000, LENGTH = 0x20000
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
     *(.sbss)
     *(.sbss.*)
    . = 0x1000;
    KEEP(*(.data.test_oi10_cpu_038))
    *(.data.*)
 } > IM0

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
