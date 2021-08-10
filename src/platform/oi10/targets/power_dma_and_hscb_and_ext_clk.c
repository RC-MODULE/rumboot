// -----------------------------------------------------------------------------
// -------------------------------- Definitions --------------------------------
// -----------------------------------------------------------------------------
#define TEST_ITERATION_COUNT 4

#define HSCB_POWER
#define DMA_POWER
// -----------------------------------------------------------------------------
// -------------------------------- CPU includes --------------------------------
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>

#include <devices/gpio_pl061.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
//#include <platform/regs/fields/mpic128.h>
#include <arch/regs/fields/dma2plb6.h>
#include <arch/regs/dma2plb6.h>
#include <platform/devices/hscb.h>
#include <arch/dma2plb6.h>
#include <devices/ugly/emi.h>
#include <platform/test_assert.h>
#include <devices/ugly/greth.h>
#include <rumboot/printf.h>
#include <rumboot/macros.h>
#include <arch/ppc_476fp_mmu.h>
#include <unistd.h>
#include <rumboot/memtest.h>
#include <platform/devices/l2c.h>

// -----------------------------------------------------------------------------
// -------------------------------- Interfaces --------------------------------
// -----------------------------------------------------------------------------

//#define PERIPH_TEST_DATA_LEN 4096
//#define PERIPH_TEST_DATA_LEN 2048
//#define PERIPH_TEST_DATA_LEN 32768
//#define PERIPH_TEST_DATA_LEN 16384
//#define PERIPH_TEST_DATA_LEN 8192
#define PERIPH_TEST_DATA_LEN 8192
#define PERIPH_TEST_DATA_LEN_BYTES      (PERIPH_TEST_DATA_LEN * sizeof(uint32_t))

#define PERIPH_TEST_PATTERN_0    0x55555555
#define PERIPH_TEST_PATTERN_1    0xAAAAAAAA


#define HSCB_TEST_DATA_LEN_BYTES        (PERIPH_TEST_DATA_LEN_BYTES)
#define DMA_TEST_DATA_LEN_BYTES         (PERIPH_TEST_DATA_LEN_BYTES)

//#define BASE_DESCR_COUNT                0x256
#define BASE_DESCR_COUNT                0x64
#define DMA2PLB6_DESCR_COUNT            (BASE_DESCR_COUNT)
#define TEST_POWER_HSCB_DESCR_TABLE_LEN (BASE_DESCR_COUNT * 8)

#define IM1_SIZE        0x64000

static uint32_t* test_hscb_im1_data_src;

// -----------------------------------------------------------------------------
// -------------------------------- Interfaces Definitions --------------------------------
// -----------------------------------------------------------------------------
static volatile bool hscb_handler = true;
static volatile bool dma2plb6_handler = true;

// ================ HSCB ===============   
struct test_pattern_t
{
    uint32_t word0[4];
    uint32_t word1[4];
};



#define SOURCE_DATA_ARR_SIZE    (PERIPH_TEST_DATA_LEN_BYTES/sizeof(struct test_pattern_t))
static struct test_pattern_t src_data_im0[SOURCE_DATA_ARR_SIZE] = {[0 ... SOURCE_DATA_ARR_SIZE-1] = {
                                                                                                    .word0 = {PERIPH_TEST_PATTERN_0, PERIPH_TEST_PATTERN_1, PERIPH_TEST_PATTERN_0, PERIPH_TEST_PATTERN_1},
                                                                                                    .word1 = {PERIPH_TEST_PATTERN_0, PERIPH_TEST_PATTERN_1, PERIPH_TEST_PATTERN_0, PERIPH_TEST_PATTERN_1}
                                                                                                 }
                                                                              };

static hscb_instance_t  hscb_cfg[4];

#ifdef HSCB_POWER
static uint32_t* test_hscb0_data_im1_src;
static uint32_t* test_hscb0_data_im1_dst;

static uint32_t* test_hscb1_data_im1_src;
static uint32_t* test_hscb1_data_im1_dst;

static uint32_t* test_hscb2_data_im1_src;
static uint32_t* test_hscb2_data_im1_dst;

static uint32_t* test_hscb3_data_im1_src;
static uint32_t* test_hscb3_data_im1_dst;

static uint32_t* HSCB0_TX_DESCR_ADDR;
static uint32_t* HSCB0_RX_DESCR_ADDR;
static uint32_t* HSCB1_TX_DESCR_ADDR;
static uint32_t* HSCB1_RX_DESCR_ADDR;

