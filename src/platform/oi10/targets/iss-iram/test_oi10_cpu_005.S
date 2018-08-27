
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/test_assert.S.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>
#include <platform/devices.h>

#define SAVE_ADDR	save_data /* IM0_BASE + 0x0001E000 */

#define TMP_REG		29
#define INF_REG		30
#define RES_REG		3
#define REGS_AS1 	7,6
#define REGS_AS2 	8,7
#define REGS_ASB	8,6,7
#define DUMP_SIZE	0x0200

#define ppc_nop xori 0,0,0

.macro zero_memory vAddr vSize
	load_addr	r4,	(\vAddr) - 4
	load_const	r5,	(\vSize) / 4
	mtctr		r5
	xor			r5,	r5,	r5
51:
	stwu		r5,	4(r4)
	bdnz		51b
.endm

/* check macro for operations with mask "opcode RA, RS, UI" */
.macro LOGIC_RA_RS_UI_CHECK	opcode,   \
							ra_reg,   \
							rs_reg,   \
							rs_val,   \
							ui_val,   \
							cmd_cnv

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
	stwu		\ra_reg,	4(RES_REG)
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
							set_CA_bit

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
	stwu		\ra_reg,	4(RES_REG)
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
							set_CA_bit

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
	stwu		\ra_reg,	4(RES_REG)
	test_event	EVENT_CHECK_CPU_PPC_PROG_MODEL_17 /* raise event for verilog */
	ppc_nop	/* for syncronization purpose */
	rumboot_putstring "\tdone.\n"
.endm


.section ".text","ax",@progbits


.global main
main:
    test_event_send_test_id "test_iss_base"
	rumboot_putstring "Start testing...\n"
	rumboot_putstring "Clearing array... "
	zero_memory	SAVE_ADDR,	DUMP_SIZE
	rumboot_putstring "done.\n"
    load_const	RES_REG,	SAVE_ADDR - 4
    load_const	r0,			0x00000000

    mtxer r0

/*  LOGIC_RA_RS_UI_CHECK	opcode,   (ra,rs),  rs_val,     ui_val,     cmd */
    LOGIC_RA_RS_UI_CHECK	addi,     REGS_AS1,	0xAA55AA55, 0x5A5A,		0x00
    LOGIC_RA_RS_UI_CHECK	addi,     REGS_AS1,	0xAA55AA55, 0xA5A5,		0x00
    LOGIC_RA_RS_UI_CHECK	addi,     REGS_AS2,	0x55AA55AA, 0x5A5A,		0x00
    LOGIC_RA_RS_UI_CHECK	addi,     REGS_AS2,	0xAA55AA55, 0xA5A5,		0x00

    LOGIC_RA_RS_UI_CHECK	addis,    REGS_AS1,	0xAA55AA55, 0x5A5A,		0x01
    LOGIC_RA_RS_UI_CHECK	addis,    REGS_AS1,	0xFFFFAA55, 0xA5A5,		0x01
    LOGIC_RA_RS_UI_CHECK	addis,    REGS_AS2,	0xAA55AA55, 0x5A5A,		0x01
    LOGIC_RA_RS_UI_CHECK	addis,    REGS_AS2,	0xFFFFAA55, 0x5A5A,		0x01

    LOGIC_RA_RS_UI_CHECK	addic,    REGS_AS1,	0xAA55AA55, 0x5A5A,		0x02
    LOGIC_RA_RS_UI_CHECK	addic,    REGS_AS1,	0xFFFFAA55, 0xA5A5,		0x02
    LOGIC_RA_RS_UI_CHECK	addic,    REGS_AS1,	0xFFFFAA55, 0x5A5A,		0x02
    LOGIC_RA_RS_UI_CHECK	addic,    REGS_AS1,	0x7FFFFFFF, 0x5A5A,		0x02
    LOGIC_RA_RS_UI_CHECK	addic,    REGS_AS1,	0x7FFFFFFF, 0xA5A5,		0x02

    LOGIC_RA_RS_UI_CHECK	addic.,   REGS_AS1,	0xAA55AA55, 0x5A5A,		0x03
    LOGIC_RA_RS_UI_CHECK	addic.,   REGS_AS1,	0xFFFFAA55, 0xA5A5,		0x03
    LOGIC_RA_RS_UI_CHECK	addic.,   REGS_AS1,	0xFFFFAA55, 0x5A5A,		0x03
    LOGIC_RA_RS_UI_CHECK	addic.,   REGS_AS1,	0x7FFFFFFF, 0x5A5A,		0x03
    LOGIC_RA_RS_UI_CHECK	addic.,   REGS_AS1,	0x7FFFFFFF, 0xA5A5,		0x03

    LOGIC_RA_RS_UI_CHECK	subfic,   REGS_AS1,	0xAA55AA55, 0x5A5A,		0x04
    LOGIC_RA_RS_UI_CHECK	subfic,   REGS_AS1,	0xFFFFAA55, 0xA5A5,		0x04
    LOGIC_RA_RS_UI_CHECK	subfic,   REGS_AS1,	0x55AA55AA, 0x5A5A,		0x04
    LOGIC_RA_RS_UI_CHECK	subfic,   REGS_AS1,	0x55AA55AA, 0xA5A5,		0x04
    LOGIC_RA_RS_UI_CHECK	subfic,   REGS_AS1,	0x00005A55, 0x5A5A,		0x04

    LOGIC_RA_RS_UI_CHECK	mulli,    REGS_AS1,	0xAA55AA55, 0x5A5A,		0x05
    LOGIC_RA_RS_UI_CHECK	mulli,    REGS_AS1,	0xAA55AA55, 0xFFFF,		0x05
    LOGIC_RA_RS_UI_CHECK	mulli,    REGS_AS1,	0x55AA55AA, 0xFFFF,		0x05
    LOGIC_RA_RS_UI_CHECK	mulli,    REGS_AS1,	0x55AA55AA, 0x0001,		0x05



