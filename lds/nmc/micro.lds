OUTPUT_FORMAT("elf32-nmc", "elf32-nmc", "elf32-nmc")
OUTPUT_ARCH(nmc)

MEMORY
{
  BOOTRAM(rwx):       ORIGIN =  0x00000, LENGTH = 0x5000
  IM3(rwx):           ORIGIN =  0x40000, LENGTH = 0x20000
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
  } > BOOTRAM

  .rwx : {
   *(.text)
   *(.text.*)
   *(.rodata)
   *(.rodata.*)
   *(.data)
   *(.data.*)
   rumboot_platform_spl_start = .;
   rumboot_platform_spl_end = .;
   _rumboot_platform_bss_start = .;
   *(.bss)
   *(.bss.*)
   *(.bsst)
   *(.bsst.*)
   *(COMMON)
   _rumboot_platform_bss_end = .;
    _rumboot_IM1_heap_start = .;
    _rumboot_IM1_heap_end = .;
  } > BOOTRAM

  .runtime 0x5A000 (NOLOAD) : {
    rumboot_platform_runtime_info = .;
    *(.rumboot_platform_runtime_info);
  } > IM3

  .stack (NOLOAD) : {
    _rumboot_platform_stack_start = .;
    . = . + 0x4000;
    _rumboot_platform_stack_end = .;
  } > IM3

  /DISCARD/ : {
    *(*)
  }
}
