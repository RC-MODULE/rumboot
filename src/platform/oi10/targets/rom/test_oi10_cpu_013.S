
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/oi10/platform/regs/regs_plb4ahb.h>

.section ".text","ax",@progbits

.global image_start
.global main

main:
image_start:
p4ahb_0:
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + REVID, REVID_VALUE, "DCR_PLB4AHB_0_BASE + REVID"
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + SEAR_ADDRUPPER, SEAR_ADDRUPPER_VALUE, "DCR_PLB4AHB_0_BASE + SEAR_ADDRUPPER"
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + SEAR_ADDRLOWER, SEAR_ADDRLOWER_VALUE, "DCR_PLB4AHB_0_BASE + SEAR_ADDRLOWER"
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + SESR, SESR_VALUE, "DCR_PLB4AHB_0_BASE + SESR"
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + TOP_ADDR, TOP_ADDR_VALUE, "DCR_PLB4AHB_0_BASE + TOP_ADDR"
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + BOT_ADDR, BOT_ADDR_VALUE, "DCR_PLB4AHB_0_BASE + BOT_ADDR"
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + ATTRS, ATTRS_VALUE, "DCR_PLB4AHB_0_BASE + ATTRS"
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + CONTROL, CONTROL_VALUE, "DCR_PLB4AHB_0_BASE + CONTROL"
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + ERR_ST, ERR_ST_VALUE, "DCR_PLB4AHB_0_BASE + ERR_ST"
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + ERR_ADDR, ERR_ADDR_VALUE, "DCR_PLB4AHB_0_BASE + ERR_ADDR"
    check_value r4,r5,r6, DCR_PLB4AHB_0_BASE + INT_MSK, INT_MSK_VALUE, "DCR_PLB4AHB_0_BASE + INT_MSK"
p4ahb1:
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + REVID, REVID_VALUE, "DCR_PLB4AHB_1_BASE + REVID"
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + SEAR_ADDRUPPER, SEAR_ADDRUPPER_VALUE, "DCR_PLB4AHB_1_BASE + SEAR_ADDRUPPER"
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + SEAR_ADDRLOWER, SEAR_ADDRLOWER_VALUE, "DCR_PLB4AHB_1_BASE + SEAR_ADDRLOWER"
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + SESR, SESR_VALUE, "DCR_PLB4AHB_1_BASE + SESR"
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + TOP_ADDR, TOP_ADDR_VALUE, "DCR_PLB4AHB_1_BASE + TOP_ADDR"
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + BOT_ADDR, BOT_ADDR_VALUE, "DCR_PLB4AHB_1_BASE + BOT_ADDR"
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + ATTRS, ATTRS_VALUE, "DCR_PLB4AHB_1_BASE + ATTRS"
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + CONTROL, CONTROL_VALUE, "DCR_PLB4AHB_1_BASE + CONTROL"
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + ERR_ST, ERR_ST_VALUE, "DCR_PLB4AHB_1_BASE + ERR_ST"
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + ERR_ADDR, ERR_ADDR_VALUE, "DCR_PLB4AHB_1_BASE + ERR_ADDR"
    check_value r4,r5,r6, DCR_PLB4AHB_1_BASE + INT_MSK, INT_MSK_VALUE, "DCR_PLB4AHB_1_BASE + INT_MSK"
p4ahb2:
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + REVID, REVID_VALUE, "DCR_PLB4AHB_2_BASE + REVID"
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + SEAR_ADDRUPPER, SEAR_ADDRUPPER_VALUE, "DCR_PLB4AHB_2_BASE + SEAR_ADDRUPPER"
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + SEAR_ADDRLOWER, SEAR_ADDRLOWER_VALUE, "DCR_PLB4AHB_2_BASE + SEAR_ADDRLOWER"
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + SESR, SESR_VALUE, "DCR_PLB4AHB_2_BASE + SESR"
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + TOP_ADDR, TOP_ADDR_VALUE, "DCR_PLB4AHB_2_BASE + TOP_ADDR"
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + BOT_ADDR, BOT_ADDR_VALUE, "DCR_PLB4AHB_2_BASE + BOT_ADDR"
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + ATTRS, ATTRS_VALUE, "DCR_PLB4AHB_2_BASE + ATTRS"
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + CONTROL, CONTROL_VALUE, "DCR_PLB4AHB_2_BASE + CONTROL"
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + ERR_ST, ERR_ST_VALUE, "DCR_PLB4AHB_2_BASE + ERR_ST"
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + ERR_ADDR, ERR_ADDR_VALUE, "DCR_PLB4AHB_2_BASE + ERR_ADDR"
    check_value r4,r5,r6, DCR_PLB4AHB_2_BASE + INT_MSK, INT_MSK_VALUE, "DCR_PLB4AHB_2_BASE + INT_MSK"

    rumboot_putstring "DEFAULT VALUES CHECKING DONE\n"
