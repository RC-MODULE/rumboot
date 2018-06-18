
#include <platform/test_event_asm.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/trace.S.h>

#define TMP_REG		29
#define INF_REG		30
#define REGS_AS 	7,6
#define REGS_ASB	8,6,7

#define UI_VAL		0x5A5A
#define RS_VAL		0xAA55AA55
#define RB_VAL		0x5A5A5A5A

#define ppc_nop xori 0,0,0

.macro subcall callLabel
	mflr	TMP_REG
	mtspr	SPR_SPRG8,	TMP_REG
	bl		\callLabel
	mfspr	TMP_REG,	SPR_SPRG8
	mtlr	TMP_REG
.endm

/* check macro for operations with mask "opcode RA, RS, UI" */
.macro LOGIC_RA_RS_UI_CHECK	opcode,     \
							ra_reg,     \
							rs_reg,     \
							rs_val,     \
							ui_val,     \
							cmd_cnv,    \
							r_res_addr

	load_const	INF_REG,	0x00000000	/* clear info register */

	/* put mask type */
	ori			INF_REG,	INF_REG, 0x01
	load_const	TMP_REG,	0x04
	slw			INF_REG,	INF_REG, TMP_REG

	/* put self-invented comand code into info reg */
	ori			INF_REG,	INF_REG, \cmd_cnv
	load_const	TMP_REG,	0x05
	slw			INF_REG,	INF_REG, TMP_REG

	/* put RA reg number into info reg */
	ori			INF_REG,	INF_REG, \ra_no
	load_const	TMP_REG,	0x05
	slw			INF_REG,	INF_REG, TMP_REG

	/* put RS reg number into info reg */
	ori			INF_REG,	INF_REG, \rs_no
	load_const	TMP_REG,	0x10
	slw			INF_REG,	INF_REG, TMP_REG

	/* put UI const into info reg */
	ori			INF_REG,	INF_REG, \ui_val

	/* load values into reg with number rs_no */
    load_const	\rs_no,		\rs_val

	/* perform operation */
    \opcode		\ra_no,		\rs_no, \ui_val
    stwu		\ra_no,		4(\r_res_addr)
    /* raise event for verilog */
    test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_18

	/* no-op for syncronization purpose */
    ppc_nop
.endm


/* check macro for operations with mask "opcode RA, RS, RB" */
.macro LOGIC_RA_RS_RB_CHECK	opcode,    \
							ra_no,     \
							rs_no,     \
							rb_no,     \
							rs_val,    \
							rb_val,    \
							cmd_cnv,   \
							r_res_addr
	/* clear info register */
	load_const	INF_REG,	0x00000000

	/* put mask tyoe */
	ori			INF_REG,	INF_REG,	0x02
	load_const	TMP_REG,	0x6
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			INF_REG,	INF_REG,	\cmd_cnv
	load_const	TMP_REG,	0x8
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put RA reg number into info reg */
	ori			INF_REG,	INF_REG,	\ra_no
	load_const	TMP_REG,	0x8
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put RS reg number into info reg */
	ori			INF_REG,	INF_REG,	\rs_no
	load_const	TMP_REG,	0x8
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put RB reg number into info reg */
	ori			INF_REG,	INF_REG,	\rb_no

    /* load values into reg with number rs_no */
    load_const	\rs_no,		\rs_val
    /* load values into reg with number rb_no */
    load_const	\rb_no,		\rb_val

    /* perform operation */
    \opcode		\ra_no,		\rs_no,	\rb_no
    stwu		\ra_no,		4(\r_res_addr)
    /* raise event for verilog */
    test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_18
    /* no-op for syncronization purpose */
    ppc_nop
.endm


/* check macro for operations with mask "opcode RA, RS" */
.macro LOGIC_RA_RS_CHECK	opcode,    \
							ra_no,     \
							rs_no,     \
							rs_val,    \
							cmd_cnv,   \
							r_res_addr
	/* clear info register */
	load_const	INF_REG,	0x00000000

	/* put mask tyoe */
	ori			INF_REG,	INF_REG,	0x03
	load_const	TMP_REG,	0x06
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			INF_REG,	INF_REG,	\cmd_cnv
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put RA reg number into info reg */
	ori			INF_REG,	INF_REG,	\ra_no
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put RS reg number into info reg */
	ori			INF_REG,	INF_REG,	\rs_no
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

    /* load values into reg with number rs_no */
    load_const	\rs_no,		\rs_val

    /* perform operation */
    \opcode		\ra_no,		\rs_no
    stwu		\ra_no,		4(\r_res_addr)
    /* raise event for verilog */
    test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_18
    /* no-op for syncronization purpose */
    ppc_nop
