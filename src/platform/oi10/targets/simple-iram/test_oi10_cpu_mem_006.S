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

#define BankAB (0b00 << 10)
#define BankCD (0b01 << 10)
#define BankEF (0b10 << 10)
#define BankGH (0b11 << 10)

#define WAY0   (reg_field(18,0))
#define WAY1   (reg_field(18,1))
#define WAY2   (reg_field(18,2))
#define WAY3   (reg_field(18,3))

//#define DCDBTRH_MASK   0xFFFFEFFF //19th bit - valid, non-mapped onto memory
#define DCDBTRH_MASK   0xFFFFFFFF

#define INDEX(index) \
    (reg_field(26,index))
#define WORD(word) \
    (reg_field(29,word))

.macro dcread_macro regT,regA
     msync
     dcread \regT, 0, \regA
     isync
.endm

.macro dcread_wm_macro regT,regA, gpr_btrl, gpr_btrh,maskR
     msync
     dcread \regT, 0, \regA
     isync
     mfspr \gpr_btrl, DCDBTRL
     mfspr \gpr_btrh, DCDBTRH
     and \gpr_btrh,\maskR,\gpr_btrh
.endm

.macro tag_run label,InitAddr0,InitAddr1
    //r2 points to data array
    lis r2, dcu_tag_array@h
    ori r2, r2, dcu_tag_array@l
    //final counter value
    load_const r4, 13
    //r5 - initial address way 0/2
    load_const r5, \InitAddr0
    //r6 - counter 0..31
    load_const r6, 0x00000000
    //dcread target - r7 //don't care
    li r7, 0x0
    //r8 - dcdbtrl, r9 - dcdbtrh
    //r10 - initial address way 1/3
    load_const r10, \InitAddr1
    //valid bid must be masked
    load_const r11, DCDBTRH_MASK

loop1_\label:
    //1st way (0 or 2)
    lwzu r3, 4(r2) //load exp value
    dcread_wm_macro r7,r10,r8,r9,r11
    cmp cr7,0,r3,r9
    TEST_ASSERT(eq,cr7,"\label running 1 error")
    addi r10,r10,64//increment 16 words (+2 to index)

    lwzu r3, 4(r2) //load exp value
    dcread_wm_macro r7,r5,r8,r9,r11
    cmp cr7,0,r3,r9
    TEST_ASSERT(eq,cr7,"\label running 1 error")
    addi r5,r5,64//increment 16 words (+2 to index)

    cmp cr7,0,r6,r4 //check counter
    beq cr7, end_loop1_\label

    addi r6,r6,1 //increment counter
    b loop1_\label
end_loop1_\label:
    rumboot_putstring "\label running 1 OK\n"
.endm


.macro tag_addr label,addr,exp_val
    //r5 - addr
    load_const r5, \addr
    //r3 - exp val
    load_const r3, \exp_val
    //r7 - don't care
    //valid bid must be masked
    load_const r11, DCDBTRH_MASK

    dcread_wm_macro r7,r5,r8,r9,r11
    cmp cr7,0,r3,r9
    TEST_ASSERT(eq,cr7,"\label Addr bus check error, exp.val: \exp_val")
.endm



.section ".text","ax",@progbits

.global main

main:
test_oi10_cpu_mem_006:

    test_event_send_test_id "test_oi10_cpu_mem_006"

