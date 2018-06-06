
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/oi10/platform/regs/regs_plb4plb6.h>

.section ".text","ax",@progbits

.global image_start
.global main

image_start:
main:
p4p6_0:
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + P46CR, P46_0_CR_VALUE, "DCR_PLB4PLB6_0_BASE + P46CR"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + ESR, ESR_VALUE, "DCR_PLB4PLB6_0_BASE + ESR"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + P46REV, P46REV_VALUE, "DCR_PLB4PLB6_0_BASE + P46REV"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + EARH, EARH_VALUE, "DCR_PLB4PLB6_0_BASE + EARH"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + EARL, EARL_VALUE, "DCR_PLB4PLB6_0_BASE + EARL"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + SNOOP_UADDRH0, SNOOP_UADDRH0_VALUE, "DCR_PLB4PLB6_0_BASE + SNOOP_UADDRH0"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + SNOOP_LADDRH0, SNOOP_LADDRH0_VALUE, "DCR_PLB4PLB6_0_BASE + SNOOP_LADDRH0"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + SNOOP_UADDRL0, SNOOP_UADDRL0_VALUE, "DCR_PLB4PLB6_0_BASE + SNOOP_UADDRL0"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + SNOOP_LADDRL0, SNOOP_LADDRL0_VALUE, "DCR_PLB4PLB6_0_BASE + SNOOP_LADDRL0"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + SNOOP_UADDRH1, SNOOP_UADDRH1_VALUE, "DCR_PLB4PLB6_0_BASE + SNOOP_UADDRH1"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + SNOOP_LADDRH1, SNOOP_LADDRH1_VALUE, "DCR_PLB4PLB6_0_BASE + SNOOP_LADDRH1"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + SNOOP_UADDRL1, SNOOP_UADDRL1_VALUE, "DCR_PLB4PLB6_0_BASE + SNOOP_UADDRL1"
    check_value r4,r5,r6, DCR_PLB4PLB6_0_BASE + SNOOP_LADDRL1, SNOOP_LADDRL1_VALUE, "DCR_PLB4PLB6_0_BASE + SNOOP_LADDRL1"
    /* DCR_PLB4PLB6_0_BASE + TESR is write-only and reset value is undefined */


running_0_1_P46_0:
    rumboot_putstring "Running 0/1 P46_0\n"
    load_const r5, 0x00000000
    load_const r4, DCR_PLB4PLB6_0_BASE + SNOOP_LADDRL0
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
    load_const r4, DCR_PLB4PLB6_0_BASE + SNOOP_LADDRL0
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


p46_0_end_checking:

test_ok:
	test_event EVENT_OK
    rumboot_putstring "TEST OK\n"
    load_const r3, 0x00
    b finish

error:
	test_event EVENT_ERROR
	rumboot_putstring "TEST ERROR\n"
	load_const r3, 0x01

finish:
	blr

