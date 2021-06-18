#include <rumboot/irq.h>
.extern _rumboot_main
.extern __main
.extern _rumboot_platform_stack_start

.global _start
.set _start, the_start

/**
 * ABI: 
 *   gr7 - 32 bit return value
 *   gr6, gr7 - 64 bit return value
 * Calling:
 *
 **/

.macro irq_vector itype,inum
    push ar6,gr6;
    ar6=\itype;
    delayed goto irq_wrapper;
    gr6=\inum;
.endm

.section .text.init

/*  Reset handler (A.K.A. NMI) is tricky. We have to setup the stack,
 *  clear the X-s from the registers and other stuff 
 */
reset_handler:
	delayed goto the_start; 
    nul;
    nul;

.align 5
/* Three internal exceptions follow */
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_OVERFLOW
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_UNDEFINED_INSTRUCTION
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_DEBUG
/*
	assign			PER_IRQ[63:0] =  {
																14'b00000000000000, 
																PPC_INT[1],NMC_INT[40:5],
																PPC_INT[0],TIMER1_INT,TIMER0_INT,
																2'b00,SI_AXI_ERR,INSTR_FETCH_ERR,
																6'b000000	
															 };
                                                             */
/* 
 * NMC INTs handling is tricky. There are 64 lines: PER_IRQ[63:0]
 * Some lines are actually fatal errors are treated here as exceptions 
 * Some lines (e.g. first 6 bits) are reserved
 * Numbering starts with internal timer IRQs with 0 and goes all the 
 * way up to 53. Interrupts share the same code in irq_wrapper
 */
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_SOFTWARE_EXCEPTION
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_SOFTWARE_EXCEPTION
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_SOFTWARE_EXCEPTION
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_SOFTWARE_EXCEPTION
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_SOFTWARE_EXCEPTION
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_SOFTWARE_EXCEPTION
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_INST_STORAGE 
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_DATA_ABORT
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_SOFTWARE_EXCEPTION
irq_vector RUMBOOT_IRQ_TYPE_EXCEPTION, RUMBOOT_IRQ_SOFTWARE_EXCEPTION
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    10
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    11
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    12
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    13
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    14
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    15
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    16
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    17
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    18
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    19
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    20
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    21
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    22
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    23
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    24
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    25
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    26
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    27
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    28
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    29
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    30
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    31
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    32
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    33
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    34
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    35
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    36
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    37
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    38
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    39
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    40
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    41
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    42
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    43
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    44
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    45
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    46
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    47
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    48
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    49
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    50
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    51
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    52
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    53
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    54
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    55
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    56
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    57
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    58
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    59
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    60
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    61
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    62
irq_vector RUMBOOT_IRQ_TYPE_NORMAL,    63

/**
 * IRQ & Exception wrapper. 
 * We'll get type in ar6, id in gr6. Controller should always be zero
 */

irq_wrapper:
    push ar5;
    push gr7;
    ar5 = 0;
    push ar5; /* Alignment */
    push gr6; /* num */
    push ar6; /* type */
    push ar5; /* ctrl_id == 0 */ 
    call _rumboot_irq_core_dispatch; 
    nul;

    /* Dismantle the args stuff*/
    ar7 = ar7 - 4;

    /* Restore old values */
    pop gr7;
    pop ar5;
    pop ar6,gr6;
    ireturn;

the_start:
    ar7 = _rumboot_platform_stack_start; /* Stack grown UP, ar7 increases on every push */
    ar0=0;
    ar1=0;
    ar2=0;
    ar3=0;
    ar4=0;
    ar5=0;
    ar6=0;
    gr0=0;
    gr1=0;
    gr2=0;
    gr3=0;
    gr4=0;
    gr5=0;
    gr6=0;
    gr7=0;
    /* Fire up our main code loop */
#if defined(RUMBOOT_NOINIT)
	call __main;
#else
	call _rumboot_main;
#endif
    /* Should never get here */

//#define NMC_SUPPORTS_HALT
#ifndef NMC_SUPPORTS_HALT
    goto dead_loop; 
#else
    goto halt_code;
#endif

dead_loop:
	delayed skip dead_loop;
	gr0 = gr0;
	gr0 = gr0;

halt_code:
.align 3
	delayed halt;
	gr0 -= gr0;
	[0h] = gr0;
