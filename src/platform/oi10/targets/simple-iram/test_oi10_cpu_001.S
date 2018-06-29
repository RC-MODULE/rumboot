
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/devices.h>

#define MIRROR_MEM	0xFFF00000

#define	ADDR26MASK	0x03FFFFFC
#define LK			0x00000001
#define AA			0x00000002
#define B_OP		0x48000000

/* Branch opcode synthesis */
.macro ppc_branch	bAddr, bFlg=0
	.long	B_OP + \bAddr - IM0_BASE + (MIRROR_MEM & ADDR26MASK) + ((\bFlg) & 3)
.endm

.global main

.section ".text","ax",@progbits

main:
	mflr		r3				/* Save */
	mtspr		SPR_SPRG8,	r3	/*  LR  */
	b			setup_tlb

tlb_entries:
 /*			MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,              V   )*/
    .long	MMU_TLB_ENTRY(  0x010,  0x80000,    0xFFF00,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND, 0b1 )
    .long	MMU_TLB_ENTRY(  0x010,  0x80010,    0xFFF10,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND, 0b1 )
	.set TLB_ENTRIES_N, (.-tlb_entries)/MMU_TLB_ENTRY_SIZE

setup_tlb:
    load_addr   r3, tlb_entries
    load_const  r4, TLB_ENTRIES_N
    bl          ppc470s_write_tlb_entries

check_cpu_ppc_prog_model_24:    /*check sequential flow*/
	rumboot_putstring "Start test...\n"
    load_const	r3,	0x00000000
    mtxer		r3
    test_event	EVENT_CHECK_CPU_PPC_PROG_MODEL_24_SEQ

check_cpu_ppc_prog_model_25:    /*check unconditional branch*/
    rumboot_putstring	"Unconditional branch:\n align: 3 "
    b			bu_b3
    test_event	EVENT_ERROR
    b 			test_error

.align 3
bu_b3:
    putchar		'4'
    putchar		' '
    b			bu_b4
    b			test_error

.align 4
bu_b4:
    putchar		'5'
    putchar		' '
    b			bu_b5
    b			test_error

.align 5
bu_b5:
    putchar		'6'
    putchar		' '
    b			bu_b6
    b			test_error

.align 6
bu_b6:
    b			bu_bl
    b			test_error


bu_bl:
	rumboot_putstring "\ntest bl/ba ... "
	load_const	r3,		0x0001
    bl			bu_blr
    cmpwi cr5,	r3,		0x0000
    beq+  cr5,	bu_bla
    b			test_error

bu_end:
	rumboot_putstring "ok!\n"
    b			check_cpu_ppc_prog_model_26

bu_ba:
	ppc_branch	bu_end,	AA
	b			test_error


bu_bla:
	load_const	r3,		0x0001
    ppc_branch	bu_blr,	AA|LK
    cmpwi cr6,	r3,		0x0000
    beq+  cr6,	bu_ba
    b			test_error

bu_blr:
	xor			r3,	r3,	r3 /* Clear r3 */
    blr
    b			test_error


check_cpu_ppc_prog_model_26:    /*check conditional branch*/
	rumboot_putstring "\ntest conditional branch\n -> "

    load_const	r4,	0x00000000
    load_const	r5,	0x00000001

    cmpw cr0,	r4, r5
    blt			bc_ble
    b			test_error

bc_ble:
	putchar		'l'
	putchar		'e'
	putchar		' '
    ble			bc_beq
    b			test_error

bc_beq:
	putchar		'e'
	putchar		'q'
	putchar		' '
    cmpw cr0,	r5,	r5
    beq			bc_bgt
    b			test_error

bc_bgt:
	putchar		'g'
	putchar		't'
	putchar		' '
    cmpw cr0,	r5, r4
    bgt			bc_bge
    b			test_error

bc_bge:
	putchar		'g'
	putchar		'e'
	putchar		' '
    bge			bc_bnl
    b			test_error

bc_bnl:
	putchar		'n'
	putchar		'l'
	putchar		' '
    cmp cr0,	r5,	r4
    bnl			bc_bne
    b			test_error

bc_bne:
	putchar		'n'
	putchar		'e'
	putchar		' '
    bne			bc_bng
    b			test_error

bc_bng:
	putchar		'n'
	putchar		'g'
	putchar		' '
    cmp cr0,	r4,	r5
    bng			bc_bso
    b			test_error

bc_bso:
	putchar		's'
	putchar		'o'
	putchar		' '
    load_const	r6,	0x7FFFFFFF
    load_const	r7,	0x00000002
	/*operands considered as signed, see PowerISA, 3.3.7*/
    addco.		r8,	r6,	r7
    bso bc_bns
    b test_error

bc_bns:
	putchar		'n'
	putchar		's'
	putchar		' '
    load_const	r6,	0xFFFFFFFF
    load_const	r7,	0x00000000
    /*SO bit of XER is not cleared automatically*/
    mtxer		r7
    addco.		r8,	r6,	r7
    bns			check_cpu_ppc_prog_model_27
    b			test_error


check_cpu_ppc_prog_model_27:    /*check XL-branch*/
	rumboot_putstring "\ntest conditional branch to lr (XL-branch)\n -> "
	load_addr	r6,	bic_blelr
    mtlr		r6
    cmpw  cr0,	r4, r5
    bltlr cr0
    b			test_error

bic_blelr:
	putchar		'l'
	putchar		'e'
	putchar		' '
	load_addr	r6,	bic_beqlr
    mtlr		r6
    blelr cr0
    b			test_error

