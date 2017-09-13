#include <rumboot/irq.h>
.section .text.startup

/* --- Mode bits in PSRs --- */

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

.global rumboot_default_irq_vectors
rumboot_default_irq_vectors:
  b      rumboot_reset_handler
  b      undefined_handler
  b      swi_handler
  b      prefetch_handler
  b      abort_handler
  nop
  b      irq_handler
  b      fiq_handler


undefined_handler:
    mov r0, #RUMBOOT_IRQ_TYPE_EXCEPTION
    mov r1, #RUMBOOT_IRQ_UNDEFINED_INSTRUCTION
    b rumboot_irq_core_dispatch


swi_handler:
  mov r0, #RUMBOOT_IRQ_TYPE_EXCEPTION
  mov r1, #RUMBOOT_IRQ_SOFTWARE_EXCEPTION
  b rumboot_irq_core_dispatch

prefetch_handler:
  mov r0, #RUMBOOT_IRQ_TYPE_EXCEPTION
  mov r1, #RUMBOOT_IRQ_PREFETCH_ABORT
  b rumboot_irq_core_dispatch


abort_handler:
  mov r0, #RUMBOOT_IRQ_TYPE_EXCEPTION
  mov r1, #RUMBOOT_IRQ_DATA_ABORT
  b rumboot_irq_core_dispatch

irq_handler:
  mov r0, #RUMBOOT_IRQ_TYPE_NORMAL
  mov r1, #RUMBOOT_IRQ_IRQ
  b rumboot_irq_core_dispatch


fiq_handler:
  mov r0, #RUMBOOT_IRQ_TYPE_NORMAL
  mov r1, #RUMBOOT_IRQ_FIQ
  b rumboot_irq_core_dispatch


endless_loop:
  b endless_loop;


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
    
    /*
     *  enter each mode in turn and
     *  initialize the registers specific to it
     */
    msr  cpsr_c, #Mode_FIQ | I_Bit | F_Bit
    mov r8,  #0
    mov r9,  #0
    mov r10, #0
    mov r11, #0
    mov r12, #0
    mov r13, #0
    mov r14, #0

    msr  cpsr_c, #Mode_IRQ | I_Bit | F_Bit
    mov r13, #0
    mov r14, #0

    msr  cpsr_c, #Mode_ABT | I_Bit | F_Bit
    mov r13, #0
    mov r14, #0

    msr  cpsr_c, #Mode_UND | I_Bit | F_Bit
    mov r13, #0
    mov r14, #0

    /*
     * System mode shares user mode registers
     */
    msr  cpsr_c, #Mode_SYS | I_Bit | F_Bit
    mov r13, #0
    mov r14, #0

    msr  cpsr_c, #Mode_MON | I_Bit | F_Bit
    mov r13, #0
    mov r14, #0

    /*
     * Initialize ALU paths
     */

    adds r0, r1, r2

    /*
     * initialize return stack
     * requires brunch prediction enabled (z=1)
     */

    mrc  p15, 0, r0, c1, c0, 0 /* read control reg */
    orr  r0, r0, #0x800        /* set z bit */
    mcr  p15, 0, r0, c1, c0, 0 /* write control reg */
    bl   call_1
call_1:
    bl   call_2
call_2:
    bl   call_3
call_3:

  /*
   *  Restore Z bit (if necessary)
   */
  bic r0, r0, #0x800
  mcr  p15, 0, r0, c1, c0, 0 /* write control reg */

  /* --- set irq stack*/
  msr  cpsr_c, #Mode_IRQ | I_Bit | F_Bit /* No interrupts*/
  ldr  sp, =rumboot_platform_irq_stack_top

  /* --- set abort stack*/
  msr  cpsr_c, #Mode_ABT | I_Bit | F_Bit /* No interrupts*/
  ldr  sp, =rumboot_platform_abt_stack_top

  /* --- set und stack*/
  msr  cpsr_c, #Mode_UND | I_Bit | F_Bit /* No interrupts*/
  ldr  sp, =rumboot_platform_und_stack_top

  /* --- set fiq stack*/
  msr  cpsr_c, #Mode_FIQ | I_Bit | F_Bit /* No interrupts*/
  ldr  sp, =rumboot_platform_fiq_stack_top

  /*--- Set USR Stack*/
  msr  cpsr_c, #Mode_SYS | I_Bit | F_Bit /* No interrupts*/
  ldr  sp, =rumboot_platform_usr_stack_top

  /*--- Set SVC Stack*/
  msr  cpsr_c, #Mode_SVC | I_Bit | F_Bit /* No interrupts*/
  ldr  sp, =rumboot_platform_svc_stack_top


  bl rumboot_main
  b endless_loop
