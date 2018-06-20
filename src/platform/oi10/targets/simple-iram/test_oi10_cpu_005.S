#include <platform/test_assert.S.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>
#include <platform/devices.h>
#include <platform/regs/regs_mpic128.h>


#define SAVE_ADDR	IM0_BASE + 0x0001E000

#define TMP_REG		29
#define INF_REG		30
#define REGS_AS 	7,6
#define REGS_ASB	8,6,7

#define ppc_nop xori 0,0,0

.macro subcall callLabel
	mflr	TMP_REG
	mtspr	SPR_SPRG8,	TMP_REG
	bl		\callLabel
	mfspr	TMP_REG,	SPR_SPRG8
	mtlr	TMP_REG
.endm

/* check macro for operations with mask "opcode RA, RS, UI" */
.macro LOGIC_RA_RS_UI_CHECK	opcode,   \
							ra_reg,   \
							rs_reg,   \
							rs_val,   \
							ui_val,   \
							cmd_cnv,  \
							res_addr

	rumboot_putstring "\t\opcode ... "
	load_const	INF_REG,	0x00000000 /* clear info register */
	load_const	\ra_reg,	0x00000000 /* clear result register */

	ori			INF_REG,	INF_REG,	0x01 /* put mask tyoe */
	load_const	TMP_REG,	0x04
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			INF_REG,	INF_REG,	\cmd_cnv
	load_const	TMP_REG,	0x05
	slw			INF_REG,	INF_REG,	TMP_REG

 	/* put RA reg number into info reg */
 	ori			INF_REG,	INF_REG,	\ra_reg
	load_const	TMP_REG,	0x05
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put RS reg number into info reg */
	ori			INF_REG,	INF_REG,	\rs_reg
	load_const	TMP_REG,	0x10
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put UI const into info reg */
	ori			INF_REG,	INF_REG,	\ui_val@l

	load_const	TMP_REG,	0x00000000
	mtxer		TMP_REG

	 /* load values into reg with number rs_no */
	load_const	\rs_reg,	\rs_val

	\opcode		\ra_reg,	\rs_reg,	\ui_val@l /* perform operation */
	stwu		\ra_reg,	4(\res_addr)
	test_event	EVENT_CHECK_CPU_PPC_PROG_MODEL_17  /* raise event for verilog */
	ppc_nop	/* for syncronization purpose */
	rumboot_putstring "\tdone.\n"
.endm

/* check macro for operations with mask "opcode RA, RS, RB" */
.macro LOGIC_RA_RS_RB_CHECK	opcode,     \
							ra_reg,     \
							rs_reg,     \
							rb_reg,     \
							rs_val,     \
							rb_val,     \
							cmd_cnv,    \
							set_CA_bit, \
							res_addr

	rumboot_putstring "\t\opcode ... "

	load_const	INF_REG,	0x00000000 /* clear info register */

	ori			INF_REG,	INF_REG,	0x02 /* put mask tyoe */
	load_const	TMP_REG,	0x06
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			INF_REG,	INF_REG,	\cmd_cnv
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			INF_REG,	INF_REG,	\ra_reg
.if (\set_CA_bit != 0)
	ori			INF_REG,	INF_REG,	0x80
.endif

	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	ori			INF_REG,	INF_REG,	\rs_reg /* put RS reg number into info reg */
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	ori			INF_REG,	INF_REG,	\rb_reg /* put RB reg number into info reg */

.if (\set_CA_bit == 0)
	load_const	TMP_REG,	0x00000000
.else
	load_const	TMP_REG,	0x20000000
.endif
	mtxer		TMP_REG

	load_const	\rs_reg,	\rs_val /* load values into reg with number rs_no */
	load_const	\rb_reg,	\rb_val /* load values into reg with number rb_no */

	\opcode		\ra_reg,	\rs_reg,	\rb_reg /* perform operation */
	stwu		\ra_reg,	4(\res_addr)
	test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_17 /* raise event for verilog */
	ppc_nop		/* for syncronization purpose */
	rumboot_putstring "\tdone.\n"
.endm

/* check macro for operations with mask "opcode RA, RS" */
.macro LOGIC_RA_RS_CHECK	opcode,     \
							ra_reg,     \
							rs_reg,     \
							rs_val,     \
							cmd_cnv,    \
							set_CA_bit, \
							res_addr

	rumboot_putstring "\t\opcode ... "

	load_const	INF_REG,	0x00000000 /* clear info register */

	ori			INF_REG,	INF_REG,	0x03 /* put mask tyoe */
	load_const	TMP_REG,	0x06
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			INF_REG,	INF_REG,	\cmd_cnv
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	ori			INF_REG,	INF_REG,	\ra_reg /* put RA reg number into info reg */
.if (\set_CA_bit != 0)
	ori			INF_REG,	INF_REG,	0x80
.endif

	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	ori			INF_REG,	INF_REG,	\rs_reg /* put RS reg number into info reg */
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

.if (\set_CA_bit == 0)
	load_const	TMP_REG,	0x00000000
.else
	load_const	TMP_REG,	0x20000000
.endif
	mtxer		TMP_REG
	load_const	\rs_reg,	\rs_val /* load values into reg with number rs_no */

	\opcode		\ra_reg,	\rs_reg /* perform operation */
	stwu		\ra_reg,	4(\res_addr)
	test_event	EVENT_CHECK_CPU_PPC_PROG_MODEL_17 /* raise event for verilog */
	ppc_nop	/* for syncronization purpose */
	rumboot_putstring "\tdone.\n"
.endm


.section ".text","ax",@progbits


.global main
main:
	rumboot_putstring "Start testing...\n"
    load_const	r3,	SAVE_ADDR - 4
    load_const	r0,	0x00000000

    mtxer r0

/*  LOGIC_RA_RS_UI_CHECK	opcode,   (ra,rs),  rs_val,     ui_val,     cmd,      res_addr  */
    LOGIC_RA_RS_UI_CHECK	addi,     REGS_AS,  0xAA55AA55, 0x5A5A,     0x00,     3 /* 1  - */
    LOGIC_RA_RS_UI_CHECK	addi,     REGS_AS,  0xAA55AA55, 0x5A5A,     0x00,     3 /* 2  - */
    LOGIC_RA_RS_UI_CHECK	addi,     REGS_AS,  0xAA55AA55, 0x5A5A,     0x00,     3 /* 3  - */
    LOGIC_RA_RS_UI_CHECK	addi,     REGS_AS,  0xAA55AA55, 0x5A5A,     0x00,     3 /* 4  - */
    LOGIC_RA_RS_UI_CHECK	addis,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x01,     3 /* 5  - */
    LOGIC_RA_RS_UI_CHECK	addis,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x01,     3 /* 6  - */
    LOGIC_RA_RS_UI_CHECK	addis,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x01,     3 /* 7  - */
    LOGIC_RA_RS_UI_CHECK	addis,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x01,     3 /* 8  - */
    LOGIC_RA_RS_UI_CHECK	addic,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x02,     3 /* 9  + */
    LOGIC_RA_RS_UI_CHECK	addic,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x02,     3 /* 10 + */
    LOGIC_RA_RS_UI_CHECK	addic,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x02,     3 /* 11 + */
    LOGIC_RA_RS_UI_CHECK	addic,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x02,     3 /* 12 + */
    LOGIC_RA_RS_UI_CHECK	addic,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x02,     3 /* 13 + */
    LOGIC_RA_RS_UI_CHECK	addic.,   REGS_AS,  0xAA55AA55, 0x5A5A,     0x03,     3 /* 14 + */
    LOGIC_RA_RS_UI_CHECK	addic.,   REGS_AS,  0xAA55AA55, 0x5A5A,     0x03,     3 /* 15 + */
    LOGIC_RA_RS_UI_CHECK	addic.,   REGS_AS,  0xAA55AA55, 0x5A5A,     0x03,     3 /* 16 + */
    LOGIC_RA_RS_UI_CHECK	addic.,   REGS_AS,  0xAA55AA55, 0x5A5A,     0x03,     3 /* 17 + */
    LOGIC_RA_RS_UI_CHECK	addic.,   REGS_AS,  0xAA55AA55, 0x5A5A,     0x03,     3 /* 18 + */
    LOGIC_RA_RS_UI_CHECK	subfic,   REGS_AS,  0xAA55AA55, 0x5A5A,     0x04,     3 /* 19 + */
    LOGIC_RA_RS_UI_CHECK	subfic,   REGS_AS,  0xAA55AA55, 0x5A5A,     0x04,     3 /* 20 + */
    LOGIC_RA_RS_UI_CHECK	subfic,   REGS_AS,  0xAA55AA55, 0x5A5A,     0x04,     3 /* 21 + */
    LOGIC_RA_RS_UI_CHECK	subfic,   REGS_AS,  0xAA55AA55, 0x5A5A,     0x04,     3 /* 22 + */
    LOGIC_RA_RS_UI_CHECK	subfic,   REGS_AS,  0xAA55AA55, 0x5A5A,     0x04,     3 /* 23 + */
    LOGIC_RA_RS_UI_CHECK	mulli,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x05,     3 /* 24 + */
    LOGIC_RA_RS_UI_CHECK	mulli,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x05,     3 /* 25 + */
    LOGIC_RA_RS_UI_CHECK	mulli,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x05,     3 /* 26 + */
    LOGIC_RA_RS_UI_CHECK	mulli,    REGS_AS,  0xAA55AA55, 0x5A5A,     0x05,     3 /* 27 + */

