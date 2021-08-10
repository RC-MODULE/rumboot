
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/mem_window.h>
#include <devices/ugly/emi.h>
#include <arch/dma2plb6.h>
#include <platform/devices/nor_1636RR4.h>
#include <platform/devices.h>

#define NUM_ELEM 32

///src
static uint32_t data_src_0 = SRAM0_BASE;
static uint32_t data_src_1 = NOR_BASE;
static uint32_t data_src_2 = SDRAM_BASE;
static uint32_t data_src_3 = NOR_BASE + 4*NUM_ELEM;

static uint32_t data_src_00 = SRAM0_BASE; //==data_src_0; //SRAM0
static uint32_t data_src_01 = SDRAM_BASE; //==data_src_2; //SDRAM
static uint32_t data_src_02 = SSRAM_BASE;
static uint32_t data_src_03 = PIPELINED_BASE;

static uint32_t data_src_04 = SRAM1_BASE;
static uint32_t data_src_05 = NOR_BASE;   //==data_src_1 //NOR
///dst
static uint32_t data_dst_0 = SRAM0_BASE + 4*NUM_ELEM;
static uint32_t data_dst_1 = SRAM0_BASE + 8*NUM_ELEM;
static uint32_t data_dst_2 = SDRAM_BASE + 0x4000;
static uint32_t data_dst_3 = SDRAM_BASE + 0x8000;

static uint32_t data_dst_00 = SDRAM_BASE + 0xC000;
static uint32_t data_dst_01 = SSRAM_BASE + 0x1000;
static uint32_t data_dst_02 = PIPELINED_BASE + 0x1000;
static uint32_t data_dst_03 = SRAM1_BASE + 0x1000;

static uint32_t data_dst_04 = SRAM0_BASE + 0x1000;
static uint32_t data_dst_05 = SRAM0_BASE + 0x2000;

void init_data ()
{
    rumboot_printf ("Init data\n") ;
    uint32_t i = 0;
    for (i = 0; i < NUM_ELEM; i++)
    {
      iowrite32  (0xAAAAAAAA + i,   data_src_0 + 4*i);
      nor_write32(0xBBBBBBBB + 2*i, data_src_1 + 4*i);
      iowrite32  (0xCCCCCCCC + 3*i, data_src_2 + 4*i);
      nor_write32(0xDDDDDDDD + 4*i, data_src_3 + 4*i);

      iowrite32  (0x55555555 + 5*i, data_src_02 + 4*i);
      iowrite32  (0x66666666 + 6*i, data_src_03 + 4*i);
      iowrite32  (0x77777777 + 7*i, data_src_04 + 4*i);
    }
    msync();
    rumboot_printf ("End init data\n");
}

void check_data (uint8_t part)
{
    rumboot_printf ("Check data (part = %x)\n", part) ;
    uint32_t i = 0;

    for (i = 0; i < NUM_ELEM; i++)
    {
      if (part == 0)
      {
          TEST_ASSERT(ioread32(data_dst_0 + 4*i) == ioread32(data_src_0 + 4*i), "ASSERT: data_dst_0 is corrupted");
          TEST_ASSERT(ioread32(data_dst_1 + 4*i) == ioread32(data_src_1 + 4*i), "ASSERT: data_dst_1 is corrupted");
          TEST_ASSERT(ioread32(data_dst_2 + 4*i) == ioread32(data_src_2 + 4*i), "ASSERT: data_dst_2 is corrupted");
          TEST_ASSERT(ioread32(data_dst_3 + 4*i) == ioread32(data_src_3 + 4*i), "ASSERT: data_dst_3 is corrupted");
      }
      if (part == 1)
      {
          TEST_ASSERT(ioread32(data_dst_00 + 4*i) == ioread32(data_src_00 + 4*i), "ASSERT: data_dst_00 is corrupted");
          TEST_ASSERT(ioread32(data_dst_01 + 4*i) == ioread32(data_src_01 + 4*i), "ASSERT: data_dst_01 is corrupted");
          TEST_ASSERT(ioread32(data_dst_02 + 4*i) == ioread32(data_src_02 + 4*i), "ASSERT: data_dst_02 is corrupted");
          TEST_ASSERT(ioread32(data_dst_03 + 4*i) == ioread32(data_src_03 + 4*i), "ASSERT: data_dst_03 is corrupted");
      }
      if (part == 2)
      {
          TEST_ASSERT(ioread32(data_dst_04 + 4*i) == ioread32(data_src_04 + 4*i), "ASSERT: data_dst_04 is corrupted");
          TEST_ASSERT(ioread32(data_dst_05 + 4*i) == ioread32(data_src_05 + 4*i), "ASSERT: data_dst_05 is corrupted");
      }
    }
    rumboot_printf ("Check data complete\n") ;

}

