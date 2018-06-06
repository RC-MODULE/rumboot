
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/devices.h>
#include <platform/oi10/platform/regs/regs_dcrarb.h>

.section ".text","ax",@progbits

.global image_start
.global main

main:
image_start:
    check_value r4,r5,r6, DCR_ARB_BASE + DCRARB_TADDR, DCRARB_TADDR_VALUE, "DCRARB_TADDR"
    check_value r4,r5,r6, DCR_ARB_BASE + DCRARB_MADDR, DCRARB_MADDR_VALUE, "DCRARB_MADDR"
    check_value r4,r5,r6, DCR_ARB_BASE + DCRARB_MASK,  DCRARB_MASK_VALUE,  "DCRARB_MASK"
    check_value r4,r5,r6, DCR_ARB_BASE + DCRARB_DACR,  DCRARB_DACR_VALUE,  "DCRARB_DACR"
    check_value r4,r5,r6, DCR_ARB_BASE + DCRARB_DAESR, DCRARB_DAESR_VALUE, "DCRARB_DAESR"
    check_value r4,r5,r6, DCR_ARB_BASE + DCRARB_DAEAR, DCRARB_DAEAR_VALUE, "DCRARB_DAEAR"
    check_value r4,r5,r6, DCR_ARB_BASE + DCRARB_DASMR, DCRARB_DASMR_VALUE, "DCRARB_DASMR"
    check_value r4,r5,r6, DCR_ARB_BASE + DCRARB_REVID, DCRARB_REVID_VALUE, "DCRARB_REVID"

    rumboot_putstring "CHECK DEFAULT VALUES DONE\n"

running_0_1_dcr_arb:
    load_const r5, 0x00000000
    load_const r4, DCR_ARB_BASE + DCRARB_TADDR
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCRARB_TADDR (running 1 init)")

dcr_arb_running_1:
    load_const r5, 0x00000001
    load_const r7, 1
    load_const r8, 0x80000000
dcr_arb_loop_1:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCRARB_TADDR (running 1)")
    cmp 0, 0, r5, r8
    beq dcr_arb_running_0
    slw r5, r5, r7
    b dcr_arb_loop_1

dcr_arb_running_0:
    load_const r5, 0xFFFFFFFF
    load_const r4, DCR_ARB_BASE + DCRARB_TADDR
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCRARB_TADDR (running 0 init)")

    load_const r5, 0xFFFFFFFE
    load_const r7, 1
    load_const r8, 0x7FFFFFFF
dcr_arb_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCRARB_TADDR (running 0)")
    cmp 0, 0, r5, r8
    beq end_checking
    slw r5, r5, r7
    ori r5, r5, 1
    b dcr_arb_loop_0

    rumboot_putstring "dcr_arb running 0/1 done\n"

end_checking:

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

