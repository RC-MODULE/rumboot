
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
// #include <mivem_mem_map.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>

// #include "test_mpw_cpu_010_0.h"
#define PREV				0x2
#define PACR				0x3
#define PESR_RC				0x4
#define PESR_W				0x5
#define PEARL				0x6
#define PEARH				0x7

#define PLB4ARB8M_0_BASE DCR_PLB4ARB8M_0_BASE
#define PLB4ARB8M_1_BASE DCR_PLB4ARB8M_1_BASE
#define PLB4ARB8M_2_BASE DCR_PLB4ARB8M_2_BASE

#define PLB4ARB8M_0_PREV	(PLB4ARB8M_0_BASE | PREV)
#define PLB4ARB8M_0_PACR	(PLB4ARB8M_0_BASE | PACR)
#define PLB4ARB8M_0_PESR_RC	(PLB4ARB8M_0_BASE | PESR_RC)
#define PLB4ARB8M_0_PESR_W	(PLB4ARB8M_0_BASE | PESR_W)
#define PLB4ARB8M_0_PEARL	(PLB4ARB8M_0_BASE | PEARL)
#define PLB4ARB8M_0_PEARH	(PLB4ARB8M_0_BASE | PEARH)

#define PLB4ARB8M_1_PREV	(PLB4ARB8M_1_BASE | PREV)
#define PLB4ARB8M_1_PACR	(PLB4ARB8M_1_BASE | PACR)
#define PLB4ARB8M_1_PESR_RC	(PLB4ARB8M_1_BASE | PESR_RC)
#define PLB4ARB8M_1_PESR_W	(PLB4ARB8M_1_BASE | PESR_W)
#define PLB4ARB8M_1_PEARL	(PLB4ARB8M_1_BASE | PEARL)
#define PLB4ARB8M_1_PEARH	(PLB4ARB8M_1_BASE | PEARH)

#define PLB4ARB8M_2_PREV	(PLB4ARB8M_2_BASE | PREV)
#define PLB4ARB8M_2_PACR	(PLB4ARB8M_2_BASE | PACR)
#define PLB4ARB8M_2_PESR_RC	(PLB4ARB8M_2_BASE | PESR_RC)
#define PLB4ARB8M_2_PESR_W	(PLB4ARB8M_2_BASE | PESR_W)
#define PLB4ARB8M_2_PEARL	(PLB4ARB8M_2_BASE | PEARL)
#define PLB4ARB8M_2_PEARH	(PLB4ARB8M_2_BASE | PEARH)

/*
#define PLB4ARB8M_3_PREV	(PLB4ARB8M_3_BASE | PREV)
#define PLB4ARB8M_3_PACR	(PLB4ARB8M_3_BASE | PACR)
#define PLB4ARB8M_3_PESR_RC	(PLB4ARB8M_3_BASE | PESR_RC)
#define PLB4ARB8M_3_PESR_W	(PLB4ARB8M_3_BASE | PESR_W)
#define PLB4ARB8M_3_PEARL	(PLB4ARB8M_3_BASE | PEARL)
#define PLB4ARB8M_3_PEARH	(PLB4ARB8M_3_BASE | PEARH)

#define PLB4ARB8M_4_PREV	(PLB4ARB8M_4_BASE | PREV)
#define PLB4ARB8M_4_PACR	(PLB4ARB8M_4_BASE | PACR)
#define PLB4ARB8M_4_PESR_RC	(PLB4ARB8M_4_BASE | PESR_RC)
#define PLB4ARB8M_4_PESR_W	(PLB4ARB8M_4_BASE | PESR_W)
#define PLB4ARB8M_4_PEARL	(PLB4ARB8M_4_BASE | PEARL)
#define PLB4ARB8M_4_PEARH	(PLB4ARB8M_4_BASE | PEARH)

#define PLB4ARB8M_5_PREV    (PLB4ARB8M_5_BASE | PREV)
#define PLB4ARB8M_5_PACR    (PLB4ARB8M_5_BASE | PACR)
#define PLB4ARB8M_5_PESR_RC (PLB4ARB8M_5_BASE | PESR_RC)
#define PLB4ARB8M_5_PESR_W  (PLB4ARB8M_5_BASE | PESR_W)
#define PLB4ARB8M_5_PEARL   (PLB4ARB8M_5_BASE | PEARL)
#define PLB4ARB8M_5_PEARH   (PLB4ARB8M_5_BASE | PEARH)

#define PLB4ARB8M_6_PREV    (PLB4ARB8M_6_BASE | PREV)
#define PLB4ARB8M_6_PACR    (PLB4ARB8M_6_BASE | PACR)
#define PLB4ARB8M_6_PESR_RC (PLB4ARB8M_6_BASE | PESR_RC)
#define PLB4ARB8M_6_PESR_W  (PLB4ARB8M_6_BASE | PESR_W)
#define PLB4ARB8M_6_PEARL   (PLB4ARB8M_6_BASE | PEARL)
#define PLB4ARB8M_6_PEARH   (PLB4ARB8M_6_BASE | PEARH)
*/

