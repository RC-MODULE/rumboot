
#define DEBUG_DCR_TEST
#define DEBUG_PUT2USPRG0

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_plb4plb6.h>


#define CVR3					r14,r15,r16 	/* Check Value Regs triade */
#define P46A0(OFFSET)			(DCR_PLB4PLB6_0_BASE + (OFFSET))

.section ".text","ax",@progbits

.global main

main:
    load_const  r3,   0x00000000
    cmpwi cr6,  r3,   0x0000      /* General test status in cr6 */

p4p6_0:
	rumboot_putstring "Checking PLB4PLB6_0 bridge...\n"
    check_value CVR3, P46A0(P46CR        ), P46_0_CR_VALUE,			"PLB4PLB6_P46CR"
    check_value CVR3, P46A0(ESR          ), ESR_VALUE,				"PLB4PLB6_ESR"
    check_value CVR3, P46A0(P46REV       ), P46REV_VALUE,			"PLB4PLB6_P46REV"
    check_value CVR3, P46A0(EARH         ), EARH_VALUE,				"PLB4PLB6_EARH"
    check_value CVR3, P46A0(EARL         ), EARL_VALUE,				"PLB4PLB6_EARL"
    check_value CVR3, P46A0(SNOOP_UADDRH0), SNOOP_UADDRH0_VALUE,	"PLB4PLB6_SNOOP_UADDRH0"
    check_value CVR3, P46A0(SNOOP_LADDRH0), SNOOP_LADDRH0_VALUE,	"PLB4PLB6_SNOOP_LADDRH0"
    check_value CVR3, P46A0(SNOOP_UADDRL0), SNOOP_UADDRL0_VALUE,	"PLB4PLB6_SNOOP_UADDRL0"
    check_value CVR3, P46A0(SNOOP_LADDRL0), SNOOP_LADDRL0_VALUE,	"PLB4PLB6_SNOOP_LADDRL0"
    check_value CVR3, P46A0(SNOOP_UADDRH1), SNOOP_UADDRH1_VALUE,	"PLB4PLB6_SNOOP_UADDRH1"
    check_value CVR3, P46A0(SNOOP_LADDRH1), SNOOP_LADDRH1_VALUE,	"PLB4PLB6_SNOOP_LADDRH1"
    check_value CVR3, P46A0(SNOOP_UADDRL1), SNOOP_UADDRL1_VALUE,	"PLB4PLB6_SNOOP_UADDRL1"
    check_value CVR3, P46A0(SNOOP_LADDRL1), SNOOP_LADDRL1_VALUE,	"PLB4PLB6_SNOOP_LADDRL1"
	rumboot_putstring "Done!\n"
    /* PLB4PLB6_0_TESR is write-only and reset value is undefined */


running_0_1_P46_0:
    rumboot_putstring "Running 0/1 P46_0\n"
    load_const  r15,  0x00000000
    load_const  r14,  P46A0(SNOOP_LADDRL0)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 1 init)")

p46_0_running_1:
    load_const  r15,  0x00000001
    load_const  r17,  0x00000001
    load_const  r18,  0x80000000
p46_0_loop_1:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 1)")
    cmpw cr0,   r15,  r18
    beq- cr0,   p46_0_running_0
    slw         r15,  r15,  r17
    b           p46_0_loop_1

p46_0_running_0:
    load_const  r15,  0xFFFFFFFF
    load_const  r14,  P46A0(SNOOP_LADDRL0)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 0 init)")

    load_const  r15,  0xFFFFFFFE
    load_const  r17,  0x00000001
    load_const  r18,  0x7FFFFFFF
p46_0_loop_0:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in P4P6_0.SNOOP_LADDRL0 (running 0)")
    cmpw cr0,   r15,  r18
    beq- cr0,   p46_0_end_checking
    slw         r15,  r15,  r17
    ori         r15,  r15,  0x00000001
    b           p46_0_loop_0


p46_0_end_checking:
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

