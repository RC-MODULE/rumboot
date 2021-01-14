
#include <platform/trace.S.h>
#include <platform/test_assert.S.h>
#include <arch/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <arch/ppc_476fp_debug_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_timer_fields.h>
#include <platform/devices.h>

#define R_ARG0              r3
#define R_ARG1              r4
#define R_RETV              R_ARG0
#define R_ADDR              r20
#define R_EXPV              r21
#define R_DATA              r22
#define R_OFFT              r23
#define R_CMP1              r24
#define R_CMP2              r25
#define R_INCR              r26
#define R_CNTR              r27
#define R_TEMP              r28
#define R_BACK              r29

#define TEST_DATA_WORD_0    0x12345678
#define CVR3                r20,r21,r22
#define CVR4                r20,r23,r21,r22


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
    mflr        R_BACK
    load_addr   R_ARG0, tlb_entries
    load_addr   R_ARG1, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries /* (r3, r4) - are parameters */
    mtlr        R_BACK

    b           test_start


test_start:

    rumboot_putstring "start checking data addressing (IM0)\n"


    load_addr   R_ADDR, vars
    load_const  R_OFFT, 0x00000000
    load_const  R_TEMP, 0x00000000
    cmpwi cr6,  R_TEMP, 0x0000

    rumboot_putstring   "stb-lbz...\n"
    ckval   stb,    lbz,    CVR3, 0xFF
    ckval   stb,    lbz,    CVR3, 0xAA
    ckval   stb,    lbz,    CVR3, 0x55

    rumboot_putstring   "stbu-lbzu...\n"
    ckval   stbu,   lbzu,   CVR3, 0xFF
    ckval   stbu,   lbzu,   CVR3, 0xAA
    ckval   stbu,   lbzu,   CVR3, 0x55

    rumboot_putstring   "sth-lhz...\n"
    ckval   sth,    lhz,    CVR3, 0xFFFF
    ckval   sth,    lhz,    CVR3, 0xAAAA
    ckval   sth,    lhz,    CVR3, 0x5555

    rumboot_putstring   "sthu-lhzu...\n"
    ckval   sthu,   lhzu,   CVR3, 0xFFF7
    ckval   sthu,   lhzu,   CVR3, 0xAAA5
    ckval   sthu,   lhzu,   CVR3, 0x555A

    rumboot_putstring   "sth-lha...\n"
    ckval   sth,    lha,    CVR3, 0x7FFF
    ckval   sth,    lha,    CVR3, 0x5AAA
    ckval   sth,    lha,    CVR3, 0x3555

    rumboot_putstring   "sthu-lhau...\n"
    ckval   sthu,   lhau,   CVR3, 0x7FFE
    ckval   sthu,   lhau,   CVR3, 0x5AA5
    ckval   sthu,   lhau,   CVR3, 0x355A

    rumboot_putstring   "stw-lwz...\n"
    ckval   stw,    lwz,    CVR3, 0xFFFFFFFF
    ckval   stw,    lwz,    CVR3, 0xAAAAAAAA
    ckval   stw,    lwz,    CVR3, 0x55555555

    rumboot_putstring   "stwu-lwzu...\n"
    ckval   stwu,   lwzu,   CVR3, 0xFFFFFFFF
    ckval   stwu,   lwzu,   CVR3, 0xAAAAAAAA
    ckval   stwu,   lwzu,   CVR3, 0x55555555

    rumboot_putstring   "stbx-lbzx...\n"
    ckvalx  stbx,   lbzx,   CVR4, 0xFF
    ckvalx  stbx,   lbzx,   CVR4, 0xAA
    ckvalx  stbx,   lbzx,   CVR4, 0x55

    rumboot_putstring   "stbux-lbzux...\n"
    ckvalx  stbux,  lbzux,  CVR4, 0xFF
    ckvalx  stbux,  lbzux,  CVR4, 0xAA
    ckvalx  stbux,  lbzux,  CVR4, 0x55

    rumboot_putstring   "sthx-lhzx...\n"
    ckvalx  sthx,   lhzx,   CVR4, 0xFFFF
    ckvalx  sthx,   lhzx,   CVR4, 0xAAAA
    ckvalx  sthx,   lhzx,   CVR4, 0x5555

    rumboot_putstring   "sthux-lhzux...\n"
    ckvalx  sthux,  lhzux,  CVR4, 0xFFFF
    ckvalx  sthux,  lhzux,  CVR4, 0xAAAA
    ckvalx  sthux,  lhzux,  CVR4, 0x5555

    rumboot_putstring   "sthx-lhax...\n"
    ckvalx  sthx,   lhax,   CVR4, 0x7FFF
    ckvalx  sthx,   lhax,   CVR4, 0x5AAA
    ckvalx  sthx,   lhax,   CVR4, 0x3555

    rumboot_putstring   "sthux-lhaux...\n"
    ckvalx  sthux,  lhaux,  CVR4, 0x7FFE
    ckvalx  sthux,  lhaux,  CVR4, 0x5AA3
    ckvalx  sthux,  lhaux,  CVR4, 0x355A

    rumboot_putstring   "stwx-lwzx...\n"
    ckvalx  stwx,   lwzx,   CVR4, 0xFFFFFFFF
    ckvalx  stwx,   lwzx,   CVR4, 0xAAAAAAAA
    ckvalx  stwx,   lwzx,   CVR4, 0x55555555

    rumboot_putstring   "stwux-lwzux...\n"
    ckvalx  stwux,  lwzux,  CVR4, 0xFFFFFFFF
    ckvalx  stwux,  lwzux,  CVR4, 0xAAAAAAAA
    ckvalx  stwux,  lwzux,  CVR4, 0x55555555
    rumboot_putstring   "done\n"

    test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_23