bic_beqlr:
	putchar		'e'
	putchar		'q'
	putchar		' '
	load_addr	r6,	bic_bgtlr
    mtlr		r6
    load_const	r6,	0x00000001
    cmpw  cr0,	r6,	r5
    beqlr cr0
    b			test_error

bic_bgtlr:
	putchar 'g'
	putchar 't'
	putchar ' '
	load_addr	r6,	bic_bgelr
    mtlr		r6
    cmpw  cr0,	r5,	r1
    bgtlr cr0
    b			test_error

bic_bgelr:
	putchar		'g'
	putchar		'e'
	putchar		' '
	load_addr	r6,	bic_bnllr
    mtlr		r6
    bgelr cr0
    b			test_error

bic_bnllr:
	putchar		'n'
	putchar		'l'
	putchar		' '
	load_addr	r6,	bic_bnelr
    mtlr		r6
    cmpw  cr0,	r5,	r4
    bnllr cr0
    b			test_error

bic_bnelr:
	putchar		'n'
	putchar		'e'
	putchar		' '
	load_addr	r6,	bic_bnglr
    mtlr		r6
    bnelr cr0
    b			test_error

bic_bnglr:
	putchar		'n'
	putchar		'g'
	putchar		' '
	load_addr	r6,	bic_bsolr
    mtlr		r6
    cmpw  cr0,	r4,	r5
    bnglr cr0
    b			test_error

bic_bsolr:
	putchar		's'
	putchar		'o'
	putchar		' '
	load_addr	r6,	bic_bnslr
    mtlr		r6
    load_const	r6,	0xFFFFFFFF
    load_const	r7,	0x80000000
    addco.		r8,	r6,	r7
    bsolr cr0
    b			test_error

bic_bnslr:
	putchar		'n'
	putchar		's'
	putchar		' '
	load_addr	r6,	bic_bdz
    mtlr		r6
    load_const	r6,	0xFFFFFFFF
    mtxer		r4
    addco.		r8,	r5,	r4
    bnslr cr0
    b			test_error

bic_bdz:
bic_bdz:
    rumboot_putstring "\ntest bdz/bdnz... "
    load_const	r6,	0x0003
    mtctr		r6
bic_bdz_loop:
	addi		r6,	r6,	-1
    bdz			bic_bdz_stop
    b			bic_bdz_loop
bic_bdz_stop:
	cmpwi cr5,	r6,	0x0000
	beq+  cr5,	bic_bdnz
	b			test_error

bic_bdnz:
    load_const	r6,	0x0003
    mtctr		r6
bic_bdnz_loop:
	addi		r6,	r6,	-1
	bdnz		bic_bdnz_loop
	beq+  cr6,	bic_bltctr
	b			test_error

bic_bltctr:
	rumboot_putstring "\ntest cond branch to ctr -> lt "
	load_addr	r6,	bic_blectr
    mtctr		r6
    cmpw   cr0,	r4,	r5
    bltctr cr0
    b			test_error

bic_blectr:
	putchar		'l'
	putchar		'e'
	putchar		' '
	load_addr	r6,	bic_beqctr
    mtctr		r6
    cmpw   cr0,	r4,	r5
    blectr cr0
    b			test_error

bic_beqctr:
	putchar		'e'
	putchar		'q'
	putchar		' '
	load_addr	r6,	bic_bgtctr
    mtctr		r6
    load_const	r5,	0x00000001
    cmpw   cr0,	r5,	r5
    beqctr cr0
    b			test_error

bic_bgtctr:
	putchar		'g'
	putchar		't'
	putchar		' '
	load_addr	r6,	bic_bgectr
    mtctr		r6
    cmpw   cr0,	r5,	r4
    bgtctr cr0
    b			test_error

bic_bgectr:
	putchar		'g'
	putchar		'e'
	putchar		' '
	load_addr	r6,	bic_bnlctr
    mtctr		r6
    cmpw   cr0,	r5,	r4
    bgectr cr0
    b			test_error

bic_bnlctr:
	putchar		'n'
	putchar		'l'
	putchar		' '
	load_addr	r6,	bic_bnectr
    mtctr		r6
    cmpw   cr0,	r5,	r4
    bnlctr cr0
    b			test_error

bic_bnectr:
	putchar		'n'
	putchar		'e'
	putchar		' '
	load_addr	r6,	bic_bngctr
    mtctr		r6
    cmpw   cr0,	r5,	r4
    bnectr cr0
    b			test_error

bic_bngctr:
	putchar		'n'
	putchar		'g'
	putchar		' '
	load_addr	r6,	bic_bsoctr
    mtctr		r6
    cmpw   cr0,	r4,	r5
    bngctr cr0
    b			test_error

bic_bsoctr:
	putchar		's'
	putchar		'o'
	putchar		' '
	load_addr	r6,	bic_bnsctr
    mtctr		r6
    load_const	r6,	0x7FFFFFFF
    load_const	r7,	0x00000002
    addco.		r8,	r6,	r7
    bsoctr cr0
    b			test_error

bic_bnsctr:
	putchar		'n'
	putchar		's'
	putchar		' '
	load_addr	r6,	test_ok
    mtctr		r6
    load_const	r6,	0xFFFFFFFF
    mtxer		r4
    addco.		r8,	r6,	r4
    bnsctr cr0
    b			test_error

test_error:
	rumboot_putstring "\nTEST ERROR\n"
	test_event		EVENT_ERROR
	load_const	r3,	RESULT_ERROR
	b finish

test_ok:
	rumboot_putstring "\nTEST OK\n"
	test_event		EVENT_OK
	load_const	r3,	RESULT_OK

finish:
	mfspr		r4,	SPR_SPRG8	/* Restore */
	mtlr		r4				/*   LR    */
	blr

