
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
// #include <mivem_mem_map.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/oi10/platform/regs/regs_plb4plb6.h>

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

.section ".BOOT_ROM.text","ax",@progbits

.global image_start
.global main

image_start:
main:
p4p6_0:
    check_value r4,r5,r6, P4P6_0_P46CR, P46_0_CR_VALUE, "P4P6_0_P46CR"
    check_value r4,r5,r6, P4P6_0_ESR, ESR_VALUE, "P4P6_0_ESR"
    check_value r4,r5,r6, P4P6_0_P46REV, P46REV_VALUE, "P4P6_0_P46REV"
    check_value r4,r5,r6, P4P6_0_EARH, EARH_VALUE, "P4P6_0_EARH"
    check_value r4,r5,r6, P4P6_0_EARL, EARL_VALUE, "P4P6_0_EARL"
    check_value r4,r5,r6, P4P6_0_SNOOP_UADDRH0, SNOOP_UADDRH0_VALUE, "P4P6_0_SNOOP_UADDRH0"
    check_value r4,r5,r6, P4P6_0_SNOOP_LADDRH0, SNOOP_LADDRH0_VALUE, "P4P6_0_SNOOP_LADDRH0"
    check_value r4,r5,r6, P4P6_0_SNOOP_UADDRL0, SNOOP_UADDRL0_VALUE, "P4P6_0_SNOOP_UADDRL0"
    check_value r4,r5,r6, P4P6_0_SNOOP_LADDRL0, SNOOP_LADDRL0_VALUE, "P4P6_0_SNOOP_LADDRL0"
    check_value r4,r5,r6, P4P6_0_SNOOP_UADDRH1, SNOOP_UADDRH1_VALUE, "P4P6_0_SNOOP_UADDRH1"
    check_value r4,r5,r6, P4P6_0_SNOOP_LADDRH1, SNOOP_LADDRH1_VALUE, "P4P6_0_SNOOP_LADDRH1"
    check_value r4,r5,r6, P4P6_0_SNOOP_UADDRL1, SNOOP_UADDRL1_VALUE, "P4P6_0_SNOOP_UADDRL1"
    check_value r4,r5,r6, P4P6_0_SNOOP_LADDRL1, SNOOP_LADDRL1_VALUE, "P4P6_0_SNOOP_LADDRL1"
    //P4P6_0_TESR is write-only and reset value is undefined


running_0_1_P46_0:
    rumboot_putstring "Running 0/1 P46_0\n"
    load_const r5, 0x00000000
    load_const r4, P4P6_0_SNOOP_LADDRL0
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 1 init)")

p46_0_running_1:
    load_const r8, 0x00000001
    load_const r7, 1
    load_const r8, 0x80000000
p46_0_loop_1:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 1)")
    cmp 0, 0, r5, r8
    beq p46_0_running_0
    slw r5, r5, r7
    b p46_0_loop_1

p46_0_running_0:
    load_const r5, 0xFFFFFFFF
    load_const r4, P4P6_0_SNOOP_LADDRL0
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 0 init)")

    load_const r5, 0xFFFFFFFE
    load_const r7, 1
    load_const r8, 0x7FFFFFFF
p46_0_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 0)")
    cmp 0, 0, r5, r8
    beq p46_0_end_checking
    slw r5, r5, r7
    ori r5, r5, 1
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