mmu_check_0:

    /* store data in IM0. */
	rumboot_putstring  "Start MMU ckeck stage 0. Store to IM0 VA...\n"
    load_addr           R_ADDR, vars
    load_const          R_EXPV, TEST_DATA_WORD_0
    stwu                R_EXPV, 0(R_ADDR)
    /* check */
    lwzu                R_DATA, 0(R_ADDR)
    cmpw  cr7,          R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at IM0_BASE")

    /* read data from virtual address 0x00400000 */
    rumboot_putstring   "Read from VA 0x00400000...\n"
    load_const          R_ADDR, 0x00400000
    ori                 R_ADDR, R_ADDR, vars@l
    lwzu                R_DATA, 0(R_ADDR)
    cmpw  cr7,          R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at 0x00400000")

    /* read data from virtual address 0x00500000 */
    rumboot_putstring   "Read from VA 0x00500000...\n"
    load_const          R_ADDR, 0x00500000
    ori                 R_ADDR, R_ADDR, vars@l
    lwzu                R_DATA, 0(R_ADDR)
    cmpw  cr7,          R_DATA, R_EXPV
    TEST_ASSERT(eq,cr7,"Write/read value mismatch at 0x00500000")

    test_event EVENT_CHECK_CPU_PPC_MMU_0

mmu_check_1:

	rumboot_putstring "Start MMU ckeck stage 1.\n"
    mflr        R_BACK              /* save LR */
    load_const  R_CMP1, 0x00000001
    load_const  R_CMP2, 0x00000002
    load_const  R_INCR, 0x00000001
    load_const  R_CNTR, 0x00000000

    load_const  R_ADDR, 0xFFF70000
    ori         R_ADDR, R_ADDR,     some_code0@l

    load_const  R_TEMP, 0xFFF80000
    ori         R_TEMP, R_TEMP,     some_code0@l

	rumboot_putstring  "Branch to 0xFFF7xxxx... "
    mtctr       R_ADDR
    bctrl       /* Do branch */
    cmpw  cr7,  R_CNTR, R_CMP1
    TEST_ASSERT(eq,cr7,"MMU check 1: value(1) mismatch")
    rumboot_putstring   "Done!\n"
    rumboot_putstring   "Branch to 0xFFF8xxxx... "
    mtctr       R_TEMP
    bctrl       /* Do branch */
    cmpw  cr7,  R_CNTR, R_CMP2
    TEST_ASSERT(eq,cr7,"MMU check 1: value(2) mismatch")
    rumboot_putstring   "Done!\n"

    test_event EVENT_CHECK_CPU_PPC_MMU_1

    bne- cr6,   test_error

test_ok:
    rumboot_putstring "TEST OK\n"
    test_event  EVENT_OK
    load_const  R_RETV, RESULT_OK
    b           finish

test_error:
    rumboot_putstring "TEST ERROR\n"
    test_event  EVENT_ERROR
    load_const  R_RETV, RESULT_ERROR

finish:
    mtlr        R_BACK
    blr


some_code0:
    add         R_CNTR, R_CNTR, R_INCR
    rumboot_putstring   " -> here!\n"
    blr
    .long 0xBAD4DEAD

.section ".data","aw",@progbits

vars:
    .long   0x00000000
    .long   0x00000000
    .long   0x00000000
    .long   0x00000000

