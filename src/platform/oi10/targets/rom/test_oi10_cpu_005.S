#include <platform/test_assert.S.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>
#include <platform/devices.h>
#include <platform/regs/regs_mpic128.h>

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

.macro sw_comparator_macro	reg_start1,     \
							reg_start2,     \
							reg_byte_count, \
							reg_tmp_offset, \
							reg_tmp_data1,  \
							reg_tmp_data2,  \
							reg_result
	load_const	\reg_tmp_offset,	0x00000000
    load_const	\reg_result,		0x00000001
    load_const	\reg_tmp_offset,	0x00000004
15: /* labels 1-3 used in rumboot_putstring, label 4 used in test_assert */
	lwzx		\reg_tmp_data1, 	\reg_start1,		\reg_tmp_offset
	lwzx		\reg_tmp_data2,		\reg_start2,		\reg_tmp_offset
	cmplw cr2,	\reg_tmp_data1,		\reg_tmp_data2
	beq   cr2, 	16f
	load_const	\reg_result,		0x00000000
/*
	rumboot_putstring "data mismatch at  offset: 0x"
	trace_hex \reg_tmp_offset
	rumboot_putstring "data in the first  array: 0x"
	trace_hex \reg_tmp_data_first
	rumboot_putstring "data in the second array: 0x"
	trace_hex \reg_tmp_data_second
*/
16:
    addi		\reg_tmp_offset,	\reg_tmp_offset,	0x00000004
    cmpw cr3,	\reg_byte_count,	\reg_tmp_offset
    bge  cr3,	15b
.endm


/* check macro for operations with mask "opcode RA, RS, UI" */
.macro LOGIC_RA_RS_UI_CHECK	opcode,   \
							ra_no,    \
							rs_no,    \
							rs_val,   \
							ui_val,   \
							cmd_cnv,  \
							res_addr
	load_const	\info_reg,	0x00000000 /* clear info register */
	load_const	\ra_no,		0x00000000 /* clear result register */

	ori			\info_reg,	\info_reg,	0x01 /* put mask tyoe */
	load_const	TMP_REG,	0x04
	slw			\info_reg,	\info_reg,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			\info_reg,	\info_reg,	\cmd_cnv
	load_const	TMP_REG,	0x05
	slw			\info_reg,	\info_reg,	TMP_REG

 	/* put RA reg number into info reg */
 	ori			\info_reg,	\info_reg,	\ra_no
	load_const	TMP_REG,	0x05
	slw			\info_reg,	\info_reg,	TMP_REG

	/* put RS reg number into info reg */
	ori			\info_reg,	\info_reg,	\rs_no
	load_const	TMP_REG,	0x10
	slw			\info_reg,	\info_reg,	TMP_REG

	/* put UI const into info reg */
	ori			\info_reg,	\info_reg,	\ui_val@l

	load_const	TMP_REG,	0x00000000
	mtxer		TMP_REG

	 /* load values into reg with number rs_no */
	load_const	\rs_no,		\rs_val

	\opcode		\ra_no,		\rs_no,		\ui_val@l /* perform operation */
	stwu		\ra_no,		4(\result_address)
	test_event	EVENT_CHECK_CPU_PPC_PROG_MODEL_17  /* raise event for verilog */
	ppc_nop	/* for syncronization purpose */
.endm

/* check macro for operations with mask "opcode RA, RS, RB" */
.macro LOGIC_RA_RS_RB_CHECK	opcode,     \
							ra_no,      \
							rs_no,      \
							rb_no,      \
							rs_val,     \
							rb_val,     \
							cmd_cnv,    \
							set_CA_bit, \
							res_addr
	load_const	\info_reg,	0x00000000 /* clear info register */

	ori			\info_reg,	\info_reg,	0x02 /* put mask tyoe */
	load_const	TMP_REG,	0x06
	slw			\info_reg,	\info_reg,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			\info_reg,	\info_reg,	\cmd_cnv
	load_const	TMP_REG,	0x08
	slw			\info_reg,	\info_reg,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			\info_reg,	\info_reg,	\ra_no
.if (\set_CA_bit != 0)
	ori			\info_reg,	\info_reg,	0x80
