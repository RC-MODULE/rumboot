
#define RUMBOOT_ASSERT_WARN_ONLY

#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/regs/regs_mpic128.h>
#include <platform/regs/regs_plb6bc.h>

#define CVR3						r21,r22,r23		/* CheckValue Regs Triade */
#define P6BCA(OFFSET)				(DCR_PLB6_BC_BASE + (OFFSET))
#define SGD_NON_SYNTH_VALUE         0xFFFFFFF0


#define CONST_SPR_CCR0    (0b0    << CTRL_CCR0_ITE_i    )\
						| (0b1    << CTRL_CCR0_PRE_i    )\
						| (0b0    << CTRL_CCR0_CRPE_i   )\
						| (CTRL_CCR0_ICS_32byte          \
								  << CTRL_CCR0_ICS_i    )\
						| (0b0    << CTRL_CCR0_DAPUIB_i )\
						| (0b0000 << CTRL_CCR0_ICWRIDX_i)\
						| (0b0    << CTRL_CCR0_DTB_i    )\
						| (0b0    << CTRL_CCR0_FLSTA_i  )\
						| (CTRL_CCR0_DQWPM_No_prediction \
								  << CTRL_CCR0_DQWPM_i  )\
						| (CTRL_CCR0_IQWPM_Use_EA        \
								  << CTRL_CCR0_IQWPM_i  )

#define CONST_SPR_CCR1    (0b00 << CTRL_CCR1_GPRPEI_i   )\
						| (0b00 << CTRL_CCR1_FPRPEI_i   )\
						| (0b00 << CTRL_CCR1_ICDPEI_i   )\
						| (0b00 << CTRL_CCR1_ICLPEI_i   )\
						| (0b00 << CTRL_CCR1_ICTPEI_i   )\
						| (0b00 << CTRL_CCR1_DCDPEI_i   )\
						| (0b00 << CTRL_CCR1_DCLPEI_i   )\
						| (0b00 << CTRL_CCR1_DCTPEI_i   )\
						| (0b0  << CTRL_CCR1_MMUTPEI_i  )\
						| (0b0  << CTRL_CCR1_MMUDPEI_i  )\
						| (CTRL_CCR1_TSS_CPU_clock       \
								<< CTRL_CCR1_TSS_i      )\
						| (0b0  << CTRL_CCR1_DPC_i      )\
						| (CTRL_CCR1_TCS_div1            \
								<< CTRL_CCR1_TCS_i      )

#define CONST_SPR_CCR2    (CTRL_CCR2_DSTG_enabled        \
								<< CTRL_CCR2_DSTG_i		)\
						| (0b0	<< CTRL_CCR2_DLFPD_i	)\
						| (0b0	<< CTRL_CCR2_DSTI_i		)\
						| (0b0	<< CTRL_CCR2_PMUD_i		)\
						| (0b0	<< CTRL_CCR2_DCSTGW_i	)\
						| (0	<< CTRL_CCR2_STGCTR_i	)\
						| (0b0	<< CTRL_CCR2_DISTG_i	)\
						| (0b0	<< CTRL_CCR2_SPC5C1_i	)\
						| (0b0	<< CTRL_CCR2_MCDTO_i	)

#define CONST_SPR_MMUBE0  (0b0 << MMU_MMUBE0_VBE0_i		)\
						| (0b0 << MMU_MMUBE0_VBE1_i		)\
						| (0b0 << MMU_MMUBE0_VBE2_i		)

#define CONST_SPR_MMUBE1  (0b0 << MMU_MMUBE1_VBE3_i		)\
						| (0b0 << MMU_MMUBE1_VBE4_i		)\
						| (0b0 << MMU_MMUBE1_VBE5_i		)

#define CONST_SPR_SSPCR   ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_SSPCR_ORD1_i)\
						| ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB ) << MMU_SSPCR_ORD2_i)\
						| (                         MMU_XSPCR_ORD_END   << MMU_SSPCR_ORD3_i)

#define CONST_SPR_ISPCR   (                         MMU_XSPCR_ORD_64KB  << MMU_ISPCR_ORD1_i)\
						| (                         MMU_XSPCR_ORD_4KB   << MMU_ISPCR_ORD2_i)\
						| (                         MMU_XSPCR_ORD_END   << MMU_ISPCR_ORD3_i)

#define CONST_SPR_USPCR   ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_USPCR_ORD1_i)\
						| ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB ) << MMU_USPCR_ORD2_i)\
						| (                         MMU_XSPCR_ORD_END   << MMU_USPCR_ORD3_i)

/* DCR register value checker */
.macro chk_sgdwr rE,rA,rR,vAddr,sName
    /* load address */
    load_const	\rA,	\vAddr
    /* load value */
    load_const	\rE,	SGD_NON_SYNTH_VALUE
    /* write value to SGDx */
    mtdcrx		\rA,	\rE
    /* read */
    mfdcrx		\rR,	\rA
    cmpw cr7,	\rR,	\rE
    TEST_ASSERT(eq,cr7,"DCR error: In \sName was expected value \vExp (address: \vAddr) ")
.endm

