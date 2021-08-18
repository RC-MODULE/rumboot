#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <arch/mmu.h>
#include <arch/mem_window.h>
#include <arch/dma2plb6.h>



#define TLB_ENTRY_EM2_CACHE_WT   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_EM2_CACHE_WB   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

//#define EVENT_RESET_DATA        TEST_EVENT_CODE_MIN + 0

#define NUM_ELEM 64


//static uint32_t input_data_transfer_32[NUM_ELEM]  __attribute__((section(".EM2.data"))) __attribute__((aligned(1024))) = {0};
//static uint32_t output_data_transfer_32[NUM_ELEM] __attribute__((section(".EM2.data"))) __attribute__((aligned(1024))) = {0};

uint32_t* input_data_transfer_32_WT = (uint32_t*)(SRAM0_BASE + 0x100);
uint32_t* output_data_transfer_32_WT = (uint32_t*)(SRAM0_BASE + 0x100 + NUM_ELEM*sizeof(uint32_t));

uint32_t* input_data_transfer_32_WB = (uint32_t*)(SRAM0_BASE + 0x200);
uint32_t* output_data_transfer_32_WB = (uint32_t*)(SRAM0_BASE + 0x200 + NUM_ELEM*sizeof(uint32_t));


static uint32_t dma2plb6_get_bytesize(transfer_width transfer_width_code)
{
    switch(transfer_width_code){
    case tr_width_byte:
        return 1;
        break;
    case tr_width_halfword:
        return 2;
        break;
    case tr_width_word:
        return 4;
        break;
    case tr_width_doubleword:
        return 8;
        break;
    case tr_width_quadword:
        return 16;
        break;
    }
    return 0;
}

static void init_data(uint32_t* addr)
{
    uint32_t i = 0, reg = 0;
    for (i = 0; i < NUM_ELEM; i++)
    {
        //reg = input_data_transfer_32[i];
        reg = i + 0xABCD0000;
        //input_data_transfer_32[i] = reg;
        iowrite32(reg, (uint32_t)(addr + i));
    }

    i = 0;

    while (i < NUM_ELEM)
    {
        dcbi(addr + i);
        i += 32;
    };
}

static bool check_data(uint32_t* input_addr, uint32_t* output_addr)
{
    rumboot_putstring("Check data...\n");
    uint32_t i = 0;
    for (i = 0; i < NUM_ELEM; i++)
    {
        if ((output_addr[i] != input_addr[i]) || (output_addr[i] != i + 0xABCD0000))
        {
            rumboot_printf("Error in output data array, index = %d\n", i);
            rumboot_printf("Expected = 0x%x\n", i + 0xABCD0000);
            rumboot_printf("Actual (in) = 0x%x\n", input_addr[i]);
            rumboot_printf("Actual (out) = 0x%x\n", output_addr[i]);
            return false;
        };
    }
    rumboot_putstring("Check data successful\n");
    return true;
}

static bool dma2plb6_memcpy(const uint32_t base_addr, const uint64_t source, const uint64_t dest, const DmaChannel channel, const transfer_width width, const rw_transfer_size size, const uint32_t count)
{
    struct dma2plb6_setup_info dma_info;
    struct channel_status status = {};
    dma_info.base_addr = base_addr;
    dma_info.source_adr = source;
    dma_info.dest_adr = dest;
    dma_info.priority = priority_medium_low;
    dma_info.striding_info.striding = striding_none;
    dma_info.tc_int_enable = false;
    dma_info.err_int_enable = false;
    dma_info.int_mode = int_mode_level_wait_clr;
    dma_info.channel = channel;
    dma_info.transfer_width = width;
    dma_info.rw_transfer_size = size;
    dma_info.count = count;

    rumboot_putstring("Call dma2plb6_memcpy\n");

    rumboot_putstring("Source address:\n");
    rumboot_printf("hi = 0x%x\n", (uint32_t)(source >> 32));
    rumboot_printf("lo = 0x%x\n", (uint32_t)(source));

    rumboot_putstring("Destination address\n");
    rumboot_printf("hi = 0x%x\n", (uint32_t)(dest >> 32));
    rumboot_printf("lo = 0x%x\n", (uint32_t)dest);

    if(dma2plb6_single_copy(&dma_info,&status) == false)
    {
        dma2plb6_trace_error_status(&status);
        return false;
    }
    return true;
}