.endif

	load_const	TMP_REG,	0x08
	slw			\info_reg,	\info_reg,	TMP_REG

	ori			\info_reg,	\info_reg,	\rs_no /* put RS reg number into info reg */
	load_const	TMP_REG,	0x08
	slw			\info_reg,	\info_reg,	TMP_REG

	ori			\info_reg,	\info_reg,	\rb_no /* put RB reg number into info reg */

.if (\set_CA_bit == 0)
	load_const	TMP_REG,	0x00000000
.else
	load_const	TMP_REG,	0x20000000
.endif
	mtxer		TMP_REG

	load_const	\rs_no,		\rs_val /* load values into reg with number rs_no */
	load_const	\rb_no,		\rb_val /* load values into reg with number rb_no */

	\opcode		\ra_no,		\rs_no,		\rb_no /* perform operation */
	stwu		\ra_no,		4(\res_addr)
	test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_17 /* raise event for verilog */
	ppc_nop		/* for syncronization purpose */
.endm

/* check macro for operations with mask "opcode RA, RS" */
.macro LOGIC_RA_RS_CHECK	opcode,     \
							ra_no,      \
							rs_no,      \
							rs_val,     \
							info_reg,   \
							cmd_cnv,    \
							temp_reg,   \
							set_CA_bit, \
							res_addr
	load_const	\info_reg,	0x00000000 /* clear info register */

	ori			\info_reg,	\info_reg,	0x03 /* put mask tyoe */
	load_const	TMP_REG,	0x06
	slw			\info_reg,	\info_reg,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			\info_reg,	\info_reg,	\cmd_cnv
	load_const	TMP_REG,	0x08
	slw			\info_reg,	\info_reg,	TMP_REG

	ori			\info_reg,	\info_reg,	\ra_no /* put RA reg number into info reg */
.if (\set_CA_bit != 0)
	ori			\info_reg,	\info_reg,	0x80
.endif

	load_const	TMP_REG,	0x08
	slw			\info_reg,	\info_reg,	TMP_REG

	ori			\info_reg,	\info_reg,	\rs_no /* put RS reg number into info reg */
	load_const	TMP_REG,	0x08
	slw			\info_reg,	\info_reg,	TMP_REG

.if (\set_CA_bit == 0)
	load_const	TMP_REG	0x00000000
.else
	load_const	TMP_REG	0x20000000
.endif
	mtxer		TMP_REG
	load_const	\rs_no,		\rs_val /* load values into reg with number rs_no */

	\opcode		\ra_no,		\rs_no /* perform operation */
	stwu		\ra_no,		4(\res_addr)
	test_event	EVENT_CHECK_CPU_PPC_PROG_MODEL_17 /* raise event for verilog */
	ppc_nop	/* for syncronization purpose */
.endm


.section ".text","ax",@progbits


.global main
main:

    load_const	r3,	0x0004FFFC
    load_const	r0,	0x00000000

    mtxer r0
