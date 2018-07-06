
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
#define CVR3					r20,r21,r22

#define CONST_SPR_MMUBE0  (0b0 << MMU_MMUBE0_VBE0_i)\
                        | (0b0 << MMU_MMUBE0_VBE1_i)\
                        | (0b0 << MMU_MMUBE0_VBE2_i)

#define CONST_SPR_MMUBE1  (0b0 << MMU_MMUBE1_VBE3_i)\
                        | (0b0 << MMU_MMUBE1_VBE4_i)\
                        | (0b0 << MMU_MMUBE1_VBE5_i)

#define CONST_SPR_SSPCR   ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_SSPCR_ORD1_i)\
                        | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB ) << MMU_SSPCR_ORD2_i)\
                        | ((0                     | MMU_XSPCR_ORD_END ) << MMU_SSPCR_ORD3_i)

#define CONST_SPR_ISPCR   (MMU_XSPCR_ORD_64KB << MMU_ISPCR_ORD1_i)\
                        | (MMU_XSPCR_ORD_4KB  << MMU_ISPCR_ORD2_i)\
                        | (MMU_XSPCR_ORD_END  << MMU_ISPCR_ORD3_i)

#define CONST_SPR_USPCR   ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_USPCR_ORD1_i)\
                        | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB ) << MMU_USPCR_ORD2_i)\
                        | ((0                     | MMU_XSPCR_ORD_END ) << MMU_USPCR_ORD3_i)

#define CONST_SPR_DBSR_RC                                  \
                          (0b1    << DEBUG_DBSR_IDE_i     )\
                        | (0b1    << DEBUG_DBSR_UDE_i     )\
                        | (0b1    << DEBUG_DBSR_ICMP_i    )\
                        | (0b1    << DEBUG_DBSR_BRT_i     )\
                        | (0b1    << DEBUG_DBSR_IRPT_i    )\
                        | (0b1    << DEBUG_DBSR_TRAP_i    )\
                        | (0b1    << DEBUG_DBSR_IAC1_i    )\
                        | (0b1    << DEBUG_DBSR_IAC2_i    )\
                        | (0b1    << DEBUG_DBSR_IAC3_i    )\
                        | (0b1    << DEBUG_DBSR_IAC4_i    )\
                        | (0b1    << DEBUG_DBSR_DAC1R_i   )\
                        | (0b1    << DEBUG_DBSR_DAC1W_i   )\
                        | (0b1    << DEBUG_DBSR_DAC2R_i   )\
                        | (0b1    << DEBUG_DBSR_DAC2W_i   )\
                        | (0b1    << DEBUG_DBSR_RET_i     )\
                        | (0b1    << DEBUG_DBSR_IAC12ATS_i)\
                        | (0b1    << DEBUG_DBSR_IAC34ATS_i)

#define CONST_SPR_CCR0                                                           \
                          (0b0                            << CTRL_CCR0_ITE_i    )\
                        | (0b1                            << CTRL_CCR0_PRE_i    )\
                        | (0b1                            << CTRL_CCR0_CRPE_i   )\
                        | (CTRL_CCR0_ICS_32byte           << CTRL_CCR0_ICS_i    )\
                        | (0b0                            << CTRL_CCR0_DAPUIB_i )\
                        | (0b0000                         << CTRL_CCR0_ICWRIDX_i)\
                        | (0b0                            << CTRL_CCR0_DTB_i    )\
                        | (0b0                            << CTRL_CCR0_FLSTA_i  )\
                        | (CTRL_CCR0_DQWPM_No_prediction  << CTRL_CCR0_DQWPM_i  )\
                        | (CTRL_CCR0_IQWPM_Use_EA         << CTRL_CCR0_IQWPM_i  )

#define CONST_SPR_CCR1                                                           \
                          (0b00                           << CTRL_CCR1_GPRPEI_i )\
                        | (0b00                           << CTRL_CCR1_FPRPEI_i )\
                        | (0b00                           << CTRL_CCR1_ICDPEI_i )\
                        | (0b00                           << CTRL_CCR1_ICLPEI_i )\
                        | (0b00                           << CTRL_CCR1_ICTPEI_i )\
                        | (0b00                           << CTRL_CCR1_DCDPEI_i )\
                        | (0b00                           << CTRL_CCR1_DCLPEI_i )\
                        | (0b00                           << CTRL_CCR1_DCTPEI_i )\
                        | (0b0                            << CTRL_CCR1_MMUTPEI_i)\
                        | (0b0                            << CTRL_CCR1_MMUDPEI_i)\
                        | (CTRL_CCR1_TSS_CPU_clock        << CTRL_CCR1_TSS_i    )\
                        | (0b0                            << CTRL_CCR1_DPC_i    )\
                        | (CTRL_CCR1_TCS_div1             << CTRL_CCR1_TCS_i    )

