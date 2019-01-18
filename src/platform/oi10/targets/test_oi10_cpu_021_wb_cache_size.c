/*
 * test_oi10_cpu_021_wt_100.c
 *
 *  Created on: Sep 3, 2018
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <rumboot/memfill.h>

#define PTRN_INC_1 0x1
#define PTRN_INC_2 0x2

#define START_ADDR          (SRAM0_BASE + 0x10000)
#define DATA_SIZE           0x40000  //256KB
#define NUM_WORDS           (DATA_SIZE/4)
#define NUM_DWORDS          (NUM_WORDS/2)
#define L2C_LINE_SIZE       128
#define GET_EXP_DATA(a, i)  (((a >> 2) + 1) * i)

bool __attribute__((section(".text.test"))) cache_testing_function()
{
    rumboot_printf ("Start testing function\n");

    rumboot_printf ("1. Write mem (pattern 1)\n");
    rumboot_memfill32( (void*)START_ADDR, NUM_WORDS + (L2C_LINE_SIZE/4), 0x00, PTRN_INC_1);
    msync();

    rumboot_printf ("2. Read and check first 16 dwords\n");
    for (uint8_t i = 0; i < (L2C_LINE_SIZE/sizeof(uint64_t)); i++) //L2C_LINE_SIZE/sizeof(uint64_t) == 16
    {
        rumboot_printf ("i = %x\n", i);
        uint32_t addr = START_ADDR + 8*i;
        uint64_t rdata = ioread64(addr);
        msync();

        if ( ((rdata << 32) == GET_EXP_DATA(addr, PTRN_INC_1)) && ((rdata & 0xFFFFFFFF) == GET_EXP_DATA((addr+0x4), PTRN_INC_1)) )
        {
            rumboot_printf ("ERROR: invalid read data");
            return false;
        }

        //l2c get way
        int32_t cache_way = -1;
        uint64_t phys_addr = rumboot_virt_to_phys((void *) addr);
        if (l2c_arracc_get_way_by_address( DCR_L2C_BASE, phys_addr >> 32, phys_addr & 0xFFFFFFFF, &cache_way ) == false)
        {
            rumboot_printf("==> Error reading (way) via L2ARRACC* <==\n");
            return false;
        }

        //l2c get data
        uint64_t l2c_data = 0;
        if(l2c_arracc_data_read_by_way(DCR_L2C_BASE, phys_addr >> 32, phys_addr & 0xFFFFFFFF, cache_way, &l2c_data) == false)
        {
            rumboot_printf("==> Error reading (data) via L2ARRACC* <==\n");
            return false;
        }

        if (l2c_data != rdata)
        {
            rumboot_printf("ERROR: invalid cache data\n");
            rumboot_printf("l2c_data_h = %x\n", l2c_data << 32);
            rumboot_printf("l2c_data_l = %x\n", l2c_data & 0xFFFFFFFF);
            return false;
        }
    }

    rumboot_printf ("Testing function completed\n");
    return true;
}