/*  LOGIC_RA_RS_UI_CHECK   opcode, ra_no, rs_no, rs_val,     ui_val, info_reg_no,  comand_no, temp_reg_no,       result_address */
    LOGIC_RA_RS_UI_CHECK   addi,   7,     6,     0xAA55AA55, 0x5A5A, 30,           0x0,       29,                3
    LOGIC_RA_RS_UI_CHECK   addi,   7,     6,     0xAA55AA55, 0x5A5A, 30,           0x0,       29,                3
    LOGIC_RA_RS_UI_CHECK   addi,   7,     6,     0xAA55AA55, 0x5A5A, 30,           0x0,       29,                3
    LOGIC_RA_RS_UI_CHECK   addi,   7,     6,     0xAA55AA55, 0x5A5A, 30,           0x0,       29,                3
    LOGIC_RA_RS_UI_CHECK   addis,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x1,       29,                3
    LOGIC_RA_RS_UI_CHECK   addis,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x1,       29,                3
    LOGIC_RA_RS_UI_CHECK   addis,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x1,       29,                3
    LOGIC_RA_RS_UI_CHECK   addis,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x1,       29,                3
    LOGIC_RA_RS_UI_CHECK   addic,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x2,       29,                3
    LOGIC_RA_RS_UI_CHECK   addic,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x2,       29,                3
    LOGIC_RA_RS_UI_CHECK   addic,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x2,       29,                3
    LOGIC_RA_RS_UI_CHECK   addic,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x2,       29,                3
    LOGIC_RA_RS_UI_CHECK   addic,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x2,       29,                3
    LOGIC_RA_RS_UI_CHECK   addic., 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x3,       29,                3
    LOGIC_RA_RS_UI_CHECK   addic., 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x3,       29,                3
    LOGIC_RA_RS_UI_CHECK   addic., 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x3,       29,                3
    LOGIC_RA_RS_UI_CHECK   addic., 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x3,       29,                3
    LOGIC_RA_RS_UI_CHECK   addic., 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x3,       29,                3
    LOGIC_RA_RS_UI_CHECK   subfic, 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x4,       29,                3
    LOGIC_RA_RS_UI_CHECK   subfic, 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x4,       29,                3
    LOGIC_RA_RS_UI_CHECK   subfic, 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x4,       29,                3
    LOGIC_RA_RS_UI_CHECK   subfic, 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x4,       29,                3
    LOGIC_RA_RS_UI_CHECK   subfic, 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x4,       29,                3
    LOGIC_RA_RS_UI_CHECK   mulli,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x5,       29,                3
    LOGIC_RA_RS_UI_CHECK   mulli,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x5,       29,                3
    LOGIC_RA_RS_UI_CHECK   mulli,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x5,       29,                3
    LOGIC_RA_RS_UI_CHECK   mulli,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x5,       29,                3

/*  LOGIC_RA_RS_RB_CHECK    ( opcode,   ra_no, rs_no, rb_no, rs_val,     rb_val,          info_reg_no, comand_no,  temp_reg_no, set_CA_bit, result_address ) */
    LOGIC_RA_RS_RB_CHECK      add,      8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x00,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      add.,     8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x01,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      addo,     8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x02,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      addo.,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x03,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subf,     8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x04,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subf.,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x05,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subfo,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x06,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subfo.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x07,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      addc,     8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x08,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      addc.,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x09,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      addco,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x0A,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      addco.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x0B,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subfc,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x0C,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subfc.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x0D,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subfco,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x0E,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subfco.,  8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x0F,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      adde,     8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x10,       29,        1,             3
    LOGIC_RA_RS_RB_CHECK      adde,     8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x10,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      adde.,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x11,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      addeo,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x12,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      addeo.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x13,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subfe,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x14,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subfe.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x15,       29,        1,             3
    LOGIC_RA_RS_RB_CHECK      subfeo,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x16,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      subfeo.,  8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x17,       29,        1,             3
    LOGIC_RA_RS_RB_CHECK      mullw,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x18,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      mullw.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x19,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      mullwo,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x1A,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      mullwo.,  8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x1B,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divw,     8,     6,     7,     0xAA55AA55, 0x00005A5A,      30,          0x1C,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divw.,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x1D,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divwo,    8,     6,     7,     0xAA55AA55, 0x00005A5A,      30,          0x1E,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divwo,    8,     6,     7,     0x80000000, 0xffffffff,      30,          0x1E,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divwo.,   8,     6,     7,     0xAA55AA55, 0xFFFF5A5A,      30,          0x1F,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divwo.,   8,     6,     7,     0x55AA55AA, 0xFFFF5A5A,      30,          0x1F,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divwo.,   8,     6,     7,     0x55AA55AA, 0x00000000,      30,          0x1F,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divwu,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x20,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divwu.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x21,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divwuo,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x22,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      divwuo.,  8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x23,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      mulhw,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x24,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      mulhw.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x25,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      mulhwu,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x26,       29,        0,             3
    LOGIC_RA_RS_RB_CHECK      mulhwu.,  8,     6,     7,     0xAA55AA55, 0x5A5A5A5A,      30,          0x27,       29,        0,             3
