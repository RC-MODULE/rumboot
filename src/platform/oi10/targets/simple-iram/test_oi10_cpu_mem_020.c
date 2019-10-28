/*
 * test_oi10_cpu_mem_020.c
 *
 *  Created on: Jan 15, 2019
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

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

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>

#define L2C_WAY_SIZE    128
#define DATA_LEN        (L2C_WAY_SIZE >> 3)


const uint32_t test_addr[] = { //len = 14
    0x00000000,
    0x00000080,
    0x00000F00,
    0x00000F80,
    0x00003300,
    0x00003380,
    0x00005500,
    0x00005580,
    0x0000AA00,
    0x0000AA80,
    0x0000CC00,
    0x0000CC80,
    0x0000F000,
    0x0000FF80
};

const uint64_t test_data[] = { //len = 14
    0x0000000000000000,
    0xFFFFFFFF00000000,
    0x00000000FFFFFFFF,
    0xFFFF0000FFFF0000,
    0x0000FFFF0000FFFF,
    0xFF00FF00FF00FF00,
    0x00FF00FF00FF00FF,
    0xF0F0F0F0F0F0F0F0,
    0x0F0F0F0F0F0F0F0F,
    0x3333333333333333,
    0x5555555555555555,
    0xAAAAAAAAAAAAAAAA,
    0xCCCCCCCCCCCCCCCC,
    0xFFFFFFFFFFFFFFFF
};

/*                           MMU_TLB_ENTRY(  ERPN,     RPN,         EPN,          DSIZ,              IL1I,   IL1D,    W,      I,  M,G,E,                 UX,UW,UR,SX,SW,SR DULXE,IULXE,TS,TID,              WAY,              BID,             V )*/
#define TLB_ENTRY0_CACHE_WT  MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

volatile uint64_t *cashed_data;

void check_read_write_via_l2carracc(int indx)
{
    int i;
    uint32_t ext_phys_addr = (uint32_t) (rumboot_virt_to_phys( (void *)test_addr[indx] ) >> 32),
             phys_addr = (uint32_t)(rumboot_virt_to_phys( (void *)test_addr[indx] ) & 0xFFFFFFFF);

    rumboot_printf("Init cache data... ");

    cashed_data = (volatile uint64_t *)test_addr[indx];

    for (i = 0; i < DATA_LEN; i++)
    {
        cashed_data[i] = test_data[indx];
        dcbst((void *)&cashed_data[i]);
    }
    msync();
    rumboot_printf("done\n");

    int cache_way = -1;
    rumboot_printf("l2c get way\n");
    if (l2c_arracc_get_way_by_address( DCR_L2C_BASE, ext_phys_addr, phys_addr, &cache_way ) == false)
    {
        rumboot_printf("==> Error reading (way) via L2ARRACC* <==\n");
        TEST_ASSERT(0, "TEST_ERROR");
    }

    rumboot_printf("Start reading via L2ARRACC*\n\n");
    for (i = 0; i < DATA_LEN; i++ )
    {
        uint32_t tag_data = 0;
        uint64_t l2c_data = 0;

        rumboot_printf("addr = %x_%x (i = %x)\n", ext_phys_addr, phys_addr+(i<<3), i);
        rumboot_printf("cache way == %x\n", cache_way);

        //**********************************************************************************

        rumboot_printf("l2c get tag info\n");
        if (l2c_arracc_tag_info_read_by_way (DCR_L2C_BASE, ext_phys_addr, phys_addr+(i<<3), cache_way, &tag_data) == false)
        {
            rumboot_printf("==> Error reading (tag) via L2ARRACC* <==\n");
            TEST_ASSERT(0, "TEST_ERROR");
        }
        rumboot_printf("tag info == %x\n", tag_data);

        rumboot_printf ("tag validation... ");
        uint32_t tmp_tag = (1 << 31) + (((ext_phys_addr << 16) + ((phys_addr) >> 16)) << 3);
        TEST_ASSERT(tag_data == tmp_tag,"TEST ERROR: invalid tag");
        rumboot_printf("done\n");

        //**********************************************************************************
        rumboot_printf("l2c get data\n");
        if(l2c_arracc_data_read_by_way(DCR_L2C_BASE, ext_phys_addr, phys_addr+(i<<3), cache_way, &l2c_data) == false)
        {
            rumboot_printf("==> Error reading (data) via L2ARRACC* <==\n");
            TEST_ASSERT(0, "TEST_ERROR");
        }
        msync();
        rumboot_printf("data_h == %x\n", (uint32_t) (l2c_data >> 32));
        rumboot_printf("data_l == %x\n", (uint32_t) l2c_data);

        rumboot_printf ("data validation... ");
        TEST_ASSERT(l2c_data == cashed_data[i],"TEST ERROR: invalid data");
        rumboot_printf("done\n\n");
    }
}

int main(void)
{
    static const tlb_entry tlb_entry_wt =
       { TLB_ENTRY0_CACHE_WT };

    emi_init(DCR_EM2_EMI_BASE);

    TEST_ASSERT(l2c_l2_read (DCR_L2C_BASE, L2C_L2ISTAT),"L2C Array Initialization Complete Event did not occur!");

    rumboot_printf("Start memory initialization...\n");
    memset(0x00, 0x00, 0x10000);
    rumboot_printf("Memory initialization done\n");

    rumboot_printf("Start TLB entries initialization... ");
    write_tlb_entries(&tlb_entry_wt, 1);
    rumboot_printf("done\n");


    for (uint32_t i = 0; i < ARRAY_SIZE(test_addr); i++)
    {
        check_read_write_via_l2carracc(i);
    }

    rumboot_printf("TEST OK\n");
    return 0;
}