void set_dma_cfg (struct dma2plb6_setup_info * dma_info, uint32_t base_addr, DmaChannel channel, transfer_width tr_width, rw_transfer_size rw_tr_size, uint64_t src, uint64_t dst, uint32_t count)
{
    dma_info->base_addr = base_addr;
    dma_info->source_adr = src;
    dma_info->dest_adr = dst;
    dma_info->priority = priority_medium_low;
    dma_info->striding_info.striding = striding_none;
    dma_info->tc_int_enable = false;
    dma_info->err_int_enable = false;
    dma_info->int_mode = int_mode_level_wait_clr;
    dma_info->channel = channel;
    dma_info->transfer_width = tr_width;
    dma_info->rw_transfer_size = rw_tr_size;
    dma_info->snp_mode =snp_mode_off;
    dma_info->count = count;
    msync();
}

int main (void)
{
    emi_init(DCR_EM2_EMI_BASE);
    init_data ();

    uint32_t size_for_dma = NUM_ELEM * 4;

    rumboot_printf ("DMA prepare...\n");
    struct dma2plb6_setup_info dma_info0;
    struct channel_status status0 = {};
    set_dma_cfg (&dma_info0, DCR_DMAPLB6_BASE, channel0, tr_width_byte, rw_tr_size_1q, rumboot_virt_to_phys((void*)data_src_0), rumboot_virt_to_phys((void*)data_dst_0), size_for_dma);

    struct dma2plb6_setup_info dma_info1;
    struct channel_status status1 = {};
    set_dma_cfg (&dma_info1, DCR_DMAPLB6_BASE, channel1, tr_width_halfword, rw_tr_size_2q, rumboot_virt_to_phys((void*)data_src_1), rumboot_virt_to_phys((void*)data_dst_1), size_for_dma/2);

    struct dma2plb6_setup_info dma_info2;
    struct channel_status status2 = {};
    set_dma_cfg (&dma_info2, DCR_DMAPLB6_BASE, channel2, tr_width_doubleword, rw_tr_size_4q, rumboot_virt_to_phys((void*)data_src_2), rumboot_virt_to_phys((void*)data_dst_2), size_for_dma/8);

    struct dma2plb6_setup_info dma_info3;
    struct channel_status status3 = {};
    set_dma_cfg (&dma_info3, DCR_DMAPLB6_BASE, channel3, tr_width_quadword, rw_tr_size_8q, rumboot_virt_to_phys((void*)data_src_3), rumboot_virt_to_phys((void*)data_dst_3), size_for_dma/16);
    rumboot_printf ("DMA prepared: ch0 (sram0->sram0), ch1 (nor->sram0), ch2 (sdram->sdram), ch3 (nor-> sdram)\n");

    rumboot_printf ("Start DMA\n");
    dma2plb6_mcpy_init(&dma_info0);
    dma2plb6_mcpy_init(&dma_info1);
    dma2plb6_mcpy_init(&dma_info2);
    dma2plb6_mcpy_init(&dma_info3);
    msync();
    dma2plb6_enable_all_channels (DCR_DMAPLB6_BASE);

    rumboot_printf ("Wait DMA...\n");
    if (!wait_dma2plb6_mcpy(&dma_info0, &status0))
    {
        rumboot_printf ("DMA transfer error\n");
        return 1;
    }

    if (!wait_dma2plb6_mcpy(&dma_info1, &status1))
    {
        rumboot_printf ("DMA transfer error\n");
        return 1;
    }
    if (!wait_dma2plb6_mcpy(&dma_info2, &status2))
    {
        rumboot_printf ("DMA transfer error\n");
        return 1;
    }

    if (!wait_dma2plb6_mcpy(&dma_info3, &status3))
    {
        rumboot_printf ("DMA transfer error\n");
        return 1;
    }
    dma2plb6_disable_all_channel (DCR_DMAPLB6_BASE);
    dma2plb6_clear_status_reg(DCR_DMAPLB6_BASE);
    rumboot_printf ("DMA transfer complete\n");
    check_data(0);

    rumboot_printf ("------------------------------------------------------------------\n");

    rumboot_printf ("DMA prepare...\n");
    set_dma_cfg (&dma_info0, DCR_DMAPLB6_BASE, channel0, tr_width_quadword, rw_tr_size_4q, rumboot_virt_to_phys((void*)data_src_00), rumboot_virt_to_phys((void*)data_dst_00), size_for_dma/16);
    set_dma_cfg (&dma_info1, DCR_DMAPLB6_BASE, channel1, tr_width_quadword, rw_tr_size_4q, rumboot_virt_to_phys((void*)data_src_01), rumboot_virt_to_phys((void*)data_dst_01), size_for_dma/16);
    set_dma_cfg (&dma_info2, DCR_DMAPLB6_BASE, channel2, tr_width_quadword, rw_tr_size_4q, rumboot_virt_to_phys((void*)data_src_02), rumboot_virt_to_phys((void*)data_dst_02), size_for_dma/16);
    set_dma_cfg (&dma_info3, DCR_DMAPLB6_BASE, channel3, tr_width_quadword, rw_tr_size_4q, rumboot_virt_to_phys((void*)data_src_03), rumboot_virt_to_phys((void*)data_dst_03), size_for_dma/16);
    rumboot_printf ("DMA prepared: ch0 (sram0->sdram), ch1 (sdram->ssram), ch2 (ssram->pipelined), ch3 (pipelined-> sram1)\n");

    rumboot_printf ("Start DMA\n");
    dma2plb6_mcpy_init(&dma_info0);
    dma2plb6_mcpy_init(&dma_info1);
    dma2plb6_mcpy_init(&dma_info2);
    dma2plb6_mcpy_init(&dma_info3);
    msync();
    dma2plb6_enable_all_channels (DCR_DMAPLB6_BASE);

    rumboot_printf ("Wait DMA...\n");
    if (!wait_dma2plb6_mcpy(&dma_info0, &status0))
    {
        rumboot_printf ("DMA transfer error\n");
        return 1;
    }

    if (!wait_dma2plb6_mcpy(&dma_info1, &status1))
    {
        rumboot_printf ("DMA transfer error\n");
        return 1;
    }
    if (!wait_dma2plb6_mcpy(&dma_info2, &status2))
    {
        rumboot_printf ("DMA transfer error\n");
        return 1;
    }

    if (!wait_dma2plb6_mcpy(&dma_info3, &status3))
    {
        rumboot_printf ("DMA transfer error\n");
        return 1;
    }
    dma2plb6_disable_all_channel (DCR_DMAPLB6_BASE);
    dma2plb6_clear_status_reg(DCR_DMAPLB6_BASE);
    rumboot_printf ("DMA transfer complete\n");
    check_data(1);

    rumboot_printf ("------------------------------------------------------------------\n");

    rumboot_printf ("DMA prepare...\n");
    set_dma_cfg (&dma_info0, DCR_DMAPLB6_BASE, channel0, tr_width_quadword, rw_tr_size_4q, rumboot_virt_to_phys((void*)data_src_04), rumboot_virt_to_phys((void*)data_dst_04), size_for_dma/16);
    set_dma_cfg (&dma_info1, DCR_DMAPLB6_BASE, channel1, tr_width_quadword, rw_tr_size_4q, rumboot_virt_to_phys((void*)data_src_05), rumboot_virt_to_phys((void*)data_dst_05), size_for_dma/16);
    rumboot_printf ("DMA prepared: ch0 (sram1->sram0), ch1 (nor->sram0)\n");

    rumboot_printf ("Start DMA\n");
    dma2plb6_mcpy_init(&dma_info0);
    dma2plb6_mcpy_init(&dma_info1);
    msync();
    dma2plb6_enable_channel(DCR_DMAPLB6_BASE, channel0);
    dma2plb6_enable_channel(DCR_DMAPLB6_BASE, channel1);

    rumboot_printf ("Wait DMA...\n");
    if (!wait_dma2plb6_mcpy(&dma_info0, &status0))
    {
        rumboot_printf ("DMA transfer error\n");
        return 1;
    }

    if (!wait_dma2plb6_mcpy(&dma_info1, &status1))
    {
        rumboot_printf ("DMA transfer error\n");
        return 1;
    }
    dma2plb6_disable_all_channel (DCR_DMAPLB6_BASE);
    dma2plb6_clear_status_reg(DCR_DMAPLB6_BASE);
    rumboot_printf ("DMA transfer complete\n");
    check_data(2);

    rumboot_printf ("TEST_OK\n");
    return 0;
}
