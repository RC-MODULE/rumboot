
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/devices.h>

#define MIRROR_MEM	0xFFF00000

/* Registers aliases */
#define R_ARG0                  r3
#define R_ARG1                  r4
#define R_RETV                  R_ARG0
#define R_ADDR                  r20     /* Unified address       */
#define R_DATA                  r21     /* Unified data          */
#define R_INCR                  r22     /* Increment value       */
#define R_MASK                  r25     /* Unified mask          */
#define R_EXPV                  r26     /* Expected value        */
#define R_TEMP                  r27     /* Temp value            */
#define R_BACK                  r28     /* Common backup         */
#define R_BKLR                  r29     /* Holds LR copy         */
#define R_ZERO                  r30     /* Holds 0x00000000      */
#define R_ONES                  r31     /* Holds 0xFFFFFFFF      */


#define	ADDR26MASK  0x03FFFFFC
#define LK          0x00000001
#define AA          0x00000002
#define B_OP        0x48000000

/* Branch opcode synthesis */
.macro ppc_branch   bAddr, bFlg=0
    .long   B_OP + \bAddr - IM0_BASE + (MIRROR_MEM & ADDR26MASK) + ((\bFlg) & 3)
.endm

.global main

.section ".text","ax",@progbits

main:
    load_const  R_ZERO, 0x00000000
    load_const  R_ONES, 0xFFFFFFFF
    load_const  R_INCR  0x00000001
    mflr        R_BKLR
    b           setup_tlb

tlb_entries:
 /*			MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,              V   )*/
    .long	MMU_TLB_ENTRY(  0x010,  0x80000,    0xFFF00,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND, 0b1 )
    .long	MMU_TLB_ENTRY(  0x010,  0x80010,    0xFFF10,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND, 0b1 )
    .set TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE

setup_tlb:
    load_addr   R_ARG0, tlb_entries
    load_const  R_ARG1, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries

check_cpu_ppc_prog_model_24:    /*check sequential flow*/
    rumboot_putstring "Start test...\n"
    mtxer       R_ZERO
    test_event  EVENT_CHECK_CPU_PPC_PROG_MODEL_24_SEQ

check_cpu_ppc_prog_model_25:    /*check unconditional branch*/
    rumboot_putstring	"Unconditional branch:\n align: "
    rumboot_putstring   "3"
    b           bu_b3
    b           test_error

.align 3
bu_b3:
    rumboot_putstring   "4"
    b           bu_b4
    b           test_error

.align 4
bu_b4:
    rumboot_putstring   "5"
    b           bu_b5
    b           test_error

.align 5
bu_b5:
    rumboot_putstring   "6"
    b           bu_b6
    b           test_error

.align 6
bu_b6:
    b           bu_bl
    b           test_error


bu_bl:
    rumboot_putstring "test bl/ba ... "
    addi        R_DATA, R_ZERO, 0x0001
    bl          bu_blr
    cmpw   cr4, R_DATA, R_ZERO
    beq+   cr4, bu_bla
    b           test_error

bu_end:
    rumboot_putstring "bl/ba ok!\n"
    b           check_cpu_ppc_prog_model_26

bu_ba:
    ppc_branch  bu_end, AA
    b           test_error


bu_bla:
    addi        R_DATA, R_ZERO, 0x0001
    ppc_branch  bu_blr,	AA|LK
    cmpw   cr5, R_DATA, R_ZERO
    beq+   cr5, bu_ba
    b           test_error

bu_blr:
    xor         R_DATA, R_DATA, R_DATA  /* Clear register */
    blr
    b           test_error


check_cpu_ppc_prog_model_26:    /*check conditional branch*/
    rumboot_putstring "test conditional branch:\n"

    addi        R_DATA, R_ZERO, 0x0001

    cmpw   cr0, R_ZERO, R_DATA
    blt         bc_ble
    b           test_error

bc_ble:
    putchar     'l'
    putchar     'e'
    putchar     ' '
    ble+   cr0, bc_beq
    b			test_error

bc_beq:
    putchar     'e'
    putchar     'q'
    putchar     ' '
    cmpw   cr0, R_DATA, R_DATA
    beq+   cr0, bc_bgt
    b           test_error

