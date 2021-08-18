// -----------------------------------------------------------------------------
// -------------------------------- Definitions --------------------------------
// -----------------------------------------------------------------------------
#define TEST_ITERATION_COUNT 4

#define HSCB_POWER
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
#define PERIPH_TEST_DATA_LEN 8192
//#define PERIPH_TEST_DATA_LEN 64
#define PERIPH_TEST_DATA_LEN_BYTES      (PERIPH_TEST_DATA_LEN * sizeof(uint32_t))

#define PERIPH_TEST_PATTERN_0    0x55555555
#define PERIPH_TEST_PATTERN_1    0xAAAAAAAA

#define HSCB_TEST_DATA_LEN_BYTES        (PERIPH_TEST_DATA_LEN_BYTES)

//-#define BASE_DESCR_COUNT                0x256
//?#define BASE_DESCR_COUNT                0x64
#define BASE_DESCR_COUNT                0x16

#define TEST_POWER_HSCB_DESCR_TABLE_LEN (BASE_DESCR_COUNT * 8)

#define IM1_SIZE        0x64000
// Always use!!!
static uint32_t* test_hscb_im1_data_src;

// -----------------------------------------------------------------------------
// -------------------------------- Interfaces Definitions --------------------------------
// -----------------------------------------------------------------------------
static volatile bool hscb_handler = true;



// ================ HSCB ===============   
struct greth_test_pattern_t
{
    uint32_t word0[4];
    uint32_t word1[4];
};



#define SOURCE_DATA_ARR_SIZE    (PERIPH_TEST_DATA_LEN_BYTES/sizeof(struct greth_test_pattern_t))
static struct greth_test_pattern_t greth_src_data_im0[SOURCE_DATA_ARR_SIZE] = {[0 ... SOURCE_DATA_ARR_SIZE-1] = {
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
void run_hscb_transfers_vial_loopback(hscb_instance_t* hscb_inst);
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

void run_hscb_transfers_vial_loopback(hscb_instance_t* hscb_inst)
{
    int i;

    hscb_rwdma_settings_t rdma_settings;
    hscb_rwdma_settings_t wdma_settings;
    
//    rumboot_printf("\n run_hscb_transfers_vial_loopback start\n");

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

// ================ Common ===============   
struct rumboot_irq_entry * create_irq_handlers(hscb_instance_t* hscb_inst)
{
    rumboot_printf( "Create irq handlers\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    init_hscb_cfg(hscb_inst);
    rumboot_irq_set_handler( tbl, SW0_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[0] );
    rumboot_irq_set_handler( tbl, SW1_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[1] );
    rumboot_irq_set_handler( tbl, SW2_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[2] );
    rumboot_irq_set_handler( tbl, SW3_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[3] );

    rumboot_irq_table_activate( tbl );

    rumboot_irq_enable( SW0_AXI_INT );
    rumboot_irq_enable( SW1_AXI_INT );
    rumboot_irq_enable( SW2_AXI_INT );
    rumboot_irq_enable( SW3_AXI_INT );

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
    test_hscb_im1_data_src = (uint32_t*)rumboot_malloc_from_heap_aligned(src_id, HSCB_TEST_DATA_LEN_BYTES, sizeof(uint64_t));


    test_hscb0_data_im1_src = test_hscb_im1_data_src;
//    test_hscb0_data_im1_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(2, HSCB_TEST_DATA_LEN_BYTES, sizeof(uint32_t));
    test_hscb0_data_im1_dst = (uint32_t*)(EM2_BASE + 0x1ff00);
    
    test_hscb1_data_im1_src = test_hscb_im1_data_src;
//    test_hscb1_data_im1_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(2, HSCB_TEST_DATA_LEN_BYTES, sizeof(uint32_t));
    test_hscb1_data_im1_dst = (uint32_t*)(EM2_BASE + 0x20000);

    test_hscb2_data_im1_src = test_hscb_im1_data_src;
//    test_hscb2_data_im1_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(2, HSCB_TEST_DATA_LEN_BYTES, sizeof(uint32_t));
    test_hscb2_data_im1_dst = (uint32_t*)(EM2_BASE + 0x3ff00);

    test_hscb3_data_im1_src = test_hscb_im1_data_src;
//    test_hscb3_data_im1_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(2, HSCB_TEST_DATA_LEN_BYTES, sizeof(uint32_t));
    test_hscb3_data_im1_dst = (uint32_t*)(EM2_BASE + 0x40000);

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

    for (int i = 0; i < (HSCB_TEST_DATA_LEN_BYTES/sizeof(greth_src_data_im0)); ++i)
    {
        memcpy((uint8_t*)test_hscb_im1_data_src + (i * sizeof(greth_src_data_im0)), greth_src_data_im0, sizeof(greth_src_data_im0));
    }
}

// -----------------------------------------------------------------------------
// -------------------------------- main --------------------------------
// -----------------------------------------------------------------------------

int main(void)
{
    struct rumboot_irq_entry *tbl;

    rumboot_putstring("\n\n\nStart test_oi10_power_3_short_test\n");
    
    emi_init(DCR_EM2_EMI_BASE);
    
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
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x0 ,dcr_read(DCR_EM2_EMI_BASE + 0x0 ));
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x8 ,dcr_read(DCR_EM2_EMI_BASE + 0x8 ));
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x10,dcr_read(DCR_EM2_EMI_BASE + 0x10));
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x18,dcr_read(DCR_EM2_EMI_BASE + 0x18));
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x20,dcr_read(DCR_EM2_EMI_BASE + 0x20));
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x28,dcr_read(DCR_EM2_EMI_BASE + 0x28));
    
    prepare_test_data(2);   // 2 - IM2
    
    tbl = create_irq_handlers(hscb_cfg);

    configure_hscb(hscb_cfg);

    hscb_handler = false;
    rumboot_printf("Start work!\n");
    run_hscb_transfers_vial_loopback(hscb_cfg);

    while (1)
    {
       if (hscb_handler)
       {
            hscb_handler = false;
       }
    }

    rumboot_printf("\nEnd of test\n");
    delete_irq_handlers(tbl);


    return 0;
}