#define PREV_RESET_VAL		0x00000301
#define PACR_RESET_VAL		0x0B000000
#define PESR_RESET_VAL		0x00000000
#define PEARL_RESET_VAL		0x00000000
#define PEARH_RESET_VAL		0x00000000

.section ".text","ax",@progbits

.global image_start
.global main

image_start:
main:

check_cpu_ppc_plb4arb_0_2:
    check_value r0,r1,r2,PLB4ARB8M_0_PREV, PREV_RESET_VAL, "PLB4ARB8M_0_PREV"
    check_value r0,r1,r2,PLB4ARB8M_0_PACR, PACR_RESET_VAL, "PLB4ARB8M_0_PACR"
    check_value r0,r1,r2,PLB4ARB8M_0_PESR_RC, PESR_RESET_VAL, "PLB4ARB8M_0_PESR_RC"

check_cpu_ppc_plb4arb_0_3:
    load_const r0, PLB4ARB8M_0_PESR_W
    load_const r1, PLB4ARB8M_0_PESR_RC
    load_const r3, 1 //for shift in running_0(1) checks
    load_const r4, 0x00000001 //start value for writing
    load_const r5, 0x80000000
    load_const r6, 0x00000000
    load_const r7, 0xFFFFFFFF

    mtdcrx r1, r7 //clear all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r6,r2
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_0.PESR not cleared")
plb4arb_0_running_1:
plb4arb_0_loop_1:
    mtdcrx r0, r4 //set bit of PESR
    mfdcrx r2, r0 //read PESR
    cmp cr7,0, r2,r4
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_0_PESR_W (running 1)")
    mtdcrx r1, r4 //clear PESR bit
    cmp 0, 0, r5, r4
    beq plb4arb_0_running_0
    slw r4, r4, r3
    b plb4arb_0_loop_1

plb4arb_0_running_0:
    mtdcrx r0, r7 //set all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_0.PESR (running 0 init)")

    load_const r4, 0x00000001 //start value for writing
plb4arb_0_loop_0:
    xor r8, r7, r4 //r8 = ~r4 (getting running 0 in r8 for compare process)
    mtdcrx r1, r4 //clear one bit of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r8
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_0.PESR (running 0)")
    mtdcrx r0, r7 //set all bits of PESR
    cmp 0, 0, r5, r4
    beq check_cpu_ppc_plb4arb_1_2
    slw r4, r4, r3
    b plb4arb_0_loop_0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

check_cpu_ppc_plb4arb_1_2:
    check_value r0,r1,r2,PLB4ARB8M_1_PREV, PREV_RESET_VAL, "PLB4ARB8M_1_PREV"
    check_value r0,r1,r2,PLB4ARB8M_1_PACR, PACR_RESET_VAL, "PLB4ARB8M_1_PACR"
    check_value r0,r1,r2,PLB4ARB8M_1_PESR_RC, PESR_RESET_VAL, "PLB4ARB8M_1_PESR_RC"

