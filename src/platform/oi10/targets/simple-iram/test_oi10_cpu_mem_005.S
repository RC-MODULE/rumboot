#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/trace.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>


#define DCDBTRL 924
#define DCDBTRH 925

#define BankA0 (0x0 << 8)
#define BankB0 (0x1 << 8)
#define BankC0 (0x2 << 8)
#define BankD0 (0x3 << 8)
#define BankE0 (0x4 << 8)
#define BankF0 (0x5 << 8)
#define BankG0 (0x6 << 8)
#define BankH0 (0x7 << 8)

#define WAY0   (reg_field(18,0))
#define WAY1   (reg_field(18,1))
#define WAY2   (reg_field(18,2))
#define WAY3   (reg_field(18,3))

#define INDEX(index) \
    (reg_field(26,index))
#define WORD(word) \
    (reg_field(29,word))

.macro dcread_macro regT,regA
     msync
     dcread \regT, 0, \regA
     isync
.endm

.macro dcread_wm_macro regT,regA, gpr_btrl, gpr_btrh
     msync
     dcread \regT, 0, \regA
     isync
     mfspr \gpr_btrl, DCDBTRL
     mfspr \gpr_btrh, DCDBTRH
.endm

.macro data_run label,InitAddr0,InitAddr1,InitAddr2,InitAddr3,increment
    //r2 points to data array
    lis r2, dcu_data_array@h
    ori r2, r2, dcu_data_array@l
    //final counter value
    load_const r4, 15
    //r5 - initial address way 0/2
    load_const r5, \InitAddr0
    //r6 - counter 0..31
    load_const r6, 0x00000000
    //dcread target - r7 //don't care
    li r7, 0x0
    //r8 - dcdbtrl
    //r13 - expected dcdbtrl
    load_const r13,0x00000000
    //r10,r11,r9
    load_const r10, \InitAddr1
    load_const r11, \InitAddr2
    load_const r9, \InitAddr3

loop1_\label:
    //way 0 w0
    lwzu r3, 4(r2)  //load exp value
    //lwzu r13, 4(r2) //load exp datap
    dcread_macro r7,r9
    cmp cr7,0,r3,r7
    TEST_ASSERT(eq,cr7,"\label run err:Way1 Word1")
    //cmp cr7,0,r13,r8
    //TEST_ASSERT(eq,cr7,"\label run error (parity,way 0 word 0)")
    addi r9,r9,64//increment 16 words (+2 to index)

    //way 0 w1
    lwzu r3, 4(r2) //load exp value
    //lwzu r13, 4(r2) //load exp datap
    dcread_macro r7,r11
    cmp cr7,0,r3,r7
    TEST_ASSERT(eq,cr7,"\label run err:Way 1 Word0")
    //cmp cr7,0,r13,r8
    //TEST_ASSERT(eq,cr7,"\label run error (parity,way 0 word 1)")
    addi r11,r11,64//increment 16 words (+2 to index)

    //way 1 w0
    lwzu r3, 4(r2) //load exp value
    //lwzu r13, 4(r2) //load exp datap
    dcread_macro r7,r10
    cmp cr7,0,r3,r7
    TEST_ASSERT(eq,cr7,"\label run err:Way0 Word1")
    //cmp cr7,0,r13,r8
    //TEST_ASSERT(eq,cr7,"\label run error (parity,way 1 word 0)")
    addi r10,r10,64//increment 16 words (+2 to index)

    //way 1 w1
    lwzu r3, 4(r2) //load exp value
    //lwzu r13, 4(r2) //load exp datap
    dcread_macro r7,r5
    cmp cr7,0,r3,r7
    TEST_ASSERT(eq,cr7,"\label run err:Way0 Word0")
    //cmp cr7,0,r13,r8
    //TEST_ASSERT(eq,cr7,"\label run error (parity,way 1 word 1)")
    addi r5,r5,64//increment 16 words (+2 to index)

    cmpi cr7,0,r6,63
    beq cr7,update1_address_\label//switch even/odd index
    cmp cr7,0,r6,r4 //check counter
    beq cr7, end_loop1_\label

    addi r6,r6,1 //increment counter +1
    b loop1_\label
