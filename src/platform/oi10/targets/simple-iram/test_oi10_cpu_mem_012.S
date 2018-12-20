#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/devices.h>

#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/regs/regs_plb6plb4.h>

.section ".text","ax",@progbits

.global main

DECLARE_CONST ( TEST_MPW_CPU_MEM_012_EVENT_START, TEST_EVENT_CODE_MIN )
DECLARE_CONST ( TEST_MPW_CPU_MEM_012_EVENT_VERIFY, TEST_EVENT_CODE_MIN + 1 )
DECLARE_CONST ( TEST_MPW_CPU_MEM_012_EVENT_FINISHED, TEST_EVENT_CODE_MIN + 2 )

.macro insert_nop_0
.endm

.macro insert_nop_1
    ori r0, r0, 0
.endm

.macro insert_nop_2
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_3
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_4
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_5
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_6
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_7
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
    ori r0, r0, 0
.endm

.macro insert_nop_8
    insert_nop_0
.endm

.macro insert_nop_9
    insert_nop_1
.endm

.macro set_bht_array_bits lbl, origin
    load_const r3, 0 //counter

    check_set_\lbl:
    addi r3, r3, 1
    cmp cr7, 0, r3, r4
    beq cr7, finish_check_set_\lbl
    b set_\lbl

    .org \origin  //drives mem[i]
    set_\lbl:
    insert_nop_\lbl
    beq check_set_\lbl //(r1=r2 counter will be incremented)

    finish_check_set_\lbl:
.endm

.macro clr_bht_array_bits lbl, origin
    load_const r3, 0 //counter

    check_clr_\lbl:
    addi r3, r3, 1
    cmp cr7, 0, r3, r4
    beq cr7, finish_check_clr_\lbl
    b clr_\lbl

    .org \origin
    clr_\lbl:
    insert_nop_\lbl
    beq finish_check_clr_\lbl
    b check_clr_\lbl
    ori r0, r0, 0

    finish_check_clr_\lbl:
.endm

main:
    test_event_send_test_id "test_oi10_cpu_mem_012"
    rumboot_putstring "\nSend event verify\n"

    b bht_check

bht_check:
    //disable GHR
    mfspr r3, SPR_CCR0
    ori r3, r3, ((1 << IBM_BIT_INDEX(32, 20)))
    mtspr SPR_CCR0, r3

check_set_bits:
    load_const r1, 5 //value for branch conditional
    load_const r2, 5 //value for branch conditional
    load_const r3, 0 //counter
    load_const r4, 6 //stop counter

    test_event TEST_MPW_CPU_MEM_012_EVENT_VERIFY

    rumboot_putstring "check set bits\n"

    cmp cr0, 0, r1, r2 //r1==r2 for creation branch taken

    set_bht_array_bits 0, 0x4000 //mem[256][15:14]
    set_bht_array_bits 1, 0x4020 //mem[258][13:12]
    set_bht_array_bits 2, 0x8040 //mem[128][11:10]
    set_bht_array_bits 3, 0x8080 //mem[384][9:8]
    set_bht_array_bits 4, 0x8100 //mem[320][7:6]
    set_bht_array_bits 5, 0x8200 //mem[288][5:4]
    set_bht_array_bits 6, 0x8400 //mem[272][3:2]
    set_bht_array_bits 7, 0x8800 //mem[264][1:0]
    set_bht_array_bits 8, 0x9000 //mem[260][15:14]
    set_bht_array_bits 9, 0xA000 //mem[257][13:12]

check_clr_bits:
    load_const r1, 6
    load_const r4, 4

    rumboot_putstring "check clear bits\n"

    cmp cr0, 0, r1, r2 //r1!=r2 for creation branch not-taken

    clr_bht_array_bits 0, 0xC000  //mem[256][15:14]
    clr_bht_array_bits 1, 0xC020  //mem[258][13:12]
    clr_bht_array_bits 2, 0x10040 //mem[128][11:10]
    clr_bht_array_bits 3, 0x10080 //mem[384][9:8]
    clr_bht_array_bits 4, 0x10100 //mem[320][7:6]
    clr_bht_array_bits 5, 0x10200 //mem[288][5:4]
    clr_bht_array_bits 6, 0x10400 //mem[272][3:2]
    clr_bht_array_bits 7, 0x10800 //mem[264][1:0]
    clr_bht_array_bits 8, 0x11000 //mem[260][15:14]
    clr_bht_array_bits 9, 0x12000 //mem[257][13:12]

test_finished:
    rumboot_putstring "\nTEST FINISHED\n"
    test_event      TEST_MPW_CPU_MEM_012_EVENT_FINISHED
    b infinite_loop

infinite_loop:
    b infinite_loop
