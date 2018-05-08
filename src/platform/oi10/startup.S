/*
 * startup.S
 *
 *  Created on: Mar 8, 2018
 *      Author: v.strukov
 */
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/regs/regs_plb6bc.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/devices.h>
#include <platform/trace.S.h>

.global unexpected_itrpt_hdr_base
.global unexpected_CI_hdr
.global unexpected_MC_hdr
.global unexpected_DS_hdr
.global unexpected_IS_hdr
.global unexpected_E_hdr
.global unexpected_A_hdr
.global unexpected_P_hdr
.global unexpected_FPU_hdr
.global unexpected_SC_hdr
.global unexpected_APU_hdr
.global unexpected_DEC_hdr
.global unexpected_FITI_hdr
.global unexpected_WTI_hdr
.global unexpected_DTLBE_hdr
.global unexpected_ITLBE_hdr
.global unexpected_D_hdr
.global unexpected_error

.section ".init.rodata","a",@progbits

.align 2
boot_tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
/* Invalidate initial TLB entry*/
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xFFFFF,    MMU_TLBE_DSIZ_4KB,      0b0,    0b0,    0b0,    0b0,    0b0,    00,     MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
/* TLB entries to remap 64KB page BOOTROM from 0x000003ff_ffff0000-0x000003ff_ffffffff to 0xffff0000-0xffffffff*/
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
/* TLB entries to remap 128KB pages IM0 from 0x00000010_00010000-0x00000010_0002ffff to 0xC0010000-0xC002ffff*/
    .long   MMU_TLB_ENTRY(  0x010,  0x00010,    0xC0010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00020,    0xC0020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
/* TLB entries to remap 128KB pages IM1 from 0x00000020_00000000-0x00000020_0001ffff to 0xD0000000-0xD001ffff*/
    .long   MMU_TLB_ENTRY(  0x020,  0x00000,    0xD0000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x020,  0x00000,    0xD0010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
/* TLB entry to remap LHSIF (SW Router) - error in memory map with this entry, please check ranges*/
/*    .long   MMU_TLB_ENTRY(  0x020,  0xFFFFF,    0xD8000,    MMU_TLBE_DSIZ_256MB,    0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )*/
DECLARE_CONST( BOOT_TLB_ENTRIES_N, (.-boot_tlb_entries)/MMU_TLB_ENTRY_SIZE )


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point


.section ".init.text","ax",@progbits

trace_msg_unexpected_itrpt:
    rumboot_putstring   "Unexpected interrupt: "
    blr

.align 4
unexpected_itrpt_hdr_base:
unexpected_CI_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Critical input\n"
    b           unexpected_error

.align 4
unexpected_MC_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Machine check\n"
    b           unexpected_error

.align 4
unexpected_DS_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Data storage\n"
    b           unexpected_error

.align 4
unexpected_IS_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Instruction storage\n"
    b           unexpected_error

.align 4
unexpected_E_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "External input\n"
    b           unexpected_error

.align 4
unexpected_A_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Alignment\n"
    b           unexpected_error

.align 4
unexpected_P_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Program\n"
    b           unexpected_error

.align 4
unexpected_FPU_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Floating point unavailable\n"
    b           unexpected_error

.align 4
unexpected_SC_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "System call\n"
    b           unexpected_error

.align 4
unexpected_APU_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Auxiliary processor unavailable\n"
    b           unexpected_error

.align 4
unexpected_DEC_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Decrementer\n"
    b           unexpected_error

.align 4
unexpected_FITI_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Fixed interval timer\n"
    b           unexpected_error

.align 4
unexpected_WTI_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Watchdog timer\n"
    b           unexpected_error

.align 4
unexpected_DTLBE_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Data TLB error\n"
    b           unexpected_error

.align 4
unexpected_ITLBE_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Instruction TLB error\n"
    b           unexpected_error

 .align 4
unexpected_D_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Debug\n"
    b           unexpected_error


unexpected_error:
    test_event  EVENT_ERROR
    b endless_loop

ppc470s_invalidate_all_tlb_entries_nosnoop:
    load_const r6, 1024       //set counter to 1024 iterations (there are 1024 TLB entries)
    mtspr SPR_CTR, r6
    load_const r6, 0          //iteration index = 0
    load_const r9, 0          //r9 = 0

invalidate_tlb_entry:
//Calculate current entry way and index
    load_const r7, 0                   //clear current entry index
    copy_field(r7, 33, 34, r6, 54)  //r7[33:34] = r6[54:55] current way
    copy_field(r7, 40, 47, r6, 56)  //r7[40:47] = r6[56:63] current index

//Read entry value
    tlbre   r8, r7, 0
    andi.   r10, r8, reg_field(20, 1) //if (tlbentry[V] == 0) goto_next_tlb_entry;
    beq     goto_next_tlb_entry

    copy_field(r8, 20, 20, r9, 0) //clear V bit r8[20] = r9[0]. r9 must be zero here

//Invalidate entry, even if bolted entry: (r8[60] == 1 && r7[33:34] == 0b00).
    oris    r7, r7, 0x8000       //set way assignement type for tlbwe. RA[36] is already 0, RA[32] = 1
    //MMUCR[STID] = tlbentry[TID]. This is done automatically in tlbre
    tlbwe   r8, r7, 0
    tlbwe   r9, r7, 1
    tlbwe   r9, r7, 2

goto_next_tlb_entry:
    addi    r6, r6, 1     //increment iteration index
    bdnz    invalidate_tlb_entry //decrement cnt and check for exit from loop

    blr //return from ppc470s_invalidate_all_tlb_entries_nosnoop

rumboot_entry_point:
rumboot_reset_handler:

    gtube_init

    load_const  r0, 0x00000000  /* make r0 "all zeros" source*/
    load_const  r1, 0xFFFFFFFF  /* make r1 "all ones" source*/

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

    mtspr       SPR_SRR0, r0
    mtspr       SPR_SRR1, r0
    mtspr       SPR_CSRR0, r0
    mtspr       SPR_CSRR1, r0
    mtspr       SPR_MCSRR0, r0
    mtspr       SPR_MCSRR1, r0
    mtspr       SPR_MCSR_C, r1
    mtspr       SPR_ESR, r0
    mtspr       SPR_DCESR, r0
    mtspr       SPR_DEAR, r0

    load_const  r3, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB)   << MMU_SSPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB)  << MMU_SSPCR_ORD2_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_SSPCR_ORD3_i)\
                  | (MMU_XSPCR_ORD_END                              << MMU_SSPCR_ORD4_i)
    mtspr       SPR_SSPCR, r3

setup_bootrom_apb_tlb_entries:
/* We must put currently used TLB entries into shadow TLB.
 Simple read would be enough in our case
  lwz         r3, 0(r3)

 TODO Program invalidation of all TLB entries is very slow. Let's skip it in tests.
 All entries are invalidated from testbench during core reset.*/
/*
#ifdef ROMBOOT_BUILD_TYPE
#if ROMBOOT_BUILD_TYPE == "Production"

    bl          ppc470s_invalidate_all_tlb_entries_nosnoop

    ppc470s_set_mem_window  r3, MEM_WINDOW_SHARED

#else
#warning
#endif
#else
#warning
#endif*/
    load_addr   r3, boot_tlb_entries
    load_const  r4, BOOT_TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries

#ifdef TEST_ENABLE_INTERRUPTS_CHECKING
enable_itrpts:
    load_const  r3, (0b1 << ITRPT_XSR_CE_i)\
                  | (0b1 << ITRPT_XSR_EE_i)\
                  | (0b1 << ITRPT_XSR_ME_i)
    mtmsr       r3
#endif

setup_plb6bc:
    load_const  r3, DCR_PLB6_BC_BASE

    addi        r4, r3, PLB6BC_SGD1
    load_const  r5, 0x00000010
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_SGD2
    load_const  r5, 0x00000010
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_CR0
    load_const  r5, (0b1 << PLB6BC_CR0_BC_DSBS_i)\
                  | (0b1 << PLB6BC_CR0_BC_MASK_MSRSP_i)\
                  | (0b1 << PLB6BC_CR0_BC_MASK_SRSNZ_i)
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_HCPP
    load_const  r5, PLB6BC_HCPP_4K
    mtdcrx      r4, r5
    b boot_init

boot_init:
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

init_spr:
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

    load_const  r3, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB)   << MMU_SSPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB)  << MMU_SSPCR_ORD2_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB)  << MMU_SSPCR_ORD3_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_SSPCR_ORD4_i)\
                  | (MMU_XSPCR_ORD_END                              << MMU_SSPCR_ORD5_i)
    mtspr       SPR_SSPCR, r3

    load_const  r3, (MMU_XSPCR_ORD_64KB     << MMU_ISPCR_ORD1_i)\
                  | (MMU_XSPCR_ORD_256MB    << MMU_ISPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_1GB      << MMU_ISPCR_ORD3_i)\
                  | (MMU_XSPCR_ORD_4KB      << MMU_ISPCR_ORD4_i)\
                  | (MMU_XSPCR_ORD_END      << MMU_ISPCR_ORD5_i)
    mtspr       SPR_ISPCR, r3

    load_const  r3, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB)   << MMU_USPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB)  << MMU_USPCR_ORD2_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB)  << MMU_USPCR_ORD3_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_USPCR_ORD4_i)\
                  | (MMU_XSPCR_ORD_END                              << MMU_USPCR_ORD5_i)
    mtspr       SPR_USPCR, r3

/*Before editing TLB let's initialize all SPR registers to default values by the spec*/
/*Most of them have X-state after reset and therefore CPU model cannot work properly*/

    mtspr       SPR_LR, r0
    mtspr       SPR_CTR, r0
    mtcr        r0
    mtspr       SPR_XER, r0

    mtspr       SPR_TCR, r0
    mtspr       SPR_TBL_W, r0
    mtspr       SPR_TBU_W, r0
    mtspr       SPR_DEC, r0
    mtspr       SPR_DECAR, r0
    load_const  r3, (0b1    << TIMER_TSR_EVW_i)\
                  | (0b1    << TIMER_TSR_WIS_i)\
                  | (0b1    << TIMER_TSR_DIS_i)\
                  | (0b1    << TIMER_TSR_FIS_i)
    mtspr       SPR_TSR_RC, r3
/* This is implemented in gtube_init*/
/*
    mtspr       SPR_SPRG0, r0
    mtspr       SPR_SPRG1, r0
    mtspr       SPR_SPRG2, r0
    mtspr       SPR_SPRG3, r0
    mtspr       SPR_SPRG4, r0
    mtspr       SPR_SPRG5, r0
    mtspr       SPR_SPRG6, r0
    mtspr       SPR_SPRG7, r0
    mtspr       SPR_SPRG8, r0
*/
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
    mtspr       SPR_RMPD, r0


setup_tlb:
    load_addr   r3, tlb_entries
    load_size   r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries /*(r3, r4) - are parameters*/

    ppc470s_set_mem_window  r3, MEM_WINDOW_0


/*TODO*/
setup_stack: /*stack is growing down*/
setup_cpu0_stack:
    load_addr   r1, rumboot_platform_stack_area_end
    stwu        r0, -8(r1) /*allocate LR and save SP of first stack frame according to ABI rules*/
    b setup_plb6plb4


setup_plb6plb4:
    bl          p64_init_simple
    b goto_boot_main

.section "ROM.text","ax",@progbits
goto_boot_main:
    bl          rumboot_main /* never returns*/

endless_loop:
    b           endless_loop