.endm

.macro sw_comparator_macro	reg_first_start,     \
							reg_second_start,    \
							reg_byte_count,      \
							reg_tmp_offset,      \
							reg_tmp_data_first,  \
							reg_tmp_data_second, \
							reg_result
    load_const	\reg_tmp_offset,	0x00000000
    load_const	\reg_result,		0x00000001
    li			\reg_tmp_offset,	0x00000004
5: /* labels 1-3 used in trace_msg, label 4 used in test_assert */
	lwzx		\reg_tmp_data_first,	\reg_first_start,	\reg_tmp_offset
	lwzx		\reg_tmp_data_second,	\reg_second_start,	\reg_tmp_offset
	cmplw cr2,	\reg_tmp_data_first,	\reg_tmp_data_second
	beq cr2,	6f
	load_const	\reg_result,		0x00000000

	rumboot_putstring "data mismatch at  offset: 0x"
	rumboot_puthex \reg_tmp_offset
	rumboot_putstring "data in the first  array: 0x"
	rumboot_puthex \reg_tmp_data_first
	rumboot_putstring "data in the second array: 0x"
	rumboot_puthex \reg_tmp_data_second
6:
	addi		\reg_tmp_offset, \reg_tmp_offset, 4
	cmpw cr3,	\reg_byte_count, \reg_tmp_offset
	bge  cr3,	5b
.endm

.global main

.section ".text","ax",@progbits
main:

/*  load_const r3,	0x0004FFFC */
    load_addr	r3, result_data
    addi		r3,	r3,-4
    load_const	r4, 0x0004FFFC
    load_const	r5, 0x00000000
.rept 128
    stwu		r5, 4(r4)
.endr
    load_const	r4,	0x0004FFFC
.rept 128
    lwz			r5,	4(r4)
.endr

/*	LOGIC_RA_RS_UI_CHECK opcode,  (ra,rs),    rs_val, ui_val, cmd_cnv, addr_reg */
    LOGIC_RA_RS_UI_CHECK andi.,   REGS_AS,    RS_VAL, UI_VAL, 0x00,    3
    LOGIC_RA_RS_UI_CHECK andis.,  REGS_AS,    RS_VAL, UI_VAL, 0x01,    3
    LOGIC_RA_RS_UI_CHECK ori,     REGS_AS,    RS_VAL, UI_VAL, 0x02,    3
    LOGIC_RA_RS_UI_CHECK oris,    REGS_AS,    RS_VAL, UI_VAL, 0x03,    3
    LOGIC_RA_RS_UI_CHECK xori,    REGS_AS,    RS_VAL, UI_VAL, 0x04,    3
    LOGIC_RA_RS_UI_CHECK xoris,   REGS_AS,    RS_VAL, UI_VAL, 0x05,    3

/*	LOGIC_RA_RS_RB_CHECK opcode,  (ra,rs,rb), rs_val, rb_val, cmd_cnv, addr_reg */
    LOGIC_RA_RS_RB_CHECK and,     REGS_ASB,   RS_VAL, RB_VAL, 0x00,    3
    LOGIC_RA_RS_RB_CHECK and.,    REGS_ASB,   RS_VAL, RB_VAL, 0x01,    3
    LOGIC_RA_RS_RB_CHECK or,      REGS_ASB,   RS_VAL, RB_VAL, 0x02,    3
    LOGIC_RA_RS_RB_CHECK or.,     REGS_ASB,   RS_VAL, RB_VAL, 0x03,    3
    LOGIC_RA_RS_RB_CHECK xor,     REGS_ASB,   RS_VAL, RB_VAL, 0x04,    3
    LOGIC_RA_RS_RB_CHECK xor.,    REGS_ASB,   RS_VAL, RB_VAL, 0x05,    3
    LOGIC_RA_RS_RB_CHECK nand,    REGS_ASB,   RS_VAL, RB_VAL, 0x06,    3
    LOGIC_RA_RS_RB_CHECK nand.,   REGS_ASB,   RS_VAL, RB_VAL, 0x07,    3
    LOGIC_RA_RS_RB_CHECK nor,     REGS_ASB,   RS_VAL, RB_VAL, 0x08,    3
    LOGIC_RA_RS_RB_CHECK nor.,    REGS_ASB,   RS_VAL, RB_VAL, 0x09,    3
    LOGIC_RA_RS_RB_CHECK eqv,     REGS_ASB,   RS_VAL, RB_VAL, 0x0A,    3
    LOGIC_RA_RS_RB_CHECK eqv.,    REGS_ASB,   RS_VAL, RB_VAL, 0x0B,    3
    LOGIC_RA_RS_RB_CHECK andc,    REGS_ASB,   RS_VAL, RB_VAL, 0x0C,    3
    LOGIC_RA_RS_RB_CHECK andc.,   REGS_ASB,   RS_VAL, RB_VAL, 0x0D,    3
    LOGIC_RA_RS_RB_CHECK orc,     REGS_ASB,   RS_VAL, RB_VAL, 0x0E,    3
    LOGIC_RA_RS_RB_CHECK orc.,    REGS_ASB,   RS_VAL, RB_VAL, 0x0F,    3
    LOGIC_RA_RS_RB_CHECK cmpb,    REGS_ASB,   RS_VAL, RB_VAL, 0x10,    3