/*  LOGIC_RA_RS_CHECK   opcode,       ra_no, rs_no, rs_val,        info_reg_no, comand_no,  temp_reg_no, set_CA_bit,    result_address) */
    LOGIC_RA_RS_CHECK   addme,        7,     6,     0xAA55AA55,    30,          0x00,       29,          0,             3
    LOGIC_RA_RS_CHECK   addme.,       7,     6,     0xAA55AA55,    30,          0x01,       29,          1,             3
    LOGIC_RA_RS_CHECK   addmeo,       7,     6,     0xAA55AA55,    30,          0x02,       29,          0,             3
    LOGIC_RA_RS_CHECK   addmeo.,      7,     6,     0xAA55AA55,    30,          0x03,       29,          1,             3
    LOGIC_RA_RS_CHECK   subfme,       7,     6,     0xAA55AA55,    30,          0x04,       29,          1,             3
    LOGIC_RA_RS_CHECK   subfme.,      7,     6,     0xAA55AA55,    30,          0x05,       29,          0,             3
    LOGIC_RA_RS_CHECK   subfmeo,      7,     6,     0xAA55AA55,    30,          0x06,       29,          1,             3
    LOGIC_RA_RS_CHECK   subfmeo.,     7,     6,     0xAA55AA55,    30,          0x07,       29,          0,             3
    LOGIC_RA_RS_CHECK   addze,        7,     6,     0xAA55AA55,    30,          0x08,       29,          0,             3
    LOGIC_RA_RS_CHECK   addze.,       7,     6,     0xAA55AA55,    30,          0x09,       29,          1,             3
    LOGIC_RA_RS_CHECK   addzeo,       7,     6,     0xAA55AA55,    30,          0x0A,       29,          0,             3
    LOGIC_RA_RS_CHECK   addzeo.,      7,     6,     0xAA55AA55,    30,          0x0B,       29,          1,             3
    LOGIC_RA_RS_CHECK   subfze,       7,     6,     0xAA55AA55,    30,          0x0C,       29,          1,             3
    LOGIC_RA_RS_CHECK   subfze.,      7,     6,     0xAA55AA55,    30,          0x0D,       29,          0,             3
    LOGIC_RA_RS_CHECK   subfzeo,      7,     6,     0xAA55AA55,    30,          0x0E,       29,          1,             3
    LOGIC_RA_RS_CHECK   subfzeo.,     7,     6,     0xAA55AA55,    30,          0x0F,       29,          0,             3
    LOGIC_RA_RS_CHECK   neg,          7,     6,     0xAA55AA55,    30,          0x10,       29,          0,             3
    LOGIC_RA_RS_CHECK   neg.,         7,     6,     0xAA55AA55,    30,          0x11,       29,          0,             3

	load_const r3, 0x00050000
	load_addr r4, iss_mem
	load_const r5, 89
	bl compare_data
	cmpwi cr0, r3, 0x1
	beq cr0, finish_ok
	b finish_error

.align 2
iss_mem:
	.long 0x00005A5A
	.long 0x00005A5A
	.long 0x00005A5A
	.long 0x00005A5A
	.long 0x5A5A0000
	.long 0x5A5A0000
	.long 0x5A5A0000
	.long 0x5A5A0000
	.long 0xAA5604AF
	.long 0xAA5604AF
	.long 0xAA5604AF
	.long 0xAA5604AF
	.long 0xAA5604AF
	.long 0xAA5604AF
	.long 0xAA5604AF
	.long 0xAA5604AF
	.long 0xAA5604AF
	.long 0xAA5604AF
	.long 0x55AAB005
	.long 0x55AAB005
	.long 0x55AAB005
	.long 0x55AAB005
	.long 0x55AAB005
	.long 0xFFFFC3E2
	.long 0xFFFFC3E2
	.long 0xFFFFC3E2
	.long 0xFFFFC3E2
	.long 0x04B004AF
	.long 0x04B004AF
	.long 0x04B004AF
	.long 0x04B004AF
	.long 0xB004B005
	.long 0xB004B005
	.long 0xB004B005
	.long 0xB004B005
	.long 0x04B004AF
	.long 0x04B004AF
	.long 0x04B004AF
	.long 0x04B004AF
	.long 0xB004B005
	.long 0xB004B005
	.long 0xB004B005
	.long 0xB004B005
	.long 0x04B004B0
	.long 0x04B004AF
	.long 0x04B004AF
	.long 0x04B004AF
	.long 0x04B004AF
	.long 0xB004B004
	.long 0xB004B005
	.long 0xB004B004
	.long 0xB004B005
	.long 0xC3E1C3E2
	.long 0xC3E1C3E2
	.long 0xC3E1C3E2
	.long 0xC3E1C3E2
	.long 0xFFFF0D48
	.long 0x00000000
	.long 0xFFFF0D48
	.long 0x00008464
	.long 0xFFFF7B9C
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0xE1C3E1C3
	.long 0xE1C3E1C3
	.long 0x3C1E3C1D
	.long 0x3C1E3C1D
	.long 0xAA55AA54
	.long 0xAA55AA55
	.long 0xAA55AA54
	.long 0xAA55AA55
	.long 0x55AA55AA
	.long 0x55AA55A9
	.long 0x55AA55AA
	.long 0x55AA55A9
	.long 0xAA55AA55
	.long 0xAA55AA56
	.long 0xAA55AA55
	.long 0xAA55AA56
	.long 0x55AA55AB
	.long 0x55AA55AA
	.long 0x55AA55AB
	.long 0x55AA55AA
	.long 0x55AA55AB
	.long 0x55AA55AB
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000