.macro chk_value rA,rR,rE,vAddr,vExp,sName
    load_const	\rE,	\vExp
    load_const	\rA,	\vAddr
    mfdcrx		\rR,	\rA
    cmpw cr7,	\rR,	\rE
    TEST_ASSERT(eq,cr7,"DCRE: expected \sName = \vExp (address: \vAddr) ")
    rumboot_putstring   "\sName: "
    rumboot_puthex      \rA
    rumboot_puthex      \rR
.endm


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point


.section ".init.text","ax",@progbits

rumboot_entry_point:
	gtube_init
    load_const  r0, 0x00000000  /* make r0 "all zeros" source */
    load_const  r1, 0xFFFFFFFF  /* make r1 "all ones" source */

	rumboot_putstring "Init SPRs...\n"

    mtspr       SPR_USPRG0, r0

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
    load_const  r2,	0x00000000
    load_const  r3,	0x00000000
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
    mtspr       SPR_DBCR0,		r0       /* disable all debug events */

    mtspr       SPR_DBSR_RC,	r1    /* clear all debug interrupts */

    load_spr	r3,	SPR_CCR0,	CONST_SPR_CCR0
    load_spr	r3,	SPR_CCR1,	CONST_SPR_CCR1
    load_spr	r3,	SPR_CCR2,	CONST_SPR_CCR2
    load_spr	r3,	SPR_MMUBE0,	CONST_SPR_MMUBE0
    load_spr	r3,	SPR_MMUBE1,	CONST_SPR_MMUBE1
    load_spr	r3,	SPR_SSPCR,	CONST_SPR_SSPCR
    load_spr	r3,	SPR_ISPCR,	CONST_SPR_ISPCR
    load_spr	r3,	SPR_USPCR,	CONST_SPR_USPCR

/*
	Before editing TLB let's initialize all SPR registers to default values by the spec
	Most of them have X-state after reset and therefore CPU model cannot work properly
*/
    mtcr        r0
    mtspr       SPR_CTR,	r0
    mtspr       SPR_LR,		r0
    mtspr       SPR_DCESR,	r0
    mtspr       SPR_CSRR0,	r0
    mtspr       SPR_CSRR1,	r0
    mtspr       SPR_MCSRR0,	r0
    mtspr       SPR_MCSRR1,	r0
    mtspr       SPR_RMPD,	r0

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
    mtspr       SPR_MCSR_C,	r1
    mtspr       SPR_DEAR,	r0
    mtspr       SPR_TCR,	r0
    mtspr       SPR_DEC,	r0
    mtspr       SPR_DECAR,	r0
    mtspr       SPR_TSR_RC,	r1
    mtspr       SPR_SRR0,	r0
    mtspr       SPR_SRR1,	r0
    mtspr       SPR_TBL_W,	r0
    mtspr       SPR_TBU_W,	r0
    mtspr       SPR_XER,	r0
    mtspr       SPR_PID,	r0

    b           setup_tlb

tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
/* Invalidate initial TLB entry*/
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xFFFFF,    MMU_TLBE_DSIZ_4KB,      0b0,    0b0,    0b0,    0b0,    0b0,    00,     MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
/* TLB entries to remap 64KB page BOOTROM from 0x000003ff_ffff0000-0x000003ff_ffffffff to 0xffff0000-0xffffffff*/
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
.set TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE

/* implement__write_tlb_entries r3, r4, r8, r9, r10 */

setup_tlb:
    load_addr   r3,	tlb_entries
    load_const  r4,	TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries

    b			test_cpu_008

unexpected_error:
	b test_error

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

test_ok:
    rumboot_putstring "\nTEST OK\n"
	test_event EVENT_OK
	xor r0,r0,r0
    exit RESULT_OK

test_error:
	rumboot_putstring "\nTEST ERROR\n"
	test_event  EVENT_ERROR
	xor r0,r0,r0
    exit RESULT_ERROR

stop:
    b stop


.section ".text","ax",@progbits