/*	LOGIC_RA_RS_CHECK    opcode,  (ra,rs),    rs_val,         cmd_cnv, addr_reg */
    LOGIC_RA_RS_CHECK    extsb,   REGS_AS,    RS_VAL,         0x00,    3
    LOGIC_RA_RS_CHECK    extsb.,  REGS_AS,    RS_VAL,         0x01,    3
    LOGIC_RA_RS_CHECK    extsh,   REGS_AS,    RS_VAL,         0x02,    3
    LOGIC_RA_RS_CHECK    extsh.,  REGS_AS,    RS_VAL,         0x03,    3
    LOGIC_RA_RS_CHECK    cntlzw,  REGS_AS,    RS_VAL,         0x04,    3
    LOGIC_RA_RS_CHECK    cntlzw., REGS_AS,    RS_VAL,         0x05,    3
    LOGIC_RA_RS_CHECK    prtyw,   REGS_AS,    RS_VAL,         0x06,    3

    load_addr	r3, result_data
    load_addr 	r4, iss_mem
    load_const	r5, 30
	subcall		compare_data
	cmpwi cr0,	r3, 0x1

/*
	.macro sw_comparator_macro	reg_first_start,     \
								reg_second_start,    \
								reg_byte_count,      \
								reg_tmp_offset,      \
								reg_tmp_data_first,  \
								reg_tmp_data_second, \
								reg_result
*/
    sw_comparator_macro r3, r4, r5, r6, r7, r8, r9
    cmpwi cr0, r9, 0x1

    beq cr0,	test_ok
    b			test_error


.align 2
iss_mem:
	.long 0x00000A50
	.long 0x0A500000
	.long 0xAA55FA5F
	.long 0xFA5FAA55
	.long 0xAA55F00F
	.long 0xF00FAA55
	.long 0x0A500A50
	.long 0x0A500A50
	.long 0xFA5FFA5F
	.long 0xFA5FFA5F
	.long 0xF00FF00F
	.long 0xF00FF00F
	.long 0xF5AFF5AF
	.long 0xF5AFF5AF
	.long 0x05A005A0
	.long 0x05A005A0
	.long 0x0FF00FF0
	.long 0x0FF00FF0
	.long 0xA005A005
	.long 0xA005A005
	.long 0xAFF5AFF5
	.long 0xAFF5AFF5
	.long 0x00000000
	.long 0x00000055
	.long 0x00000055
	.long 0xFFFFAA55
	.long 0xFFFFAA55
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000


	/* AG: */
compare_data:
	cmpwi cr0,	r5,  0
	li 			r8,  1
	beq cr0,	.compare_data_L1
	li 			r10, 0
.compare_data_L4:
	lwzx		r9,  r3,  r10
	lwzx 		r7,  r4,  r10
	addi		r10, r10, 4
	cmplw cr7,	r5,  r10
	xor			r9,  r9,  r7
	addic		r9,  r9,  -1
	subfe		r9,  r9,  r9
	and			r8,  r8,  r9
	bgt cr7,	.compare_data_L4
.compare_data_L1:
	mr			r3,  r8
	blr

test_ok:
	rumboot_putstring "TEST OK\n"
	test_event		EVENT_OK
	load_const r3,	RESULT_OK
	b finish

test_error:
    rumboot_putstring "TEST ERROR\n"
    test_event		EVENT_ERROR
    load_const r3,	RESULT_ERROR

finish:
	blr

.section ".issdata","aw",@progbits

result_data:
.rept 512
    .long 0x00000000
.endr

