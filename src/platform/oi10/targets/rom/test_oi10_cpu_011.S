
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
// #include <mivem_mem_map.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>

// #include "test_cpu_0007.h"

#define SB0CR			0x0
#define SB1CR			0x1
#define SB2CR			0x2
#define SB3CR			0x3
#define BEAR			0x4
#define BESR0			0x5
#define BESR1			0x6
#define PMEG			0x7
#define CID				0x8
#define REVID			0x9
#define DPC				0xA

#define SRAMMC2PLB4_0_BASE DCR_SRAMMC2PLB4_0_BASE
#define SRAMMC2PLB4_1_BASE DCR_SRAMMC2PLB4_1_BASE

#define SRAMMC2PLB4_0_SB0CR		(SRAMMC2PLB4_0_BASE | SB0CR)
#define SRAMMC2PLB4_0_SB1CR		(SRAMMC2PLB4_0_BASE | SB1CR)
#define SRAMMC2PLB4_0_SB2CR		(SRAMMC2PLB4_0_BASE | SB2CR)
#define SRAMMC2PLB4_0_SB3CR		(SRAMMC2PLB4_0_BASE | SB3CR)
#define SRAMMC2PLB4_0_BEAR		(SRAMMC2PLB4_0_BASE | BEAR)
#define SRAMMC2PLB4_0_BESR0		(SRAMMC2PLB4_0_BASE | BESR0)
#define SRAMMC2PLB4_0_BESR1		(SRAMMC2PLB4_0_BASE | BESR1)
#define SRAMMC2PLB4_0_PMEG		(SRAMMC2PLB4_0_BASE | PMEG)
#define SRAMMC2PLB4_0_CID		(SRAMMC2PLB4_0_BASE | CID)
#define SRAMMC2PLB4_0_REVID		(SRAMMC2PLB4_0_BASE | REVID)
#define SRAMMC2PLB4_0_DPC		(SRAMMC2PLB4_0_BASE | DPC)

#define SRAMMC2PLB4_1_SB0CR		(SRAMMC2PLB4_1_BASE | SB0CR)
#define SRAMMC2PLB4_1_SB1CR		(SRAMMC2PLB4_1_BASE | SB1CR)
#define SRAMMC2PLB4_1_SB2CR		(SRAMMC2PLB4_1_BASE | SB2CR)
#define SRAMMC2PLB4_1_SB3CR		(SRAMMC2PLB4_1_BASE | SB3CR)
#define SRAMMC2PLB4_1_BEAR		(SRAMMC2PLB4_1_BASE | BEAR)
#define SRAMMC2PLB4_1_BESR0		(SRAMMC2PLB4_1_BASE | BESR0)
#define SRAMMC2PLB4_1_BESR1		(SRAMMC2PLB4_1_BASE | BESR1)
#define SRAMMC2PLB4_1_PMEG		(SRAMMC2PLB4_1_BASE | PMEG)
#define SRAMMC2PLB4_1_CID		(SRAMMC2PLB4_1_BASE | CID)
#define SRAMMC2PLB4_1_REVID		(SRAMMC2PLB4_1_BASE | REVID)
#define SRAMMC2PLB4_1_DPC		(SRAMMC2PLB4_1_BASE | DPC)

//next values from sys_ctrl.v ALTCR_PGM
#define SRAMMC2PLB4_0_SB0CR_RESET_VAL			0xFFFC098F
#define SRAMMC2PLB4_0_SB1CR_RESET_VAL			0xFFFD098F
#define SRAMMC2PLB4_0_SB2CR_RESET_VAL			0xFFFE098F
#define SRAMMC2PLB4_0_SB3CR_RESET_VAL			0xFFFF098F

#define SRAMMC2PLB4_1_SB0CR_RESET_VAL         0x00040980
#define SRAMMC2PLB4_1_SB1CR_RESET_VAL         0x00050980
#define SRAMMC2PLB4_1_SB2CR_RESET_VAL         0x00060980
#define SRAMMC2PLB4_1_SB3CR_RESET_VAL         0x00070980

//
#define BEAR_RESET_VAL			0x00000000
#define BESR0_RESET_VAL			0x00000000
#define BESR1_RESET_VAL			0x00000000
#define PMEG_RESET_VAL			reg_field(10,0xF)
#define CID_RESET_VAL			0x322B0000
#define REVID_RESET_VAL			0x00000160

#define SYS_DATA_PARITY         0
#define DPC_RESET_VAL			reg_field(0,SYS_DATA_PARITY)

.section ".BOOT_ROM.text","ax",@progbits

.global image_start
.global main

image_start:
main:

check_cpu_ppc_srammc2plb4_0_2:
    check_value r0,r1,r2, SRAMMC2PLB4_0_SB0CR, SRAMMC2PLB4_0_SB0CR_RESET_VAL, "SRAMMC2PLB4_0_SB0CR"
    check_value r0,r1,r2, SRAMMC2PLB4_0_SB1CR, SRAMMC2PLB4_0_SB1CR_RESET_VAL, "SRAMMC2PLB4_0_SB1CR"
    check_value r0,r1,r2, SRAMMC2PLB4_0_SB2CR, SRAMMC2PLB4_0_SB2CR_RESET_VAL, "SRAMMC2PLB4_0_SB2CR"
    check_value r0,r1,r2, SRAMMC2PLB4_0_SB3CR, SRAMMC2PLB4_0_SB3CR_RESET_VAL, "SRAMMC2PLB4_0_SB3CR"
    check_value r0,r1,r2, SRAMMC2PLB4_0_BEAR, BEAR_RESET_VAL, "SRAMMC2PLB4_0_BEAR"
    check_value r0,r1,r2, SRAMMC2PLB4_0_BESR0, BESR0_RESET_VAL, "SRAMMC2PLB4_0_BESR0"
    check_value r0,r1,r2, SRAMMC2PLB4_0_BESR1, BESR1_RESET_VAL, "SRAMMC2PLB4_0_BESR1"
    check_value r0,r1,r2, SRAMMC2PLB4_0_PMEG, PMEG_RESET_VAL, "SRAMMC2PLB4_0_PMEG"
    check_value r0,r1,r2, SRAMMC2PLB4_0_CID, CID_RESET_VAL, "SRAMMC2PLB4_0_CID"
    check_value r0,r1,r2, SRAMMC2PLB4_0_REVID, REVID_RESET_VAL, "SRAMMC2PLB4_0_REVID"
    check_value r0,r1,r2, SRAMMC2PLB4_0_DPC, DPC_RESET_VAL, "SRAMMC2PLB4_0_DPC"

check_cpu_ppc_srammc2plb4_0_3:
    load_const r1, 0x00000000
    load_const r0, SRAMMC2PLB4_0_BEAR
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 1 init)")

srammc2plb4_0_running_1:
    load_const r1, 0x00000001
    load_const r3, 1
    load_const r4, 0x80000000
srammc2plb4_0_loop_1:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 1)")
    cmp 0, 0, r1, r4
    beq srammc2plb4_0_running_0
    slw r1, r1, r3
    b srammc2plb4_0_loop_1

srammc2plb4_0_running_0:
    load_const r1, 0xFFFFFFFF
    load_const r0, SRAMMC2PLB4_0_BEAR
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 0 init)")

    load_const r1, 0xFFFFFFFE
    load_const r3, 1
    load_const r4, 0x7FFFFFFF
srammc2plb4_0_loop_0:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 0)")
    cmp 0, 0, r1, r4
    beq check_cpu_ppc_srammc2plb4_1_2
    slw r1, r1, r3
    ori r1, r1, 1
    b srammc2plb4_0_loop_0

check_cpu_ppc_srammc2plb4_1_2:
    check_value r0,r1,r2, SRAMMC2PLB4_1_SB0CR, SRAMMC2PLB4_1_SB0CR_RESET_VAL,"SRAMMC2PLB4_1_SB0CR"
    check_value r0,r1,r2, SRAMMC2PLB4_1_SB1CR, SRAMMC2PLB4_1_SB1CR_RESET_VAL,"SRAMMC2PLB4_1_SB1CR"
    check_value r0,r1,r2, SRAMMC2PLB4_1_SB2CR, SRAMMC2PLB4_1_SB2CR_RESET_VAL,"SRAMMC2PLB4_1_SB2CR"
    check_value r0,r1,r2, SRAMMC2PLB4_1_SB3CR, SRAMMC2PLB4_1_SB3CR_RESET_VAL,"SRAMMC2PLB4_1_SB3CR"
    check_value r0,r1,r2, SRAMMC2PLB4_1_BEAR, BEAR_RESET_VAL,"SRAMMC2PLB4_1_BEAR"
    check_value r0,r1,r2, SRAMMC2PLB4_1_BESR0, BESR0_RESET_VAL,"SRAMMC2PLB4_1_BESR0"
    check_value r0,r1,r2, SRAMMC2PLB4_1_BESR1, BESR1_RESET_VAL,"SRAMMC2PLB4_1_BESR1"
    check_value r0,r1,r2, SRAMMC2PLB4_1_PMEG, PMEG_RESET_VAL,"SRAMMC2PLB4_1_PMEG"
    check_value r0,r1,r2, SRAMMC2PLB4_1_CID, CID_RESET_VAL,"SRAMMC2PLB4_1_CID"
    check_value r0,r1,r2, SRAMMC2PLB4_1_REVID, REVID_RESET_VAL,"SRAMMC2PLB4_1_REVID"
    check_value r0,r1,r2, SRAMMC2PLB4_1_DPC, DPC_RESET_VAL,"SRAMMC2PLB4_1_DPC"

check_cpu_ppc_srammc2plb4_1_3:
    load_const r1, 0x00000000
    load_const r0, SRAMMC2PLB4_1_BEAR
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 1 init)")

srammc2plb4_1_running_1:
    load_const r1, 0x00000001
    load_const r3, 1
    load_const r4, 0x80000000
srammc2plb4_1_loop_1:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 1)")
    cmp 0, 0, r1, r4
    beq srammc2plb4_1_running_0
    slw r1, r1, r3
    b srammc2plb4_1_loop_1

srammc2plb4_1_running_0:
    load_const r1, 0xFFFFFFFF
    load_const r0, SRAMMC2PLB4_1_BEAR
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 0 init)")

    load_const r1, 0xFFFFFFFE
    load_const r3, 1
    load_const r4, 0x7FFFFFFF
srammc2plb4_1_loop_0:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 0)")
    cmp 0, 0, r1, r4
    beq test_ok
    slw r1, r1, r3
    ori r1, r1, 1
    b srammc2plb4_1_loop_0

test_ok:
    load_const r1, BEAR_RESET_VAL
    load_const r0, SRAMMC2PLB4_0_BEAR
    mtdcrx r0, r1
    load_const r0, SRAMMC2PLB4_1_BEAR
    mtdcrx r0, r1
    test_event EVENT_OK
    li r3, 0x00
    b finish

error:
     test_event EVENT_ERROR
     li r3, 0x01

finish:
	blr
    // b finish

