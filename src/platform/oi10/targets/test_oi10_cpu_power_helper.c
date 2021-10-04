/*
 * test_oi10_cpu_power_helper.c
 *
 *  Created on: 
 *   Original Author: 
 *   Rebuild by     : 
 */
// 

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <rumboot/memfill.h>
#include <devices/ugly/hscb.h>
#include <devices/ugly/emi.h>
#include <arch/ppc_476fp_mmu.h>
#include <devices/com_simple.h>
#include <devices/rcm_cp.h>

extern void test_oi10_power_1_7();

int __attribute__((section(".text.test"))) check_cpu_power(void)
{
    
    test_oi10_power_1_7();       

    return 0;
}