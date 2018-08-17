
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
#include <platform/devices/dma2plb6.h>

#include <platform/devices.h>

#define NUM_ELEM    256
///src
static uint32_t* em2_cs0_sram_out = (uint32_t*)(SRAM0_BASE + 0x100); /// 0 CHANNEL
static uint32_t* em2_cs1_nor_out = (uint32_t*)(NOR_BASE + 0x200); /// 1 CHANNEL
//static uint32_t* em0_cs2_sdram_out = (uint32_t*)(SDRAM_BASE + 0x300); /// 2 CHANNEL
//static uint32_t* em2_cs1_nor_out_3000 = (uint32_t*)(NOR_BASE + 0x3000);// + 0x3000); /// 3 CHANNEL
///dst
static uint32_t* em2_cs0_sram_in = (uint32_t*)(SRAM0_BASE + 0x100 + NUM_ELEM*sizeof(uint32_t)); // + 0x1000); ///0 CHANNEL
static uint32_t* em2_cs0_sram_in_2000 = (uint32_t*)(SRAM0_BASE + 0x200 + NUM_ELEM*sizeof(uint32_t)); // + 0x2000); /// 1 CHANNEL
//static uint32_t* em2_cs0_sdram_in_4000 = (uint32_t*)(SDRAM_BASE + 0x300 + NUM_ELEM*sizeof(uint32_t)); // + 0x4000); /// 2 CHANNEL
//static uint32_t* em2_cs2_sdram_in_8000 = (uint32_t*)(SDRAM_BASE + 0x400 + NUM_ELEM*sizeof(uint32_t)); /// 3 CHANNEL

uint32_t dma_single_copy(dma2plb6_setup_info * setup_info, channel_status * status)
{
    dma2plb6_mcpy(setup_info);
    do{
        *status = dma2plb6_ch_get_status(setup_info->base_addr,setup_info->channel);
        if(status->error_detected)
            return false;
    }
    while(status->busy | !status->terminalcnt_reached);
    return true;
}

uint32_t check_data ()
{
    rumboot_printf("Check data...\n");
    //rumboot_printf("Count of data elements 0x%x \n", NUM_ELEM);
    rumboot_printf("Count of data elements: %d \n", NUM_ELEM);
    register uint32_t i = 0;
    for (i = 0; i < NUM_ELEM; ++i)
    {
        //rumboot_printf("Num elem = %d\n: ", i); //trace_hex(i);

        if (((uint32_t*)em2_cs0_sram_in[i] != (uint32_t*)em2_cs0_sram_out[i]) &&
                ((uint32_t*)em2_cs0_sram_in_2000[i] != (uint32_t*) em2_cs1_nor_out[i]))// &&
                //((uint32_t*)em2_cs0_sdram_in_4000[i] != (uint32_t*)em0_cs2_sdram_out[i]))
        {
            ///em2.cs0_in->im2.cs0_out
            rumboot_printf ("Error in output data 1, addr = %d\n",((uint32_t*)em2_cs0_sram_out)[i]);
            rumboot_printf("Expected data = %d\n", ((uint32_t*)em2_cs0_sram_out)[i]);
            rumboot_printf("Actual data = %d\n", ((uint32_t*)em2_cs0_sram_in)[i]);
            ///em2.cs1.nor_in->em2.cs0_sram_out
            rumboot_printf("Error in output data 2, addr = %d\n", ((uint32_t*) em2_cs1_nor_out)[i]);
            rumboot_printf("Expected data = %d\n", ((uint32_t*)em2_cs1_nor_out)[i]);
            rumboot_printf("Actual data = %d\n", ((uint32_t *)em2_cs0_sram_in_2000)[i]);
            ///em2.cs2.sdram_in->em2.cs2.sdram_out
     //       rumboot_printf ("Error in output data 3, addr =  %d\n", ((uint32_t*)em0_cs2_sdram_out)[i]);
     //       rumboot_printf("Expected data = %d\n",((uint32_t*) em0_cs2_sdram_out)[i]);
     //       rumboot_printf("Actual data = %d\n", ((uint32_t*) em2_cs0_sdram_in_4000)[i]);

            return 0;
        };

        ///em2.cs1.nor->em0.cs2.sdram
//        if ((uint32_t*)em2_cs2_sdram_in_8000[i] != (uint32_t*) em2_cs1_nor_out_3000[i])
//        {
//            rumboot_printf ("Error in output data 4, addr = %d\n", ((uint32_t*)em2_cs1_nor_out_3000)[i]);
//            rumboot_printf("Expected data = %d\n", ((uint32_t*)em2_cs1_nor_out_3000)[i]);
//            rumboot_printf("Actual data = %d\n", ((uint32_t*) em2_cs2_sdram_in_8000)[i]);
//            return 0;
//        };
    }

    rumboot_printf ("Check data successful\n");
    return true;
}



