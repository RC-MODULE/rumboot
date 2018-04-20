
#undef __linux__
#include <arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/trace.S.h>
#include <platform/oi10/platform/ppc470s/mmu.S.h>
#include <arch/ppc/ppc_476fp_debug_fields.h>
#include <arch/ppc/ppc_476fp_ctrl_fields.h>
#include <arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/oi10/platform/devices.h>

.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:
    load_const  r0, 0x00000000  // make r0 "all zeros" source
    mtspr       SPR_SPRG0, r0
    mtspr       SPR_SPRG1, r0
    mtspr       SPR_SPRG2, r0
    mtspr       SPR_SPRG3, r0
    mtspr       SPR_SPRG4, r0
    mtspr       SPR_SPRG5, r0
    mtspr       SPR_SPRG6, r0
    mtspr       SPR_SPRG7, r0
    mtspr       SPR_SPRG8, r0

    lis  r0, 0x00000000  // make r0 "all zeros" source
    load_const  r1, 0xFFFFFFFF  // make r1 "all ones" source
    lis  r2, 0x00000000
    lis  r3, 0x00000000
    lis  r4, 0x00000000
    lis  r5, 0x00000000
    lis  r6, 0x00000000
    lis  r7, 0x00000000
    lis  r8, 0x00000000
    lis  r9, 0x00000000
    lis  r10,0x00000000
    lis  r11,0x00000000
    lis  r12,0x00000000
    lis  r13,0x00000000
    lis  r14,0x00000000
    lis  r15,0x00000000
    lis  r16,0x00000000
    lis  r17,0x00000000
    lis  r18,0x00000000
    lis  r19,0x00000000
    lis  r20,0x00000000
    lis  r21,0x00000000
    lis  r22,0x00000000
    lis  r23,0x00000000
    lis  r24,0x00000000
    lis  r25,0x00000000
    lis  r26,0x00000000
    lis  r27,0x00000000
    lis  r28,0x00000000
    lis  r29,0x00000000
    lis  r30,0x00000000
    lis  r31,0x00000000

init_spr:
//according to User's Manual ch. 9.4 Initialization Software Requirements
    mtspr       SPR_DBCR0, r0       // disable all debug events

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
    mtspr       SPR_DBSR_RC, r3     // clear all debug interrupts

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

//Before editing TLB let's initialize all SPR registers to default values by the spec
//Most of them have X-state after reset and therefore CPU model cannot work properly

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

//    mtspr       SPR_SPRG0, r0
//    mtspr       SPR_SPRG1, r0
//    mtspr       SPR_SPRG2, r0
//    mtspr       SPR_SPRG3, r0
//    mtspr       SPR_SPRG4, r0
//    mtspr       SPR_SPRG5, r0
//    mtspr       SPR_SPRG6, r0
//    mtspr       SPR_SPRG7, r0
//    mtspr       SPR_SPRG8, r0

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

    load_const r0 0x00000000
    mtxer r0
check_cpu_ppc_prog_model_24:    //check sequential flow
    //test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_24_SEQ

check_cpu_ppc_prog_model_25:    //check unconditional branch
    //trace_msg "check uncond branch (align 3)\n"
    b bu_b3
    //test_event EVENT_ERROR
b_err:
    b b_err

.align 3
bu_b3:
    //trace_msg "check uncond branch (align 4)\n"
    b bu_b4
    b error

.align 4
bu_b4:
    //trace_msg "check uncond branch (align 5)\n"
    b bu_b5
    b error

.align 5
bu_b5:
    //trace_msg "check uncond branch (align 6)\n"
    b bu_b6
    b error

.align 6
bu_b6:
    b bu_b
    b error

bu_ba:
    //trace_msg "check bl\n"
    bl bu_bl
    b bu_bla

bu_bla:
    bl bu_bl
    b check_cpu_ppc_prog_model_26

bu_b:
    //trace_msg "check ba\n"
    ba bu_ba
    b error

bu_bl:
    blr
    b error

check_cpu_ppc_prog_model_26:    //check conditional branch
    //trace_msg "check blt\n"
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    blt bc_ble
    b error

bc_ble:
    //trace_msg "check ble\n"
    ble bc_beq
    b error

bc_beq:
    //trace_msg "check beq\n"
    load_const r0, 1
    load_const r1, 1
    cmp 0, 0, r0, r1
    beq bc_bgt
    b error

bc_bgt:
    //trace_msg "check bgt\n"
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bgt bc_bge
    b error

bc_bge:
    //trace_msg "check bge\n"
    bge bc_bnl
    b error

bc_bnl:
    //trace_msg "check bnl\n"
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bnl bc_bne
    b error

bc_bne:
    //trace_msg "check bne\n"
    bne bc_bng
    b error

bc_bng:
    //trace_msg "check bng\n"
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    bng bc_bso
    b error

bc_bso:
    //trace_msg "check bso\n"
    load_const r0, 0x7FFFFFFF
    load_const r1, 0x00000002
    addco. r2, r0, r1 //operands considered as signed, see PowerISA, 3.3.7
    bso bc_bns
    b error

bc_bns:
    //trace_msg "check bns\n"
    load_const r0, 0xFFFFFFFF
    load_const r1, 0x00000000
    mtxer r1            //SO bit of XER is not cleared automatically
    addco. r2, r0, r1
    bns check_cpu_ppc_prog_model_27
    b error

check_cpu_ppc_prog_model_27:    //check XL-branch
    //trace_msg "check bltlr\n"
    lis r0, bic_blelr@h
    ori r0, r0, bic_blelr@l
    mtspr 8, r0
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    bltlr
    b error

