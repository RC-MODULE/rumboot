OUTPUT_FORMAT("elf32-nmc", "elf32-nmc", "elf32-nmc")
OUTPUT_ARCH(nmc)

MEMORY
{
  IM1(rwx):       ORIGIN =  0x00000, LENGTH = 0x20000
  IM2(rwx):       ORIGIN =  0x20000, LENGTH = 0x20000
  IM3(rwx):       ORIGIN =  0x40000, LENGTH = 0x20000
  SRAM(rwx):      ORIGIN =  0x100000000, LENGTH  = 0x100000
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
  } > IM1

  .rwx : {
    _rumboot_IM1_heap_start = .;
    . = . + 0x1000;
    _rumboot_IM1_heap_end = .;
   *(.text)
   *(.text.*)
   *(.rodata)
   *(.rodata.*)
   *(.data)
   *(.data.*)
  } > IM1


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
  } > IM1

  .stack (NOLOAD) : {
    _rumboot_platform_stack_start = .;
    . = . + 0x8000;
    _rumboot_platform_stack_end = .;    
  } > IM1

  .im2_heap (NOLOAD) : {
    _rumboot_IM2_heap_start = .;
    . = . + 0x4000;
    _rumboot_IM2_heap_end = .;    
  } > IM2

  .im3_heap (NOLOAD) : {
    _rumboot_IM3_heap_start = .;
    . = . + 0x4000;
    _rumboot_IM3_heap_end = .;    
  } > IM3

  .sram_heap (NOLOAD) : {
    _rumboot_SRAM_heap_start = .;
    . = . + 0x4000;
    _rumboot_SRAM_heap_end = .;    
  } > SRAM

  .runtime 0x5A000 (NOLOAD) : {
    rumboot_platform_runtime_info = .;
    *(.rumboot_platform_runtime_info);
  } > IM3

}
