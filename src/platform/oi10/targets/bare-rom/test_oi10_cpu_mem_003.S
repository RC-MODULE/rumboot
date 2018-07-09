
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/trace.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>


.section ".init.start","ax",@progbits

boot_start:
    b           rumboot_entry_point

.section ".init.text","ax",@progbits

rumboot_entry_point:
	gtube_init
	test_event_send_test_id "test_oi10_cpu_mem_003"
    load_const  r0, 0x00000000  /* make r0 "all zeros" source */
    load_const  r1, 0xFFFFFFFF  /* make r1 "all ones" source  */
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
/* according to User's Manual ch. 9.4 Initialization Software Requirements */
    mtspr       SPR_DBCR0, r0       /* disable all debug events */

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
    mtspr       SPR_DBSR_RC, r3     /* clear all debug interrupts */

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

    load_const  r3, ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_4KB)   << MMU_SSPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_16KB)  << MMU_SSPCR_ORD2_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_64KB)  << MMU_SSPCR_ORD3_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1MB)   << MMU_SSPCR_ORD4_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_16MB)  << MMU_SSPCR_ORD5_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_256MB) << MMU_SSPCR_ORD6_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB)   << MMU_SSPCR_ORD7_i)
    mtspr       SPR_SSPCR, r3

    load_const  r3, (MMU_XSPCR_ORD_4KB   << MMU_ISPCR_ORD1_i)\
                  | (MMU_XSPCR_ORD_16KB  << MMU_ISPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_64KB  << MMU_ISPCR_ORD3_i)\
                  | (MMU_XSPCR_ORD_1MB   << MMU_ISPCR_ORD4_i)\
                  | (MMU_XSPCR_ORD_16MB  << MMU_ISPCR_ORD5_i)\
                  | (MMU_XSPCR_ORD_256MB << MMU_ISPCR_ORD6_i)\
                  | (MMU_XSPCR_ORD_1GB   << MMU_ISPCR_ORD7_i)
    mtspr       SPR_ISPCR, r3

    load_const  r3, ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_4KB)   << MMU_USPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_16KB)  << MMU_USPCR_ORD2_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_64KB)  << MMU_USPCR_ORD3_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1MB)   << MMU_USPCR_ORD4_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_16MB)  << MMU_USPCR_ORD5_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_256MB) << MMU_USPCR_ORD6_i)\
                  | ((MMU_SUSPCR_ORD_PID_ONLY | MMU_XSPCR_ORD_1GB)   << MMU_USPCR_ORD7_i)
    mtspr       SPR_USPCR, r3

/*
	Before editing TLB let's initialize all SPR registers to default values by the spec
	Most of them have X-state after reset and therefore CPU model cannot work properly
*/

    mtspr       SPR_LR,		r0
    mtspr       SPR_CTR,	r0
    mtcr        r0
    mtspr       SPR_XER,	r0

    mtspr       SPR_SRR0,	r0
    mtspr       SPR_SRR1,	r0
    mtspr       SPR_CSRR0,	r0
    mtspr       SPR_CSRR1,	r0
    mtspr       SPR_MCSRR0,	r0
    mtspr       SPR_MCSRR1,	r0
    mtspr       SPR_MCSR_C,	r1
    mtspr       SPR_ESR,	r0
    mtspr       SPR_DCESR,	r0
    mtspr       SPR_DEAR,	r0

    mtspr       SPR_TCR,	r0
    mtspr       SPR_TBL_W,	r0
    mtspr       SPR_TBU_W,	r0
    mtspr       SPR_DEC,	r0
    mtspr       SPR_DECAR,	r0
    load_const  r3, (0b1    << TIMER_TSR_EVW_i)\
                  | (0b1    << TIMER_TSR_WIS_i)\
                  | (0b1    << TIMER_TSR_DIS_i)\
                  | (0b1    << TIMER_TSR_FIS_i)
    mtspr       SPR_TSR_RC,	r3

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
    mtspr       SPR_RMPD,	r0

    b           test_cpu_mem_003


DECLARE_CONST( UTLB_TEST_DATA_SIZE, 4 )
DECLARE_CONST( UTLB_TEST_DATA_N,    6 )

BEGIN_ENUM( UTLB_TEST_DATA )
DECLARE_ENUM_VAL( UTLB_TEST_TLBE_ADDR,  0*UTLB_TEST_DATA_SIZE )
DECLARE_ENUM_VAL( UTLB_TEST_TLBE_TAG,   1*UTLB_TEST_DATA_SIZE )
DECLARE_ENUM_VAL( UTLB_TEST_TLBE_DATA,  2*UTLB_TEST_DATA_SIZE )
DECLARE_ENUM_VAL( UTLB_TEST_TLBE_ATTR,  3*UTLB_TEST_DATA_SIZE )
DECLARE_ENUM_VAL( UTLB_TEST_MMUCR,      4*UTLB_TEST_DATA_SIZE )
DECLARE_ENUM_VAL( UTLB_TEST_TLBWE_RA,   5*UTLB_TEST_DATA_SIZE )
END_ENUM( UTLB_TEST_DATA )

DECLARE_CONST( TEST_TLB_ENTRY_SIZE, UTLB_TEST_DATA_N*UTLB_TEST_DATA_SIZE )

#define TEST_TLB_ENTRY(index,way,EPN,EPNPar,V,TS,DSIZ,DSIZPar,TID,TIDPar,ERPN,ERPNPar,RPN,RPNPar,IL1I,IL1D,W,I,M,G,E,UX,UW,UR,SX,SW,SR,U,SPPar)\
    MMU_TLBE_ADDR(index,way),\
    MMU_TLBE_TAG(EPN,V,TS,DSIZ,0b0,EPNPar,DSIZPar,TIDPar),\
    MMU_TLBE_DATA(ERPN,RPN,ERPNPar,RPNPar),\
    MMU_TLBE_ATTR(IL1I,IL1D,W,I,M,G,E,UX,UW,UR,SX,SW,SR,U,SPPar),\
    MMU_MMUCR(0b0,0b0,0b0,TS,TID),\
    ((0b100 | way) << MMU_TLBWE_RA_WAY_i) | (MMU_TLBWE_BE_UND << MMU_TLBWE_RA_BE_i)

test_write_tlb_entries: // write UTLB DATA for invalig (V=0) UTLB TAG entry
/*			TEST_TLB_ENTRY( index,      way,    EPN,                    EPNPar, V,      TS,     DSIZ,       DSIZPar,TID,                TIDPar, ERPN,           ERPNPar,RPN,                    RPNPar, IL1I,   IL1D,   W,      I,      M,      G,      E,      UX,     UW,     UR,     SX,     SW,     SR,     U,      SPPar) */
    .long   TEST_TLB_ENTRY( 0b11110111, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0000000011110111, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b0 )
test_read_tlb_entries:
/*			TEST_TLB_ENTRY( index,      way,    EPN,                    EPNPar, V,      TS,     DSIZ,       DSIZPar,TID,                TIDPar, ERPN,           ERPNPar,RPN,                    RPNPar, IL1I,   IL1D,   W,      I,      M,      G,      E,      UX,     UW,     UR,     SX,     SW,     SR,     U,      SPPar) */
    .long   TEST_TLB_ENTRY( 0b11111111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111100001111, 0b1,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111110, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111100001110, 0b0,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b0,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11111101, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111100001101, 0b0,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b0,    0b1,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11111011, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111100001011, 0b0,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11110111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111100000111, 0b0,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    0b1,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11101111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111100011111, 0b0,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11011111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111100101111, 0b0,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b10111111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111101001111, 0b0,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1110, 0b1 )
    .long   TEST_TLB_ENTRY( 0b01111111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111110001111, 0b0,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1101, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00000000, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111011110000, 0b0,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1011, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00000001, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111110111110001, 0b1,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b0111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00000010, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111101111110010, 0b1,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000100, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111011111110100, 0b1,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b0,    0b1,    0b1,    0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00001000, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1110111111111000, 0b1,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00010000, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1101111111100000, 0b1,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00100000, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1011111111010000, 0b1,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b01000000, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0111111110110000, 0b1,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b10000000, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111101110000, 0b0,    0b1111111111,   0b1,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11111111, 0b01,   0b11111111111111111110, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000000001110, 0b0,    0b1111111110,   0b0,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111110, 0b01,   0b11111111111111111101, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000000001100, 0b1,    0b1111111101,   0b0,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111101, 0b01,   0b11111111111111111011, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000000001001, 0b1,    0b1111111011,   0b0,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111011, 0b01,   0b11111111111111110111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000000000011, 0b1,    0b1111110111,   0b0,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11110111, 0b01,   0b11111111111111101111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000000010111, 0b1,    0b1111101111,   0b0,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11101111, 0b01,   0b11111111111111011111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000000111111, 0b1,    0b1111011111,   0b0,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11011111, 0b01,   0b11111111111110111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000001101111, 0b1,    0b1110111111,   0b0,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b10111111, 0b01,   0b11111111111101111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000011001111, 0b1,    0b1101111111,   0b0,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b01111111, 0b01,   0b11111111111011111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000010011111, 0b1,    0b1011111111,   0b0,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000000, 0b01,   0b11111111110111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000111010000, 0b1,    0b0111111111,   0b0,    0b11111111111111111111, 0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000001, 0b01,   0b11111111101111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000001010110001, 0b0,    0b1111111111,   0b1,    0b11111111111111111110, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000010, 0b01,   0b11111111011111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000010001110010, 0b0,    0b1111111111,   0b1,    0b11111111111111111101, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000100, 0b01,   0b11111110111111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000100011110101, 0b0,    0b1111111111,   0b1,    0b11111111111111111011, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00001000, 0b01,   0b11111101111111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0001000011111010, 0b0,    0b1111111111,   0b1,    0b11111111111111110111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00010000, 0b01,   0b11111011111111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0010000011100100, 0b0,    0b1111111111,   0b1,    0b11111111111111101111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00100000, 0b01,   0b11110111111111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0100000011011000, 0b0,    0b1111111111,   0b1,    0b11111111111111011111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b01000000, 0b01,   0b11101111111111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1000000010100000, 0b0,    0b1111111111,   0b1,    0b11111111111110111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b10000000, 0b01,   0b11011111111111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000001010000, 0b1,    0b1111111111,   0b1,    0b11111111111101111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111111, 0b10,   0b10111111111111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111101001111, 0b0,    0b1111111111,   0b1,    0b11111111111011111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111110, 0b10,   0b01111111111111111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111110001110, 0b1,    0b1111111111,   0b1,    0b11111111110111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111101, 0b10,   0b00000000000000000001, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111111111100, 0b1,    0b1111111111,   0b1,    0b11111111101111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111011, 0b10,   0b00000000000000000010, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111111111001, 0b1,    0b1111111111,   0b1,    0b11111111011111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11110111, 0b10,   0b00000000000000000100, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111111110011, 0b1,    0b1111111111,   0b1,    0b11111110111111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11101111, 0b10,   0b00000000000000001000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111111100111, 0b1,    0b1111111111,   0b1,    0b11111101111111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11011111, 0b10,   0b00000000000000010000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111111001111, 0b1,    0b1111111111,   0b1,    0b11111011111111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b10111111, 0b10,   0b00000000000000100000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111110011111, 0b1,    0b1111111111,   0b1,    0b11110111111111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b01111111, 0b10,   0b00000000000001000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111100111111, 0b1,    0b1111111111,   0b1,    0b11101111111111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000000, 0b10,   0b00000000000010000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111010000000, 0b1,    0b1111111111,   0b1,    0b11011111111111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000001, 0b10,   0b00000000000100000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111110100010001, 0b0,    0b1111111111,   0b1,    0b10111111111111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000010, 0b10,   0b00000000001000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111101100100010, 0b0,    0b1111111111,   0b1,    0b01111111111111111111, 0b0,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1,    0b1111, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000100, 0b10,   0b00000000010000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111011101000100, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b1,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00001000, 0b10,   0b00000000100000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1110111110001000, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00010000, 0b10,   0b00000001000000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1101111100010001, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00100000, 0b10,   0b00000010000000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1011111100100010, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b01000000, 0b10,   0b00000100000000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0111111101000100, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b10000000, 0b10,   0b00001000000000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111110001000, 0b1,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111111, 0b11,   0b00010000000000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000011101111, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0001, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111110, 0b11,   0b00100000000000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000011011110, 0b1,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0010, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111101, 0b11,   0b01000000000000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000010111101, 0b1,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0100, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11111011, 0b11,   0b10000000000000000000, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0000000001111011, 0b1,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b1000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11110111, 0b11,   0b00000000000000000000, 0b1,    0b0,    0b0,    0b000000,   0b1,    0b0000000011110111, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11101111, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b000001,   0b1,    0b0000000011101111, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b11011111, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b000011,   0b0,    0b0000000011011111, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b10111111, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b000111,   0b1,    0b0000000010111111, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b01111111, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b001111,   0b0,    0b0000000001111111, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000000, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b011111,   0b1,    0b0000000100000000, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000001, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b111111,   0b0,    0b0000001000000001, 0b1,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b0 )
    .long   TEST_TLB_ENTRY( 0b00000010, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b011111,   0b1,    0b0000010000000010, 0b1,    0b0000000001,   0b0,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00000100, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b001111,   0b0,    0b0000100000000100, 0b1,    0b0000000010,   0b0,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00001000, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b000111,   0b1,    0b0001000000001000, 0b1,    0b0000000100,   0b0,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00010000, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b000011,   0b0,    0b0010000000010000, 0b1,    0b0000001000,   0b0,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00100000, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b000001,   0b1,    0b0100000000100000, 0b1,    0b0000010000,   0b0,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b01000000, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b1,    0b000000,   0b1,    0b1000000001000000, 0b1,    0b0000100000,   0b0,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b10000000, 0b11,   0b00000000000000000000, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0000000010000000, 0b0,    0b0001000000,   0b0,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00001110, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111111111110, 0b0,    0b0010000000,   0b0,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00001101, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111111111101, 0b0,    0b0100000000,   0b0,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00001011, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111111111011, 0b0,    0b1000000000,   0b0,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00000111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111111110111, 0b0,    0b0000000000,   0b1,    0b00000000000000000001, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00011111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111111101111, 0b0,    0b0000000000,   0b1,    0b00000000000000000010, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00101111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111111011111, 0b0,    0b0000000000,   0b1,    0b00000000000000000100, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b01001111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111110111111, 0b0,    0b0000000000,   0b1,    0b00000000000000001000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b10001111, 0b00,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b1111111101111111, 0b0,    0b0000000000,   0b1,    0b00000000000000010000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00001110, 0b01,   0b11111111111111111110, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111111011111111, 0b0,    0b0000000000,   0b1,    0b00000000000000100000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00001101, 0b01,   0b11111111111111111101, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111110111111111, 0b0,    0b0000000000,   0b1,    0b00000000000001000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00001011, 0b01,   0b11111111111111111011, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111101111111111, 0b0,    0b0000000000,   0b1,    0b00000000000010000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00000111, 0b01,   0b11111111111111110111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1111011111111111, 0b0,    0b0000000000,   0b1,    0b00000000000100000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00011111, 0b01,   0b11111111111111101111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1110111111111111, 0b0,    0b0000000000,   0b1,    0b00000000001000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b00101111, 0b01,   0b11111111111111011111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1101111111111111, 0b0,    0b0000000000,   0b1,    0b00000000010000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b01001111, 0b01,   0b11111111111110111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b1011111111111111, 0b0,    0b0000000000,   0b1,    0b00000000100000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b10001111, 0b01,   0b11111111111101111111, 0b0,    0b1,    0b0,    0b000000,   0b0,    0b0111111111111111, 0b0,    0b0000000000,   0b1,    0b00000001000000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11110001, 0b10,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0000000000000001, 0b0,    0b0000000000,   0b1,    0b00000010000000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11110010, 0b10,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0000000000000010, 0b0,    0b0000000000,   0b1,    0b00000100000000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11110100, 0b10,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0000000000000100, 0b0,    0b0000000000,   0b1,    0b00001000000000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11111000, 0b10,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0000000000001000, 0b0,    0b0000000000,   0b1,    0b00010000000000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11100000, 0b10,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0000000000010000, 0b0,    0b0000000000,   0b1,    0b00100000000000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b11010000, 0b10,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0000000000100000, 0b0,    0b0000000000,   0b1,    0b01000000000000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b10110000, 0b10,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0000000001000000, 0b0,    0b0000000000,   0b1,    0b10000000000000000000, 0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
    .long   TEST_TLB_ENTRY( 0b01110000, 0b10,   0b11111111111111111111, 0b1,    0b1,    0b0,    0b000000,   0b0,    0b0000000010000000, 0b0,    0b0000000000,   0b1,    0b00000000000000000000, 0b1,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0,    0b0000, 0b1 )
.set TEST_READ_TLB_ENTRIES_N, ((.-test_read_tlb_entries)/TEST_TLB_ENTRY_SIZE)
.set TEST_WRITE_TLB_ENTRIES_N, ((.-test_write_tlb_entries)/TEST_TLB_ENTRY_SIZE)

DECLARE_CONST( MMU_MMUCR_STID_MASK,  0x0000FFFF )


test_cpu_mem_003:

BEGIN_ENUM( test_event_code )
DECLARE_ENUM_VAL(   TEC_CHECK_MMU_UTLB_READ,   TEST_EVENT_CODE_MIN )
DECLARE_ENUM_NEXT(  TEC_CHECK_MMU_UTLB_WRITE )
DECLARE_ENUM_NEXT(  TEC_INIT )
DECLARE_ENUM_NEXT(  TEC_FINISH )
END_ENUM( test_event_code )

	test_event  TEC_INIT

check_mmu_utlb_read:
    test_event  TEC_CHECK_MMU_UTLB_READ

    load_addr   r1, test_read_tlb_entries
    load_const  r2, 0
read_next_test_tlb_entry:
    lwz         r3, UTLB_TEST_TLBE_ADDR(r1)

    lwz         r4, UTLB_TEST_TLBE_TAG(r1)
    lwz         r5, UTLB_TEST_MMUCR(r1)
    andi.       r5, r5, MMU_MMUCR_STID_MASK
    tlbre       r6, r3, MMU_TLB_ENTRY_TAG
    cmpw        r6, r4
    TEST_ASSERT(eq, cr0, "Wrong UTLB TAG(EPN...)")

    mfspr       r7, SPR_MMUCR
    andi.       r7, r7, MMU_MMUCR_STID_MASK
    cmpw        r7, r5
    TEST_ASSERT(eq, cr0, "Wrong UTLB TAG(TID)")

    lwz         r4, UTLB_TEST_TLBE_DATA(r1)
    tlbre       r6, r3, MMU_TLB_ENTRY_DATA
    cmpw        r6, r4
    TEST_ASSERT(eq, cr0, "Wrong UTLB DATA(ERPN...)")

    lwz         r4, UTLB_TEST_TLBE_ATTR(r1)
    tlbre       r6, r3, MMU_TLB_ENTRY_ATTR
    cmpw        r6, r4
    TEST_ASSERT(eq, cr0, "Wrong UTLB DATA(IL1I...)")

    addi        r1, r1, TEST_TLB_ENTRY_SIZE
    addi        r2, r2, 1
    cmplwi      cr7, r2, TEST_READ_TLB_ENTRIES_N
    bne+        cr7, read_next_test_tlb_entry

check_mmu_utlb_write:
    test_event  TEC_CHECK_MMU_UTLB_WRITE

    load_addr   r1, test_write_tlb_entries
    load_const  r2, 0
write_next_test_tlb_entry:
    lwz         r3, UTLB_TEST_MMUCR(r1)
    mtspr       SPR_MMUCR, r3
    lwz         r4, UTLB_TEST_TLBWE_RA(r1)

    lwz         r5, UTLB_TEST_TLBE_TAG(r1)
    tlbwe       r5, r4, MMU_TLB_ENTRY_TAG

    lwz         r5, UTLB_TEST_TLBE_DATA(r1)
    tlbwe       r5, r4, MMU_TLB_ENTRY_DATA

    lwz         r5, UTLB_TEST_TLBE_ATTR(r1)
    tlbwe       r5, r4, MMU_TLB_ENTRY_ATTR

    addi        r1, r1, TEST_TLB_ENTRY_SIZE
    addi        r2, r2, 1
    cmplwi      cr7, r2, TEST_WRITE_TLB_ENTRIES_N
    bne+        cr7, write_next_test_tlb_entry

test_ok:
	test_event  TEC_FINISH
	exit 0x00

test_error:
 	rumboot_putstring "TEST_ERROR\n"
    exit 0x01