#define CONST_SPR_CCR2                                                           \
                          (CTRL_CCR2_DSTG_enabled         << CTRL_CCR2_DSTG_i   )\
                        | (0b0                            << CTRL_CCR2_DLFPD_i  )\
                        | (0b0                            << CTRL_CCR2_DSTI_i   )\
                        | (0b0                            << CTRL_CCR2_PMUD_i   )\
                        | (0b0                            << CTRL_CCR2_DCSTGW_i )\
                        | (0                              << CTRL_CCR2_STGCTR_i )\
                        | (0b0                            << CTRL_CCR2_DISTG_i  )\
                        | (0b0                            << CTRL_CCR2_SPC5C1_i )\
                        | (0b0                            << CTRL_CCR2_MCDTO_i  )


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:

    gtube_init

    load_const  r0,     0x00000000
    load_const  r1,     0x00000000
    load_const  r2,     0x00000000
    load_const  r3,     0x00000000
    load_const  r4,     0x00000000
    load_const  r5,     0x00000000
    load_const  r6,     0x00000000
    load_const  r7,     0x00000000
    load_const  r8,     0x00000000
    load_const  r9,     0x00000000
    load_const  r10,    0x00000000
    load_const  r11,    0x00000000
    load_const  r12,    0x00000000
    load_const  r13,    0x00000000
    load_const  r14,    0x00000000
    load_const  r15,    0x00000000
    load_const  r16,    0x00000000
    load_const  r17,    0x00000000
    load_const  r18,    0x00000000
    load_const  r19,    0x00000000
    load_const  r20,    0x00000000
    load_const  r21,    0x00000000
    load_const  r22,    0x00000000
    load_const  r23,    0x00000000
    load_const  r24,    0x00000000
    load_const  r25,    0x00000000
    load_const  r26,    0x00000000
    load_const  r27,    0x00000000
    load_const  r28,    0x00000000
    load_const  r29,    0x00000000
    load_const  r30,    0x00000000
    load_const  r31,    0xFFFFFFFF

	rumboot_putstring "Init SPRs... "

    mtspr       SPR_USPRG0, r30

    mtspr       SPR_SRR0,   r30
    mtspr       SPR_SRR1,   r30
    mtspr       SPR_CSRR0,  r30
    mtspr       SPR_CSRR1,  r30
    mtspr       SPR_MCSRR0, r30
    mtspr       SPR_MCSRR1, r30
    mtspr       SPR_MCSR_C, r31
    mtspr       SPR_ESR,    r30
    mtspr       SPR_DCESR,  r30
    mtspr       SPR_DEAR,   r30

/*according to User's Manual ch. 9.4 Initialization Software Requirements*/
    mtspr       SPR_DBCR0, r30       /* disable all debug events*/

    load_const  r3, CONST_SPR_DBSR_RC
    mtspr       SPR_DBSR_RC,    r3     /* clear all debug interrupts*/

    load_const  r3, CONST_SPR_CCR0
    mtspr       SPR_CCR0,   r3

    load_const  r3, CONST_SPR_CCR0
    mtspr       SPR_CCR1,   r3

    load_const  r3, CONST_SPR_CCR0
    mtspr       SPR_CCR2,   r3

    load_const  r3, CONST_SPR_MMUBE0
    mtspr       SPR_MMUBE0, r3

    load_const  r3, CONST_SPR_MMUBE1
    mtspr       SPR_MMUBE1, r3

    load_const  r3, CONST_SPR_SSPCR
    mtspr       SPR_SSPCR,  r3

    load_const  r3, CONST_SPR_ISPCR
    mtspr       SPR_ISPCR,  r3

    load_const  r3, CONST_SPR_USPCR
    mtspr       SPR_USPCR,  r3

