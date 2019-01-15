OUTPUT_ARCH(ARM)
MEMORY
{
  AMB0 (rx) : ORIGIN = 0x00000, LENGTH = 0x0003E000
  AMB1 (rxw): ORIGIN = 0x40000, LENGTH = 0x00040000
}

SECTIONS
{

 .header : {
   KEEP(*(.header))
 } > AMB1


 .preinit_array     :
 {
   PROVIDE_HIDDEN (__preinit_array_start = .);
   KEEP (*(.preinit_array*))
   PROVIDE_HIDDEN (__preinit_array_end = .);
 } > AMB1

 .init_array :
 {
   PROVIDE_HIDDEN (__init_array_start = .);
   KEEP (*(SORT(.init_array.*)))
   KEEP (*(.init_array*))
   PROVIDE_HIDDEN (__init_array_end = .);
 } > AMB1

 .rwx : {
  rumboot_AMB1_heap_start = .;
  rumboot_AMB1_heap_end = .;

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
  } > AMB1

 .runtime 0x7f000 (NOLOAD):
 {
  /* Runtime info is at the very top of iram */
  rumboot_platform_runtime_info = .;
  *(.rumboot_platform_runtime_info);
 }

}