static uint32_t* HSCB2_TX_DESCR_ADDR;
static uint32_t* HSCB2_RX_DESCR_ADDR;
static uint32_t* HSCB3_TX_DESCR_ADDR;
static uint32_t* HSCB3_RX_DESCR_ADDR;

static uint32_t hscb_wr_desc;
static uint32_t hscb_rd_desc;

static uint32_t wdma_settings_reg;
static uint32_t rdma_settings_reg;

void configure_hscb(hscb_instance_t* hscb_inst);
void run_hscb_transfers_via_loopback(hscb_instance_t* hscb_inst);
#endif

// ================ DMA ===============   
#ifdef DMA_POWER
static uint8_t* test_dma2plb6_0_data_src;
static uint8_t* test_dma2plb6_0_data_dst;
static size_t test_dma2plb6_0_data_size = DMA_TEST_DATA_LEN_BYTES/4;

struct dma2plb6_scatter_gather_descriptor
{
    uint32_t channel_control_word;
    uint32_t LK : 1;
    uint32_t count_and_control : 31;
    uint32_t data_stride;
    uint32_t reserved;
    uint32_t source_address_high;
    uint32_t source_address_low;
    uint32_t destination_address_high;
    uint32_t destination_address_low;
    uint32_t link_next_SG_descr_addr_high;
    uint32_t link_next_SG_descr_addr_low;
    uint64_t reserved_alignment;
};

static struct dma2plb6_scatter_gather_descriptor* dma2plb6_chnl0_sg_descr;


static DmaChannel dma_channel0 = channel0;

static struct dma2plb6_setup_info dma_info_0;

static struct dma2plb6_setup_info dma_info_default = {
    .base_addr = DCR_DMAPLB6_BASE,
    .source_adr =0x00,
    .dest_adr = 0x00,
    .priority = priority_medium_low,
    .striding_info.striding = striding_none,
    .tc_int_enable = false,
    .err_int_enable = false,
    .int_mode = int_mode_level_wait_clr,
    .channel = channel0,
 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//    .rw_transfer_size = rw_tr_size_1q,
    .rw_transfer_size = rw_tr_size_8q,
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    .transfer_width = tr_width_byte,
    .count = 0x00,
    .snp_mode = snp_mode_off
};
static void handler_dma2plb6( int irq, void *arg );
#endif


// -----------------------------------------------------------------------------
// -------------------------------- Functions --------------------------------
// -----------------------------------------------------------------------------

// ================ HSBC ===============   
#ifdef HSCB_POWER
static void handler_hscb( int irq, void *arg )
{
//    rumboot_printf( "\nInterrupt!\n");

    hscb_instance_t* hscb_inst = (hscb_instance_t* ) arg;
    // Read and clear WDMA status
    hscb_get_wdma_status(hscb_inst->src_hscb_base_addr);
    
    // Rewrite HSCB addresses
    iowrite32(hscb_inst->real_rdma_addr, hscb_inst->src_hscb_base_addr + HSCB_RDMA_DESC_ADDR);
    iowrite32(hscb_inst->real_wdma_addr, hscb_inst->src_hscb_base_addr + HSCB_WDMA_DESC_ADDR);
    uint32_t rd_addr = hscb_inst->real_rdma_addr;
    uint32_t wr_addr = hscb_inst->real_wdma_addr;

//    rumboot_printf("rd_addr = %x\n", rd_addr);
//    rumboot_printf("wr_addr = %x\n", wr_addr);
    // Update descriptors
    int i;
    for (i=0; i<BASE_DESCR_COUNT; i++) {
        iowrite32(hscb_rd_desc, rd_addr);
        rd_addr +=8;
        iowrite32(hscb_wr_desc, wr_addr);
        wr_addr +=8;
    }
    
    // Start HSCB
    iowrite32(wdma_settings_reg, hscb_inst->src_hscb_base_addr + HSCB_WDMA_SETTINGS);
    iowrite32(rdma_settings_reg, hscb_inst->src_hscb_base_addr + HSCB_RDMA_SETTINGS);
//    rumboot_printf("\nEnd of handler!\n");
    hscb_handler = true;
}

