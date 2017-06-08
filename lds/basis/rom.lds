OUTPUT_ARCH(ARM)
MEMORY
{
    ROM (rx): ORIGIN = 0x0, LENGTH = 0x40000
    IM0 (rwx): ORIGIN = 0x40000, LENGTH = 0x40000
}

SECTIONS
{

 .ro : {
  *(.text.startup)
  *(.text)
  *(.rodata)
 } > ROM

 .rw : {
  . = ALIGN(8);

  rumboot_platform_spl_start = .;
  . = . + 0x20000; /* spl image start */
  rumboot_platform_spl_end = .;

  rumboot_platform_runtime_info = .;
  *(.rumboot_platform_runtime_info);
  . = ALIGN(8);

  rumboot_platform_heap_start = .;
  . = . + 0xf000; /* 60k heap */
  rumboot_platform_heap_end = .;

  . = . + 0x10000; /* 64k stack */
  rumboot_platform_stack_top = .;

 } > IM0

}
