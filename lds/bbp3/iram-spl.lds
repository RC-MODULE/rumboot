OUTPUT_ARCH(ARM)
MEMORY
{
  IRAM (rxw): ORIGIN = 0x0008000, LENGTH = 0x78000
}

SECTIONS
{

 .header : {
   KEEP(*(.header))
 } > IRAM


 .preinit_array     :
 {
   PROVIDE_HIDDEN (__preinit_array_start = .);
   KEEP (*(.preinit_array*))
   PROVIDE_HIDDEN (__preinit_array_end = .);
 } > IRAM

 .init_array :
 {
   PROVIDE_HIDDEN (__init_array_start = .);
   KEEP (*(SORT(.init_array.*)))
   KEEP (*(.init_array*))
   PROVIDE_HIDDEN (__init_array_end = .);
 } > IRAM

 .rwx : {
  rumboot_IRAM_heap_start = .;
  rumboot_IRAM_heap_end = .;

  *(.text)
  *(.text.*)
  *(.rodata)
  *(.data)

  rumboot_platform_bss_start = .;
  *(.bss)
  *(.bss.*)
  rumboot_platform_bss_end = .;
  *(.data.*)
  rumboot_platform_spl_start = .;

  rumboot_platform_spl_end = .;
  } > IRAM

 .runtime 0x7f000 (NOLOAD):
 {
  /* Runtime info is at the very top of iram */
  rumboot_platform_runtime_info = .;
  *(.rumboot_platform_runtime_info);
 }

}
