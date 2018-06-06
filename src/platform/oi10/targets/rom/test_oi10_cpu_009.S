
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
// #include <mivem_mem_map.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>

// #include "test_mpw_cpu_009_0.h"
#define P64CR				0x0
#define ESR					0x1
#define P64REV				0x2
#define EARH				0x3
#define EARL				0x4
#define ADDRL0				0x5
#define ADDRH0				0x6
#define ADDRL1				0x7
#define ADDRH1				0x8
#define TESR				0x9

#define PLB6PLB4_0_BASE DCR_PLB6PLB4_0_BASE
#define PLB6PLB4_1_BASE DCR_PLB6PLB4_1_BASE
#define PLB6PLB4_2_BASE DCR_PLB6PLB4_2_BASE
#define PLB6PLB4_3_BASE DCR_PLB6PLB4_3_BASE

#define PLB6PLB4_0_P64CR	(PLB6PLB4_0_BASE | P64CR)
#define PLB6PLB4_0_ESR		(PLB6PLB4_0_BASE | ESR)
#define PLB6PLB4_0_P64REV	(PLB6PLB4_0_BASE | P64REV)
#define PLB6PLB4_0_EARH		(PLB6PLB4_0_BASE | EARH)
#define PLB6PLB4_0_EARL		(PLB6PLB4_0_BASE | EARL)
#define PLB6PLB4_0_ADDRL0	(PLB6PLB4_0_BASE | ADDRL0)
#define PLB6PLB4_0_ADDRH0	(PLB6PLB4_0_BASE | ADDRH0)
#define PLB6PLB4_0_ADDRL1	(PLB6PLB4_0_BASE | ADDRL1)
#define PLB6PLB4_0_ADDRH1	(PLB6PLB4_0_BASE | ADDRH1)
#define PLB6PLB4_0_TESR		(PLB6PLB4_0_BASE | TESR)

#define PLB6PLB4_1_P64CR	(PLB6PLB4_1_BASE | P64CR)
#define PLB6PLB4_1_ESR		(PLB6PLB4_1_BASE | ESR)
#define PLB6PLB4_1_P64REV	(PLB6PLB4_1_BASE | P64REV)
#define PLB6PLB4_1_EARH		(PLB6PLB4_1_BASE | EARH)
#define PLB6PLB4_1_EARL		(PLB6PLB4_1_BASE | EARL)
#define PLB6PLB4_1_ADDRL0	(PLB6PLB4_1_BASE | ADDRL0)
#define PLB6PLB4_1_ADDRH0	(PLB6PLB4_1_BASE | ADDRH0)
#define PLB6PLB4_1_ADDRL1	(PLB6PLB4_1_BASE | ADDRL1)
#define PLB6PLB4_1_ADDRH1	(PLB6PLB4_1_BASE | ADDRH1)
#define PLB6PLB4_1_TESR		(PLB6PLB4_1_BASE | TESR)

#define PLB6PLB4_2_P64CR    (PLB6PLB4_2_BASE | P64CR)
#define PLB6PLB4_2_ESR      (PLB6PLB4_2_BASE | ESR)
#define PLB6PLB4_2_P64REV   (PLB6PLB4_2_BASE | P64REV)
#define PLB6PLB4_2_EARH     (PLB6PLB4_2_BASE | EARH)
#define PLB6PLB4_2_EARL     (PLB6PLB4_2_BASE | EARL)
#define PLB6PLB4_2_ADDRL0   (PLB6PLB4_2_BASE | ADDRL0)
#define PLB6PLB4_2_ADDRH0   (PLB6PLB4_2_BASE | ADDRH0)
#define PLB6PLB4_2_ADDRL1   (PLB6PLB4_2_BASE | ADDRL1)
#define PLB6PLB4_2_ADDRH1   (PLB6PLB4_2_BASE | ADDRH1)
#define PLB6PLB4_2_TESR     (PLB6PLB4_2_BASE | TESR)