int main(void)
{
    emi_init(DCR_EM2_EMI_BASE);

    uint64_t src = 0, dst = 0;
    transfer_width dma_transfer_width_cur;
    rw_transfer_size dma_rw_transfer_size_cur;

    //cache WT
    rumboot_putstring("Set cache WT\n");
    static const tlb_entry em0_tlb_entry_wt = {TLB_ENTRY_EM2_CACHE_WT};
    write_tlb_entries(&em0_tlb_entry_wt,1);
    msync();
    isync();

    init_data(input_data_transfer_32_WT);

    src = get_physical_addr((uint32_t)input_data_transfer_32_WT, 0);
    dst = get_physical_addr((uint32_t)output_data_transfer_32_WT, 0);

    dma_transfer_width_cur = tr_width_word;
    dma_rw_transfer_size_cur = rw_tr_size_4q;

    if (dma2plb6_memcpy(DCR_DMAPLB6_BASE, src, dst, channel0, dma_transfer_width_cur, dma_rw_transfer_size_cur, (NUM_ELEM*sizeof(uint32_t))/dma2plb6_get_bytesize(dma_transfer_width_cur)))
    {
        if (!dma2plb6_memcpy(DCR_DMAPLB6_BASE, dst, src, channel0, dma_transfer_width_cur, dma_rw_transfer_size_cur, (NUM_ELEM*sizeof(uint32_t))/dma2plb6_get_bytesize(dma_transfer_width_cur)))
        {
            rumboot_putstring("DMA dst -> src ERROR\n");
            return 1;
        }
    }
    else
    {
        rumboot_putstring("DMA src -> dst ERROR\n");
        return 1;
    }

    if (!check_data(input_data_transfer_32_WT, output_data_transfer_32_WT))
    {
        rumboot_putstring("Data error\n");
        return 1;
    }

    dci(0);
    dci(2);



    //cache WB
    rumboot_putstring("Set cache WB\n");
    static const tlb_entry em0_tlb_entry_wb = {TLB_ENTRY_EM2_CACHE_WB};
    write_tlb_entries(&em0_tlb_entry_wb,1);
    msync();
    isync();

    init_data(input_data_transfer_32_WB);

    src = get_physical_addr((uint32_t)input_data_transfer_32_WB, 0);
    dst = get_physical_addr((uint32_t)output_data_transfer_32_WB, 0);

    dma_transfer_width_cur = tr_width_word;
    dma_rw_transfer_size_cur = rw_tr_size_4q;


    if (dma2plb6_memcpy(DCR_DMAPLB6_BASE, src, dst, channel0, dma_transfer_width_cur, dma_rw_transfer_size_cur, (NUM_ELEM*sizeof(uint32_t))/dma2plb6_get_bytesize(dma_transfer_width_cur)))
    {
        if (!dma2plb6_memcpy(DCR_DMAPLB6_BASE, dst, src, channel0, dma_transfer_width_cur, dma_rw_transfer_size_cur, (NUM_ELEM*sizeof(uint32_t))/dma2plb6_get_bytesize(dma_transfer_width_cur)))
        {
            rumboot_putstring("DMA dst -> src ERROR\n");
            return 1;
        }
    }
    else
    {
        rumboot_putstring("DMA src -> dst ERROR\n");
        return 1;
    }

    msync();

    if (!check_data(input_data_transfer_32_WB, output_data_transfer_32_WB))
    {
        rumboot_putstring("Data error\n");
        return 1;
    }

    return 0;
}
