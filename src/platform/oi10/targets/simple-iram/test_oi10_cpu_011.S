
#define DEBUG_DCR_TEST
#define DEBUG_PUT2USPRG0

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_srammc2plb4.h>


/* values from units/cpu/units/ppc_chip/verilog/rtl/PPC470S_TOP111323221111.v */
#define SB0CR_0_RESET_VAL   0xFFFF048F
#define SB1CR_0_RESET_VAL   0xFFFF448F
#define SB2CR_0_RESET_VAL   0xFFFF848F
#define SB3CR_0_RESET_VAL   0xFFFFC48F
#define SB0CR_1_RESET_VAL   0x80000780
#define SB1CR_1_RESET_VAL   0x80008780
#define SB2CR_1_RESET_VAL   0x80010780
#define SB3CR_1_RESET_VAL   0x80018780

#define SRC2P40(OFFSET)     (DCR_SRAMMC2PLB4_0_BASE + (OFFSET))
#define SRC2P41(OFFSET)     (DCR_SRAMMC2PLB4_1_BASE + (OFFSET))

#define CVR3                r14,r15,r16

.section ".text","ax",@progbits

.global main

main:
    load_const  r3,   0x00000000
    cmpwi cr6,  r3,   0x0000      /* General test status in cr6 */

check_cpu_ppc_srammc2plb4_0_2:
    rumboot_putstring "Check reset values of SRAMMC2PLB4_0\n"
    check_value CVR3, SRC2P40(SRAMMC2PLB4_SB0CR), SB0CR_0_RESET_VAL,        "SRAMMC2PLB4_0_SB0CR"
    check_value CVR3, SRC2P40(SRAMMC2PLB4_SB1CR), SB1CR_0_RESET_VAL,        "SRAMMC2PLB4_0_SB1CR"
    check_value CVR3, SRC2P40(SRAMMC2PLB4_SB2CR), SB2CR_0_RESET_VAL,        "SRAMMC2PLB4_0_SB2CR"
    check_value CVR3, SRC2P40(SRAMMC2PLB4_SB3CR), SB3CR_0_RESET_VAL,        "SRAMMC2PLB4_0_SB3CR"
    check_value CVR3, SRC2P40(SRAMMC2PLB4_BEAR ), SRAMMC2PLB4_BEAR_DFLT,    "SRAMMC2PLB4_0_BEAR "
    check_value CVR3, SRC2P40(SRAMMC2PLB4_BESR0), SRAMMC2PLB4_BESRx_DFLT,   "SRAMMC2PLB4_0_BESR0"
    check_value CVR3, SRC2P40(SRAMMC2PLB4_BESR1), SRAMMC2PLB4_BESRx_DFLT,   "SRAMMC2PLB4_0_BESR1"
    check_value CVR3, SRC2P40(SRAMMC2PLB4_PMEG ), SRAMMC2PLB4_PMEG_DFLT,    "SRAMMC2PLB4_0_PMEG "
    check_value CVR3, SRC2P40(SRAMMC2PLB4_CID  ), SRAMMC2PLB4_CID_DFLT,     "SRAMMC2PLB4_0_CID  "
    check_value CVR3, SRC2P40(SRAMMC2PLB4_REVID), SRAMMC2PLB4_REVID_DFLT,   "SRAMMC2PLB4_0_REVID"
    check_value CVR3, SRC2P40(SRAMMC2PLB4_DPC  ), SRAMMC2PLB4_DPC_DFLT,     "SRAMMC2PLB4_0_DPC  "
    rumboot_putstring "Done!\n"

check_cpu_ppc_srammc2plb4_0_3:
    rumboot_putstring "Check running 0/1 in SRAMMC2PLB4_0...\n"
    rumboot_putstring "Init:00000000\n"
    load_const  r15,  0x00000000
    load_const  r14,  SRC2P40(SRAMMC2PLB4_BEAR)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
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
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 1)")
    cmpw cr0,   r15,  r18
    beq- cr0,   srammc2plb4_0_running_0
    slw         r15,  r15,  r17
    b           srammc2plb4_0_loop_1

srammc2plb4_0_running_0:
    rumboot_putstring "Init:FFFFFFFF\n"
    load_const  r15,  0xFFFFFFFF
    load_const  r14,  SRC2P40(SRAMMC2PLB4_BEAR)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 0 init)")

    rumboot_putstring "Running <0>\n"
    load_const  r15,  0xFFFFFFFE
    load_const  r17,  0x00000001
    load_const  r18,  0x7FFFFFFF
