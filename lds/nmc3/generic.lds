OUTPUT_FORMAT("elf32-nmc", "elf32-nmc", "elf32-nmc")
OUTPUT_ARCH(nmc)

MEMORY
{
  IRAM(rwx):       ORIGIN =  0x00000000, LENGTH = 0x60000
}

SECTIONS
{
  .header : {
    KEEP(*(.header))
  }

  .rwx : {
    KEEP(*(.text.init))
   *(.text)
   *(.text.*)
   *(.rodata)
   *(.rodata.*)
   *(.data)
   *(.data.*)
  } > IRAM

  .bss : {
    rumboot_platform_spl_start = .;
    rumboot_platform_spl_end = .;
    _rumboot_platform_bss_start = .;
    *(.bss)
    *(.bss.*)
    *(.bsst)
    *(.bsst.*)
    _rumboot_platform_bss_end = .;
  } > IRAM

  .stack (NOLOAD) : {
    _rumboot_platform_stack_start = .;
    . = . + 0x4000;
    _rumboot_platform_stack_end = .;    
  } > IRAM

  .runtime 0x5A000 (NOLOAD) : {
    rumboot_platform_runtime_info = .;
    *(.rumboot_platform_runtime_info);
  } > IRAM

}
