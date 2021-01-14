/*
 * startup.S
 *
 *  Created on: Mar 8, 2018
 *      Author: v.strukov
 */
#include <arch/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>
#include <arch/mmu.S.h>
#include <arch/regs/plb6bc.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/ppc_476fp_debug_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_timer_fields.h>
#include <platform/devices.h>
#include <platform/regs/sctl.h>
#include <platform/trace.S.h>
#include <platform/bootm.h>
#include <rumboot/irq.h>


.section ".init.rodata","a",@progbits

.align 3
const_0:
    .long 0x00000000
    .long 0x00000000

.align 4
rom_tlb_entries:
nor_tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
/* TLB entries to remap 64KB page BOOTROM from 0x000003FF_FFFF0000-0x000003FF_FFFFFFFF to 0xFFFF0000-0xFFFFFFFF (bolted entries must be defined first)*/
    .long   MMU_TLB_ENTRY(  0x3FF,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x01F,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_0,     0b1 )
/* Invalidate initial TLB entry*/
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0xFFFFF,    MMU_TLBE_DSIZ_4KB,      0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b0,0b0,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
DECLARE_CONST( ROM_TLB_ENTRIES_N, (.-rom_tlb_entries)/MMU_TLB_ENTRY_SIZE )
    .long   MMU_TLB_ENTRY(  0x000,  0x7FFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_0,     0b1 )
DECLARE_CONST( NOR_TLB_ENTRIES_N, (.-nor_tlb_entries)/MMU_TLB_ENTRY_SIZE )


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point


.section "itrpt.init.text","ax",@progbits

trace_msg_unexpected_itrpt:
    rumboot_putstring   "Unexpected interrupt: "
    blr

.align 4
unexpected_itrpt_hdr_base:
unexpected_CI_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Critical Input\n"
    b           unexpected_error

.align 4
unexpected_MC_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Machine Check\n"
    b           unexpected_error

.align 4
unexpected_DS_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Data Storage\n"
    b           unexpected_error

.align 4
unexpected_IS_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Instruction Storage\n"
    b           unexpected_error

.align 4
unexpected_EI_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "External Input\n"
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
    rumboot_putstring   "Floating-Point Unavailable\n"
    b           unexpected_error

.align 4
unexpected_SC_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "System Call\n"
    b           unexpected_error

.align 4
unexpected_APU_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Auxiliary Processor Unavailable\n"
    b           unexpected_error

.align 4
unexpected_DEC_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Decrementer\n"
    b           unexpected_error

.align 4
unexpected_FIT_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Fixed-Interval Timer\n"
    b           unexpected_error

.align 4
unexpected_WDT_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Watchdog Timer\n"
    b           unexpected_error

.align 4
unexpected_DTLBE_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Data TLB Error\n"
    b           unexpected_error

.align 4
unexpected_ITLBE_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Instruction TLB Error\n"
    b           unexpected_error

 .align 4
unexpected_D_hdr:
    bl          trace_msg_unexpected_itrpt
    rumboot_putstring   "Debug\n"
    b           unexpected_error


unexpected_error:
    test_event  EVENT_ERROR
    b endless_loop


.section ".init.text","ax",@progbits

.global rumboot_entry_point
rumboot_entry_point:
rumboot_reset_handler:

init_gpr:
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

init_branch_spr:
    mtspr       SPR_LR, r0
    mtspr       SPR_CTR, r0
    mtcr        r0
    mtspr       SPR_XER, r0

init_sprg:
    mtspr       SPR_SPRG0, r0
    mtspr       SPR_SPRG1, r0
    mtspr       SPR_SPRG2, r0
    mtspr       SPR_SPRG3, r0
    mtspr       SPR_SPRG4, r0
    mtspr       SPR_SPRG5, r0
    mtspr       SPR_SPRG6, r0
    mtspr       SPR_SPRG7, r0
    mtspr       SPR_SPRG8, r0
    mtspr       SPR_USPRG0, r0


    gtube_init

init_itrtp_spr:
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
    li          r3, unexpected_EI_hdr@l
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
    li          r3, unexpected_FIT_hdr@l
    mtspr       SPR_IVOR11, r3
    li          r3, unexpected_WDT_hdr@l
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