check_cpu_ppc_plb4arb_1_3:
    load_const r0, PLB4ARB8M_1_PESR_W
    load_const r1, PLB4ARB8M_1_PESR_RC
    load_const r3, 1 //for shift in running_0(1) checks
    load_const r4, 0x00000001 //start value for writing
    load_const r5, 0x80000000
    load_const r6, 0x00000000
    load_const r7, 0xFFFFFFFF

    mtdcrx r1, r7 //clear all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r6,r2
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_1.PESR not cleared")
plb4arb_1_running_1:
plb4arb_1_loop_1:
    mtdcrx r0, r4 //set bit of PESR
    mfdcrx r2, r0 //read PESR
    cmp cr7,0, r2,r4
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_1_PESR_W (running 1)")
    mtdcrx r1, r4 //clear PESR bit
    cmp 0, 0, r5, r4
    beq plb4arb_1_running_0
    slw r4, r4, r3
    b plb4arb_1_loop_1

plb4arb_1_running_0:
    mtdcrx r0, r7 //set all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_1.PESR (running 0 init)")

    load_const r4, 0x00000001 //start value for writing
plb4arb_1_loop_0:
    xor r8, r7, r4 //r8 = ~r4 (getting running 0 in r8 for compare process)
    mtdcrx r1, r4 //clear one bit of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r8
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_1.PESR (running 0)")
    mtdcrx r0, r7 //set all bits of PESR
    cmp 0, 0, r5, r4
    beq check_cpu_ppc_plb4arb_2_2
    slw r4, r4, r3
    b plb4arb_1_loop_0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

check_cpu_ppc_plb4arb_2_2:
    check_value r0,r1,r2,PLB4ARB8M_2_PREV, PREV_RESET_VAL, "PLB4ARB8M_2_PREV"
    check_value r0,r1,r2,PLB4ARB8M_2_PACR, PACR_RESET_VAL, "PLB4ARB8M_2_PACR"
    check_value r0,r1,r2,PLB4ARB8M_2_PESR_RC, PESR_RESET_VAL, "PLB4ARB8M_2_PESR_RC"

check_cpu_ppc_plb4arb_2_3:
    load_const r0, PLB4ARB8M_2_PESR_W
    load_const r1, PLB4ARB8M_2_PESR_RC
    load_const r3, 1 //for shift in running_0(1) checks
    load_const r4, 0x00000001 //start value for writing
    load_const r5, 0x80000000
    load_const r6, 0x00000000
    load_const r7, 0xFFFFFFFF

    mtdcrx r1, r7 //clear all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r6,r2
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_2.PESR not cleared")
plb4arb_2_running_1:
plb4arb_2_loop_1:
    mtdcrx r0, r4 //set bit of PESR
    mfdcrx r2, r0 //read PESR
    cmp cr7,0, r2,r4
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_2_PESR_W (running 1)")
    mtdcrx r1, r4 //clear PESR bit
    cmp 0, 0, r5, r4
    beq plb4arb_2_running_0
    slw r4, r4, r3
    b plb4arb_2_loop_1

plb4arb_2_running_0:
    mtdcrx r0, r7 //set all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_2.PESR (running 0 init)")

    load_const r4, 0x00000001 //start value for writing
plb4arb_2_loop_0:
    xor r8, r7, r4 //r8 = ~r4 (getting running 0 in r8 for compare process)
    mtdcrx r1, r4 //clear one bit of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r8
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_2.PESR (running 0)")
    mtdcrx r0, r7 //set all bits of PESR
    cmp 0, 0, r5, r4
    beq check_cpu_ppc_plb4arb_3_2
    slw r4, r4, r3
    b plb4arb_2_loop_0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

check_cpu_ppc_plb4arb_3_2:
//    check_value r0,r1,r2,PLB4ARB8M_3_PREV, PREV_RESET_VAL, "PLB4ARB8M_3_PREV"
//    check_value r0,r1,r2,PLB4ARB8M_3_PACR, PACR_RESET_VAL, "PLB4ARB8M_3_PACR"
//    check_value r0,r1,r2,PLB4ARB8M_3_PESR_RC, PESR_RESET_VAL, "PLB4ARB8M_3_PESR_RC"

