
#define DEBUG_DCR_TEST
#define CHECK_VALUE_VERBOSE

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_plb4arb8m.h>

#define CVR3				r4,r5,r6

#define P4A8M0(OFFSET)		(DCR_PLB4ARB8M_0_BASE + (OFFSET))
#define P4A8M1(OFFSET)		(DCR_PLB4ARB8M_1_BASE + (OFFSET))
#define P4A8M2(OFFSET)		(DCR_PLB4ARB8M_2_BASE + (OFFSET))

#define eq6                 cr6*4+eq
#define eq7                 cr7*4+eq


.section ".text","ax",@progbits

.global main

main:

check_cpu_ppc_plb4arb_0_2:
    load_const  r3,   0x00000000
    cmpwi cr6,  r3,   0x0000      /* General test status in cr6 */
    rumboot_putstring "check_cpu_ppc_plb4arb_0_2\n"
    check_value CVR3, P4A8M0(PREV   ), PREV_0_RESET_VAL,	"PLB4ARB8M_0_PREV"
    check_value CVR3, P4A8M0(PACR   ), PACR_0_RESET_VAL,	"PLB4ARB8M_0_PACR"
    check_value CVR3, P4A8M0(PESR_RC), PESR_0_RESET_VAL,	"PLB4ARB8M_0_PESR_RC"

check_cpu_ppc_plb4arb_0_3:
	rumboot_putstring "check_cpu_ppc_plb4arb_0_3\n"
    load_const  r14,  P4A8M0(PESR_W)
    load_const  r15,  P4A8M0(PESR_RC)
    load_const  r17,  0x00000001    /* for shift in running_0(1) checks */
    load_const  r18,  0x00000001    /* start value for writing */
    load_const  r19,  0x80000000
    load_const  r20,  0x00000000
    load_const  r21,  0xFFFFFFFF

    mtdcrx      r15,  r21       /* clear all bits of PESR */
    mfdcrx      r16,  r15       /* read PESR */
    cmpw cr7,   r20,  r16
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_0_PESR not cleared")
plb4arb_0_running_1:
plb4arb_0_loop_1:
    mtdcrx      r14,  r18       /* set bit of PESR */
    mfdcrx      r16,  r14       /* read PESR */
    cmpw cr7,   r16,  r18
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_0_PESR_W (running 1)")
    mtdcrx      r15,  r18       /* clear PESR bit */
    cmpw cr0,   r19,  r18
    beq- cr0,   plb4arb_0_running_0
    slw         r18,  r18, r17
    b           plb4arb_0_loop_1

plb4arb_0_running_0:
    mtdcrx      r14,  r21       /* set all bits of PESR */
    mfdcrx      r16,  r15       /* read PESR */
    cmpw cr7,   r16,  r21
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_0_PESR (running 0 init)")
    crand       eq6,  eq6,  eq7

    load_const r8, 0x00000001   /* start value for writing */
plb4arb_0_loop_0:
    xor         r22,  r21, r18  /* r22 = ~r8 (getting running 0 in r8 for compare process) */
    mtdcrx      r15,  r18       /* clear one bit of PESR */
    mfdcrx      r16,  r15       /* read PESR */
    cmpw cr7,   r16,  r22
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_0_PESR (running 0)")	/* FAIL */
    crand       eq6,  eq6,  eq7
    mtdcrx      r14,  r21       /* set all bits of PESR */
    cmpw cr0,   r19,  r18
    beq- cr0,   check_cpu_ppc_plb4arb_1_2
    slw         r18,  r18, r17
    b           plb4arb_0_loop_0


check_cpu_ppc_plb4arb_1_2:
    check_value CVR3, P4A8M1(PREV   ), PREV_1_RESET_VAL,    "PLB4ARB8M_1_PREV"
    check_value CVR3, P4A8M1(PACR   ), PACR_1_RESET_VAL,    "PLB4ARB8M_1_PACR"
    check_value CVR3, P4A8M1(PESR_RC), PESR_1_RESET_VAL,    "PLB4ARB8M_1_PESR_RC"

