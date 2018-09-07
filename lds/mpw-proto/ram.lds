MEMORY
{
    IM0 (rwx): ORIGIN = 0x40000, LENGTH = 0x40000
}

SECTIONS
{


.text :
{
  KEEP(*(.header))
  *(.rumboot_platform_runtime_info);
  *(.text)
  *(.text.*)
  *(.rodata)
  *(.data)
  *(.rodata.*)
  *(.data.*)
} > IM0

.bss :
{
  rumboot_platform_bss_start = .;
  *(.bss)
  *(.bss.*)
  rumboot_platform_bss_end = .;
  rumboot_platform_heap_start = .;
  . = . + 0xf000; /* 60k heap */
  rumboot_platform_heap_end = .;
  rumboot_platform_spl_start = .;
  . = . + 0xf00;
  rumboot_platform_spl_end = .;
} > IM0

}
