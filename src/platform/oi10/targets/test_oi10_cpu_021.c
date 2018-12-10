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
#define TLB_ENTRY_CACHE_VALID      MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      L2C_IL1I_BIT,    L2C_IL1D_BIT,    L2C_WRITE_MODE_BIT,    L2C_INHIBIT_BIT, L2C_MEMORY_COHERENCE_BIT,   0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define BASE_INIT_VALUE  0x00
uint32_t __attribute__((section(".data"),aligned(32))) volatile sram0_data[64] = { BASE_INIT_VALUE };

uint8_t __attribute__((section(".text.test"))) cache_testing_function()
{
    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_VALID};
    static const tlb_entry sram0_tlb_entry_non_cacheable_valid = {TLB_ENTRY_NOCACHE_VALID};

    uint32_t addr = (uint32_t) sram0_data;
    rumboot_printf("1. ADDR = 0x%x, INIT VALUE = 0x%x\n", addr, BASE_INIT_VALUE);

    //2
    rumboot_printf("2. Cache on\n");
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid,1); //cache on

    //3
    const uint32_t const_A = 0xAAAAAAAA;
    rumboot_printf("3. A = 0x%x\n", const_A);

    //4
    rumboot_printf("4. Write A in 0x%x\n", addr);
    iowrite32(const_A, addr);
    msync();

    //5
    rumboot_printf("5. Cache off\n");
    write_tlb_entries(&sram0_tlb_entry_non_cacheable_valid,1); //cache off

    //6
    rumboot_printf("6. Read from 0x%x\n", addr);
    uint32_t read_data = ioread32 (addr);
    msync();

    //7
    rumboot_printf("7. Check read data\n");
    if (L2C_INHIBIT_BIT)
    {
      if (read_data != const_A) return 1;
    }
    else
    {
      if (L2C_WRITE_MODE_BIT)
        {if (ioread32((uint32_t) sram0_data) != const_A) return 1;}
      else
        {if (ioread32((uint32_t) sram0_data) != BASE_INIT_VALUE) return 1;}
    };

    const uint32_t const_B = 0xBBBBBBBB;
    if (L2C_WRITE_MODE_BIT)
    {
        //8
        rumboot_printf("8. B = 0x%x\n", const_B);

        //9
        rumboot_printf("9. Write B in 0x%x\n", addr);
        iowrite32(const_B, addr);
        msync();
    }
    else
        rumboot_printf("8. Skip for this test (available for WT only)\n9. Skip for this test (available for WT only)\n");

    //10
    rumboot_printf("10. Cache on. Read from 0x%x\n", addr);
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid,1); //cache on
    read_data = ioread32 (addr);
    msync();

    //11
    rumboot_printf("11. Check read data\n");
    if (read_data != const_A) return 2;

    if (L2C_WRITE_MODE_BIT)
    {
        //12
        rumboot_printf("12. Write A in 0x%x\n", addr);
        iowrite32(const_A, addr);
        msync();
    }
    else
        rumboot_printf("12. Skip for this test (available for WT only)\n");

    //13
    rumboot_printf("13. Flush (0x%x)\n", addr);
    dcbf((uint32_t *) addr);
    msync();

    //14
    rumboot_printf("14. Cache off. Read from 0x%x\n", addr);
    write_tlb_entries(&sram0_tlb_entry_non_cacheable_valid,1); //cache off
    read_data = ioread32 (addr);
    msync();

    //15
    rumboot_printf("15. Check read data\n");
    if (read_data != const_A) return 3;

    //16
    rumboot_printf("16. Cache on. Read from 0x%x\n", addr);
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid,1); //cache on
    read_data = ioread32 (addr);
    msync();

    //17
    rumboot_printf("17. Check read data\n");
    if (read_data != const_A) return 4;

    return 0;
}
