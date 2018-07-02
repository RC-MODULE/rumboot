
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>

#define DEBUG_DCR_TEST

#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/devices.h>
#include <platform/regs/regs_dcrarb.h>

#define eq6                     cr6*4+eq
#define eq7                     cr7*4+eq

#define CVR3					r4,r5,r6 	/* Check Value Regs triade */
#define ARBA(OFFSET)			(DCR_ARB_BASE + (OFFSET))


.section ".text","ax",@progbits

.global main

main:
    load_const  r3,   0x00000000
    cmpwi cr6,  r3,   0x0000      /* General test status in cr6 */

    rumboot_putstring "CHECK DEFAULT VALUES...\n"
    check_value CVR3, ARBA(DCRARB_TADDR), DCRARB_TADDR_VALUE, "DCRARB_TADDR"
    check_value CVR3, ARBA(DCRARB_MADDR), DCRARB_MADDR_VALUE, "DCRARB_MADDR"
    check_value CVR3, ARBA(DCRARB_MASK ), DCRARB_MASK_VALUE,  "DCRARB_MASK "
    check_value CVR3, ARBA(DCRARB_DACR ), DCRARB_DACR_VALUE,  "DCRARB_DACR "
    check_value CVR3, ARBA(DCRARB_DAESR), DCRARB_DAESR_VALUE, "DCRARB_DAESR"
    check_value CVR3, ARBA(DCRARB_DAEAR), DCRARB_DAEAR_VALUE, "DCRARB_DAEAR"
    check_value CVR3, ARBA(DCRARB_DASMR), DCRARB_DASMR_VALUE, "DCRARB_DASMR"
    check_value CVR3, ARBA(DCRARB_REVID), DCRARB_REVID_VALUE, "DCRARB_REVID"
    rumboot_putstring "CHECK DEFAULT VALUES DONE\n"

running_0_1_dcr_arb:
    rumboot_putstring "dcr_arb running 0/1 ...\n"

    load_const  r15,  0x00000000
    load_const  r14,  ARBA(DCRARB_TADDR)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Read/write error in DCRARB_TADDR (running 1 init)")

dcr_arb_running_1:
    load_const  r15,  0x00000001
    load_const  r17,  0x00000001
    load_const  r18,  0x80000000
dcr_arb_loop_1:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Read/write error in DCRARB_TADDR (running 1)")
    cmpw cr0,   r15,  r18
    beq- cr0,   dcr_arb_running_0
    slw         r15,  r15,  r17
    b           dcr_arb_loop_1

dcr_arb_running_0:
    load_const  r15,  0xFFFFFFFF
    load_const  r14,  ARBA(DCRARB_TADDR)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Read/write error in DCRARB_TADDR (running 0 init)")

    load_const  r15,  0xFFFFFFFE
    load_const  r17,  0x00000001
    load_const  r18,  0x7FFFFFFF
dcr_arb_loop_0:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Read/write error in DCRARB_TADDR (running 0)")
    cmpw cr0,   r15,  r18
    beq- cr0,   end_checking
    slw         r15,  r15,  r17
    ori         r15,  r15,  0x00000001
    b           dcr_arb_loop_0

end_checking:
    rumboot_putstring "dcr_arb running 0/1 done\n"
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