/*Before editing TLB let's initialize all SPR registers to default values by the spec*/
/*Most of them have X-state after reset and therefore CPU model cannot work properly*/

    mtspr       SPR_LR,     r30
    mtspr       SPR_CTR,	r30
    mtcr                    r30
    mtspr       SPR_XER,    r30

    mtspr       SPR_TCR,    r30
    mtspr       SPR_TBL_W,  r30
    mtspr       SPR_TBU_W,  r30
    mtspr       SPR_DEC,    r30
    mtspr       SPR_DECAR,  r30
    load_const  r3, (0b1    << TIMER_TSR_EVW_i)\
                  | (0b1    << TIMER_TSR_WIS_i)\
                  | (0b1    << TIMER_TSR_DIS_i)\
                  | (0b1    << TIMER_TSR_FIS_i)
    mtspr       SPR_TSR_RC, r3

    mtspr       SPR_IAC1,   r30
    mtspr       SPR_IAC2,   r30
    mtspr       SPR_IAC3,   r30
    mtspr       SPR_IAC4,   r30
    mtspr       SPR_DAC1,   r30
    mtspr       SPR_DAC2,   r30
    mtspr       SPR_DVC1,   r30
    mtspr       SPR_DVC2,   r30
    mtspr       SPR_DBCR1,  r30
    mtspr       SPR_DBCR2,  r30
    mtspr       SPR_DBDR,   r30
    mtspr       SPR_RMPD,   r30

    rumboot_putstring  "Done!\n"

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
	rumboot_putstring  "Init TLB entries... "
    load_addr   r3, tlb_entries
    load_const  r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries
    isync
	rumboot_putstring  "Done!\n"

    bl          begin_test
    bne- cr6,   test_error

test_ok:
    rumboot_putstring   "TEST_OK!\n"
    test_event          EVENT_OK
    exit                RESULT_OK
    b                   test_finish

test_error:
    rumboot_putstring   "TEST_ERROR!\n"
    test_event          EVENT_ERROR
    exit                RESULT_ERROR

test_finish:
    test_event          EVENT_FINISHED
test_finish_loop:
    b                   test_finish_loop

.section ".text","ax",@progbits

begin_test:
check_cpu_ppc_plb6plb4_0_2:
    cmpwi cr6,  r30,    0x0000
    rumboot_putstring   "Check PLB6PLB4_0 reset values...\n"
    check_value CVR3,   P640(P64CR ),   P64CR_0_RESET_VAL,              "PLB6PLB4_0_P64CR"
    check_value CVR3,   P640(ESR   ),   ESR_RESET_VAL,                  "PLB6PLB4_0_ESR"
    check_value CVR3,   P640(P64REV),   P64REV_RESET_VAL,               "PLB6PLB4_0_P64REV"
    check_value CVR3,   P640(EARH  ),   EARH_RESET_VAL,                 "PLB6PLB4_0_EARH"
    check_value CVR3,   P640(EARL  ),   EARL_RESET_VAL,                 "PLB6PLB4_0_EARL"
    check_value CVR3,   P640(ADDRL0),   PLB6PLB4_0_ADDRL0_RESET_VAL,    "PLB6PLB4_0_ADDRL0"
    check_value CVR3,   P640(ADDRH0),   PLB6PLB4_0_ADDRH0_RESET_VAL,    "PLB6PLB4_0_ADDRH0"
    check_value CVR3,   P640(ADDRL1),   ADDRL1_RESET_VAL,               "PLB6PLB4_0_ADDRL1"
    check_value CVR3,   P640(ADDRH1),   ADDRH1_RESET_VAL,               "PLB6PLB4_0_ADDRH1"
    check_value CVR3,   P640(TESR  ),   TESR_RESET_VAL,                 "PLB6PLB4_0_TESR"
    rumboot_putstring   "done.\n"

check_cpu_ppc_plb6plb4_0_3:
	rumboot_putstring  "Check PLB6PLB4_0 running <1>\n"
    load_const  r11,    0x00000000
    load_const  r10,    P640(ADDRL1)
    mtdcrx      r10,    r11
    mfdcrx      r12,    r10
    cmpw cr7,   r12,    r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL0")

plb6plb4_0_running_1:
    load_const  r11,    0x00000001
    load_const  r3,     0x00000001
    load_const  r4,     0x80000000
plb6plb4_0_loop_1:
    mtdcrx      r10,    r11
    mfdcrx      r12,    r10
    cmpw cr7,   r12,    r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL1 (running 1)")
    cmpw cr0,   r11,    r4
    beq- cr0,   plb6plb4_0_running_0
    slw         r11,    r11,    r3
    b           plb6plb4_0_loop_1