bc_bgt:
    putchar     'g'
    putchar     't'
    putchar     ' '
    cmpw   cr0, R_DATA, R_ZERO
    bgt+   cr0, bc_bge
    b			test_error

bc_bge:
    putchar		'g'
    putchar		'e'
    putchar		' '
    bge+   cr0, bc_bnl
    b           test_error

bc_bnl:
    putchar     'n'
    putchar     'l'
    putchar     ' '
    cmpw   cr0, R_DATA, R_ZERO
    bnl+   cr0, bc_bne
    b           test_error

bc_bne:
    putchar		'n'
    putchar		'e'
    putchar		' '
    bne+   cr0, bc_bng
    b           test_error

bc_bng:
    putchar     'n'
    putchar     'g'
    putchar     ' '
    cmpw   cr0, R_ZERO, R_DATA
    bng+   cr0, bc_bso
    b           test_error

bc_bso:
    putchar     's'
    putchar     'o'
    putchar     ' '
    load_const  R_MASK, 0x7FFFFFFF
    load_const  R_TEMP, 0x00000002
    /* operands considered as signed, see PowerISA, 3.3.7 */
    addco.      R_DATA, R_MASK, R_TEMP
    bso+   cr0, bc_bns
    b           test_error

bc_bns:
    putchar     'n'
    putchar     's'
    putchar     ' '
    /* SO bit of XER is not cleared automatically */
    mtxer       R_ZERO
    addco.      R_DATA, R_ONES, R_ZERO
    bns+   cr0, check_cpu_ppc_prog_model_27
    b           test_error

check_cpu_ppc_prog_model_27:    /*check XL-branch*/
    rumboot_putstring "test conditional branch to lr (XL-branch)\n -> "
    load_addr   R_ADDR, bic_blelr
    mtlr        R_ADDR
    cmpw   cr0, R_ZERO, R_ADDR
    bltlr+ cr0
    b           test_error

bic_blelr:
    putchar     'l'
    putchar     'e'
    putchar     ' '
    load_addr   R_ADDR, bic_beqlr
    mtlr        R_ADDR
    blelr+ cr0
    b           test_error

bic_beqlr:
    putchar     'e'
    putchar     'q'
    putchar     ' '
    load_addr   R_ADDR, bic_bgtlr
    mtlr        R_ADDR
    load_const  R_TEMP, 0x00000001
    cmpw   cr0, R_INCR, R_TEMP
    beqlr+ cr0
    b           test_error

bic_bgtlr:
    putchar     'g'
    putchar     't'
    putchar     ' '
    load_addr   R_ADDR, bic_bgelr
    mtlr        R_ADDR
    cmpw  cr0,  R_INCR, R_ZERO
    bgtlr cr0
    b           test_error

bic_bgelr:
    putchar     'g'
    putchar     'e'
    putchar     ' '
    load_addr   R_ADDR, bic_bnllr
    mtlr        R_ADDR
    bgelr+ cr0
    b           test_error

bic_bnllr:
    putchar     'n'
    putchar     'l'
    putchar     ' '
    load_addr   R_ADDR, bic_bnelr
    mtlr        R_ADDR
    cmpw   cr0, R_ONES, R_ZERO
    bnllr+ cr0
    b           test_error

bic_bnelr:
    putchar     'n'
    putchar     'e'
    putchar     ' '
    load_addr   R_ADDR, bic_bnglr
    mtlr        R_ADDR
    bnelr+ cr0
    b           test_error

bic_bnglr:
    putchar     'n'
    putchar     'g'
    putchar     ' '
    load_addr   R_ADDR, bic_bsolr
    mtlr        R_ADDR
    cmpw   cr0, R_ZERO, R_ONES
    bnglr+ cr0
    b           test_error

bic_bsolr:
    putchar     's'
    putchar     'o'
    putchar     ' '
    load_addr   R_ADDR, bic_bnslr
    mtlr        R_ADDR
    load_const  R_TEMP, 0x80000000
    addco.      R_DATA, R_ONES, R_TEMP
    bsolr+ cr0
    b           test_error

bic_bnslr:
    putchar     'n'
    putchar     's'
    putchar     ' '
    load_addr   R_ADDR, bic_bdz
    mtlr        R_ADDR
    mtxer       R_ZERO
    addco.      R_DATA, R_ONES, R_ZERO
    bnslr+ cr0
    b           test_error

