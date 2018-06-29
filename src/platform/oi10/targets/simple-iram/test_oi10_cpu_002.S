
#include <platform/trace.S.h>
#include <platform/test_assert.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/devices.h>

#define TEST_DATA_WORD_0            0x12345678
#define CVR3                        r11,r12,r13
#define CVR4                        r11,r10,r12,r13


    /*        store |load  |addr |data |temp|expect
              cmd   |cmd   |reg  |reg  |reg |value 	*/
.macro ckval, stCmd, ldCmd, aReg, dReg, eReg, eVal
    load_const  \eReg,  \eVal
    \stCmd      \eReg,  0(\aReg)
    \ldCmd      \dReg,  0(\aReg)
    cmpw cr7,   \dReg,  \eReg
    TEST_ASSERT(eq,cr7,"\stCmd/\ldCmd write/read value \eVal mismatch")
.endm

    /*         store |load | addr |index|data |temp |expect
               cmd   |cmd  | reg  | reg |reg  |reg  |value 	*/
.macro ckvalx, stCmd, ldCmd, aReg, xReg, dReg, eReg, eVal
    load_const  \eReg,  \eVal
    \stCmd      \eReg,  \aReg,  \xReg
    \ldCmd      \dReg,  \aReg,  \xReg
    cmpw cr7,   \dReg,  \eReg
    TEST_ASSERT(eq,cr7,"\stCmd/\ldCmd write/read value \eVal mismatch")
.endm

.section ".text","ax",@progbits

.global main
.global some_code0

main:
    mtspr   SPR_SPRG7,   r1     /* Save stack! */
    mflr    r29                 /* save LR */
    mtspr   SPR_SPRG8,   r29    /* ------- */

    /* ppc470s_set_mem_window  r3, MEM_WINDOW_SHARED */
    b       setup_tlb

tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   ) */
    .long   MMU_TLB_ENTRY(  0x010,  0x80000,    0xFFF70,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x80000,    0xFFF80,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x80000,    0x00400,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x80010,    0x00410,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x80000,    0x00500,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
    .long   MMU_TLB_ENTRY(  0x010,  0x80010,    0x00510,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
.set TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE


/* implement__write_tlb_entries r3, r4, r8, r9, r10 */

setup_tlb:
    load_addr   r3, tlb_entries
    load_addr   r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries /* (r3, r4) - are parameters */

    b           test_oi10_cpu_002


test_oi10_cpu_002:

    rumboot_putstring "start checking data addressing (IM0)\n"

    load_addr   r11,    vars
    load_const  r10,    0       /* Clear index (offse) */

    rumboot_putstring "stb-lbz...\n"
    ckval	stb,	lbz,	CVR3, 0xFF
    ckval	stb,	lbz,	CVR3, 0xAA
    ckval	stb,	lbz,	CVR3, 0x55

    rumboot_putstring "stbu-lbzu...\n"
    ckval	stbu,	lbzu,	CVR3, 0xFF
    ckval	stbu,	lbzu,	CVR3, 0xAA
    ckval	stbu,	lbzu,	CVR3, 0x55

    rumboot_putstring "sth-lhz...\n"
    ckval	sth,	lhz,	CVR3, 0xFFFF
    ckval	sth,	lhz,	CVR3, 0xAAAA
    ckval	sth,	lhz,	CVR3, 0x5555

    rumboot_putstring "sthu-lhzu...\n"
    ckval	sthu,	lhzu,	CVR3, 0xFFF7
    ckval	sthu,	lhzu,	CVR3, 0xAAA5
    ckval	sthu,	lhzu,	CVR3, 0x555A

    rumboot_putstring "sth-lha...\n"
    ckval	sth,	lha,	CVR3, 0x7FFF
    ckval	sth,	lha,	CVR3, 0x5AAA
    ckval	sth,	lha,	CVR3, 0x3555

    rumboot_putstring "sthu-lhau...\n"
    ckval	sthu,	lhau,	CVR3, 0x7FFE
    ckval	sthu,	lhau,	CVR3, 0x5AA5
    ckval	sthu,	lhau,	CVR3, 0x355A

    rumboot_putstring "stw-lwz...\n"
    ckval	stw,	lwz,	CVR3, 0xFFFFFFFF
    ckval	stw,	lwz,	CVR3, 0xAAAAAAAA
    ckval	stw,	lwz,	CVR3, 0x55555555

    rumboot_putstring "stwu-lwzu...\n"
    ckval	stwu,	lwzu,	CVR3, 0xFFFFFFFF
    ckval	stwu,	lwzu,	CVR3, 0xAAAAAAAA
    ckval	stwu,	lwzu,	CVR3, 0x55555555

    rumboot_putstring "stbx-lbzx...\n"
    ckvalx	stbx,	lbzx,	CVR4, 0xFF
    ckvalx	stbx,	lbzx,	CVR4, 0xAA
    ckvalx	stbx,	lbzx,	CVR4, 0x55

    rumboot_putstring "stbux-lbzux...\n"
    ckvalx	stbux,	lbzux,	CVR4, 0xFF
    ckvalx	stbux,	lbzux,	CVR4, 0xAA
    ckvalx	stbux,	lbzux,	CVR4, 0x55

    rumboot_putstring "sthx-lhzx...\n"
    ckvalx	sthx,	lhzx,	CVR4, 0xFFFF
    ckvalx	sthx,	lhzx,	CVR4, 0xAAAA
    ckvalx	sthx,	lhzx,	CVR4, 0x5555

    rumboot_putstring "sthux-lhzux...\n"
    ckvalx	sthux,	lhzux,	CVR4, 0xFFFF
    ckvalx	sthux,	lhzux,	CVR4, 0xAAAA
    ckvalx	sthux,	lhzux,	CVR4, 0x5555

    rumboot_putstring "sthx-lhax...\n"
    ckvalx	sthx,	lhax,	CVR4, 0x7FFF
    ckvalx	sthx,	lhax,	CVR4, 0x5AAA
    ckvalx	sthx,	lhax,	CVR4, 0x3555

    rumboot_putstring "sthux-lhaux...\n"
    ckvalx	sthux,	lhaux,	CVR4, 0x7FFE
    ckvalx	sthux,	lhaux,	CVR4, 0x5AA3
    ckvalx	sthux,	lhaux,	CVR4, 0x355A

    rumboot_putstring "stwx-lwzx...\n"
    ckvalx	stwx,	lwzx,	CVR4, 0xFFFFFFFF
    ckvalx	stwx,	lwzx,	CVR4, 0xAAAAAAAA
    ckvalx	stwx,	lwzx,	CVR4, 0x55555555

    rumboot_putstring "stwux-lwzux...\n"
    ckvalx	stwux,	lwzux,	CVR4, 0xFFFFFFFF
    ckvalx	stwux,	lwzux,	CVR4, 0xAAAAAAAA
    ckvalx	stwux,	lwzux,	CVR4, 0x55555555

    test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_23

mmu_check_0:
    load_const r5, 0x123
    load_const r4, 0x321

    /* store data in IM0. */
	rumboot_putstring "Start MMU ckeck stage 0. Store to IM0 VA...\n"
    load_addr           r6, vars
    load_const          r7, TEST_DATA_WORD_0
    stwu                r7, 0(r6)
    /* check */
    lwzu                r10, 0(r6)
    cmpw  cr7,          r10, r7
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at IM0_BASE")

    /* read data from virtual address 0x00400000 */
	rumboot_putstring   "Read from VA 0x00400000...\n"
    load_const          r6,         0x00400000
    addi                r6,     r6, vars@l
    lwzu                r11,    0(r6)
    cmpw  cr7,          r10,    r11
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at 0x00400000")

    /* read data from virtual address 0x00500000 */
	rumboot_putstring   "Read from VA 0x00500000...\n"
    load_const          r6,         0x00500000
    addi                r6,     r6, vars@l
    lwzu                r12,    0(r6)
    cmpw  cr7,          r10,    r12
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at 0x00500000")

    test_event EVENT_CHECK_CPU_PPC_MMU_0

mmu_check_1:

	rumboot_putstring "\nStart MMU ckeck stage 1.\n"
    mflr r29                        /* save LR */
    load_const  r11,    0x00000001  /* cmp1 */
    load_const  r12,    0x00000002  /* cmp2 */
    load_const  r13,    0x00000001  /* incr val */
    load_const  r10,    0x00000000  /* counter */

    load_const  r17,    0xFFF70000
    ori         r17,    r17,    some_code0@l

    load_const  r18,    0xFFF80000
    ori         r18,    r18,    some_code0@l

	rumboot_putstring "Branch to 0xFFF7xxxx... "
    mtctr       r17
    bctrl       /* Do branch */
    cmpw cr7,   r11,    r10
    TEST_ASSERT(eq,cr7,"MMU check 1: value(1) mismatch")
    rumboot_putstring "Done!\nBranch to 0xFFF8xxxx... "
    mtctr       r18
    bctrl       /* Do branch */
    cmpw  cr7,  r12,    r10
    TEST_ASSERT(eq,cr7,"MMU check 1: value(2) mismatch")
    rumboot_putstring "Done!\n"

    test_event EVENT_CHECK_CPU_PPC_MMU_1

    b test_ok


test_ok:
    rumboot_putstring "\nTEST OK\n"
    test_event      EVENT_OK
    load_const r3,  RESULT_OK
    b finish

test_error:
    rumboot_putstring "\nTEST ERROR\n"
    test_event      EVENT_ERROR
    load_const r3,  RESULT_ERROR

finish:
    mfspr r29, SPR_SPRG8    /* Restore LR */
    mtlr  r29               /* ---------- */
    mfspr r1,  SPR_SPRG7    /* Restore stack! */
    blr


some_code0:
    add r10,r10,r13
    rumboot_putstring "ok "
    blr
    .long 0xBAD4DEAD

.section ".data","aw",@progbits

vars:
    .long   0x00000000
    .long   0x00000000
    .long   0x00000000
    .long   0x00000000