plb6plb4_0_running_0:
    rumboot_putstring   "done.\n"
    rumboot_putstring   "Check PLB6PLB4_0 running <0>\n"
    load_const  r11,    0xFFFFFFFF
    load_const  r10,    P640(ADDRL1)
    mtdcrx      r10,    r11
    mfdcrx      r12,    r10
    cmpw cr7,   r12,    r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL1 (running 0)")

    load_const  r11,    0xFFFFFFFE
    load_const  r3,     0x00000001
    load_const  r4,     0x7FFFFFFF
plb6plb4_0_loop_0:
    mtdcrx      r10,    r11
    mfdcrx      r12,    r10
    cmpw cr7,   r12,    r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_0_ADDRL1 (running 0)")
    cmpw cr0,   r11,    r4
    beq- cr0,   end_plb6plb4_0
    slw         r11,    r11,    r3
    ori         r11,    r11,    0x0001
    b           plb6plb4_0_loop_0
end_plb6plb4_0:
    rumboot_putstring   "done.\n"

check_cpu_ppc_plb6plb4_1_2:
    rumboot_putstring   "Check PLB6PLB4_1 reset values...\n"
    check_value CVR3,   P641(P64CR ), P64CR_1_RESET_VAL,            "PLB6PLB4_1_P64CR"
    check_value CVR3,   P641(ESR   ), ESR_RESET_VAL,                "PLB6PLB4_1_ESR"
    check_value CVR3,   P641(P64REV), P64REV_RESET_VAL,             "PLB6PLB4_1_P64REV"
    check_value CVR3,   P641(EARH  ), EARH_RESET_VAL,               "PLB6PLB4_1_EARH"
    check_value CVR3,   P641(EARL  ), EARL_RESET_VAL,               "PLB6PLB4_1_EARL"
    check_value CVR3,   P641(ADDRL0), PLB6PLB4_1_ADDRL0_RESET_VAL,  "PLB6PLB4_1_ADDRL0"
    check_value CVR3,   P641(ADDRH0), PLB6PLB4_1_ADDRH0_RESET_VAL,  "PLB6PLB4_1_ADDRH0"
    check_value CVR3,   P641(ADDRL1), ADDRL1_RESET_VAL,             "PLB6PLB4_1_ADDRL1"
    check_value CVR3,   P641(ADDRH1), ADDRH1_RESET_VAL,             "PLB6PLB4_1_ADDRH1"
    check_value CVR3,   P641(TESR  ), TESR_RESET_VAL,               "PLB6PLB4_1_TESR"
    rumboot_putstring   "done.\n"

check_cpu_ppc_plb6plb4_1_3:
    rumboot_putstring   "Check PLB6PLB4_1 running <1>\n"
    load_const  r11,    0x00000000
    load_const  r10,    P641(ADDRL1)
    mtdcrx      r10,    r11
    mfdcrx      r12,    r10
    cmpw cr7,   r12,    r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1")

plb6plb4_1_running_1:
    load_const  r11,	0x00000001
    load_const  r3,     0x00000001
    load_const  r4,     0x80000000
plb6plb4_1_loop_1:
    mtdcrx      r10,    r11
    mfdcrx      r12,    r10
    cmpw cr7,   r12,    r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1 (running 1)")
    cmpw cr0,   r11,    r4
    beq- cr0,   plb6plb4_1_running_0
    slw         r11,    r11,    r3
    b           plb6plb4_1_loop_1

plb6plb4_1_running_0:
    rumboot_putstring   "done.\n"
    rumboot_putstring   "Check PLB6PLB4_1 running <0>\n"
    load_const  r11,    0xFFFFFFFF
    load_const  r10,    P641(ADDRL1)
    mtdcrx      r10,    r11
    mfdcrx      r12,    r10
    cmpw cr7,   r12,    r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1 (running 0)")

    load_const  r11,    0xFFFFFFFE
    load_const  r3,     0x00000001
    load_const  r4,     0x7FFFFFFF
plb6plb4_1_loop_0:
    mtdcrx      r10,    r11
    mfdcrx      r12,    r10
    cmpw cr7,   r12,    r11
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_1_ADDRL1 (running 0)")
    cmpw cr0,   r11,    r4
    beq- cr0,   end_plb6plb4_1
    slw         r11,    r11,    r3
    ori         r11,    r11,    0x0001
    b plb6plb4_1_loop_0
end_plb6plb4_1:
    rumboot_putstring   "done.\n"

    blr


