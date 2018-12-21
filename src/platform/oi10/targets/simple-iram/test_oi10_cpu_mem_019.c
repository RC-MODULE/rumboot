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

#include <platform/common_macros/common_macros.h>

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>

#define PRINT_READ_WRITE_L2ARRACC_INFO

//0x00000000_00000000ULL - 0x00000001_FFFFFFFFULL - SRAM_SDRAM EM2

#define L2C_SIZE        (256*1024)
#define L2C_WAY_SIZE    128
#define L2C_TIMEOUT     0x00000020

#define ADDR_ARRAY_LEN  12
#define DATA_LEN        (L2C_WAY_SIZE >> 3)


const uint64_t test_phys_addr[ADDR_ARRAY_LEN] = {
    0x000000100ULL,
    0x0000FFF80ULL,
    0x00FF00F80ULL,
    0x0CCCCCC80ULL,
    0x0AAAAAA80ULL,
    0x000F0F080ULL,
    0x10F0F0F00ULL,
    0x1000FF000ULL,
    0x133333300ULL,
    0x155555500ULL,
    0x10FF00000ULL,
    0x10FFFFF80ULL
};

const uint32_t test_addr[ADDR_ARRAY_LEN] = {
    0x00010100,
    0x0002FF80,
    0x00030F80,
    0x0004CC80,
    0x0005AA80,
    0x0006F080,
    0x00070F00,
    0x0008F000,
    0x00093300,
    0x000A5500,
    0x000B0000,
    0x000CFF80,
};

/*                           MMU_TLB_ENTRY( ERPN, RPN,    EPN,    DSIZ,              IL1I,IL1D,W,I,M,G,E,                 UX,UW,UR,SX,SW,SR DULXE,IULXE,TS,TID,              WAY,              BID,             V )*/
#define TLB_ENTRY0_CACHE_WB  MMU_TLB_ENTRY( 0x000,0x00000,0x00010,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY1_CACHE_WB  MMU_TLB_ENTRY( 0x000,0x000F0,0x00020,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY2_CACHE_WB  MMU_TLB_ENTRY( 0x000,0x0FF00,0x00030,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY3_CACHE_WB  MMU_TLB_ENTRY( 0x000,0xCCCC0,0x00040,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY4_CACHE_WB  MMU_TLB_ENTRY( 0x000,0xAAAA0,0x00050,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY5_CACHE_WB  MMU_TLB_ENTRY( 0x000,0x00F00,0x00060,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY6_CACHE_WB  MMU_TLB_ENTRY( 0x001,0x0F0F0,0x00070,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY7_CACHE_WB  MMU_TLB_ENTRY( 0x001,0x000F0,0x00080,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY8_CACHE_WB  MMU_TLB_ENTRY( 0x001,0x33330,0x00090,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY9_CACHE_WB  MMU_TLB_ENTRY( 0x001,0x55550,0x000A0,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY10_CACHE_WB MMU_TLB_ENTRY( 0x001,0x0FF00,0x000B0,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )
#define TLB_ENTRY11_CACHE_WB MMU_TLB_ENTRY( 0x001,0x0FFF0,0x000C0,MMU_TLBE_DSIZ_64KB,1,   1,   1,0,1,1,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_SHARED,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )

#define CREATE_DWORD(addr,i) (((addr << 12) + i) | ( ( 0x000000000000BEEFULL + ( i << 16 ) ) << 32 ))

volatile uint64_t *cashed_data;
volatile uint64_t ref_data[DATA_LEN];
volatile uint64_t l2c_data[DATA_LEN];

void check_read_write_via_l2carracc(int indx)
{
    int i;
    uint32_t ext_phys_addr,phys_addr;

    ext_phys_addr = (uint32_t)(test_phys_addr[indx] >> 32);
    phys_addr = (uint32_t)(test_phys_addr[indx] & 0xFFFFFFFF);
    rumboot_printf("Check read/write via L2ARRACC* started (ext_phys_addr = 0x%x; phys_addr = 0x%x)\n",ext_phys_addr,phys_addr);

    rumboot_printf("  Init data... ");

    cashed_data = (volatile uint64_t *)test_addr[indx];

    for (i = 0; i < DATA_LEN; i++)
    {
        cashed_data[i] = CREATE_DWORD(test_addr[indx],i);
        dcbst((uint64_t *)&cashed_data[i]);
    }
    msync();
    rumboot_printf("done\n");

    rumboot_printf("  Start reading... ");
    for (i = 0; i < DATA_LEN; i++ )
    {
        ref_data[i] = cashed_data[i];
    }
    msync();
    rumboot_printf("done\n");

    rumboot_printf("  Start reading via L2ARRACC*\n");
    for (i = 0; i < DATA_LEN; i++ )
    {
        #ifdef PRINT_READ_WRITE_L2ARRACC_INFO
            if(l2c_arracc_get_data_by_address(DCR_L2C_BASE,ext_phys_addr,phys_addr+(i<<3),&l2c_data[i]) == false)
            {
                rumboot_printf("==> Error reading via L2ARRACC* <==\n");
                TEST_ASSERT(0, "TEST_ERROR");
            }
        #else
            l2c_data[i] = l2c_arracc_data_read(DCR_L2C_BASE, ext_phys_addr,phys_addr+(i<<3));
            if(i & 0x1F) rumboot_printf(".");
            else rumboot_printf("\n  .");
        #endif
        rumboot_printf("\n");
    }
    msync();
    rumboot_printf(" done\n\n");

    rumboot_printf("  Check data... ");
    for (i = 0; i < DATA_LEN; i++)
    {
        TEST_ASSERT(l2c_data[i] == ref_data[i],"Data not equal!");
    }
    rumboot_printf("done\n");
}

int main(void)
{
    int i;
    volatile uint32_t data;

    static const tlb_entry tlb_entry[ADDR_ARRAY_LEN] =
       {{TLB_ENTRY0_CACHE_WB}, {TLB_ENTRY1_CACHE_WB}, {TLB_ENTRY2_CACHE_WB}, {TLB_ENTRY3_CACHE_WB},
        {TLB_ENTRY4_CACHE_WB}, {TLB_ENTRY5_CACHE_WB}, {TLB_ENTRY6_CACHE_WB}, {TLB_ENTRY7_CACHE_WB},
        {TLB_ENTRY8_CACHE_WB}, {TLB_ENTRY9_CACHE_WB}, {TLB_ENTRY10_CACHE_WB},{TLB_ENTRY11_CACHE_WB}};

    emi_init(DCR_EM2_EMI_BASE);

    data = l2c_l2_read (DCR_L2C_BASE, L2C_L2ISTAT);
    i = 0;
    while (((data & 0x1) != 1) && (i < L2C_TIMEOUT))
    {
        data = l2c_l2_read (DCR_L2C_BASE, L2C_L2ISTAT);
        i++;
    }
    TEST_ASSERT((data & 0x1),"L2C Array Initialization Complete Event did not occur!");
    msync();

    rumboot_printf("Start TLB entries initialization... ");
    write_tlb_entries(tlb_entry, ADDR_ARRAY_LEN);
    rumboot_printf("done\n");

    rumboot_printf("Start memory initialization... ");
    for (i = 0; i < ADDR_ARRAY_LEN; ++i)
    {
        memset((uint32_t *)test_addr[i], 0, L2C_WAY_SIZE);
    }
    rumboot_printf("done\n");

    for (i = 0; i < ADDR_ARRAY_LEN; ++i)
    {
        check_read_write_via_l2carracc(i);
    }

    rumboot_printf("TEST OK\n");
    return 0;
}

