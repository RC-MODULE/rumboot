OUTPUT_ARCH(ARM)
MEMORY
{
    ROM (rx): ORIGIN = 0x0, LENGTH = 0x8000
    IM0 (rwx): ORIGIN = 0x40000, LENGTH = 0x40000
    DDR0 (rwx): ORIGIN = 0x80000000, LENGTH = 0x800000
    DDR1 (rwx): ORIGIN = 0xC0000000, LENGTH = 0x800000
}

SECTIONS
{

 .header : {
   KEEP(*(.header))
 } > IM0

 .rwx : {
  KEEP(*(.text.startup.rumboot_default_irq_vectors))
  *(.text.startup)

  rumboot_im0_heap_start = .;
  . = . + 0x8000; /* 32k IM0 heap */
  rumboot_im0_heap_end = .;

  *(.text)
  *(.text.*)
  *(.rodata)
  *(.data)

  rumboot_platform_bss_start = .;
  *(.bss)
  *(.bss.*)
  rumboot_platform_bss_end = .;
  *(.data.*)
  } > IM0

 .stacks (NOLOAD) : {
  /* This is where stacks start */
  rumboot_platform_stack_area_start = .;

  . = . + 0x400; /* 1k FIQ stack */
  rumboot_platform_fiq_stack_top = .;

  . = . + 0x400; /* 1k ABT stack */
  rumboot_platform_abt_stack_top = .;

  . = . + 0x400; /* 1k UND stack */
  rumboot_platform_und_stack_top = .;

  . = . + 0x400; /* 1k USR stack */
  rumboot_platform_usr_stack_top = .;

  . = . + 0x1000; /* 4k IRQ stack */
  rumboot_platform_irq_stack_top = .;

  . = . + 0x4000; /* 16k svc stack */
  rumboot_platform_svc_stack_top = .;

  /* This is where stacks end */
  rumboot_platform_stack_area_end = .;
  rumboot_platform_spl_start = .;
  rumboot_platform_spl_end = .;
 } > IM0

 .preinit_array     :
 {
   PROVIDE_HIDDEN (__preinit_array_start = .);
   KEEP (*(.preinit_array*))
   PROVIDE_HIDDEN (__preinit_array_end = .);
 } > IM0

 .init_array :
 {
   PROVIDE_HIDDEN (__init_array_start = .);
   KEEP (*(SORT(.init_array.*)))
   KEEP (*(.init_array*))
   PROVIDE_HIDDEN (__init_array_end = .);
 } > IM0

 .runtime 0x7f000 (NOLOAD):
 {
  /* Runtime info is at the very top of iram */
  rumboot_platform_runtime_info = .;
  *(.rumboot_platform_runtime_info);
 }

 .im1_heap 0x7b000 (NOLOAD):
 {
  rumboot_im1_heap_start = .;
  . = . + 0x4000; /* 16k IM1 heap */
  rumboot_im1_heap_end = .;
 }

 .ddr0 (NOLOAD):
 {
   rumboot_ddr0_heap_start = .;
   . = . + 0x800000; /* 8Mb DDR0 heap */
   rumboot_ddr0_heap_end = .;
 } > DDR0

 .ddr1 (NOLOAD):
 {
   rumboot_ddr1_heap_start = .;
   . = . + 0x800000; /* 8Mb DDR0 heap */
   rumboot_ddr1_heap_end = .;
 } > DDR1

}
