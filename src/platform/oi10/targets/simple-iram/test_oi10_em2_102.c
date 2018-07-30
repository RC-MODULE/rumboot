#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>



#define TLB_ENTRY_CACHE_ON    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_OFF   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b1 )


//const uint32_t EVENT_OI10_EM2_102_CHECK_CACHE_OFF = 0x00001000;
//const uint32_t EVENT_OI10_EM2_102_CHECK_CACHE_ON = 0x00001001;


//uint32_t check_instruction_read_access_em2() __attribute__((section(".EM2.text")));
uint32_t check_instruction_read_access_sram0()
{
    uint32_t i, j, k, l, m, n;

    for(i=0; i<32; i++)
    {
//        rumboot_printf("Before: Reading instructions from SRAM0 is OK\n");
        j=i+i;
        k=2-i;
        l=i*i;
        m=10*i;
        n=13+i;
        rumboot_printf("After: Reading instructions from SRAM0 is OK\n");
        rumboot_printf("i = %d, j = %d, k = %d, l = %d, m = %d, n = %d\n", i, j, k, l, m, n);
//        rumboot_printf("After: Reading instructions from SRAM0 is OK\n");
    };

    rumboot_printf("Reading instructions from SRAM0 is OK\n");

    return 0xBABADEDA;
};


int main(void)
{
    emi_init();

    rumboot_printf("Copy code from IM0 to SRAM0 memory ... \n");

    uint32_t check_instruction_read_access_sram0_start_addr = (uint32_t)&check_instruction_read_access_sram0;
    uint32_t check_instruction_read_access_sram0_end_addr = (uint32_t)&main;

    //rumboot_printf("check_instruction_read_access_sram0_start_addr = 0x%x\n", check_instruction_read_access_sram0_start_addr);
    //rumboot_printf("check_instruction_read_access_sram0_end_addr = 0x%x\n", check_instruction_read_access_sram0_end_addr);

    for (uint32_t addr = check_instruction_read_access_sram0_start_addr, sram0_offset = 0;
            addr < check_instruction_read_access_sram0_end_addr;
            addr+=4, sram0_offset+=4) {
//        uint32_t val = ioread32(addr);
//        rumboot_printf("source addr = 0x%x, val = 0x%x, dest addr = 0x%x\n", addr, val, (SRAM0_BASE + sram0_offset));
        iowrite32(ioread32(addr), SRAM0_BASE + sram0_offset);
    }

    rumboot_printf("Check SRAM0 instructions reading (CACHE OFF)\n");

    static const tlb_entry sram0_tlb_entry_cache_off = {TLB_ENTRY_CACHE_OFF};
    write_tlb_entries(&sram0_tlb_entry_cache_off, 1);
    msync();
    isync();

    rumboot_printf("Testing ... \n");
    //test_event(EVENT_OI10_EM2_102_CHECK_CACHE_OFF);
    TEST_ASSERT(((uint32_t (*)())(SRAM0_BASE))() == 0xBABADEDA, "Error occurred during SRAM0 function call");


    //cache on
    rumboot_printf("Check SRAM0 instructions reading (CACHE ON)\n");

    static const tlb_entry sram0_tlb_entry_cache_on = {TLB_ENTRY_CACHE_ON};
    write_tlb_entries(&sram0_tlb_entry_cache_on, 1);
    msync();
    isync();

    rumboot_printf("Testing ... \n");
    //test_event(EVENT_OI10_EM2_102_CHECK_CACHE_ON);
    TEST_ASSERT(((uint32_t (*)())(SRAM0_BASE))() == 0xBABADEDA, "Error occurred during SRAM0 function call");

    return 0;
}
