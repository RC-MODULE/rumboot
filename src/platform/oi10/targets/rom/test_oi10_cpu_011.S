
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/oi10/platform/regs/regs_srammc2plb4.h>

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


//
#define BEAR_RESET_VAL			0x00000000
#define BESR0_RESET_VAL			0x00000000
#define BESR1_RESET_VAL			0x00000000
#define PMEG_RESET_VAL			reg_field(10,0xF)
#define CID_RESET_VAL			0x322B0000
#define REVID_RESET_VAL			0x00000160

#define SYS_DATA_PARITY         0
#define DPC_RESET_VAL			reg_field(0,SYS_DATA_PARITY)

.section ".text","ax",@progbits

.global image_start
.global main

image_start:
main:

check_cpu_ppc_srammc2plb4_0_2:
    check_value r4,r5,r6, SRAMMC2PLB4_0_SB0CR, SRAMMC2PLB4_0_SB0CR_RESET_VAL, "SRAMMC2PLB4_0_SB0CR"
    check_value r4,r5,r6, SRAMMC2PLB4_0_SB1CR, SRAMMC2PLB4_0_SB1CR_RESET_VAL, "SRAMMC2PLB4_0_SB1CR"
    check_value r4,r5,r6, SRAMMC2PLB4_0_SB2CR, SRAMMC2PLB4_0_SB2CR_RESET_VAL, "SRAMMC2PLB4_0_SB2CR"
    check_value r4,r5,r6, SRAMMC2PLB4_0_SB3CR, SRAMMC2PLB4_0_SB3CR_RESET_VAL, "SRAMMC2PLB4_0_SB3CR"
    check_value r4,r5,r6, SRAMMC2PLB4_0_BEAR, BEAR_RESET_VAL, "SRAMMC2PLB4_0_BEAR"
    check_value r4,r5,r6, SRAMMC2PLB4_0_BESR0, BESR0_RESET_VAL, "SRAMMC2PLB4_0_BESR0"
    check_value r4,r5,r6, SRAMMC2PLB4_0_BESR1, BESR1_RESET_VAL, "SRAMMC2PLB4_0_BESR1"
    check_value r4,r5,r6, SRAMMC2PLB4_0_PMEG, PMEG_RESET_VAL, "SRAMMC2PLB4_0_PMEG"
    check_value r4,r5,r6, SRAMMC2PLB4_0_CID, CID_RESET_VAL, "SRAMMC2PLB4_0_CID"
    check_value r4,r5,r6, SRAMMC2PLB4_0_REVID, REVID_RESET_VAL, "SRAMMC2PLB4_0_REVID"
    check_value r4,r5,r6, SRAMMC2PLB4_0_DPC, DPC_RESET_VAL, "SRAMMC2PLB4_0_DPC"

check_cpu_ppc_srammc2plb4_0_3:
    load_const r5, 0x00000000
    load_const r4, SRAMMC2PLB4_0_BEAR
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 1 init)")

srammc2plb4_0_running_1:
    load_const r5, 0x00000001
    load_const r7, 1
    load_const r8, 0x80000000
srammc2plb4_0_loop_1:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 1)")
    cmp 0, 0, r5, r8
    beq srammc2plb4_0_running_0
    slw r5, r5, r7
    b srammc2plb4_0_loop_1

srammc2plb4_0_running_0:
    load_const r5, 0xFFFFFFFF
    load_const r4, SRAMMC2PLB4_0_BEAR
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 0 init)")

    load_const r5, 0xFFFFFFFE
    load_const r7, 1
    load_const r8, 0x7FFFFFFF
srammc2plb4_0_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 0)")
    cmp cr0,0, r5,r8
    beq check_cpu_ppc_srammc2plb4_1_2
    slw r5, r5, r7
    ori r5, r5, 1
    b srammc2plb4_0_loop_0

check_cpu_ppc_srammc2plb4_1_2:
    check_value r4,r5,r6, SRAMMC2PLB4_1_SB0CR, SRAMMC2PLB4_1_SB0CR_RESET_VAL,"SRAMMC2PLB4_1_SB0CR"
    check_value r4,r5,r6, SRAMMC2PLB4_1_SB1CR, SRAMMC2PLB4_1_SB1CR_RESET_VAL,"SRAMMC2PLB4_1_SB1CR"
    check_value r4,r5,r6, SRAMMC2PLB4_1_SB2CR, SRAMMC2PLB4_1_SB2CR_RESET_VAL,"SRAMMC2PLB4_1_SB2CR"
    check_value r4,r5,r6, SRAMMC2PLB4_1_SB3CR, SRAMMC2PLB4_1_SB3CR_RESET_VAL,"SRAMMC2PLB4_1_SB3CR"
    check_value r4,r5,r6, SRAMMC2PLB4_1_BEAR, BEAR_RESET_VAL,"SRAMMC2PLB4_1_BEAR"
    check_value r4,r5,r6, SRAMMC2PLB4_1_BESR0, BESR0_RESET_VAL,"SRAMMC2PLB4_1_BESR0"
    check_value r4,r5,r6, SRAMMC2PLB4_1_BESR1, BESR1_RESET_VAL,"SRAMMC2PLB4_1_BESR1"
    check_value r4,r5,r6, SRAMMC2PLB4_1_PMEG, PMEG_RESET_VAL,"SRAMMC2PLB4_1_PMEG"
    check_value r4,r5,r6, SRAMMC2PLB4_1_CID, CID_RESET_VAL,"SRAMMC2PLB4_1_CID"
    check_value r4,r5,r6, SRAMMC2PLB4_1_REVID, REVID_RESET_VAL,"SRAMMC2PLB4_1_REVID"
    check_value r4,r5,r6, SRAMMC2PLB4_1_DPC, DPC_RESET_VAL,"SRAMMC2PLB4_1_DPC"

check_cpu_ppc_srammc2plb4_1_3:
    load_const r5, 0x00000000
    load_const r4, SRAMMC2PLB4_1_BEAR
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 1 init)")

srammc2plb4_1_running_1:
    load_const r5, 0x00000001
    load_const r7, 1
    load_const r8, 0x80000000
srammc2plb4_1_loop_1:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 1)")
    cmp 0, 0, r5, r8
    beq srammc2plb4_1_running_0
    slw r5, r5, r7
    b srammc2plb4_1_loop_1

srammc2plb4_1_running_0:
    load_const r5, 0xFFFFFFFF
    load_const r4, SRAMMC2PLB4_1_BEAR
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 0 init)")

    load_const r5, 0xFFFFFFFE
    load_const r7, 1
    load_const r8, 0x7FFFFFFF
srammc2plb4_1_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 0)")
    cmp 0, 0, r5, r8
    beq test_ok
    slw r5, r5, r7
    ori r5, r5, 1
    b srammc2plb4_1_loop_0

test_ok:
    load_const r5, BEAR_RESET_VAL
    load_const r4, SRAMMC2PLB4_0_BEAR
    mtdcrx r4, r5
    load_const r0, SRAMMC2PLB4_1_BEAR
    mtdcrx r4, r5
    test_event EVENT_OK
    li r3, 0x00
    b finish

error:
     test_event EVENT_ERROR
     li r3, 0x01

finish:
	blr
    // b finish