/* check running 1,0 */

running_0_1_p4ahb_0:
    load_const r5, 0x00000000
    load_const r4, DCR_PLB4AHB_0_BASE + CONTROL
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_BASE + CONTROL (running 1 init)")

p4ahb_0_running_1:
    load_const r5, 0x00000001
    load_const r7, 1
    load_const r8, 0x00000080
p4ahb_0_loop_1:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_BASE + CONTROL (running 1)")
    cmp 0, 0, r5, r8
    beq p4ahb_0_running_0
    slw r5, r5, r7
    b p4ahb_0_loop_1

p4ahb_0_running_0:
    load_const r5, 0x000000FF
    load_const r4, DCR_PLB4AHB_0_BASE + CONTROL
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_BASE + CONTROL (running 0 init)")

    load_const r5, 0x000000FE
    load_const r7, 1
    load_const r8, 0x0000007F
    load_const r9, 0x000000FF
p4ahb_0_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_BASE + CONTROL (running 0)")
    cmp 0, 0, r5, r8
    beq p4ahb_0_done
    slw r5, r5, r7
    ori r5, r5, 1
    and r5, r5, r9
    b p4ahb_0_loop_0
 p4ahb_0_done:
    rumboot_putstring "p4ahb_0 running 0/1 done\n"


running_0_1_p4ahb_1:
    load_const r5, 0x00000000
    load_const r4, DCR_PLB4AHB_1_BASE + CONTROL
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_1_BASE + CONTROL (running 1 init)")

p4ahb_1_running_1:
    load_const r5, 0x00000001
    load_const r7, 1
    load_const r8, 0x00000080
p4ahb_1_loop_1:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_1_BASE + CONTROL (running 1)")
    cmp 0, 0, r5, r8
    beq p4ahb_1_running_0
    slw r5, r5, r7
    b p4ahb_1_loop_1

p4ahb_1_running_0:
    load_const r5, 0x000000FF
    load_const r4, DCR_PLB4AHB_1_BASE + CONTROL
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_1_BASE + CONTROL (running 0 init)")

    load_const r5, 0x000000FE
    load_const r7, 1
    load_const r8, 0x0000007F
    load_const r9, 0x000000FF
p4ahb_1_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_1_BASE + CONTROL (running 0)")
    cmp 0, 0, r5, r8
    beq p4ahb_1_done
    slw r5, r5, r7
    ori r5, r5, 1
    and r5, r5, r9
    b p4ahb_1_loop_0
p4ahb_1_done:
    rumboot_putstring "p4ahb_1 running 0/1 done\n"


running_0_1_p4ahb_2:
    load_const r5, 0x00000000
    load_const r4, DCR_PLB4AHB_2_BASE + CONTROL
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_2_BASE + CONTROL (running 1 init)")

p4ahb_2_running_1:
    load_const r5, 0x00000001
    load_const r7, 1
    load_const r8, 0x00000080
p4ahb_2_loop_1:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_2_BASE + CONTROL (running 1)")
    cmp 0, 0, r5, r8
    beq p4ahb_2_running_0
    slw r5, r5, r7
    b p4ahb_2_loop_1

p4ahb_2_running_0:
    load_const r5, 0x000000FF
    load_const r4, DCR_PLB4AHB_2_BASE + CONTROL
    mtdcrx r4, r5
    mfdcrx r2, r0
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_2_BASE + CONTROL (running 0 init)")

    load_const r5, 0x000000FE
    load_const r7, 1
    load_const r8, 0x0000007F
    load_const r9, 0x000000FF
p4ahb_2_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_2_BASE + CONTROL (running 0)")
    cmp cr0, 0, r5, r8
    beq p4ahb_2_done
    slw r5, r5, r7
    ori r5, r5, 1
    and r5, r5, r9
    b p4ahb_2_loop_0
p4ahb_2_done:
    rumboot_putstring "p4ahb_2 running 0/1 done\n"

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