update1_address_\label: //increment indexes +20 or +40
    addi r5, r5, \increment
    addi r10, r10, \increment
    addi r11, r11, \increment
    addi r9, r9, \increment
    addi r6,r6,1 //increment counter +1
    b loop1_\label

end_loop1_\label:
    rumboot_putstring "\label run OK\n"
.endm

.macro data_addr label,addr0,addr1,addr2,addr3,exp_val
    //addr
    load_const r5, \addr0 //WAY 0, word 0
    load_const r10, \addr1 //WAY 0, word 1
    load_const r11, \addr2 //WAY 1, word 0
    load_const r9, \addr3 //WAY 1, word 1
    //r3 - exp val (volatile)
    load_const r3, \exp_val
    //r4 - exp val (constant, =0)
    load_const r4, 0x00000000
    //r7 - data
    load_const r7, 0x00000000

    dcread_macro r7,r9
    cmp cr7,0,r7,r3
    TEST_ASSERT(eq,cr7,"\label Abus:Way1 Word1, exp:\exp_val")

    dcread_macro r7,r11
    cmp cr7,0,r7,r4
    TEST_ASSERT(eq,cr7,"\label Abus:Way1 Word0, exp:\exp_val")

    dcread_macro r7,r10
    cmp cr7,0,r7,r4
    TEST_ASSERT(eq,cr7,"\label Abus:Way0 Word1, exp:\exp_val")

    dcread_macro r7,r5
    cmp cr7,0,r7,r4
    TEST_ASSERT(eq,cr7,"\label Abus:Way0 Word0, exp:\exp_val")
.endm



.section ".text","ax",@progbits

.global main

main:
test_oi10_cpu_mem_005:

    test_event_send_test_id "test_oi10_cpu_mem_005"

