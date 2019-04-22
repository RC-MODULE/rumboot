#include <rumboot/irq.h>

#define Mode_USR    0x10
#define Mode_FIQ    0x11
#define Mode_IRQ    0x12
#define Mode_SVC    0x13
#define Mode_MON    0x16
#define Mode_ABT    0x17
#define Mode_UND    0x1B
#define Mode_SYS    0x1F

/* --- Interrupt disable flags in PSRs --- */

#define I_Bit   0x80  /* when I bit is set, IRQ is disabled */
#define F_Bit   0x40  /* when F bit is set, FIQ is disabled */
#define A_Bit   0x100 /* when A bit is set, Imbrecise Data Abort Interrupts disabled */


/* --- Stack at the end of AMB0 --- */

#define FIQ_Size              256*2
#define IRQ_Size              256*2
#define SVC_Size              256*2
#define ABT_Size              256*1
#define UND_Size              256*1
#define USR_Size              256*2

.section .text.startup
.global rumboot_default_irq_vectors
rumboot_default_irq_vectors:
.align 5
  b      rumboot_reset_handler
  b      undefined_handler
  b      swi_handler
  b      prefetch_handler
  b      abort_handler
  nop
  b      irq_handler
  b      fiq_handler

undefined_handler:
    mov r0, #0
    mov r1, #RUMBOOT_IRQ_TYPE_EXCEPTION
    mov r2, #RUMBOOT_IRQ_UNDEFINED_INSTRUCTION
    bl rumboot_irq_core_dispatch
    movs pc,lr
    nop

swi_handler:
  push     {r0-r3, r12, lr}
  subs     r0, lr, #4
  ldr      r2, [r0]
  and      r2, r2, #0xff
  mov      r0, #0
  mov      r1, #RUMBOOT_IRQ_TYPE_SOFT
  bl       rumboot_irq_core_dispatch
  pop      {r0-r3, r12, lr}
  movs     pc,lr
  nop

prefetch_handler:
  mov r0, #0
  mov r1, #RUMBOOT_IRQ_TYPE_EXCEPTION
  mov r2, #RUMBOOT_IRQ_PREFETCH_ABORT
  bl rumboot_irq_core_dispatch
  movs pc,lr
  nop


abort_handler:
  mov r0, #0
  mov r1, #RUMBOOT_IRQ_TYPE_EXCEPTION
  mov r2, #RUMBOOT_IRQ_DATA_ABORT
  bl rumboot_irq_core_dispatch
  movs pc,lr
  nop

irq_handler:
  push    {r0-r3, r12, lr}
  mov     r0, #0
  mov     r1, #RUMBOOT_IRQ_TYPE_NORMAL
  mov     r2, #0
  blx     rumboot_irq_core_dispatch
  POP     {r0-r3, r12, lr}
  subs    pc, lr, #4
  nop

fiq_handler:
  mov r0, #0
  mov r1, #RUMBOOT_IRQ_TYPE_FIQ
  mov r2, #0
  bl rumboot_irq_core_dispatch
  movs pc,lr
  nop

.global rumboot_reset_handler
rumboot_reset_handler:
    mov r0,  #0
    mov r1,  #0
    mov r2,  #0
    mov r3,  #0
    mov r4,  #0
    mov r5,  #0
    mov r6,  #0
    mov r7,  #0
    mov r8,  #0
    mov r9,  #0
    mov r10, #0
    mov r11, #0
    mov r12, #0
    mov r13, #0
    mov r14, #0

    MOV     R0,#0xf00000              @ Write CPACR (Coprocessor Access Control Register)
    MCR     p15,0,R0,c1,c0,2          @ to enable coprocessors cp10 & cp11
    MRC     p15,0,R0,c1,c0,2          @ Read CPACR
    UBFX    R0, R0, #20, #1           @ if bit 20 is zero then so the bits 23:21 so FPU present


    MOV r0,#1<<30
    MCR     p10, #7, r0, c8, c0, 0    @ enable FPU by writing FPEXC
    MOV r0,#0
    MCR     p10, #7, r0, c1, c0, 0    @ init FPSR
    LDC p11, c0, [r0], {32}           @ init registers d0-d15

    @VMOV.F32 S0, R0     @ command to test FPU turn On.

    BLX rumboot_main

    BL endless_loop

endless_loop:
  b endless_loop;
