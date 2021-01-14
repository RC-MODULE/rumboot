
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <arch/mmu.S.h>
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
    test_event_send_test_id "test_oi10_cpu_001"
    // rumboot_putstring "Start test...\n"
    mtxer       R_ZERO
    test_event  EVENT_CHECK_CPU_PPC_PROG_MODEL_24_SEQ
    nop
    nop
    nop
    nop

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
    rumboot_putstring "le\n"
    ble+   cr0, bc_beq
    b			test_error

bc_beq:
    rumboot_putstring "eq\n"
    cmpw   cr0, R_DATA, R_DATA
    beq+   cr0, bc_bgt
    b           test_error

bc_bgt:
    rumboot_putstring "gt\n"
    cmpw   cr0, R_DATA, R_ZERO
    bgt+   cr0, bc_bge
    b			test_error

bc_bge:
    rumboot_putstring "ge\n"
    bge+   cr0, bc_bnl
    b           test_error

bc_bnl:
    rumboot_putstring "nl\n"
    cmpw   cr0, R_DATA, R_ZERO
    bnl+   cr0, bc_bne
    b           test_error

bc_bne:
    rumboot_putstring "ne\n"
    bne+   cr0, bc_bng
    b           test_error

bc_bng:
    rumboot_putstring "ng\n"
    cmpw   cr0, R_ZERO, R_DATA
    bng+   cr0, bc_bso
    b           test_error

bc_bso:
    rumboot_putstring "so\n"
    load_const  R_MASK, 0x7FFFFFFF
    load_const  R_TEMP, 0x00000002
    /* operands considered as signed, see PowerISA, 3.3.7 */
    addco.      R_DATA, R_MASK, R_TEMP
    bso+   cr0, bc_bns
    b           test_error

bc_bns:
    rumboot_putstring "ns\n"
    /* SO bit of XER is not cleared automatically */
    mtxer       R_ZERO
    addco.      R_DATA, R_ONES, R_ZERO
    bns+   cr0, check_cpu_ppc_prog_model_27
    b           test_error

check_cpu_ppc_prog_model_27:    /*check XL-branch*/
    rumboot_putstring   "test conditional branch to lr (XL-branch):\n"
    rumboot_putstring   "lt\n"
    load_addr   R_ADDR, bic_blelr
    mtlr        R_ADDR
    cmpw   cr0, R_ZERO, R_INCR
    bltlr+ cr0
    b           test_error

bic_blelr:
    rumboot_putstring   "le\n"
    load_addr   R_ADDR, bic_beqlr
    mtlr        R_ADDR
    blelr+ cr0
    b           test_error

bic_beqlr:
    rumboot_putstring   "eq\n"
    load_addr   R_ADDR, bic_bgtlr
    mtlr        R_ADDR
    load_const  R_TEMP, 0x00000001
    cmpw   cr0, R_INCR, R_TEMP
    beqlr+ cr0
    b           test_error

bic_bgtlr:
    rumboot_putstring "gt\n"
    load_addr   R_ADDR, bic_bgelr
    mtlr        R_ADDR
    cmpw  cr0,  R_INCR, R_ZERO
    bgtlr cr0
    b           test_error

bic_bgelr:
    rumboot_putstring "ge\n"
    load_addr   R_ADDR, bic_bnllr
    mtlr        R_ADDR
    bgelr+ cr0
    b           test_error

bic_bnllr:
    rumboot_putstring "nl\n"
    load_addr   R_ADDR, bic_bnelr
    mtlr        R_ADDR
    cmpw   cr0, R_INCR, R_ZERO
    bnllr+ cr0
    b           test_error

bic_bnelr:
    rumboot_putstring "ne\n"
    load_addr   R_ADDR, bic_bnglr
    mtlr        R_ADDR
    bnelr+ cr0
    b           test_error

bic_bnglr:
    rumboot_putstring "ng\n"
    load_addr   R_ADDR, bic_bsolr
    mtlr        R_ADDR
    cmpw   cr0, R_ZERO, R_INCR
    bnglr+ cr0
    b           test_error

bic_bsolr:
    rumboot_putstring "so\n"
    load_addr   R_ADDR, bic_bnslr
    mtlr        R_ADDR
    load_const  R_TEMP, 0x80000000
    addco.      R_DATA, R_ONES, R_TEMP
    bsolr+ cr0
    b           test_error

bic_bnslr:
    rumboot_putstring "ns\n"
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
    rumboot_putstring "test conditional branch to ctr:\n"
    rumboot_putstring "lt\n"
    load_addr   R_ADDR, bic_blectr
    mtctr       R_ADDR
    cmpw   cr0, R_ZERO, R_INCR
    bltctr+     cr0
    b           test_error

bic_blectr:
    rumboot_putstring "le\n"
    load_addr   R_ADDR, bic_beqctr
    mtctr       R_ADDR
    cmpw   cr0, R_ZERO, R_INCR
    blectr+     cr0
    b           test_error

bic_beqctr:
    rumboot_putstring "eq\n"
    load_addr   R_ADDR, bic_bgtctr
    mtctr       R_ADDR
    cmpw   cr0, R_INCR, R_INCR
    beqctr+     cr0
    b           test_error

bic_bgtctr:
    rumboot_putstring "gt\n"
    load_addr   R_ADDR, bic_bgectr
    mtctr       R_ADDR
    cmpw   cr0, R_INCR, R_ZERO
    bgtctr+     cr0
    b           test_error

bic_bgectr:
    rumboot_putstring "ge\n"
    load_addr   R_ADDR, bic_bnlctr
    mtctr       R_ADDR
    cmpw   cr0, R_INCR, R_ZERO
    bgectr+     cr0
    b           test_error

bic_bnlctr:
    rumboot_putstring "nl\n"
    load_addr   R_ADDR, bic_bnectr
    mtctr       R_ADDR
    cmpw   cr0, R_INCR, R_ZERO
    bnlctr+     cr0
    b           test_error

bic_bnectr:
    rumboot_putstring "ne\n"
    load_addr   R_ADDR, bic_bngctr
    mtctr       R_ADDR
    cmpw   cr0, R_ONES, R_ZERO
    bnectr+     cr0
    b           test_error

bic_bngctr:
    rumboot_putstring "ng\n"
    load_addr   R_ADDR, bic_bsoctr
    mtctr       R_ADDR
    cmpw   cr0, R_ZERO, R_INCR
    bngctr+     cr0
    b           test_error

bic_bsoctr:
    rumboot_putstring "so\n"
    load_addr   R_ADDR, bic_bnsctr
    mtctr       R_ADDR
    load_const  R_MASK, 0x7FFFFFFF
    load_const  R_TEMP, 0x00000002
    addco.      R_DATA, R_MASK, R_TEMP
    bsoctr+     cr0
    b           test_error

bic_bnsctr:
    rumboot_putstring "ns\n"
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