check_cpu_ppc_plb4arb_1_3:
    load_const  r14,  P4A8M1(PESR_W )
    load_const  r15,  P4A8M1(PESR_RC)
    load_const  r17,  0x00000001        /* for shift in running_0(1) checks */
    load_const  r18,  0x00000001        /* start value for writing */
    load_const  r19,  0x80000000
    load_const  r20,  0x00000000
    load_const  r21,  0xFFFFFFFF

    mtdcrx      r15,  r21               /* clear all bits of PESR */
    mfdcrx      r16,  r15               /* read PESR */
    cmpw cr7,   r20,  r16
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_1_PESR not cleared")
plb4arb_1_running_1:
plb4arb_1_loop_1:
    mtdcrx      r14,  r18       /* set bit of PESR */
    mfdcrx      r16,  r14       /* read PESR */
    cmpw cr7,   r16,  r18
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_1_PESR_W (running 1)")
    mtdcrx      r15,  r18       /* clear PESR bit */
    cmpw cr0,   r19,  r18
    beq- cr0,   plb4arb_1_running_0
    slw         r18,  r18,  r17
    b           plb4arb_1_loop_1

plb4arb_1_running_0:
    mtdcrx      r14,  r21       /* set all bits of PESR */
    mfdcrx      r16,  r15       /* read PESR */
    cmpw cr7,   r16,  r21
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_1_PESR (running 0 init)")

    load_const  r8,  0x00000001 /* start value for writing */
plb4arb_1_loop_0:
    xor         r22,  r21, r18   /* r12 = ~r8 (getting running 0 in r8 for compare process) */
    mtdcrx      r15,  r18       /* clear one bit of PESR */
    mfdcrx      r16,  r15       /* read PESR */
    cmpw cr7,   r16,  r22
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_1_PESR (running 0)")
    mtdcrx      r14,  r21       /* set all bits of PESR */
    cmpw cr0,   r19,  r18
    beq- cr0,   check_cpu_ppc_plb4arb_2_2
    slw         r18,  r18,  r17
    b           plb4arb_1_loop_0


check_cpu_ppc_plb4arb_2_2:
    check_value CVR3, P4A8M2(PREV   ), PREV_2_RESET_VAL,    "PLB4ARB8M_2_PREV"
    check_value CVR3, P4A8M2(PACR   ), PACR_2_RESET_VAL,    "PLB4ARB8M_2_PACR"
    check_value CVR3, P4A8M2(PESR_RC), PESR_2_RESET_VAL,    "PLB4ARB8M_2_PESR_RC"

check_cpu_ppc_plb4arb_2_3:
    load_const  r14,  P4A8M2(PESR_W )
    load_const  r15,  P4A8M2(PESR_RC)
    load_const  r17,  0x00000001	/* for shift in running_0(1) checks */
    load_const  r18,  0x00000001	/* start value for writing */
    load_const  r19,  0x80000000
    load_const  r20,  0x00000000
    load_const  r21,  0xFFFFFFFF

    mtdcrx      r15,  r21			/* clear all bits of PESR */
    mfdcrx      r16,  r15			/* read PESR */
    cmpw cr7,   r20,  r16
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_2_PESR not cleared")
plb4arb_2_running_1:
plb4arb_2_loop_1:
    mtdcrx      r14,  r18			/* set bit of PESR */
    mfdcrx      r16,  r14			/* read PESR */
    cmpw cr7,   r16,  r18
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_2_PESR_W (running 1)")
    mtdcrx      r15,  r18			/* clear PESR bit */
    cmpw cr0,   r19,  r18
    beq- cr0,   plb4arb_2_running_0
    slw         r18,  r18,  r17
    b           plb4arb_2_loop_1

plb4arb_2_running_0:
    mtdcrx      r14,  r21			/* set all bits of PESR */
    mfdcrx      r16,  r15			/* read PESR */
    cmpw cr7,   r16,  r21
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_2_PESR (running 0 init)")

    load_const  r18,   0x00000001 	/* start value for writing */
plb4arb_2_loop_0:
    xor         r22,  r21, r18   	/* r22 = ~r18 (getting running 0 in r8 for compare process) */
    mtdcrx      r15,  r18           /* clear one bit of PESR */
    mfdcrx      r16,  r15           /* read PESR */
    cmpw cr7,   r16,  r22
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_2_PESR (running 0)")
    mtdcrx      r14,  r21           /* set all bits of PESR */
    cmpw cr0,   r19,  r18
    beq- cr0,   plb4arb_2_end
    slw         r18,  r18,  r17
    b           plb4arb_2_loop_0
plb4arb_2_end:

test_ok:
	rumboot_putstring "TEST OK\n"
	test_event		EVENT_OK
	load_const r3,	RESULT_OK
	b finish

error:
	rumboot_putstring "TEST ERROR\n"
	test_event		EVENT_ERROR
	load_const r3,	RESULT_ERROR

finish:
	blr