srammc2plb4_0_loop_0:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_0_BEAR (running 0)")
    cmpw cr0,   r15,  r18
    beq check_cpu_ppc_srammc2plb4_1_2
    slw         r15,  r15,  r17
    ori         r15,  r15,  0x00000001
    b srammc2plb4_0_loop_0

check_cpu_ppc_srammc2plb4_1_2:
    rumboot_putstring "Done!\n"
    rumboot_putstring "Check reset values of SRAMMC2PLB4_1...\n"
    check_value CVR3, SRC2P41(SRAMMC2PLB4_SB0CR), SB0CR_1_RESET_VAL,        "SRAMMC2PLB4_1_SB0CR"
    check_value CVR3, SRC2P41(SRAMMC2PLB4_SB1CR), SB1CR_1_RESET_VAL,        "SRAMMC2PLB4_1_SB1CR"
    check_value CVR3, SRC2P41(SRAMMC2PLB4_SB2CR), SB2CR_1_RESET_VAL,        "SRAMMC2PLB4_1_SB2CR"
    check_value CVR3, SRC2P41(SRAMMC2PLB4_SB3CR), SB3CR_1_RESET_VAL,        "SRAMMC2PLB4_1_SB3CR"
    check_value CVR3, SRC2P41(SRAMMC2PLB4_BEAR ), SRAMMC2PLB4_BEAR_DFLT,    "SRAMMC2PLB4_1_BEAR "
    check_value CVR3, SRC2P41(SRAMMC2PLB4_BESR0), SRAMMC2PLB4_BESRx_DFLT,   "SRAMMC2PLB4_1_BESR0"
    check_value CVR3, SRC2P41(SRAMMC2PLB4_BESR1), SRAMMC2PLB4_BESRx_DFLT,   "SRAMMC2PLB4_1_BESR1"
    check_value CVR3, SRC2P41(SRAMMC2PLB4_PMEG ), SRAMMC2PLB4_PMEG_DFLT,    "SRAMMC2PLB4_1_PMEG "
    check_value CVR3, SRC2P41(SRAMMC2PLB4_CID  ), SRAMMC2PLB4_CID_DFLT,     "SRAMMC2PLB4_1_CID  "
    check_value CVR3, SRC2P41(SRAMMC2PLB4_REVID), SRAMMC2PLB4_REVID_DFLT,   "SRAMMC2PLB4_1_REVID"
    check_value CVR3, SRC2P41(SRAMMC2PLB4_DPC  ), SRAMMC2PLB4_DPC_DFLT,     "SRAMMC2PLB4_1_DPC  "
    rumboot_putstring "Done!\n"

check_cpu_ppc_srammc2plb4_1_3:
    rumboot_putstring "Check running 0/1 in SRAMMC2PLB4_1...\n"
    rumboot_putstring "Init:00000000\n"
    load_const  r15,  0x00000000
    load_const  r14,  SRC2P41(SRAMMC2PLB4_BEAR)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
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
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 1)")
    cmpw cr0,   r15,  r18
    beq- cr0,   srammc2plb4_1_running_0
    slw         r15,  r15,  r17
    b           srammc2plb4_1_loop_1

srammc2plb4_1_running_0:
    rumboot_putstring "Init:FFFFFFFF\n"
    load_const  r15,  0xFFFFFFFF
    load_const  r14,  SRC2P41(SRAMMC2PLB4_BEAR)
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 0 init)")
    rumboot_putstring "Running <0>\n"
    load_const  r15,  0xFFFFFFFE
    load_const  r17,  0x00000001
    load_const  r18,  0x7FFFFFFF
srammc2plb4_1_loop_0:
    mtdcrx      r14,  r15
    mfdcrx      r16,  r14
    cmpw cr7,   r16,  r15
    TEST_ASSERT(eq,cr7,"Read/write error in SRAMMC2PLB4_1_BEAR (running 0)")
    cmpw cr0,   r15,  r18
    beq- cr0,   test_end
    slw         r15,  r15,  r17
    ori         r15,  r15,  0x0001
    b           srammc2plb4_1_loop_0

test_end:
    rumboot_putstring "Done!\n"
    load_const  r14,  SRC2P40(SRAMMC2PLB4_BEAR)
    load_const  r15,  SRC2P41(SRAMMC2PLB4_BEAR)
    load_const  r16,  SRAMMC2PLB4_BEAR_DFLT
    mtdcrx      r14,  r16
    mtdcrx      r15,  r16
    bne- cr6,   test_error

test_ok:
    rumboot_putstring "TEST OK\n"
    test_event        EVENT_OK
    load_const r3,    RESULT_OK
	b finish

test_error:
    rumboot_putstring "TEST ERROR\n"
    test_event        EVENT_ERROR
    load_const r3,    RESULT_ERROR

finish:
    blr

