
#undef __linux__
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/devices.h>

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

b	test_oi10_cpu_001

// .section ".text","ax",@progbits

test_oi10_cpu_001:

	rumboot_putstring "\ntest unconditional branch align "

    load_const r0 0x00000000
    mtxer r0
check_cpu_ppc_prog_model_24:    /*check sequential flow*/
    test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_24_SEQ

check_cpu_ppc_prog_model_25:    /*check unconditional branch*/
    putchar '3'
    putchar ' '
    b bu_b3
    test_event EVENT_ERROR
b_err:
    b b_err

.align 3
bu_b3:
    putchar '4'
    putchar ' '
    b bu_b4
    b error

.align 4
bu_b4:
    putchar '5'
    putchar ' '
    b bu_b5
    b error

.align 5
bu_b5:
    putchar '6'
    putchar ' '
    b bu_b6
    b error

.align 6
bu_b6:
    b bu_b
    b error

	rumboot_putstring "\ntest bl/ba"

bu_ba:
    bl bu_bl
    b bu_bla

bu_bla:
    bl bu_bl
    b check_cpu_ppc_prog_model_26

bu_b:
    ba bu_ba
    b error

bu_bl:
    blr
    b error

	rumboot_putstring "\ntest conditional branch\n -> "

check_cpu_ppc_prog_model_26:    /*check conditional branch*/
	putchar '0'
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    blt bc_ble
    b error

bc_ble:
	putchar '1'
    ble bc_beq
    b error

bc_beq:
	putchar '2'
    load_const r0, 1
    load_const r1, 1
    cmp 0, 0, r0, r1
    beq bc_bgt
    b error

bc_bgt:
	putchar '3'
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bgt bc_bge
    b error

bc_bge:
	putchar '4'
    bge bc_bnl
    b error

bc_bnl:
	putchar '5'
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bnl bc_bne
    b error

bc_bne:
	putchar '6'
    bne bc_bng
    b error

bc_bng:
	putchar '7'
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    bng bc_bso
    b error

bc_bso:
	putchar '8'
    load_const r0, 0x7FFFFFFF
    load_const r1, 0x00000002
    addco. r2, r0, r1 /*operands considered as signed, see PowerISA, 3.3.7*/
    bso bc_bns
    b error

bc_bns:
	putchar '9'
    load_const r0, 0xFFFFFFFF
    load_const r1, 0x00000000
    mtxer r1            /*SO bit of XER is not cleared automatically*/
    addco. r2, r0, r1
    bns check_cpu_ppc_prog_model_27
    b error

	rumboot_putstring "\ntest conditional branch to lr (XL-branch)\n -> "

check_cpu_ppc_prog_model_27:    /*check XL-branch*/
	putchar '0'
    lis r0, bic_blelr@h
    ori r0, r0, bic_blelr@l
    mtspr 8, r0
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    bltlr
    b error

bic_blelr:
	putchar '1'
    lis r0, bic_beqlr@h
    ori r0, r0, bic_beqlr@l
    mtspr 8, r0
    blelr
    b error

bic_beqlr:
	putchar '2'
    lis r0, bic_bgtlr@h
    ori r0, r0, bic_bgtlr@l
    mtspr 8, r0
    load_const r0, 1
    load_const r1, 1
    cmp 0, 0, r0, r1
    beqlr
    b error

bic_bgtlr:
	putchar '3'
    lis r0, bic_bgelr@h
    ori r0, r0, bic_bgelr@l
    mtspr 8, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bgtlr
    b error

bic_bgelr:
	putchar '4'
    lis r0, bic_bnllr@h
    ori r0, r0, bic_bnllr@l
    mtspr 8, r0
    bgelr
    b error

bic_bnllr:
	putchar '5'
    lis r0, bic_bnelr@h
    ori r0, r0, bic_bnelr@l
    mtspr 8, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bnllr
    b error

