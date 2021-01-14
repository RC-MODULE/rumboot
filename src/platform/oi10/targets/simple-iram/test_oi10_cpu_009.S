
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <arch/mmu.S.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/ppc_476fp_debug_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_timer_fields.h>
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


.global main

.section ".text","ax",@progbits

main:
test_init:
    set_cr      cr6,    eq
    load_const  R_ZERO, 0x00000000
    load_const  R_SHFT, 0x00000001
    not         R_ONES, R_ZERO
    b           test_begin

test_end:
    bne- cr6,   test_error

test_ok:
    rumboot_putstring   "TEST_OK!\n"
    load_const  R_RETV, RESULT_OK
    b                   test_finish

test_error:
    rumboot_putstring   "TEST_ERROR!\n"
    load_const  R_RETV, RESULT_ERROR

test_finish:
    blr

test_begin:
    rumboot_putstring   "Check PLB6PLB4_0 reset values...\n"
/*  check_value CVR3,   P640(P64CR ),   P64CR_0_RESET_VAL,              "PLB6PLB4_0_P64CR"  */
    check_value CVR3,   P640(ESR   ),   ESR_RESET_VAL,                  "PLB6PLB4_0_ESR"
    check_value CVR3,   P640(P64REV),   P64REV_RESET_VAL,               "PLB6PLB4_0_P64REV"
    check_value CVR3,   P640(EARH  ),   EARH_RESET_VAL,                 "PLB6PLB4_0_EARH"
    check_value CVR3,   P640(EARL  ),   EARL_RESET_VAL,                 "PLB6PLB4_0_EARL"
    check_value CVR3,   P640(ADDRL0),   PLB6PLB4_0_ADDRL0_RESET_VAL,    "PLB6PLB4_0_ADDRL0"
    check_value CVR3,   P640(ADDRH0),   PLB6PLB4_0_ADDRH0_RESET_VAL,    "PLB6PLB4_0_ADDRH0"
/*  check_value CVR3,   P640(ADDRL1),   ADDRL1_RESET_VAL,               "PLB6PLB4_0_ADDRL1" */
/*  check_value CVR3,   P640(ADDRH1),   ADDRH1_RESET_VAL,               "PLB6PLB4_0_ADDRH1" */
    rumboot_putstring   "done.\n"

check_cpu_ppc_plb6plb4_1_2:
    rumboot_putstring   "Check PLB6PLB4_1 reset values...\n"
    check_value CVR3,   P641(P64CR ),   P64CR_1_RESET_VAL,              "PLB6PLB4_1_P64CR"
    check_value CVR3,   P641(ESR   ),   ESR_RESET_VAL,                  "PLB6PLB4_1_ESR"
    check_value CVR3,   P641(P64REV),   P64REV_RESET_VAL,               "PLB6PLB4_1_P64REV"
    check_value CVR3,   P641(EARH  ),   EARH_RESET_VAL,                 "PLB6PLB4_1_EARH"
    check_value CVR3,   P641(EARL  ),   EARL_RESET_VAL,                 "PLB6PLB4_1_EARL"
    check_value CVR3,   P641(ADDRL0),   PLB6PLB4_1_ADDRL0_RESET_VAL,    "PLB6PLB4_1_ADDRL0"
    check_value CVR3,   P641(ADDRH0),   PLB6PLB4_1_ADDRH0_RESET_VAL,    "PLB6PLB4_1_ADDRH0"
    check_value CVR3,   P641(ADDRL1),   ADDRL1_RESET_VAL,               "PLB6PLB4_1_ADDRL1"
    check_value CVR3,   P641(ADDRH1),   ADDRH1_RESET_VAL,               "PLB6PLB4_1_ADDRH1"
    rumboot_putstring   "done.\n"

    /*       block number | DCR-addr      | mask      | fill val  | begin val | end val   | reg name |  */
/*  check_running_10    0,  P640(ADDRL1),   0xFFFFFFFF, 0x00000001, 0x00000001, 0x80000000, "ADDRL1"    */
    check_running_10    1,  P641(ADDRL1),   0xFFFFFFFF, 0x00000001, 0x00000001, 0x80000000, "ADDRL1"

    /*           block number | base DCR-address   |    */
    check_running_10_tesr   0,  DCR_PLB6PLB4_0_BASE
    check_running_10_tesr   1,  DCR_PLB6PLB4_1_BASE


    b           test_end