check_cpu_ppc_plb4arb_3_3:
/*
    load_const r0, PLB4ARB8M_3_PESR_W
    load_const r1, PLB4ARB8M_3_PESR_RC
    load_const r3, 1 //for shift in running_0(1) checks
    load_const r4, 0x00000001 //start value for writing
    load_const r5, 0x80000000
    load_const r6, 0x00000000
    load_const r7, 0xFFFFFFFF


    mtdcrx r1, r7 //clear all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r6,r2
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_3.PESR not cleared")
*/
plb4arb_3_running_1:
plb4arb_3_loop_1:
/*
    mtdcrx r0, r4 //set bit of PESR
    mfdcrx r2, r0 //read PESR
    cmp cr7,0, r2,r4
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_3_PESR_W (running 1)")
    mtdcrx r1, r4 //clear PESR bit
    cmp 0, 0, r5, r4
    beq plb4arb_3_running_0
    slw r4, r4, r3
    b plb4arb_3_loop_1
*/

plb4arb_3_running_0:
/*
    mtdcrx r0, r7 //set all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_3.PESR (running 0 init)")

    load_const r4, 0x00000001 //start value for writing
*/
plb4arb_3_loop_0:
/*
    xor r8, r7, r4 //r8 = ~r4 (getting running 0 in r8 for compare process)
    mtdcrx r1, r4 //clear one bit of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r8
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_3.PESR (running 0)")
    mtdcrx r0, r7 //set all bits of PESR
    cmp 0, 0, r5, r4
    beq check_cpu_ppc_plb4arb_4_2
    slw r4, r4, r3
    b plb4arb_3_loop_0
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

check_cpu_ppc_plb4arb_4_2:
/*
    check_value r0,r1,r2,PLB4ARB8M_4_PREV, PREV_RESET_VAL, "PLB4ARB8M_4_PREV"
    check_value r0,r1,r2,PLB4ARB8M_4_PACR, PACR_RESET_VAL, "PLB4ARB8M_4_PACR"
    check_value r0,r1,r2,PLB4ARB8M_4_PESR_RC, PESR_RESET_VAL, "PLB4ARB8M_4_PESR_RC"
*/

check_cpu_ppc_plb4arb_4_3:
/*
    load_const r0, PLB4ARB8M_4_PESR_W
    load_const r1, PLB4ARB8M_4_PESR_RC
    load_const r3, 1 //for shift in running_0(1) checks
    load_const r4, 0x00000001 //start value for writing
    load_const r5, 0x80000000
    load_const r6, 0x00000000
    load_const r7, 0xFFFFFFFF

    mtdcrx r1, r7 //clear all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r6,r2
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_4.PESR not cleared")
*/
plb4arb_4_running_1:
plb4arb_4_loop_1:
/*
    mtdcrx r0, r4 //set bit of PESR
    mfdcrx r2, r0 //read PESR
    cmp cr7,0, r2,r4
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_4_PESR_W (running 1)")
    mtdcrx r1, r4 //clear PESR bit
    cmp 0, 0, r5, r4
    beq plb4arb_4_running_0
    slw r4, r4, r3
    b plb4arb_4_loop_1
*/

plb4arb_4_running_0:
/*
    mtdcrx r0, r7 //set all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_4.PESR (running 0 init)")

    load_const r4, 0x00000001 //start value for writing
*/
plb4arb_4_loop_0:
/*
    xor r8, r7, r4 //r8 = ~r4 (getting running 0 in r8 for compare process)
    mtdcrx r1, r4 //clear one bit of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r8
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_4.PESR (running 0)")
    mtdcrx r0, r7 //set all bits of PESR
    cmp 0, 0, r5, r4
    beq test_ok
    slw r4, r4, r3
    b plb4arb_4_loop_0
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

check_cpu_ppc_plb4arb_5_2:
/*
    check_value r0,r1,r2,PLB4ARB8M_5_PREV, PREV_RESET_VAL, "PLB4ARB8M_5_PREV"
    check_value r0,r1,r2,PLB4ARB8M_5_PACR, PACR_RESET_VAL, "PLB4ARB8M_5_PACR"
    check_value r0,r1,r2,PLB4ARB8M_5_PESR_RC, PESR_RESET_VAL, "PLB4ARB8M_5_PESR_RC"
*/
check_cpu_ppc_plb4arb_5_3:
/*
    load_const r0, PLB4ARB8M_5_PESR_W
    load_const r1, PLB4ARB8M_5_PESR_RC
    load_const r3, 1 //for shift in running_0(1) checks
    load_const r4, 0x00000001 //start value for writing
    load_const r5, 0x80000000
    load_const r6, 0x00000000
    load_const r7, 0xFFFFFFFF

    mtdcrx r1, r7 //clear all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r6,r2
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_5.PESR not cleared")
*/
plb4arb_5_running_1:
plb4arb_5_loop_1:
/*
    mtdcrx r0, r4 //set bit of PESR
    mfdcrx r2, r0 //read PESR
    cmp cr7,0, r2,r4
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_5_PESR_W (running 1)")
    mtdcrx r1, r4 //clear PESR bit
    cmp 0, 0, r5, r4
    beq plb4arb_5_running_0
    slw r4, r4, r3
    b plb4arb_5_loop_1
*/

