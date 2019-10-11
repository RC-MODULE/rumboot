/*
 * test_oi10_cpu_mem_016.c
 *
 *  Created on: Dec 21, 2018
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


// FixMe: Hack
#ifndef L2C_TIMEOUT
    #define L2C_TIMEOUT     0x00000020
#endif

const MEM_WINDOW test_windows[] =
{
    MEM_WINDOW_0,
    //MEM_WINDOW_1,
    //MEM_WINDOW_2,
    MEM_WINDOW_3
};

const uint32_t test_addr[] = {
    0x00000000,
    0x0000FF80,
    0x3FFF0000,
    0x00FF0080,
    0x3F000000,
    0x0F0F0F00,
    0x60F0F080,
    0x33333380,
    0x55555580,
    0x6AAAAA80,
    0x6CCCCC80,
    0x6FFFFF80
};

/*                           MMU_TLB_ENTRY(  ERPN,     RPN,         EPN,          DSIZ,              IL1I,   IL1D,    W,      I,  M,G,E,                 UX,UW,UR,SX,SW,SR DULXE,IULXE,TS,TID,              WAY,              BID,             V )*/
#define TLB_ENTRY0_CACHE_WT  MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY1_CACHE_WT  MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY2_CACHE_WT  MMU_TLB_ENTRY(  0x000,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY3_CACHE_WT  MMU_TLB_ENTRY(  0x000,  0xc0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY4_CACHE_WT  MMU_TLB_ENTRY(  0x001,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY5_CACHE_WT  MMU_TLB_ENTRY(  0x001,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY6_CACHE_WT  MMU_TLB_ENTRY(  0x001,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY7_CACHE_WT  MMU_TLB_ENTRY(  0x001,  0xc0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define CREATE_DWORD(addr,i) (((addr << 12) + i) | ( ( 0x000000000000BEEFULL + ( i << 16 ) ) << 32 ))

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
        cashed_data[i] = CREATE_DWORD(test_addr[indx],i);
        dcbst((void *)&cashed_data[i]);
    }
    msync();
    rumboot_printf("done\n");

    int32_t cache_way = -1;
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
    static const tlb_entry tlb_entry_wt[] =
       {
        {TLB_ENTRY0_CACHE_WT}, {TLB_ENTRY1_CACHE_WT}, {TLB_ENTRY2_CACHE_WT}, {TLB_ENTRY3_CACHE_WT},
        {TLB_ENTRY4_CACHE_WT}, {TLB_ENTRY5_CACHE_WT}, {TLB_ENTRY6_CACHE_WT}, {TLB_ENTRY7_CACHE_WT},
       };

    emi_init(DCR_EM2_EMI_BASE);

    volatile uint32_t data;
    data = l2c_l2_read (DCR_L2C_BASE, L2C_L2ISTAT);
    int i = 0;
    while (((data & 0x1) != 1) && (i < L2C_TIMEOUT))
    {
        data = l2c_l2_read (DCR_L2C_BASE, L2C_L2ISTAT);
        i++;
    }
    TEST_ASSERT((data & 0x1),"L2C Array Initialization Complete Event did not occur!");
    msync();

    rumboot_printf("Start memory initialization...\n");
    for ( i = 0; i < ARRAY_SIZE(test_windows); i++)
    {
        set_mem_window(test_windows[i]);
        for (uint32_t j = 0; j < ARRAY_SIZE(test_addr); j++)
        {
            for (uint32_t k = 0; k < (L2C_WAY_SIZE >> 2); k++)
            {
               iowrite32(0x00, test_addr[j] + 4*k);
            }
            msync();
        }
    }
    rumboot_printf("Memory initialization done\n");

    rumboot_printf("Start TLB entries initialization... ");
    write_tlb_entries(tlb_entry_wt, ARRAY_SIZE(tlb_entry_wt));
    rumboot_printf("done\n");


    for ( i = 0; i < ARRAY_SIZE(test_windows); i++)
    {
        set_mem_window(test_windows[i]);
        for (uint32_t j = 0; j < ARRAY_SIZE(test_addr); j++)
        {
            check_read_write_via_l2carracc(j);
        }
    }

    rumboot_printf("TEST OK\n");
    return 0;
}