/*
# Memory save for Wed Feb  7 09:28:09 2018
# Address     = 0x00050000
# Byte length = 0x00000200

00050000  00005A5A 00005A5A 00005A5A 00005A5A
00050010  5A5A0000 5A5A0000 5A5A0000 5A5A0000
00050020  AA5604AF AA5604AF AA5604AF AA5604AF
00050030  AA5604AF AA5604AF AA5604AF AA5604AF
00050040  AA5604AF AA5604AF 55AAB005 55AAB005
00050050  55AAB005 55AAB005 55AAB005 FFFFC3E2
00050060  FFFFC3E2 FFFFC3E2 FFFFC3E2 04B004AF
00050070  04B004AF 04B004AF 04B004AF B004B005
00050080  B004B005 B004B005 B004B005 04B004AF
00050090  04B004AF 04B004AF 04B004AF B004B005
000500A0  B004B005 B004B005 B004B005 04B004B0
000500B0  04B004AF 04B004AF 04B004AF 04B004AF
000500C0  B004B004 B004B005 B004B004 B004B005
000500D0  C3E1C3E2 C3E1C3E2 C3E1C3E2 C3E1C3E2
000500E0  FFFF0D48 00000000 FFFF0D48 00000000
000500F0  00008464 FFFF7B9C 00000000 00000001
00050100  00000001 00000001 00000001 E1C3E1C3
00050110  E1C3E1C3 3C1E3C1D 3C1E3C1D AA55AA54
00050120  AA55AA55 AA55AA54 AA55AA55 55AA55AA
00050130  55AA55A9 55AA55AA 55AA55A9 AA55AA55
00050140  AA55AA56 AA55AA55 AA55AA56 55AA55AB
00050150  55AA55AA 55AA55AB 55AA55AA 55AA55AB
00050160  55AA55AB 00000000 00000000 00000000
00050170  00000000 00000000 00000000 00000000
00050180  00000000 00000000 00000000 00000000
00050190  00000000 00000000 00000000 00000000
000501A0  00000000 00000000 00000000 00000000
000501B0  00000000 00000000 00000000 00000000
000501C0  00000000 00000000 00000000 00000000
000501D0  00000000 00000000 00000000 00000000
000501E0  00000000 00000000 00000000 00000000
000501F0  00000000 00000000 00000000 00000000
*/


/* AG: */
compare_data:
	cmpwi cr0,r5,0
	li r8,1
	beq cr0, .compare_data_L1
	li r10,0
.compare_data_L4:
	lwzx r9,r3,r10
	lwzx r7,r4,r10
	addi r10,r10,4
	cmplw cr7,r5,r10
	xor r9,r9,r7
	addic r9,r9,-1
	subfe r9,r9,r9
	and r8,r8,r9
	bgt cr7, .compare_data_L4
.compare_data_L1:
	mr r3,r8
	blr

finish_error:
	rumboot_putstring "TEST ERROR\n"
	test_event		EVENT_ERROR
	load_const r3,	RESULT_ERROR
	b finish

finish_ok:
	rumboot_putstring "TEST OK\n"
	test_event		EVENT_OK
	load_const r3,	RESULT_OK

finish:
	blr


