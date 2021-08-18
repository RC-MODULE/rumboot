#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <arch/common_macros.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/mem_window.h>
#include <devices/ugly/emi.h>
#include <arch/dma2plb6.h>


#define TLB_ENTRY_CACHE_ON    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_OFF   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b1 )


const uint32_t EVENT_OI10_EM2_102_CHECK_CACHE_OFF = 0x00001000;
const uint32_t EVENT_OI10_EM2_102_CHECK_CACHE_ON = 0x00001001;


//uint32_t check_instruction_read_access_em2() __attribute__((section(".EM2.text")));
uint32_t check_instruction_read_access_sram0_initial()
{
    uint32_t i, j, k, l, m, n;

    for(i=0; i<32; i++)
    {
        j=i+i;
        k=2-i;
        l=i*i;
        m=10*i;
        n=13+i;
    };

    iowrite32((j*k-l+n-m), (SRAM0_BASE + 512));

    return 0xBABADEDA;
};

uint32_t check_instruction_read_access_sram0_replace()
{
    uint32_t i, j, k, l, m, n;

    for(i=0; i<32; i++)
    {
        j=i+i;
        k=2-i;
        l=i*i;
        m=10*i;
        n=13+i;
    };

    iowrite32((j*k-l+n-m), (SRAM0_BASE + 512));

    return 0xDEDABABA;
};


int main(void)
{
    const uint32_t CALL_ITERATIONS_COUNT = 100;

    static struct dma2plb6_setup_info dma_setup_info =
    {
        .base_addr = DCR_DMAPLB6_BASE,
        .priority = priority_medium_low,
        .striding_info.striding = striding_none,
        .tc_int_enable = false,
        .err_int_enable = false,
        .int_mode = int_mode_level_wait_clr,
        .channel = channel0,
        .transfer_width = tr_width_byte,
        .rw_transfer_size = rw_tr_size_1q,
        .snp_mode =snp_mode_off
    };

    dma_setup_info.source_adr = rumboot_virt_to_phys((uint32_t*)&check_instruction_read_access_sram0_replace);
    dma_setup_info.dest_adr = rumboot_virt_to_phys((uint32_t*)SRAM0_BASE);
    dma_setup_info.count = ((uint32_t)&main - (uint32_t)&check_instruction_read_access_sram0_replace);


    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Copying code from IM0 to SRAM0 memory ... \n");

    uint32_t check_instruction_read_access_sram0_start_addr = (uint32_t)&check_instruction_read_access_sram0_initial;
    uint32_t check_instruction_read_access_sram0_end_addr = (uint32_t)&check_instruction_read_access_sram0_replace;

    dma2plb6_mcpy_init(&dma_setup_info);


    for (uint32_t addr = check_instruction_read_access_sram0_start_addr, sram0_offset = 0;
            addr < check_instruction_read_access_sram0_start_addr + 640;
            addr+=4, sram0_offset+=4) {

        if (addr < check_instruction_read_access_sram0_end_addr)
            iowrite32(ioread32(addr), SRAM0_BASE + sram0_offset);
        else
            iowrite32(0x24000000, SRAM0_BASE + sram0_offset);
    }

    rumboot_printf("Check SRAM0 instructions reading (CACHE OFF)\n");

    static const tlb_entry sram0_tlb_entry_cache_off = {TLB_ENTRY_CACHE_OFF};
    write_tlb_entries(&sram0_tlb_entry_cache_off, 1);
    msync();
    isync();

    rumboot_printf("Testing ... \n");
    dma2plb6_enable_channel(DCR_DMAPLB6_BASE, channel0);

    for (uint32_t i = 0; i < CALL_ITERATIONS_COUNT; ++i)
    {
        (void)((uint32_t (*)())(SRAM0_BASE))();
    }

    uint32_t test_result = ((uint32_t (*)())(SRAM0_BASE))();
    TEST_ASSERT(test_result == 0xDEDABABA, "Error occurred during SRAM0 function call");



    //cache on
    for (uint32_t addr = check_instruction_read_access_sram0_start_addr, sram0_offset = 0;
            addr < check_instruction_read_access_sram0_start_addr + 640;
            addr+=4, sram0_offset+=4) {

        if (addr < check_instruction_read_access_sram0_end_addr)
            iowrite32(ioread32(addr), SRAM0_BASE + sram0_offset);
        else
            iowrite32(0x24000000, SRAM0_BASE + sram0_offset);
    }

    rumboot_printf("Check SRAM0 instructions reading (CACHE ON)\n");

    static const tlb_entry sram0_tlb_entry_cache_on = {TLB_ENTRY_CACHE_ON};
    write_tlb_entries(&sram0_tlb_entry_cache_on, 1);
    msync();
    isync();

    rumboot_printf("Testing ... \n");
    dma2plb6_enable_channel(DCR_DMAPLB6_BASE, channel0);

    for (uint32_t i = 0; i < CALL_ITERATIONS_COUNT; ++i)
    {
        (void)((uint32_t (*)())(SRAM0_BASE))();
    }

    test_result = ((uint32_t (*)())(SRAM0_BASE))();
    TEST_ASSERT(test_result == 0xBABADEDA, "Error occurred during SRAM0 function call");

    return 0;
}