bic_bnelr:
	putchar '6'
    lis r0, bic_bnglr@h
    ori r0, r0, bic_bnglr@l
    mtspr 8, r0
    bnelr
    b error

bic_bnglr:
	putchar '7'
    lis r0, bic_bsolr@h
    ori r0, r0, bic_bsolr@l
    mtspr 8, r0
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    bnglr
    b error

bic_bsolr:
	putchar '8'
    lis r0, bic_bnslr@h
    ori r0, r0, bic_bnslr@l
    mtspr 8, r0
    load_const r0, 0xFFFFFFFF
    load_const r1, 0x80000000
    addco. r2, r0, r1
    bsolr
    b error

bic_bnslr:
	putchar '9'
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

    rumboot_putstring "\ntest bdz/bdzn "
    load_const r0, 0x1
    mtspr 9, r0
    bdz bic_bdnz

bic_bdnz:
    load_const r0, 0x1
    mtspr 9, r0
    bdnz bic_bltctr

	rumboot_putstring "\ntest cond branch to ctr"
bic_bltctr:
	putchar '0'
    lis r0, bic_blectr@h
    ori r0, r0, bic_blectr@l
    mtspr 9, r0
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    bltctr
    b error

bic_blectr:
	putchar '1'
    lis r0, bic_beqctr@h
    ori r0, r0, bic_beqctr@l
    mtspr 9, r0
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    blectr
    b error

bic_beqctr:
	putchar '2'
    lis r0, bic_bgtctr@h
    ori r0, r0, bic_bgtctr@l
    mtspr 9, r0
    load_const r0, 1
    load_const r1, 1
    cmp 0, 0, r0, r1
    beqctr
    b error

bic_bgtctr:
	putchar '3'
    lis r0, bic_bgectr@h
    ori r0, r0, bic_bgectr@l
    mtspr 9, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bgtctr
    b error

bic_bgectr:
	putchar '4'
    lis r0, bic_bnlctr@h
    ori r0, r0, bic_bnlctr@l
    mtspr 9, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bgectr
    b error

bic_bnlctr:
	putchar '5'
    lis r0, bic_bnectr@h
    ori r0, r0, bic_bnectr@l
    mtspr 9, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bnlctr
    b error

bic_bnectr:
	putchar '6'
    lis r0, bic_bngctr@h
    ori r0, r0, bic_bngctr@l
    mtspr 9, r0
    load_const r0, 1
    load_const r1, 0
    cmp 0, 0, r0, r1
    bnectr
    b error

bic_bngctr:
	putchar '7'
    lis r0, bic_bsoctr@h
    ori r0, r0, bic_bsoctr@l
    mtspr 9, r0
    load_const r0, 0
    load_const r1, 1
    cmp 0, 0, r0, r1
    bngctr
    b error

bic_bsoctr:
	putchar '8'
    lis r0, bic_bnsctr@h
    ori r0, r0, bic_bnsctr@l
    mtspr 9, r0
    load_const r0, 0x7FFFFFFF
    load_const r1, 0x00000002
    addco. r2, r0, r1
    bsoctr
    b error

bic_bnsctr:
	putchar '9'
    lis r0, test_ok@h
    ori r0, r0, test_ok@l
    mtspr 9, r0
    load_const r0, 0xFFFFFFFF
    load_const r1, 0x00000000
    mtxer r1
    addco. r2, r0, r1
    bnsctr
    b error

	rumboot_putstring "\nEND\n"


test_ok:

    putchar '\n'
    putchar 'T'
    putchar 'E'
    putchar 'S'
	putchar 'T'
	putchar ' '
    putchar 'O'
    putchar 'K'
    putchar '\n'
    exit 0

stop:
    b stop

error:

    putchar '\n'
    putchar 'T'
    putchar 'E'
    putchar 'S'
    putchar 'T'
    putchar ' '
    putchar 'E'
    putchar 'R'
    putchar 'R'
    putchar 'O'
    putchar 'R'
    putchar '\n'
    exit 1

