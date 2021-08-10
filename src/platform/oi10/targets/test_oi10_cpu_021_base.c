/*
 * test_oi10_cpu_021_base.c
 *
 *  Created on: Sep 4, 2018
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/common_macros.h>
#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <arch/dma2plb6.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>
#include <platform/regs/regs_l2c_l2.h>
#include <arch/dma2plb6.h>

typedef uint8_t func();
#define NUM_BYTE      (1024 * 8)

uint8_t __attribute__((section(".data"),aligned(0x100))) volatile im0_data[NUM_BYTE] = { 0 };

int main(void)
{
    rumboot_printf("Starting base test\n");
    emi_init(DCR_EM2_EMI_BASE);

    TEST_ASSERT(l2c_l2_read (DCR_L2C_BASE, L2C_L2ISTAT),"L2C Array Initialization Complete Event did not occur!");

    rumboot_printf("Copy SRAM0BIN to IM0 (addr = 0x%x)\n", im0_data);
    rumboot_platform_request_file("SRAM0BIN", (uint32_t) im0_data);
    rumboot_printf("Copy successful (data = 0x%x)\n", ioread32((uint32_t)im0_data));
    rumboot_printf("Prepare DMA (im0 -> sram0)...\n");

    struct dma2plb6_setup_info dma_info;
    struct channel_status status = {};
    dma_info.base_addr = DCR_DMAPLB6_BASE;
    dma_info.source_adr = rumboot_virt_to_phys((void*) im0_data);
    dma_info.dest_adr = rumboot_virt_to_phys((void*) SRAM0_BASE);
    dma_info.priority = priority_medium_low;
    dma_info.striding_info.striding = striding_none;
    dma_info.tc_int_enable = false;
    dma_info.err_int_enable = false;
    dma_info.int_mode = int_mode_level_wait_clr;
    dma_info.channel = channel0;
    dma_info.transfer_width = tr_width_quadword;
    dma_info.rw_transfer_size = rw_tr_size_8q;
    dma_info.count = NUM_BYTE/16;

    rumboot_printf("DMA prepared, starting copy.\n");
    dma2plb6_mcpy(&dma_info);
    if (!wait_dma2plb6_mcpy (&dma_info, &status))
    {
        dma2plb6_trace_error_status(&status);
        return 1;
    }
    msync();
    rumboot_printf("Code and data copied to SRAM0\n");
    rumboot_printf("Starting test code from SRAM0 ...\n");
    func* f = (func*)(SRAM0_BASE);
    uint8_t result = f();
    if (result != 0)
    {
        switch (result) // result == error code
        {
            case 1: rumboot_printf("COMPARE FAIL: step 7\n"); break;
            case 2: rumboot_printf("COMPARE FAIL: step 11\n"); break;
            case 3: rumboot_printf("COMPARE FAIL: step 15\n"); break;
            case 4: rumboot_printf("COMPARE FAIL: step 17\n"); break;
        }
        rumboot_printf("TEST_ERROR\n");
        return 1;
    }

    rumboot_printf("TEST_OK\n");
    return 0;
}
