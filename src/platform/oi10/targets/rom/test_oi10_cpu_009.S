
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/oi10/platform/regs/regs_plb6plb4.h>

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


