OUTPUT_ARCH(ARM)
MEMORY
{
    ROM (rx): ORIGIN = 0x0, LENGTH = 0x40000
    IM0 (rwx): ORIGIN = 0x40000, LENGTH = 0x40000
}

SECTIONS
{
 .rwx : {
  KEEP(*(.header))
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
  . = . + 0xf000; /* 60k heap */
  /* This is where stacks end */
  rumboot_platform_stack_area_end = .;

 } > IM0

 /DISCARD/ :
 {
     *(.text.startup)
 }

}