void init_hscb_cfg(hscb_instance_t* hscb_inst)
{
    (hscb_inst + 0)->src_hscb_base_addr   = HSCB0_BASE;
    (hscb_inst + 0)->dst_hscb_base_addr   = HSCB1_BASE;
    (hscb_inst + 0)->src_addr             = test_hscb0_data_im1_src;
    (hscb_inst + 0)->dst_addr             = test_hscb0_data_im1_dst;
    (hscb_inst + 0)->src_size             = HSCB_TEST_DATA_LEN_BYTES;
    (hscb_inst + 0)->dst_size             = HSCB_TEST_DATA_LEN_BYTES;
    (hscb_inst + 0)->tx_descr_addr        = (uint32_t)HSCB0_TX_DESCR_ADDR;
    (hscb_inst + 0)->rx_descr_addr        = (uint32_t)HSCB0_RX_DESCR_ADDR;
    (hscb_inst + 0)->real_rdma_addr       = rumboot_virt_to_dma(HSCB0_TX_DESCR_ADDR);
    (hscb_inst + 0)->real_wdma_addr       = rumboot_virt_to_dma(HSCB0_RX_DESCR_ADDR);

    (hscb_inst + 1)->src_hscb_base_addr   = HSCB1_BASE;
    (hscb_inst + 1)->dst_hscb_base_addr   = HSCB0_BASE;
    (hscb_inst + 1)->src_addr             = test_hscb1_data_im1_src;
    (hscb_inst + 1)->dst_addr             = test_hscb1_data_im1_dst;
    (hscb_inst + 1)->src_size             = HSCB_TEST_DATA_LEN_BYTES;
    (hscb_inst + 1)->dst_size             = HSCB_TEST_DATA_LEN_BYTES;
    (hscb_inst + 1)->tx_descr_addr        = (uint32_t)HSCB1_TX_DESCR_ADDR;
    (hscb_inst + 1)->rx_descr_addr        = (uint32_t)HSCB1_RX_DESCR_ADDR;
    (hscb_inst + 1)->real_rdma_addr       = rumboot_virt_to_dma(HSCB1_TX_DESCR_ADDR);
    (hscb_inst + 1)->real_wdma_addr       = rumboot_virt_to_dma(HSCB1_RX_DESCR_ADDR);

    (hscb_inst + 2)->src_hscb_base_addr   = HSCB2_BASE;
    (hscb_inst + 2)->dst_hscb_base_addr   = HSCB3_BASE;
    (hscb_inst + 2)->src_addr             = test_hscb2_data_im1_src;
    (hscb_inst + 2)->dst_addr             = test_hscb2_data_im1_dst;
    (hscb_inst + 2)->src_size             = HSCB_TEST_DATA_LEN_BYTES;
    (hscb_inst + 2)->dst_size             = HSCB_TEST_DATA_LEN_BYTES;
    (hscb_inst + 2)->tx_descr_addr        = (uint32_t)HSCB2_TX_DESCR_ADDR;
    (hscb_inst + 2)->rx_descr_addr        = (uint32_t)HSCB2_RX_DESCR_ADDR;
    (hscb_inst + 2)->real_rdma_addr       = rumboot_virt_to_dma(HSCB2_TX_DESCR_ADDR);
    (hscb_inst + 2)->real_wdma_addr       = rumboot_virt_to_dma(HSCB2_RX_DESCR_ADDR);

    (hscb_inst + 3)->src_hscb_base_addr   = HSCB3_BASE;
    (hscb_inst + 3)->dst_hscb_base_addr   = HSCB2_BASE;
    (hscb_inst + 3)->src_addr             = test_hscb3_data_im1_src;
    (hscb_inst + 3)->dst_addr             = test_hscb3_data_im1_dst;
    (hscb_inst + 3)->src_size             = HSCB_TEST_DATA_LEN_BYTES;
    (hscb_inst + 3)->dst_size             = HSCB_TEST_DATA_LEN_BYTES;
    (hscb_inst + 3)->tx_descr_addr        = (uint32_t)HSCB3_TX_DESCR_ADDR;
    (hscb_inst + 3)->rx_descr_addr        = (uint32_t)HSCB3_RX_DESCR_ADDR;
    (hscb_inst + 3)->real_rdma_addr       = rumboot_virt_to_dma(HSCB3_TX_DESCR_ADDR);
    (hscb_inst + 3)->real_wdma_addr       = rumboot_virt_to_dma(HSCB3_RX_DESCR_ADDR);
}