/*  LOGIC_RA_RS_RB_CHECK    opcode, (ra,rs,rb), rs_val,     rb_val,     cmd,  CA  */
    LOGIC_RA_RS_RB_CHECK	add,      REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x00, 0
    LOGIC_RA_RS_RB_CHECK	add.,     REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x01, 0
    LOGIC_RA_RS_RB_CHECK	addo,     REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x02, 0
    LOGIC_RA_RS_RB_CHECK	addo.,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x03, 0

    LOGIC_RA_RS_RB_CHECK	subf,     REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x04, 0
    LOGIC_RA_RS_RB_CHECK	subf.,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x05, 0
    LOGIC_RA_RS_RB_CHECK	subfo,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x06, 0
    LOGIC_RA_RS_RB_CHECK	subfo.,	  REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x07, 0

    LOGIC_RA_RS_RB_CHECK	addc,     REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x08, 0
    LOGIC_RA_RS_RB_CHECK	addc.,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x09, 0
    LOGIC_RA_RS_RB_CHECK	addco,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x0A, 0
    LOGIC_RA_RS_RB_CHECK	addco.,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x0B, 0

    LOGIC_RA_RS_RB_CHECK	subfc,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x0C, 0
    LOGIC_RA_RS_RB_CHECK	subfc.,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x0D, 0
    LOGIC_RA_RS_RB_CHECK	subfco,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x0E, 0
    LOGIC_RA_RS_RB_CHECK	subfco.,  REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x0F, 0

    LOGIC_RA_RS_RB_CHECK	adde,     REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x10, 0

    LOGIC_RA_RS_RB_CHECK	adde,     REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x10, 1

    LOGIC_RA_RS_RB_CHECK	adde.,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x11, 0
    LOGIC_RA_RS_RB_CHECK	addeo,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x12, 1
    LOGIC_RA_RS_RB_CHECK	addeo.,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x13, 0

    LOGIC_RA_RS_RB_CHECK	subfe,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x14, 0
    LOGIC_RA_RS_RB_CHECK	subfe.,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x15, 1
    LOGIC_RA_RS_RB_CHECK	subfeo,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x16, 0
    LOGIC_RA_RS_RB_CHECK	subfeo.,  REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x17, 1

    LOGIC_RA_RS_RB_CHECK	mullw,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x18, 0
    LOGIC_RA_RS_RB_CHECK	mullw.,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x19, 0
    LOGIC_RA_RS_RB_CHECK	mullwo,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x1A, 0
    LOGIC_RA_RS_RB_CHECK	mullwo.,  REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x1B, 0

    LOGIC_RA_RS_RB_CHECK	divw,     REGS_ASB,	0xAA55AA55, 0x00005A5A, 0x1C, 0
    LOGIC_RA_RS_RB_CHECK	divw.,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x1D, 0
    LOGIC_RA_RS_RB_CHECK	divwo,    REGS_ASB,	0xAA55AA55, 0x00005A5A, 0x1E, 0
    LOGIC_RA_RS_RB_CHECK	divwo,    REGS_ASB,	0x80000000, 0xFFFFFFFF, 0x1E, 0
    LOGIC_RA_RS_RB_CHECK	divwo.,   REGS_ASB,	0xAA55AA55, 0xFFFF5A5A, 0x1F, 0
    LOGIC_RA_RS_RB_CHECK	divwo.,   REGS_ASB,	0x55AA55AA, 0xFFFF5A5A, 0x1F, 0
    LOGIC_RA_RS_RB_CHECK	divwo.,   REGS_ASB,	0x55AA55AA, 0x00000000, 0x1F, 0

    LOGIC_RA_RS_RB_CHECK	divwu,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x20, 0
    LOGIC_RA_RS_RB_CHECK	divwu.,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x21, 0
    LOGIC_RA_RS_RB_CHECK	divwuo,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x22, 0
    LOGIC_RA_RS_RB_CHECK	divwuo.,  REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x23, 0

    LOGIC_RA_RS_RB_CHECK	mulhw,    REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x24, 0
    LOGIC_RA_RS_RB_CHECK	mulhw.,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x25, 0

    LOGIC_RA_RS_RB_CHECK	mulhwu,   REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x26, 0
    LOGIC_RA_RS_RB_CHECK	mulhwu.,  REGS_ASB,	0xAA55AA55, 0x5A5A5A5A, 0x27, 0

