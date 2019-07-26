OUTPUT_FORMAT("elf32-nmc", "elf32-nmc", "elf32-nmc")
OUTPUT_ARCH(nmc)

MEMORY
{
  LOADERMEM (rx):  ORIGIN =  0x00000000, LENGTH = 0x00000200
  IRAM(rwx):       ORIGIN =  0x00000200, LENGTH = 0x01fe0000
}

SECTIONS
{
  .text_init : ALIGN(0x8)
  {
    *(.text_init);
    *(.text);
    *(.text.*);
  } > IRAM

  .bss : ALIGN (0x8) {
    _rumboot_platform_bss_start = .;
    *(.bss)
    *(.bss.*)
    _rumboot_platform_bss_end = .;
  } > IRAM

   .data : { 
       *(.data) 
       *(.data.*) 
    }
}