/*  LOGIC_RA_RS_RB_CHECK    opcode, (ra,rs,rb), rs_val,     rb_val,     cmd,  CA, res_addr  */
    LOGIC_RA_RS_RB_CHECK	add,      REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x00, 0,  3 /* 28 + */
    LOGIC_RA_RS_RB_CHECK	add.,     REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x01, 0,  3 /* 29 + */
    LOGIC_RA_RS_RB_CHECK	addo,     REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x02, 0,  3 /* 30 + */
    LOGIC_RA_RS_RB_CHECK	addo.,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x03, 0,  3 /* 31 + */
    LOGIC_RA_RS_RB_CHECK	subf,     REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x04, 0,  3 /* 32 + */
    LOGIC_RA_RS_RB_CHECK	subf.,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x05, 0,  3 /* 33 + */
    LOGIC_RA_RS_RB_CHECK	subfo,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x06, 0,  3 /* 34 + */
    LOGIC_RA_RS_RB_CHECK	subfo.,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x07, 0,  3 /* 35 + */
    LOGIC_RA_RS_RB_CHECK	addc,     REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x08, 0,  3 /* 36 + */
    LOGIC_RA_RS_RB_CHECK	addc.,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x09, 0,  3 /* 37 + */
    LOGIC_RA_RS_RB_CHECK	addco,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x0A, 0,  3 /* 38 + */
    LOGIC_RA_RS_RB_CHECK	addco.,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x0B, 0,  3 /* 39 + */
    LOGIC_RA_RS_RB_CHECK	subfc,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x0C, 0,  3 /* 40 + */
    LOGIC_RA_RS_RB_CHECK	subfc.,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x0D, 0,  3 /* 41 + */
    LOGIC_RA_RS_RB_CHECK	subfco,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x0E, 0,  3 /* 42 + */
    LOGIC_RA_RS_RB_CHECK	subfco.,  REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x0F, 0,  3 /* 43 + */
    LOGIC_RA_RS_RB_CHECK	adde,     REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x10, 1,  3 /* 44 + */
    LOGIC_RA_RS_RB_CHECK	adde,     REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x10, 0,  3 /* 45 + */
    LOGIC_RA_RS_RB_CHECK	adde.,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x11, 0,  3 /* 46 + */
    LOGIC_RA_RS_RB_CHECK	addeo,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x12, 0,  3 /* 47 + */
    LOGIC_RA_RS_RB_CHECK	addeo.,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x13, 0,  3 /* 48 + */
    LOGIC_RA_RS_RB_CHECK	subfe,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x14, 0,  3 /* 49 + */
    LOGIC_RA_RS_RB_CHECK	subfe.,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x15, 1,  3 /* 50 + */
    LOGIC_RA_RS_RB_CHECK	subfeo,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x16, 0,  3 /* 51 + */
    LOGIC_RA_RS_RB_CHECK	subfeo.,  REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x17, 1,  3 /* 52 + */
    LOGIC_RA_RS_RB_CHECK	mullw,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x18, 0,  3 /* 53 + */
    LOGIC_RA_RS_RB_CHECK	mullw.,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x19, 0,  3 /* 54 + */
    LOGIC_RA_RS_RB_CHECK	mullwo,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x1A, 0,  3 /* 55 + */
    LOGIC_RA_RS_RB_CHECK	mullwo.,  REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x1B, 0,  3 /* 56 + */
    LOGIC_RA_RS_RB_CHECK	divw,     REGS_ASB, 0xAA55AA55, 0x00005A5A, 0x1C, 0,  3 /* 57 + */
    LOGIC_RA_RS_RB_CHECK	divw.,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x1D, 0,  3 /* 58 + */
    LOGIC_RA_RS_RB_CHECK	divwo,    REGS_ASB, 0xAA55AA55, 0x00005A5A, 0x1E, 0,  3 /* 59 + */
    LOGIC_RA_RS_RB_CHECK	divwo,    REGS_ASB, 0x80000000, 0xffffffff, 0x1E, 0,  3 /* 60 + */
    LOGIC_RA_RS_RB_CHECK	divwo.,   REGS_ASB, 0xAA55AA55, 0xFFFF5A5A, 0x1F, 0,  3 /* 61 + */
    LOGIC_RA_RS_RB_CHECK	divwo.,   REGS_ASB, 0x55AA55AA, 0xFFFF5A5A, 0x1F, 0,  3 /* 62 - */
    LOGIC_RA_RS_RB_CHECK	divwo.,   REGS_ASB, 0x55AA55AA, 0x00000000, 0x1F, 0,  3 /* 63 - */
    LOGIC_RA_RS_RB_CHECK	divwu,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x20, 0,  3 /* 64 - */
    LOGIC_RA_RS_RB_CHECK	divwu.,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x21, 0,  3 /* 65 - */
    LOGIC_RA_RS_RB_CHECK	divwuo,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x22, 0,  3 /* 66 + */
    LOGIC_RA_RS_RB_CHECK	divwuo.,  REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x23, 0,  3 /* 67 + */
    LOGIC_RA_RS_RB_CHECK	mulhw,    REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x24, 0,  3 /* 68 + */
    LOGIC_RA_RS_RB_CHECK	mulhw.,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x25, 0,  3 /* 69 - */
    LOGIC_RA_RS_RB_CHECK	mulhwu,   REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x26, 0,  3 /* 70 + */
    LOGIC_RA_RS_RB_CHECK	mulhwu.,  REGS_ASB, 0xAA55AA55, 0x5A5A5A5A, 0x27, 0,  3 /* 71 - */