void configure_hscb(hscb_instance_t* hscb_inst)
{
    int i;
//    rumboot_printf("\n configure_hscb start\n");
    for (i=0; i<4; i++)
    {
        hscb_sw_rst((hscb_inst+i)->src_hscb_base_addr);
        hscb_adma_sw_rst((hscb_inst+i)->src_hscb_base_addr);
    }


    //config
    for (i=0; i<4; i++)
        hscb_config_for_receive_and_transmit_same_multiple(hscb_inst + i, TEST_POWER_HSCB_DESCR_TABLE_LEN / HSCB_DESCR_SIZE);

//    //enable
//    for (i=0; i<4; i++)
//        hscb_enable((hscb_inst+i)->src_hscb_base_addr);
//
    iowrite32(0x21000021,0xC030000c);
    iowrite32(0x21000021,0xC030100c);
    iowrite32(0x21000021,0xC030200c);
    iowrite32(0x21000021,0xC030300c);  
    
    //wait status
    for (i=0; i<4; i++)
        TEST_ASSERT(hscb_wait_status((hscb_inst+i)->src_hscb_base_addr, HSCB_STATUS_ACTIVE_LINK_mask), "Failed to waiting ACTIVE_LINK on HSCB");
//    rumboot_printf("\n configure_hscb end\n");
}

void run_hscb_transfers_via_loopback(hscb_instance_t* hscb_inst)
{
    int i;

    hscb_rwdma_settings_t rdma_settings;
    hscb_rwdma_settings_t wdma_settings;
    
//    rumboot_printf("\n run_hscb_transfers_via_loopback start\n");

    for (i = 0; i < 4; ++i)
//    for (i = 0; i < 2; ++i)
    {

        hscb_get_wdma_settings((hscb_inst+i)->src_hscb_base_addr, &wdma_settings);
        wdma_settings.rw_bad_desc = true;
        //wdma_settings.en_rwdma = true;
        wdma_settings.en_rwdma_desc_tbl = true;
        wdma_settings.rwdma_long_len = true;
        hscb_set_wdma_settings((hscb_inst+i)->src_hscb_base_addr, &wdma_settings);

        hscb_get_rdma_settings((hscb_inst+i)->src_hscb_base_addr, &rdma_settings);
        //rdma_settings.en_rwdma = true;
        rdma_settings.en_rwdma_desc_tbl = true;
        rdma_settings.rwdma_long_len = true;
        hscb_set_rdma_settings((hscb_inst+i)->src_hscb_base_addr, &rdma_settings);
    }
    hscb_wr_desc = ioread32(ioread32(hscb_inst->src_hscb_base_addr + HSCB_WDMA_SYS_ADDR));
    hscb_rd_desc = ioread32(ioread32(hscb_inst->src_hscb_base_addr + HSCB_RDMA_SYS_ADDR));
    //hscb_wr_desc = ioread32(ioread32(hscb_inst->src_hscb_base_addr + HSCB_WDMA_SYS_ADDR));
    //hscb_rd_desc = ioread32(ioread32(hscb_inst->src_hscb_base_addr + HSCB_RDMA_SYS_ADDR));
    
//    rumboot_printf("hscb_wr_desc = %x\n", hscb_wr_desc);
//    rumboot_printf("hscb_rd_desc = %x\n", hscb_rd_desc);

    wdma_settings_reg = ioread32((hscb_inst+0)->src_hscb_base_addr + HSCB_WDMA_SETTINGS) | (0x1 << HSCB_XDMA_SETTINGS_EN_XDMA_i );
    rdma_settings_reg = ioread32((hscb_inst+0)->src_hscb_base_addr + HSCB_RDMA_SETTINGS) | (0x1 << HSCB_XDMA_SETTINGS_EN_XDMA_i );

//    rumboot_printf("wdma_settings_reg = %x\n", wdma_settings_reg);
    
    iowrite32(ioread32((hscb_inst+0)->src_hscb_base_addr + HSCB_RDMA_SETTINGS) | (0x1 << HSCB_XDMA_SETTINGS_EN_XDMA_i ), (hscb_inst+0)->src_hscb_base_addr + HSCB_RDMA_SETTINGS);
    iowrite32(ioread32((hscb_inst+0)->src_hscb_base_addr + HSCB_WDMA_SETTINGS) | (0x1 << HSCB_XDMA_SETTINGS_EN_XDMA_i ), (hscb_inst+0)->src_hscb_base_addr + HSCB_WDMA_SETTINGS);

    iowrite32(ioread32((hscb_inst+1)->src_hscb_base_addr + HSCB_RDMA_SETTINGS) | (0x1 << HSCB_XDMA_SETTINGS_EN_XDMA_i ), (hscb_inst+1)->src_hscb_base_addr + HSCB_RDMA_SETTINGS);
    iowrite32(ioread32((hscb_inst+1)->src_hscb_base_addr + HSCB_WDMA_SETTINGS) | (0x1 << HSCB_XDMA_SETTINGS_EN_XDMA_i ), (hscb_inst+1)->src_hscb_base_addr + HSCB_WDMA_SETTINGS);

    iowrite32(ioread32((hscb_inst+2)->src_hscb_base_addr + HSCB_RDMA_SETTINGS) | (0x1 << HSCB_XDMA_SETTINGS_EN_XDMA_i ), (hscb_inst+2)->src_hscb_base_addr + HSCB_RDMA_SETTINGS);
    iowrite32(ioread32((hscb_inst+2)->src_hscb_base_addr + HSCB_WDMA_SETTINGS) | (0x1 << HSCB_XDMA_SETTINGS_EN_XDMA_i ), (hscb_inst+2)->src_hscb_base_addr + HSCB_WDMA_SETTINGS);

    iowrite32(ioread32((hscb_inst+3)->src_hscb_base_addr + HSCB_RDMA_SETTINGS) | (0x1 << HSCB_XDMA_SETTINGS_EN_XDMA_i ), (hscb_inst+3)->src_hscb_base_addr + HSCB_RDMA_SETTINGS);
    iowrite32(ioread32((hscb_inst+3)->src_hscb_base_addr + HSCB_WDMA_SETTINGS) | (0x1 << HSCB_XDMA_SETTINGS_EN_XDMA_i ), (hscb_inst+3)->src_hscb_base_addr + HSCB_WDMA_SETTINGS);

}
#endif

