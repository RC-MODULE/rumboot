
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_config.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/trace.S.h>


/* check macro for operations with mask "opcode RA, RS, UI" */
.macro LOGIC_RA_RS_UI_CHECK opcode, ra_no, rs_no, rs_val, ui_val, info_reg_no, comand_conventional_no, temp_reg_no, r_result_address
	load_const \info_reg_no, 0x00000000 //clear info register

	/* put mask type */
	ori \info_reg_no, \info_reg_no, 0x1
	load_const \temp_reg_no, 0x4
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put self-invented comand code into info reg */
	ori \info_reg_no, \info_reg_no, \comand_conventional_no
	load_const \temp_reg_no, 0x5
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put RA reg number into info reg */
	ori \info_reg_no, \info_reg_no, \ra_no
	load_const \temp_reg_no, 0x5
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put RS reg number into info reg */
	ori \info_reg_no, \info_reg_no, \rs_no
	load_const \temp_reg_no, 0x10
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put UI const into info reg */
	ori \info_reg_no, \info_reg_no, \ui_val

	/* load values into reg with number rs_no */
    load_const \rs_no, \rs_val

	/* perform operation */
    \opcode \ra_no, \rs_no, \ui_val
    stwu \ra_no, 4(\r_result_address)
    /* raise event for verilog */
    test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_18

	/* no-op for syncronization purpose */
    xori 0,0,0
.endm


/* check macro for operations with mask "opcode RA, RS, RB" */
.macro LOGIC_RA_RS_RB_CHECK opcode, ra_no, rs_no, rb_no, rs_val, rb_val, info_reg_no, comand_conventional_no, temp_reg_no, r_result_address
	/* clear info register */
	load_const \info_reg_no, 0x00000000

	/* put mask tyoe */
	ori \info_reg_no, \info_reg_no, 0x2
	load_const \temp_reg_no, 0x6
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put self-invented comand code into info reg */
	ori \info_reg_no, \info_reg_no, \comand_conventional_no
	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put RA reg number into info reg */
	ori \info_reg_no, \info_reg_no, \ra_no
	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put RS reg number into info reg */
	ori \info_reg_no, \info_reg_no, \rs_no
	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put RB reg number into info reg */
	ori \info_reg_no, \info_reg_no, \rb_no

    /* load values into reg with number rs_no */
    load_const \rs_no, \rs_val
    /* load values into reg with number rb_no */
    load_const \rb_no, \rb_val

    /* perform operation */
    \opcode \ra_no, \rs_no, \rb_no
    stwu \ra_no, 4(\r_result_address)
    /* raise event for verilog */
    test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_18
    /* no-op for syncronization purpose */
    xori 0,0,0
.endm


/* check macro for operations with mask "opcode RA, RS" */
.macro LOGIC_RA_RS_CHECK opcode, ra_no, rs_no, rs_val, info_reg_no, comand_conventional_no, temp_reg_no, r_result_address
	/* clear info register */
	load_const \info_reg_no, 0x00000000

	/* put mask tyoe */
	ori \info_reg_no, \info_reg_no, 0x3
	load_const \temp_reg_no, 0x6
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put self-invented comand code into info reg */
	ori \info_reg_no, \info_reg_no, \comand_conventional_no
	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put RA reg number into info reg */
	ori \info_reg_no, \info_reg_no, \ra_no
	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	/* put RS reg number into info reg */
	ori \info_reg_no, \info_reg_no, \rs_no
	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

    /* load values into reg with number rs_no */
    load_const \rs_no, \rs_val

    /* perform operation */
    \opcode \ra_no, \rs_no
    stwu \ra_no, 4(\r_result_address)
    /* raise event for verilog */
    test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_18
    /* no-op for syncronization purpose */
    xori 0,0,0
.endm

.macro sw_comparator_macro reg_first_start, reg_second_start, reg_byte_count, reg_tmp_offset, reg_tmp_data_first, reg_tmp_data_second, reg_result
    load_const \reg_tmp_offset, 0
    load_const \reg_result, 1
    li \reg_tmp_offset, 4
5: /* labels 1-3 used in trace_msg, label 4 used in test_assert */
        lwzx \reg_tmp_data_first, \reg_first_start, \reg_tmp_offset
        lwzx \reg_tmp_data_second, \reg_second_start, \reg_tmp_offset
        cmplw cr2, \reg_tmp_data_first, \reg_tmp_data_second
        beq cr2, 6f
        load_const \reg_result, 0

        rumboot_putstring "data mismatch at  offset: 0x"
        rumboot_puthex \reg_tmp_offset
        rumboot_putstring "data in the first  array: 0x"
        rumboot_puthex \reg_tmp_data_first
        rumboot_putstring "data in the second array: 0x"
        rumboot_puthex \reg_tmp_data_second
6:
    addi \reg_tmp_offset, \reg_tmp_offset, 4
    cmpw cr3, \reg_byte_count, \reg_tmp_offset
    bge cr3, 5b
.endm


/* .section ".IM0.text","ax",@progbits */ /* old def??? */

.global main

.section ".text","ax",@progbits
main:

/*  load_const r3, 0x0004FFFC */
    load_addr r3, result_data
    addi r3,r3,-4
    rumboot_puthex r3
    load_const r4, 0x0004FFFC
    load_const r5, 0
.rept 128
    stwu r5, 4(r4)
.endr
    load_const r4, 0x0004FFFC
.rept 128
    lwz r5, 4(r4)
.endr

/*	LOGIC_RA_RS_UI_CHECK  opcode, ra_no, rs_no, rs_val,     ui_val, info_reg_no, comand_conventional_no, temp_reg_no, address_reg */
    LOGIC_RA_RS_UI_CHECK  andi.,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x0,                   29,            3
    LOGIC_RA_RS_UI_CHECK  andis., 7,     6,     0xAA55AA55, 0x5A5A, 30,           0x1,                   29,            3
    LOGIC_RA_RS_UI_CHECK  ori,    7,     6,     0xAA55AA55, 0x5A5A, 30,           0x2,                   29,            3
    LOGIC_RA_RS_UI_CHECK  oris,   7,     6,     0xAA55AA55, 0x5A5A, 30,           0x3,                   29,            3
    LOGIC_RA_RS_UI_CHECK  xori,   7,     6,     0xAA55AA55, 0x5A5A, 30,           0x4,                   29,            3
    LOGIC_RA_RS_UI_CHECK  xoris,  7,     6,     0xAA55AA55, 0x5A5A, 30,           0x5,                   29,            3

/*	LOGIC_RA_RS_RB_CHECK      opcode, ra_no, rs_no, rb_no, rs_val,     rb_val,     info_reg_no, comand_conventional_no, temp_reg_no,   address_reg */
    LOGIC_RA_RS_RB_CHECK      and,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x0,                    29,            3
    LOGIC_RA_RS_RB_CHECK      and.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x1,                    29,            3
    LOGIC_RA_RS_RB_CHECK      or,     8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x2,                    29,            3
    LOGIC_RA_RS_RB_CHECK      or.,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x3,                    29,            3
    LOGIC_RA_RS_RB_CHECK      xor,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x4,                    29,            3
    LOGIC_RA_RS_RB_CHECK      xor.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x5,                    29,            3
    LOGIC_RA_RS_RB_CHECK      nand,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x6,                    29,            3
    LOGIC_RA_RS_RB_CHECK      nand.,  8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x7,                    29,            3
    LOGIC_RA_RS_RB_CHECK      nor,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x8,                    29,            3
    LOGIC_RA_RS_RB_CHECK      nor.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x9,                    29,            3
    LOGIC_RA_RS_RB_CHECK      eqv,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0xA,                    29,            3
    LOGIC_RA_RS_RB_CHECK      eqv.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0xB,                    29,            3
    LOGIC_RA_RS_RB_CHECK      andc,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0xC,                    29,            3
    LOGIC_RA_RS_RB_CHECK      andc.,  8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0xD,                    29,            3
    LOGIC_RA_RS_RB_CHECK      orc,    8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0xE,                    29,            3
    LOGIC_RA_RS_RB_CHECK      orc.,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0xF,                    29,            3
    LOGIC_RA_RS_RB_CHECK      cmpb,   8,     6,     7,     0xAA55AA55, 0x5A5A5A5A, 30,          0x10,                   29,            3

/*	LOGIC_RA_RS_CHECK   opcode,  ra_no, rs_no, rs_val,     info_reg_no, comand_conventional_no, temp_reg_no, address_reg */
    LOGIC_RA_RS_CHECK   extsb,   7,     6,     0xAA55AA55, 30,          0x0,                    29,            3
    LOGIC_RA_RS_CHECK   extsb.,  7,     6,     0xAA55AA55, 30,          0x1,                    29,            3
    LOGIC_RA_RS_CHECK   extsh,   7,     6,     0xAA55AA55, 30,          0x2,                    29,            3
    LOGIC_RA_RS_CHECK   extsh.,  7,     6,     0xAA55AA55, 30,          0x3,                    29,            3
    LOGIC_RA_RS_CHECK   cntlzw,  7,     6,     0xAA55AA55, 30,          0x4,                    29,            3
    LOGIC_RA_RS_CHECK   cntlzw., 7,     6,     0xAA55AA55, 30,          0x5,                    29,            3
    LOGIC_RA_RS_CHECK   prtyw,   7,     6,     0xAA55AA55, 30,          0x6,                    29,            3

    load_addr r3, result_data
    load_addr  r4, iss_mem
    load_const r5, 30
	bl compare_data
	cmpwi cr0, r3, 0x1

/*	.macro sw_comparator_macro reg_first_start, reg_second_start, reg_byte_count, reg_tmp_offset, reg_tmp_data_first, reg_tmp_data_second, reg_result */
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

test_error:
    rumboot_putstring "TEST ERROR\n"
    test_event		EVENT_ERROR
    load_const r3,	RESULT_ERROR

test_ok:
	rumboot_putstring "TEST OK\n"
	test_event		EVENT_OK
	load_const r3,	RESULT_OK
	b finish

finish:
	blr


.section ".text", "aw", @progbits
result_data:
.rept 512
    .long 0
.endr
