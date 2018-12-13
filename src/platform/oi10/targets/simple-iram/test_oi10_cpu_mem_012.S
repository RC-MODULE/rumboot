#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/devices.h>

#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_plb6plb4.h>

.macro insert_nop_0
.endm

.macro insert_nop_1
    ori r0, r0, 0
.endm

.macro insert_nop_2
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_3
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_4
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_5
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_6
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_7
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_8
    insert_nop_0
.endm

.macro insert_nop_9
    insert_nop_1
.endm

.macro set_bht_array_bits lbl, origin
    load_const r3, 0 //counter

    check_set_\lbl:
    addi r3, r3, 1
    cmp cr7, 0, r3, r4
    beq cr7, finish_check_set_\lbl
    b set_\lbl

    .org \origin  //drives mem[i]
    set_\lbl:
    insert_nop_\lbl
    beq check_set_\lbl //(r1=r2 counter will be incremented)

    finish_check_set_\lbl:
.endm

.macro clr_bht_array_bits lbl, origin
    load_const r3, 0 //counter

    check_clr_\lbl:
    addi r3, r3, 1
    cmp cr7, 0, r3, r4
    beq cr7, finish_check_clr_\lbl
    b clr_\lbl

    .org \origin
    clr_\lbl:
    insert_nop_\lbl
    beq finish_check_clr_\lbl
    b check_clr_\lbl
    ori r0, r0, 0

    finish_check_clr_\lbl:
.endm

.section ".text","ax",@progbits

.global main

main:
//according to User's Manual ch. 9.4 Initialization Software Requirements
    load_const  r0, 0x00000000 //DBCR0 value
    mtspr       SPR_DBCR0, r0       //DBCR0 clear

//Setup DBSR
    mfspr       r0, SPR_DBSR_RC        //read current DBSR
    mtspr       SPR_DBSR_RC, r0        //clear all valid bits

//CCR0                 ITE                 PRE                  CRPE               ICS                  DAPUIB               ICWRIDX                 DTB                  FLSTA                DQWPM                 IQWPM
    load_const  r0, reg_field(0, 0b0) | reg_field(1, 0b1) | reg_field(4, 0b0) | reg_field(10, 0b0) | reg_field(11, 0b0) | reg_field(15, 0b0000) | reg_field(16, 0b0) | reg_field(23, 0b0) | reg_field(29, 0b00) | reg_field(30, 0b1)
    mtspr       SPR_CCR0, r0

//CCR1                GPRPEI             FPRPEI                 ICDPEI              ICLPEI                ICTPEI                DCDPEI               DCLPEI                 DCTPEI              MMUTPEI              MMUDPEI                 TSS                 DPC                    TCS
    load_const  0, reg_field(1, 0b00) | reg_field(3, 0b00) | reg_field(5, 0b00) | reg_field(7, 0b00) | reg_field(9, 0b00) | reg_field(11, 0b00) | reg_field(13, 0b00) | reg_field(15, 0b00) | reg_field(16, 0b0) | reg_field(17, 0b0) | reg_field(19, 0b0) | reg_field(21, 0b0) | reg_field(23, 0b00)
    mtspr       SPR_CCR1, r0