DCU_DATA_CHECK:
    //send to tb that ready for dcu fillilg
    test_event EVENT_DCU_DATA_FILL_1

    data_run Array_A,0x00000000 | WAY0, 0x00000004 | WAY0, 0x00000000 | WAY1, 0x00000004 | WAY1, 0x20
    data_run Array_B,0x00000008 | WAY0, 0x0000000C | WAY0, 0x00000008 | WAY1, 0x0000000C | WAY1, 0x20
    data_run Array_C,0x00000010 | WAY0, 0x00000014 | WAY0, 0x00000010 | WAY1, 0x00000014 | WAY1, 0x20
    data_run Array_D,0x00000018 | WAY0, 0x0000001C | WAY0, 0x00000018 | WAY1, 0x0000001C | WAY1, 0x20
    data_run Array_E,0x00000020 | WAY0, 0x00000024 | WAY0, 0x00000020 | WAY1, 0x00000024 | WAY1, -0x20
    data_run Array_F,0x00000028 | WAY0, 0x0000002C | WAY0, 0x00000028 | WAY1, 0x0000002C | WAY1, -0x20
    data_run Array_G,0x00000030 | WAY0, 0x00000034 | WAY0, 0x00000030 | WAY1, 0x00000034 | WAY1, -0x20
    data_run Array_H,0x00000038 | WAY0, 0x0000003C | WAY0, 0x00000038 | WAY1, 0x0000003C | WAY1, -0x20

    test_event EVENT_DCU_DATA_FILL_ADDR

    //Array A run
    data_addr Data_abus_A,   WAY2 | INDEX(0xFF) | WORD(4),WAY2 | INDEX(0xFF) | WORD(5),WAY3 | INDEX(0xFF) | WORD(4),WAY3 | INDEX(0xFF) | WORD(5),BankA0 | 0xFF
    data_addr Data_abus_A,   WAY0 | INDEX(0x00) | WORD(0),WAY0 | INDEX(0x00) | WORD(1),WAY1 | INDEX(0x00) | WORD(0),WAY1 | INDEX(0x00) | WORD(1),BankA0 | 0x00
    data_addr Data_abus_A,   WAY0 | INDEX(0x8F) | WORD(0),WAY0 | INDEX(0x8F) | WORD(1),WAY1 | INDEX(0x8F) | WORD(0),WAY1 | INDEX(0x8F) | WORD(1),BankA0 | 0xF0
    data_addr Data_abus_A,   WAY2 | INDEX(0x70) | WORD(4),WAY2 | INDEX(0x70) | WORD(5),WAY3 | INDEX(0x70) | WORD(4),WAY3 | INDEX(0x70) | WORD(5),BankA0 | 0x0F
    data_addr Data_abus_A,   WAY0 | INDEX(0xB3) | WORD(0),WAY0 | INDEX(0xB3) | WORD(1),WAY1 | INDEX(0xB3) | WORD(0),WAY1 | INDEX(0xB3) | WORD(1),BankA0 | 0xCC
    data_addr Data_abus_A,   WAY2 | INDEX(0x4C) | WORD(4),WAY2 | INDEX(0x4C) | WORD(5),WAY3 | INDEX(0x4C) | WORD(4),WAY3 | INDEX(0x4C) | WORD(5),BankA0 | 0x33
    data_addr Data_abus_A,   WAY2 | INDEX(0x95) | WORD(4),WAY2 | INDEX(0x95) | WORD(5),WAY3 | INDEX(0x95) | WORD(4),WAY3 | INDEX(0x95) | WORD(5),BankA0 | 0xAA
    data_addr Data_abus_A,   WAY0 | INDEX(0x6A) | WORD(0),WAY0 | INDEX(0x6A) | WORD(1),WAY1 | INDEX(0x6A) | WORD(0),WAY1 | INDEX(0x6A) | WORD(1),BankA0 | 0x55
    rumboot_putstring "Abus_A check OK\n"

    //Array B run
    data_addr Data_abus_B,   WAY2 | INDEX(0xFF) | WORD(6),WAY2 | INDEX(0xFF) | WORD(7),WAY3 | INDEX(0xFF) | WORD(6),WAY3 | INDEX(0xFF) | WORD(7),BankB0 | 0xFF
    data_addr Data_abus_B,   WAY0 | INDEX(0x00) | WORD(2),WAY0 | INDEX(0x00) | WORD(3),WAY1 | INDEX(0x00) | WORD(2),WAY1 | INDEX(0x00) | WORD(3),BankB0 | 0x00
    data_addr Data_abus_B,   WAY0 | INDEX(0x8F) | WORD(2),WAY0 | INDEX(0x8F) | WORD(3),WAY1 | INDEX(0x8F) | WORD(2),WAY1 | INDEX(0x8F) | WORD(3),BankB0 | 0xF0
    data_addr Data_abus_B,   WAY2 | INDEX(0x70) | WORD(6),WAY2 | INDEX(0x70) | WORD(7),WAY3 | INDEX(0x70) | WORD(6),WAY3 | INDEX(0x70) | WORD(7),BankB0 | 0x0F
    data_addr Data_abus_B,   WAY0 | INDEX(0xB3) | WORD(2),WAY0 | INDEX(0xB3) | WORD(3),WAY1 | INDEX(0xB3) | WORD(2),WAY1 | INDEX(0xB3) | WORD(3),BankB0 | 0xCC
    data_addr Data_abus_B,   WAY2 | INDEX(0x4C) | WORD(6),WAY2 | INDEX(0x4C) | WORD(7),WAY3 | INDEX(0x4C) | WORD(6),WAY3 | INDEX(0x4C) | WORD(7),BankB0 | 0x33
    data_addr Data_abus_B,   WAY2 | INDEX(0x95) | WORD(6),WAY2 | INDEX(0x95) | WORD(7),WAY3 | INDEX(0x95) | WORD(6),WAY3 | INDEX(0x95) | WORD(7),BankB0 | 0xAA
    data_addr Data_abus_B,   WAY0 | INDEX(0x6A) | WORD(2),WAY0 | INDEX(0x6A) | WORD(3),WAY1 | INDEX(0x6A) | WORD(2),WAY1 | INDEX(0x6A) | WORD(3),BankB0 | 0x55
    rumboot_putstring "Abus_B check OK\n"

    //Array C run
    data_addr Data_abus_C,   WAY0 | INDEX(0xFF) | WORD(4),WAY0 | INDEX(0xFF) | WORD(5),WAY1 | INDEX(0xFF) | WORD(4),WAY1 | INDEX(0xFF) | WORD(5),BankC0 | 0xFF
    data_addr Data_abus_C,   WAY2 | INDEX(0x00) | WORD(0),WAY2 | INDEX(0x00) | WORD(1),WAY3 | INDEX(0x00) | WORD(0),WAY3 | INDEX(0x00) | WORD(1),BankC0 | 0x00
    data_addr Data_abus_C,   WAY2 | INDEX(0x8F) | WORD(0),WAY2 | INDEX(0x8F) | WORD(1),WAY3 | INDEX(0x8F) | WORD(0),WAY3 | INDEX(0x8F) | WORD(1),BankC0 | 0xF0
    data_addr Data_abus_C,   WAY0 | INDEX(0x70) | WORD(4),WAY0 | INDEX(0x70) | WORD(5),WAY1 | INDEX(0x70) | WORD(4),WAY1 | INDEX(0x70) | WORD(5),BankC0 | 0x0F
    data_addr Data_abus_C,   WAY2 | INDEX(0xB3) | WORD(0),WAY2 | INDEX(0xB3) | WORD(1),WAY3 | INDEX(0xB3) | WORD(0),WAY3 | INDEX(0xB3) | WORD(1),BankC0 | 0xCC
    data_addr Data_abus_C,   WAY0 | INDEX(0x4C) | WORD(4),WAY0 | INDEX(0x4C) | WORD(5),WAY1 | INDEX(0x4C) | WORD(4),WAY1 | INDEX(0x4C) | WORD(5),BankC0 | 0x33
    data_addr Data_abus_C,   WAY0 | INDEX(0x95) | WORD(4),WAY0 | INDEX(0x95) | WORD(5),WAY1 | INDEX(0x95) | WORD(4),WAY1 | INDEX(0x95) | WORD(5),BankC0 | 0xAA
    data_addr Data_abus_C,   WAY2 | INDEX(0x6A) | WORD(0),WAY2 | INDEX(0x6A) | WORD(1),WAY3 | INDEX(0x6A) | WORD(0),WAY3 | INDEX(0x6A) | WORD(1),BankC0 | 0x55
    rumboot_putstring "Abus_C check OK\n"

    //Array D run
    data_addr Data_abus_D,   WAY0 | INDEX(0xFF) | WORD(6),WAY0 | INDEX(0xFF) | WORD(7),WAY1 | INDEX(0xFF) | WORD(6),WAY1 | INDEX(0xFF) | WORD(7),BankD0 | 0xFF
    data_addr Data_abus_D,   WAY2 | INDEX(0x00) | WORD(2),WAY2 | INDEX(0x00) | WORD(3),WAY3 | INDEX(0x00) | WORD(2),WAY3 | INDEX(0x00) | WORD(3),BankD0 | 0x00
    data_addr Data_abus_D,   WAY2 | INDEX(0x8F) | WORD(2),WAY2 | INDEX(0x8F) | WORD(3),WAY3 | INDEX(0x8F) | WORD(2),WAY3 | INDEX(0x8F) | WORD(3),BankD0 | 0xF0
    data_addr Data_abus_D,   WAY0 | INDEX(0x70) | WORD(6),WAY0 | INDEX(0x70) | WORD(7),WAY1 | INDEX(0x70) | WORD(6),WAY1 | INDEX(0x70) | WORD(7),BankD0 | 0x0F
    data_addr Data_abus_D,   WAY2 | INDEX(0xB3) | WORD(2),WAY2 | INDEX(0xB3) | WORD(3),WAY3 | INDEX(0xB3) | WORD(2),WAY3 | INDEX(0xB3) | WORD(3),BankD0 | 0xCC
    data_addr Data_abus_D,   WAY0 | INDEX(0x4C) | WORD(6),WAY0 | INDEX(0x4C) | WORD(7),WAY1 | INDEX(0x4C) | WORD(6),WAY1 | INDEX(0x4C) | WORD(7),BankD0 | 0x33
    data_addr Data_abus_D,   WAY0 | INDEX(0x95) | WORD(6),WAY0 | INDEX(0x95) | WORD(7),WAY1 | INDEX(0x95) | WORD(6),WAY1 | INDEX(0x95) | WORD(7),BankD0 | 0xAA
    data_addr Data_abus_D,   WAY2 | INDEX(0x6A) | WORD(2),WAY2 | INDEX(0x6A) | WORD(3),WAY3 | INDEX(0x6A) | WORD(2),WAY3 | INDEX(0x6A) | WORD(3),BankD0 | 0x55
    rumboot_putstring "Abus_D check OK\n"

    //Array E run
    data_addr Data_abus_E,   WAY2 | INDEX(0x7F) | WORD(4),WAY2 | INDEX(0x7F) | WORD(5),WAY3 | INDEX(0x7F) | WORD(4),WAY3 | INDEX(0x7F) | WORD(5),BankE0 | 0xFF
    data_addr Data_abus_E,   WAY0 | INDEX(0x80) | WORD(0),WAY0 | INDEX(0x80) | WORD(1),WAY1 | INDEX(0x80) | WORD(0),WAY1 | INDEX(0x80) | WORD(1),BankE0 | 0x00
    data_addr Data_abus_E,   WAY0 | INDEX(0x0F) | WORD(0),WAY0 | INDEX(0x0F) | WORD(1),WAY1 | INDEX(0x0F) | WORD(0),WAY1 | INDEX(0x0F) | WORD(1),BankE0 | 0xF0
    data_addr Data_abus_E,   WAY2 | INDEX(0xF0) | WORD(4),WAY2 | INDEX(0xF0) | WORD(5),WAY3 | INDEX(0xF0) | WORD(4),WAY3 | INDEX(0xF0) | WORD(5),BankE0 | 0x0F
    data_addr Data_abus_E,   WAY0 | INDEX(0x33) | WORD(0),WAY0 | INDEX(0x33) | WORD(1),WAY1 | INDEX(0x33) | WORD(0),WAY1 | INDEX(0x33) | WORD(1),BankE0 | 0xCC
    data_addr Data_abus_E,   WAY2 | INDEX(0xCC) | WORD(4),WAY2 | INDEX(0xCC) | WORD(5),WAY3 | INDEX(0xCC) | WORD(4),WAY3 | INDEX(0xCC) | WORD(5),BankE0 | 0x33
    data_addr Data_abus_E,   WAY2 | INDEX(0x15) | WORD(4),WAY2 | INDEX(0x15) | WORD(5),WAY3 | INDEX(0x15) | WORD(4),WAY3 | INDEX(0x15) | WORD(5),BankE0 | 0xAA
    data_addr Data_abus_E,   WAY0 | INDEX(0xEA) | WORD(0),WAY0 | INDEX(0xEA) | WORD(1),WAY1 | INDEX(0xEA) | WORD(0),WAY1 | INDEX(0xEA) | WORD(1),BankE0 | 0x55
    rumboot_putstring "Abus_E check OK\n"

    //Array F run
    data_addr Data_abus_F,   WAY2 | INDEX(0x7F) | WORD(6),WAY2 | INDEX(0x7F) | WORD(7),WAY3 | INDEX(0x7F) | WORD(6),WAY3 | INDEX(0x7F) | WORD(7),BankF0 | 0xFF
    data_addr Data_abus_F,   WAY0 | INDEX(0x80) | WORD(2),WAY0 | INDEX(0x80) | WORD(3),WAY1 | INDEX(0x80) | WORD(2),WAY1 | INDEX(0x80) | WORD(3),BankF0 | 0x00
    data_addr Data_abus_F,   WAY0 | INDEX(0x0F) | WORD(2),WAY0 | INDEX(0x0F) | WORD(3),WAY1 | INDEX(0x0F) | WORD(2),WAY1 | INDEX(0x0F) | WORD(3),BankF0 | 0xF0
    data_addr Data_abus_F,   WAY2 | INDEX(0xF0) | WORD(6),WAY2 | INDEX(0xF0) | WORD(7),WAY3 | INDEX(0xF0) | WORD(6),WAY3 | INDEX(0xF0) | WORD(7),BankF0 | 0x0F
    data_addr Data_abus_F,   WAY0 | INDEX(0x33) | WORD(2),WAY0 | INDEX(0x33) | WORD(3),WAY1 | INDEX(0x33) | WORD(2),WAY1 | INDEX(0x33) | WORD(3),BankF0 | 0xCC
    data_addr Data_abus_F,   WAY2 | INDEX(0xCC) | WORD(6),WAY2 | INDEX(0xCC) | WORD(7),WAY3 | INDEX(0xCC) | WORD(6),WAY3 | INDEX(0xCC) | WORD(7),BankF0 | 0x33
    data_addr Data_abus_F,   WAY2 | INDEX(0x15) | WORD(6),WAY2 | INDEX(0x15) | WORD(7),WAY3 | INDEX(0x15) | WORD(6),WAY3 | INDEX(0x15) | WORD(7),BankF0 | 0xAA
    data_addr Data_abus_F,   WAY0 | INDEX(0xEA) | WORD(2),WAY0 | INDEX(0xEA) | WORD(3),WAY1 | INDEX(0xEA) | WORD(2),WAY1 | INDEX(0xEA) | WORD(3),BankF0 | 0x55
    rumboot_putstring "Abus_F check OK\n"

    //Array G run
    data_addr Data_abus_G,   WAY0 | INDEX(0x7F) | WORD(4),WAY0 | INDEX(0x7F) | WORD(5),WAY1 | INDEX(0x7F) | WORD(4),WAY1 | INDEX(0x7F) | WORD(5),BankG0 | 0xFF
    data_addr Data_abus_G,   WAY2 | INDEX(0x80) | WORD(0),WAY2 | INDEX(0x80) | WORD(1),WAY3 | INDEX(0x80) | WORD(0),WAY3 | INDEX(0x80) | WORD(1),BankG0 | 0x00
    data_addr Data_abus_G,   WAY2 | INDEX(0x0F) | WORD(0),WAY2 | INDEX(0x0F) | WORD(1),WAY3 | INDEX(0x0F) | WORD(0),WAY3 | INDEX(0x0F) | WORD(1),BankG0 | 0xF0
    data_addr Data_abus_G,   WAY0 | INDEX(0xF0) | WORD(4),WAY0 | INDEX(0xF0) | WORD(5),WAY1 | INDEX(0xF0) | WORD(4),WAY1 | INDEX(0xF0) | WORD(5),BankG0 | 0x0F
    data_addr Data_abus_G,   WAY2 | INDEX(0x33) | WORD(0),WAY2 | INDEX(0x33) | WORD(1),WAY3 | INDEX(0x33) | WORD(0),WAY3 | INDEX(0x33) | WORD(1),BankG0 | 0xCC
    data_addr Data_abus_G,   WAY0 | INDEX(0xCC) | WORD(4),WAY0 | INDEX(0xCC) | WORD(5),WAY1 | INDEX(0xCC) | WORD(4),WAY1 | INDEX(0xCC) | WORD(5),BankG0 | 0x33
    data_addr Data_abus_G,   WAY0 | INDEX(0x15) | WORD(4),WAY0 | INDEX(0x15) | WORD(5),WAY1 | INDEX(0x15) | WORD(4),WAY1 | INDEX(0x15) | WORD(5),BankG0 | 0xAA
    data_addr Data_abus_G,   WAY2 | INDEX(0xEA) | WORD(0),WAY2 | INDEX(0xEA) | WORD(1),WAY3 | INDEX(0xEA) | WORD(0),WAY3 | INDEX(0xEA) | WORD(1),BankG0 | 0x55
    rumboot_putstring "Abus_G check OK\n"

    //Array H run
    data_addr Data_abus_H,   WAY0 | INDEX(0x7F) | WORD(6),WAY0 | INDEX(0x7F) | WORD(7),WAY1 | INDEX(0x7F) | WORD(6),WAY1 | INDEX(0x7F) | WORD(7),BankH0 | 0xFF
    data_addr Data_abus_H,   WAY2 | INDEX(0x80) | WORD(2),WAY2 | INDEX(0x80) | WORD(3),WAY3 | INDEX(0x80) | WORD(2),WAY3 | INDEX(0x80) | WORD(3),BankH0 | 0x00
    data_addr Data_abus_H,   WAY2 | INDEX(0x0F) | WORD(2),WAY2 | INDEX(0x0F) | WORD(3),WAY3 | INDEX(0x0F) | WORD(2),WAY3 | INDEX(0x0F) | WORD(3),BankH0 | 0xF0
    data_addr Data_abus_H,   WAY0 | INDEX(0xF0) | WORD(6),WAY0 | INDEX(0xF0) | WORD(7),WAY1 | INDEX(0xF0) | WORD(6),WAY1 | INDEX(0xF0) | WORD(7),BankH0 | 0x0F
    data_addr Data_abus_H,   WAY2 | INDEX(0x33) | WORD(2),WAY2 | INDEX(0x33) | WORD(3),WAY3 | INDEX(0x33) | WORD(2),WAY3 | INDEX(0x33) | WORD(3),BankH0 | 0xCC
    data_addr Data_abus_H,   WAY0 | INDEX(0xCC) | WORD(6),WAY0 | INDEX(0xCC) | WORD(7),WAY1 | INDEX(0xCC) | WORD(6),WAY1 | INDEX(0xCC) | WORD(7),BankH0 | 0x33
    data_addr Data_abus_H,   WAY0 | INDEX(0x15) | WORD(6),WAY0 | INDEX(0x15) | WORD(7),WAY1 | INDEX(0x15) | WORD(6),WAY1 | INDEX(0x15) | WORD(7),BankH0 | 0xAA
    data_addr Data_abus_H,   WAY2 | INDEX(0xEA) | WORD(2),WAY2 | INDEX(0xEA) | WORD(3),WAY3 | INDEX(0xEA) | WORD(2),WAY3 | INDEX(0xEA) | WORD(3),BankH0 | 0x55
    rumboot_putstring "Abus_H check OK\n"