// ================ DMA ===============   
#ifdef DMA_POWER
static void handler_dma2plb6( int irq, void *arg )
{
//    channel_status dma_status;
    DmaChannel dma_channel = *(DmaChannel*)arg;
    dma2plb6_clear_interrupt(DCR_DMAPLB6_BASE, dma_channel);

    //clearing SG status bit
    dcr_write(get_addr(dma_channel, PLB6_DMA_SR, DCR_DMAPLB6_BASE), (1 << IBM_BIT_INDEX(32,SG)));

//    rumboot_printf("DMA2PLB6: PLB6_DMA_SR after clear = 0x%x\n", dcr_read(get_addr(dma_channel, PLB6_DMA_SR, DCR_DMAPLB6_BASE)));
    dcr_read(get_addr(dma_channel, PLB6_DMA_SR, DCR_DMAPLB6_BASE));

    dma2plb6_handler = true;
}

uint32_t dma2plb6_get_bytesize(transfer_width transfer_width_code)
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

struct dma2plb6_setup_info configure_dma2plb6_channel(DmaChannel channel, void* addr_src, void* addr_dst, size_t length)
{
    struct dma2plb6_setup_info dma_info = dma_info_default;
    dma_info.source_adr = rumboot_virt_to_phys(addr_src);
    dma_info.dest_adr = rumboot_virt_to_phys(addr_dst);
    dma_info.channel = channel;
    dma_info.count = length/dma2plb6_get_bytesize(dma_info.transfer_width);
    return dma_info;
}

static void configure_dma2plb6_descriptors(DmaChannel channel, uint8_t* data_src, uint8_t* data_dst, size_t length, struct dma2plb6_scatter_gather_descriptor* dma2plb6_descriptors, uint32_t descriptors_count)
{
    uint32_t i = 0;

    struct dma2plb6_setup_info dma_info = dma_info_default;

    uint32_t const control_word = //int mode, int enable
                                    (dma_info.int_mode << IBM_BIT_INDEX(32,CIM))
                                  | (dma_info.tc_int_enable << IBM_BIT_INDEX(32,TCIE))
                                  | (dma_info.err_int_enable << IBM_BIT_INDEX(32,EIE))
                                    //transfer options
                                  | (dma_info.transfer_width << IBM_BIT_INDEX(32,PW))
                                  | (dma_info.striding_info.striding << IBM_BIT_INDEX(32,DSD))   //setup DSD and DSS
                                  | (1 << IBM_BIT_INDEX(32,TCE))                                  //Enable terminal count
                                  | (dma_info.priority << IBM_BIT_INDEX(32,CP))
                                  | (dma_info.rw_transfer_size << IBM_BIT_INDEX(32,PF))
                                  | (dma_info.snp_mode << IBM_BIT_INDEX(32,DSC))
                                  | (0x1 << IBM_BIT_INDEX(32, CE));

    while (--descriptors_count)
    {
        dma2plb6_descriptors[i].LK = 0x1;
        dma2plb6_descriptors[i].channel_control_word = control_word;
        dma2plb6_descriptors[i].count_and_control = length;
        dma2plb6_descriptors[i].data_stride = 0x0;
        dma2plb6_descriptors[i].source_address_high = (rumboot_virt_to_phys(data_src)) >> 32;
        dma2plb6_descriptors[i].source_address_low = (rumboot_virt_to_phys(data_src) & 0xFFFFFFFF);
        dma2plb6_descriptors[i].destination_address_high = (rumboot_virt_to_phys(data_dst)) >> 32;
        dma2plb6_descriptors[i].destination_address_low = (rumboot_virt_to_phys(data_dst) & 0xFFFFFFFF);
        dma2plb6_descriptors[i].link_next_SG_descr_addr_high = (rumboot_virt_to_phys(&dma2plb6_descriptors[i + 1])) >> 32;
        dma2plb6_descriptors[i].link_next_SG_descr_addr_low = (rumboot_virt_to_phys(&dma2plb6_descriptors[i + 1])) & 0xFFFFFFFF;
        i++;
    }

    dma2plb6_descriptors[i].LK = 0x1;
    dma2plb6_descriptors[i].channel_control_word = control_word;
    dma2plb6_descriptors[i].count_and_control = length;
    dma2plb6_descriptors[i].data_stride = 0x0;
    dma2plb6_descriptors[i].source_address_high = (rumboot_virt_to_phys(data_src)) >> 32;
    dma2plb6_descriptors[i].source_address_low = (rumboot_virt_to_phys(data_src) & 0xFFFFFFFF);
    dma2plb6_descriptors[i].destination_address_high = (rumboot_virt_to_phys(data_dst)) >> 32;
    dma2plb6_descriptors[i].destination_address_low = (rumboot_virt_to_phys(data_dst) & 0xFFFFFFFF);
    dma2plb6_descriptors[i].link_next_SG_descr_addr_high = (rumboot_virt_to_phys(&dma2plb6_descriptors[0])) >> 32;
    dma2plb6_descriptors[i].link_next_SG_descr_addr_low = (rumboot_virt_to_phys(&dma2plb6_descriptors[0])) & 0xFFFFFFFF;
}

