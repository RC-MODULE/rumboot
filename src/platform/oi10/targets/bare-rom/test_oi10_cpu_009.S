
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/devices.h>

#define DEBUG_DCR_TEST

#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_plb6plb4.h>

#define P640(OFFSET)			DCR_PLB6PLB4_0_BASE + (OFFSET)
#define P641(OFFSET)			DCR_PLB6PLB4_1_BASE + (OFFSET)
#define CVR3					r10,r11,r12

#define MMUCR_REALE 0x80000000

/*                                    LL             */
/*                    |  ERPN  |      11  SSSUUUUUUU */
/*                  --9876543210UWIMGEID--XWRXWR3210 */
#define RMPD_DATA 0b00000000000000101011001011010000


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:

    gtube_init

    load_const  r0, 0x00000000  /* make r0 "all zeros" source*/
    load_const  r1, 0xFFFFFFFF  /* make r1 "all ones" source*/
    load_const  r2, 0x00000000
    load_const  r3, 0x00000000
    load_const  r4, 0x00000000
    load_const  r5, 0x00000000
    load_const  r6, 0x00000000
    load_const  r7, 0x00000000
    load_const  r8, 0x00000000
    load_const  r9, 0x00000000
    load_const  r10,0x00000000
    load_const  r11,0x00000000
    load_const  r12,0x00000000
    load_const  r13,0x00000000
    load_const  r14,0x00000000
    load_const  r15,0x00000000
    load_const  r16,0x00000000
    load_const  r17,0x00000000
    load_const  r18,0x00000000
    load_const  r19,0x00000000
    load_const  r20,0x00000000
    load_const  r21,0x00000000
    load_const  r22,0x00000000
    load_const  r23,0x00000000
    load_const  r24,0x00000000
    load_const  r25,0x00000000
    load_const  r26,0x00000000
    load_const  r27,0x00000000
    load_const  r28,0x00000000
    load_const  r29,0x00000000
    load_const  r30,0x00000000
    load_const  r31,0x00000000

	rumboot_putstring "Init SPRs... "

    mtspr       SPR_USPGR0,	r0

    mtspr       SPR_SRR0,	r0
    mtspr       SPR_SRR1,	r0
    mtspr       SPR_CSRR0,	r0
    mtspr       SPR_CSRR1,	r0
    mtspr       SPR_MCSRR0,	r0
    mtspr       SPR_MCSRR1,	r0
    mtspr       SPR_MCSR_C,	r1
    mtspr       SPR_ESR,	r0
    mtspr       SPR_DCESR,	r0
    mtspr       SPR_DEAR,	r0

