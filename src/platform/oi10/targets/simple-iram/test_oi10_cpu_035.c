/*
 * test_oi10_cpu_035.c
 *
 *  Created on: Jan 21, 2019
 *      Author: v.fufaev
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <platform/common_macros/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>

enum {
       TEC_CHECK_BHT_MEMORY = TEST_EVENT_CODE_MIN
} test_event_code;

volatile uint32_t tbl_value[64];

void __attribute__((noinline)) start_func(void)
{
    asm (
        ".set SPR_CCR0, 947         \n\t"
        ".set SPR_TBL_R, 268        \n\t"
        ".set SPR_TBL_W, 284        \n\t"
        ".set r3, 3                 \n\t"
        ".set r4, 4                 \n\t"
        ".set r5, 5                 \n\t"
        ".set r6, 6                 \n\t"
        ".set r7, 7                 \n\t"
        ".set cr0, 0                \n\t"
        ".set cr1, 1                \n\t"
        ".set cr2, 2                \n\t"
        "test_oi10_cpu_035:         \n\t"
        "    li  r3, 0x7CDE         \n\t"
        "    lis r4, 0              \n\t"
        "    lis r5, 0              \n\t"
        "    lis r6,tbl_value@h     \n\t"
        "    ori r6,r6,tbl_value@l  \n\t"
        "    addi r6, r6, -4        \n\t"
        "    lis r3, 0              \n\t"
        "    addi r3, r3, 1         \n\t"
        "    cmpwi cr0, r3, 0       \n\t"
        "    lis r3, 0              \n\t"
        ".align 5                   \n\t"
        "start_branching:           \n\t"
        ".rept (512*8)              \n\t"
        "1:                         \n\t"
        "    bge+ 1f                \n\t"
        ".endr                      \n\t"
        "1:                         \n\t"
        "    addi r3, r3, 1         \n\t"
        "    cmpwi cr2, r3, 8       \n\t"
        "    ble+ cr2,start_branching  \n\t"
        "    isync                  \n\t"
        "    lis r3, 0              \n\t"
        "    lis r4, 1              \n\t"
        "    cmpwi cr0, r4, 0       \n\t"
        "next_branching_stage:      \n\t"
        "    li r7,0                \n\t"
        "    mtspr SPR_TBL_W,r7     \n\t"
        ".align 5                   \n\t"
        ".rept 64                   \n\t"
        "1:                         \n\t"
        "   bge+ 1f                 \n\t"
        ".endr                      \n\t"
        "1:                         \n\t"
        "    mfspr r7,SPR_TBL_R     \n\t"
        "    stwu r7,4(r6)          \n\t"
        "    addi r3, r3, 1         \n\t"
        "    cmpwi cr0, r3, 16      \n\t"
        "    bge- continue_second_branching  \n\t"
        "    cmpwi cr0, r3, 0       \n\t"
        "    b next_branching_stage \n\t"
        "continue_second_branching: \n\t"
        "    isync                  \n\t"
        "    lis r3, 0              \n\t"
        "    lis r4, 1              \n\t"
        "    cmpwi cr0, r4, 0       \n\t"
        "    b rebranching          \n\t"
        "third_branching_stage:     \n\t"
        "    li r7,0                \n\t"
        "    mtspr SPR_TBL_W,r7     \n\t"
        ".align 5                   \n\t"
        ".rept 64                   \n\t"
        "1:                         \n\t"
        "   bge+ 1f                 \n\t"
        ".endr                      \n\t"
        "1:                         \n\t"
        "    mfspr r7,SPR_TBL_R     \n\t"
        "    stwu r7,4(r6)          \n\t"
        "    isync                  \n\t"
        "    addi r3, r3, 1         \n\t"
        "    cmpwi cr0, r3, 16      \n\t"
        "    blt+ third_branching_stage  \n\t"
        "end_branching_stage:       \n\t"
        "    lis r3,next_branching_stage@h    \n\t"
        "    ori r3,r3,next_branching_stage@l \n\t"
        "    mtctr r3               \n\t"
        "    lis r3, 0              \n\t"
        "    addi r5, r5, 1         \n\t"
        "    cmpwi cr1, r5, 2       \n\t"
        "    bltctr+ cr1            \n\t"
        "    b ok                   \n\t"
        "rebranching:               \n\t"
        "   b third_branching_stage \n\t"
        "ok:                        \n\t"
        :::"3","4","5","6","7"
    );
}


int main(void)
{
    uint32_t i,t1,t2;

    test_event_send_test_id("test_oi10_cpu_035");
    //rumboot_printf("Start test_oi10_cpu_035.\n");

    rumboot_printf("Disable GHR... ");
    uint32_t ccr0_val = spr_read(SPR_CCR0);
    ccr0_val |= (1 << IBM_BIT_INDEX(32, 20));
    spr_write(SPR_CCR0, ccr0_val);
    rumboot_printf("Done.\n");

    rumboot_printf("Start test.\n");
    test_event(TEC_CHECK_BHT_MEMORY );
    spr_write(SPR_TBU_W,0);
    spr_write(SPR_TBL_W,0);
    start_func();

    t1 = 0;
    t2 = 0;

    for (i = 16; i < 32; i++) {t1 += tbl_value[i];}
    for (i = 48; i < 64; i++) {t2 += tbl_value[i];}

    rumboot_printf("First loop: %d cycles \n", t1);
    rumboot_printf(" Iterations: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d (cycles)\n",
        tbl_value[16],tbl_value[17],tbl_value[18],tbl_value[19],tbl_value[20],tbl_value[21],tbl_value[22],tbl_value[23],
        tbl_value[24],tbl_value[25],tbl_value[26],tbl_value[27],tbl_value[28],tbl_value[28],tbl_value[30],tbl_value[31]);

    rumboot_printf("Second loop: %d cycles\n", t2);
    rumboot_printf(" Iterations: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d (cycles)\n",
        tbl_value[48],tbl_value[49],tbl_value[50],tbl_value[51],tbl_value[52],tbl_value[53],tbl_value[54],tbl_value[55],
        tbl_value[56],tbl_value[57],tbl_value[58],tbl_value[59],tbl_value[60],tbl_value[61],tbl_value[62],tbl_value[63]);

    TEST_ASSERT((t1 > t2),"The performance has not enhanced!");

    t1 = (tbl_value[16] + tbl_value[17] + tbl_value[18] + tbl_value[19]) >> 2;
    t2 = (tbl_value[28] + tbl_value[29] + tbl_value[30] + tbl_value[31]) >> 2;
    TEST_ASSERT((t1 > (2*t2)),"The performance has not enhanced!");

    t1 = (tbl_value[48] + tbl_value[49] + tbl_value[50] + tbl_value[51]) >> 2;
    t2 = (tbl_value[60] + tbl_value[61] + tbl_value[62] + tbl_value[63]) >> 2;
    TEST_ASSERT((t1 > (2*t2)),"The performance has not enhanced!");

    rumboot_printf("TEST OK\n");
    return 0;
}
