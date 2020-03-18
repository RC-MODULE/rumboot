#include <arch/ppc_476fp_asm.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/trace.S.h>


DECLARE_CONST( PHYSICAL_SPACE_NOT_ENTERED,  0x407E47E8 )
DECLARE_CONST( PHYSICAL_SPACE_ENTERED,      0xE47E8ED1 )

DECLARE_CONST( HANDLER_NOT_CALLED,          0x407CA11E )
DECLARE_CONST( HANDLER_CALLED,              0xCA11ED11 )

DECLARE_CONST( TB_RESULT_UNDEFINED,         0x00000000 )
DECLARE_CONST( TB_RESULT_OK,                0x08111111 )
DECLARE_CONST( TB_RESULT_FINISHED,          0xF14158ED )
DECLARE_CONST( TB_RESULT_ERROR,             0xE8808111 )

.macro sync_all
    isync
    mbar
    tlbsync
    msync
.endm

.section ".text","ax",@progbits
.org 0x00010000
.global main


main:
    full_prologue
    b           setup_initial_tlb

initial_tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                  V   )*/
    .long   MMU_TLB_ENTRY(  0x010,  0x80000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b1,0b1,0b1,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,     0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x80010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b1,0b1,0b1,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,     0b1 )
DECLARE_CONST( INITIAL_TLB_ENTRIES_N, (.-initial_tlb_entries)/MMU_TLB_ENTRY_SIZE )

/* implement__write_tlb_entries r3, r4, r8, r9, r10 */

setup_initial_tlb:
    rumboot_putstring "Setting up initial TLBs...\n"
    load_addr   r3, initial_tlb_entries
    load_const  r4, INITIAL_TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries  /* (r3, r4) - are parameters */
    sync_all
    load_addr   r3, test_cpu_020_tlb
    mtctr       r3
    bctr

test_cpu_020_tlb:
    load_const  r3, 0x80040000
    load_const  r4, 0x80010000
    load_const  r5, 65536
    bl         memcpy
    msync

    rumboot_putstring "Init interrupt vectors...\n"
    lis         r3, interrupt_handlers@h
    mtspr       SPR_IVPR, r3

    li          r3, unexpected_interrupt_handler@l
    mtspr       SPR_IVOR0,  r3
    mtspr       SPR_IVOR1,  r3
    mtspr       SPR_IVOR2,  r3
    mtspr       SPR_IVOR3,  r3
    mtspr       SPR_IVOR4,  r3
    mtspr       SPR_IVOR5,  r3
/*  mtspr       SPR_IVOR6,  r3*/
    mtspr       SPR_IVOR7,  r3
/*  mtspr       SPR_IVOR8,  r3 */
    mtspr       SPR_IVOR9,  r3
    mtspr       SPR_IVOR10, r3
    mtspr       SPR_IVOR11, r3
    mtspr       SPR_IVOR12, r3
/*  mtspr       SPR_IVOR13, r3*/
/*  mtspr       SPR_IVOR14, r3 */
    mtspr       SPR_IVOR15, r3

    li          r3, dtlb_miss_handler@l
    mtspr       SPR_IVOR13, r3

    li          r3, pgm_interrupt_handler@l
    mtspr       SPR_IVOR6,  r3

    li          r3, system_call_handler@l
    mtspr       SPR_IVOR8, r3

    li          r3, itlb_miss_handler@l
    mtspr       SPR_IVOR14, r3

    load_const  r21, PHYSICAL_SPACE_NOT_ENTERED
    load_const  r22, HANDLER_NOT_CALLED

enter_usermode:
    load_addr   r3, usermode_entry
    mfmsr       r23
    load_const  r4, (0b1 << ITRPT_XSR_PR_i)
    mtsrr0      r3
    mtsrr1      r4
    rumboot_putstring "Entering usermode...\n"
    rfi

generate_syscall:
    sc

usermode_entry:
    b           generate_syscall


test_error:
    full_epilogue
    load_const      r3, RESULT_ERROR
    blr

test_ok:
    full_epilogue
    load_const       r3, RESULT_OK
    blr

notify_page_2:
    rumboot_putstring "We are at the second memory page!\n"
    blr

check_tlb_sync:
    nop

    load_const  r3, PHYSICAL_SPACE_ENTERED
    cmpw        cr7, r3, r21
    TEST_ASSERT( eq, cr7, "Physical space not entered" )

    load_const  r4, HANDLER_CALLED
    cmpw        cr7, r4, r22
    TEST_ASSERT( eq, cr7, "Intruction TLB miss handler not called" )

    load_addr   r3, test_ok
    mtsrr0      r3
    mtsrr1      r23
    rfi

physical_entry:
    load_const  r21, PHYSICAL_SPACE_ENTERED
    load_addr   r3, notify_page_2
    mtctr       r3
    bctrl
    /* bl notify_page_2 */
    load_addr   r3,  check_tlb_sync
    mtctr       r3
    bctr

/* This stuff do nothing */
    .long   0x55AA55AA
    .long   0xAA55AA55
    .long   0xFFFFFFFF
    .long   0xFFFFFFFF


.section ".text.rom","ax",@progbits

missed_tlb_replace_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                    IL1I,   IL1D,    W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                  V   ) */
    .long   MMU_TLB_ENTRY(  0x020,  0xC0040,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b1,0b1,0b1,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,     0b1 )
DECLARE_CONST( MISSED_TLB_REPLACE_ENTRIES_N, (.-missed_tlb_replace_entries)/MMU_TLB_ENTRY_SIZE )

.align 4
interrupt_handlers:
unexpected_interrupt_handler:
    rumboot_putstring   "Unexpected unknown interrupt!\n"
    b           test_error

.align 4
dtlb_miss_handler:
    rumboot_putstring   "Unexpected dtlb miss interrupt!\n"
    b           test_error

.align 4
pgm_interrupt_handler:
    rumboot_putstring   "Unexpected program interrupt (6)!\n"
    b           test_error

.align 4
system_call_handler:
    rumboot_putstring "Processing syscall... "
    //              MMU_MMUCR( REALE, DULXE, IULXE, TS,  TID               )
    load_const  r3, MMU_MMUCR( 0b0,   0b0,   0b0,   0b0, MEM_WINDOW_SHARED )
    load_const  r4, 0x80010000
    mtspr       SPR_MMUCR, r3
    isync
    tlbivax     0, r4
    sync_all
    rfi

.align 4
itlb_miss_handler:
    rumboot_putstring "Processing ITLB miss... \n"
    load_const  r3,  HANDLER_CALLED
    cmpw        cr4, r3, r22
    beq-        cr4, itlb_miss_2
    mr          r22, r3
    rumboot_putstring "Add TLB entries\n"
    load_addr   r3, missed_tlb_replace_entries
    load_const  r4, MISSED_TLB_REPLACE_ENTRIES_N
    bl          ppc470s_write_tlb_entries
    sync_all
    load_addr   r3, physical_entry
    mtctr       r3
    bctr
itlb_miss_2:
    mfsrr0      r30
    mfsrr0      r29
    rumboot_putstring "Called twice!\n"
    load_addr   r3, check_tlb_sync
    mtctr       r3
    bctr
    /* rfi */

