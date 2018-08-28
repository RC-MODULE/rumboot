
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>


#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/devices.h>
#include <platform/regs/regs_plb6dma.h>


.section ".BOOT_ROM.init.text","ax",@progbits


.global boot_init
boot_init:
    load_const  r0, 0x00000000  // make r0 "all zeros" source
    load_const  r1, 0xFFFFFFFF  // make r1 "all ones" source
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
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_1GB)    << MMU_SSPCR_ORD2_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_SSPCR_ORD3_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB)  << MMU_SSPCR_ORD4_i)\
                  | (MMU_XSPCR_ORD_END                              << MMU_SSPCR_ORD5_i)
    mtspr       SPR_SSPCR, r3

    load_const  r3, (MMU_XSPCR_ORD_64KB     << MMU_ISPCR_ORD1_i)\
                  | (MMU_XSPCR_ORD_1GB      << MMU_ISPCR_ORD2_i)\
                  | (MMU_XSPCR_ORD_4KB      << MMU_ISPCR_ORD3_i)\
                  | (MMU_XSPCR_ORD_256MB    << MMU_ISPCR_ORD4_i)\
                  | (MMU_XSPCR_ORD_END      << MMU_ISPCR_ORD5_i)
    mtspr       SPR_ISPCR, r3

    load_const  r3, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB)   << MMU_USPCR_ORD1_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_1GB)    << MMU_USPCR_ORD2_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)    << MMU_USPCR_ORD3_i)\
                  | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB)  << MMU_USPCR_ORD4_i)\
                  | (MMU_XSPCR_ORD_END                              << MMU_USPCR_ORD5_i)
    mtspr       SPR_USPCR, r3

//Before editing TLB let's initialize all SPR registers to default values by the spec
//Most of them have X-state after reset and therefore CPU model cannot work properly

    mtspr       SPR_LR, r0
    mtspr       SPR_CTR, r0
    mtcr        r0
    mtspr       SPR_XER, r0

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

    mtspr       SPR_SPRG0, r0
    mtspr       SPR_SPRG1, r0
    mtspr       SPR_SPRG2, r0
    mtspr       SPR_SPRG3, r0
    mtspr       SPR_SPRG4, r0
    mtspr       SPR_SPRG5, r0
    mtspr       SPR_SPRG6, r0
    mtspr       SPR_SPRG7, r0
    mtspr       SPR_SPRG8, r0

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

    b           setup_tlb