test_cpu_008:
	rumboot_putstring "start testing values...\n"

    set_cr    cr6,  eq  /* General test status in cr6 */

    /* check all default values */
    chk_value CVR3,	P6BCA(PLB6BC_CR0),			PLB6_BC_CR0_VALUE,			"PLB6BC_CR0         "
    chk_value CVR3,	P6BCA(PLB6BC_PRI),			PLB6_BC_PRI_VALUE,			"PLB6BC_PRI         "
    chk_value CVR3,	P6BCA(PLB6BC_TSNOOP),		PLB6_BC_TSNOOP_VALUE,		"PLB6BC_TSNOOP      "
    chk_value CVR3,	P6BCA(PLB6BC_PAAM_WIN_EXT),	PLB6_BC_PAAM_WIN_EXT_VALUE,	"PLB6BC_PAAM_WIN_EXT"
    /* non-used segments must be written with value 0xFFFFFFF0. */
    chk_value CVR3,	P6BCA(PLB6BC_SGD1),			PLB6_BC_SGD1_VALUE,			"PLB6BC_SGD1        "
    chk_value CVR3,	P6BCA(PLB6BC_SGD2),			PLB6_BC_SGD2_VALUE,			"PLB6BC_SGD2        "
    chk_value CVR3,	P6BCA(PLB6BC_SGD3),			PLB6_BC_SGD3_VALUE			"PLB6BC_SGD3        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD4),										"PLB6BC_SGD4        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD5),										"PLB6BC_SGD5        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD6),										"PLB6BC_SGD6        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD7),										"PLB6BC_SGD7        "
    chk_value CVR3,	P6BCA(PLB6BC_ERR),          PLB6_BC_ERR_VALUE,			"PLB6BC_ERR         "
    chk_value CVR3,	P6BCA(PLB6BC_MSRSP),        PLB6_BC_MSRSP_VALUE,		"PLB6BC_MSRSP       "
    chk_value CVR3,	P6BCA(PLB6BC_HCPP),         PLB6_BC_HCPP_VALUE,			"PLB6BC_HCPP        "
    chk_value CVR3,	P6BCA(PLB6BC_HD),			PLB6_BC_HD_VALUE,			"PLB6BC_HD          "
    chk_value CVR3,	P6BCA(PLB6BC_QD),			PLB6_BC_QD_VALUE,			"PLB6BC_QD          "
    chk_value CVR3,	P6BCA(PLB6BC_SHD),			PLB6_BC_SHD_VALUE,			"PLB6BC_SHD         "
    chk_value CVR3,	P6BCA(PLB6BC_REVID),		PLB6_BC_REVID_VALUE,		"PLB6BC_REVID       "
    /* ADDR bus now is checked */

    /* check DATA I/O buses */
    rumboot_putstring "\nSliding '1' and '0' (low) ... \n <1> "
running_1_low:
    load_const	r20,	P6BCA(PLB6BC_SGD1)  /* DCR addr */
    load_const	r21,	0x00000010          /* begin val */
    load_const	r23, 	0x00000001          /* shift */
    load_const	r24, 	0x80000000          /* max val for compare */
loop_1_low:
	mtdcrx		r20,	r21
	mfdcrx		r22,	r20
	cmpw cr7,	r21,	r22
	TEST_ASSERT(eq, cr7, "PLB6 DCRDATA: Running one error (loop)")
	cmpw cr0,	r21,	r24
	/* beq running_1_hi */
	beq- cr0,	running_0_low
	slw			r21,	r21,	r23
	b loop_1_low

running_0_low:
	rumboot_putstring "\n<0> "
    load_const	r21,	0xFFFFFFF0		/* begin val */

    mtdcrx		r20,	r21
    mfdcrx		r22,	r20
    cmpw cr7,	r21,	r22
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running zero error (init)")
    load_const	r21,	0xFFFFFFE0		/* begin val  */
    load_const	r24,	0x7FFFFFF0		/* finish val */
loop_0_low:
    mtdcrx		r20,	r21
    mfdcrx		r22,	r20
    cmpw cr7,	r21,	r22
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running zero error (loop)")
loop_0_low_ok:
    cmpw cr0,	r21,	r24
    /* beq running_0_hi */
    beq- cr0,	loop_0_low_end
    slw			r21,	r21,	r23
    ori			r21,	r21,	0x0010
    b loop_0_low
loop_0_low_end:

    rumboot_putstring "\nSliding '1' and '0' (high) ... \n <1> "
running_1_high:
    load_const	r20,	P6BCA(PLB6BC_HCPP)  /* DCR addr */
    load_const	r21,	0x00000001          /* begin val */
    load_const	r24, 	0x00000008          /* max val for compare */
	load_const	r25,	0x0000000F			/* bits mask */
loop_1_high:
	ori			r26,	r21,	0x0400
	mtdcrx		r20,	r26
	mfdcrx		r22,	r20
	cmpw cr7,	r26,	r22
	TEST_ASSERT(eq, cr7, "PLB6 DCRDATA: Running one (high) error (loop)")
	cmpw cr0,	r21,	r24
	/* beq running_1_hi */
	beq- cr0,	running_0_high
	slw			r21,	r21,	r23
	b loop_1_high

running_0_high:
	rumboot_putstring "\n<0> "
    load_const	r21,	0x03FFFFFF		/* begin val */

    mtdcrx		r20,	r21
    mfdcrx		r22,	r20
    cmpw cr7,	r21,	r22
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running zero (high) error (init)")
    load_const	r21,	0x0000000E		/* begin val  */
    load_const	r24,	0x00000007		/* finish val */
loop_0_high:
	ori			r26,	r21,	0x0400
	mtdcrx		r20,	r26
    mfdcrx		r22,	r20
    cmpw cr7,	r26,	r22
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running zero (high) error (loop)")
    cmpw cr0,	r21,	r24
    /* beq running_0_hi */
    beq- cr0,	loop_0_high_end
    slw			r21,	r21,	r23
    ori			r21,	r21,	0x0001
    and			r21,	r21,	r25
    b loop_0_high
loop_0_high_end:


	load_addr	r24,	test_ok
	load_addr	r25,	test_error
	mtctr		r24
	beqctr+		cr6
	mtctr		r25
	bctr


