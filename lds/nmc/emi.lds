OUTPUT_FORMAT("elf32-nmc", "elf32-nmc", "elf32-nmc")
OUTPUT_ARCH(nmc)

MEMORY
{
  IRAM(rwx):       ORIGIN =  0x00000000, LENGTH  = 0x60000
  SRAM(rwx):       ORIGIN =  0x100000000, LENGTH  = 0x100000
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
  } > SRAM

  .rwx : {
   *(.text)
   *(.text.*)
   *(.rodata)
   *(.rodata.*)
   *(.data)
   *(.data.*)
   _rumboot_IM2_heap_start = .;
   _rumboot_IM2_heap_end = .;
   _rumboot_IM3_heap_start = .;
   _rumboot_IM3_heap_end = .;
   _rumboot_SRAM_heap_start = .;
   _rumboot_SRAM_heap_end = .;
  } > SRAM

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
  } > SRAM


  .heap : {
    _rumboot_IM1_heap_start = .;
    . = . + 0x1000;
    _rumboot_IM1_heap_end = .;
  } > SRAM

  .stack : {
    _rumboot_platform_stack_start = .;
    . = . + 0x4000;
    _rumboot_platform_stack_end = .;
  } > SRAM

  .runtime 0x5A000 (NOLOAD) : {
    rumboot_platform_runtime_info = .;
    *(.rumboot_platform_runtime_info);
  } > IRAM

}