DCU_TAG_CHECK:
    test_event EVENT_DCU_TAG_FILL_1
    //                              way x     way x+1
    tag_run Tag_db_AB, 0x00000000, 0x00002000
    tag_run Tag_db_CD, 0x00004000, 0x00006000
    tag_run Tag_db_EF, 0x00000020, 0x00002020
    tag_run Tag_db_GH, 0x00004020, 0x00006020

    test_event EVENT_DCU_TAG_FILL_ADDR
    //                            way               index               word
    //running 1, bank AB
    tag_addr Tag_abus_AB, (reg_field(18,0) | reg_field(26,0xFF) | reg_field(29,0)), 0x0000007F | BankAB
    tag_addr Tag_abus_AB, (reg_field(18,0) | reg_field(26,0x00) | reg_field(29,0)), 0x00000000 | BankAB
    tag_addr Tag_abus_AB, (reg_field(18,0) | reg_field(26,0x0E) | reg_field(29,0)), 0x00000070 | BankAB
    tag_addr Tag_abus_AB, (reg_field(18,0) | reg_field(26,0xF1) | reg_field(29,0)), 0x0000000F | BankAB
    tag_addr Tag_abus_AB, (reg_field(18,0) | reg_field(26,0x32) | reg_field(29,0)), 0x0000004C | BankAB
    tag_addr Tag_abus_AB, (reg_field(18,0) | reg_field(26,0xCD) | reg_field(29,0)), 0x00000033 | BankAB
    tag_addr Tag_abus_AB, (reg_field(18,0) | reg_field(26,0x95) | reg_field(29,0)), 0x0000002A | BankAB
    tag_addr Tag_abus_AB, (reg_field(18,0) | reg_field(26,0x6A) | reg_field(29,0)), 0x00000055 | BankAB
    rumboot_putstring "Tag_abus_AB checking OK\n"

    //running 1, bank CD
    tag_addr Tag_abus_CD, (reg_field(18,2) | reg_field(26,0xFF) | reg_field(29,0)), 0x0000007F | BankCD
    tag_addr Tag_abus_CD, (reg_field(18,2) | reg_field(26,0x00) | reg_field(29,0)), 0x00000000 | BankCD
    tag_addr Tag_abus_CD, (reg_field(18,2) | reg_field(26,0x0E) | reg_field(29,0)), 0x00000070 | BankCD
    tag_addr Tag_abus_CD, (reg_field(18,2) | reg_field(26,0xF1) | reg_field(29,0)), 0x0000000F | BankCD
    tag_addr Tag_abus_CD, (reg_field(18,2) | reg_field(26,0x32) | reg_field(29,0)), 0x0000004C | BankCD
    tag_addr Tag_abus_CD, (reg_field(18,2) | reg_field(26,0xCD) | reg_field(29,0)), 0x00000033 | BankCD
    tag_addr Tag_abus_CD, (reg_field(18,2) | reg_field(26,0x95) | reg_field(29,0)), 0x0000002A | BankCD
    tag_addr Tag_abus_CD, (reg_field(18,2) | reg_field(26,0x6A) | reg_field(29,0)), 0x00000055 | BankCD
    rumboot_putstring "Tag_abus_CD checking OK\n"

    //running 1, bank EF
    tag_addr Tag_abus_EF, (reg_field(18,0) | reg_field(26,0x7F) | reg_field(29,0)), 0x0000007F | BankEF
    tag_addr Tag_abus_EF, (reg_field(18,0) | reg_field(26,0x80) | reg_field(29,0)), 0x00000000 | BankEF
    tag_addr Tag_abus_EF, (reg_field(18,0) | reg_field(26,0x8E) | reg_field(29,0)), 0x00000070 | BankEF
    tag_addr Tag_abus_EF, (reg_field(18,0) | reg_field(26,0x71) | reg_field(29,0)), 0x0000000F | BankEF
    tag_addr Tag_abus_EF, (reg_field(18,0) | reg_field(26,0xB2) | reg_field(29,0)), 0x0000004C | BankEF
    tag_addr Tag_abus_EF, (reg_field(18,0) | reg_field(26,0x4D) | reg_field(29,0)), 0x00000033 | BankEF
    tag_addr Tag_abus_EF, (reg_field(18,0) | reg_field(26,0x15) | reg_field(29,0)), 0x0000002A | BankEF
    tag_addr Tag_abus_EF, (reg_field(18,0) | reg_field(26,0xEA) | reg_field(29,0)), 0x00000055 | BankEF
    rumboot_putstring "Tag_abus_EF checking OK\n"

    //running 1, bank GH
    tag_addr Tag_abus_GH, (reg_field(18,2) | reg_field(26,0x7F) | reg_field(29,0)), 0x0000007F | BankGH
    tag_addr Tag_abus_GH, (reg_field(18,2) | reg_field(26,0x80) | reg_field(29,0)), 0x00000000 | BankGH
    tag_addr Tag_abus_GH, (reg_field(18,2) | reg_field(26,0x8E) | reg_field(29,0)), 0x00000070 | BankGH
    tag_addr Tag_abus_GH, (reg_field(18,2) | reg_field(26,0x71) | reg_field(29,0)), 0x0000000F | BankGH
    tag_addr Tag_abus_GH, (reg_field(18,2) | reg_field(26,0xB2) | reg_field(29,0)), 0x0000004C | BankGH
    tag_addr Tag_abus_GH, (reg_field(18,2) | reg_field(26,0x4D) | reg_field(29,0)), 0x00000033 | BankGH
    tag_addr Tag_abus_GH, (reg_field(18,2) | reg_field(26,0x15) | reg_field(29,0)), 0x0000002A | BankGH
    tag_addr Tag_abus_GH, (reg_field(18,2) | reg_field(26,0xEA) | reg_field(29,0)), 0x00000055 | BankGH
    rumboot_putstring "Tag_abus_GH checking OK\n"

test_ok:
    rumboot_putstring "TEST_OK\n"
    exit 0x00

test_error:
    rumboot_putstring "TEST_ERROR\n"
    exit 0x01



dcu_tag_array:
.long 0x00000000 //empty not used

.long 0XFFFFEFFF //0X7FFFFFFF
.long 0XFFFFEFFF //0X7FFFFFFF
.long 0000000000 //0000000000
.long 0000000000 //0000000000
.long 0000000000 //0000000000
.long 0XFFFFEBFF //0X7FFFFFFE
.long 0XFFFFEFFF //0X7FFFFFFF
.long 0X00000400 //0X00000001
.long 0X07FFEC00 //0X0000FFFF
.long 0X0FFFE800 //0X0001FFFE
.long 0XF80003FF //0X7FFF0000
.long 0XF00007FF //0XFFFE0001
.long 0XF807EC07 //0X00FF00FF
.long 0XF00FE80F //0X01FE01FE
.long 0X07F803F8 //0X7F00FF00
.long 0X0FF007F0 //0XFE01FE01
.long 0X78786C78 //0X0F0F0F0F
.long 0XF0F0E8F0 //0X1E1E1E1E
.long 0X87878387 //0X70F0F0F0
.long 0X0F0F070F //0XE1E1E1E1
.long 0X99998D99 //0X33333333
.long 0X33332B33 //0X66666666
.long 0X66666266 //0X4CCCCCCC
.long 0XCCCCC4CC //0X99999999
.long 0XAAAAA6AA //0X55555555
.long 0X55554955 //0XAAAAAAAA
.long 0X55554955 //0X2AAAAAAA
.long 0XAAAAA6AA //0X55555555
