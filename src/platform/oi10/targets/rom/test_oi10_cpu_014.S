
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/devices.h>

/* DCR address space, DCRARB */

#define DCRARB_TADDR  DCR_ARB_BASE + 0x0
#define DCRARB_MADDR  DCR_ARB_BASE + 0x1
#define DCRARB_MASK   DCR_ARB_BASE + 0x2
#define DCRARB_DACR   DCR_ARB_BASE + 0x3
#define DCRARB_DAESR  DCR_ARB_BASE + 0x4
#define DCRARB_DAEAR  DCR_ARB_BASE + 0x5
#define DCRARB_DASMR  DCR_ARB_BASE + 0x6
#define DCRARB_REVID  DCR_ARB_BASE + 0x7


/* from cpu/units/dcrarb4m_v001/verilog/rtl/DCRARB4M.v */
#define DCRARB_REVID_HARDCODED  0x00000102

/* from */
#define STRAP_ARBFAIRNESS       0
#define STRAP_ARBLEGACYPROTECT  0

#define DCRARB_TADDR_VALUE      0x00000000
#define DCRARB_MADDR_VALUE      0x00000000
#define DCRARB_MASK_VALUE       0x00000000
#define DCRARB_DACR_VALUE       reg_field(1, STRAP_ARBFAIRNESS) | reg_field(0,STRAP_ARBLEGACYPROTECT)
#define DCRARB_DAESR_VALUE      0x00000000
#define DCRARB_DAEAR_VALUE      0x00000000
#define DCRARB_DASMR_VALUE      0x00000000
#define DCRARB_REVID_VALUE      DCRARB_REVID_HARDCODED


.section ".text","ax",@progbits

.global image_start
.global main

main:
image_start:
    check_value r4,r5,r6, DCRARB_TADDR, DCRARB_TADDR_VALUE, "DCRARB_TADDR"
    check_value r4,r5,r6, DCRARB_MADDR, DCRARB_MADDR_VALUE, "DCRARB_MADDR"
    check_value r4,r5,r6, DCRARB_MASK, DCRARB_MASK_VALUE, "DCRARB_MASK"
    check_value r4,r5,r6, DCRARB_DACR, DCRARB_DACR_VALUE, "DCRARB_DACR"
    check_value r4,r5,r6, DCRARB_DAESR, DCRARB_DAESR_VALUE, "DCRARB_DAESR"
    check_value r4,r5,r6, DCRARB_DAEAR, DCRARB_DAEAR_VALUE, "DCRARB_DAEAR"
    check_value r4,r5,r6, DCRARB_DASMR, DCRARB_DASMR_VALUE, "DCRARB_DASMR"
    check_value r4,r5,r6, DCRARB_REVID, DCRARB_REVID_VALUE, "DCRARB_REVID"

    rumboot_putstring "CHECK DEFAULT VALUES DONE\n"

running_0_1_dcr_arb:
    load_const r5, 0x00000000
    load_const r4, DCRARB_TADDR
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
    load_const r4, DCRARB_TADDR
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
    li r3, 0x00
    b finish

error:
     test_event EVENT_ERROR
     li r3, 0x01

finish:
	blr

