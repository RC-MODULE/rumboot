MEMORY
{
    SRAM0 (rwx)       : ORIGIN = 0x00000000, LENGTH = 0x10000000
}

SECTIONS
{
    .rwx : {
        . = 0x0;
        KEEP(*(.text.test_oi10_cpu_021))
    
        *(.text)
        *(.text.*)
    
        *(.rodata)
        *(.rodata.*)
        *(.data)
        *(.data.*)
         *(.sbss)
         *(.sbss.*)
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
