#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
// #include <mivem_mem_map.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>


// AG FIX
#define NO_TRACE_HEX

// #include "test_mpw_cpu_007_0.h"
//MPIC Registers
#include <platform/oi10/platform/regs/regs_mpic.h>


.section ".text","ax",@progbits

.global image_start
.global main

main:
image_start:
    //reset mpic
    load_const r1, MPICx_R_BIT
    load_const r0, MPICx_GCF0_ADDRESS
    mtdcrx r0, r1

read_MPICx_GCF0_R_BIT:
    mfdcrx r2, r0
    cmp 0,0, r2,r0
    beq read_MPICx_GCF0_R_BIT

check_cpu_ppc_mpic_3:
     check_value r0,r1,r2, MPICx_VP0_ADDRESS, MPICx_VP0_VALUE,"MPICx_VP0"
     check_value r0,r1,r2, MPICx_DST0_ADDRESS, MPICx_DST0_VALUE,"MPICx_DST0"
     check_value r0,r1,r2, MPICx_CTP0_ADDRESS, MPICx_CTP0_VALUE,"MPICx_CTP0"
     check_value r0,r1,r2, MPICx_CTP1_ADDRESS, MPICx_CTP1_VALUE,"MPICx_CTP1"
     check_value r0,r1,r2, MPICx_CTP2_ADDRESS, MPICx_CTP2_VALUE,"MPICx_CTP2"
     check_value r0,r1,r2, MPICx_CTP3_ADDRESS, MPICx_CTP3_VALUE,"MPICx_CTP3"
     check_value r0,r1,r2, MPICx_NCIAR0_ADDRESS,MPICx_NCIAR0_VALUE,"MPICx_NCIAR0"
     check_value r0,r1,r2, MPICx_NCIAR1_ADDRESS,MPICx_NCIAR1_VALUE,"MPICx_NCIAR1"
     check_value r0,r1,r2, MPICx_NCIAR2_ADDRESS,MPICx_NCIAR2_VALUE,"MPICx_NCIAR2"
     check_value r0,r1,r2, MPICx_NCIAR3_ADDRESS,MPICx_NCIAR3_VALUE,"MPICx_NCIAR3"
     check_value r0,r1,r2, MPICx_NCEOI0_ADDRESS, MPICx_NCEOI0_VALUE,"MPICx_NCEOI0"
     check_value r0,r1,r2, MPICx_NCEOI1_ADDRESS, MPICx_NCEOI1_VALUE,"MPICx_NCEOI1"
     check_value r0,r1,r2, MPICx_NCEOI2_ADDRESS, MPICx_NCEOI2_VALUE,"MPICx_NCEOI2"
     check_value r0,r1,r2, MPICx_NCEOI3_ADDRESS, MPICx_NCEOI3_VALUE,"MPICx_NCEOI3"
     check_value r0,r1,r2, MPICx_CIAR0_ADDRESS,MPICx_CIAR0_VALUE,"MPICx_CIAR0"
     check_value r0,r1,r2, MPICx_CIAR1_ADDRESS,MPICx_CIAR1_VALUE,"MPICx_CIAR1"
     check_value r0,r1,r2, MPICx_CIAR2_ADDRESS,MPICx_CIAR2_VALUE,"MPICx_CIAR2"
     check_value r0,r1,r2, MPICx_CIAR3_ADDRESS,MPICx_CIAR3_VALUE,"MPICx_CIAR3"
     check_value r0,r1,r2, MPICx_CEOI0_ADDRESS,MPICx_CEOI0_VALUE,"MPICx_CEOI0"
     check_value r0,r1,r2, MPICx_CEOI1_ADDRESS,MPICx_CEOI1_VALUE,"MPICx_CEOI1"
     check_value r0,r1,r2, MPICx_CEOI2_ADDRESS,MPICx_CEOI2_VALUE,"MPICx_CEOI2"
     check_value r0,r1,r2, MPICx_CEOI3_ADDRESS,MPICx_CEOI3_VALUE,"MPICx_CEOI3"
     check_value r0,r1,r2, MPICx_IAR0_ADDRESS,MPICx_IAR0_VALUE,"MPICx_IAR0"
     check_value r0,r1,r2, MPICx_IAR1_ADDRESS ,MPICx_IAR1_VALUE,"MPICx_IAR1"
     check_value r0,r1,r2, MPICx_IAR2_ADDRESS ,MPICx_IAR2_VALUE,"MPICx_IAR2"
     check_value r0,r1,r2, MPICx_IAR3_ADDRESS ,MPICx_IAR3_VALUE,"MPICx_IAR3"
     check_value r0,r1,r2, MPICx_EOI0_ADDRESS ,MPICx_EOI0_VALUE,"MPICx_EOI0"
     check_value r0,r1,r2, MPICx_EOI1_ADDRESS ,MPICx_EOI1_VALUE,"MPICx_EOI1"
     check_value r0,r1,r2, MPICx_EOI2_ADDRESS ,MPICx_EOI2_VALUE,"MPICx_EOI2"
     check_value r0,r1,r2, MPICx_EOI3_ADDRESS ,MPICx_EOI3_VALUE,"MPICx_EOI3"
     check_value r0,r1,r2, MPICx_FRG0_ADDRESS ,MPICx_FRG0_VALUE,"MPICx_FRG0"
     check_value r0,r1,r2, MPICx_GCF0_ADDRESS ,MPICx_GCF0_VALUE,"MPICx_GCF0"
     check_value r0,r1,r2, MPICx_VITC_ADDRESS ,MPICx_VITC_VALUE,"MPICx_VITC"
     check_value r0,r1,r2, MPICx_VRID_ADDRESS ,MPICx_VRID_VALUE,"MPICx_VRID"
     check_value r0,r1,r2, MPICx_PINI_ADDRESS ,MPICx_PINI_VALUE,"MPICx_PINI"
     check_value r0,r1,r2, MPICx_IVP0_ADDRESS ,MPICx_IVP0_VALUE,"MPICx_IVP0"
     check_value r0,r1,r2, MPICx_IVP1_ADDRESS ,MPICx_IVP1_VALUE,"MPICx_IVP1"
     check_value r0,r1,r2, MPICx_IVP2_ADDRESS ,MPICx_IVP2_VALUE,"MPICx_IVP1"
     check_value r0,r1,r2, MPICx_IVP3_ADDRESS ,MPICx_IVP3_VALUE,"MPICx_IVP3"
     check_value r0,r1,r2, MPICx_SPV_ADDRESS  ,MPICx_SPV_VALUE,"MPICx_SPV"
     check_value r0,r1,r2, MPICx_TFR_ADDRESS  ,MPICx_TFR_VALUE,"MPICx_TFR"
     check_value r0,r1,r2, MPICx_TCC0_ADDRESS ,MPICx_TCC0_VALUE,"MPICx_TCC0"
     check_value r0,r1,r2, MPICx_TCC1_ADDRESS ,MPICx_TCC1_VALUE,"MPICx_TCC1"
     check_value r0,r1,r2, MPICx_TCC2_ADDRESS ,MPICx_TCC2_VALUE,"MPICx_TCC2"
     check_value r0,r1,r2, MPICx_TCC3_ADDRESS ,MPICx_TCC3_VALUE,"MPICx_TCC3"
     check_value r0,r1,r2, MPICx_TBC0_ADDRESS ,MPICx_TBC0_VALUE,"MPICx_TBC0"
     check_value r0,r1,r2, MPICx_TBC1_ADDRESS ,MPICx_TBC1_VALUE,"MPICx_TBC1"
     check_value r0,r1,r2, MPICx_TBC2_ADDRESS ,MPICx_TBC2_VALUE,"MPICx_TBC2"
     check_value r0,r1,r2, MPICx_TBC3_ADDRESS ,MPICx_TBC3_VALUE,"MPICx_TBC3"
     check_value r0,r1,r2, MPICx_TVP0_ADDRESS ,MPICx_TVP0_VALUE,"MPICx_TVP0"
     check_value r0,r1,r2, MPICx_TVP1_ADDRESS ,MPICx_TVP1_VALUE,"MPICx_TVP1"
     check_value r0,r1,r2, MPICx_TVP2_ADDRESS ,MPICx_TVP2_VALUE,"MPICx_TVP2"
     check_value r0,r1,r2, MPICx_TVP3_ADDRESS ,MPICx_TVP3_VALUE,"MPICx_TVP3"
     check_value r0,r1,r2, MPICx_TDST0_ADDRESS,MPICx_TDST0_VALUE,"MPICx_TDST0"
     check_value r0,r1,r2, MPICx_TDST1_ADDRESS,MPICx_TDST1_VALUE,"MPICx_TDST1"
     check_value r0,r1,r2, MPICx_TDST2_ADDRESS,MPICx_TDST2_VALUE,"MPICx_TDST2"
     check_value r0,r1,r2, MPICx_TDST3_ADDRESS,MPICx_TDST3_VALUE,"MPICx_TDST3"

check_cpu_ppc_mpic_4_5:
    load_const r1, 0x00000000
    load_const r0, MPICx_TFR_ADDRESS
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR")

running_1:
    load_const r1, 0x00000001
    load_const r3, 1
    load_const r4, 0x80000000
loop_1:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR (running 1)")
    cmp 0, 0, r1, r4
    beq running_0
    slw r1, r1, r3
    b loop_1

running_0:
    load_const r1, 0xFFFFFFFF
    load_const r0, MPICx_TFR_ADDRESS
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR (running 0)")

    load_const r1, 0xFFFFFFFE
    load_const r3, 1
    load_const r4, 0x7FFFFFFF
loop_0:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR (running 0)")
    cmp 0, 0, r1, r4
    beq test_ok
    slw r1, r1, r3
    ori r1, r1, 1
    b loop_0

test_ok:
     test_event EVENT_OK
     li r3, 0x00
     b finish

error:
     test_event EVENT_ERROR
     li r3, 0x01

finish:
	blr
    // b finish