bic_blelr:
    //trace_msg "check blelr\n"
    lis r0, bic_beqlr@h
    ori r0, r0, bic_beqlr@l
    mtspr 8, r0
    blelr
    b error

bic_beqlr:
    //trace_msg "check beqlr\n"
    lis r0, bic_bgtlr@h
    ori r0, r0, bic_bgtlr@l
    mtspr 8, r0
    load_const r0, 1
    load_const r1, 1
    cmp 0, 0, r0, r1
    beqlr
    b error

bic_bgtlr:
    //trace_msg "check bgtlr\n"
    lis r0, bic_bgelr@h
    ori r0, r0, bic_bgelr@l
    mtspr 8, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bgtlr
    b error

bic_bgelr:
    //trace_msg "check bgelr\n"
    lis r0, bic_bnllr@h
    ori r0, r0, bic_bnllr@l
    mtspr 8, r0
    bgelr
    b error

bic_bnllr:
    //trace_msg "check bnllr\n"
    lis r0, bic_bnelr@h
    ori r0, r0, bic_bnelr@l
    mtspr 8, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bnllr
    b error

bic_bnelr:
    //trace_msg "check bnelr\n"
    lis r0, bic_bnglr@h
    ori r0, r0, bic_bnglr@l
    mtspr 8, r0
    bnelr
    b error

bic_bnglr:
    //trace_msg "check bnglr\n"
    lis r0, bic_bsolr@h
    ori r0, r0, bic_bsolr@l
    mtspr 8, r0
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    bnglr
    b error

bic_bsolr:
    //trace_msg "check bsolr\n"
    lis r0, bic_bnslr@h
    ori r0, r0, bic_bnslr@l
    mtspr 8, r0
    load_const r0, 0xFFFFFFFF
    load_const r1, 0x80000000
    addco. r2, r0, r1
    bsolr
    b error

bic_bnslr:
    //trace_msg "check bnslr\n"
    lis r0, bic_bdz@h
    ori r0, r0, bic_bdz@l
    mtspr 8, r0
    load_const r0, 0xFFFFFFFF
    load_const r1, 0x00000000
    mtxer r1
    addco. r2, r0, r1
    bnslr
    b error

bic_bdz:
    //trace_msg "check bdz\n"
    load_const r0, 0x1
    mtspr 9, r0
    bdz bic_bdnz

bic_bdnz:
    //trace_msg "check bdnz\n"
    load_const r0, 0x1
    mtspr 9, r0
    bdnz bic_bltctr

bic_bltctr:
    //trace_msg "check bltctr\n"
    lis r0, bic_blectr@h
    ori r0, r0, bic_blectr@l
    mtspr 9, r0
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    bltctr
    b error

bic_blectr:
    //trace_msg "check blectr\n"
    lis r0, bic_beqctr@h
    ori r0, r0, bic_beqctr@l
    mtspr 9, r0
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    blectr
    b error

bic_beqctr:
    //trace_msg "check beqctr\n"
    lis r0, bic_bgtctr@h
    ori r0, r0, bic_bgtctr@l
    mtspr 9, r0
    load_const r0, 1
    load_const r1, 1
    cmp 0, 0, r0, r1
    beqctr
    b error

bic_bgtctr:
    //trace_msg "check bgtctr\n"
    lis r0, bic_bgectr@h
    ori r0, r0, bic_bgectr@l
    mtspr 9, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bgtctr
    b error

bic_bgectr:
    //trace_msg "check bgectr\n"
    lis r0, bic_bnlctr@h
    ori r0, r0, bic_bnlctr@l
    mtspr 9, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bgectr
    b error

bic_bnlctr:
    //trace_msg "check bnlctr\n"
    lis r0, bic_bnectr@h
    ori r0, r0, bic_bnectr@l
    mtspr 9, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bnlctr
    b error

bic_bnectr:
    //trace_msg "check bnectr\n"
    lis r0, bic_bngctr@h
    ori r0, r0, bic_bngctr@l
    mtspr 9, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bnectr
    b error

bic_bngctr:
    //trace_msg "check bngctr\n"
    lis r0, bic_bsoctr@h
    ori r0, r0, bic_bsoctr@l
    mtspr 9, r0
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    bngctr
    b error

bic_bsoctr:
    //trace_msg "check bsoctr\n"
    lis r0, bic_bnsctr@h
    ori r0, r0, bic_bnsctr@l
    mtspr 9, r0
    load_const r0, 0x7FFFFFFF
    load_const r1, 0x00000002
    addco. r2, r0, r1
    bsoctr
    b error

bic_bnsctr:
    //trace_msg "check bnsctr\n"
    lis r0, test_ok@h
    ori r0, r0, test_ok@l
    mtspr 9, r0
    load_const r0, 0xFFFFFFFF
    load_const r1, 0x00000000
    mtxer r1
    addco. r2, r0, r1
    bnsctr
    b error

test_ok:

    gtube_init
//    putchar 'T'
//    putchar 'E'
//    putchar 'S'
//    putchar 'T'
//    putchar ' '
    putchar 'O'
    putchar 'K'
    putchar '\n'
    exit 0
stop:
    b stop

error:

    gtube_init
//    putchar 'T'
//    putchar 'E'
//    putchar 'S'
//    putchar 'T'
//    putchar ' '
    putchar 'E'
    putchar 'R'
    putchar 'R'
    putchar 'O'
    putchar 'R'
    putchar '\n'
    exit 1