tlb_entries:
//          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
// TLB entries to remap 64KB pages IM0 from 0x00000010_00040000-0x00000010_0007FFFF to 0x00040000-0x0007FFFF
    .long   MMU_TLB_ENTRY(  0x010,  0x00040,    0x00040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00050,    0x00050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00060,    0x00060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x00070,    0x00070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
// TLB entries to remap 1GB pages DDR3 EM0 from 0x00000000_00000000-0x00000001_FFFFFFFF to 0x40000000-0x7FFFFFFF
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
DECLARE_CONST( TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE )

setup_tlb:
//    set_mem_window  r3, MEM_WINDOW_SHARED
    load_addr   r3, tlb_entries
    load_const  r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries

setup_stack: //stack is growing down
    mfspr       r3, SPR_PIR         //read Processor ID Register
    cmpwi       r3, PPC0_CORE_ID    //if Processor ID Register is 0 (primary)
    beq         setup_cpu0_stack
    //else
    load_addr   r1, cpu1_stack_top
    b           setup_cpu1_stack
setup_cpu0_stack:
    load_addr   r1, cpu0_stack_top
setup_cpu1_stack:
    stwu        r0, -8(r1) //allocate LR and save SP of first stack frame according to ABI rules

setup_ddr:
    bl          ddr_init_em0

    b           test_mpw_cpu_020_plb6_0

never_comes_here:
    test_event  EVENT_ERROR
    b           never_comes_here


.section ".IM0.data","aw",@progbits

BEGIN_ENUM( TEST_DATA )
DECLARE_ENUM_VAL( TD_RWNITC,            0x83417C7D )
DECLARE_ENUM_VAL( TD_READ,              0x8EAD7EDA )
DECLARE_ENUM_VAL( TD_RWITM,             0x831737DA )
DECLARE_ENUM_VAL( TD_READ_ATOMIC,       0x8EADA77D )
DECLARE_ENUM_VAL( TD_WRITE_WITH_FLUSH,  0x3817EF7D )
DECLARE_ENUM_VAL( TD_WRITE_WITH_KILL,   0x3817E87D )
DECLARE_ENUM_VAL( TD_WRITE_WITH_CLEAN,  0x3817EC7D )
DECLARE_ENUM_VAL( TD_DCLAIM,            0xDC1A137D )
//DECLARE_ENUM_VAL( TD_IKILL,             0x181117DA )
DECLARE_ENUM_VAL( TD_CLEAN,             0xC1EA47DA )
DECLARE_ENUM_VAL( TD_FLUSH,             0xF19587DA )
END_ENUM( TEST_DATA )

.align 4
plb6_rwnitc_test_data:
.long   TD_RWNITC
.align 4
plb6_write_with_flush_test_data:
.long   !TD_WRITE_WITH_FLUSH


.section ".IM0.text","ax",@progbits

BEGIN_ENUM( TEST_EVENT_CODE )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_RWNITC,            TEST_EVENT_CODE_MIN )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_READ,              TEST_EVENT_CODE_MIN+1 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_RWITM,             TEST_EVENT_CODE_MIN+2 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_READ_ATOMIC,       TEST_EVENT_CODE_MIN+3 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_WRITE_WITH_FLUSH,  TEST_EVENT_CODE_MIN+4 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_WRITE_WITH_KILL,   TEST_EVENT_CODE_MIN+5 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_WRITE_WITH_CLEAN,  TEST_EVENT_CODE_MIN+6 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_MSYNC,             TEST_EVENT_CODE_MIN+7 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_MBAR,              TEST_EVENT_CODE_MIN+8 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_TLBSYNC,           TEST_EVENT_CODE_MIN+9 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_DCLAIM,            TEST_EVENT_CODE_MIN+10 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_IKILL,             TEST_EVENT_CODE_MIN+11 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_CLEAN,             TEST_EVENT_CODE_MIN+12 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_FLUSH,             TEST_EVENT_CODE_MIN+13 )
DECLARE_ENUM_VAL( TEC_CHECK_PLB6_TLBIE,             TEST_EVENT_CODE_MIN+14 )
END_ENUM( TEST_EVENT_CODE )


test_mpw_cpu_020_plb6_0:

check_plb6_rwnitc:
    trace_msg   "check_plb6_rwnitc\n"

    load_addr   r1, plb6_rwnitc_test_data
    test_event  TEC_CHECK_PLB6_RWNITC
    lwz         r1, 0(r1)
    load_const  r0, TD_RWNITC
    cmpw        r0, r1
    TEST_ASSERT( eq, cr0, "No expected RWNITC test data" )

check_plb6_read:
    trace_msg   "check_plb6_read\n"

    load_addr   r1, plb6_read_test_data
    test_event  TEC_CHECK_PLB6_READ
    lwz         r1, 0(r1)
    load_const  r0, TD_READ
    cmpw        r0, r1
    TEST_ASSERT( eq, cr0, "No expected Read test data" )

check_plb6_rwitm:
    trace_msg   "check_plb6_rwitm\n"

    load_addr   r1, plb6_rwitm_test_data
    test_event  TEC_CHECK_PLB6_RWITM
    dcbtst      0, r1, 2
    lwz         r1, 0(r1)
    load_const  r0, TD_RWITM
    cmpw        r0, r1
    TEST_ASSERT( eq, cr0, "No expected RWITM test data" )

check_plb6_read_atomic:
    trace_msg   "check_plb6_read_atomic\n"

    load_addr   r1, plb6_read_atomic_test_data
    test_event  TEC_CHECK_PLB6_READ_ATOMIC
    lwarx       r1, 0, r1
    load_const  r0, TD_READ_ATOMIC
    cmpw        r0, r1
    TEST_ASSERT( eq, cr0, "No expected Read Atomic test data" )

