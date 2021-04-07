OUTPUT_ARCH(ARM)
MEMORY
{
  IRAM (rxw): ORIGIN = 0x0008000, LENGTH = 0x78000
}

SECTIONS
{

 .header : {
   rumboot_self = .;
   KEEP(*(.header))
 } > IRAM

 .rwx : {
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
