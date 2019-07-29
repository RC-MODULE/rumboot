OUTPUT_FORMAT("elf32-nmc", "elf32-nmc", "elf32-nmc")
OUTPUT_ARCH(nmc)

MEMORY
{
  LOADERMEM (a):   ORIGIN =  0x00000000, LENGTH = 0x00000800
  IRAM(rwx):       ORIGIN =  0x00000800, LENGTH = 0x0001f800
}

SECTIONS
{

  .easynmc_args : {
    *(._easynmc_args)
  } > IRAM

  .easynmc_stdin : {
   *(.easynmc_stdin)
   *(.easynmc_stdin.*)
  } > IRAM

  .easynmc_stdout : {
   *(.easynmc_stdout)
   *(.easynmc_stdout.*)
  } > IRAM

  .rwx : {
   *(.rumboot_platform_runtime_info)
   *(.easynmc_stdout)
   *(.text)
   *(.text.*)
   *(.rodata)
   *(.data)
   *(.data.*) 
  } > IRAM

  .bss : {
    _rumboot_platform_bss_start = .;
    *(.bss)
    *(.bss.*)
    _rumboot_platform_bss_end = .;
  } > IRAM

  .stack (NOLOAD) : {
    _rumboot_platform_stack_start = .;
    . = . + 0x4000;
    _rumboot_platform_stack_end = .;    
  } > IRAM
}
