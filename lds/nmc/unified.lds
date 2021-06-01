OUTPUT_FORMAT("elf32-nmc", "elf32-nmc", "elf32-nmc")
OUTPUT_ARCH(nmc)

MEMORY
{
  IRAM(rwx):       ORIGIN =  0x00000000, LENGTH = 0x60000
}

SECTIONS
{
  OVERLAY : {
    .header {
      KEEP(*(.header))
    }
    .init {
      KEEP(*(.text.init))
    }    
  } > IRAM

  .rwx : {
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
    *(COMMON)
    _rumboot_platform_bss_end = .;
  } > IRAM


  .heap (NOLOAD) : {
    _rumboot_IRAM_heap_start = .;
    . = . + 0x1000;
    _rumboot_IRAM_heap_end = .;
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
