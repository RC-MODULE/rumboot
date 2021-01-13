/*
 * startup.S
 *
 *  Created on: Mar 8, 2018
 *      Author: v.strukov
 */
#include <arch/ppc_476fp_asm.h>
#include <platform/devices.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/ppc_476fp_debug_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_timer_fields.h>
#include <arch/ppc_476fp_mmu_fields.h>

//#include <platform/test_event_asm.h>
//#include <platform/trace.S.h>


#include <arch/regs/plb6bc.h>
#include <platform/regs/sctl.h>
#include <platform/bootm.h>

#include <rumboot/irq.h>


.section ".init.rodata","a",@progbits

.align 3
const_0:
    .long 0x00000000
    .long 0x00000000


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point


.section "itrpt.init.text","ax",@progbits

trace_msg_unexpected_itrpt:
    blr

.align 4
unexpected_itrpt_hdr_base:
unexpected_CI_hdr:
    b           unexpected_error

.align 4
unexpected_MC_hdr:
    b           unexpected_error

.align 4
unexpected_DS_hdr:
    b           unexpected_error

.align 4
unexpected_IS_hdr:
    b           unexpected_error

.align 4
unexpected_EI_hdr:
    b           unexpected_error

.align 4
unexpected_A_hdr:
    b           unexpected_error

.align 4
unexpected_P_hdr:
    b           unexpected_error

.align 4
unexpected_FPU_hdr:
    b           unexpected_error

.align 4
unexpected_SC_hdr:
    b           unexpected_error

.align 4
unexpected_APU_hdr:
    b           unexpected_error

.align 4
unexpected_DEC_hdr:
    b           unexpected_error

.align 4
unexpected_FIT_hdr:
    b           unexpected_error

.align 4
unexpected_WDT_hdr:
    b           unexpected_error

.align 4
unexpected_DTLBE_hdr:
    b           unexpected_error

.align 4
unexpected_ITLBE_hdr:
    b           unexpected_error

 .align 4
unexpected_D_hdr:
    b           unexpected_error

unexpected_error:
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
    /* Initialize gtube */
    load_const  r2, 0x0003c0de 
    mtspr       SPR_SPRG0, r2

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

    // Call platform glue to init tlb entries
    bl rumboot_ppc_platform_setup_low

//---------------------
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

    bl rumboot_ppc_platform_setup_high
    b goto_boot_main

.section ".text.rom","ax",@progbits

goto_boot_main:
    /* Initialize the runtime  */
    //gtube_init_runtime
    bl          rumboot_main /* never returns */

endless_loop:
    b           endless_loop
