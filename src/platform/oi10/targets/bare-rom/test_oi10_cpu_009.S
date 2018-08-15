
#define RUMBOOT_ASSERT_WARN_ONLY

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

#define R_ARG0                  r3
#define R_ARG1                  r4
#define R_ARG2                  r5
#define R_ARG3                  r6
#define R_RETV                  R_ARG0
#define R_RNGM                  r18     /* Range mask            */
#define R_RSLT                  r19     /* Result of command     */
#define R_ADDR                  r20     /* Address of I/O reg    */
#define R_WADR                  R_ADDR  /* Write I/O reg address */
#define R_RADR                  r21     /* Read I/O reg address  */
#define R_CADR                  R_RADR  /* Clear I/O reg address */
#define R_DATA                  r22     /* Readed data for check */
#define R_EXPV                  r23     /* Expected value        */
#define R_SHFT                  r24     /* Shift value           */
#define R_TERM                  r25     /* Termination template  */
#define R_MASK                  r26     /* Register mask         */
#define R_FILL                  r27     /* Fill value            */
#define R_TEMP                  r28     /* Temporary data        */
#define R_BACK                  r29     /* Common backup         */
#define R_ZERO                  r30     /* Holds 0x00000000      */
#define R_ONES                  r31     /* Holds 0xFFFFFFFF      */

#define TESR_MASK               0xFFFFF000
#define TESR_RANGE_MASK         0xFF800000
#define TESR_R0_FILL            0x00001000
#define TESR_R1_FILL            0x00800000
#define TESR_R10_BEG            0x00001000
#define TESR_R10_END            0x80000000

#define P640(OFFSET)			DCR_PLB6PLB4_0_BASE + (OFFSET)
#define P641(OFFSET)			DCR_PLB6PLB4_1_BASE + (OFFSET)
#define CVR3					r15,r16,r17

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

.macro check_running_10 blknum regaddr regmask regfill begval endval regname
    rumboot_putstring  "Check PLB6PLB4_\()\blknum\()_\()\regname running <1>\n"
    load_const  R_SHFT, 0x00000001
    /* Running <1> init */
    load_const  R_ADDR, \regaddr
    load_const  R_MASK, \regmask
    load_const  R_FILL, \regfill
    load_const  R_EXPV, \begval
    load_const  R_TERM, \endval
    mfdcrx      R_BACK, R_ADDR
    mtdcrx      R_ADDR, R_ZERO
    mfdcrx      R_DATA, R_ADDR
    and         R_DATA, R_DATA, R_MASK
    cmpw  cr7,  R_DATA, R_ZERO
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_\()\blknum\()_\()\regname (running 1 init)")
    /* Running <1> loop */
loop_1_\()\blknum:
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    and         R_DATA, R_DATA, R_MASK
    cmpw  cr7,  R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_\()\blknum\()_\()\regname (running 1)")
    cmpw  cr0,  R_EXPV, R_TERM
    beq-  cr0,  end_1_\()\blknum
    slw         R_EXPV, R_EXPV, R_SHFT
    b           loop_1_\()\blknum
end_1_\()\blknum:
    rumboot_putstring   "done.\n"
    rumboot_putstring   "Check PLB6PLB4_\()\blknum\()_\()\regname running <0>\n"
    /* Running <0> init */
    and         R_EXPV, R_ONES, R_MASK
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    and         R_DATA, R_DATA, R_MASK
    cmpw  cr7,  R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_\()\blknum\()_\()\regname (running 0 init)")
    load_const  R_EXPV, \begval
    not         R_EXPV, R_EXPV
    not         R_TERM, R_TERM
    /* Running <0> loop */
loop_0_\()\blknum:
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    and         R_DATA, R_DATA, R_MASK
    cmpw cr7,   R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_\()\blknum\()_\()\regname (running 0)")
    cmpw cr0,   R_EXPV, R_TERM
    beq- cr0,   end_0_\()\blknum
    slw         R_EXPV, R_EXPV, R_SHFT
    or          R_EXPV, R_EXPV, R_FILL
    b           loop_0_\()\blknum
end_0_\()\blknum:
    mtdcrx      R_ADDR, R_BACK
    rumboot_putstring   "done.\n"
.endm

.macro check_clr_esr crF
    mtdcrx      R_CADR, R_MASK
    mfdcrx      R_DATA, R_RADR
    and         R_DATA, R_DATA, R_MASK
    cmpw  \crF, R_DATA, R_ZERO
.endm

.macro check_running_10_tesr blknum dcrbase
    rumboot_putstring  "Check PLB6PLB4_\()\blknum\()_ESR/TESR running <1>\n"
    load_const  R_SHFT, 0x00000001
    /* Running <1> init */
    load_const  R_WADR, \dcrbase + TESR
    load_const  R_RADR, \dcrbase +  ESR
    load_const  R_MASK, TESR_MASK
    load_const  R_RNGM, TESR_RANGE_MASK
    load_const  R_FILL, TESR_R1_FILL
    load_const  R_EXPV, TESR_R10_BEG
    load_const  R_TERM, TESR_R10_END
    mfdcrx      R_BACK, R_RADR
    mtdcrx      R_WADR, R_ZERO
    mfdcrx      R_DATA, R_ADDR
    and         R_DATA, R_DATA, R_MASK
    and         R_BACK, R_BACK, R_MASK
    cmpw  cr7,  R_DATA, R_ZERO
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_\()\blknum\()_ESR/TESR (running 1 init)")
    /* Running <1> loop */
tesr_loop_1_\()\blknum:
    and         R_RSLT, R_EXPV, R_RNGM
    cmpw  cr5,  R_RSLT, R_ZERO
    beq+  cr5,  tesr_fill_1_\()\blknum
    or          R_TEMP, R_EXPV, R_ZERO
    b           tesr_skip_1_\()\blknum
tesr_fill_1_\()\blknum:
    or          R_TEMP, R_EXPV, R_FILL
tesr_skip_1_\()\blknum:
    mtdcrx      R_WADR, R_TEMP
    mfdcrx      R_DATA, R_RADR
    and         R_DATA, R_DATA, R_MASK
    cmpw  cr7,  R_DATA, R_TEMP
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_\()\blknum\()_ESR/TESR (running 1)")
    check_clr_esr  cr4
    TEST_ASSERT(eq,cr4,"PLB6PLB4_\()\blknum\()_ESR clear error (running 1)")
    cmpw  cr3,  R_EXPV, R_TERM
    beq-  cr3,  tesr_end_1_\()\blknum
    slw         R_EXPV, R_EXPV, R_SHFT
    b           tesr_loop_1_\()\blknum
tesr_end_1_\()\blknum:
    rumboot_putstring   "done.\n"
    rumboot_putstring   "Check PLB6PLB4_\()\blknum\()_ESR/TESR running <0>\n"
    /* Running <0> init */
    load_const  R_FILL, TESR_R0_FILL
    and         R_EXPV, R_ONES, R_MASK
    mtdcrx      R_WADR, R_EXPV
    mfdcrx      R_DATA, R_RADR
    and         R_DATA, R_DATA, R_MASK
    cmpw  cr7,  R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_\()\blknum\()_ESR/TESR (running 0 init)")
    check_clr_esr  cr4
    TEST_ASSERT(eq,cr4,"PLB6PLB4_\()\blknum\()_ESR clear error (running 0 init)")
    load_const  R_EXPV, TESR_R10_BEG
    not         R_EXPV, R_EXPV
    not         R_TERM, R_TERM
    and         R_TERM, R_TERM, R_MASK
    and         R_EXPV, R_EXPV, R_MASK
    /* Running <0> loop */
tesr_loop_0_\()\blknum:
    mtdcrx      R_WADR, R_EXPV
    mfdcrx      R_DATA, R_RADR
    and         R_DATA, R_DATA, R_MASK
    cmpw  cr7,  R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6PLB4_\()\blknum\()_ESR/TESR (running 0)")
    check_clr_esr  cr4
    TEST_ASSERT(eq,cr4,"PLB6PLB4_\()\blknum\()_ESR clear error (running 0)")
    cmpw  cr3,  R_EXPV, R_TERM
    beq-  cr3,  tesr_end_0_\()\blknum
    slw         R_EXPV, R_EXPV, R_SHFT
    or          R_EXPV, R_EXPV, R_FILL
    b           tesr_loop_0_\()\blknum
tesr_end_0_\()\blknum:
    mtdcrx      R_WADR, R_BACK
    rumboot_putstring   "done.\n"
.endm


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
    load_const  R_ZERO, 0x00000000
    load_const  R_ONES, 0xFFFFFFFF

	rumboot_putstring "Init SPRs... "

    mtspr       SPR_USPRG0, R_ZERO
    mtspr       SPR_SRR0,   R_ZERO
    mtspr       SPR_SRR1,   R_ZERO
    mtspr       SPR_CSRR0,  R_ZERO
    mtspr       SPR_CSRR1,  R_ZERO
    mtspr       SPR_MCSRR0, R_ZERO
    mtspr       SPR_MCSRR1, R_ZERO
    mtspr       SPR_ESR,    R_ZERO
    mtspr       SPR_DCESR,  R_ZERO
    mtspr       SPR_DEAR,   R_ZERO
    mtspr       SPR_MCSR_C, R_ONES

/*according to User's Manual ch. 9.4 Initialization Software Requirements*/
    mtspr       SPR_DBCR0, R_ZERO       /* disable all debug events*/

    load_const  R_DATA, CONST_SPR_DBSR_RC
    mtspr       SPR_DBSR_RC,    R_DATA     /* clear all debug interrupts*/

    load_const  R_DATA, CONST_SPR_CCR0
    mtspr       SPR_CCR0,   R_DATA

    load_const  R_DATA, CONST_SPR_CCR0
    mtspr       SPR_CCR1,   R_DATA

    load_const  R_DATA, CONST_SPR_CCR0
    mtspr       SPR_CCR2,   R_DATA

    load_const  R_DATA, CONST_SPR_MMUBE0
    mtspr       SPR_MMUBE0, R_DATA

    load_const  R_DATA, CONST_SPR_MMUBE1
    mtspr       SPR_MMUBE1, R_DATA

    load_const  R_DATA, CONST_SPR_SSPCR
    mtspr       SPR_SSPCR,  R_DATA

    load_const  R_DATA, CONST_SPR_ISPCR
    mtspr       SPR_ISPCR,  R_DATA

    load_const  R_DATA, CONST_SPR_USPCR
    mtspr       SPR_USPCR,  R_DATA

/*Before editing TLB let's initialize all SPR registers to default values by the spec*/
/*Most of them have X-state after reset and therefore CPU model cannot work properly*/

    mtspr       SPR_LR,     R_ZERO
    mtspr       SPR_CTR,	R_ZERO
    mtcr                    R_ZERO
    mtspr       SPR_XER,    R_ZERO

    mtspr       SPR_TCR,    R_ZERO
    mtspr       SPR_TBL_W,  R_ZERO
    mtspr       SPR_TBU_W,  R_ZERO
    mtspr       SPR_DEC,    R_ZERO
    mtspr       SPR_DECAR,  R_ZERO
    load_const  R_DATA,   (0b1  << TIMER_TSR_EVW_i)\
                        | (0b1  << TIMER_TSR_WIS_i)\
                        | (0b1  << TIMER_TSR_DIS_i)\
                        | (0b1  << TIMER_TSR_FIS_i)
    mtspr       SPR_TSR_RC, R_DATA

    mtspr       SPR_IAC1,   R_ZERO
    mtspr       SPR_IAC2,   R_ZERO
    mtspr       SPR_IAC3,   R_ZERO
    mtspr       SPR_IAC4,   R_ZERO
    mtspr       SPR_DAC1,   R_ZERO
    mtspr       SPR_DAC2,   R_ZERO
    mtspr       SPR_DVC1,   R_ZERO
    mtspr       SPR_DVC2,   R_ZERO
    mtspr       SPR_DBCR1,  R_ZERO
    mtspr       SPR_DBCR2,  R_ZERO
    mtspr       SPR_DBDR,   R_ZERO
    mtspr       SPR_RMPD,   R_ZERO

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
    load_addr   R_ARG0, tlb_entries
    load_const  R_ARG1, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries
    isync
	rumboot_putstring  "Done!\n"

    bl          begin_test
    bne- cr6,   test_error

test_ok:
    rumboot_putstring   "TEST_OK!\n"
    exit                RESULT_OK
    b                   test_finish

test_error:
    rumboot_putstring   "TEST_ERROR!\n"
    exit                RESULT_ERROR

test_finish:
test_finish_loop:
    b                   test_finish_loop

.section ".text","ax",@progbits

begin_test:
    set_cr      cr6,    eq

check_cpu_ppc_plb6plb4_0_2:
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
    rumboot_putstring   "done.\n"

check_cpu_ppc_plb6plb4_1_2:
    rumboot_putstring   "Check PLB6PLB4_1 reset values...\n"
    check_value CVR3,   P641(P64CR ),   P64CR_1_RESET_VAL,            "PLB6PLB4_1_P64CR"
    check_value CVR3,   P641(ESR   ),   ESR_RESET_VAL,                "PLB6PLB4_1_ESR"
    check_value CVR3,   P641(P64REV),   P64REV_RESET_VAL,             "PLB6PLB4_1_P64REV"
    check_value CVR3,   P641(EARH  ),   EARH_RESET_VAL,               "PLB6PLB4_1_EARH"
    check_value CVR3,   P641(EARL  ),   EARL_RESET_VAL,               "PLB6PLB4_1_EARL"
    check_value CVR3,   P641(ADDRL0),   PLB6PLB4_1_ADDRL0_RESET_VAL,  "PLB6PLB4_1_ADDRL0"
    check_value CVR3,   P641(ADDRH0),   PLB6PLB4_1_ADDRH0_RESET_VAL,  "PLB6PLB4_1_ADDRH0"
    check_value CVR3,   P641(ADDRL1),   ADDRL1_RESET_VAL,             "PLB6PLB4_1_ADDRL1"
    check_value CVR3,   P641(ADDRH1),   ADDRH1_RESET_VAL,             "PLB6PLB4_1_ADDRH1"
    rumboot_putstring   "done.\n"

    /*       block number | DCR-addr      | mask      | fill val  | begin val | end val   | reg name |  */
    check_running_10    0,  P640(ADDRL1),   0xFFFFFFFF, 0x00000001, 0x00000001, 0x80000000, "ADDRL1"
    check_running_10    1,  P640(ADDRL1),   0xFFFFFFFF, 0x00000001, 0x00000001, 0x80000000, "ADDRL1"

    /*           block number | base DCR-address   |    */
    check_running_10_tesr   0,  DCR_PLB6PLB4_0_BASE
    check_running_10_tesr   1,  DCR_PLB6PLB4_1_BASE

    blr


