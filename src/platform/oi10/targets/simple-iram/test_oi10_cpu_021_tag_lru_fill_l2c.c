/*
 * test_oi10_cpu_021_tag_lru_fill_l2c.c
 *
 *  Created on: JAN 9, 2019
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
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/mem_window.h>
#include <devices/ugly/emi.h>
#include <platform/devices/l2c.h>

                         //MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,    W,      I,      M,      G,        E,                   UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,      TID,               WAY,                BID,                V   )
#define TLB_ENTRY_WB_W0    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_WB_W3    MMU_TLB_ENTRY(  0x001,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define START_ADDR      (SRAM0_BASE) // = 0x00
#define DATA_SIZE       0x40000      // L2_SIZE = 256KB
#define WORD_NUM        0x800        // 2048 (DATA_SIZE/128)
#define CACHE_LINE_SIZE 128          // L2
#define GET_DATA(x)     ((x << 16) + x)

static void init_test_data (void)
{
    rumboot_memfill8_modelling((void*)START_ADDR,  DATA_SIZE, 0x00, 0x00);
    for (uint32_t ind = 0, addr = START_ADDR; ind < WORD_NUM; ind++ , addr += CACHE_LINE_SIZE)
        iowrite32(GET_DATA(ind), addr);
    msync();
}

static bool check_mem (void)
{
    rumboot_printf("Number of words = 0x%x\n", WORD_NUM);
    uint32_t lru_bits_tmp = 1;

    for (uint32_t ind = 0, addr = START_ADDR; ind < WORD_NUM; ind++ , addr += CACHE_LINE_SIZE)
    {
        rumboot_printf("word = %x, ", ind);
        //read and modification
        uint32_t exp_data = ~(ioread32(addr));

        //write modified value
        iowrite32(exp_data, addr);
        msync();

        //check with L2C Array Interface
        uint32_t ext_phys_addr = (uint32_t) (rumboot_virt_to_phys( (void *)addr ) >> 32),
                 phys_addr = (uint32_t)(rumboot_virt_to_phys( (void *)addr ) & 0xFFFFFFFF);
        rumboot_printf("paddr = %x_%x, ", ext_phys_addr, phys_addr);

        //get way
        int cache_way = -1;
        if (l2c_arracc_get_way_by_address( DCR_L2C_BASE, ext_phys_addr, phys_addr, &cache_way ) == false)
        {
            rumboot_printf("ERROR: reading (way) via L2ARRACC*\n");
            return false;
        }
        rumboot_printf("cache way = %x\n", cache_way);

        //get tag
        uint32_t tag_data = 0;
        if (l2c_arracc_tag_info_read_by_way (DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &tag_data) == false)
        {
            rumboot_printf("ERROR: reading (tag) via L2ARRACC*\n");
            return false;
        }
        uint32_t exp_tag = (3 << 30) + (((ext_phys_addr << 16) + ((phys_addr) >> 16)) << 3); //cache state[31:29] == 0b110 == modified
        if (tag_data != exp_tag)
        {
            rumboot_printf("exp_tag = %x\nact_tag = %x\n", exp_tag, tag_data);
            rumboot_printf("ERROR: invalid tag\n");
            return false;
        }

        //get lru
        uint32_t lru_data = 0;
        if (l2c_arracc_lru_info_read (DCR_L2C_BASE, ext_phys_addr, phys_addr, &lru_data) == false)
        {
            rumboot_printf("ERROR: reading (lru) via L2ARRACC*\n");
            return false;
        }

        if (ind%0x200 == 0)
        {
            if( (lru_data & 0xf8000000) == lru_bits_tmp )
            {
                rumboot_printf ("ERROR: lru bits not changed\n");
                return false;
            }
            lru_bits_tmp = lru_data & 0xf8000000;

            switch (cache_way)
            {
            case 0:
                if ( (lru_data & 0x3FC) != (1 << 9) )
                {
                    rumboot_printf ("ERROR: inclusive bits (way0) not correct\n");
                    return false;
                }
                break;
            case 1:
                if ( (lru_data & 0x3FC) != ((1 << 9) | (1 << 7)) )
                {
                    rumboot_printf ("ERROR: inclusive bits (way1) not correct\n");
                    return false;
                }
                break;
            case 2:
                if ( (lru_data & 0x3FC) != ((1 << 9) | (1 << 7) | (1 << 5)) )
                {
                    rumboot_printf ("ERROR: inclusive bits (way2) not correct\n");
                    return false;
                }
                break;
            case 3:
                if ( (lru_data & 0x3FC) != ((1 << 9) | (1 << 7) | (1 << 5) | (1 << 3)) )
                {
                    rumboot_printf ("ERROR: inclusive bits (way3) not correct\n");
                    return false;
                }
                break;
            }
        }
        else
            if ( (lru_data & 0xf8000000) != lru_bits_tmp )
            {
                rumboot_printf ("ERROR: lru bits changed (not expected)\n");
            }

    }
    return true;
}

int main (void)
{
    rumboot_printf("Emi init\n");
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Init data\n");
    init_test_data ();
    rumboot_printf("Data initialized\n");

    rumboot_printf("Set tlb (l1, l2 cache on)\n");
    static const tlb_entry sram0_tlb_entry_write_back = {TLB_ENTRY_WB_W0};
    write_tlb_entries(&sram0_tlb_entry_write_back, 1);

    rumboot_printf("Read, modification and check with L2C Array Interface\n");

    if ( !check_mem () ) {
        rumboot_printf ("TEST ERROR\n");
        return 1;
    }

    rumboot_printf("TEST OK\n");
    return 0;
}
