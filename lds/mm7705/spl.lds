MEMORY
{
    IM0 (rwx): ORIGIN = 0x48000, LENGTH = 0x38000
}

SECTIONS
{
 .rwx : {
  KEEP(*(.header))
  KEEP(*(.text.startup))
  *(.text)
  *(.rodata)
  *(.data)
rumboot_platform_runtime_info = .;
  *(.rumboot_platform_runtime_info);
  rumboot_platform_bss_start = .;
  *(.bss)
  *(.bss.*)
  rumboot_platform_bss_end = .;
  *(.data.*)

  rumboot_platform_heap_start = .;
  . = . + 0x4000; /* 32k heap */
  rumboot_platform_heap_end = .;

  . = . + 0x4000; /* 32k stacks */

  rumboot_platform_spl_start = .;
  rumboot_platform_spl_end = .;
  rumboot_platform_stack_area_end = .;
 } > IM0

 /DISCARD/ :
 {
     *(.text.startup)
 }

}