init_other_spr:
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
                  | (0b0                            << CTRL_CCR0_DBTAC_i)\
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

    mtspr       SPR_TCR, r0
    mtspr       SPR_TBL_W, r0
    mtspr       SPR_TBU_W, r0
    mtspr       SPR_DEC, r0
    mtspr       SPR_DECAR, r0
    load_const  r3, (0b1    << TIMER_TSR_ENW_i)\
                  | (0b1    << TIMER_TSR_WIS_i)\
                  | (0b1    << TIMER_TSR_DIS_i)\
                  | (0b1    << TIMER_TSR_FIS_i)
    mtspr       SPR_TSR_RC, r3

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

    load_const  r3, (MMU_XSPCR_ORD_64KB     << MMU_ISPCR_ORD1_i)\
                  | (MMU_XSPCR_ORD_1MB      << MMU_ISPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_4KB      << MMU_ISPCR_ORD3_i)\
                  | (MMU_XSPCR_ORD_1GB      << MMU_ISPCR_ORD4_i)\
                  | (MMU_XSPCR_ORD_16KB     << MMU_ISPCR_ORD5_i)\
                  | (MMU_XSPCR_ORD_16MB     << MMU_ISPCR_ORD6_i)\
                  | (MMU_XSPCR_ORD_256MB    << MMU_ISPCR_ORD7_i)
    mtspr       SPR_ISPCR, r3

    load_const  r3, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB)   << MMU_USPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_1MB)    << MMU_USPCR_ORD2_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_USPCR_ORD3_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_1GB)    << MMU_USPCR_ORD4_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_16KB)   << MMU_USPCR_ORD5_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_16MB)   << MMU_USPCR_ORD6_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB)  << MMU_USPCR_ORD7_i)
    mtspr       SPR_USPCR, r3

    load_const  r3, ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_64KB) << MMU_SSPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_SSPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_END                              << MMU_SSPCR_ORD3_i)
    mtspr       SPR_SSPCR, r3

setup_boot_tlb_entries:
// Program invalidation of all TLB entries is very slow. Let's skip it in tests.
// All entries are invalidated from testbench during core reset.
#ifndef CMAKE_BUILD_TYPE_DEBUG
    bl          ppc470s_invalidate_all_tlb_entries_nosnoop
#endif

    load_const  r3, MEM_WINDOW_1
    mtspr SPR_PID, r3

    load_const  r3, DCR_SCTL_BASE+SCTL_SYS_BOOT_CFG
    mfdcrx      r3, r3
    load_const  r4, BOOTM_NOR_BOOT
    and.        r3, r4, r3
    bne         write_rom_tlb_entries

write_nor_tlb_entries:
    load_addr   r3, nor_tlb_entries
    load_size   r4, NOR_TLB_ENTRIES_N
    b           write_xxx_tlb_entries

write_rom_tlb_entries:
    load_addr   r3, rom_tlb_entries
    load_size   r4, ROM_TLB_ENTRIES_N

write_xxx_tlb_entries:
    bl          ppc470s_write_tlb_entries

init_fpu_regs:
    load_const  r3, (0b1 << ITRPT_XSR_FP_i) /* enable fpu */
    mtmsr       r3

    load_addr   r3, const_0

    lfd         f0, 0(r3)
    lfd         f1, 0(r3)
    lfd         f2, 0(r3)
    lfd         f3, 0(r3)
    lfd         f4, 0(r3)
    lfd         f5, 0(r3)
    lfd         f6, 0(r3)
    lfd         f7, 0(r3)
    lfd         f8, 0(r3)
    lfd         f9, 0(r3)
    lfd         f10, 0(r3)
    lfd         f11, 0(r3)
    lfd         f12, 0(r3)
    lfd         f13, 0(r3)
    lfd         f14, 0(r3)
    lfd         f15, 0(r3)

    lfd         f16, 0(r3)
    lfd         f17, 0(r3)
    lfd         f18, 0(r3)
    lfd         f19, 0(r3)
    lfd         f20, 0(r3)
    lfd         f21, 0(r3)
    lfd         f22, 0(r3)
    lfd         f23, 0(r3)
    lfd         f24, 0(r3)
    lfd         f25, 0(r3)
    lfd         f26, 0(r3)
    lfd         f27, 0(r3)
    lfd         f28, 0(r3)
    lfd         f29, 0(r3)
    lfd         f30, 0(r3)
    lfd         f31, 0(r3)

