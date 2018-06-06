
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
// #include <mivem_mem_map.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>


// #include "test_mpw_cpu_012_0.h"

//OFFSET

#define P46CR          0x0
#define ESR            0x1
#define P46REV         0x2
#define EARH           0x3
#define EARL           0x4
#define SNOOP_UADDRH0  0x5
#define SNOOP_LADDRH0  0x6
#define SNOOP_UADDRL0  0x7
#define SNOOP_LADDRL0  0x8
#define SNOOP_UADDRH1  0x9
#define SNOOP_LADDRH1  0xA
#define SNOOP_UADDRL1  0xB
#define SNOOP_LADDRL1  0xC
#define TESR           0xD //WO

#define PLB4PLB6_WATERMARK_VALUE 0x00000112 //from /trunk/cpu/units/plb4plb6_v001/verilog/rtl/PLB4PLB6.defines.v
#define P4P6_0_I_M_SELF_ID          0x2
#define P4P6_1_I_M_SELF_ID          0x5
#define P4P6_2_I_M_SELF_ID          0x6
#define P4P6_0_PNCR_VALUE           0x0E// //See table  2-5 plb4to6bridge_ds_v14_2012SEP17_pub.pdf
#define P4P6_1_PNCR_VALUE           0xBB// //See table  2-5 plb4to6bridge_ds_v14_2012SEP17_pub.pdf
#define P4P6_2_PNCR_VALUE           0x8A// //See table  2-5 plb4to6bridge_ds_v14_2012SEP17_pub.pdf

#define P46_0_CR_VALUE          reg_field(31,P4P6_0_PNCR_VALUE) | reg_field(21,0x1) | reg_field(20,0x1) | reg_field(19,0) | reg_field(18,0b011) | reg_field(5,0b001) | reg_field(2,0b001)
#define P46_1_CR_VALUE          reg_field(31,P4P6_1_PNCR_VALUE) | reg_field(21,0x1) | reg_field(20,0x1) | reg_field(19,0) | reg_field(18,0b011) | reg_field(5,0b001) | reg_field(2,0b001)
#define P46_2_CR_VALUE          reg_field(31,P4P6_2_PNCR_VALUE) | reg_field(21,0x1) | reg_field(20,0x1) | reg_field(19,0) | reg_field(18,0b011) | reg_field(5,0b001) | reg_field(2,0b001)
#define ESR_VALUE            0x00000000
#define P46REV_VALUE         PLB4PLB6_WATERMARK_VALUE
#define EARH_VALUE           0x00000000
#define EARL_VALUE           0x00000000
#define SNOOP_UADDRH0_VALUE  0x00000000
#define SNOOP_LADDRH0_VALUE  0x00000000
#define SNOOP_UADDRL0_VALUE  0x00000000
#define SNOOP_LADDRL0_VALUE  0x00000000
#define SNOOP_UADDRH1_VALUE  0x00000000
#define SNOOP_LADDRH1_VALUE  0x00000000
#define SNOOP_UADDRL1_VALUE  0x00000000
#define SNOOP_LADDRL1_VALUE  0x00000000


//P4P6_0
#define PLB4PLB6_0_BASE DCR_PLB4PLB6_0_BASE
#define P4P6_0_P46CR          (PLB4PLB6_0_BASE + P46CR)
#define P4P6_0_ESR            (PLB4PLB6_0_BASE + ESR)
#define P4P6_0_P46REV         (PLB4PLB6_0_BASE + P46REV)
#define P4P6_0_EARH           (PLB4PLB6_0_BASE + EARH)
#define P4P6_0_EARL           (PLB4PLB6_0_BASE + EARL)
#define P4P6_0_SNOOP_UADDRH0  (PLB4PLB6_0_BASE + SNOOP_UADDRH0)
#define P4P6_0_SNOOP_LADDRH0  (PLB4PLB6_0_BASE + SNOOP_LADDRH0)
#define P4P6_0_SNOOP_UADDRL0  (PLB4PLB6_0_BASE + SNOOP_UADDRL0)
#define P4P6_0_SNOOP_LADDRL0  (PLB4PLB6_0_BASE + SNOOP_LADDRL0)
#define P4P6_0_SNOOP_UADDRH1  (PLB4PLB6_0_BASE + SNOOP_UADDRH1)
#define P4P6_0_SNOOP_LADDRH1  (PLB4PLB6_0_BASE + SNOOP_LADDRH1)
#define P4P6_0_SNOOP_UADDRL1  (PLB4PLB6_0_BASE + SNOOP_UADDRL1)
#define P4P6_0_SNOOP_LADDRL1  (PLB4PLB6_0_BASE + SNOOP_LADDRL1)
#define P4P6_0_TESR           (PLB4PLB6_0_BASE + TESR)


