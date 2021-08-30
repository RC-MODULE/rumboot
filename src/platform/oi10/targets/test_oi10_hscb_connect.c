/*
 * test_oi10_hscb.c
 *
 *  Created on: Jul 23, 2018
 *      Author: r.galiulin
 */


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_hscb.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <rumboot/memfill.h>
#include <devices/ugly/hscb.h>
#include <devices/ugly/emi.h>
#include <platform/devices/nor_1636RR4.h>
#include <arch/ppc_476fp_mmu.h>

int main() {
    uint32_t reg = 0;

    rumboot_printf( "HSCB connection test \n");

    reg = ioread32(HSCB_DEV_ADDR + 0x0C);
    if (!(reg & 0x00000001))
        iowrite32(0x1, HSCB_DEV_ADDR + 0x0C); // Enable HSCB
    
    rumboot_printf( "HSCB enabled \n");
    
    reg = ioread32(HSCB_DEV_ADDR + 0x10);
    
    reg = 0;
    while (!reg){
        reg = ioread32(HSCB_DEV_ADDR + 0x10);
        rumboot_printf( "HSCB STATUS = %x \n", reg);
        reg = reg & 0x3c7;
        rumboot_printf( "HSCB active_link = %x \n", reg);
//        iowrite32(0x0, HSCB1_BASE + 0x0C); // Enable HSCB
    }
    
    rumboot_printf( "HSCB disconnection detect! \n");

    return 1;
}
