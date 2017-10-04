OUTPUT_ARCH(ARM)
MEMORY
{
    ROM (rx): ORIGIN = 0x0, LENGTH = 0x40000
    IM0 (rwx): ORIGIN = 0x40000, LENGTH = 0x40000
}

SECTIONS
{
 .ro : {
  KEEP(*(.header))
  *(.text)
  *(.rodata)
  *(.bss)
  *(.data)
  *(.bss.*)
  *(.data.*)

  /* This is where stacks start */
  rumboot_platform_stack_area_start = .;

  /* This is where stacks end */
  rumboot_platform_stack_area_end = .;

 } > IM0

 /DISCARD/ :
 {
     *(.text.startup)
 }

}
