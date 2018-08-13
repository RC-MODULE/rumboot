
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

#define input_data_transfer_em2_1   0xC0000001 //init see in .svh
#define input_data_transfer_em3_1   0xD0010002
#define input_data_transfer_em2_2   0xC0003008
#define input_data_transfer_em3_2   0xD0011000

#define output_data_transfer_em2_1  0xC0001003
#define output_data_transfer_em2_2  0xC0002006
#define output_data_transfer_em2_3  0xC0004008
#define output_data_transfer_em2_4  0xC0005000

uint32_t check_data ()
{
    rumboot_printf("Check data...\n");
    rumboot_printf("Count of data elements 0x%x \n", NUM_ELEM);
    register uint32_t i = 0;
    for (i = 0; i < NUM_ELEM; i++)
    {
        rumboot_printf("Num elem: "); //trace_hex(i);

        if (((uint32_t*) output_data_transfer_em2_1) [i] != ((uint32_t*) input_data_transfer_em2_1)[i])
        {
            rumboot_printf ("Error in output data 1, addr = %d\n",((uint32_t*) output_data_transfer_em2_1)[i]);

            //trace_hex (&(((uint32_t*)output_data_transfer_em2_1) [i]));
            rumboot_printf("Expected data = %d\n", ((uint32_t*)input_data_transfer_em2_1)[i]); //trace_hex(((uint32_t*)input_data_transfer_em2_1) [i]);
            rumboot_printf("Actual data = %d\n", ((uint32_t*)output_data_transfer_em2_1)[i]); //trace_hex(((uint32_t*)output_data_transfer_em2_1) [i]);
            return 0;
        };

        if (((uint32_t*) output_data_transfer_em2_2) [i] != ((uint32_t*) input_data_transfer_em3_1) [i])
        {
            rumboot_printf("Error in output data 2, addr = %d\n", ((uint32_t*) output_data_transfer_em2_2)[i]);
            //trace_hex (&(((uint32_t*)output_data_transfer_em2_2) [i]));
            rumboot_printf("Expected data = %d\n", ((uint32_t*)input_data_transfer_em3_1)[i]); //trace_hex(((uint32_t*) input_data_transfer_em3_1) [i]);
            rumboot_printf("Actual data = %d\n", ((uint32_t *)output_data_transfer_em2_2)[i]); //trace_hex(((uint32_t*)output_data_transfer_em2_2) [i]);
            return 0;
        };

        if (((uint32_t*) output_data_transfer_em2_3) [i] != ((uint32_t*) input_data_transfer_em2_2)[i])
        {
            rumboot_printf ("Error in output data 3, addr =  %d\n", ((uint32_t*)output_data_transfer_em2_3)[i]);
            //trace_hex (&(((uint32_t*)output_data_transfer_em2_3) [i]));
            rumboot_printf("Expected data = %d\n",((uint32_t*) input_data_transfer_em2_2)[i]); //trace_hex(((uint32_t*)input_data_transfer_em2_2) [i]);
            rumboot_printf("Actual data = %d\n", ((uint32_t*) output_data_transfer_em2_3)[i]); //trace_hex(((uint32_t*)output_data_transfer_em2_3) [i]);
            return 0;
        };

        if (((uint32_t*)output_data_transfer_em2_4) [i] != ((uint32_t*) input_data_transfer_em3_2)[i])
        {
            rumboot_printf ("Error in output data 4, addr = %d\n", ((uint32_t*)output_data_transfer_em2_4)[i]);
            //trace_hex (&(((uint32_t*)output_data_transfer_em2_4) [i]));
            rumboot_printf("Expected data = %d\n", ((uint32_t*)input_data_transfer_em3_2)[i]); //trace_hex(((uint32_t*) input_data_transfer_em3_2) [i]);
            rumboot_printf("Actual data = %d\n", ((uint32_t*)output_data_transfer_em2_4)[i]); //trace_hex(((uint32_t*)output_data_transfer_em2_4) [i]);
            return 0;
        };

    }
    rumboot_printf ("Check data successful\n");
    return 1;
}

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

void dma2plb6_memcpy(const uint32_t base_addr, const uint64_t source, const uint64_t dest, const DmaChannel channel, const transfer_width width, const rw_transfer_size size, const uint32_t count, uint8_t wait)
{
    dma2plb6_setup_info dma_info;
    channel_status status = {0};
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

    rumboot_printf("Call dma2plb6_memcpy\n");

    rumboot_printf("Source address:\n");
    rumboot_printf("hi= %d\n", (uint32_t)(source >> 32)); //trace_hex((uint32_t)(source >> 32));
    rumboot_printf("lo= %d\n", (uint32_t)(source)); //trace_hex((uint32_t)(source));

    rumboot_printf("Destination address\n");
    rumboot_printf("hi= %d\n", (uint32_t)(dest >> 32)); //trace_hex((uint32_t)(dest >> 32));
    rumboot_printf("lo= %d\n", (uint32_t)dest); //trace_hex((uint32_t)dest);


    if (wait == 0){ dma2plb6_mcpy(&dma_info);}
    else
    {
        if(dma_single_copy(&dma_info,&status) == false)
        {
            dma2plb6_trace_status(status);
        }
    }

}

uint64_t get_physical_addr_em2 (uint32_t ea)
{
    return ea + 0x400000000 - 0xc0000000;
}

uint64_t get_physical_addr_em3 (uint32_t ea)
{
    return ea + 0x600000000 - 0xd0000000;
}

int main (void)
{
    uint64_t src = 0, dst = 0;
    uint32_t size_for_dma = NUM_ELEM * sizeof(uint32_t);
    emi_init();

    //init_data see in .svh

    src = get_physical_addr_em2((uint32_t)input_data_transfer_em2_1);
    dst = get_physical_addr_em2((uint32_t)output_data_transfer_em2_1);
    dma2plb6_memcpy (EM2_BASE, src, dst, channel0, tr_width_byte, rw_tr_size_1q, size_for_dma, 0);

    src = get_physical_addr_em3((uint32_t)input_data_transfer_em3_1);
    dst = get_physical_addr_em2((uint32_t)output_data_transfer_em2_2);
    dma2plb6_memcpy (EM2_BASE, src, dst, channel1, tr_width_halfword, rw_tr_size_2q, size_for_dma/2, 0);

    src = get_physical_addr_em2((uint32_t)input_data_transfer_em2_2);
    dst = get_physical_addr_em2((uint32_t)output_data_transfer_em2_3);
    dma2plb6_memcpy (EM2_BASE, src, dst, channel2, tr_width_doubleword, rw_tr_size_4q, size_for_dma/8, 0);

    src = get_physical_addr_em3((uint32_t)input_data_transfer_em3_2);
    dst = get_physical_addr_em2((uint32_t)output_data_transfer_em2_4);
    dma2plb6_memcpy (EM2_BASE, src, dst, channel3, tr_width_quadword, rw_tr_size_8q, size_for_dma/16, 1);

    if (check_data())
        rumboot_putstring("TEST_OK\n");
        return 0;
    //else
    rumboot_putstring("TEST_ERROR\n");
    return 1;
}