/*
uint32_t check_data ()
{
    rumboot_printf("Check data...\n");
    //rumboot_printf("Count of data elements 0x%x \n", NUM_ELEM);
    rumboot_printf("Count of data elements: %d \n", NUM_ELEM);
    register uint32_t i = 0;
    for (i = 0; i < NUM_ELEM; ++i)
    {
        //rumboot_printf("Num elem = %d\n: ", i); //trace_hex(i);
        ///em2.cs0_in->im2.cs0_out
        if ((uint32_t*)em2_cs0_sram_in[i] != (uint32_t*)em2_cs0_sram_out[i])
        {
            rumboot_printf ("Error in output data 1, addr = %d\n",((uint32_t*)em2_cs0_sram_out)[i]);
            rumboot_printf("Expected data = %d\n", ((uint32_t*)em2_cs0_sram_out)[i]);
            rumboot_printf("Actual data = %d\n", ((uint32_t*)em2_cs0_sram_in)[i]);
            return 0;
        };

        ///em2.cs1.nor_in->em2.cs0_sram_out
        if ((uint32_t*)em2_cs0_sram_in_2000[i] != (uint32_t*) em2_cs1_nor_out[i])
        {
            rumboot_printf("Error in output data 2, addr = %d\n", ((uint32_t*) em2_cs1_nor_out)[i]);
            rumboot_printf("Expected data = %d\n", ((uint32_t*)em2_cs1_nor_out)[i]);
            rumboot_printf("Actual data = %d\n", ((uint32_t *)em2_cs0_sram_in_2000)[i]);
            return 0;
        };

        ///em2.cs2.sdram_in->em2.cs2.sdram_out
        if ((uint32_t*)em2_cs0_sdram_in_4000[i] != (uint32_t*)em0_cs2_sdram_out[i])
        {
            rumboot_printf ("Error in output data 3, addr =  %d\n", ((uint32_t*)em0_cs2_sdram_out)[i]);
            //trace_hex (&(((uint32_t*)output_data_transfer_em2_3) [i]));
            rumboot_printf("Expected data = %d\n",((uint32_t*) em0_cs2_sdram_out)[i]); //trace_hex(((uint32_t*)input_data_transfer_em2_2) [i]);
            rumboot_printf("Actual data = %d\n", ((uint32_t*) em2_cs0_sdram_in_4000)[i]); ;
            return 0;
        };

        ///em2.cs1.nor->em0.cs2.sdram
//        if ((uint32_t*)em2_cs2_sdram_in_8000[i] != (uint32_t*) em2_cs1_nor_out_3000[i])
//        {
//            rumboot_printf ("Error in output data 4, addr = %d\n", ((uint32_t*)em2_cs1_nor_out_3000)[i]);
//            rumboot_printf("Expected data = %d\n", ((uint32_t*)em2_cs1_nor_out_3000)[i]);
//            rumboot_printf("Actual data = %d\n", ((uint32_t*) em2_cs2_sdram_in_8000)[i]);
//            return 0;
//        };
    }

    rumboot_printf ("Check data successful\n");
    return true;
}
*/

void dma2plb6_trace_status(channel_status status)
{
    switch(status.spec_error_status)
    {
        case error_alignnment:
        case error_scatter_alignment:
            rumboot_printf ("EM2_BASE: Error alignment\n");
            break;
        case error_read_data_regular:
        case error_read_data_scatter_or_resume:
            rumboot_printf ("EM2_BASE: Error read data\n");
            break;
        case error_read_request_regular:
        case error_read_request_scatter_or_resume:
            rumboot_printf ("EM2_BASE: Error read request\n");
            break;
        case error_write_request:
            rumboot_printf ("EM2_BASE: Error write request\n");
            break;
        default:
            rumboot_printf ("EM2_BASE: Unexpected status\n");
    }
}

static bool dma2plb6_memcpy(const uint32_t base_addr, const uint64_t source, const uint64_t dest,
        const DmaChannel channel, const transfer_width width,
        const rw_transfer_size size,
        const uint32_t count)
{
    dma2plb6_setup_info dma_info;
    channel_status status = {};
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

    if(dma2plb6_single_copy(&dma_info, &status) == false)
    {
        dma2plb6_trace_status(status);
        return false;
    }
    return true;
}

int main (void)
{
    uint64_t src = 0, dst = 0;
    uint32_t size_for_dma = NUM_ELEM * sizeof(uint32_t);

    emi_init(DCR_EM2_EMI_BASE);

    src = rumboot_virt_to_dma((uint32_t*)em2_cs0_sram_out);
    dst = rumboot_virt_to_dma((uint32_t*)em2_cs0_sram_in);
    dma2plb6_memcpy (DCR_DMAPLB6_BASE, src, dst, channel0, tr_width_byte, rw_tr_size_1q, size_for_dma);

    src = rumboot_virt_to_dma((uint32_t*)em2_cs1_nor_out); //source from
    dst = rumboot_virt_to_dma((uint32_t*)em2_cs0_sram_in_2000);
    dma2plb6_memcpy (DCR_DMAPLB6_BASE, src, dst, channel1, tr_width_halfword, rw_tr_size_2q, size_for_dma/2);
/*
    src = rumboot_virt_to_dma((uint32_t*)em0_cs2_sdram_out);
    dst = rumboot_virt_to_dma((uint32_t*)em2_cs0_sdram_in_4000);
    dma2plb6_memcpy (DCR_DMAPLB6_BASE, src, dst, channel2, tr_width_doubleword, rw_tr_size_4q, size_for_dma/8);
*/
/*
    src = rumboot_virt_to_dma((uint32_t*)em2_cs1_nor_out_3000);
    dst = rumboot_virt_to_dma((uint32_t*)em2_cs2_sdram_in_8000);
    dma2plb6_memcpy (DCR_DMAPLB6_BASE, src, dst, channel3, tr_width_quadword, rw_tr_size_8q, size_for_dma/16);
*/
    if (check_data())
        rumboot_putstring("TEST_OK\n");
        return 0;
    //else
    rumboot_putstring("TEST_ERROR\n");
    return 1;
}
