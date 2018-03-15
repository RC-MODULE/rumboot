OUTPUT_ARCH(ARM)
MEMORY
{
    ROM (rx): ORIGIN = 0x0, LENGTH = 0x8000
    IM0 (rwx): ORIGIN = 0x40000, LENGTH = 0x40000
}

SECTIONS
{

 .rwx : {
  KEEP(*(.header))

  rumboot_im0_heap_start = .;
  . = . + 0x4000; /* 16k IM0 heap */
  rumboot_im0_heap_end = .;

  KEEP(*(.text.startup))
  *(.text)
  *(.rodata)
  *(.data)

  rumboot_platform_bss_start = .;
  *(.bss)
  *(.bss.*)
  rumboot_platform_bss_end = .;
  *(.data.*)

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


 .runtime 0x7f000 (NOLOAD): {
  /* Runtime info is at the very top of iram */
  rumboot_platform_runtime_info = .;
  *(.rumboot_platform_runtime_info);
 }

 .im1_heap 0x7b000 (NOLOAD): {
  rumboot_im1_heap_start = .;
  . = . + 0x4000; /* 16k IM1 heap */
  rumboot_im1_heap_end = .;
 }

 /DISCARD/ :
 {
     *(.text.startup)
 }

}