/*  LOGIC_RA_RS_CHECK		opcode,   (ra,rs),  rs_val,                 cmd,  CA  */
    LOGIC_RA_RS_CHECK		addme,	  REGS_AS1,	0xAA55AA55,				0x00, 0
    LOGIC_RA_RS_CHECK		addme.,   REGS_AS1,	0xAA55AA55,				0x01, 1
    LOGIC_RA_RS_CHECK		addmeo,   REGS_AS1,	0xAA55AA55,				0x02, 0
    LOGIC_RA_RS_CHECK		addmeo.,  REGS_AS1,	0xAA55AA55,				0x03, 1

    LOGIC_RA_RS_CHECK		subfme,   REGS_AS1,	0xAA55AA55,				0x04, 1
    LOGIC_RA_RS_CHECK		subfme.,  REGS_AS1,	0xAA55AA55,				0x05, 0
    LOGIC_RA_RS_CHECK		subfmeo,  REGS_AS1,	0xAA55AA55,				0x06, 1
    LOGIC_RA_RS_CHECK		subfmeo., REGS_AS1,	0xAA55AA55,				0x07, 0

    LOGIC_RA_RS_CHECK		addze,    REGS_AS1,	0xAA55AA55,				0x08, 0
    LOGIC_RA_RS_CHECK		addze.,   REGS_AS1,	0xAA55AA55,				0x09, 1
    LOGIC_RA_RS_CHECK		addzeo,   REGS_AS1,	0xAA55AA55,				0x0A, 0
    LOGIC_RA_RS_CHECK		addzeo.,  REGS_AS1,	0xAA55AA55,				0x0B, 1

    LOGIC_RA_RS_CHECK		subfze,   REGS_AS1,	0xAA55AA55,				0x0C, 1
    LOGIC_RA_RS_CHECK		subfze.,  REGS_AS1,	0xAA55AA55,				0x0D, 0
    LOGIC_RA_RS_CHECK		subfzeo,  REGS_AS1,	0xAA55AA55,				0x0E, 1
    LOGIC_RA_RS_CHECK		subfzeo., REGS_AS1,	0xAA55AA55,				0x0F, 0

    LOGIC_RA_RS_CHECK		neg,      REGS_AS1,	0xAA55AA55,				0x10, 0
    LOGIC_RA_RS_CHECK		neg.,     REGS_AS1,	0xAA55AA55,				0x11, 0

    load_addr   r15,    save_data
    load_const  r16,    DUMP_SIZE
    _rumboot_issdump    r15, r16

    b test_ok


test_error:
    rumboot_putstring   "TEST ERROR\n"
    load_const  r3,     RESULT_ERROR
    b finish

test_ok:
    rumboot_putstring   "TEST OK\n"
    load_const  r3,     RESULT_OK

finish:
	blr


.section ".issdata","aw",@progbits

save_data:



