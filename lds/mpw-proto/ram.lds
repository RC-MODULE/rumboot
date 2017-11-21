MEMORY
{
    IM0 (rwx): ORIGIN = 0x40000, LENGTH = 0x40000
}

SECTIONS
{
 .rwx : {
  KEEP(*(.header))
  KEEP(*(.text.startup))
  *(.rumboot_platform_runtime_info);
  *(.text)
  *(.rodata)
  *(.data)
  rumboot_platform_bss_start = .;
  *(.bss)
  *(.bss.*)
  rumboot_platform_bss_end = .;
  *(.data.*)

  rumboot_platform_heap_start = .;
  . = . + 0xf000; /* 60k heap */
  rumboot_platform_heap_end = .;

  /* This is where stacks start */
  rumboot_platform_stack_area_start = .;

  . = . + 0x10000; /* 64k stack */

  /* This is where stacks end */
  rumboot_platform_stack_area_end = .;
 } > IM0


 /DISCARD/ :
 {
 /*     *(.text.startup) */
 }

}
