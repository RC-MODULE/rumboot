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
  b      rumboot_undefined_handler
  b      rumboot_swi_handler
  b      rumboot_prefetch_handler
  b      rumboot_abort_handler
  nop
  b      rumboot_irq_handler
  b      rumboot_fiq_handler


rumboot_undefined_handler:
  b endless_loop

rumboot_swi_handler:
  b endless_loop

rumboot_prefetch_handler:
  b endless_loop

rumboot_abort_handler:
  b endless_loop

endless_loop:
  b endless_loop;

rumboot_irq_handler:
  b endless_loop

rumboot_fiq_handler:
  b endless_loop

.global rumboot_reset_handler

rumboot_reset_handler:
    MOV R0,  #0
    MOV R1,  #0
    MOV R2,  #0
    MOV R3,  #0
    MOV R4,  #0
    MOV R5,  #0
    MOV R6,  #0
    MOV R7,  #0
    MOV R8,  #0
    MOV R9,  #0
    MOV R10, #0
    MOV R11, #0
    MOV R12, #0
    MOV R13, #0
    MOV R14, #0

    /*
     *  Enter each mode in turn and
     *  initialize the registers specific to it
     */
    MSR  CPSR_c, #Mode_FIQ | I_Bit | F_Bit
    MOV R8,  #0
    MOV R9,  #0
    MOV R10, #0
    MOV R11, #0
    MOV R12, #0
    MOV R13, #0
    MOV R14, #0

    MSR  CPSR_c, #Mode_IRQ | I_Bit | F_Bit
    MOV R13, #0
    MOV R14, #0

    MSR  CPSR_c, #Mode_ABT | I_Bit | F_Bit
    MOV R13, #0
    MOV R14, #0

    MSR  CPSR_c, #Mode_UND | I_Bit | F_Bit
    MOV R13, #0
    MOV R14, #0

    /*
     * System mode shares user mode registers
     */
    MSR  CPSR_c, #Mode_SYS | I_Bit | F_Bit
    MOV R13, #0
    MOV R14, #0

    MSR  CPSR_c, #Mode_MON | I_Bit | F_Bit
    MOV R13, #0
    MOV R14, #0

    /*
     * Initialize ALU paths
     */

    ADDS R0, R1, R2

    /*
     * Initialize return stack
     * Requires brunch prediction enabled (Z=1)
     */

    MRC  p15, 0, R0, c1, c0, 0 /* read control reg */
    ORR  R0, R0, #0x800        /* set Z bit */
    MCR  p15, 0, R0, c1, c0, 0 /* write control reg */
    BL   call_1
call_1:
    BL   call_2
call_2:
    BL   call_3
call_3:

  /*
   *  Restore Z bit (if necessary)
   */
  BIC R0, R0, #0x800
  MCR  p15, 0, R0, c1, c0, 0 /* write control reg */

  /* --- Set IRQ Stack*/
  MSR  CPSR_c, #Mode_IRQ | I_Bit | F_Bit /* No interrupts*/
  LDR  SP, =rumboot_platform_irq_stack_top

  /*--- Set USR Stack*/
  MSR  CPSR_c, #Mode_SYS | I_Bit | F_Bit /* No interrupts*/
  LDR  SP, =rumboot_platform_usr_stack_top

  /*--- Set SVC Stack*/
  MSR  CPSR_c, #Mode_SVC | I_Bit | F_Bit /* No interrupts*/
  LDR  SP, =rumboot_platform_svc_stack_top

  bl rumboot_main
  b endless_loop
