
OUTPUT_ARCH( "riscv" )
ENTRY(_start)

MEMORY {
  RAM (rwx)  : ORIGIN = 0x0, LENGTH = 1M
  EMEM0 (rwx): ORIGIN = 0x01000000, LENGTH = 0x7effffff
  EMEM1 (rwx): ORIGIN = 0x80000000, LENGTH = 0x7fffffff
}

STACK_SIZE = 1024;

CL_SIZE = 32;

SECTIONS {

  /* code segment */
  .text.init 0 : { 
    FILL(0);
    . = 0x100 - 12;
    SIM_EXIT = .;
    LONG(0x13);
    SIM_STOP = .;
    LONG(0x6F);
    LONG(-1);
    . = 0x100;
    PROVIDE(__TEXT_START__ = .);
    *(.text.init) 
  } >RAM

  .text  : {
    *crt.o(.text .text.*)
    *(.text .text.*)
    *(sc_test_section)
    . = ALIGN(CL_SIZE);
     PROVIDE(__TEXT_END__ = .);
  } >RAM 

  /* data segment */
  .data : {
    *(.data .data.*)
    . = ALIGN(CL_SIZE);
  } >RAM

 .extmem0 (NOLOAD): {
   rumboot_emem0_heap_start = .;
   . = . + 1024M;
   rumboot_emem0_heap_end = .;
 } > EMEM0

 .extmem1 (NOLOAD): {
   rumboot_emem1_heap_start = .;
   . = . + 1024M;
   rumboot_emem1_heap_end = .;
 } > EMEM1

 .runtime 0xfc00 (NOLOAD):
 {
  /* Runtime info is at the very top of iram */
  rumboot_platform_runtime_info = .;
  *(.rumboot_platform_runtime_info);
 }

  .sdata : {
    __global_pointer$ = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    . = ALIGN(CL_SIZE);
  } >RAM

  /* thread-local data segment */
  .tdata : {
    PROVIDE(_tls_data = .);
    PROVIDE(_tdata_begin = .);
    *(.tdata .tdata.*)
    PROVIDE(_tdata_end = .);
    . = ALIGN(CL_SIZE);
  } >RAM

  .tbss : {
    rumboot_platform_bss_start = .;
    *(.tbss .tbss.*)
    . = ALIGN(CL_SIZE);
    rumboot_platform_bss_end = .;
  } >RAM

  .heap : {
    rumboot_IRAM_heap_start = . ;
    . = . + 4K;
    rumboot_IRAM_heap_end = . ;
  } > RAM

  /* bss segment */
  .sbss : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  } >RAM

  .bss : {
    *(.bss .bss.*)
    . = ALIGN(CL_SIZE);
    PROVIDE(__BSS_END__ = .);
  } >RAM

  _end = .;
  PROVIDE(__end = .);

  /* End of uninitalized data segement */

  .stack ORIGIN(RAM) + LENGTH(RAM) - STACK_SIZE : {
    FILL(0);
    PROVIDE(__STACK_START__ = .);
    . += STACK_SIZE;
    PROVIDE(__C_STACK_TOP__ = .);
    PROVIDE(__STACK_END__ = .);
  } >RAM

  /DISCARD/ : {
    *(.eh_frame .eh_frame.*)
  }
}

