/*
 * test_oi10_cpu_021_tag_lru_fill_l1d.c
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
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>

                      //MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,    W,      I,      M,      G,        E,                   UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,      TID,               WAY,                BID,                V   )
#define TLB_ENTRY_WB    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define START_ADDR      SRAM0_BASE
#define DATA_SIZE       0x8000 // 32KB
#define WORD_NUM        0x400  // 1024 (DATA_SIZE/32)
#define CACHE_LINE_SIZE 32     // L1
#define GET_DATA(x)     ((x << 16) + x)

void init_test_data (void)
{
    rumboot_memfill8_modelling((void*)START_ADDR,  DATA_SIZE, 0x00, 0x00);
    for (uint32_t ind = 0, addr = START_ADDR; ind < WORD_NUM; ind++ , addr += CACHE_LINE_SIZE)
        iowrite32(GET_DATA(ind), addr);
    msync();
}

int main (void)
{
    rumboot_printf("Emi init\n");
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Init data\n");
    //1024 words, align 32
    init_test_data();
    rumboot_printf("Data initialized\n");

    rumboot_printf("Set tlb (l1, l2 cache on)\n");
    static const tlb_entry sram0_tlb_entry_write_back = {TLB_ENTRY_WB};
    write_tlb_entries(&sram0_tlb_entry_write_back, 1);

    rumboot_printf("Read, modification, write and check with dcread\n");
    rumboot_printf("Number of words = 0x%x\n", WORD_NUM);
    for (uint32_t ind = 0, addr = START_ADDR; ind < WORD_NUM; ind++ , addr += CACHE_LINE_SIZE)
    {
        rumboot_printf("word = %x, addr = %x\n", ind, addr);
        //read and modification
        uint32_t exp_data = ~(ioread32(addr)),
                 act_data = 0x00;

        //write modified value
        iowrite32(exp_data, addr);
        msync();

        //check with dcread
        act_data = dcread (addr);
        isync();
        uint32_t reg_DCDBTRH = spr_read(SPR_DCDBTRH),
                 reg_DCDBTRL = spr_read(SPR_DCDBTRL);

        TEST_ASSERT( exp_data == act_data,                "TEST ERROR: Invalid cache data");
        TEST_ASSERT( (reg_DCDBTRH >> 13) == (addr >> 13), "TEST_ERROR: Invalid tag in DCDBTRH");
        TEST_ASSERT( (reg_DCDBTRH & 0x1000) == 0x1000,    "TEST_ERROR: Valid bit in DCDBTRH[12] not expected");

        /*
         * addr = [0x0000 - 0x2000), way[14:13] = 0b00, LRUV = 0b1000
         * addr = [0x2000 - 0x4000), way[14:13] = 0b01, LRUV = 0b1100
         * addr = [0x4000 - 0x6000), way[14:13] = 0b10, LRUV = 0b1110
         * addr = [0x6000 - 0x8000), way[14:13] = 0b11, LRUV = 0b1111
         * >> supa formula:
        */
        //                LRUV                              ( WAY = {0,1,2,3}  )
        TEST_ASSERT( (reg_DCDBTRL >> 28) == ( (0xF << (3 - ((addr >> 13) & 0x3))) & 0xF), "TEST_ERROR: Invalid LRUV in DCDBTRL");
    }

    rumboot_printf("TEST OK\n");
    return 0;
}