bic_bdz:
bic_bdz:
    rumboot_putstring "test bdz/bdnz...\n"
    load_const  R_DATA, 0x0003
    mtctr       R_DATA
bic_bdz_loop:
    addi        R_DATA, R_DATA, -1
    bdz         bic_bdz_stop
    b           bic_bdz_loop
bic_bdz_stop:
    cmpw   cr3, R_DATA, R_ZERO
    beq+   cr3, bic_bdnz
    b			test_error

bic_bdnz:
    load_const  R_DATA, 0x0003
    mtctr       R_DATA
bic_bdnz_loop:
    addi        R_DATA, R_DATA, -1
    bdnz        bic_bdnz_loop
    cmpw   cr4, R_DATA, R_ZERO
    beq+   cr4, bic_bltctr
    b           test_error

bic_bltctr:
    rumboot_putstring "test cond branch to ctr:\n"
    putchar     'l'
    putchar     't'
    putchar     ' '
    load_addr   R_ADDR, bic_blectr
    mtctr       R_ADDR
    cmpw   cr0, R_ZERO, R_INCR
    bltctr+     cr0
    b           test_error

bic_blectr:
    putchar     'l'
    putchar     'e'
    putchar     ' '
    load_addr   R_ADDR, bic_beqctr
    mtctr       R_ADDR
    cmpw   cr0, R_ZERO, R_INCR
    blectr+     cr0
    b           test_error

bic_beqctr:
    putchar     'e'
    putchar     'q'
    putchar     ' '
    load_addr   R_ADDR, bic_bgtctr
    mtctr       R_ADDR
    cmpw   cr0, R_INCR, R_INCR
    beqctr+     cr0
    b           test_error

bic_bgtctr:
    putchar     'g'
    putchar     't'
    putchar     ' '
    load_addr   R_ADDR, bic_bgectr
    mtctr       R_ADDR
    cmpw   cr0, R_INCR, R_ZERO
    bgtctr+     cr0
    b           test_error

bic_bgectr:
    putchar     'g'
    putchar     'e'
    putchar     ' '
    load_addr   R_ADDR, bic_bnlctr
    mtctr       R_ADDR
    cmpw   cr0, R_INCR, R_ZERO
    bgectr+     cr0
    b           test_error

bic_bnlctr:
    putchar     'n'
    putchar     'l'
    putchar     ' '
    load_addr   R_ADDR, bic_bnectr
    mtctr       R_ADDR
    cmpw   cr0, R_INCR, R_ZERO
    bnlctr+     cr0
    b           test_error

bic_bnectr:
    putchar     'n'
    putchar     'e'
    putchar     ' '
    load_addr   R_ADDR, bic_bngctr
    mtctr       R_ADDR
    cmpw   cr0, R_ONES, R_ZERO
    bnectr+     cr0
    b           test_error

bic_bngctr:
    putchar     'n'
    putchar     'g'
    putchar     ' '
    load_addr   R_ADDR, bic_bsoctr
    mtctr       R_ADDR
    cmpw   cr0, R_ZERO, R_ONES
    bngctr+     cr0
    b           test_error

bic_bsoctr:
    putchar     's'
    putchar     'o'
    putchar     ' '
    load_addr   R_ADDR, bic_bnsctr
    mtctr       R_ADDR
    load_const  R_MASK, 0x7FFFFFFF
    load_const  R_TEMP, 0x00000002
    addco.      R_DATA, R_MASK, R_TEMP
    bsoctr+     cr0
    b           test_error

bic_bnsctr:
    putchar     'n'
    putchar     's'
    putchar     ' '
    load_addr   R_ADDR, test_ok
    mtctr       R_ADDR
    mtxer       R_ZERO
    addco.      R_DATA, R_ONES, R_ZERO
    bnsctr+     cr0
    b           test_error

test_error:
    rumboot_putstring "\nTEST ERROR\n"
    load_const  R_RETV, RESULT_ERROR
    b finish

test_ok:
    rumboot_putstring "\nTEST OK\n"
    load_const  R_RETV, RESULT_OK

finish:
    mtlr        R_BKLR
    blr         /* RETURN */

