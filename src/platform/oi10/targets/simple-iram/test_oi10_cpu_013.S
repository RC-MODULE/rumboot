
#define DEBUG_DCR_TEST
#define DEBUG_PUT2USPRG0
#define DEBUG_CONTINUE_ON_ASSERT
#define RUMBOOT_ASSERT_WARN_ONLY

/*
#define UNIDIRECTIONAL_BRIDGE
*/

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_plb4ahb.h>

#define CVR3					r14,r15,r16 	/* Check Value Regs triade */
#define P4A0(OFFSET)			(DCR_PLB4AHB_0_BASE + (OFFSET))


.section ".text","ax",@progbits

.global main

main:
    load_const  r3,   0x00000000
    cmpwi cr6,  r3,   0x0000      /* General test status in cr6 */

p4ahb_0:
    rumboot_putstring "Check default values...\n"

    check_value CVR3, P4A0(REVID         ), REVID_VALUE,			"PLB4AHB_0_REVID"
    check_value CVR3, P4A0(SEAR_ADDRUPPER), SEAR_ADDRUPPER_VALUE,	"PLB4AHB_0_SEAR_ADDRUPPER"
    check_value CVR3, P4A0(SEAR_ADDRLOWER), SEAR_ADDRLOWER_VALUE,	"PLB4AHB_0_SEAR_ADDRLOWER"
    check_value CVR3, P4A0(SESR          ), SESR_VALUE,				"PLB4AHB_0_SESR"
    check_value CVR3, P4A0(TOP_ADDR      ), TOP_ADDR_VALUE,			"PLB4AHB_0_TOP_ADDR"
    check_value CVR3, P4A0(BOT_ADDR      ), BOT_ADDR_VALUE,			"PLB4AHB_0_BOT_ADDR"
    check_value CVR3, P4A0(ATTRS         ), ATTRS_VALUE,			"PLB4AHB_0_ATTRS"
    check_value CVR3, P4A0(CONTROL       ), CONTROL_VALUE,			"PLB4AHB_0_CONTROL"
    check_value CVR3, P4A0(ERR_ST        ), ERR_ST_VALUE,			"PLB4AHB_0_ERR_ST"
    check_value CVR3, P4A0(ERR_ADDR      ), ERR_ADDR_VALUE,			"PLB4AHB_0_ERR_ADDR"
    check_value CVR3, P4A0(INT_MSK       ), INT_MSK_VALUE,			"PLB4AHB_0_INT_MSK"

    rumboot_putstring "Done!\n"

running_0_1_p4ahb_0:
    load_const  r15,  0x00000000
    load_const  r14,  P4A0(CONTROL)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_CONTROL (running 1 init)")

p4ahb_0_running_1:
    load_const  r15,  0x00000001
    load_const  r17,  0x00000001
    load_const  r18,  0x00000080
p4ahb_0_loop_1:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_CONTROL (running 1)")
    cmpw cr0,   r15,  r18
    beq- cr0,   p4ahb_0_running_0
    slw         r15,  r15,  r17
    b           p4ahb_0_loop_1

p4ahb_0_running_0:
    load_const  r15,  0x000000FF
    load_const  r14,  P4A0(CONTROL)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_CONTROL (running 0 init)")

    load_const  r15,  0x000000FE
    load_const  r17,  0x00000001
    load_const  r18,  0x0000007F
    load_const  r19,  0x000000FF
p4ahb_0_loop_0:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_PLB4AHB_0_CONTROL (running 0)")
    cmpw cr0,   r15,  r18
    beq- cr0,   p4ahb_0_done
    slw         r15,  r15,  r17
    ori         r15,  r15,  0x00000001
    and         r15,  r15,  r19
    b           p4ahb_0_loop_0

 p4ahb_0_done:
    rumboot_putstring "p4ahb_0 running 0/1 done!\n"
    bne- cr6,   test_error

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

