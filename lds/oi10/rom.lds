MEMORY
{
    ROM (rx): ORIGIN = 0x00000,  LENGTH = 0x40000
    IM0 (rwx): ORIGIN = 0x40000, LENGTH = 0x40000
}

SECTIONS
{

  .text : {
    KEEP(*(.text.startup))
    *(.text)
    *(.text.*)
    *(.rodata)
    . = ALIGN(32);    
  } > ROM

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