.section ".BOOT_ROM.text","ax",@progbits

.global image_start
.global main

image_start:
main:
p4p6_0:
    check_value r0,r1,r2, P4P6_0_P46CR, P46_0_CR_VALUE, "P4P6_0_P46CR"
    check_value r0,r1,r2, P4P6_0_ESR, ESR_VALUE, "P4P6_0_ESR"
    check_value r0,r1,r2, P4P6_0_P46REV, P46REV_VALUE, "P4P6_0_P46REV"
    check_value r0,r1,r2, P4P6_0_EARH, EARH_VALUE, "P4P6_0_EARH"
    check_value r0,r1,r2, P4P6_0_EARL, EARL_VALUE, "P4P6_0_EARL"
    check_value r0,r1,r2, P4P6_0_SNOOP_UADDRH0, SNOOP_UADDRH0_VALUE, "P4P6_0_SNOOP_UADDRH0"
    check_value r0,r1,r2, P4P6_0_SNOOP_LADDRH0, SNOOP_LADDRH0_VALUE, "P4P6_0_SNOOP_LADDRH0"
    check_value r0,r1,r2, P4P6_0_SNOOP_UADDRL0, SNOOP_UADDRL0_VALUE, "P4P6_0_SNOOP_UADDRL0"
    check_value r0,r1,r2, P4P6_0_SNOOP_LADDRL0, SNOOP_LADDRL0_VALUE, "P4P6_0_SNOOP_LADDRL0"
    check_value r0,r1,r2, P4P6_0_SNOOP_UADDRH1, SNOOP_UADDRH1_VALUE, "P4P6_0_SNOOP_UADDRH1"
    check_value r0,r1,r2, P4P6_0_SNOOP_LADDRH1, SNOOP_LADDRH1_VALUE, "P4P6_0_SNOOP_LADDRH1"
    check_value r0,r1,r2, P4P6_0_SNOOP_UADDRL1, SNOOP_UADDRL1_VALUE, "P4P6_0_SNOOP_UADDRL1"
    check_value r0,r1,r2, P4P6_0_SNOOP_LADDRL1, SNOOP_LADDRL1_VALUE, "P4P6_0_SNOOP_LADDRL1"
    //P4P6_0_TESR is write-only and reset value is undefined


running_0_1_P46_0:
    rumboot_putstring "Running 0/1 P46_0\n"
    load_const r1, 0x00000000
    load_const r0, P4P6_0_SNOOP_LADDRL0
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 1 init)")

p46_0_running_1:
    load_const r1, 0x00000001
    load_const r3, 1
    load_const r4, 0x80000000
p46_0_loop_1:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 1)")
    cmp 0, 0, r1, r4
    beq p46_0_running_0
    slw r1, r1, r3
    b p46_0_loop_1

p46_0_running_0:
    load_const r1, 0xFFFFFFFF
    load_const r0, P4P6_0_SNOOP_LADDRL0
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 0 init)")

    load_const r1, 0xFFFFFFFE
    load_const r3, 1
    load_const r4, 0x7FFFFFFF
p46_0_loop_0:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 0)")
    cmp 0, 0, r1, r4
    beq p46_0_end_checking
    slw r1, r1, r3
    ori r1, r1, 1
    b p46_0_loop_0

/////////////////////////////////////////////////////////////////////////////////////////


p46_0_end_checking:

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