/*according to User's Manual ch. 9.4 Initialization Software Requirements*/
    mtspr       SPR_DBCR0, r0       /* disable all debug events*/

    load_const  r3, (0b1    << DEBUG_DBSR_IDE_i)\
                  | (0b1    << DEBUG_DBSR_UDE_i)\
                  | (0b1    << DEBUG_DBSR_ICMP_i)\
                  | (0b1    << DEBUG_DBSR_BRT_i)\
                  | (0b1    << DEBUG_DBSR_IRPT_i)\
                  | (0b1    << DEBUG_DBSR_TRAP_i)\
                  | (0b1    << DEBUG_DBSR_IAC1_i)\
                  | (0b1    << DEBUG_DBSR_IAC2_i)\
                  | (0b1    << DEBUG_DBSR_IAC3_i)\
                  | (0b1    << DEBUG_DBSR_IAC4_i)\
                  | (0b1    << DEBUG_DBSR_DAC1R_i)\
                  | (0b1    << DEBUG_DBSR_DAC1W_i)\
                  | (0b1    << DEBUG_DBSR_DAC2R_i)\
                  | (0b1    << DEBUG_DBSR_DAC2W_i)\
                  | (0b1    << DEBUG_DBSR_RET_i)\
                  | (0b1    << DEBUG_DBSR_IAC12ATS_i)\
                  | (0b1    << DEBUG_DBSR_IAC34ATS_i)
    mtspr       SPR_DBSR_RC, r3     /* clear all debug interrupts*/

    load_const  r3, (0b0                            << CTRL_CCR0_ITE_i)\
                  | (0b1                            << CTRL_CCR0_PRE_i)\
                  | (0b1                            << CTRL_CCR0_CRPE_i)\
                  | (CTRL_CCR0_ICS_32byte           << CTRL_CCR0_ICS_i)\
                  | (0b0                            << CTRL_CCR0_DAPUIB_i)\
                  | (0b0000                         << CTRL_CCR0_ICWRIDX_i)\
                  | (0b0                            << CTRL_CCR0_DTB_i)\
                  | (0b0                            << CTRL_CCR0_FLSTA_i)\
                  | (CTRL_CCR0_DQWPM_No_prediction  << CTRL_CCR0_DQWPM_i)\
                  | (CTRL_CCR0_IQWPM_Use_EA         << CTRL_CCR0_IQWPM_i)
    mtspr       SPR_CCR0, r3

    load_const  r3, (0b00                       << CTRL_CCR1_GPRPEI_i)\
                  | (0b00                       << CTRL_CCR1_FPRPEI_i)\
                  | (0b00                       << CTRL_CCR1_ICDPEI_i)\
                  | (0b00                       << CTRL_CCR1_ICLPEI_i)\
                  | (0b00                       << CTRL_CCR1_ICTPEI_i)\
                  | (0b00                       << CTRL_CCR1_DCDPEI_i)\
                  | (0b00                       << CTRL_CCR1_DCLPEI_i)\
                  | (0b00                       << CTRL_CCR1_DCTPEI_i)\
                  | (0b0                        << CTRL_CCR1_MMUTPEI_i)\
                  | (0b0                        << CTRL_CCR1_MMUDPEI_i)\
                  | (CTRL_CCR1_TSS_CPU_clock    << CTRL_CCR1_TSS_i)\
                  | (0b0                        << CTRL_CCR1_DPC_i)\
                  | (CTRL_CCR1_TCS_div1         << CTRL_CCR1_TCS_i)
    mtspr       SPR_CCR1, r3

    load_const  r3, (CTRL_CCR2_DSTG_enabled << CTRL_CCR2_DSTG_i)\
                  | (0b0                    << CTRL_CCR2_DLFPD_i)\
                  | (0b0                    << CTRL_CCR2_DSTI_i)\
                  | (0b0                    << CTRL_CCR2_PMUD_i)\
                  | (0b0                    << CTRL_CCR2_DCSTGW_i)\
                  | (0                      << CTRL_CCR2_STGCTR_i)\
                  | (0b0                    << CTRL_CCR2_DISTG_i)\
                  | (0b0                    << CTRL_CCR2_SPC5C1_i)\
                  | (0b0                    << CTRL_CCR2_MCDTO_i)
    mtspr       SPR_CCR2, r3


/*Before editing TLB let's initialize all SPR registers to default values by the spec*/
/*Most of them have X-state after reset and therefore CPU model cannot work properly*/

    mtspr       SPR_LR,		r0
    mtspr       SPR_CTR,	r0
    mtcr        r0
    mtspr       SPR_XER,	r0

    mtspr       SPR_TCR,	r0
    mtspr       SPR_TBL_W,	r0
    mtspr       SPR_TBU_W,	r0
    mtspr       SPR_DEC,	r0
    mtspr       SPR_DECAR,	r0
    load_const  r3, (0b1    << TIMER_TSR_EVW_i)\
                  | (0b1    << TIMER_TSR_WIS_i)\
                  | (0b1    << TIMER_TSR_DIS_i)\
                  | (0b1    << TIMER_TSR_FIS_i)
    mtspr       SPR_TSR_RC, r3

    mtspr       SPR_IAC1,	r0
    mtspr       SPR_IAC2,	r0
    mtspr       SPR_IAC3,	r0
    mtspr       SPR_IAC4,	r0
    mtspr       SPR_DAC1,	r0
    mtspr       SPR_DAC2,	r0
    mtspr       SPR_DVC1,	r0
    mtspr       SPR_DVC2,	r0
    mtspr       SPR_DBCR1,	r0
    mtspr       SPR_DBCR2,	r0
    mtspr       SPR_DBDR,	r0
    mtspr       SPR_RMPD,	r0

	rumboot_putstring "Done!\n"





test_ok:
	rumboot_putstring "TEST_OK!\n"
    test_event EVENT_OK
    exit RESULT_OK
    b test_finish

test_error:
	rumboot_putstring "TEST_ERROR!\n"
    test_event EVENT_ERROR
    exit RESULT_ERROR
    b test_finish

test_finish:
    test_event EVENT_FINISHED
test_finish_loop:
    b test_finish_loop

.section ".text","ax",@progbits

begin_test:
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

	b test_ok




