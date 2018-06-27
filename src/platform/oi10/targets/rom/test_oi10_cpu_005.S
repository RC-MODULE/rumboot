
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/regs/regs_mpic.h>

// #include <sw_comparator.S.h>
#ifndef SW_COMPARATOR_S_H_
#define SW_COMPARATOR_S_H_


.macro sw_comparator_macro reg_first_start, reg_second_start, reg_byte_count, reg_tmp_offset, reg_tmp_data_first, reg_tmp_data_second, reg_result
    load_const \reg_tmp_offset, 0
    load_const \reg_result, 1
    li \reg_tmp_offset, 4
5: //labels 1-3 used in rumboot_putstring, label 4 used in test_assert
        lwzx \reg_tmp_data_first, \reg_first_start, \reg_tmp_offset
        lwzx \reg_tmp_data_second, \reg_second_start, \reg_tmp_offset
        cmplw cr2, \reg_tmp_data_first, \reg_tmp_data_second
        beq cr2, 6f
        load_const \reg_result, 0
/*
        rumboot_putstring "data mismatch at  offset: 0x"
        trace_hex \reg_tmp_offset
        rumboot_putstring "data in the first  array: 0x"
        trace_hex \reg_tmp_data_first
        rumboot_putstring "data in the second array: 0x"
        trace_hex \reg_tmp_data_second
*/
6:
    addi \reg_tmp_offset, \reg_tmp_offset, 4
    cmpw cr3, \reg_byte_count, \reg_tmp_offset
    bge cr3, 5b
.endm

#endif /* SW_COMPARATOR_S_H_ */

// #include "iss_mem_dmp.h"
/*
 * iss_mem_dmp.h
 *
 *  Created on: Dec 11, 2017
 *      Author: v.strukov
 */

#ifndef ISS_MEM_DMP_H_
#define ISS_MEM_DMP_H_

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

#endif /* ISS_MEM_DMP_H_ */
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
00050090  04B004AF 04B004AF 04B004AF B004B0s05
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


// #include "test_mpw_cpu_005_0.h"
#ifndef _TEST_CPU_005_H_
#define _TEST_CPU_005_H_

#include <platform/oi10/platform/arch/ppc/ppc_476fp_config.h>

// check macro for operations with mask "opcode RA, RS, UI"
.macro LOGIC_RA_RS_UI_CHECK opcode, ra_no, rs_no, rs_val, ui_val, info_reg_no, comand_conventional_no, temp_reg_no, result_address
	load_const \info_reg_no, 0x00000000 //clear info register
	load_const \ra_no, 0x00000000 //clear result register

	ori \info_reg_no, \info_reg_no, 0x1 //put mask tyoe
	load_const \temp_reg_no, 0x4
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \comand_conventional_no //put self-invented comand code into info reg
	load_const \temp_reg_no, 0x5
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \ra_no //put RA reg number into info reg
	load_const \temp_reg_no, 0x5
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \rs_no //put RS reg number into info reg
	load_const \temp_reg_no, 0x10
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \ui_val@l //put UI const into info reg

	load_const 0 0x00000000
	mtxer 0
	load_const \rs_no, \rs_val //load values into reg with number rs_no

	\opcode \ra_no, \rs_no, \ui_val@l //perform operation
	stwu \ra_no, 4(\result_address)
	test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_17  //raise event for verilog
	xori 0,0,0	//for syncronization purpose
.endm

// check macro for operations with mask "opcode RA, RS, RB"
.macro LOGIC_RA_RS_RB_CHECK opcode, ra_no, rs_no, rb_no, rs_val, rb_val, info_reg_no, comand_conventional_no, temp_reg_no, set_CA_bit, result_address //
	load_const \info_reg_no, 0x00000000 //clear info register

	ori \info_reg_no, \info_reg_no, 0x2 //put mask tyoe
	load_const \temp_reg_no, 0x6
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \comand_conventional_no //put self-invented comand code into info reg
	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \ra_no //put RA reg number into info reg
.if (\set_CA_bit != 0)
	ori \info_reg_no, \info_reg_no, 0x80