static void configure_dma2plb6_scatter_gather()
{
    configure_dma2plb6_descriptors(channel0, test_dma2plb6_0_data_src, test_dma2plb6_0_data_dst, test_dma2plb6_0_data_size, dma2plb6_chnl0_sg_descr, DMA2PLB6_DESCR_COUNT);
}


void configure_dma()
{
    dma_info_0 = configure_dma2plb6_channel(channel0, test_dma2plb6_0_data_src, test_dma2plb6_0_data_dst, test_dma2plb6_0_data_size);
    dma2plb6_mcpy_init(&dma_info_0);
}

static void clear_dma2plb6_status()
{
    //clearing SR register
    dcr_write(DCR_DMAPLB6_BASE + PLB6_DMA_SR, (1 << IBM_BIT_INDEX(32,(RI0 * (0+1)))) |
                                              ((1 << IBM_BIT_INDEX(32,CS)) << IBM_BIT_INDEX(4, 0)));
}

static void run_dma2plb6_scatter_gather_transfer()
{
    clear_dma2plb6_status();

    dcr_write(get_addr(0, PLB6_DMA_SGH, DCR_DMAPLB6_BASE), rumboot_virt_to_phys(dma2plb6_chnl0_sg_descr) >> 32);
    dcr_write(get_addr(0, PLB6_DMA_SGL, DCR_DMAPLB6_BASE), rumboot_virt_to_phys(dma2plb6_chnl0_sg_descr) & 0xFFFFFFFF);
    //Setting SSG bit
    dcr_write(get_addr(0, PLB6_DMA_SGC, DCR_DMAPLB6_BASE), (1 << IBM_BIT_INDEX(32,0)));
}
#endif