#define PLB6PLB4_3_P64CR    (PLB6PLB4_3_BASE | P64CR)
#define PLB6PLB4_3_ESR      (PLB6PLB4_3_BASE | ESR)
#define PLB6PLB4_3_P64REV   (PLB6PLB4_3_BASE | P64REV)
#define PLB6PLB4_3_EARH     (PLB6PLB4_3_BASE | EARH)
#define PLB6PLB4_3_EARL     (PLB6PLB4_3_BASE | EARL)
#define PLB6PLB4_3_ADDRL0   (PLB6PLB4_3_BASE | ADDRL0)
#define PLB6PLB4_3_ADDRH0   (PLB6PLB4_3_BASE | ADDRH0)
#define PLB6PLB4_3_ADDRL1   (PLB6PLB4_3_BASE | ADDRL1)
#define PLB6PLB4_3_ADDRH1   (PLB6PLB4_3_BASE | ADDRH1)
#define PLB6PLB4_3_TESR     (PLB6PLB4_3_BASE | TESR)

#define PLB6PLB4_WATERMARK_VALUE 0x00000108 //from /dut_hlb_cpu_v1.02/trunk/cpu/units/plb6plb4_v001/verilog/rtl/PLB6PLB4.defines.v
//from http://svn.module.ru/r42_mm7705/mm7705/trunk/toplevel/verilog/rtl/v1/mpw7705_const.vh
//"STRAP_" +define
#define STRAP_PLB6PLB4_0_ADDRL0 0x000003FF
#define STRAP_PLB6PLB4_0_ADDRH0 0x000003FF
#define STRAP_PLB6PLB4_1_ADDRL0 0x00000010
#define STRAP_PLB6PLB4_1_ADDRH0 0x00000010
#define STRAP_PLB6PLB41_I_ADDRL0  0x00000012
#define STRAP_PLB6PLB41_I_ADDRH0  0x00000012
#define STRAP_PLB6PLB42_I_ADDRL0  0x00000013
#define STRAP_PLB6PLB42_I_ADDRH0  0x00000013


#define P64CR_0_RESET_VAL               0x00000303//due configuration
#define P64CR_RESET_VAL					0x00000203
#define ESR_RESET_VAL					0x00000000
#define P64REV_RESET_VAL				PLB6PLB4_WATERMARK_VALUE
#define EARH_RESET_VAL					0x00000000
#define EARL_RESET_VAL					0x00000000
#define PLB6PLB4_0_ADDRL0_RESET_VAL		STRAP_PLB6PLB4_0_ADDRL0
#define PLB6PLB4_0_ADDRH0_RESET_VAL		STRAP_PLB6PLB4_0_ADDRH0

#define PLB6PLB4_1_ADDRL0_RESET_VAL		STRAP_PLB6PLB4_1_ADDRL0
#define PLB6PLB4_1_ADDRH0_RESET_VAL		STRAP_PLB6PLB4_1_ADDRH0

#define PLB6PLB4_2_ADDRL0_RESET_VAL     STRAP_PLB6PLB41_I_ADDRL0
#define PLB6PLB4_2_ADDRH0_RESET_VAL     STRAP_PLB6PLB41_I_ADDRH0

#define PLB6PLB4_3_ADDRL0_RESET_VAL     STRAP_PLB6PLB42_I_ADDRL0
#define PLB6PLB4_3_ADDRH0_RESET_VAL     STRAP_PLB6PLB42_I_ADDRH0
#define ADDRL1_0_RESET_VAL              0x00000011
#define ADDRH1_0_RESET_VAL              0x00000011
#define ADDRL1_RESET_VAL				0x00000000
#define ADDRH1_RESET_VAL				0x00000000
#define TESR_RESET_VAL					0x00000000

.section ".text","ax",@progbits

.global image_start
.global main

image_start:
main:

check_cpu_ppc_plb6plb4_0_2:
     check_value r10,r11,r12, PLB6PLB4_0_P64CR, P64CR_0_RESET_VAL, "PLB6PLB4_0_P64CR"
     check_value r10,r11,r12, PLB6PLB4_0_ESR, ESR_RESET_VAL, "PLB6PLB4_0_ESR"
     check_value r10,r11,r12, PLB6PLB4_0_P64REV, P64REV_RESET_VAL,"PLB6PLB4_0_P64REV"
     check_value r10,r11,r12, PLB6PLB4_0_EARH, EARH_RESET_VAL,"PLB6PLB4_0_EARH"
     check_value r10,r11,r12, PLB6PLB4_0_EARL, EARL_RESET_VAL, "PLB6PLB4_0_EARL"
     check_value r10,r11,r12, PLB6PLB4_0_ADDRL0, PLB6PLB4_0_ADDRL0_RESET_VAL, "PLB6PLB4_0_ADDRL0"
     check_value r10,r11,r12, PLB6PLB4_0_ADDRH0, PLB6PLB4_0_ADDRH0_RESET_VAL, "PLB6PLB4_0_ADDRH0"
     check_value r10,r11,r12, PLB6PLB4_0_ADDRL1, ADDRL1_0_RESET_VAL, "PLB6PLB4_0_ADDRL1"
     check_value r10,r11,r12, PLB6PLB4_0_ADDRH1, ADDRH1_0_RESET_VAL, "PLB6PLB4_0_ADDRH1"
     check_value r10,r11,r12, PLB6PLB4_0_TESR, TESR_RESET_VAL, "PLB6PLB4_0_TESR"

check_cpu_ppc_plb6plb4_0_3:
    load_const r11, 0x00000000
    load_const r10, PLB6PLB4_0_ADDRL1
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL0")

plb6plb4_0_running_1:
    load_const r11, 0x00000001
    load_const r3, 1
    load_const r4, 0x80000000
plb6plb4_0_loop_1:
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL1 (running 1)")
    cmp 0, 0, r11, r4
    beq plb6plb4_0_running_0
    slw r11, r11, r3
    b plb6plb4_0_loop_1

plb6plb4_0_running_0:
    load_const r11, 0xFFFFFFFF
    load_const r10, PLB6PLB4_0_ADDRL1
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL1 (running 0)")

    load_const r11, 0xFFFFFFFE
    load_const r3, 1
    load_const r4, 0x7FFFFFFF
plb6plb4_0_loop_0:
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL1 (running 0)")
    cmp 0, 0, r11, r4
    beq end_plb6plb4_0
    slw r11, r11, r3
    ori r11, r11, 1
    b plb6plb4_0_loop_0
end_plb6plb4_0:

check_cpu_ppc_plb6plb4_1_2:
     check_value r10,r11,r12, PLB6PLB4_1_P64CR, P64CR_RESET_VAL, "PLB6PLB4_1_P64CR_reg"
     check_value r10,r11,r12, PLB6PLB4_1_ESR, ESR_RESET_VAL, "PLB6PLB4_1_ESR_reg"
     check_value r10,r11,r12, PLB6PLB4_1_P64REV, P64REV_RESET_VAL, "PLB6PLB4_1_P64REV_reg"
     check_value r10,r11,r12, PLB6PLB4_1_EARH, EARH_RESET_VAL, "PLB6PLB4_1_EARH_reg"
     check_value r10,r11,r12, PLB6PLB4_1_EARL, EARL_RESET_VAL, "PLB6PLB4_1_EARL_reg"
     check_value r10,r11,r12, PLB6PLB4_1_ADDRL0, PLB6PLB4_1_ADDRL0_RESET_VAL, "PLB6PLB4_1_ADDRL0_reg"
     check_value r10,r11,r12, PLB6PLB4_1_ADDRH0, PLB6PLB4_1_ADDRH0_RESET_VAL, "PLB6PLB4_1_ADDRH0_reg"
     check_value r10,r11,r12, PLB6PLB4_1_ADDRL1, ADDRL1_RESET_VAL, "PLB6PLB4_1_ADDRL1_reg"
     check_value r10,r11,r12, PLB6PLB4_1_ADDRH1, ADDRH1_RESET_VAL,"PLB6PLB4_1_ADDRH1_reg"
     check_value r10,r11,r12, PLB6PLB4_1_TESR, TESR_RESET_VAL, "PLB6PLB4_1_TESR_reg"

check_cpu_ppc_plb6plb4_1_3:
    load_const r11, 0x00000000
    load_const r10, PLB6PLB4_1_ADDRL1
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1")

plb6plb4_1_running_1:
    load_const r11, 0x00000001
    load_const r3, 1
    load_const r4, 0x80000000
plb6plb4_1_loop_1:
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1 (running 1)")
    cmp 0, 0, r11, r4
    beq plb6plb4_1_running_0
    slw r11, r11, r3
    b plb6plb4_1_loop_1

plb6plb4_1_running_0:
    load_const r11, 0xFFFFFFFF
    load_const r10, PLB6PLB4_1_ADDRL1
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1 (running 0)")

    load_const r11, 0xFFFFFFFE
    load_const r3, 1
    load_const r4, 0x7FFFFFFF
plb6plb4_1_loop_0:
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1 (running 0)")
    cmp 0, 0, r11, r4
    beq end_plb6plb4_1
    slw r11, r11, r3
    ori r11, r11, 1
    b plb6plb4_1_loop_0
end_plb6plb4_1:

check_cpu_ppc_plb6plb4_2_2:
/*
     check_value r10,r11,r12, PLB6PLB4_2_P64CR, P64CR_RESET_VAL, "PLB6PLB4_2_P64CR_reg"
     check_value r10,r11,r12, PLB6PLB4_2_ESR, ESR_RESET_VAL, "PLB6PLB4_2_ESR_reg"
     check_value r10,r11,r12, PLB6PLB4_2_P64REV, P64REV_RESET_VAL, "PLB6PLB4_2_P64REV_reg"
     check_value r10,r11,r12, PLB6PLB4_2_EARH, EARH_RESET_VAL, "PLB6PLB4_2_EARH_reg"
     check_value r10,r11,r12, PLB6PLB4_2_EARL, EARL_RESET_VAL, "PLB6PLB4_2_EARL_reg"
     check_value r10,r11,r12, PLB6PLB4_2_ADDRL0, PLB6PLB4_2_ADDRL0_RESET_VAL, "PLB6PLB4_2_ADDRL0_reg"
     check_value r10,r11,r12, PLB6PLB4_2_ADDRH0, PLB6PLB4_2_ADDRH0_RESET_VAL, "PLB6PLB4_2_ADDRH0_reg"
     check_value r10,r11,r12, PLB6PLB4_2_ADDRL1, ADDRL1_RESET_VAL, "PLB6PLB4_2_ADDRL1_reg"
     check_value r10,r11,r12, PLB6PLB4_2_ADDRH1, ADDRH1_RESET_VAL, "PLB6PLB4_2_ADDRH1_reg"
     check_value r10,r11,r12, PLB6PLB4_2_TESR, TESR_RESET_VAL, "PLB6PLB4_2_TESR_reg"
*/

check_cpu_ppc_plb6plb4_2_3:

/*    load_const r11, 0x00000000
    load_const r10, PLB6PLB4_2_ADDRL1
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_2_ADDRL1")
*/

plb6plb4_2_running_1:
/*
    load_const r11, 0x00000001
    load_const r3, 1
    load_const r4, 0x80000000
*/
plb6plb4_2_loop_1:
/*
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_2_ADDRL1 (running 1)")
    cmp 0, 0, r11, r4
    beq plb6plb4_2_running_0
    slw r11, r11, r3
    b plb6plb4_2_loop_1
*/

plb6plb4_2_running_0:
/*
    load_const r11, 0xFFFFFFFF
    load_const r10, PLB6PLB4_2_ADDRL1
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_2_ADDRL1 (running 0)")

    load_const r11, 0xFFFFFFFE
    load_const r3, 1
    load_const r4, 0x7FFFFFFF
*/
plb6plb4_2_loop_0:
/*
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_2_ADDRL1 (running 0)")
    cmp 0, 0, r11, r4
    beq end_plb6plb4_2
    slw r11, r11, r3
    ori r11, r11, 1
    b plb6plb4_2_loop_0
*/
end_plb6plb4_2:

check_cpu_ppc_plb6plb4_3_2:
/*
     check_value r10,r11,r12, PLB6PLB4_3_P64CR, P64CR_RESET_VAL, "PLB6PLB4_3_P64CR_reg"
     check_value r10,r11,r12, PLB6PLB4_3_ESR, ESR_RESET_VAL, "PLB6PLB4_3_ESR_reg"
     check_value r10,r11,r12, PLB6PLB4_3_P64REV, P64REV_RESET_VAL, "PLB6PLB4_3_P64REV_reg"
     check_value r10,r11,r12, PLB6PLB4_3_EARH, EARH_RESET_VAL, "PLB6PLB4_3_EARH_reg"
     check_value r10,r11,r12, PLB6PLB4_3_EARL, EARL_RESET_VAL, "PLB6PLB4_3_EARL_reg"
     check_value r10,r11,r12, PLB6PLB4_3_ADDRL0, PLB6PLB4_3_ADDRL0_RESET_VAL, "PLB6PLB4_3_ADDRL0_reg"
     check_value r10,r11,r12, PLB6PLB4_3_ADDRH0, PLB6PLB4_3_ADDRH0_RESET_VAL, "PLB6PLB4_3_ADDRH0_reg"
     check_value r10,r11,r12, PLB6PLB4_3_ADDRL1, ADDRL1_RESET_VAL, "PLB6PLB4_3_ADDRL1_reg"
     check_value r10,r11,r12, PLB6PLB4_3_ADDRH1, ADDRH1_RESET_VAL, "PLB6PLB4_3_ADDRH1_reg"
     check_value r10,r11,r12, PLB6PLB4_3_TESR, TESR_RESET_VAL, "PLB6PLB4_3_TESR_reg"
*/
check_cpu_ppc_plb6plb4_3_3:
/*
    load_const r11, 0x00000000
    load_const r10, PLB6PLB4_3_ADDRL1
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_3_ADDRL1")
*/

plb6plb4_3_running_1:
/*
    load_const r11, 0x00000001
    load_const r3, 1
    load_const r4, 0x80000000
*/
plb6plb4_3_loop_1:
/*
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_3_ADDRL1 (running 1)")
    cmp 0, 0, r11, r4
    beq plb6plb4_3_running_0
    slw r11, r11, r3
    b plb6plb4_3_loop_1
*/
plb6plb4_3_running_0:
/*
    load_const r11, 0xFFFFFFFF
    load_const r10, PLB6PLB4_3_ADDRL1
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_3_ADDRL1 (running 0)")

    load_const r11, 0xFFFFFFFE
    load_const r3, 1
    load_const r4, 0x7FFFFFFF
*/
plb6plb4_3_loop_0:
/*
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_3_ADDRL1 (running 0)")
    cmp 0, 0, r11, r4
    beq end_plb6plb4_3
    slw r11, r11, r3
    ori r11, r11, 1
    b plb6plb4_3_loop_0
*/
end_plb6plb4_3:

test_ok:
    //load_const r11, PLB6PLB4_0_ADDRL0_RESET_VAL
    //load_const r10, PLB6PLB4_0_ADDRL0
    //mtdcrx r10, r11
    //load_const r10, PLB6PLB4_1_ADDRL0
    //mtdcrx r10, r11

    test_event EVENT_OK
    li r3, 0x00
    b finish

error:
	test_event EVENT_ERROR
	li r3, 0x01

finish:
	blr
    // b finish

