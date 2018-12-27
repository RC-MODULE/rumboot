
#include <platform/test_event_asm.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/trace.S.h>
#include <platform/devices.h>


#define SAVE_ADDR       save_data
#define SAVE_MEM_SIZE   0x0100

#define TMP_REG		4
#define INF_REG		5
#define REGS_AS 	7,6
#define REGS_ASB	8,6,7

#define UI_VAL		0x5A5A
#define RS_VAL		0xAA55AA55
#define RB_VAL		0x5A5A5A5A

#define ppc_nop xori 0,0,0


/* check macro for operations with mask "opcode RA, RS, UI" */
.macro LOGIC_RA_RS_UI_CHECK	opcode,     \
							ra_reg,     \
							rs_reg,     \
							rs_val,     \
							ui_val,     \
							cmd_cnv,    \
							r_res_addr

	rumboot_putstring "\opcode ... "
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
	ori			INF_REG,	INF_REG, \ra_reg
	load_const	TMP_REG,	0x05
	slw			INF_REG,	INF_REG, TMP_REG

	/* put RS reg number into info reg */
	ori			INF_REG,	INF_REG, \rs_reg
	load_const	TMP_REG,	0x10
	slw			INF_REG,	INF_REG, TMP_REG

	/* put UI const into info reg */
	ori			INF_REG,	INF_REG, \ui_val

	/* load values into reg with number rs_no */
    load_const	\rs_reg,	\rs_val

	/* perform operation */
    \opcode		\ra_reg,	\rs_reg, \ui_val
    stwu		\ra_reg,	4(\r_res_addr)

	/* no-op for syncronization purpose */
    ppc_nop
	rumboot_putstring "\tdone.\n"
.endm


/* check macro for operations with mask "opcode RA, RS, RB" */
.macro LOGIC_RA_RS_RB_CHECK	opcode,    \
							ra_reg,    \
							rs_reg,    \
							rb_reg,    \
							rs_val,    \
							rb_val,    \
							cmd_cnv,   \
							r_res_addr

	rumboot_putstring "\opcode ... "
	/* clear info register */
	load_const	INF_REG,	0x00000000

	/* put mask tyoe */
	ori			INF_REG,	INF_REG,	0x02
	load_const	TMP_REG,	0x06
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put self-invented comand code into info reg */
	ori			INF_REG,	INF_REG,	\cmd_cnv
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put RA reg number into info reg */
	ori			INF_REG,	INF_REG,	\ra_reg
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put RS reg number into info reg */
	ori			INF_REG,	INF_REG,	\rs_reg
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put RB reg number into info reg */
	ori			INF_REG,	INF_REG,	\rb_reg

    /* load values into reg with number rs_no */
    load_const	\rs_reg,	\rs_val
    /* load values into reg with number rb_no */
    load_const	\rb_reg,	\rb_val

    /* perform operation */
    \opcode		\ra_reg,	\rs_reg,	\rb_reg
    stwu		\ra_reg,	4(\r_res_addr)
    /* no-op for syncronization purpose */
    ppc_nop
	rumboot_putstring "\tdone.\n"
.endm


/* check macro for operations with mask "opcode RA, RS" */
.macro LOGIC_RA_RS_CHECK	opcode,    \
							ra_reg,    \
							rs_reg,    \
							rs_val,    \
							cmd_cnv,   \
							r_res_addr

	rumboot_putstring "\opcode ... "
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
	ori			INF_REG,	INF_REG,	\ra_reg
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

	/* put RS reg number into info reg */
	ori			INF_REG,	INF_REG,	\rs_reg
	load_const	TMP_REG,	0x08
	slw			INF_REG,	INF_REG,	TMP_REG

    /* load values into reg with number rs_no */
    load_const	\rs_reg,	\rs_val

    /* perform operation */
    \opcode		\ra_reg,	\rs_reg
    stwu		\ra_reg,	4(\r_res_addr)
    /* no-op for syncronization purpose */
    ppc_nop
	rumboot_putstring "\tdone.\n"
.endm

.macro sw_compare	reg_start1,     \
					reg_start2,     \
					reg_byte_count, \
					reg_tmp_offset, \
					reg_tmp_data1,  \
					reg_tmp_data2,  \
					reg_result

	rumboot_putstring "Comparing data... "
    load_const	\reg_tmp_offset,	0x00000004
    load_const	\reg_result,		0x00000001
5: /* labels 1-3 used in trace_msg, label 4 used in test_assert */
	lwzx		\reg_tmp_data1,	\reg_start1,	\reg_tmp_offset
	lwzx		\reg_tmp_data2,	\reg_start2,	\reg_tmp_offset
	cmplw cr2,	\reg_tmp_data1,	\reg_tmp_data2
	beq+  cr2,	6f
	load_const	\reg_result,		0x00000000

	rumboot_putstring	"\ndata mismatch!\n"
	mtspr		SPR_SPRG5,		\reg_tmp_offset
	mtspr		SPR_SPRG6,		\reg_tmp_data1
	mtspr		SPR_SPRG7,		\reg_tmp_data2

6:
	addi		\reg_tmp_offset, \reg_tmp_offset, 0x0004
	cmpw cr3,	\reg_byte_count, \reg_tmp_offset
	bge  cr3,	5b
	rumboot_putstring "done!\n"
.endm


.section ".data","aw",@nobits

.align 4
save_data:
    .space SAVE_MEM_SIZE, 0x00


.section ".data","aw",@progbits

.align 4
event_data:
    .long TEST_EVENT_CREATE_ISS_DUMP
    .long save_data
    .long SAVE_MEM_SIZE
#define EVENT_DATA_SIZE 3


.global main

.section ".text","ax",@progbits
main:
    mflr        r3
    mtspr       SPR_VRSAVE, r3

    test_event_send_test_id "test_iss_base"
    rumboot_putstring "Start testing...\n"
    load_addr   r3, SAVE_ADDR - 4

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

    load_const  r3, EVENT_TESTEVENT
    load_addr   r4, event_data
    load_const  r5, EVENT_DATA_SIZE
    bl          rumboot_platform_event_raise

    b test_ok

test_ok:
    rumboot_putstring "TEST OK\n"
    load_const  r3, RESULT_OK
    b finish

test_error:
    rumboot_putstring "TEST ERROR\n"
    load_const  r3, RESULT_ERROR

finish:
    mfspr       r5, SPR_VRSAVE
    mtlr        r5
    blr