check_plb6_write_with_flush:
    trace_msg   "check_plb6_write_with_flush\n"

    load_addr   r1, plb6_write_with_flush_test_data
    load_const  r0, TD_WRITE_WITH_FLUSH
    test_event  TEC_CHECK_PLB6_WRITE_WITH_FLUSH
    stw         r0, 0(r1)

check_plb6_write_with_kill:
    trace_msg   "check_plb6_write_with_kill\n"

    load_addr   r1, plb6_write_with_kill_test_data
    lwz         r2, 0(r1)
    load_const  r0, TD_WRITE_WITH_KILL
    stw         r0, 0(r1)
    mbar
    test_event  TEC_CHECK_PLB6_WRITE_WITH_KILL
    dcbf        0, r1
    msync

check_plb6_write_with_clean:
    trace_msg   "check_plb6_write_with_clean\n"

    load_addr   r1, plb6_write_with_clean_test_data
    lwz         r2, 0(r1)
    load_const  r0, TD_WRITE_WITH_CLEAN
    stw         r0, 0(r1)
    mbar
    test_event  TEC_CHECK_PLB6_WRITE_WITH_CLEAN
    dcbst       0, r1
    msync

check_plb6_msync:
    trace_msg   "check_plb6_msync\n"

    test_event  TEC_CHECK_PLB6_MSYNC
    msync

check_plb6_mbar:
    trace_msg   "check_plb6_mbar\n"

    test_event  TEC_CHECK_PLB6_MBAR
    mbar

check_plb6_tlbsync:
    trace_msg   "check_plb6_tlbsync\n"

    test_event  TEC_CHECK_PLB6_TLBSYNC
    tlbsync

check_plb6_dclaim:
    trace_msg   "check_plb6_dclaim\n"

    load_addr   r1, plb6_dclaim_test_data
    test_event  TEC_CHECK_PLB6_DCLAIM
    dcbz        0, r1
    msync

check_plb6_ikill:
    trace_msg   "check_plb6_ikill\n"

    bl          plb6_ikill_test_code
    load_addr   r1, plb6_ikill_test_code

    test_event  TEC_CHECK_PLB6_IKILL
    icbi        0, r1

check_plb6_clean:
    trace_msg   "check_plb6_clean\n"

    load_addr   r1, plb6_clean_test_data
    lwz         r2, 0(r1)
    load_const  r0, TD_CLEAN
    stw         r0, 0(r1)
    dcbi        0, r1
    mbar
    test_event  TEC_CHECK_PLB6_CLEAN
    dcbst       0, r1
    msync

check_plb6_flush:
    trace_msg   "check_plb6_flush\n"

    load_addr   r1, plb6_flush_test_data
    lwz         r2, 0(r1)
    load_const  r0, TD_FLUSH
    stw         r0, 0(r1)
    dcbi        0, r1
    mbar
    test_event  TEC_CHECK_PLB6_FLUSH
    dcbf        0, r1
    msync

check_plb6_tlbie:
    trace_msg   "check_plb6_tlbie\n"

    load_const  r1, 0x00000000
    test_event  TEC_CHECK_PLB6_TLBIE
    tlbivax     0, r1
    msync

/*
error:
    test_event  EVENT_ERROR
    b           finish
*/
finish:
    test_event  EVENT_FINISHED
    b           finish


.section ".EM0.data","awx",@progbits
.org 0x00000000
__ddr_init_data:
.space 128, 0x00
.align 7
plb6_read_test_data:
.long   TD_READ
.align 7
plb6_rwitm_test_data:
.long   TD_RWITM
.align 7
plb6_read_atomic_test_data:
.long   TD_READ_ATOMIC
.align 7
plb6_write_with_kill_test_data:
.long   !TD_WRITE_WITH_KILL
.align 7
plb6_write_with_clean_test_data:
.long   !TD_WRITE_WITH_CLEAN
.align 7
plb6_dclaim_test_data:
.long   TD_DCLAIM
.align 7
plb6_ikill_test_code:
    blr
//.long   TD_IKILL
.align 7
plb6_clean_test_data:
.long   TD_CLEAN
.align 7
plb6_flush_test_data:
.long   TD_FLUSH
.align 7
em0_data_end:
