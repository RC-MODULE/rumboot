/*
 * test_oi10_sys_010.c
 *
 *  Created on: Nov 07, 2018
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
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
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
        :::"3","4","5"
    );
}