/*  LOGIC_RA_RS_CHECK		opcode,   (ra,rs),  rs_val,                 cmd,  CA, res_addr  */
    LOGIC_RA_RS_CHECK		addme,    REGS_AS,  0xAA55AA55,             0x00, 0,  3 /* 72 - */
    LOGIC_RA_RS_CHECK		addme.,   REGS_AS,  0xAA55AA55,             0x01, 1,  3 /* 73 - */
    LOGIC_RA_RS_CHECK		addmeo,   REGS_AS,  0xAA55AA55,             0x02, 0,  3 /* 74 - */
    LOGIC_RA_RS_CHECK		addmeo.,  REGS_AS,  0xAA55AA55,             0x03, 1,  3 /* 75 - */
    LOGIC_RA_RS_CHECK		subfme,   REGS_AS,  0xAA55AA55,             0x04, 1,  3 /* 76 - */
    LOGIC_RA_RS_CHECK		subfme.,  REGS_AS,  0xAA55AA55,             0x05, 0,  3 /* 77 - */
    LOGIC_RA_RS_CHECK		subfmeo,  REGS_AS,  0xAA55AA55,             0x06, 1,  3 /* 78 - */
    LOGIC_RA_RS_CHECK		subfmeo., REGS_AS,  0xAA55AA55,             0x07, 0,  3 /* 79 - */
    LOGIC_RA_RS_CHECK		addze,    REGS_AS,  0xAA55AA55,             0x08, 0,  3 /* 70 - */
    LOGIC_RA_RS_CHECK		addze.,   REGS_AS,  0xAA55AA55,             0x09, 1,  3 /* 81 - */
    LOGIC_RA_RS_CHECK		addzeo,   REGS_AS,  0xAA55AA55,             0x0A, 0,  3 /* 82 - */
    LOGIC_RA_RS_CHECK		addzeo.,  REGS_AS,  0xAA55AA55,             0x0B, 1,  3 /* 83 - */
    LOGIC_RA_RS_CHECK		subfze,   REGS_AS,  0xAA55AA55,             0x0C, 1,  3 /* 84 - */
    LOGIC_RA_RS_CHECK		subfze.,  REGS_AS,  0xAA55AA55,             0x0D, 0,  3 /* 85 - */
    LOGIC_RA_RS_CHECK		subfzeo,  REGS_AS,  0xAA55AA55,             0x0E, 1,  3 /* 86 - */
    LOGIC_RA_RS_CHECK		subfzeo., REGS_AS,  0xAA55AA55,             0x0F, 0,  3 /* 87 - */
    LOGIC_RA_RS_CHECK		neg,      REGS_AS,  0xAA55AA55,             0x10, 0,  3 /* 88 + */
    LOGIC_RA_RS_CHECK		neg.,     REGS_AS,  0xAA55AA55,             0x11, 0,  3 /* 89 - */

