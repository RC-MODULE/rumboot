
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_mmu.S.h>
#include <platform/oi10/platform/ppc470s/mmu.S.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/oi10/platform/regs/regs_mpic128.h>
#include <platform/oi10/platform/regs/regs_plb6bc.h>

/* Uncomment for extra debug info */
/*
#define DEBUG_DCR_TEST 1
*/

#define SGD_NON_SYNTH_VALUE         0xFFFFFFF0

#define CONST_SPR_CCR0 (0b0 << CTRL_CCR0_ITE_i)\
                  | (0b1 << CTRL_CCR0_PRE_i)\
                  | (0b0 << CTRL_CCR0_CRPE_i)\
                  | (CTRL_CCR0_ICS_32byte << CTRL_CCR0_ICS_i)\
                  | (0b0 << CTRL_CCR0_DAPUIB_i)\
                  | (0b0000 << CTRL_CCR0_ICWRIDX_i)\
                  | (0b0 << CTRL_CCR0_DTB_i)\
                  | (0b0 << CTRL_CCR0_FLSTA_i)\
                  | (CTRL_CCR0_DQWPM_No_prediction << CTRL_CCR0_DQWPM_i)\
                  | (CTRL_CCR0_IQWPM_Use_EA << CTRL_CCR0_IQWPM_i)

#define CONST_SPR_CCR1 (0b00 << CTRL_CCR1_GPRPEI_i)\
                  | (0b00 << CTRL_CCR1_FPRPEI_i)\
                  | (0b00 << CTRL_CCR1_ICDPEI_i)\
                  | (0b00 << CTRL_CCR1_ICLPEI_i)\
                  | (0b00 << CTRL_CCR1_ICTPEI_i)\
                  | (0b00 << CTRL_CCR1_DCDPEI_i)\
                  | (0b00 << CTRL_CCR1_DCLPEI_i)\
                  | (0b00 << CTRL_CCR1_DCTPEI_i)\
                  | (0b0 << CTRL_CCR1_MMUTPEI_i)\
                  | (0b0 << CTRL_CCR1_MMUDPEI_i)\
                  | (CTRL_CCR1_TSS_CPU_clock << CTRL_CCR1_TSS_i)\
                  | (0b0 << CTRL_CCR1_DPC_i)\
                  | (CTRL_CCR1_TCS_div1 << CTRL_CCR1_TCS_i)

#define CONST_SPR_CCR2 (CTRL_CCR2_DSTG_enabled << CTRL_CCR2_DSTG_i)\
                  | (0b0 << CTRL_CCR2_DLFPD_i)\
                  | (0b0 << CTRL_CCR2_DSTI_i)\
                  | (0b0 << CTRL_CCR2_PMUD_i)\
                  | (0b0 << CTRL_CCR2_DCSTGW_i)\
                  | (0 << CTRL_CCR2_STGCTR_i)\
                  | (0b0 << CTRL_CCR2_DISTG_i)\
                  | (0b0 << CTRL_CCR2_SPC5C1_i)\
                  | (0b0 << CTRL_CCR2_MCDTO_i)

#define CONST_SPR_MMUBE0 (0b0 << MMU_MMUBE0_VBE0_i)\
                  | (0b0 << MMU_MMUBE0_VBE1_i)\
                  | (0b0 << MMU_MMUBE0_VBE2_i)

#define CONST_SPR_MMUBE1 (0b0 << MMU_MMUBE1_VBE3_i)\
                  | (0b0 << MMU_MMUBE1_VBE4_i)\
                  | (0b0 << MMU_MMUBE1_VBE5_i)

#define CONST_SPR_SSPCR ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_SSPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB) << MMU_SSPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_END << MMU_SSPCR_ORD3_i)

#define CONST_SPR_ISPCR (MMU_XSPCR_ORD_64KB << MMU_ISPCR_ORD1_i)\
                  | (MMU_XSPCR_ORD_4KB << MMU_ISPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_END << MMU_ISPCR_ORD3_i)

#define CONST_SPR_USPCR ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_USPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB) << MMU_USPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_END << MMU_USPCR_ORD3_i)

/* DCR register value checker */
.macro sgd_wr_check rE,rA,rR,raddress,reg_name
    /* load address */
    load_const \rA, \raddress
    /* load value */
    load_const \rE, SGD_NON_SYNTH_VALUE
    /* write value to SGDx */
    mtdcrx \rA, \rE
    /* read */
    mfdcrx \rR, \rA
    cmp cr7,0, \rR,\rE
    TEST_ASSERT(eq,cr7,"DCR error: In \reg_name was expected value \exp_value (address \raddress) ")

#ifdef DEBUG_DCR_TEST
    rumboot_putstring "\reg_name OK\n"
#endif
.endm

.section ".init.text","ax",@progbits

rumboot_putstring_unexpected_itrpt:
    rumboot_putstring   "Unexpected interrupt: "
    blr

.align 4
unexpected_itrpt_hdr_base:
unexpected_CI_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Critical input\n"
    b           unexpected_error

.align 4
unexpected_MC_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Machine check\n"
    b           unexpected_error

.align 4
unexpected_DS_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Data storage\n"
    b           unexpected_error

.align 4
unexpected_IS_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Instruction storage\n"
    b           unexpected_error

.align 4
unexpected_E_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "External input\n"
    b           unexpected_error

.align 4
unexpected_A_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Alignment\n"
    b           unexpected_error

.align 4
unexpected_P_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Program\n"
    b           unexpected_error

.align 4
unexpected_FPU_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Floating point unavailable\n"
    b           unexpected_error
  .align 4
unexpected_SC_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "System call\n"
    b           unexpected_error
.align 4
unexpected_APU_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Auxiliary processor unavailable\n"
    b           unexpected_error

.align 4
unexpected_DEC_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Decrementer\n"
    b           unexpected_error

.align 4
unexpected_FITI_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Fixed interval timer\n"
    b           unexpected_error

.align 4
unexpected_WTI_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Watchdog timer\n"
    b           unexpected_error

.align 4
unexpected_DTLBE_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Data TLB error\n"
    b           unexpected_error

.align 4
unexpected_ITLBE_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Instruction TLB error\n"
    b           unexpected_error

 .align 4
unexpected_D_hdr:
    bl          rumboot_putstring_unexpected_itrpt
    rumboot_putstring   "Debug\n"
    b           unexpected_error

.section ".init.start","ax",@progbits

boot_start:
    b           cpu_008_entry_point

.section ".init.text","ax",@progbits

cpu_008_entry_point:
	gtube_init
    load_const  r0, 0x00000000  /* make r0 "all zeros" source */
    load_const  r1, 0xFFFFFFFF  /* make r1 "all ones" source */

    mtspr       SPR_USPGR0, r0

    lis         r3, unexpected_itrpt_hdr_base@h
    mtspr       SPR_IVPR, r3
    li          r3, unexpected_CI_hdr@l
    mtspr       SPR_IVOR0, r3
    li          r3, unexpected_MC_hdr@l
    mtspr       SPR_IVOR1, r3
    li          r3, unexpected_DS_hdr@l
    mtspr       SPR_IVOR2, r3
    li          r3, unexpected_IS_hdr@l
    mtspr       SPR_IVOR3, r3
    li          r3, unexpected_E_hdr@l
    mtspr       SPR_IVOR4, r3
    li          r3, unexpected_A_hdr@l
    mtspr       SPR_IVOR5, r3
    li          r3, unexpected_P_hdr@l
    mtspr       SPR_IVOR6, r3
    li          r3, unexpected_FPU_hdr@l
    mtspr       SPR_IVOR7, r3
    li          r3, unexpected_SC_hdr@l
    mtspr       SPR_IVOR8, r3
    li          r3, unexpected_APU_hdr@l
    mtspr       SPR_IVOR9, r3
    li          r3, unexpected_DEC_hdr@l
    mtspr       SPR_IVOR10, r3
    li          r3, unexpected_FITI_hdr@l
    mtspr       SPR_IVOR11, r3
    li          r3, unexpected_WTI_hdr@l
    mtspr       SPR_IVOR12, r3
    li          r3, unexpected_DTLBE_hdr@l
    mtspr       SPR_IVOR13, r3
    li          r3, unexpected_ITLBE_hdr@l
    mtspr       SPR_IVOR14, r3
    li          r3, unexpected_D_hdr@l
    mtspr       SPR_IVOR15, r3

