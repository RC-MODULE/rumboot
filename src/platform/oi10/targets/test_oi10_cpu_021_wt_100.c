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

#define L2C_INHIBIT_BIT 0b0
#define L2C_MEMORY_COHERENCE_BIT 0b1
                       //          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,               M,                          G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,           V   )
#define TLB_ENTRY_NOCACHE_VALID    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,             0b0,                        0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_NOCACHE_INVALID  MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,             0b0,                        0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY_CACHE_VALID      MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    L2C_INHIBIT_BIT, L2C_MEMORY_COHERENCE_BIT,   0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_INVALID    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    L2C_INHIBIT_BIT, L2C_MEMORY_COHERENCE_BIT,   0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )

#define TEST_MPW_CPU_021_CONST_BABADEDA     0xBABADEDA
#define TEST_MPW_CPU_021_CONST_7E57DA7A     0x7E57DA7A

uint32_t __attribute__((section(".data"),aligned(16))) volatile sram0_data[64] = { 0 };

void __attribute__((section(".text.test_oi10_cpu_021"))) cache_testing_function()
{
    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_VALID};
    static const tlb_entry sram0_tlb_entry_non_cacheable_valid = {TLB_ENTRY_NOCACHE_VALID};

    rumboot_printf("Cache_testing_function started %x\n", (uint32_t) sram0_data);

    //step 1 - initial value to EM0
    iowrite32 (TEST_MPW_CPU_021_CONST_7E57DA7A, (uint32_t) sram0_data);
    msync();
    rumboot_printf ("step 1 - SRAM0 initialized\n");

    //step 2 - switch page to write-through mode
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid,1);

    rumboot_printf("step 2 - switched page to write-back mode.\n");
    //steps 3, 4 - put data into write-through cacheable page
    iowrite32(TEST_MPW_CPU_021_CONST_BABADEDA, (uint32_t) sram0_data);
    rumboot_printf("steps 3, 4 - data sent to write-through cacheable page\n");
    msync();

    write_tlb_entries(&sram0_tlb_entry_non_cacheable_valid,1);
    msync();
    isync();
    rumboot_printf("step 5 - switched to cache inhibited page\n");

    //steps 6,7 - read from cache inhibited page
    if (L2C_INHIBIT_BIT)
        TEST_ASSERT(ioread32((uint32_t) sram0_data) == TEST_MPW_CPU_021_CONST_BABADEDA,"steps 6,7 - FAIL, reading from cache inhibited page: failed, data not changed (cache is off).");
    else
        TEST_ASSERT(ioread32((uint32_t) sram0_data) == TEST_MPW_CPU_021_CONST_7E57DA7A,"steps 6,7 - FAIL, reading from cache inhibited page: failed, data changed.");
    rumboot_printf("steps 6,7 - reading from cache inhibited page: OK\n");
    msync();

    //steps 8,9 - read from write-back cacheable page
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid,1);
    msync();
    isync();
    TEST_ASSERT(ioread32((uint32_t) sram0_data) == TEST_MPW_CPU_021_CONST_BABADEDA ,"steps 8,9 - FAIL, reading from cacheable page: failed, data not changed.") ;
    rumboot_printf("steps 8,9 - reading from write-back cacheable page: OK\n");
    msync();

    //step 10 - cache flush
    dcbf((uint32_t *) sram0_data);
    rumboot_printf("step 10 - cache flush\n");
    msync();

    //steps 11,12 - read new data from cache inhibited page
    write_tlb_entries(&sram0_tlb_entry_non_cacheable_valid,1);
    msync();
    isync();
    TEST_ASSERT(ioread32((uint32_t) sram0_data) == TEST_MPW_CPU_021_CONST_BABADEDA,"steps 11, 12 - FAIL, reading from cache inhibited page after flush: failed, data not changed.");
    rumboot_printf("steps 11,12 - reading from cache inhibited page after flush: OK\n");
    msync();

    //steps 13,14 - read new data from cacheable page
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid,1);
    msync();
    isync();
    TEST_ASSERT(ioread32((uint32_t) sram0_data) == TEST_MPW_CPU_021_CONST_BABADEDA,"steps 13, 14 - FAIL, reading from cacheable page after flush: failed, data not changed.");
    rumboot_printf("steps 13,14 - reading from cacheable page after flush: OK\n");
    msync();
}
