#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>

/* MPIC Registers */
#include <platform/oi10/platform/regs/regs_mpic128.h>


.section ".text","ax",@progbits

.global image_start
.global main

main:
image_start:
    /* reset mpic */
    load_const r5, MPIC128_R_BIT
    load_const r4, DCR_MPIC128_BASE + GCF0
    mtdcrx r4, r5

read_MPICx_GCF0_R_BIT:
    mfdcrx r6, r4
    cmp cr0,0, r6,r4
    beq read_MPICx_GCF0_R_BIT

check_cpu_ppc_mpic_3:
     check_value r4,r5,r6, DCR_MPIC128_BASE + VP0,    MPIC128_VP0_VALUE,   "DCR_MPIC128_BASE + VP0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + DST0,   MPIC128_DST0_VALUE,  "DCR_MPIC128_BASE + DST0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CTP0,   MPIC128_CTP0_VALUE,  "DCR_MPIC128_BASE + CTP0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CTP1,   MPIC128_CTP1_VALUE,  "DCR_MPIC128_BASE + CTP1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CTP2,   MPIC128_CTP2_VALUE,  "DCR_MPIC128_BASE + CTP2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CTP3,   MPIC128_CTP3_VALUE,  "DCR_MPIC128_BASE + CTP3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + NCIAR0, MPIC128_NCIAR0_VALUE,"DCR_MPIC128_BASE + NCIAR0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + NCIAR1, MPIC128_NCIAR1_VALUE,"DCR_MPIC128_BASE + NCIAR1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + NCIAR2, MPIC128_NCIAR2_VALUE,"DCR_MPIC128_BASE + NCIAR2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + NCIAR3, MPIC128_NCIAR3_VALUE,"DCR_MPIC128_BASE + NCIAR3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + NCEOI0, MPIC128_NCEOI0_VALUE,"DCR_MPIC128_BASE + NCEOI0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + NCEOI1, MPIC128_NCEOI1_VALUE,"DCR_MPIC128_BASE + NCEOI1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + NCEOI2, MPIC128_NCEOI2_VALUE,"DCR_MPIC128_BASE + NCEOI2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + NCEOI3, MPIC128_NCEOI3_VALUE,"DCR_MPIC128_BASE + NCEOI3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CIAR0,  MPIC128_CIAR0_VALUE, "DCR_MPIC128_BASE + CIAR0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CIAR1,  MPIC128_CIAR1_VALUE, "DCR_MPIC128_BASE + CIAR1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CIAR2,  MPIC128_CIAR2_VALUE, "DCR_MPIC128_BASE + CIAR2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CIAR3,  MPIC128_CIAR3_VALUE, "DCR_MPIC128_BASE + CIAR3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CEOI0,  MPIC128_CEOI0_VALUE, "DCR_MPIC128_BASE + CEOI0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CEOI1,  MPIC128_CEOI1_VALUE, "DCR_MPIC128_BASE + CEOI1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CEOI2,  MPIC128_CEOI2_VALUE, "DCR_MPIC128_BASE + CEOI2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + CEOI3,  MPIC128_CEOI3_VALUE, "DCR_MPIC128_BASE + CEOI3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + IAR0,   MPIC128_IAR0_VALUE,  "DCR_MPIC128_BASE + IAR0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + IAR1,   MPIC128_IAR1_VALUE,  "DCR_MPIC128_BASE + IAR1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + IAR2,   MPIC128_IAR2_VALUE,  "DCR_MPIC128_BASE + IAR2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + IAR3,   MPIC128_IAR3_VALUE,  "DCR_MPIC128_BASE + IAR3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + EOI0,   MPIC128_EOI0_VALUE,  "DCR_MPIC128_BASE + EOI0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + EOI1,   MPIC128_EOI1_VALUE,  "DCR_MPIC128_BASE + EOI1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + EOI2,   MPIC128_EOI2_VALUE,  "DCR_MPIC128_BASE + EOI2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + EOI3,   MPIC128_EOI3_VALUE,  "DCR_MPIC128_BASE + EOI3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + FRG0,   MPIC128_FRG0_VALUE,  "DCR_MPIC128_BASE + FRG0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + GCF0,   MPIC128_GCF0_VALUE,  "DCR_MPIC128_BASE + GCF0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + VITC,   MPIC128_VITC_VALUE,  "DCR_MPIC128_BASE + VITC"
     check_value r4,r5,r6, DCR_MPIC128_BASE + VRID,   MPIC128_VRID_VALUE,  "DCR_MPIC128_BASE + VRID"
     check_value r4,r5,r6, DCR_MPIC128_BASE + PINI,   MPIC128_PINI_VALUE,  "DCR_MPIC128_BASE + PINI"
     check_value r4,r5,r6, DCR_MPIC128_BASE + IVP0,   MPIC128_IVP0_VALUE,  "DCR_MPIC128_BASE + IVP0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + IVP1,   MPIC128_IVP1_VALUE,  "DCR_MPIC128_BASE + IVP1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + IVP2,   MPIC128_IVP2_VALUE,  "DCR_MPIC128_BASE + IVP1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + IVP3,   MPIC128_IVP3_VALUE,  "DCR_MPIC128_BASE + IVP3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + SPV,    MPIC128_SPV_VALUE,   "DCR_MPIC128_BASE + SPV"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TFR,    MPIC128_TFR_VALUE,   "DCR_MPIC128_BASE + TFR"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TCC0,   MPIC128_TCC0_VALUE,  "DCR_MPIC128_BASE + TCC0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TCC1,   MPIC128_TCC1_VALUE,  "DCR_MPIC128_BASE + TCC1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TCC2,   MPIC128_TCC2_VALUE,  "DCR_MPIC128_BASE + TCC2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TCC3,   MPIC128_TCC3_VALUE,  "DCR_MPIC128_BASE + TCC3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TBC0,   MPIC128_TBC0_VALUE,  "DCR_MPIC128_BASE + TBC0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TBC1,   MPIC128_TBC1_VALUE,  "DCR_MPIC128_BASE + TBC1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TBC2,   MPIC128_TBC2_VALUE,  "DCR_MPIC128_BASE + TBC2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TBC3,   MPIC128_TBC3_VALUE,  "DCR_MPIC128_BASE + TBC3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TVP0,   MPIC128_TVP0_VALUE,  "DCR_MPIC128_BASE + TVP0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TVP1,   MPIC128_TVP1_VALUE,  "DCR_MPIC128_BASE + TVP1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TVP2,   MPIC128_TVP2_VALUE,  "DCR_MPIC128_BASE + TVP2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TVP3,   MPIC128_TVP3_VALUE,  "DCR_MPIC128_BASE + TVP3"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TDST0,  MPIC128_TDST0_VALUE, "DCR_MPIC128_BASE + TDST0"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TDST1,  MPIC128_TDST1_VALUE, "DCR_MPIC128_BASE + TDST1"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TDST2,  MPIC128_TDST2_VALUE, "DCR_MPIC128_BASE + TDST2"
     check_value r4,r5,r6, DCR_MPIC128_BASE + TDST3,  MPIC128_TDST3_VALUE, "DCR_MPIC128_BASE + TDST3"

check_cpu_ppc_mpic_4_5:
    load_const r5, 0x00000000
    load_const r4, DCR_MPIC128_BASE + TFR
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
    load_const r4, DCR_MPIC128_BASE + TFR
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
     test_event EVENT_OK
     load_const r3, 0x00
     rumboot_putstring "TEST OK\n"
     b finish

error:
     test_event EVENT_ERROR
     load_const r3, 0x01
     rumboot_putstring "TEST ERROR\n"

finish:
	blr