boot_init:
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

init_spr:
/* according to User's Manual ch. 9.4 Initialization Software Requirements */
    mtspr       SPR_DBCR0,r0       /* disable all debug events */

    mtspr       SPR_DBSR_RC, r1    /* clear all debug interrupts */

    load_const  r3, CONST_SPR_CCR0
    mtspr       SPR_CCR0, r3

    load_const  r3, CONST_SPR_CCR1
    mtspr       SPR_CCR1, r3

    load_const  r3, CONST_SPR_CCR2
    mtspr       SPR_CCR2, r3

    load_const  r3, CONST_SPR_MMUBE0
    mtspr       SPR_MMUBE0, r3

    load_const  r3, CONST_SPR_MMUBE1
    mtspr       SPR_MMUBE1, r3

    load_const  r3, CONST_SPR_SSPCR
    mtspr       SPR_SSPCR, r3

    load_const  r3, CONST_SPR_ISPCR
    mtspr       SPR_ISPCR, r3

    load_const  r3, CONST_SPR_USPCR
    mtspr       SPR_USPCR, r3

/*
	Before editing TLB let's initialize all SPR registers to default values by the spec
	Most of them have X-state after reset and therefore CPU model cannot work properly
*/
    mtspr       SPR_CTR, r0
    mtcr        r0
    mtspr       SPR_LR, r0
    mtspr       SPR_DCESR, r0
    mtspr       SPR_CSRR0, r0
    mtspr       SPR_CSRR1, r0
    mtspr       SPR_MCSRR0, r0
    mtspr       SPR_MCSRR1, r0
    mtspr       SPR_RMPD, r0

    mtspr       SPR_IAC1, r0
    mtspr       SPR_IAC2, r0
    mtspr       SPR_IAC3, r0
    mtspr       SPR_IAC4, r0
    mtspr       SPR_DAC1, r0
    mtspr       SPR_DAC2, r0
    mtspr       SPR_DVC1, r0
    mtspr       SPR_DVC2, r0
    mtspr       SPR_DBCR1, r0
    mtspr       SPR_DBCR2, r0
    mtspr       SPR_DBDR, r0
    mtspr       SPR_MCSR_C, r1
    mtspr       SPR_DEAR, r0
    mtspr       SPR_TCR, r0
    mtspr       SPR_DEC, r0
    mtspr       SPR_DECAR, r0
    mtspr       SPR_TSR_RC, r1
    mtspr       SPR_SRR0, r0
    mtspr       SPR_SRR1, r0
    mtspr       SPR_TBL_W, r0
    mtspr       SPR_TBU_W, r0
    mtspr       SPR_XER, r0
    mtspr       SPR_PID, r0

    b           setup_tlb

infinite_loop_008:
    b infinite_loop_008

tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
/* Invalidate initial TLB entry*/
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xFFFFF,    MMU_TLBE_DSIZ_4KB,      0b0,    0b0,    0b0,    0b0,    0b0,    00,     MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
/* TLB entries to remap 64KB page BOOTROM from 0x000003ff_ffff0000-0x000003ff_ffffffff to 0xffff0000-0xffffffff*/
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
.set TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE

/* implement__write_tlb_entries r3, r4, r8, r9, r10 */

setup_tlb:
    load_addr   r3, tlb_entries
    load_const  r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries

    b test_mpw_cpu_008

.section ".text","ax",@progbits

