
#define DEBUG_DCR_TEST
#define DEBUG_PUT2USPRG0

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_srammc2plb4.h>

#define eq6                     cr6*4+eq
#define eq7                     cr7*4+eq

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

.global main

main:
    load_const  r3,   0x00000000
    cmpwi cr6,  r3,   0x0000      /* General test status in cr6 */

check_cpu_ppc_srammc2plb4_0_2:
	rumboot_putstring "Check reset values of SRAMMC2PLB4_0\n"
    check_value CVR3, SRC2P40(SB0CR), SB0CR_0_RESET_VAL, "SRAMMC2PLB4_0_SB0CR"
    check_value CVR3, SRC2P40(SB1CR), SB1CR_0_RESET_VAL, "SRAMMC2PLB4_0_SB1CR"
    check_value CVR3, SRC2P40(SB2CR), SB2CR_0_RESET_VAL, "SRAMMC2PLB4_0_SB2CR"
    check_value CVR3, SRC2P40(SB3CR), SB3CR_0_RESET_VAL, "SRAMMC2PLB4_0_SB3CR"
    check_value CVR3, SRC2P40(BEAR ), BEAR_RESET_VAL,    "SRAMMC2PLB4_0_BEAR "
    check_value CVR3, SRC2P40(BESR0), BESR0_RESET_VAL,   "SRAMMC2PLB4_0_BESR0"
    check_value CVR3, SRC2P40(BESR1), BESR1_RESET_VAL,   "SRAMMC2PLB4_0_BESR1"
    check_value CVR3, SRC2P40(PMEG ), PMEG_RESET_VAL,    "SRAMMC2PLB4_0_PMEG "
    check_value CVR3, SRC2P40(CID  ), CID_RESET_VAL,     "SRAMMC2PLB4_0_CID  "
    check_value CVR3, SRC2P40(REVID), REVID_RESET_VAL,   "SRAMMC2PLB4_0_REVID"
    check_value CVR3, SRC2P40(DPC  ), DPC_RESET_VAL,     "SRAMMC2PLB4_0_DPC  "
    rumboot_putstring "Done!\n"

check_cpu_ppc_srammc2plb4_0_3:
    rumboot_putstring "Check running 0/1 in SRAMMC2PLB4_0...\n"
    rumboot_putstring "Init:00000000\n"
    load_const  r15,  0x00000000
    load_const  r14,  SRC2P40(BEAR)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 1 init)")

srammc2plb4_0_running_1:
    rumboot_putstring "Running <1>\n"
    load_const  r15,  0x00000001
    load_const  r17,  0x00000001
    load_const  r18,  0x80000000
srammc2plb4_0_loop_1:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6, eq7
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 1)")
    cmpw cr0,   r15,  r18
    beq- cr0,   srammc2plb4_0_running_0
    slw         r15,  r15,  r17
    b           srammc2plb4_0_loop_1

srammc2plb4_0_running_0:
    rumboot_putstring "Init:FFFFFFFF\n"
    load_const  r15,  0xFFFFFFFF
    load_const  r14,  SRC2P40(BEAR)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6, eq7
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 0 init)")

    rumboot_putstring "Running <0>\n"
    load_const  r15,  0xFFFFFFFE
    load_const  r17,  0x00000001
    load_const  r18,  0x7FFFFFFF
srammc2plb4_0_loop_0:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6, eq7
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 0)")
    cmpw cr0,   r15,  r18
    beq check_cpu_ppc_srammc2plb4_1_2
    slw         r15,  r15,  r17
    ori         r15,  r15,  0x00000001
    b srammc2plb4_0_loop_0

check_cpu_ppc_srammc2plb4_1_2:
    rumboot_putstring "Done!\n"
    rumboot_putstring "Check reset values of SRAMMC2PLB4_1...\n"
    check_value CVR3, SRC2P41(SB0CR), SB0CR_1_RESET_VAL,	"SRAMMC2PLB4_1_SB0CR"
    check_value CVR3, SRC2P41(SB1CR), SB1CR_1_RESET_VAL,	"SRAMMC2PLB4_1_SB1CR"
    check_value CVR3, SRC2P41(SB2CR), SB2CR_1_RESET_VAL,	"SRAMMC2PLB4_1_SB2CR"
    check_value CVR3, SRC2P41(SB3CR), SB3CR_1_RESET_VAL,	"SRAMMC2PLB4_1_SB3CR"
    check_value CVR3, SRC2P41(BEAR ), BEAR_RESET_VAL,		"SRAMMC2PLB4_1_BEAR "
    check_value CVR3, SRC2P41(BESR0), BESR0_RESET_VAL,		"SRAMMC2PLB4_1_BESR0"
    check_value CVR3, SRC2P41(BESR1), BESR1_RESET_VAL,		"SRAMMC2PLB4_1_BESR1"
    check_value CVR3, SRC2P41(PMEG ), PMEG_RESET_VAL,		"SRAMMC2PLB4_1_PMEG "
    check_value CVR3, SRC2P41(CID  ), CID_RESET_VAL,		"SRAMMC2PLB4_1_CID  "
    check_value CVR3, SRC2P41(REVID), REVID_RESET_VAL,		"SRAMMC2PLB4_1_REVID"
    check_value CVR3, SRC2P41(DPC  ), DPC_RESET_VAL,		"SRAMMC2PLB4_1_DPC  "
    rumboot_putstring "Done!\n"

check_cpu_ppc_srammc2plb4_1_3:
    rumboot_putstring "Check running 0/1 in SRAMMC2PLB4_1...\n"
    rumboot_putstring "Init:00000000\n"
    load_const  r15,  0x00000000
    load_const  r14,  SRC2P41(BEAR)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6, eq7
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 1 init)")

srammc2plb4_1_running_1:
    rumboot_putstring "Running <1>\n"
    load_const  r15,  0x00000001
    load_const  r17,  0x00000001
    load_const  r18,  0x80000000
srammc2plb4_1_loop_1:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6, eq7
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 1)")
    cmpw cr0,   r15,  r18
    beq- cr0,   srammc2plb4_1_running_0
    slw         r15,  r15,  r17
    b           srammc2plb4_1_loop_1

srammc2plb4_1_running_0:
    rumboot_putstring "Init:FFFFFFFF\n"
    load_const  r15,  0xFFFFFFFF
    load_const  r14,  SRC2P41(BEAR)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 0 init)")
    rumboot_putstring "Running <0>\n"
    load_const  r15,  0xFFFFFFFE
    load_const  r17,  0x00000001
    load_const  r18,  0x7FFFFFFF
srammc2plb4_1_loop_0:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    crand       eq6,  eq6,  eq7
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 0)")
    cmpw cr0,   r15,  r18
    beq- cr0,   test_end
    slw         r15,  r15,  r17
    ori         r15,  r15,  0x0001
    b           srammc2plb4_1_loop_0

test_end:
    rumboot_putstring "Done!\n"
    load_const  r14,  SRC2P40(BEAR)
    load_const  r15,  SRC2P41(BEAR)
    load_const  r16,  BEAR_RESET_VAL
    mtdcrx      r14,  r16
    mtdcrx      r15,  r16
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