test_ok:
    rumboot_putstring "TEST_OK\n"
    exit 0x00

test_error:
    rumboot_putstring "TEST_ERROR\n"
    exit 0x01



dcu_data_array:
.long 0xAAAAAAAA //NU
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0x00000000
.long 0xFFFFFFFF
.long 0x00000000
.long 0x00000000
.long 0xFFFFFFFF
.long 0x00000000
.long 0xFFFFFFFF
.long 0x0000FFFF
.long 0x0000FFFF
.long 0x0000FFFF
.long 0x0000FFFF
.long 0xFFFF0000
.long 0xFFFF0000
.long 0xFFFF0000
.long 0xFFFF0000
.long 0x00FF00FF
.long 0x00FF00FF
.long 0x00FF00FF
.long 0x00FF00FF
.long 0xFF00FF00
.long 0xFF00FF00
.long 0xFF00FF00
.long 0xFF00FF00
.long 0x0F0F0F0F
.long 0x0F0F0F0F
.long 0x0F0F0F0F
.long 0x0F0F0F0F
.long 0xF0F0F0F0
.long 0xF0F0F0F0
.long 0xF0F0F0F0
.long 0xF0F0F0F0
.long 0x33333333
.long 0x33333333
.long 0x33333333
.long 0x33333333
.long 0xCCCCCCCC
.long 0xCCCCCCCC
.long 0xCCCCCCCC
.long 0xCCCCCCCC
.long 0x55555555
.long 0x55555555
.long 0x55555555
.long 0x55555555
.long 0xAAAAAAAA
.long 0xAAAAAAAA
.long 0xAAAAAAAA
.long 0xAAAAAAAA