enable_itrpts:
    load_const  r3, (0b1 << ITRPT_XSR_CE_i)\
                  | (0b1 << ITRPT_XSR_EE_i)\
                  | (0b1 << ITRPT_XSR_ME_i) /* enable interrupts, disable fpu */
    mtmsr       r3

setup_boot_rom_im0_p64:
    load_const  r3, DCR_PLB6PLB4_0_BASE
    /* enable requests to BOOT_ROM mirror (0x0000001F_FFFF0000:0x0000001F_FFFFFFFF) and IM0 (0x00000010_80000000:0x00000010_8001FFFF) */
    load_const  r4, 0x00000010
    load_const  r5, 0x0000001F
    bl          p64_set_split_range_address

setup_plb6bc:
    load_const  r6, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB)   << MMU_SSPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_1MB)    << MMU_SSPCR_ORD2_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_SSPCR_ORD3_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_1GB)    << MMU_SSPCR_ORD4_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_16KB)   << MMU_SSPCR_ORD5_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_16MB)   << MMU_SSPCR_ORD6_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB)  << MMU_SSPCR_ORD7_i)

    load_const  r7, MEM_WINDOW_SHARED

    load_const  r3, DCR_PLB6_BC_BASE

    addi        r4, r3, PLB6BC_SGD1
    load_const  r5, 0x00000010
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_SGD2
    load_const  r5, 0x00000010
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_SGD3
    load_const  r5, 0x00000020
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_HCPP
    load_const  r5, PLB6BC_HCPP_4K
    mtdcrx      r4, r5

    addi        r4, r3, PLB6BC_CR0
    load_const  r5, (0b1 << PLB6BC_CR0_BC_DSBS_i)\
                  | (0b1 << PLB6BC_CR0_BC_MASK_MSRSP_i)\
                  | (0b1 << PLB6BC_CR0_BC_MASK_SRSNZ_i)
.align 4
    mtdcrx      r4, r5          /* enable segmentation on P6BC */
    mtspr       SPR_SSPCR, r6   /* must be set explicitly after segmentation on P6BC has been enabled */
    mtspr       SPR_PID, r7     /* ppc470s_set_mem_window  r7, MEM_WINDOW_0 */
    isync

setup_tlb:
    load_addr   r3, tlb_entries
    load_size   r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries /*(r3, r4) - are parameters*/

setup_stack: /*stack is growing down*/
    load_addr   r1, rumboot_platform_stack_area_end
    stwu        r0, -16(r1) /*allocate LR and save SP of first stack frame according to ABI rules*/
    b goto_boot_main

.section ".text.irq","ax",@progbits

.global rumboot_itrpt_hdr_base
.global rumboot_CI_hdr
.global rumboot_MC_hdr
.global rumboot_DS_hdr
.global rumboot_IS_hdr
.global rumboot_EI_hdr
.global rumboot_A_hdr
.global rumboot_P_hdr
.global rumboot_FPU_hdr
.global rumboot_SC_hdr
.global rumboot_APU_hdr
.global rumboot_DEC_hdr
.global rumboot_FIT_hdr
.global rumboot_WDT_hdr
.global rumboot_DTLBE_hdr
.global rumboot_ITLBE_hdr
.global rumboot_D_hdr

.align 4
rumboot_itrpt_hdr_base:
rumboot_CI_hdr:
    full_prologue   1

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_NORMAL
    load_const  r5, 0
    bl          rumboot_irq_core_dispatch

    full_epilogue   1

    rfci

.align 4
rumboot_MC_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_MACHINE_CHECK
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfmci

.align 4
rumboot_DS_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_DATA_STORAGE
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_IS_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_INST_STORAGE
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_EI_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_NORMAL
    load_const  r5, 0
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_A_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_ALIGNMENT
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_P_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_PROGRAM
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_FPU_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_FP_UNAVAILABLE
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_SC_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_SYSTEM_CALL
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_APU_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_AP_UNAVAILABLE
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_DEC_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_DECREMENTER
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_FIT_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_FIXED_INTERVAL_TIMER
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_WDT_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_WATCHDOG_TIMER
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfci

.align 4
rumboot_DTLBE_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_DATA_TLB_ERROR
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_ITLBE_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_INST_TLB_ERROR
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

 .align 4
rumboot_D_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_DEBUG
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfci

.section ".text.rom","ax",@progbits

goto_boot_main:
    /* Initialize the runtime  */
    gtube_init_runtime
    bl          rumboot_main /* never returns */

endless_loop:
    b           endless_loop
