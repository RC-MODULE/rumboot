/*#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

int main()
{
#if 0
    rumboot_print_logo();
    rumboot_printf("rumboot: spl start: 0x%x end: 0x%x\n",
               &rumboot_platform_spl_start,
               &rumboot_platform_spl_end);
    rumboot_printf("test_oi10_cpu_004\n\n");
#endif
    return 0;
}

#include <test_event_asm.h>
#include "test_cpu_0013.h"
#include "iss_mem_dmp.h"
#include <mivem_mem_map.h>
#include <mpic128/mpic128_regs.h>
#include <sw_comparator.S.h>


//.section ".IM0.text","ax",@progbits

//.global image_start
.global main

//image_start:
main:
.section ".ROM.text","ax",@progbits
    //LOGIC_RA_RS_UI_CHECK opcode, ra_no, rs_no, rs_val,     ui_val, info_reg_no, comand_conventional_no, temp_reg_no, address_reg
//    load_const r3, 0x0004FFFC
    load_addr r3, result_data
    addi r3,r3,-4
    //trace_hex r3
    load_const r4, 0x0004FFFC
    load_const r5, 0
.rept 128
    stwu r5, 4(r4)
.endr
    load_const r4, 0x0004FFFC
.rept 128
    lwz r5, 4(r4)
.endr

    LOGIC_RA_RS_UI_CHECK  andi.,  1,     0,     0xAA55AA55, 0x5A5A, 30,           0x0,                   29,            3
    LOGIC_RA_RS_UI_CHECK  andis., 1,     0,     0xAA55AA55, 0x5A5A, 30,           0x1,                   29,            3
    LOGIC_RA_RS_UI_CHECK  ori,    1,     0,     0xAA55AA55, 0x5A5A, 30,           0x2,                   29,            3
    LOGIC_RA_RS_UI_CHECK  oris,   1,     0,     0xAA55AA55, 0x5A5A, 30,           0x3,                   29,            3
    LOGIC_RA_RS_UI_CHECK  xori,   1,     0,     0xAA55AA55, 0x5A5A, 30,           0x4,                   29,            3
    LOGIC_RA_RS_UI_CHECK  xoris,  1,     0,     0xAA55AA55, 0x5A5A, 30,           0x5,                   29,            3

    //LOGIC_RA_RS_RB_CHECK    opcode, ra_no, rs_no, rb_no, rs_val,     rb_val,     info_reg_no, comand_conventional_no, temp_reg_no, address_reg
    LOGIC_RA_RS_RB_CHECK      and,    2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x0,                    29,            3
    LOGIC_RA_RS_RB_CHECK      and.,   2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x1,                    29,            3
    LOGIC_RA_RS_RB_CHECK      or,     2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x2,                    29,            3
    LOGIC_RA_RS_RB_CHECK      or.,    2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x3,                    29,            3
    LOGIC_RA_RS_RB_CHECK      xor,    2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x4,                    29,            3
    LOGIC_RA_RS_RB_CHECK      xor.,   2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x5,                    29,            3
    LOGIC_RA_RS_RB_CHECK      nand,   2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x6,                    29,            3
    LOGIC_RA_RS_RB_CHECK      nand.,  2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x7,                    29,            3
    LOGIC_RA_RS_RB_CHECK      nor,    2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x8,                    29,            3
    LOGIC_RA_RS_RB_CHECK      nor.,   2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x9,                    29,            3
    LOGIC_RA_RS_RB_CHECK      eqv,    2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0xA,                    29,            3
    LOGIC_RA_RS_RB_CHECK      eqv.,   2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0xB,                    29,            3
    LOGIC_RA_RS_RB_CHECK      andc,   2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0xC,                    29,            3
    LOGIC_RA_RS_RB_CHECK      andc.,  2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0xD,                    29,            3
    LOGIC_RA_RS_RB_CHECK      orc,    2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0xE,                    29,            3
    LOGIC_RA_RS_RB_CHECK      orc.,   2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0xF,                    29,            3
    LOGIC_RA_RS_RB_CHECK      cmpb,   2,     0,     1,     0xAA55AA55, 0x5A5A5A5A, 30,          0x10,                   29,            3

    //LOGIC_RA_RS_CHECK  opcode,  ra_no, rs_no, rs_val,     info_reg_no, comand_conventional_no, temp_reg_no, address_reg
    LOGIC_RA_RS_CHECK   extsb,   1,     0,     0xAA55AA55, 30,          0x0,                    29,            3
    LOGIC_RA_RS_CHECK   extsb.,  1,     0,     0xAA55AA55, 30,          0x1,                    29,            3
    LOGIC_RA_RS_CHECK   extsh,   1,     0,     0xAA55AA55, 30,          0x2,                    29,            3
    LOGIC_RA_RS_CHECK   extsh.,  1,     0,     0xAA55AA55, 30,          0x3,                    29,            3
    LOGIC_RA_RS_CHECK   cntlzw,  1,     0,     0xAA55AA55, 30,          0x4,                    29,            3
    LOGIC_RA_RS_CHECK   cntlzw., 1,     0,     0xAA55AA55, 30,          0x5,                    29,            3
    LOGIC_RA_RS_CHECK   prtyw,   1,     0,     0xAA55AA55, 30,          0x6,                    29,            3

    load_addr r3, result_data
    load_addr  r4, iss_mem
    load_const r5, 30
//    bl compare_data
//    cmpwi cr0, r3, 0x1

//.macro sw_comparator_macro reg_first_start, reg_second_start, reg_byte_count, reg_tmp_offset, reg_tmp_data_first, reg_tmp_data_second, reg_result
    sw_comparator_macro r3, r4, r5, r6, r7, r8, r9
    cmpwi cr0, r9, 0x1

    beq cr0, test_ok

error:
    test_event EVENT_ERROR


test_ok:
     test_event EVENT_OK
     b finish

finish:
    b finish

.section ".IM0.data", "aw", @progbits
result_data:
.rept 512
    .long 0
.endr
*/