plb4arb_5_running_0:
/*
    mtdcrx r0, r7 //set all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_5.PESR (running 0 init)")

    load_const r4, 0x00000001 //start value for writing
*/
plb4arb_5_loop_0:
/*
    xor r8, r7, r4 //r8 = ~r4 (getting running 0 in r8 for compare process)
    mtdcrx r1, r4 //clear one bit of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r8
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_5.PESR (running 0)")
    mtdcrx r0, r7 //set all bits of PESR
    cmp 0, 0, r5, r4
    beq test_ok
    slw r4, r4, r3
    b plb4arb_5_loop_0
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

check_cpu_ppc_plb4arb_6_2:
/*
    check_value r0,r1,r2,PLB4ARB8M_6_PREV, PREV_RESET_VAL, "PLB4ARB8M_6_PREV"
    check_value r0,r1,r2,PLB4ARB8M_6_PACR, PACR_RESET_VAL, "PLB4ARB8M_6_PACR"
    check_value r0,r1,r2,PLB4ARB8M_6_PESR_RC, PESR_RESET_VAL, "PLB4ARB8M_6_PESR_RC"
*/

check_cpu_ppc_plb4arb_6_3:
/*
    load_const r0, PLB4ARB8M_6_PESR_W
    load_const r1, PLB4ARB8M_6_PESR_RC
    load_const r3, 1 //for shift in running_0(1) checks
    load_const r4, 0x00000001 //start value for writing
    load_const r5, 0x80000000
    load_const r6, 0x00000000
    load_const r7, 0xFFFFFFFF

    mtdcrx r1, r7 //clear all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r6,r2
    TEST_ASSERT(eq,cr7,"Bits in PLB4ARB8M_6.PESR not cleared")
*/
plb4arb_6_running_1:
plb4arb_6_loop_1:
/*
    mtdcrx r0, r4 //set bit of PESR
    mfdcrx r2, r0 //read PESR
    cmp cr7,0, r2,r4
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_6_PESR_W (running 1)")
    mtdcrx r1, r4 //clear PESR bit
    cmp 0, 0, r5, r4
    beq plb4arb_6_running_0
    slw r4, r4, r3
    b plb4arb_6_loop_1
*/
plb4arb_6_running_0:
/*
    mtdcrx r0, r7 //set all bits of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r7
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_6.PESR (running 0 init)")

    load_const r4, 0x00000001 //start value for writing
*/
plb4arb_6_loop_0:
/*
    xor r8, r7, r4 //r8 = ~r4 (getting running 0 in r8 for compare process)
    mtdcrx r1, r4 //clear one bit of PESR
    mfdcrx r2, r1 //read PESR
    cmp cr7,0, r2,r8
    TEST_ASSERT(eq,cr7,"Value mismatch: PLB4ARB8M_6.PESR (running 0)")
    mtdcrx r0, r7 //set all bits of PESR
    cmp 0, 0, r5, r4
    beq test_ok
    slw r4, r4, r3
    b plb4arb_6_loop_0
*/

test_ok:
    test_event EVENT_OK
    li r3, 0x00
    b finish

error:
     test_event EVENT_ERROR
     li r3, 0x01

finish:
	blr
    b finish

