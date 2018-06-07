#include <platform/trace.S.h>
#include <platform/test_assert.S.h>
#include <platform/test_event_asm.h>
#include <platform/devices.h>

#define DEBUG_DCR_TEST
#include <platform/arch/ppc/test_macro_asm.S.h>

/* MPIC Registers */
#include <platform/regs/regs_mpic128.h>


.section ".text","ax",@progbits

.global main

main:
    /* reset mpic */
    load_const r5, MPIC128_R_BIT
    load_const r4, DCR_MPIC128_BASE + MPIC128_GCF0
    mtdcrx r4, r5

read_MPICx_GCF0_R_BIT:
    mfdcrx r6, r4
    cmp cr0,0, r6,r4
    beq read_MPICx_GCF0_R_BIT

check_cpu_ppc_mpic_3:
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_VP0,    MPIC128_VP0_VALUE,   "MPIC128_VP0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_DST0,   MPIC128_DST0_VALUE,  "MPIC128_DST0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CTP0,   MPIC128_CTP0_VALUE,  "MPIC128_CTP0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CTP1,   MPIC128_CTP1_VALUE,  "MPIC128_CTP1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CTP2,   MPIC128_CTP2_VALUE,  "MPIC128_CTP2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CTP3,   MPIC128_CTP3_VALUE,  "MPIC128_CTP3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_NCIAR0, MPIC128_NCIAR0_VALUE,"MPIC128_NCIAR0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_NCIAR1, MPIC128_NCIAR1_VALUE,"MPIC128_NCIAR1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_NCIAR2, MPIC128_NCIAR2_VALUE,"MPIC128_NCIAR2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_NCIAR3, MPIC128_NCIAR3_VALUE,"MPIC128_NCIAR3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_NCEOI0, MPIC128_NCEOI0_VALUE,"MPIC128_NCEOI0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_NCEOI1, MPIC128_NCEOI1_VALUE,"MPIC128_NCEOI1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_NCEOI2, MPIC128_NCEOI2_VALUE,"MPIC128_NCEOI2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_NCEOI3, MPIC128_NCEOI3_VALUE,"MPIC128_NCEOI3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CIAR0,  MPIC128_CIAR0_VALUE, "MPIC128_CIAR0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CIAR1,  MPIC128_CIAR1_VALUE, "MPIC128_CIAR1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CIAR2,  MPIC128_CIAR2_VALUE, "MPIC128_CIAR2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CIAR3,  MPIC128_CIAR3_VALUE, "MPIC128_CIAR3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CEOI0,  MPIC128_CEOI0_VALUE, "MPIC128_CEOI0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CEOI1,  MPIC128_CEOI1_VALUE, "MPIC128_CEOI1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CEOI2,  MPIC128_CEOI2_VALUE, "MPIC128_CEOI2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_CEOI3,  MPIC128_CEOI3_VALUE, "MPIC128_CEOI3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_IAR0,   MPIC128_IAR0_VALUE,  "MPIC128_IAR0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_IAR1,   MPIC128_IAR1_VALUE,  "MPIC128_IAR1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_IAR2,   MPIC128_IAR2_VALUE,  "MPIC128_IAR2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_IAR3,   MPIC128_IAR3_VALUE,  "MPIC128_IAR3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_EOI0,   MPIC128_EOI0_VALUE,  "MPIC128_EOI0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_EOI1,   MPIC128_EOI1_VALUE,  "MPIC128_EOI1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_EOI2,   MPIC128_EOI2_VALUE,  "MPIC128_EOI2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_EOI3,   MPIC128_EOI3_VALUE,  "MPIC128_EOI3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_FRG0,   MPIC128_FRG0_VALUE,  "MPIC128_FRG0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_GCF0,   MPIC128_GCF0_VALUE,  "MPIC128_GCF0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_VITC,   MPIC128_VITC_VALUE,  "MPIC128_VITC"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_VRID,   MPIC128_VRID_VALUE,  "MPIC128_VRID"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_PINI,   MPIC128_PINI_VALUE,  "MPIC128_PINI"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_IVP0,   MPIC128_IVP0_VALUE,  "MPIC128_IVP0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_IVP1,   MPIC128_IVP1_VALUE,  "MPIC128_IVP1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_IVP2,   MPIC128_IVP2_VALUE,  "MPIC128_IVP1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_IVP3,   MPIC128_IVP3_VALUE,  "MPIC128_IVP3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_SPV,    MPIC128_SPV_VALUE,   "MPIC128_SPV"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TFR,    MPIC128_TFR_VALUE,   "MPIC128_TFR"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TCC0,   MPIC128_TCC0_VALUE,  "MPIC128_TCC0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TCC1,   MPIC128_TCC1_VALUE,  "MPIC128_TCC1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TCC2,   MPIC128_TCC2_VALUE,  "MPIC128_TCC2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TCC3,   MPIC128_TCC3_VALUE,  "MPIC128_TCC3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TBC0,   MPIC128_TBC0_VALUE,  "MPIC128_TBC0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TBC1,   MPIC128_TBC1_VALUE,  "MPIC128_TBC1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TBC2,   MPIC128_TBC2_VALUE,  "MPIC128_TBC2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TBC3,   MPIC128_TBC3_VALUE,  "MPIC128_TBC3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TVP0,   MPIC128_TVP0_VALUE,  "MPIC128_TVP0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TVP1,   MPIC128_TVP1_VALUE,  "MPIC128_TVP1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TVP2,   MPIC128_TVP2_VALUE,  "MPIC128_TVP2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TVP3,   MPIC128_TVP3_VALUE,  "MPIC128_TVP3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TDST0,  MPIC128_TDST0_VALUE, "MPIC128_TDST0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TDST1,  MPIC128_TDST1_VALUE, "MPIC128_TDST1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TDST2,  MPIC128_TDST2_VALUE, "MPIC128_TDST2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + MPIC128_TDST3,  MPIC128_TDST3_VALUE, "MPIC128_TDST3"

check_cpu_ppc_mpic_4_5:
    load_const r5, 0x00000000
    load_const r4, DCR_MPIC128_BASE + MPIC128_TFR
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR")

running_1:
    load_const r5, 0x00000001
    load_const r7, 1
    load_const r8, 0x80000000
loop_1:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR (running 1)")
    cmp cr0, 0, r5, r8
    beq running_0
    slw r5, r5, r7
    b loop_1

running_0:
    load_const r5, 0xFFFFFFFF
    load_const r4, DCR_MPIC128_BASE + MPIC128_TFR
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR (running 0)")

    load_const r5, 0xFFFFFFFE
    load_const r7, 1
    load_const r8, 0x7FFFFFFF
loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR (running 0)")
    cmp cr0, 0, r5, r8
    beq test_ok
    slw r5, r5, r7
    ori r5, r5, 1
    b loop_0

test_ok:
	rumboot_putstring "TEST OK\n"
	test_event		EVENT_OK
	load_const r3,	RESULT_OK
	b finish

test_error:
	rumboot_putstring "TEST ERROR\n"
	test_event		EVENT_ERROR
	load_const r3,	RESULT_ERROR

finish:
	blr


