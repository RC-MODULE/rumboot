
#define DEBUG_DCR_TEST
#define DEBUG_PUT2USPRG0

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_srammc2plb4.h>

#define SB0CR					0x00
#define SB1CR					0x01
#define SB2CR					0x02
#define SB3CR					0x03
#define BEAR					0x04
#define BESR0					0x05
#define BESR1					0x06
#define PMEG					0x07
#define CID						0x08
#define REVID					0x09
#define DPC						0x0A

#define SB0CR_0_RESET_VAL		SRAMMC2PLB4_0_SB0CR_RESET_VAL
#define SB1CR_0_RESET_VAL		SRAMMC2PLB4_0_SB1CR_RESET_VAL
#define SB2CR_0_RESET_VAL		SRAMMC2PLB4_0_SB2CR_RESET_VAL
#define SB3CR_0_RESET_VAL		SRAMMC2PLB4_0_SB3CR_RESET_VAL
#define SB0CR_1_RESET_VAL		SRAMMC2PLB4_1_SB0CR_RESET_VAL
#define SB1CR_1_RESET_VAL		SRAMMC2PLB4_1_SB1CR_RESET_VAL
#define SB2CR_1_RESET_VAL		SRAMMC2PLB4_1_SB2CR_RESET_VAL
#define SB3CR_1_RESET_VAL		SRAMMC2PLB4_1_SB3CR_RESET_VAL
#define BEAR_RESET_VAL			0x00000000
#define BESR0_RESET_VAL			0x00000000
#define BESR1_RESET_VAL			0x00000000
#define PMEG_RESET_VAL			reg_field(10,0x0F)
#define CID_RESET_VAL			0x322B0000
#define REVID_RESET_VAL			0x00000160

#define CVR3					r4,r5,r6

#define SRC2P40(OFFSET)			(DCR_SRAMMC2PLB4_0_BASE + (OFFSET))
#define SRC2P41(OFFSET)			(DCR_SRAMMC2PLB4_1_BASE + (OFFSET))

#define SYS_DATA_PARITY         0
#define DPC_RESET_VAL			reg_field(0,SYS_DATA_PARITY)

.section ".text","ax",@progbits

.global image_start
.global main

image_start:
main:

check_cpu_ppc_srammc2plb4_0_2:
	rumboot_putstring "check_cpu_ppc_srammc2plb4_0_2\n"
    check_value CVR3, SRC2P40(SB0CR), SB0CR_0_RESET_VAL,	"SRAMMC2PLB4_0_SB0CR"
    check_value CVR3, SRC2P40(SB1CR), SB1CR_0_RESET_VAL,	"SRAMMC2PLB4_0_SB1CR"
    check_value CVR3, SRC2P40(SB2CR), SB2CR_0_RESET_VAL,	"SRAMMC2PLB4_0_SB2CR"
    check_value CVR3, SRC2P40(SB3CR), SB3CR_0_RESET_VAL,	"SRAMMC2PLB4_0_SB3CR"
    check_value CVR3, SRC2P40(BEAR ), BEAR_RESET_VAL,		"SRAMMC2PLB4_0_BEAR"
    check_value CVR3, SRC2P40(BESR0), BESR0_RESET_VAL,		"SRAMMC2PLB4_0_BESR0"
    check_value CVR3, SRC2P40(BESR1), BESR1_RESET_VAL,		"SRAMMC2PLB4_0_BESR1"
    check_value CVR3, SRC2P40(PMEG ), PMEG_RESET_VAL,		"SRAMMC2PLB4_0_PMEG"
    check_value CVR3, SRC2P40(CID  ), CID_RESET_VAL,		"SRAMMC2PLB4_0_CID"
    check_value CVR3, SRC2P40(REVID), REVID_RESET_VAL,		"SRAMMC2PLB4_0_REVID"
    check_value CVR3, SRC2P40(DPC  ), DPC_RESET_VAL,		"SRAMMC2PLB4_0_DPC"

check_cpu_ppc_srammc2plb4_0_3:
	rumboot_putstring "check_cpu_ppc_srammc2plb4_0_3\n"
    load_const r5,	0x00000000
    load_const r4,	SRC2P40(BEAR)
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 1 init)")

srammc2plb4_0_running_1:
    load_const r5,	0x00000001
    load_const r7,	0x00000001
    load_const r8,	0x80000000
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
    load_const r5,	0xFFFFFFFF
    load_const r4,	SRC2P40(BEAR)
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 0 init)")

    load_const r5,	0xFFFFFFFE
    load_const r7,	0x00000001
    load_const r8,	0x7FFFFFFF
srammc2plb4_0_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 0)")
    cmp cr0,0, r5,r8
    beq check_cpu_ppc_srammc2plb4_1_2
    slw r5, r5, r7
    ori r5, r5,		0x00000001
    b srammc2plb4_0_loop_0

check_cpu_ppc_srammc2plb4_1_2:
	rumboot_putstring "check_cpu_ppc_srammc2plb4_1_2\n"
    check_value CVR3, SRC2P41(SB0CR), SB0CR_1_RESET_VAL,	"SRAMMC2PLB4_1_SB0CR"
    check_value CVR3, SRC2P41(SB1CR), SB1CR_1_RESET_VAL,	"SRAMMC2PLB4_1_SB1CR"
    check_value CVR3, SRC2P41(SB2CR), SB2CR_1_RESET_VAL,	"SRAMMC2PLB4_1_SB2CR"
    check_value CVR3, SRC2P41(SB3CR), SB3CR_1_RESET_VAL,	"SRAMMC2PLB4_1_SB3CR"
    check_value CVR3, SRC2P41(BEAR ), BEAR_RESET_VAL,		"SRAMMC2PLB4_1_BEAR"
    check_value CVR3, SRC2P41(BESR0), BESR0_RESET_VAL,		"SRAMMC2PLB4_1_BESR0"
    check_value CVR3, SRC2P41(BESR1), BESR1_RESET_VAL,		"SRAMMC2PLB4_1_BESR1"
    check_value CVR3, SRC2P41(PMEG ), PMEG_RESET_VAL,		"SRAMMC2PLB4_1_PMEG"
    check_value CVR3, SRC2P41(CID  ), CID_RESET_VAL,		"SRAMMC2PLB4_1_CID"
    check_value CVR3, SRC2P41(REVID), REVID_RESET_VAL,		"SRAMMC2PLB4_1_REVID"
    check_value CVR3, SRC2P41(DPC  ), DPC_RESET_VAL,		"SRAMMC2PLB4_1_DPC"

check_cpu_ppc_srammc2plb4_1_3:
    load_const r5,	0x00000000
    load_const r4,	SRC2P41(BEAR)
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6, r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 1 init)")

srammc2plb4_1_running_1:
    load_const r5,	0x00000001
    load_const r7,	0x00000001
    load_const r8,	0x80000000
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
    load_const r5,	0xFFFFFFFF
    load_const r4,	SRC2P41(BEAR)
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 0 init)")

    load_const r5,	0xFFFFFFFE
    load_const r7,	0x00000001
    load_const r8,	0x7FFFFFFF
srammc2plb4_1_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,	r5
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 0)")
    cmp cr0,0, r5,	r8
    beq test_end
    slw r5, r5, r7
    ori r5, r5, 1
    b srammc2plb4_1_loop_0

test_end:
    load_const r4,	SRC2P40(BEAR)
    load_const r5,	SRC2P41(BEAR)
    load_const r6,	BEAR_RESET_VAL
    mtdcrx r4, r6
    mtdcrx r5, r6

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


