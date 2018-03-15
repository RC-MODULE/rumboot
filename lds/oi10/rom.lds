MEMORY
{
    IM0 (rwx)       : ORIGIN = 0x00040000, LENGTH = 0x20000
    IM1 (rwx)       : ORIGIN = 0x00060000, LENGTH = 0x20000
    ROM (rx)        : ORIGIN = 0xFFFF0000, LENGTH = 0x10000
}

SECTIONS
{

.text : {
    *(.text)    
    *(.text.*)
    *(.rodata)    
    . = ALIGN(32);
          } > ROM
.got2 : {
    *(.got2)
    } > ROM
.init.text 0xFFFFF000:{
        KEEP(*(.init.text))
        KEEP(*(.init.rodata))
} > ROM
.init.start 0xFFFFFFFC :    {
        KEEP(*(.init.start))
    }>ROM  
 .rw : {
  . = ALIGN(8);

  rumboot_platform_spl_start = .;
  . = . + 0x10000; /* spl image start */
  rumboot_platform_spl_end = .;

  . = ALIGN(8);

  rumboot_platform_heap_start = ABSOLUTE(.);
  . = . + 0x8000; /* 60k heap */
  rumboot_platform_heap_end = ABSOLUTE(.);

  /* This is where stacks start */
  rumboot_platform_stack_area_start = .;
  . = . + 0x7fa0; /* 60k stack */

  /* This is where stacks end */
  rumboot_platform_stack_area_end = .;
 } > IM0

 .runtime 0x7f000 (NOLOAD): {
  /* Runtime info is at the very top of iram */
  rumboot_platform_runtime_info = .;
  *(.rumboot_platform_runtime_info);
 }


 /DISCARD/ :
 {
     libc.a ( * )
     libm.a ( * )
     libgcc.a ( * )
     /* Make sure data & bss go to hell */
     *(.data)
     *(.bss)
     *(.bss.*)
     *(COMMON)
 }

}
