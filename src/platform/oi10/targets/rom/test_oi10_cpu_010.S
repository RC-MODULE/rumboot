
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>

#define DEBUG_DCR_TEST

#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_plb4arb8m.h>

#define CVR3				r4,r5,r6

#define P4A8M0(OFFSET)		(DCR_PLB4ARB8M_0_BASE + (OFFSET))
#define P4A8M1(OFFSET)		(DCR_PLB4ARB8M_1_BASE + (OFFSET))
#define P4A8M2(OFFSET)		(DCR_PLB4ARB8M_2_BASE + (OFFSET))


.section ".text","ax",@progbits

.global main

main:

check_cpu_ppc_plb4arb_0_2:
	rumboot_putstring "check_cpu_ppc_plb4arb_0_2\n"
    check_value CVR3, P4A8M0(PREV   ), PREV_0_RESET_VAL,	"PLB4ARB8M_0_PREV"
    check_value CVR3, P4A8M0(PACR   ), PACR_0_RESET_VAL,	"PLB4ARB8M_0_PACR"
    check_value CVR3, P4A8M0(PESR_RC), PESR_0_RESET_VAL,	"PLB4ARB8M_0_PESR_RC"

check_cpu_ppc_plb4arb_0_3:
	rumboot_putstring "check_cpu_ppc_plb4arb_0_3\n"
    load_const r4,	P4A8M0(PESR_W)
    load_const r5,	P4A8M0(PESR_RC)
    load_const r7,	0x00000001 /* for shift in running_0(1) checks */
    load_const r8,	0x00000001 /* start value for writing */
    load_const r9,	0x80000000
    load_const r10,	0x00000000
    load_const r11,	0xFFFFFFFF

    mtdcrx r5, r11	/* clear all bits of PESR */
    mfdcrx r6, r5	/* read PESR */
    cmp cr7,0, r10, r6
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_0_PESR not cleared")
plb4arb_0_running_1:
plb4arb_0_loop_1:
    mtdcrx r4, r8	/* set bit of PESR */
    mfdcrx r6, r4	/* read PESR */
    cmp cr7,0, r6,r8
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_0_PESR_W (running 1)")
    mtdcrx r5, r8	/* clear PESR bit */
    cmp cr0, 0, r9, r8
    beq plb4arb_0_running_0
    slw r8, r8, r7
    b plb4arb_0_loop_1

plb4arb_0_running_0:
    mtdcrx r4, r11	/* set all bits of PESR */
    mfdcrx r6, r5	/* read PESR */
    cmp cr7,0, r6,r11
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_0_PESR (running 0 init)")

    load_const r8, 0x00000001 /* start value for writing */
plb4arb_0_loop_0:
    xor r12, r11, r8	/* r12 = ~r8 (getting running 0 in r8 for compare process) */
    mtdcrx r5, r8	/* clear one bit of PESR */
    mfdcrx r6, r5	/* read PESR */
    cmp cr7,0, r6,r12
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_0_PESR (running 0)")	/* FAIL */
    mtdcrx r4, r11	/* set all bits of PESR */
    cmp cr0, 0, r9, r8
    beq check_cpu_ppc_plb4arb_1_2
    slw r8, r8, r7
    b plb4arb_0_loop_0


check_cpu_ppc_plb4arb_1_2:
    check_value CVR3, P4A8M1(PREV   ), PREV_1_RESET_VAL,	"PLB4ARB8M_1_PREV"
    check_value CVR3, P4A8M1(PACR   ), PACR_1_RESET_VAL,	"PLB4ARB8M_1_PACR"
    check_value CVR3, P4A8M1(PESR_RC), PESR_1_RESET_VAL,	"PLB4ARB8M_1_PESR_RC"