//CCR2                 DSTG                  DLFPD            DSTI                  PMUD               DCSTGW                STGCTR                  DISTG               SPC5C1               MCDTO
    load_const  r0, reg_field(1, 0b00) | reg_field(2, 0b0) | reg_field(4, 0b0) | reg_field(9, 0b0) | reg_field(11, 0b0) | reg_field(15, 0b0000) | reg_field(16, 0b0) | reg_field(20, 0b0) | reg_field(21, 0b0)
    mtspr       SPR_CCR2, r0

    load_const  r0, (0b0 << MMU_MMUBE0_VBE0_i) | (0b0 << MMU_MMUBE0_VBE1_i) | (0b0 << MMU_MMUBE0_VBE2_i)
    mtspr       SPR_MMUBE0, r0

    load_const  r0, (0b0 << MMU_MMUBE1_VBE3_i) | (0b0 << MMU_MMUBE1_VBE4_i) | (0b0 << MMU_MMUBE1_VBE5_i)
    mtspr       SPR_MMUBE1, r0

    load_const  r0, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_SSPCR_ORD1_i) | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB) << MMU_SSPCR_ORD2_i) | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB) << MMU_SSPCR_ORD3_i) | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB) << MMU_SSPCR_ORD4_i) | (MMU_XSPCR_ORD_END << MMU_SSPCR_ORD5_i)
    mtspr       SPR_SSPCR, r0

    load_const  r0, (MMU_XSPCR_ORD_64KB << MMU_SSPCR_ORD1_i) | (MMU_XSPCR_ORD_4KB << MMU_SSPCR_ORD2_i) | (MMU_XSPCR_ORD_256MB << MMU_SSPCR_ORD3_i) | (MMU_XSPCR_ORD_1GB << MMU_SSPCR_ORD4_i) | (MMU_XSPCR_ORD_END << MMU_SSPCR_ORD5_i)
    mtspr       SPR_ISPCR, r0

    load_const  r0, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_SSPCR_ORD1_i) | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB) << MMU_SSPCR_ORD2_i) | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB) << MMU_SSPCR_ORD3_i) | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB) << MMU_SSPCR_ORD4_i) | (MMU_XSPCR_ORD_END << MMU_SSPCR_ORD5_i)
    mtspr       SPR_USPCR, r0

    load_const r0, 0
    //test_event TEST_MPW_CPU_MEM_012_EVENT_START
    b bht_check

    bht_check:
    //disable GHR
    mfspr r3, SPR_CCR0
    ori r3, r3, ((1 << IBM_BIT_INDEX(32, 20)))
    mtspr SPR_CCR0, r3

    check_set_bits:
    load_const r1, 5 //value for branch conditional
    load_const r2, 5 //value for branch conditional
    load_const r3, 0 //counter
    load_const r4, 6 //stop counter
    cmp cr0, 0, r1, r2 //r1==r2 for creation branch taken

    set_bht_array_bits 0, 0x4000 //mem[0][15:14]
    set_bht_array_bits 1, 0x4020 //mem[2][13:12]
    set_bht_array_bits 2, 0x8040 //mem[256][11:10]
    set_bht_array_bits 3, 0x8080 //mem[128][9:8]
    set_bht_array_bits 4, 0x8100 //mem[64][7:6]
    set_bht_array_bits 5, 0x8200 //mem[32][5:4]
    set_bht_array_bits 6, 0x8400 //mem[16][3:2]
    set_bht_array_bits 7, 0x8800 //mem[8][1:0]
    set_bht_array_bits 8, 0x9000 //mem[4][15:14]
    set_bht_array_bits 9, 0xA000 //mem[1][13:12]

    check_clr_bits:
    load_const r1, 6
    load_const r4, 4
    cmp cr0, 0, r1, r2 //r1!=r2 for creation branch not-taken

    clr_bht_array_bits 0, 0xC000 //mem[0][15:14]
    clr_bht_array_bits 1, 0xC020 //mem[2][13:12]
    clr_bht_array_bits 2, 0x10040 //mem[256][11:10]
    clr_bht_array_bits 3, 0x10080 //mem[128][9:8]
    clr_bht_array_bits 4, 0x10100 //mem[64][7:6]
    clr_bht_array_bits 5, 0x10200 //mem[32][5:4]
    clr_bht_array_bits 6, 0x10400 //mem[16][3:2]
    clr_bht_array_bits 7, 0x10800 //mem[8][1:0]
    clr_bht_array_bits 8, 0x11000 //mem[4][15:14]
    clr_bht_array_bits 9, 0x12000 //mem[1][13:12]

    //test_event EVENT_FINISHED
    test_ok:
    rumboot_putstring "\nTEST OK\n"
    test_event      EVENT_OK
    load_const r3,  RESULT_OK
    b infinite_loop

    infinite_loop:
        b infinite_loop
