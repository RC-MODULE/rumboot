MEMORY
{
    ROM_SHIM (rx): ORIGIN = 0x40000,  LENGTH = 0x20000
    IM0 (rwx): ORIGIN = 0x60000, LENGTH = 0x20000
}

SECTIONS
{

  .text : {
    KEEP(*(.header))
    *(.text)
    *(.text.*)
    *(.rodata)
    *(.rodata.*)
  } > ROM_SHIM

 .rw : {
  . = ALIGN(8);

  rumboot_platform_spl_start = .;
  . = . + 0x10000; /* spl image start */
  rumboot_platform_spl_end = .;

  . = ALIGN(8);

  rumboot_platform_heap_start = .;
  rumboot_platform_heap_end = .;

  /* This is where stacks start */
  rumboot_platform_stack_area_start = .;
  . = . + 0x7fa0; /* 60k stack */

  /* This is where stacks end */
  rumboot_platform_runtime_info = .;
  *(.rumboot_platform_runtime_info);

  . = ALIGN(8);
  rumboot_platform_stack_area_end = .;
 } > IM0

 /DISCARD/ :
 {
     libc.a ( * )
     libm.a ( * )
     libgcc.a ( * )
     /* Make sure data & bss go to hell */
     *(.data)
     *(.data.*)
     *(.bss)
     *(.bss.*)
     *(COMMON)
 }

}