.endif

	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \rs_no //put RS reg number into info reg
	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \rb_no //put RB reg number into info reg

.if (\set_CA_bit == 0)
	load_const 0 0x00000000
.else
	load_const 0 0x20000000
.endif
	mtxer 0

	load_const \rs_no, \rs_val //load values into reg with number rs_no
	load_const \rb_no, \rb_val //load values into reg with number rb_no

	\opcode \ra_no, \rs_no, \rb_no //perform operation
	stwu \ra_no, 4(\result_address)
	test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_17 //raise event for verilog
	xori 0,0,0	//for syncronization purpose
.endm

// check macro for operations with mask "opcode RA, RS"
.macro LOGIC_RA_RS_CHECK opcode, ra_no, rs_no, rs_val, info_reg_no, comand_conventional_no, temp_reg_no, set_CA_bit, result_address
	load_const \info_reg_no, 0x00000000 //clear info register

	ori \info_reg_no, \info_reg_no, 0x3 //put mask tyoe
	load_const \temp_reg_no, 0x6
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \comand_conventional_no //put self-invented comand code into info reg
	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \ra_no //put RA reg number into info reg
.if (\set_CA_bit != 0)
	ori \info_reg_no, \info_reg_no, 0x80
.endif

	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

	ori \info_reg_no, \info_reg_no, \rs_no //put RS reg number into info reg
	load_const \temp_reg_no, 0x8
	slw \info_reg_no, \info_reg_no, \temp_reg_no

.if (\set_CA_bit == 0)
	load_const 0 0x00000000
.else
	load_const 0 0x20000000
.endif
	mtxer 0
load_const \rs_no, \rs_val //load values into reg with number rs_no

\opcode \ra_no, \rs_no //perform operation
stwu \ra_no, 4(\result_address)
test_event EVENT_CHECK_CPU_PPC_PROG_MODEL_17 //raise event for verilog
xori 0,0,0	//for syncronization purpose
.endm

#endif //_TEST_CPU_005_H_


.section ".text","ax",@progbits


.global main
main:
.global boot_main
boot_main:
test_mpw_cpu_005_0:

    load_const r3 0x0004FFFC
    load_const r0 0x00000000

    mtxer r0
//  LOGIC_RA_RS_UI_CHECK   opcode, ra_no, rs_no, rs_val,     ui_val, info_reg_no,  comand_no, temp_reg_no,       result_address
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

//  LOGIC_RA_RS_RB_CHECK    ( opcode,   ra_no, rs_no, rb_no, rs_val,     rb_val,          info_reg_no, comand_no,  temp_reg_no, set_CA_bit, result_address )
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
//  LOGIC_RA_RS_CHECK   opcode,       ra_no, rs_no, rs_val,        info_reg_no, comand_no,  temp_reg_no, set_CA_bit,    result_address)
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

/*
    test_setup_stack:
    // FIXME !AG
    load_const r3, MPICx_BASE+MPIC128x_WAI_PR
    mfdcrx r3, r3
    cmpwi r3, PPC0_CORE_ID
    beq test_setup_cpu0_stack
    //else
    load_addr r1, cpu1_stack_top
    b test_setup_cpu1_stack

    test_setup_cpu0_stack:
    
    load_addr r1, cpu0_stack_top

    test_setup_cpu1_stack:
    stwu r0, -8(r1)
*/
    load_const r3, 0x00050000
    load_addr r4, iss_mem
    load_const r5, 89
    bl compare_data
    cmpwi cr0, r3, 0x1
    beq cr0, finish_ok

test_event EVENT_ERROR

finish_error:
li r3, 0x01
b finish_error

//test_ok:
finish_ok:
test_event EVENT_OK
li r3, 0x00
b finish

finish:
blr
// b finish

// AG: Manualy generated by GCC from C code
compare_data:
        stwu r1,-16(r1)
        cmpwi cr0,r5,0
        li r8,1
        stw r30,8(r1)
        beq cr0,.compare_data_L1
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
        bgt cr7,.compare_data_L4
.compare_data_L1:
        mr r3,r8
        lwz r30,8(r1)
        addi r1,r1,16
        blr


