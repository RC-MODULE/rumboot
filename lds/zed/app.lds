OUTPUT_ARCH(ARM)
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
MEMORY
{
    DDR0 : ORIGIN = 0x00100000, LENGTH = 0x00100000
    RAM0 : ORIGIN = 0x00000000, LENGTH = 0x00030000
    RAM1 : ORIGIN = 0xFFFF0000, LENGTH = 0x0000FE00
}

SECTIONS
{
     .ro : {
	  *(.text.startup)
      *(.rumboot_platform_runtime_info)
      *(.rumboot_platform_runtime_info.*)
	  *(.text)
      *(.text.*)
	  *(.rodata)
	  *(.rodata.*)
	 } > RAM0

     .rw : {
	  *(.data)
	  *(.data.*)
       rumboot_platform_bss_start = .;
	  *(.bss)
	  *(.bss.*)
       rumboot_platform_bss_end = .;
       *(COMMON)
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
       rumboot_heap_start = .;
       . = . + 0x4000;
       rumboot_heap_end = .;

	 } > RAM0

     . = ALIGN(8);

}