// ================ Common ===============   
struct rumboot_irq_entry * create_irq_handlers(hscb_instance_t* hscb_inst)
{
    rumboot_printf( "Create irq handlers\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
#ifdef HSCB_POWER
    init_hscb_cfg(hscb_inst);
    rumboot_irq_set_handler( tbl, SW0_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[0] );
    rumboot_irq_set_handler( tbl, SW1_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[1] );
    rumboot_irq_set_handler( tbl, SW2_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[2] );
    rumboot_irq_set_handler( tbl, SW3_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[3] );
#endif
#ifdef DMA_POWER
    rumboot_irq_set_handler( tbl, DMA2PLB6_DMA_IRQ_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_dma2plb6, &dma_channel0 );
#endif

    rumboot_irq_table_activate( tbl );
    
#ifdef HSCB_POWER
    rumboot_irq_enable( SW0_AXI_INT );
    rumboot_irq_enable( SW1_AXI_INT );
    rumboot_irq_enable( SW2_AXI_INT );
    rumboot_irq_enable( SW3_AXI_INT );
#endif
#ifdef DMA_POWER
    rumboot_irq_enable( DMA2PLB6_DMA_IRQ_0 );
#endif
    rumboot_irq_sei();

    return tbl;
}

void delete_irq_handlers(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}


void prepare_test_data(uint32_t src_id)
{
    emi_init(DCR_EM2_EMI_BASE);

    test_hscb_im1_data_src = (uint32_t*)rumboot_malloc_from_heap_aligned(src_id, HSCB_TEST_DATA_LEN_BYTES, sizeof(uint64_t));


#ifdef HSCB_POWER
    test_hscb0_data_im1_src = test_hscb_im1_data_src;
//    test_hscb0_data_im1_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(2, HSCB_TEST_DATA_LEN_BYTES, sizeof(uint32_t));
    test_hscb0_data_im1_dst = (uint32_t*)(EM2_BASE + 0xfff000);
    
    test_hscb1_data_im1_src = test_hscb_im1_data_src;
//    test_hscb1_data_im1_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(2, HSCB_TEST_DATA_LEN_BYTES, sizeof(uint32_t));
    test_hscb1_data_im1_dst = (uint32_t*)(EM2_BASE + 0xfff000);

    test_hscb2_data_im1_src = test_hscb_im1_data_src;
//    test_hscb2_data_im1_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(2, HSCB_TEST_DATA_LEN_BYTES, sizeof(uint32_t));
    test_hscb2_data_im1_dst = (uint32_t*)(EM2_BASE + 0xfff000);

    test_hscb3_data_im1_src = test_hscb_im1_data_src;
//    test_hscb3_data_im1_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(2, HSCB_TEST_DATA_LEN_BYTES, sizeof(uint32_t));
    test_hscb3_data_im1_dst = (uint32_t*)(EM2_BASE + 0xfff000);
#endif
#ifdef DMA_POWER
    test_dma2plb6_0_data_src = (uint8_t*)rumboot_malloc_from_heap_aligned(0, DMA_TEST_DATA_LEN_BYTES, 64);
    rumboot_printf("test_dma2plb6_0_data_src = 0x%X \n",test_dma2plb6_0_data_src);    
    
    memcpy((uint8_t*)test_dma2plb6_0_data_src, src_data_im0, sizeof(src_data_im0));
    
    test_dma2plb6_0_data_dst = (uint8_t*)EM2_BASE;
#endif






#ifdef HSCB_POWER
    HSCB0_TX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, TEST_POWER_HSCB_DESCR_TABLE_LEN, sizeof(uint64_t));
    rumboot_memfill8_modelling((void*)HSCB0_TX_DESCR_ADDR, TEST_POWER_HSCB_DESCR_TABLE_LEN, 0x00, 0x00);

    HSCB0_RX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, TEST_POWER_HSCB_DESCR_TABLE_LEN, sizeof(uint64_t));
    rumboot_memfill8_modelling((void*)HSCB0_RX_DESCR_ADDR, TEST_POWER_HSCB_DESCR_TABLE_LEN, 0x00, 0x00);

    HSCB1_TX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, TEST_POWER_HSCB_DESCR_TABLE_LEN, sizeof(uint64_t));
    rumboot_memfill8_modelling((void*)HSCB1_TX_DESCR_ADDR, TEST_POWER_HSCB_DESCR_TABLE_LEN, 0x00, 0x00);

    HSCB1_RX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, TEST_POWER_HSCB_DESCR_TABLE_LEN, sizeof(uint64_t));
    rumboot_memfill8_modelling((void*)HSCB1_RX_DESCR_ADDR, TEST_POWER_HSCB_DESCR_TABLE_LEN, 0x00, 0x00);


    HSCB2_TX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, TEST_POWER_HSCB_DESCR_TABLE_LEN, sizeof(uint64_t));
    rumboot_memfill8_modelling((void*)HSCB2_TX_DESCR_ADDR, TEST_POWER_HSCB_DESCR_TABLE_LEN, 0x00, 0x00);

    HSCB2_RX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, TEST_POWER_HSCB_DESCR_TABLE_LEN, sizeof(uint64_t));
    rumboot_memfill8_modelling((void*)HSCB2_RX_DESCR_ADDR, TEST_POWER_HSCB_DESCR_TABLE_LEN, 0x00, 0x00);

    HSCB3_TX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, TEST_POWER_HSCB_DESCR_TABLE_LEN, sizeof(uint64_t));
    rumboot_memfill8_modelling((void*)HSCB3_TX_DESCR_ADDR, TEST_POWER_HSCB_DESCR_TABLE_LEN, 0x00, 0x00);

    HSCB3_RX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, TEST_POWER_HSCB_DESCR_TABLE_LEN, sizeof(uint64_t));
    rumboot_memfill8_modelling((void*)HSCB3_RX_DESCR_ADDR, TEST_POWER_HSCB_DESCR_TABLE_LEN, 0x00, 0x00);

    memcpy((uint8_t*)test_hscb_im1_data_src, src_data_im0, sizeof(src_data_im0));