test_mpw_cpu_008:
	rumboot_putstring "start testing values...\n"
    /* check all default values */
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_CR0,          \
    						PLB6_BC_CR0_VALUE,				"PLB6BC_CR0         "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_PRI,          \
    						PLB6_BC_PRI_VALUE,				"PLB6BC_PRI         "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_TSNOOP,       \
    						PLB6_BC_TSNOOP_VALUE,			"PLB6BC_TSNOOP      "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_PAAM_WIN_EXT, \
    						PLB6_BC_PAAM_WIN_EXT_VALUE,		"PLB6BC_PAAM_WIN_EXT"
    /* non-used segments must be written with value 0xFFFFFFF0. */
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_SGD1,         \
    						PLB6_BC_SGD1_VALUE,				"PLB6BC_SGD1        "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_SGD2,         \
    						PLB6_BC_SGD2_VALUE,				"PLB6BC_SGD2        "
    sgd_wr_check r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_SGD3, "PLB6BC_SGD3        "
    sgd_wr_check r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_SGD4, "PLB6BC_SGD4        "
    sgd_wr_check r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_SGD5, "PLB6BC_SGD5        "
    sgd_wr_check r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_SGD6, "PLB6BC_SGD6        "
    sgd_wr_check r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_SGD7, "PLB6BC_SGD7        "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_ERR,          \
    						PLB6_BC_ERR_VALUE,				"PLB6BC_ERR         "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_MSRSP,        \
    						PLB6_BC_MSRSP_VALUE,			"PLB6BC_MSRSP       "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_HCPP,         \
    						PLB6_BC_HCPP_VALUE,				"PLB6BC_HCPP        "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_HD,           \
    						PLB6_BC_HD_VALUE,				"PLB6BC_HD          "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_QD,           \
    						PLB6_BC_QD_VALUE,				"PLB6BC_QD          "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_SHD,          \
    						PLB6_BC_SHD_VALUE,				"PLB6BC_SHD         "
    check_value r0,r1,r2,	DCR_PLB6_BC_BASE + PLB6BC_REVID,        \
    						PLB6_BC_REVID_VALUE,			"PLB6BC_REVID       "
    /* ADDR bus now is checked */

    /* check DATA I/O buses */
    rumboot_putstring "\nSliding '1' and '0' ...\n"
running_1_low:
	putchar '<'
	putchar '1'
	putchar '>'
	putchar ' '
    load_const r0, DCR_PLB6_BC_BASE + PLB6BC_SGD1   /* pointer */
    load_const r1, 0x10                /* begin val */
    load_const r3, 1                   /* shift */
    load_const r4, 0x80000000          /* max val for compare */
loop_1_low:
	mtdcrx r0, r1
	mfdcrx r2, r0
	cmp cr7, 0, r2,r1
	TEST_ASSERT(eq, cr7, "PLB6 DCRDATA: Running one error")
	cmp 0, 0, r1, r4
	/* beq running_1_hi */
	beq running_0_low
	slw r1, r1, r3
	b loop_1_low

running_0_low:
	putchar '\n'
	putchar '<'
	putchar '0'
	putchar '>'
	putchar ' '
    load_const r1, 0xFFFFFFF0
    load_const r0, DCR_PLB6_BC_BASE + PLB6BC_SGD1
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running null error")

    load_const r1, 0xFFFFFFE0       /* begin val  */
    load_const r3, 1                /* shift      */
    load_const r4, 0x7FFFFFF0       /* finish val */
loop_0_low:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running null error")
    cmp 0, 0, r1, r4
    /* beq running_0_hi */
    beq test_ok
    slw r1, r1, r3
    ori r1, r1, 0x10
    b loop_0_low

test_ok:
    test_event EVENT_FINISHED
    rumboot_putstring "\nTEST OK\n"
    exit RESULT_OK
stop:
    b stop

unexpected_error:
    test_event  EVENT_ERROR
	rumboot_putstring "\nTEST ERROR\n"
    exit RESULT_ERROR



