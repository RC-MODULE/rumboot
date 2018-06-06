
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>

#define DEBUG_DCR_TEST

#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/oi10/platform/regs/regs_plb6plb4.h>

#define P640(OFFSET)			DCR_PLB6PLB4_0_BASE + (OFFSET)
#define P641(OFFSET)			DCR_PLB6PLB4_1_BASE + (OFFSET)
#define CVR3					r10,r11,r12


.section ".text","ax",@progbits

.global main

main:

check_cpu_ppc_plb6plb4_0_2:
	rumboot_putstring "check_cpu_ppc_plb6plb4_0_2\n"
	check_value CVR3, P640(P64CR ),	P64CR_0_RESET_VAL,				"PLB6PLB4_0_P64CR"
	check_value CVR3, P640(ESR   ),	ESR_RESET_VAL,					"PLB6PLB4_0_ESR"
	check_value CVR3, P640(P64REV),	P64REV_RESET_VAL				"PLB6PLB4_0_P64REV"
	check_value CVR3, P640(EARH  ),	EARH_RESET_VAL,					"PLB6PLB4_0_EARH"
	check_value CVR3, P640(EARL  ),	EARL_RESET_VAL,					"PLB6PLB4_0_EARL"
	check_value CVR3, P640(ADDRL0),	PLB6PLB4_0_ADDRL0_RESET_VAL,	"PLB6PLB4_0_ADDRL0"
	check_value CVR3, P640(ADDRH0),	PLB6PLB4_0_ADDRH0_RESET_VAL,	"PLB6PLB4_0_ADDRH0"
	check_value CVR3, P640(ADDRL1),	ADDRL1_0_RESET_VAL,				"PLB6PLB4_0_ADDRL1"
	check_value CVR3, P640(ADDRH1),	ADDRH1_0_RESET_VAL,				"PLB6PLB4_0_ADDRH1"
	check_value CVR3, P640(TESR  ),	TESR_RESET_VAL,					"PLB6PLB4_0_TESR"

check_cpu_ppc_plb6plb4_0_3:
	rumboot_putstring "check_cpu_ppc_plb6plb4_0_3\n"
    load_const r11, 0x00000000
    load_const r10, P640(ADDRL1)
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL0")

plb6plb4_0_running_1:
    load_const r11,	0x00000001
    load_const r3,	0x00000001
    load_const r4,	0x80000000
plb6plb4_0_loop_1:
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL1 (running 1)")
    cmp 0, 0, r11, r4
    beq plb6plb4_0_running_0
    slw r11, r11, r3
    b plb6plb4_0_loop_1

plb6plb4_0_running_0:
    load_const r11, 0xFFFFFFFF
    load_const r10, P640(ADDRL1)
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL1 (running 0)")

    load_const r11,	0xFFFFFFFE
    load_const r3,	0x00000001
    load_const r4,	0x7FFFFFFF
plb6plb4_0_loop_0:
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL1 (running 0)")
    cmp 0, 0, r11, r4
    beq end_plb6plb4_0
    slw r11, r11, r3
    ori r11, r11, 1
    b plb6plb4_0_loop_0
end_plb6plb4_0:

check_cpu_ppc_plb6plb4_1_2:
	rumboot_putstring "check_cpu_ppc_plb6plb4_1_2\n"
	check_value CVR3, P641(P64CR ), P64CR_RESET_VAL,				"PLB6PLB4_1_P64CR"
	check_value CVR3, P641(ESR   ), ESR_RESET_VAL,					"PLB6PLB4_1_ESR"
	check_value CVR3, P641(P64REV), P64REV_RESET_VAL,				"PLB6PLB4_1_P64REV"
	check_value CVR3, P641(EARH  ), EARH_RESET_VAL,					"PLB6PLB4_1_EARH"
	check_value CVR3, P641(EARL  ), EARL_RESET_VAL,					"PLB6PLB4_1_EARL"
	check_value CVR3, P641(ADDRL0), PLB6PLB4_1_ADDRL0_RESET_VAL,	"PLB6PLB4_1_ADDRL0"
	check_value CVR3, P641(ADDRH0), PLB6PLB4_1_ADDRH0_RESET_VAL,	"PLB6PLB4_1_ADDRH0"
	check_value CVR3, P641(ADDRL1), ADDRL1_RESET_VAL,				"PLB6PLB4_1_ADDRL1"
	check_value CVR3, P641(ADDRH1), ADDRH1_RESET_VAL,				"PLB6PLB4_1_ADDRH1"
	check_value CVR3, P641(TESR  ), TESR_RESET_VAL,					"PLB6PLB4_1_TESR"

check_cpu_ppc_plb6plb4_1_3:
	rumboot_putstring "check_cpu_ppc_plb6plb4_1_3\n"
    load_const r11, 0x00000000
    load_const r10, P641(ADDRL1)
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1")

plb6plb4_1_running_1:
    load_const r11,	0x00000001
    load_const r3,	0x00000001
    load_const r4,	0x80000000
plb6plb4_1_loop_1:
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1 (running 1)")
    cmp 0, 0, r11, r4
    beq plb6plb4_1_running_0
    slw r11, r11, r3
    b plb6plb4_1_loop_1

plb6plb4_1_running_0:
    load_const r11, 0xFFFFFFFF
    load_const r10, P641(ADDRL1)
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1 (running 0)")

    load_const r11,	0xFFFFFFFE
    load_const r3,	0x00000001
    load_const r4,	0x7FFFFFFF
plb6plb4_1_loop_0:
    mtdcrx r10, r11
    mfdcrx r12, r10
    cmp cr7,0, r12,r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1 (running 0)")
    cmp 0, 0, r11, r4
    beq end_plb6plb4_1
    slw r11, r11, r3
    ori r11, r11, 1
    b plb6plb4_1_loop_0
end_plb6plb4_1:

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