#endif
#ifdef DMA_POWER
    dma2plb6_chnl0_sg_descr = (struct dma2plb6_scatter_gather_descriptor *)rumboot_malloc_from_heap_aligned(0, DMA2PLB6_DESCR_COUNT * sizeof(struct dma2plb6_scatter_gather_descriptor), 2 * sizeof(uint64_t));

    rumboot_memfill8_modelling((void*)dma2plb6_chnl0_sg_descr, DMA2PLB6_DESCR_COUNT * sizeof(struct dma2plb6_scatter_gather_descriptor), 0x00, 0x00);
#endif

}

// -----------------------------------------------------------------------------
// -------------------------------- main --------------------------------
// -----------------------------------------------------------------------------

int main(void)
{
    struct rumboot_irq_entry *tbl;

    rumboot_printf("\n\n\nStart power_dma_and_hscb_and_ext_clk\n");
    
    rumboot_printf( "Init PLL \n");    
    dcr_write(0x8008003c,0x1ACCE551);
    rumboot_printf( "CKDIVMODE3(CLK_EXT) -> 1\n");
    dcr_write(0x80080130,0x1);
    rumboot_printf( "EN CKDIVMODE\n");
    dcr_write(0x80080060,0x1);
    
    
    prepare_test_data(2);   // 2 - IM2

//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x0 ,dcr_read(DCR_EM2_EMI_BASE + 0x0 ));
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x8 ,dcr_read(DCR_EM2_EMI_BASE + 0x8 ));
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x10,dcr_read(DCR_EM2_EMI_BASE + 0x10));
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x18,dcr_read(DCR_EM2_EMI_BASE + 0x18));
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x20,dcr_read(DCR_EM2_EMI_BASE + 0x20));
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x28,dcr_read(DCR_EM2_EMI_BASE + 0x28));
    
    dcr_write(DCR_EM2_EMI_BASE + 0x8 , 0x0);
    dcr_write(DCR_EM2_EMI_BASE + 0x10, 0x0);
    dcr_write(DCR_EM2_EMI_BASE + 0x18, 0x0);
    dcr_write(DCR_EM2_EMI_BASE + 0x20, 0x0);
    dcr_write(DCR_EM2_EMI_BASE + 0x28, 0x0);
    
    dcr_read(DCR_EM2_EMI_BASE + 0x0 );
    dcr_read(DCR_EM2_EMI_BASE + 0x8 );
    dcr_read(DCR_EM2_EMI_BASE + 0x10);
    dcr_read(DCR_EM2_EMI_BASE + 0x18);
    dcr_read(DCR_EM2_EMI_BASE + 0x20);
    dcr_read(DCR_EM2_EMI_BASE + 0x28);
    
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x0 ,dcr_read(DCR_EM2_EMI_BASE + 0x0 ));
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x8 ,dcr_read(DCR_EM2_EMI_BASE + 0x8 ));
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x10,dcr_read(DCR_EM2_EMI_BASE + 0x10));
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x18,dcr_read(DCR_EM2_EMI_BASE + 0x18));
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x20,dcr_read(DCR_EM2_EMI_BASE + 0x20));
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x28,dcr_read(DCR_EM2_EMI_BASE + 0x28));
    
    tbl = create_irq_handlers(hscb_cfg);

#ifdef HSCB_POWER
    configure_hscb(hscb_cfg);
#endif 

#ifdef DMA_POWER   
    configure_dma2plb6_scatter_gather();
#endif

    rumboot_printf("\nRunning tests ...\n");

#ifdef DMA_POWER    
    dma2plb6_handler = false;
    run_dma2plb6_scatter_gather_transfer();
#endif
#ifdef HSCB_POWER    
    hscb_handler = false;
    run_hscb_transfers_via_loopback(hscb_cfg);
#endif


    while (1)
    { 
        if (dma2plb6_handler) 
        {    
            dma2plb6_handler = false;
            run_dma2plb6_scatter_gather_transfer();
        }
        if (hscb_handler)
            hscb_handler = false;
    }

    rumboot_printf("\nEnd of test\n");
    delete_irq_handlers(tbl);

    return 0;
}