/*
	load_const	r3,	SAVE_ADDR
	load_addr	r4,	iss_mem
	load_addr 	r5,	iss_mem_end
	sub			r5,	r5,	r4
	load_const	r5,	89*4
	subcall		compare_data
	cmpwi cr2,	r3,	0x01
	beq+  cr2,	test_ok
*/
	b			test_ok
	b			test_error

/*
--------+++++++++++++++++++
                           ++++++++++++++++++++++++++++++++----+++-+-+-
                                                                       ----------------+-
*/
/* AG: */
/*
compare_data:
	rumboot_putstring	"compare data... "
	load_const	r8,		0x0001
	cmpwi cr0,	r5,		0x0000
	beq-  cr0,	compare_data_end
	load_const	r10,	0x0000
compare_data_loop:
	lwzx		r9,		r3,  r10
	lwzx		r7,		r4,  r10
	addi		r10,	r10, 0x0004
	cmpw cr6,	r5,		r10
	cmpw cr5,	r7,		r9
	beq+ cr5,	compare_data_true
	li			r13,	'-'
	b			compare_data_false
compare_data_true:
	li			r13,	'+'
compare_data_false:
	_putchar	r13,	r12
	xor			r9,		r9,  r7
	addic		r9,		r9,  -1
	subfe		r9,		r9,  r9
	and			r8,		r8,  r9
	bgt+ cr6,	compare_data_loop
compare_data_end:
	mr			r3,		r8
	rumboot_putstring	" done!\n"
	blr
*/

test_error:
	rumboot_putstring "TEST ERROR\n"
	test_event		EVENT_ERROR
	load_const r3,	RESULT_ERROR
	b finish

test_ok:
	rumboot_putstring "TEST OK\n"
	test_event		EVENT_OK
	load_const r3,	RESULT_OK

finish:
	blr


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
iss_mem_end:



