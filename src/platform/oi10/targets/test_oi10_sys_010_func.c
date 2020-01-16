/*
 * test_oi10_sys_010_func.c
 *
 *  Created on: Feb, 2019
 *      Author: v.fufaev
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/regs/regs_cldcr.h>
#include <platform/regs/regs_itrace.h>

void __attribute__((section(".text.test"))) trace_bt_func()
{
    asm volatile (
        ".set r3, 3               \n\t"
        ".set r4, 4               \n\t"
        ".set r5, 5               \n\t"
        ".set r6, 6               \n\t"
        ".set cr1, 1              \n\t"
        ".set cr2, 2              \n\t"
        "    lis r3, 0            \n\t"
        "    mflr r4              \n\t"
        "    mfctr r5             \n\t"
        "    lis r6,lab4@h        \n\t"
        "    ori r6,r6,lab4@l     \n\t"
        "    mtctr r6             \n\t"
        "    cmpwi cr1, r3, 1     \n\t"
        "    cmpwi cr2, r3, 0     \n\t"
        "    b  lab1              \n\t"
        "lab1:                    \n\t"
        "    beq+ cr2,lab2        \n\t"
        "lab2:                    \n\t"
        "    blt+ cr1,lab3        \n\t"
        "lab3:                    \n\t"
        "    bl lab4              \n\t"
        "    bltctrl cr1          \n\t"
        "    bl lab4              \n\t"
        "    bltctrl cr1          \n\t"
        "    bl lab4              \n\t"
        "    bltctrl cr1          \n\t"
        "    bl lab4              \n\t"
        "    bltctrl cr1          \n\t"
        "    bgectrl cr1          \n\t"
        "    bl lab4              \n\t"
        "    bltctrl cr1          \n\t"
        "    b  lab5              \n\t"
        "lab4:                    \n\t"
        "    blr                  \n\t"
        "lab5:                    \n\t"
        "    mtctr r5             \n\t"
        "    mtlr r4              \n\t"
        :::"r3","r4","r5","r6"
    );
}


void trace1_bt_func1();
void trace1_bt_func2();
void trace1_bt_func3();
void trace1_bt_func4();
void trace1_bt_func5();
void trace1_bt_func6();
void trace2_bt_func1();
void trace2_bt_func2();
void trace2_bt_func3();
void trace2_bt_func4();
void trace2_bt_func5();
void trace2_bt_func6();


void __attribute__((section(".trace1.funcs"))) trace1_bt_func1()
{
    asm volatile (
        "    bl trace2_bt_func1   \n\t"
        :::"lr"
    );
}
void __attribute__((section(".trace1.funcs"))) trace1_bt_func2()
{
    asm volatile (
        "    bl trace2_bt_func2   \n\t"
        :::"lr"
    );
}
void __attribute__((section(".trace1.funcs"))) trace1_bt_func3()
{
    asm volatile (
        "    bl trace2_bt_func3   \n\t"
        :::"lr"
    );
}
void __attribute__((section(".trace1.funcs"))) trace1_bt_func4()
{
    asm volatile (
        ".set r3, 3               \n\t"
        "    lis r3,trace2_bt_func4@h    \n\t"
        "    ori r3,r3,trace2_bt_func4@l \n\t"
        "    mtctr r3             \n\t"
        "    bctrl                \n\t"
        :::"r3","lr","ctr"
    );
}

void __attribute__((section(".trace1.funcs"))) trace1_bt_func5()
{
    asm volatile (
        ".set r3, 3               \n\t"
        "    lis r3,trace2_bt_func5@h    \n\t"
        "    ori r3,r3,trace2_bt_func5@l \n\t"
        "    mtctr r3             \n\t"
        "    bctrl                \n\t"
        :::"r3","lr","ctr"
    );
}

void __attribute__((section(".trace1.funcs"))) trace1_bt_func6()
{
    asm volatile (
        ".set r3, 3               \n\t"
        "    lis r3,trace2_bt_func6@h    \n\t"
        "    ori r3,r3,trace2_bt_func6@l \n\t"
        "    mtctr r3             \n\t"
        "    bctrl                \n\t"
        :::"r3","lr","ctr"
    );
}


void __attribute__((section(".trace2.funcs"))) trace2_bt_func1()
{
    asm volatile (
        "    bl trace1_bt_func2   \n\t"
        :::"lr"
    );
}
void __attribute__((section(".trace2.funcs"))) trace2_bt_func2()
{
    asm volatile (
        "    bl trace1_bt_func3   \n\t"
        :::"lr"
    );
}
void __attribute__((section(".trace2.funcs"))) trace2_bt_func3()
{
    asm volatile (
        ".set r3, 3               \n\t"
        "    lis r3,trace1_bt_func4@h    \n\t"
        "    ori r3,r3,trace1_bt_func4@l \n\t"
        "    mtctr r3             \n\t"
        "    bctrl                \n\t"
        :::"r3","lr","ctr"
    );
}

void __attribute__((section(".trace2.funcs"))) trace2_bt_func4()
{
    asm volatile (
        ".set r3, 3               \n\t"
        "    lis r3,trace1_bt_func5@h    \n\t"
        "    ori r3,r3,trace1_bt_func5@l \n\t"
        "    mtctr r3             \n\t"
        "    bctrl                \n\t"
        :::"r3","lr","ctr"
    );
}

void __attribute__((section(".trace2.funcs"))) trace2_bt_func5()
{
    asm volatile (
        ".set r3, 3               \n\t"
        "    lis r3,trace1_bt_func6@h    \n\t"
        "    ori r3,r3,trace1_bt_func6@l \n\t"
        "    mtctr r3             \n\t"
        "    bctrl                \n\t"
        :::"r3","lr","ctr"
    );
}

void __attribute__((section(".trace2.funcs"))) trace2_bt_func6()
{
}


