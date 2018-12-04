
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>

#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/devices.h>
#include <platform/regs/regs_plb6dma.h>

#include <platform/ppc470s/mmu.S.h>


#define EM2_ADDR(addr)      (addr)-em2_data_begin+EM2_BASE

.section ".text","ax",@progbits

.global main

main:
    mflr        r3              /* Save */
    mtspr       SPR_USPRG0, r3  /* LR */

    load_addr   r3, DCR_EM2_EMI_BASE
    bl          emi_init

    load_const  r3, EM2_BASE
    load_addr   r4, em2_data_begin
    load_size   r5, EM2_DATA_SIZE
    bl          memcpy

    b           setup_tlb

tlb_entries:
//          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
    .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b1 )
    .set TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE

setup_tlb:
    load_addr   r3, tlb_entries
    load_size   r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries /*(r3, r4) - are parameters*/

    b           test_oi10_cpu_020_plb6_0


.section ".data","aw",@progbits

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


.section ".text","ax",@progbits

BEGIN_ENUM( test_event_code )
DECLARE_ENUM_VAL( EVENT_CHECK_PLB6_RWNITC, TEST_EVENT_CODE_MIN )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_READ )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_RWITM )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_READ_ATOMIC )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_WRITE_WITH_FLUSH )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_WRITE_WITH_KILL )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_WRITE_WITH_CLEAN )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_MSYNC )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_MBAR )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_TLBSYNC )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_DCLAIM )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_IKILL )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_CLEAN )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_FLUSH )
DECLARE_ENUM_NEXT( EVENT_CHECK_PLB6_TLBIE )
END_ENUM( test_event_code )


test_oi10_cpu_020_plb6_0:
    test_event_send_test_id "test_oi10_cpu_020_plb6"

check_plb6_rwnitc:
    rumboot_putstring   "check_plb6_rwnitc\n"

    load_addr   r3, plb6_rwnitc_test_data
    test_event  EVENT_CHECK_PLB6_RWNITC
    lwz         r3, 0(r3)
    load_const  r4, TD_RWNITC
    cmpw        r4, r3
    TEST_ASSERT( eq, cr0, "No expected RWNITC test data" )

check_plb6_read:
    rumboot_putstring   "check_plb6_read\n"

    load_addr   r3, EM2_ADDR(plb6_read_test_data)
    test_event  EVENT_CHECK_PLB6_READ
    lwz         r3, 0(r3)
    load_const  r4, TD_READ
    cmpw        r4, r3
    TEST_ASSERT( eq, cr0, "No expected Read test data" )

check_plb6_rwitm:
    rumboot_putstring   "check_plb6_rwitm\n"

    load_addr   r3, EM2_ADDR(plb6_rwitm_test_data)
    test_event  EVENT_CHECK_PLB6_RWITM
    dcbtst      0, r3, 2
    lwz         r3, 0(r3)
    load_const  r4, TD_RWITM
    cmpw        r4, r3
    TEST_ASSERT( eq, cr0, "No expected RWITM test data" )

check_plb6_read_atomic:
    rumboot_putstring   "check_plb6_read_atomic\n"

    load_addr   r3, EM2_ADDR(plb6_read_atomic_test_data)
    test_event  EVENT_CHECK_PLB6_READ_ATOMIC
    lwarx       r3, 0, r3
    load_const  r4, TD_READ_ATOMIC
    cmpw        r4, r3
    TEST_ASSERT( eq, cr0, "No expected Read Atomic test data" )

check_plb6_write_with_flush:
    rumboot_putstring   "check_plb6_write_with_flush\n"

    load_addr   r3, plb6_write_with_flush_test_data
    load_const  r4, TD_WRITE_WITH_FLUSH
    test_event  EVENT_CHECK_PLB6_WRITE_WITH_FLUSH
    stw         r4, 0(r3)

check_plb6_write_with_kill:
    rumboot_putstring   "check_plb6_write_with_kill\n"

    load_addr   r3, EM2_ADDR(plb6_write_with_kill_test_data)
    lwz         r2, 0(r3)
    load_const  r4, TD_WRITE_WITH_KILL
    stw         r4, 0(r3)
    mbar
    test_event  EVENT_CHECK_PLB6_WRITE_WITH_KILL
    dcbf        0, r3
    msync

check_plb6_write_with_clean:
    rumboot_putstring   "check_plb6_write_with_clean\n"

    load_addr   r3, EM2_ADDR(plb6_write_with_clean_test_data)
    lwz         r2, 0(r3)
    load_const  r4, TD_WRITE_WITH_CLEAN
    stw         r4, 0(r3)
    mbar
    test_event  EVENT_CHECK_PLB6_WRITE_WITH_CLEAN
    dcbst       0, r3
    msync

check_plb6_msync:
    rumboot_putstring   "check_plb6_msync\n"

    test_event  EVENT_CHECK_PLB6_MSYNC
    msync

check_plb6_mbar:
    rumboot_putstring   "check_plb6_mbar\n"

    test_event  EVENT_CHECK_PLB6_MBAR
    mbar

check_plb6_tlbsync:
    rumboot_putstring   "check_plb6_tlbsync\n"

    test_event  EVENT_CHECK_PLB6_TLBSYNC
    tlbsync

check_plb6_dclaim:
    rumboot_putstring   "check_plb6_dclaim\n"

    load_addr   r3, EM2_ADDR(plb6_dclaim_test_data)
    test_event  EVENT_CHECK_PLB6_DCLAIM
    dcbz        0, r3
    msync

check_plb6_ikill:
    rumboot_putstring   "check_plb6_ikill\n"

    load_addr   r3, EM2_ADDR(plb6_ikill_test_code)
    mtctr       r3
    bctrl

    test_event  EVENT_CHECK_PLB6_IKILL
    icbi        0, r3

check_plb6_clean:
    rumboot_putstring   "check_plb6_clean\n"

    load_addr   r3, EM2_ADDR(plb6_clean_test_data)
    lwz         r2, 0(r3)
    load_const  r4, TD_CLEAN
    stw         r4, 0(r3)
    dcbi        0, r3
    mbar
    test_event  EVENT_CHECK_PLB6_CLEAN
    dcbst       0, r3
    msync

check_plb6_flush:
    rumboot_putstring   "check_plb6_flush\n"

    load_addr   r3, EM2_ADDR(plb6_flush_test_data)
    lwz         r2, 0(r3)
    load_const  r4, TD_FLUSH
    stw         r4, 0(r3)
    dcbi        0, r3
    mbar
    test_event  EVENT_CHECK_PLB6_FLUSH
    dcbf        0, r3
    msync

check_plb6_tlbie:
    rumboot_putstring   "check_plb6_tlbie\n"

    load_const  r3, IM1_BASE
    test_event  EVENT_CHECK_PLB6_TLBIE
    tlbivax     0, r3
    msync

test_ok:
    rumboot_putstring "TEST OK\n"
    load_const  r3,  RESULT_OK
    b finish

test_error:
    rumboot_putstring "TEST ERROR\n"
    load_const  r3,  RESULT_ERROR

finish:
    mfspr       r5, SPR_USPRG0  /* LR */
    mtlr        r5              /* Restore */
    blr

.section ".data","aw",@progbits
.align 7
em2_data_begin:
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
em2_data_end:

.set EM2_DATA_SIZE, (.-em2_data_begin)
