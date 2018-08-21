
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
#include <platform/devices/nor_1636RR4.h>
#include <platform/devices.h>

#define NUM_ELEM 256
///src
static uint32_t em2_cs0_sram_out = (SRAM0_BASE);
static uint32_t em2_cs1_nor_out = (NOR_BASE);
static uint32_t em2_cs2_sdram_out = (SDRAM_BASE);
static uint32_t em2_cs1_nor_out_3000 = (NOR_BASE) + 4 * NUM_ELEM;
///dst
static uint32_t em2_cs0_sram_in = (SRAM0_BASE) + 4 * NUM_ELEM;
static uint32_t em2_cs0_sram_in_2000 = (SRAM0_BASE) + 2 * 4 * NUM_ELEM;
static uint32_t em2_cs2_sdram_in_4000 = (SDRAM_BASE) + 4 * NUM_ELEM;
static uint32_t em2_cs2_sdram_in_8000 = (SDRAM_BASE) + 2 * 4 * NUM_ELEM;

static void init_data(uint32_t addr, uint32_t value)
{
    uint32_t j = 0;
    for (j = 0; j < NUM_ELEM; j++)
    {
        iowrite32(value, (uint32_t)(addr + 4*j));
    }
}

static void init_nor_data(uint32_t addr, uint32_t value)
{
    uint32_t j = 0;
    for(j = 0; j < NUM_ELEM; j++ )
    {
        nor_write(value, (uint32_t)(addr + 4 * j));
    }
}

static bool check_data ()
{
    rumboot_printf("Check data...\n");
    rumboot_printf("Count of data elements: %d \n", NUM_ELEM);
    register uint32_t i = 0;
    for (i = 0; i < NUM_ELEM; i++)
    {
        ///em2.cs0_in->im2.cs0_out
        if ( ioread32(em2_cs0_sram_in + 4* i) != ioread32 (em2_cs0_sram_out + 4 * i))
        {
            rumboot_printf ("Error in output data 1, addr = %d\n", ioread32(em2_cs0_sram_out[i]));
            rumboot_printf("Expected data = %d\n", ioread32(em2_cs0_sram_out[i]));
            rumboot_printf("Actual data = %d\n", ioread32(em2_cs0_sram_in[i]));
            return false;
        };
        ///em2.cs1.nor_in->em2.cs0_sram_out
        if (ioread32(em2_cs0_sram_in_2000 + 4 * i) != ioread32(em2_cs1_nor_out + 4 * i))
        {
            rumboot_printf("Error in output data 2, addr = %d\n", ioread32(em2_cs1_nor_out[i]));
            rumboot_printf("Expected data = %d\n", ioread32(em2_cs1_nor_out[i]));
            rumboot_printf("Actual data = %d\n", ioread32(em2_cs0_sram_in_2000[i]));
            return false;
        };
        ///em2.cs2.sdram_in->em2.cs2.sdram_out
        if (ioread32(em2_cs2_sdram_in_4000 + 4*i) != ioread32(em2_cs2_sdram_out + 4*i))
        {
            rumboot_printf ("Error in output data 3, addr =  %d\n", ioread32(em2_cs2_sdram_out[i]));
            rumboot_printf("Expected data = %d\n",ioread32(em2_cs2_sdram_out[i]));
            rumboot_printf("Actual data = %d\n", ioread32(em2_cs2_sdram_in_4000[i]));
            return false;
        };
        ///em2.cs1.nor->em0.cs2.sdram
       if (ioread32(em2_cs2_sdram_in_8000 + 4*i) != ioread32(em2_cs1_nor_out_3000 + 4 *i))
        {
            rumboot_printf ("Error in output data 4, addr = %d\n", ioread(em2_cs1_nor_out_3000[i]));
            rumboot_printf("Expected data = %d\n", ioread32(em2_cs1_nor_out_3000[i]));
            rumboot_printf("Actual data = %d\n", ioread32(em2_cs2_sdram_in_8000[i]));
            return false;
        };
    }
    rumboot_printf ("Check data successful\n");
    return true;
}

