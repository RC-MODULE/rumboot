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
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/dma2plb6.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/devices/dma2plb6.h>

typedef uint8_t func();
#define L2C_TIMEOUT   0x00000020
#define NUM_BYTE      (1024 * 8)

uint8_t __attribute__((section(".data"),aligned(0x100))) volatile im0_data[NUM_BYTE] = { 0 };

inline void dma2plb6_trace_status(channel_status status)
{
    switch(status.spec_error_status)
    {
        case error_alignnment:
        case error_scatter_alignment:
            rumboot_printf ("DMA2PLB6: Error alignment\n");
            break;
        case error_read_data_regular:
        case error_read_data_scatter_or_resume:
            rumboot_printf ("DMA2PLB6: Error read data\n");
            break;
        case error_read_request_regular:
        case error_read_request_scatter_or_resume:
            rumboot_printf ("DMA2PLB6: Error read request\n");
            break;
        case error_write_request:
            rumboot_printf ("DMA2PLB6: Error write request\n");
            break;
        default:
            rumboot_printf ("DMA2PLB6: Unexpected status\n");
    }
}

int main(void)
{
    rumboot_printf("Starting base test\n");
    emi_init(DCR_EM2_EMI_BASE);

    int         i;
    uint32_t    data;

    data = l2c_l2_read (DCR_L2C_BASE, L2C_L2ISTAT);
    i = 0;
    while (((data & 0x1) != 1) && (i < L2C_TIMEOUT))
    {
        data = l2c_l2_read (DCR_L2C_BASE, L2C_L2ISTAT);
        i++;
    }
    TEST_ASSERT((data & 0x1),"L2C Array Initialization Complete Event did not occur!");

    rumboot_printf("Copy SRAM0BIN to IM0 (addr = 0x%x)\n", im0_data);
    rumboot_platform_request_file("SRAM0BIN", (uint32_t) im0_data);
    rumboot_printf("Copy successful (data = 0x%x)\n", ioread32((uint32_t)im0_data));
    rumboot_printf("Prepare DMA (im0 -> sram0)...\n");

    dma2plb6_setup_info dma_info;
    channel_status status = {};
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
        dma2plb6_trace_status(status);
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
            case 1: rumboot_printf("steps 6,7 - FAIL, reading from cache inhibited page: failed, data not changed (cache is off).\n"); break;
            case 2: rumboot_printf("steps 6,7 - FAIL, reading from cache inhibited page: failed, data not changed (cache is on).\n"); break;
            case 3: rumboot_printf("steps 6,7 - FAIL, reading from cache inhibited page: failed, data changed.\n"); break;
            case 4: rumboot_printf("steps 8,9 - FAIL, reading from cacheable page: failed, data not changed.\n"); break;
            case 5: rumboot_printf("steps 11, 12 - FAIL, reading from cache inhibited page after flush: failed, data not changed.\n"); break;
            case 6: rumboot_printf("steps 13, 14 - FAIL, reading from cacheable page after flush: failed, data not changed.\n"); break;
        }
        rumboot_printf("TEST_ERROR\n");
        return 1;
    }

    rumboot_printf("TEST_OK\n");
    return 0;
}