check_cpu_ppc_plb4arb_1_3:
    load_const r4,	P4A8M1(PESR_W )
    load_const r5,	P4A8M1(PESR_RC)
    load_const r7,	0x00000001 /* for shift in running_0(1) checks */
    load_const r8,	0x00000001 /* start value for writing */
    load_const r9,	0x80000000
    load_const r10,	0x00000000
    load_const r11,	0xFFFFFFFF

    mtdcrx r5, r11 /* clear all bits of PESR */
    mfdcrx r6, r5 /* read PESR */
    cmp cr7,0, r10,r6
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_1_PESR not cleared")
plb4arb_1_running_1:
plb4arb_1_loop_1:
    mtdcrx r4, r8 /* set bit of PESR */
    mfdcrx r6, r4 /* read PESR */
    cmp cr7,0, r6,r8
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_1_PESR_W (running 1)")
    mtdcrx r5, r8 /* clear PESR bit */
    cmp cr0, 0, r9, r8
    beq plb4arb_1_running_0
    slw r8, r8, r7
    b plb4arb_1_loop_1

plb4arb_1_running_0:
    mtdcrx r4, r11 /* set all bits of PESR */
    mfdcrx r6, r5 /* read PESR */
    cmp cr7,0, r6,r11
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_1_PESR (running 0 init)")

    load_const r8, 0x00000001 /* start value for writing */
plb4arb_1_loop_0:
    xor r12, r11, r8 /* r12 = ~r8 (getting running 0 in r8 for compare process) */
    mtdcrx r5, r8 /* clear one bit of PESR */
    mfdcrx r6, r5 /* read PESR */
    cmp cr7,0, r6,r12
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_1_PESR (running 0)")
    mtdcrx r4, r11 /* set all bits of PESR */
    cmp cr0, 0, r9, r8
    beq check_cpu_ppc_plb4arb_2_2
    slw r8, r8, r7
    b plb4arb_1_loop_0


check_cpu_ppc_plb4arb_2_2:
    check_value CVR3, P4A8M2(PREV   ), PREV_2_RESET_VAL,	"PLB4ARB8M_2_PREV"
    check_value CVR3, P4A8M2(PACR   ), PACR_2_RESET_VAL,	"PLB4ARB8M_2_PACR"
    check_value CVR3, P4A8M2(PESR_RC), PESR_2_RESET_VAL,	"PLB4ARB8M_2_PESR_RC"

check_cpu_ppc_plb4arb_2_3:
    load_const r4,	P4A8M2(PESR_W )
    load_const r5,	P4A8M2(PESR_RC)
    load_const r7,	0x00000001	/* for shift in running_0(1) checks */
    load_const r8,	0x00000001	/* start value for writing */
    load_const r9,	0x80000000
    load_const r10,	0x00000000
    load_const r11,	0xFFFFFFFF

    mtdcrx r5, r11				/* clear all bits of PESR */
    mfdcrx r6, r5				/* read PESR */
    cmp cr7,0, r10,r6
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_2_PESR not cleared")
plb4arb_2_running_1:
plb4arb_2_loop_1:
    mtdcrx r4, r8				/* set bit of PESR */
    mfdcrx r6, r4				/* read PESR */
    cmp cr7,0, r6,r8
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_2_PESR_W (running 1)")
    mtdcrx r5, r8				/* clear PESR bit */
    cmp cr0, 0, r9, r8
    beq plb4arb_2_running_0
    slw r8, r8, r7
    b plb4arb_2_loop_1

plb4arb_2_running_0:
    mtdcrx r4, r11				/* set all bits of PESR */
    mfdcrx r6, r5				/* read PESR */
    cmp cr7,0, r6,r11
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_2_PESR (running 0 init)")

    load_const r8, 0x00000001 	/* start value for writing */
plb4arb_2_loop_0:
    xor r12, r11, r8			/* r12 = ~r8 (getting running 0 in r8 for compare process) */
    mtdcrx r5, r8				/* clear one bit of PESR */
    mfdcrx r6, r5				/* read PESR */
    cmp cr7,0, r6,r12
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_2_PESR (running 0)")
    mtdcrx r4, r11				/* set all bits of PESR */
    cmp cr0, 0, r9, r8
    beq plb4arb_2_end
    slw r8, r8, r7
    b plb4arb_2_loop_0
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