int main (void)
{
    emi_init(DCR_EM2_EMI_BASE);
    uint32_t j = 0;

    rumboot_printf ("Init data0\n") ;
    init_data(em2_cs0_sram_out, 0xaaaaaaaa);
    rumboot_printf ("Init data1\n") ;

    /*
    {
        for (j = 0; j < NUM_ELEM; j++)
        {
            nor_write32(0xbbbbbbbb, (em2_cs1_nor_out + 4*j));
        }
    }
    */

    rumboot_printf("Init data1\n");
    init_nor_data(em2_cs1_nor_out, 0xbbbbbbbb);
    rumboot_printf ("Init data2\n") ;
    init_data(em2_cs2_sdram_out, 0xcccccccc);
    rumboot_printf("Init data3 \n");
    init_nor_data(em2_cs1_nor_out, 0xdddddddd);

    uint32_t size_for_dma = NUM_ELEM * sizeof(uint32_t);

    dma2plb6_setup_info dma_info0;
    channel_status status0 = {};
    dma_info0.base_addr = DCR_DMAPLB6_BASE;
    dma_info0.source_adr = rumboot_virt_to_dma((uint32_t*)em2_cs0_sram_out);
    dma_info0.dest_adr = rumboot_virt_to_dma((uint32_t*)em2_cs0_sram_in);
    dma_info0.priority = priority_medium_low;
    dma_info0.striding_info.striding = striding_none;
    dma_info0.tc_int_enable = false;
    dma_info0.err_int_enable = false;
    dma_info0.int_mode = int_mode_level_wait_clr;
    dma_info0.channel = channel0;
    dma_info0.transfer_width = tr_width_byte;
    dma_info0.rw_transfer_size = rw_tr_size_1q;
    dma_info0.count = size_for_dma;

    dma2plb6_setup_info dma_info1;
    channel_status status1 = {};
    dma_info1.base_addr = DCR_DMAPLB6_BASE;
    dma_info1.source_adr = rumboot_virt_to_dma((uint32_t*)em2_cs1_nor_out);
    dma_info1.dest_adr = rumboot_virt_to_dma((uint32_t*)em2_cs0_sram_in_2000);
    dma_info1.priority = priority_medium_low;
    dma_info1.striding_info.striding = striding_none;
    dma_info1.tc_int_enable = false;
    dma_info1.err_int_enable = false;
    dma_info1.int_mode = int_mode_level_wait_clr;
    dma_info1.channel = channel1;
    dma_info1.transfer_width = tr_width_halfword;
    dma_info1.rw_transfer_size = rw_tr_size_2q;
    dma_info1.count = size_for_dma/2;

    dma2plb6_setup_info dma_info2;
    channel_status status2 = {};
    dma_info2.base_addr = DCR_DMAPLB6_BASE;
    dma_info2.source_adr = rumboot_virt_to_dma((uint32_t*)em2_cs2_sdram_out);
    dma_info2.dest_adr = rumboot_virt_to_dma((uint32_t*)em2_cs2_sdram_in_4000);
    dma_info2.priority = priority_medium_low;
    dma_info2.striding_info.striding = striding_none;
    dma_info2.tc_int_enable = false;
    dma_info2.err_int_enable = false;
    dma_info2.int_mode = int_mode_level_wait_clr;
    dma_info2.channel = channel2;
    dma_info2.transfer_width = tr_width_doubleword;
    dma_info2.rw_transfer_size = rw_tr_size_4q;
    dma_info2.count = size_for_dma/8;

    dma2plb6_setup_info dma_info3;
    channel_status status3 = {};
    dma_info3.base_addr = DCR_DMAPLB6_BASE;
    dma_info3.source_adr = rumboot_virt_to_dma((uint32_t*)em2_cs2_sdram_out);
    dma_info3.dest_adr = rumboot_virt_to_dma((uint32_t*)em2_cs2_sdram_in_8000);
    dma_info3.priority = priority_medium_low;
    dma_info3.striding_info.striding = striding_none;
    dma_info3.tc_int_enable = false;
    dma_info3.err_int_enable = false;
    dma_info3.int_mode = int_mode_level_wait_clr;
    dma_info3.channel = channel3;
    dma_info3.transfer_width = tr_width_quadword;
    dma_info3.rw_transfer_size = rw_tr_size_8q;
    dma_info3.count = size_for_dma/16;

    rumboot_printf ("Start dma0\n");
    dma2plb6_mcpy(&dma_info0);
    rumboot_printf ("Start dma1\n");
    dma2plb6_mcpy(&dma_info1);
    rumboot_printf ("Start dma2\n");
    dma2plb6_mcpy(&dma_info2);
    rumboot_printf ("Start dma3\n");
    dma2plb6_mcpy(&dma_info3);

    rumboot_printf ("Start wait...\n");
    if (wait_dma2plb6_mcpy(&dma_info0, &status0) && wait_dma2plb6_mcpy(&dma_info1, &status1) &&
            wait_dma2plb6_mcpy(&dma_info2, &status2) && wait_dma2plb6_mcpy(&dma_info3, &status3) == false) return 1; // == TEST_EROR

    if (check_data())
    {
       rumboot_printf ("TEST_OK\n") ;
       return 0;
    }

    rumboot_printf ("TEST_ERROR\n") ;
    return 1;
}
