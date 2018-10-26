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
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_l2c_l2.h>

//#define L2C_IL1I_BIT, L2C_IL1D_BIT, L2C_WRITE_MODE_BIT, L2C_INHIBIT_BIT, L2C_MEMORY_COHERENCE_BIT see in oi10.cmake

                                 //MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,            IL1D,            W,                     I,               M,                          G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,           V   )
#define TLB_ENTRY_NOCACHE_VALID    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,             0b1,             0b0,                   0b1,             0b0,                        0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_NOCACHE_INVALID  MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,             0b1,             0b0,                   0b1,             0b0,                        0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY_CACHE_VALID      MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      L2C_IL1I_BIT,    L2C_IL1D_BIT,    L2C_WRITE_MODE_BIT,    L2C_INHIBIT_BIT, L2C_MEMORY_COHERENCE_BIT,   0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_INVALID    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      L2C_IL1I_BIT,    L2C_IL1D_BIT,    L2C_WRITE_MODE_BIT,    L2C_INHIBIT_BIT, L2C_MEMORY_COHERENCE_BIT,   0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )

#define TEST_CONST_1     0x7E57DA7A
#define TEST_CONST_2     0xBABADEDA

uint32_t __attribute__((section(".data"),aligned(16))) volatile sram0_data[64] = { 0 };

uint8_t __attribute__((section(".text.test_oi10_cpu_021"))) cache_testing_function()
{
    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_VALID};
    static const tlb_entry sram0_tlb_entry_non_cacheable_valid = {TLB_ENTRY_NOCACHE_VALID};

    //step 1 - initial value to SRAM0
    iowrite32 (TEST_CONST_1, (uint32_t) sram0_data);
    msync();

    //step 2 - switch to cacheable page
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid,1);

    //steps 3, 4 - put data in cacheable page
    iowrite32(TEST_CONST_2, (uint32_t) sram0_data);
    msync();

    //switch to inhibited page
    write_tlb_entries(&sram0_tlb_entry_non_cacheable_valid,1);
    msync();
    isync();

    //steps 6,7 - read from cache inhibited page
    if (L2C_INHIBIT_BIT)
    {
      if (ioread32((uint32_t) sram0_data) != TEST_CONST_2) return 1;
    }
    else
    {
      if (L2C_WRITE_MODE_BIT)
        {if (ioread32((uint32_t) sram0_data) != TEST_CONST_2) return 2;}
      else
        {if (ioread32((uint32_t) sram0_data) != TEST_CONST_1) return 3;}
    }
    msync();

    //steps 8,9 - read from cacheable page
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid,1);
    msync();
    isync();
     if (ioread32((uint32_t) sram0_data) != TEST_CONST_2) return 4;
    msync();

    //step 10 - cache flush
    dcbf((uint32_t *) sram0_data);
    msync();

    //steps 11,12 - read new data from cache inhibited page
    write_tlb_entries(&sram0_tlb_entry_non_cacheable_valid,1);
    msync();
    isync();
    if (ioread32((uint32_t) sram0_data) != TEST_CONST_2) return 5;
     msync();

    //steps 13,14 - read new data from cacheable page
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid,1);
    msync();
    isync();
    if (ioread32((uint32_t) sram0_data) != TEST_CONST_2) return 6;
    msync();

    //steps 16,17
    //write in cache
    dci(2);
    iowrite32 (TEST_CONST_1, (uint32_t) sram0_data);
    //inhibit page
    write_tlb_entries(&sram0_tlb_entry_non_cacheable_valid,1);
    iowrite32 (TEST_CONST_2, (uint32_t) sram0_data);
    if (ioread32((uint32_t) sram0_data) != TEST_CONST_2) return 7;
    //cacheable page
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid,1);
    //read from cache
    if (L2C_INHIBIT_BIT)
    {
        if (ioread32((uint32_t) sram0_data) != TEST_CONST_2) return 8;
    }
    else
        if (ioread32((uint32_t) sram0_data) != TEST_CONST_1) return 9;

    return 0;
}
