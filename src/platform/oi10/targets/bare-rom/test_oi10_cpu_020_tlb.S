#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/trace.S.h>


DECLARE_CONST( PHYSICAL_SPACE_NOT_ENTERED,  0x407E47E8 )
DECLARE_CONST( PHYSICAL_SPACE_ENTERED,      0xE47E8ED1 )

DECLARE_CONST( HANDLER_NOT_CALLED,  		0x407CA11E )
DECLARE_CONST( HANDLER_CALLED,      		0xCA11ED11 )

DECLARE_CONST( TB_RESULT_UNDEFINED,			0x00000000 )
DECLARE_CONST( TB_RESULT_OK,				0x08111111 )
DECLARE_CONST( TB_RESULT_FINISHED,			0xF14158ED )
DECLARE_CONST( TB_RESULT_ERROR,				0xE8808111 )

.macro sync_all
    isync
    mbar
    tlbsync
    msync
.endm

.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point


.section ".init.text","ax",@progbits

rumboot_entry_point:
	gtube_init
    load_const  r0, 0x00000000
    load_const  r1, 0xFFFFFFFF
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


	rumboot_putstring "Init SPRs...\n"
/* according to User's Manual ch. 9.4 Initialization Software Requirements */
    mtspr       SPR_DBCR0, r0       /* disable all debug events */

    mtspr       SPR_DBSR_RC, r1     /* clear all debug interrupts */

#define CONST_SPR_CCR0                              \
					(0b0	<< CTRL_CCR0_ITE_i    ) \
                  | (0b1	<< CTRL_CCR0_PRE_i    ) \
                  | (0b1	<< CTRL_CCR0_CRPE_i   ) \
                  | (CTRL_CCR0_ICS_32byte           \
							<< CTRL_CCR0_ICS_i    ) \
                  | (0b0	<< CTRL_CCR0_DAPUIB_i ) \
                  | (0b0000	<< CTRL_CCR0_ICWRIDX_i) \
                  | (0b0	<< CTRL_CCR0_DTB_i    ) \
                  | (0b0	<< CTRL_CCR0_FLSTA_i  ) \
                  | (CTRL_CCR0_DQWPM_No_prediction  \
                  			<< CTRL_CCR0_DQWPM_i  ) \
                  | (CTRL_CCR0_IQWPM_Use_EA         \
							<< CTRL_CCR0_IQWPM_i  )
#define CONST_SPR_CCR1                              \
					(0b00	<< CTRL_CCR1_GPRPEI_i ) \
                  | (0b00	<< CTRL_CCR1_FPRPEI_i ) \
                  | (0b00	<< CTRL_CCR1_ICDPEI_i ) \
                  | (0b00	<< CTRL_CCR1_ICLPEI_i ) \
                  | (0b00	<< CTRL_CCR1_ICTPEI_i ) \
                  | (0b00	<< CTRL_CCR1_DCDPEI_i ) \
                  | (0b00	<< CTRL_CCR1_DCLPEI_i ) \
                  | (0b00	<< CTRL_CCR1_DCTPEI_i ) \
                  | (0b0	<< CTRL_CCR1_MMUTPEI_i) \
                  | (0b0	<< CTRL_CCR1_MMUDPEI_i) \
                  | (CTRL_CCR1_TSS_CPU_clock        \
							<< CTRL_CCR1_TSS_i    ) \
                  | (0b0	<< CTRL_CCR1_DPC_i    ) \
                  | (CTRL_CCR1_TCS_div1             \
							<< CTRL_CCR1_TCS_i    )
#define CONST_SPR_CCR2                              \
					(CTRL_CCR2_DSTG_enabled         \
							<< CTRL_CCR2_DSTG_i   ) \
                  | (0b0	<< CTRL_CCR2_DLFPD_i  ) \
                  | (0b0	<< CTRL_CCR2_DSTI_i   ) \
                  | (0b0	<< CTRL_CCR2_PMUD_i   ) \
                  | (0b0	<< CTRL_CCR2_DCSTGW_i ) \
                  | (0		<< CTRL_CCR2_STGCTR_i ) \
                  | (0b0	<< CTRL_CCR2_DISTG_i  ) \
                  | (0b0	<< CTRL_CCR2_SPC5C1_i ) \
                  | (0b0	<< CTRL_CCR2_MCDTO_i  )
#define CONST_SPR_MMUBE0                            \
					(0b0 << MMU_MMUBE0_VBE0_i     ) \
                  | (0b0 << MMU_MMUBE0_VBE1_i     ) \
                  | (0b0 << MMU_MMUBE0_VBE2_i     )
