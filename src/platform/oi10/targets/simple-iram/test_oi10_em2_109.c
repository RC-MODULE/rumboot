
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

#define NUM_ELEM 512

///src
static uint32_t data_src_0 = (SRAM0_BASE);
static uint32_t data_src_1 = (NOR_BASE);
static uint32_t data_src_2 = (SDRAM_BASE);
static uint32_t data_src_3 = (NOR_BASE) + 4 * NUM_ELEM;

///dst
static uint32_t data_dst_0 = (SRAM0_BASE) + 4 * NUM_ELEM;
static uint32_t data_dst_1 = (SRAM0_BASE) + 8 * NUM_ELEM;
static uint32_t data_dst_2 = (SDRAM_BASE) + 4 * NUM_ELEM;
static uint32_t data_dst_3 = (SDRAM_BASE) + 8 * NUM_ELEM;

void init_data ()
{
    rumboot_printf ("Init data\n") ;
    uint32_t i = 0;
    for (i = 0; i < NUM_ELEM; i++)
    {
      iowrite32 (0xAAAAAAAA + i, data_src_0 + 4*i);
      msync();
      nor_write32(0xBBBBBBBB + 2*i, data_src_1 + 4*i);
      msync();
      iowrite32 (0xCCCCCCCC + 3*i, data_src_2 + 4*i);
      msync();
      nor_write32(0xDDDDDDDD + 4*i, data_src_3 + 4*i);
      msync();
    }
    rumboot_printf ("End init data\n");
}

void check_data ()
{
    rumboot_printf ("Check data\n") ;

    uint32_t i = 0;
    for (i = 0; i < NUM_ELEM; i++)
    {
        /*
      TEST_ASSERT(ioread32(data_dst_0 + 4*i) == 0xAAAAAAAA + i, "ASSERT: data_dst_0 is corrupted");
      TEST_ASSERT(ioread32(data_dst_1 + 4*i) == 0xBBBBBBBB + 2*i, "ASSERT: data_dst_1 is corrupted");
      TEST_ASSERT(ioread32(data_dst_2 + 4*i) == 0xCCCCCCCC + 3*i, "ASSERT: data_dst_2 is corrupted");
      TEST_ASSERT(ioread32(data_dst_3 + 4*i) == 0xDDDDDDDD + 4*i, "ASSERT: data_dst_3 is corrupted");
      */
        rumboot_printf("DST0(%x) = %x\n", i, ioread32(data_dst_0 + 4*i));
        rumboot_printf("DST1(%x) = %x\n", i, ioread32(data_dst_1 + 4*i));
        rumboot_printf("DST2(%x) = %x\n", i, ioread32(data_dst_2 + 4*i));
        rumboot_printf("DST3(%x) = %x\n", i, ioread32(data_dst_3 + 4*i));
    }
}

int main (void)
{
    emi_init(DCR_EM2_EMI_BASE);
    init_data ();

    uint32_t size_for_dma = NUM_ELEM * sizeof(uint32_t);

    dma2plb6_setup_info dma_info0;
    channel_status status0 = {};
    dma_info0.base_addr = DCR_DMAPLB6_BASE;
    dma_info0.source_adr = rumboot_virt_to_dma((uint32_t*)data_src_0);
    dma_info0.dest_adr = rumboot_virt_to_dma((uint32_t*)data_dst_0);
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
    dma_info1.source_adr = rumboot_virt_to_dma((uint32_t*)data_src_1);
    dma_info1.dest_adr = rumboot_virt_to_dma((uint32_t*)data_dst_1);
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
    dma_info2.source_adr = rumboot_virt_to_dma((uint32_t*)data_src_2);
    dma_info2.dest_adr = rumboot_virt_to_dma((uint32_t*)data_dst_2);
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
    dma_info3.source_adr = rumboot_virt_to_dma((uint32_t*)data_src_3);
    dma_info3.dest_adr = rumboot_virt_to_dma((uint32_t*)data_dst_3);
    dma_info3.priority = priority_medium_low;
    dma_info3.striding_info.striding = striding_none;
    dma_info3.tc_int_enable = false;
    dma_info3.err_int_enable = false;
    dma_info3.int_mode = int_mode_level_wait_clr;
    dma_info3.channel = channel3;
    dma_info3.transfer_width = tr_width_quadword;
    dma_info3.rw_transfer_size = rw_tr_size_8q;
    dma_info3.count = size_for_dma/16;

    rumboot_printf ("Start DMA\n");
    dma2plb6_mcpy(&dma_info0);
    dma2plb6_mcpy(&dma_info1);
    dma2plb6_mcpy(&dma_info2);
    dma2plb6_mcpy(&dma_info3);

    rumboot_printf ("Wait DMA...\n");
    if (wait_dma2plb6_mcpy(&dma_info0, &status0))
    {
        rumboot_printf ("DMA channel0 completed...\n");
        if (wait_dma2plb6_mcpy(&dma_info1, &status1))
        {
            rumboot_printf ("DMA channel1 completed...\n");
            if (wait_dma2plb6_mcpy(&dma_info2, &status2))
            {
              rumboot_printf ("DMA channel2 completed...\n");
              if (wait_dma2plb6_mcpy(&dma_info3, &status3))
              {
                  rumboot_printf ("DMA channel3 completed...\n");
                  msync();
                  check_data ();
              }
              else
              {
                  rumboot_printf ("TEST ERROR: DMA channel3 fail\n");
                  return 1;
              }
            }
            else
            {
                  rumboot_printf ("TEST ERROR: DMA channel2 fail\n");
                  return 1;
            }
        }
        else
        {
           rumboot_printf ("TEST ERROR: DMA channel1 fail\n");
           return 1;
        }
    }
    else
    {
      rumboot_printf ("TEST ERROR: DMA channel0 fail\n");
      return 1;
    }

    rumboot_printf ("TEST_OK\n");
    return 0;
}