#define CONST_SPR_MMUBE1                            \
					(0b0 << MMU_MMUBE1_VBE3_i     ) \
                  | (0b0 << MMU_MMUBE1_VBE4_i     ) \
                  | (0b0 << MMU_MMUBE1_VBE5_i     )

#define CONST_SPR_SSPCR \
					((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_SSPCR_ORD2_i)\
                  | ((                        MMU_XSPCR_ORD_END)    << MMU_SSPCR_ORD3_i)
#define CONST_SPR_USPCR \
					((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_USPCR_ORD2_i)\
                  | ((                        MMU_XSPCR_ORD_END)    << MMU_USPCR_ORD3_i)
#define CONST_SPR_ISPCR \
					(                         MMU_XSPCR_ORD_4KB     << MMU_ISPCR_ORD2_i)\
                  | (                         MMU_XSPCR_ORD_END     << MMU_ISPCR_ORD3_i)

    load_const  r3, CONST_SPR_CCR0
    mtspr       SPR_CCR0,	r3
    load_const  r3, CONST_SPR_CCR1
    mtspr       SPR_CCR1,	r3
    load_const  r3, CONST_SPR_CCR2
    mtspr       SPR_CCR2,	r3
    load_const  r3, CONST_SPR_MMUBE0
    mtspr       SPR_MMUBE0,	r3
    load_const  r3, CONST_SPR_MMUBE1
    mtspr       SPR_MMUBE1,	r3
    /*
    load_const  r3, CONST_SPR_SSPCR
    mtspr       SPR_SSPCR,	r3
    load_const  r3, CONST_SPR_ISPCR
    mtspr       SPR_ISPCR,	r3
    load_const  r3, CONST_SPR_USPCR
    mtspr       SPR_USPCR,	r3
    */

	/*
	Initialize all SPR registers to default values by the spec
	Most of them have X-state after reset and therefore CPU model cannot work properly
	*/

    mtcr        r0
    mtlr        r0
    mtctr       r0
    mtxer       r0
    mtmsr       r0
    mtspr       SPR_DCESR,	r0
    mtspr       SPR_CSRR0,	r0
    mtspr       SPR_CSRR1,	r0
    mtspr       SPR_MCSRR0,	r0
    mtspr       SPR_MCSRR1,	r0
    mtspr       SPR_RMPD,	r0
    mtspr       SPR_PID,	r0

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

    cmpwi cr6,  r0,         0x0000
    b           setup_initial_tlb


initial_tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
/* Invalidate initial TLB entry*/
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xFFFFF,    MMU_TLBE_DSIZ_4KB,      0b0,    0b0,    0b0,    0b0,    0b0,    00,     MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFFF,    0xFFFFF,    MMU_TLBE_DSIZ_4KB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b1,0b0,0b1,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_4KB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b1,0b0,0b1,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
DECLARE_CONST( INITIAL_TLB_ENTRIES_N, (.-initial_tlb_entries)/MMU_TLB_ENTRY_SIZE )

missed_tlb_replace_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   ) */
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF1,    0xFFFF0,    MMU_TLBE_DSIZ_4KB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b1,0b0,0b1,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
DECLARE_CONST( MISSED_TLB_REPLACE_ENTRIES_N, (.-missed_tlb_replace_entries)/MMU_TLB_ENTRY_SIZE )

/* implement__write_tlb_entries r3, r4, r8, r9, r10 */

setup_initial_tlb:
	rumboot_putstring "Setting up initial TLBs...\n"
    load_addr   r3, initial_tlb_entries
    load_const  r4, INITIAL_TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries  /* (r3, r4) - are parameters */
	sync_all
    load_addr   r0, test_cpu_020_tlb
    mtctr       r0
    bctr

test_cpu_020_tlb:
	rumboot_putstring "Init interrupt vectors...\n"
    lis         r0, interrupt_handlers@h
    mtspr       SPR_IVPR, r0

    li          r0, unexpected_interrupt_handler@l
    mtspr       SPR_IVOR0,	r0
    mtspr       SPR_IVOR1,	r0
    mtspr       SPR_IVOR2,	r0
    mtspr       SPR_IVOR3,	r0
    mtspr       SPR_IVOR4,	r0
    mtspr       SPR_IVOR5,	r0
    mtspr       SPR_IVOR6,	r0
    mtspr       SPR_IVOR7,	r0
/*	mtspr       SPR_IVOR8,	r0 */
    mtspr       SPR_IVOR9,	r0
    mtspr       SPR_IVOR10,	r0
    mtspr       SPR_IVOR11,	r0
    mtspr       SPR_IVOR12,	r0
    mtspr       SPR_IVOR13,	r0
/*	mtspr       SPR_IVOR14,	r0 */
    mtspr       SPR_IVOR15,	r0

    li			r0, dtlb_miss_handler@l
    mtspr		SPR_IVOR13,	r0

    li			r0, pgm_interrupt_handler@l
    mtspr		SPR_IVOR6,	r0

    li          r0, system_call_handler@l
    mtspr       SPR_IVOR8, r0

    li          r0, itlb_miss_handler@l
    mtspr       SPR_IVOR14, r0

    load_const  r21, PHYSICAL_SPACE_NOT_ENTERED
    load_const  r22, HANDLER_NOT_CALLED

enter_usermode:
    load_addr	r0, usermode_entry
    load_const  r1, (0b1 << ITRPT_XSR_PR_i)
    mtsrr0		r0
    mtsrr1		r1
	rumboot_putstring "Entering usermode...\n"
	rfi

generate_syscall:
	sc

usermode_entry:
	b			physical_entry


test_error:
	rumboot_putstring "TEST ERROR\n"
    test_event  EVENT_ERROR
    exit		RESULT_ERROR
    b           finish

test_ok:
	rumboot_putstring "TEST OK\n"
    test_event  EVENT_OK
    exit		RESULT_OK
    b           finish

finish:
    test_event  EVENT_FINISHED
finish_loop:
    b           finish_loop


.align 4
interrupt_handlers:
unexpected_interrupt_handler:
    rumboot_putstring   "Unexpected unknown interrupt!\n"
    b           test_error

.align 4
dtlb_miss_handler:
    rumboot_putstring   "Unexpected dtlb miss interrupt!\n"
    b           test_error

.align 4
pgm_interrupt_handler:
    rumboot_putstring   "Unexpected program interrupt (6)!\n"
    b           test_error

.align 4
system_call_handler:
	rumboot_putstring "Processing syscall... "
	/*              MMU_MMUCR( REALE, DULXE, IULXE, TS,  TID               ) */
    load_const  r0, MMU_MMUCR( 0b0,   0b0,   0b0,   0b0, MEM_WINDOW_SHARED )
    load_const  r1, 0xFFFF0000
    mtspr       SPR_MMUCR, r0
    isync
    tlbivax     0, r1
	sync_all
    rumboot_putstring "Done!\n"
    b			physical_entry
    /* rfi */

.align 4
itlb_miss_handler:
	rumboot_putstring "Processing ITLB miss... \n"
	load_const	r3,  HANDLER_CALLED
	cmpw		cr4, r3, r22
	beq-		cr4, itlb_miss_2
	mr			r22, r3
	rumboot_putstring "Add TLB entries\n"
    load_addr   r3, missed_tlb_replace_entries
    load_const  r4, MISSED_TLB_REPLACE_ENTRIES_N
    bl          ppc470s_write_tlb_entries
    sync_all
    load_addr	r3, physical_entry
    mtctr		r3
    bctr
itlb_miss_2:
	mfsrr0		r30
	mfsrr0		r29
	rumboot_putstring "Called twice!\n"
	load_addr	r3, check_tlb_sync
	mtctr		r3
	bctr
    /* rfi */

notify_page_2:
    rumboot_putstring "We are at the second memory page!\n"
    blr

check_tlb_sync:
    nop

    load_const  r1, PHYSICAL_SPACE_ENTERED
    cmpw        cr7, r1, r21
    TEST_ASSERT( eq, cr7, "Physical space not entered" )

    load_const  r2, HANDLER_CALLED
    cmpw        cr7, r2, r22
    TEST_ASSERT( eq, cr7, "Intruction TLB miss handler not called" )

    bne-		cr6, test_error
    b			test_ok


.section ".text","ax",@progbits

.org 0x00000000
physical_entry:
	load_addr	r3, generate_syscall
	mtctr		r3
    bctr

.org 0x00001000
    load_const  r21, PHYSICAL_SPACE_ENTERED
    load_addr	r3, notify_page_2
    mtctr		r3
    bctrl
	/* bl notify_page_2 */
    load_addr	r3,  check_tlb_sync
    mtctr		r3
    bctr

/* This stuff do nothing */
.org 0x00002000
	.long	0x55AA55AA
	.long	0xAA55AA55
	.long	0xFFFFFFFF
	.long	0xFFFFFFFF


