
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>

#include <platform/interrupts.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/regs/sctl.h>
#include <platform/devices/emi.h>
#include <platform/test_assert.h>

#include <platform/devices/greth.h>
#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/devices/emi.h>
#include <platform/devices/dma2plb6.h>
#include <platform/devices/nor_1636RR4.h>
#include <platform/devices/hscb.h>

#define GRETH_TEST_DATA_LEN_BYTES   250
#define GRETH_EDCL_DATA_LEN_BYTES    16

#define GRETH_TEST_DATA_MISALIGN_IM0  0
#define GRETH_TEST_DATA_MISALIGN_IM1  0
#define GRETH_TEST_DATA_MISALIGN_IM2  0
#define GRETH_TEST_DATA_MISALIGN_EM2  0

#define EDCL_TEST_ADDR_IM0  (IM0_BASE + 0x4000 + 0x100)
#define EDCL_TEST_ADDR_IM1  (IM1_BASE + 0x4000 + 0x100)
#define EDCL_TEST_ADDR_IM2  (IM2_BASE + 0x4000 + 0x100)
#define EDCL_TEST_ADDR_EM2  (EM2_BANK3_BASE)

#define EVENT_CHECK_GRETH0_RX_ER   0x00001000
#define EVENT_CHECK_GRETH1_RX_ER   0x00001001
#define EVENT_CHECK_GRETH0_RX_COL  0x00001002
#define EVENT_CHECK_GRETH1_RX_COL  0x00001003
#define EVENT_CHECK_RUN_HPROT_MONITOR     0x00001004
#define EVENT_CHECK_STOP_HPROT_MONITOR    0x00001005

//setting non-default EDCLADDRL values
#define EDCLADDRL0_TEST  0x0
#define EDCLADDRL1_TEST  0x1
#define EDCLIP0_TEST     (EDCLIP | EDCLADDRL0_TEST)
#define EDCLIP1_TEST     (EDCLIP | EDCLADDRL1_TEST)

#define TST_MAC_MSB     0xFFFF
#define TST_MAC_LSB     0xFFFFFFFF
greth_mac_t tst_greth_mac       = {TST_MAC_MSB, TST_MAC_LSB};
greth_mac_t tst_greth_edcl_mac0 = {EDCLMAC_MSB, (EDCLMAC_LSB | EDCLADDRL0_TEST) };
greth_mac_t tst_greth_edcl_mac1 = {EDCLMAC_MSB, (EDCLMAC_LSB | EDCLADDRL1_TEST) };

#ifdef CHECK_AXI_PLB6_BURST
//initial data in IM0 (will be copy to source ETH areas)
struct test_pattern_t
{
    uint32_t word0[4];
    uint32_t word1[4];
};
#define PERIPH_TEST_DATA_LEN 250
#define PERIPH_TEST_DATA_LEN_BYTES (PERIPH_TEST_DATA_LEN * sizeof(uint32_t))
#define SOURCE_DATA_ARR_SIZE    (PERIPH_TEST_DATA_LEN * sizeof(uint32_t)/sizeof(struct test_pattern_t))
#define PERIPH_TEST_PATTERN_0    0x5A5A5A5A
#define PERIPH_TEST_PATTERN_1    0xA5A5A5A5
static struct test_pattern_t test_hscbx_src_data[SOURCE_DATA_ARR_SIZE] = {[0 ... SOURCE_DATA_ARR_SIZE-1] = {
                                                                                                    .word0 = {PERIPH_TEST_PATTERN_0, PERIPH_TEST_PATTERN_1, PERIPH_TEST_PATTERN_0, PERIPH_TEST_PATTERN_1},
                                                                                                    .word1 = {PERIPH_TEST_PATTERN_1, PERIPH_TEST_PATTERN_0, PERIPH_TEST_PATTERN_1, PERIPH_TEST_PATTERN_0}
                                                                                                 }
                                                                };

static hscb_instance_t  hscb_cfg[4];
static volatile bool hscb_handler[4] = {false, false, false, false};


static uint32_t* test_hscb0_data_im0_src;
static uint32_t* test_hscb0_data_im0_dst;

static uint32_t* test_hscb1_data_im0_src;
static uint32_t* test_hscb1_data_im0_dst;

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

#endif

static uint32_t volatile GRETH0_IRQ_HANDLED = 0;
static uint32_t volatile GRETH1_IRQ_HANDLED = 0;

struct greth_instance {
    uint32_t  base_addr;
    uint32_t volatile*  irq_handled;
};

static void handler_eth( int irq, void *arg )
{
    uint32_t cur_status;
    uint32_t mask;
    struct greth_instance* gr_inst = (struct greth_instance* ) arg;

    rumboot_printf( "GRETH%d(0x%X) IRQ arrived  \n", GET_GRETH_IDX(gr_inst->base_addr), gr_inst->base_addr );
    mask = ((1 << GRETH_STATUS_IA) |
            (1 << GRETH_STATUS_TS) |
            (1 << GRETH_STATUS_TA) |
            (1 << GRETH_STATUS_RA) |
            (1 << GRETH_STATUS_TI) |
            (1 << GRETH_STATUS_TE) |
            (1 << GRETH_STATUS_RI) |
            (1 << GRETH_STATUS_RE));

    cur_status = greth_get_status(gr_inst->base_addr);
    if (cur_status & (1 << GRETH_STATUS_RI))
    {
        *(gr_inst->irq_handled) = 1;
        rumboot_printf("Receive interrupt (0x%x)\n", cur_status );
    }
    if (cur_status & (1 << GRETH_STATUS_RE))
    {
        *(gr_inst->irq_handled) = 3;
        rumboot_printf("Receive Error interrupt (0x%x)\n", cur_status );
    }
    if (!(cur_status & (1 << GRETH_STATUS_RI)) && !(cur_status & (1 << GRETH_STATUS_RE)))
    {
        *(gr_inst->irq_handled) = 2;
        rumboot_printf( "Unexpected status (0x%x)\n", cur_status );
    }
    greth_clear_status_bits(gr_inst->base_addr, mask);
    rumboot_printf("Exit from handler\n");
}

#ifdef CHECK_AXI_PLB6_BURST
static void handler_hscb( int irq, void *arg )
{
    uint32_t adma_ch_status_src;
    hscb_instance_t* hscb_inst = (hscb_instance_t* ) arg;
    uint32_t idx;
    adma_ch_status_src = hscb_get_adma_ch_status(hscb_inst->src_hscb_base_addr);
    idx = GET_HSCB_IDX_BY_ADDR(hscb_inst->src_hscb_base_addr);
    rumboot_printf( "HSCB%d(0x%X) ADMA_IRQ status: 0x%X\n", idx, hscb_inst->src_hscb_base_addr, adma_ch_status_src);
    if (adma_ch_status_src & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
    {
        rumboot_printf( "Detected WDMA_IRQ status: 0x%X\n", hscb_get_wdma_status(hscb_inst->src_hscb_base_addr));
    }
    else
    {
        TEST_ASSERT(0, "Unexpected IRQ status");
    }

    hscb_handler[idx] = true;
}
#endif

static struct greth_instance in[ ] = {
                                        {
                                            .base_addr   = GRETH_0_BASE,
                                            .irq_handled = &GRETH0_IRQ_HANDLED
                                        },
                                        {
                                            .base_addr   = GRETH_1_BASE,
                                            .irq_handled = &GRETH1_IRQ_HANDLED
                                        }
                                     };

#ifdef CHECK_AXI_PLB6_BURST
void init_hscb_cfg(hscb_instance_t* hscb_inst)
{
    (hscb_inst + 0)->src_hscb_base_addr   = HSCB0_BASE;
    (hscb_inst + 0)->dst_hscb_base_addr   = HSCB1_BASE;
    (hscb_inst + 0)->src_addr             = test_hscb0_data_im0_src;
    (hscb_inst + 0)->dst_addr             = test_hscb0_data_im0_dst;
    (hscb_inst + 0)->src_size             = sizeof(test_hscbx_src_data);
    (hscb_inst + 0)->dst_size             = sizeof(test_hscbx_src_data);
    (hscb_inst + 0)->tx_descr_addr        = (uint32_t)HSCB0_TX_DESCR_ADDR;
    (hscb_inst + 0)->rx_descr_addr        = (uint32_t)HSCB0_RX_DESCR_ADDR;

    (hscb_inst + 1)->src_hscb_base_addr   = HSCB1_BASE;
    (hscb_inst + 1)->dst_hscb_base_addr   = HSCB0_BASE;
    (hscb_inst + 1)->src_addr             = test_hscb1_data_im0_src;
    (hscb_inst + 1)->dst_addr             = test_hscb1_data_im0_dst;
    (hscb_inst + 1)->src_size             = sizeof(test_hscbx_src_data);
    (hscb_inst + 1)->dst_size             = sizeof(test_hscbx_src_data);
    (hscb_inst + 1)->tx_descr_addr        = (uint32_t)HSCB1_TX_DESCR_ADDR;
    (hscb_inst + 1)->rx_descr_addr        = (uint32_t)HSCB1_RX_DESCR_ADDR;

    (hscb_inst + 2)->src_hscb_base_addr   = HSCB2_BASE;
    (hscb_inst + 2)->dst_hscb_base_addr   = HSCB3_BASE;
    (hscb_inst + 2)->src_addr             = test_hscb2_data_im1_src;
    (hscb_inst + 2)->dst_addr             = test_hscb2_data_im1_dst;
    (hscb_inst + 2)->src_size             = sizeof(test_hscbx_src_data);
    (hscb_inst + 2)->dst_size             = sizeof(test_hscbx_src_data);
    (hscb_inst + 2)->tx_descr_addr        = (uint32_t)HSCB2_TX_DESCR_ADDR;
    (hscb_inst + 2)->rx_descr_addr        = (uint32_t)HSCB2_RX_DESCR_ADDR;

    (hscb_inst + 3)->src_hscb_base_addr   = HSCB3_BASE;
    (hscb_inst + 3)->dst_hscb_base_addr   = HSCB2_BASE;
    (hscb_inst + 3)->src_addr             = test_hscb3_data_im1_src;
    (hscb_inst + 3)->dst_addr             = test_hscb3_data_im1_dst;
    (hscb_inst + 3)->src_size             = sizeof(test_hscbx_src_data);
    (hscb_inst + 3)->dst_size             = sizeof(test_hscbx_src_data);
    (hscb_inst + 3)->tx_descr_addr        = (uint32_t)HSCB3_TX_DESCR_ADDR;
    (hscb_inst + 3)->rx_descr_addr        = (uint32_t)HSCB3_RX_DESCR_ADDR;
}

struct rumboot_irq_entry * create_hscb_irq_handlers(hscb_instance_t* hscb_inst)
{
    rumboot_printf( "Create hscb irq handlers\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );


    init_hscb_cfg(hscb_inst);
    rumboot_irq_set_handler( tbl, SW0_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[0] );
    rumboot_irq_set_handler( tbl, SW1_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[1] );
    rumboot_irq_set_handler( tbl, SW2_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[2] );
    rumboot_irq_set_handler( tbl, SW3_AXI_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_hscb, &hscb_inst[3] );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( SW0_AXI_INT );
    rumboot_irq_enable( SW1_AXI_INT );
    rumboot_irq_enable( SW2_AXI_INT );
    rumboot_irq_enable( SW3_AXI_INT );

    rumboot_irq_sei();

    return tbl;
}
#endif

struct rumboot_irq_entry * create_greth01_irq_handlers()
{
    rumboot_printf( "Enable GRETH irqs\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    GRETH0_IRQ_HANDLED = 0;
    GRETH1_IRQ_HANDLED = 0;

    rumboot_irq_set_handler( tbl, ETH0_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_eth, &in[0] );
    rumboot_irq_set_handler( tbl, ETH1_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_eth, &in[1] );

    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( ETH0_INT );
    rumboot_irq_enable( ETH1_INT );
    rumboot_irq_sei();

    return tbl;
}

void delete_irq_handlers(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}



uint32_t edcl_seq_number = 0;
uint8_t* edcl_test_data_im0_wr;


greth_descr_t*  tx_descriptor_data_;
greth_descr_t*  rx_descriptor_data_;

uint8_t* test_data_im0_src;
uint8_t* test_data_im0_dst;


void mdio_check(uint32_t base_addr)
{
    rumboot_printf("MDIO check for GRETH%d(0x%x)\n", GET_GRETH_IDX(base_addr), base_addr);
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_CTRL      )==ETH_PHY_CTRL_DEFAULT      , "Error at mdio reading ETH_PHY_CTRL       register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_STATUS    )==ETH_PHY_STATUS_DEFAULT    , "Error at mdio reading ETH_PHY_STATUS     register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_ID0       )==ETH_PHY_ID0_DEFAULT       , "Error at mdio reading ETH_PHY_ID0        register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_ID1       )==ETH_PHY_ID1_DEFAULT       , "Error at mdio reading ETH_PHY_ID1        register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_ANEGADV   )==ETH_PHY_ANEGADV_DEFAULT   , "Error at mdio reading ETH_PHY_ANEGADV    register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_ANEGLP    )==ETH_PHY_ANEGLP_DEFAULT    , "Error at mdio reading ETH_PHY_ANEGLP     register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_ANEGXP    )==ETH_PHY_ANEGXP_DEFAULT    , "Error at mdio reading ETH_PHY_ANEGXP     register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_ANEGNPTX  )==ETH_PHY_ANEGNPTX_DEFAULT  , "Error at mdio reading ETH_PHY_ANEGNPTX   register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_ANEGNPLP  )==ETH_PHY_ANEGNPLP_DEFAULT  , "Error at mdio reading ETH_PHY_ANEGNPLP   register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_MSTSLVCTRL)==ETH_PHY_MSTSLVCTRL_DEFAULT, "Error at mdio reading ETH_PHY_MSTSLVCTRL register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_MSTSLVSTAT)==ETH_PHY_MSTSLVSTAT_DEFAULT, "Error at mdio reading ETH_PHY_MSTSLVSTAT register\n");
    TEST_ASSERT(greth_mdio_read(base_addr, ETH_PHY_ADDR, ETH_PHY_EXTSTATUS )==ETH_PHY_EXTSTATUS_DEFAULT , "Error at mdio reading ETH_PHY_EXTSTATUS  register\n");
}

#define GRETH_SRC_DATA_SEL  true
#define GRETH_DST_DATA_SEL  false



void prepare_test_data()
{
    uint32_t i=0;

    //bool toggle = true;

    rumboot_putstring("Preparing data");
    tx_descriptor_data_ = (greth_descr_t*) rumboot_malloc_from_heap_aligned(1, 3*sizeof(greth_descr_t), 1024);
    rx_descriptor_data_ = (greth_descr_t*) rumboot_malloc_from_heap_aligned(1, 3*sizeof(greth_descr_t), 1024);
    rumboot_printf("Allocate from im1 for descrs(0x%X / 0x%x)\n", (uint32_t) tx_descriptor_data_, (uint32_t) rx_descriptor_data_);

    test_data_im0_src = (uint8_t* )rumboot_malloc_from_heap_misaligned(0, GRETH_TEST_DATA_LEN_BYTES, 16, GRETH_TEST_DATA_MISALIGN_IM0);
    rumboot_printf("Allocate from im0 for src (0x%X)\n", (uint32_t) test_data_im0_src);
    test_data_im0_dst = (uint8_t* )rumboot_malloc_from_heap_misaligned(0, GRETH_TEST_DATA_LEN_BYTES, 16, GRETH_TEST_DATA_MISALIGN_IM0);
    rumboot_printf("Allocate from im0 for dst (0x%X)\n", (uint32_t) test_data_im0_dst);

    rumboot_putstring("Fill im0 src array");
    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x00FF0000) >> 16);
    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0xFF000000) >> 24);
    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x000000FF) >>  0);
    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x0000FF00) >>  8);

    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x000000FF) >>  0);
    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x0000FF00) >>  8);
    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x000000FF) >>  0);
    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x0000FF00) >>  8);

    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x000000FF) >>  0);
    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x0000FF00) >>  8);
    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x00FF0000) >> 16);
    test_data_im0_src[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0xFF000000) >> 24);

    test_data_im0_src[i++] = (uint8_t)(((GRETH_TEST_DATA_LEN_BYTES - 14) & 0x00FF) >> 0);
    test_data_im0_src[i++] = (uint8_t)(((GRETH_TEST_DATA_LEN_BYTES - 14) & 0xFF00) >> 8);
    test_data_im0_src[i++] = (uint8_t)(0);
    test_data_im0_src[i++] = (uint8_t)(0);

    while(i<GRETH_TEST_DATA_LEN_BYTES)
    {
        test_data_im0_src[i] = i;
        i++;
    }
#ifdef CHECK_AXI_PLB6_BURST
    test_hscb0_data_im0_src = (uint32_t*)rumboot_malloc_from_heap_aligned(0, PERIPH_TEST_DATA_LEN_BYTES, 16/*sizeof(uint32_t)*/);
    test_hscb0_data_im0_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(0, PERIPH_TEST_DATA_LEN_BYTES, 16/*sizeof(uint32_t)*/);

    test_hscb1_data_im0_src = (uint32_t*)rumboot_malloc_from_heap_aligned(0, PERIPH_TEST_DATA_LEN_BYTES, 16/*sizeof(uint32_t)*/);
    test_hscb1_data_im0_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(0, PERIPH_TEST_DATA_LEN_BYTES, 16/*sizeof(uint32_t)*/);

    test_hscb2_data_im1_src = (uint32_t*)rumboot_malloc_from_heap_aligned(1, PERIPH_TEST_DATA_LEN_BYTES, 16/*sizeof(uint32_t)*/);
    test_hscb2_data_im1_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(1, PERIPH_TEST_DATA_LEN_BYTES, 16/*sizeof(uint32_t)*/);

    test_hscb3_data_im1_src = (uint32_t*)rumboot_malloc_from_heap_aligned(1, PERIPH_TEST_DATA_LEN_BYTES, 16/*sizeof(uint32_t)*/);
    test_hscb3_data_im1_dst = (uint32_t*)rumboot_malloc_from_heap_aligned(1, PERIPH_TEST_DATA_LEN_BYTES, 16/*sizeof(uint32_t)*/);

    memcpy(test_hscb0_data_im0_src,  test_hscbx_src_data, sizeof(test_hscbx_src_data));
    memcpy(test_hscb1_data_im0_src,  test_hscbx_src_data, sizeof(test_hscbx_src_data));
    memcpy(test_hscb2_data_im1_src,  test_hscbx_src_data, sizeof(test_hscbx_src_data));
    memcpy(test_hscb3_data_im1_src,  test_hscbx_src_data, sizeof(test_hscbx_src_data));

    HSCB0_TX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(0, 0x40, sizeof(uint32_t));
    HSCB0_RX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(0, 0x40, sizeof(uint32_t));
    HSCB1_TX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(0, 0x40, sizeof(uint32_t));
    HSCB1_RX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(0, 0x40, sizeof(uint32_t));

    HSCB2_TX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, 0x40, sizeof(uint32_t));
    HSCB2_RX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, 0x40, sizeof(uint32_t));
    HSCB3_TX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, 0x40, sizeof(uint32_t));
    HSCB3_RX_DESCR_ADDR = (uint32_t*)rumboot_malloc_from_heap_aligned(1, 0x40, sizeof(uint32_t));
#endif

    rumboot_putstring("Preparing data finished");
}

void check_transfer_via_external_loopback(uint32_t base_addr_src_eth,  uint8_t * test_data_src, uint8_t * test_data_dst)
{
    uint32_t base_addr_dst_eth;
    uint32_t volatile* eth_handled_flag_ptr;

    //test_event(EVENT_CHECK_RUN_HPROT_MONITOR);//checking switch u_nic400_oi10_axi32.hprot_eth_1(0)_s from 0x3 to 0xF (by request from JIRA-78)
    //dcr_write(DCR_SCTL_BASE + SCTL_IFSYS_ETH_HPROT, 0x3F3F3F3F);

    TEST_ASSERT((base_addr_src_eth==GRETH_1_BASE)||(base_addr_src_eth==GRETH_0_BASE), "Wrong GRETH base address\n");
    base_addr_dst_eth = base_addr_src_eth==GRETH_0_BASE ? GRETH_1_BASE : GRETH_0_BASE;

    if (base_addr_src_eth==GRETH_1_BASE)
    {
        base_addr_dst_eth = GRETH_0_BASE;
        GRETH0_IRQ_HANDLED = 0;
        eth_handled_flag_ptr = &GRETH0_IRQ_HANDLED;
    }
    else
    {
        base_addr_dst_eth = GRETH_1_BASE;
        GRETH1_IRQ_HANDLED = 0;
        eth_handled_flag_ptr = &GRETH1_IRQ_HANDLED;
    }

    rumboot_printf("\nChecking transfer from 0x%X to 0x%x\n", (uint32_t)test_data_src, (uint32_t) test_data_dst);

    greth_configure_for_receive( base_addr_dst_eth, test_data_dst, GRETH_TEST_DATA_LEN_BYTES, rx_descriptor_data_, &tst_greth_mac);
    greth_configure_for_transmit( base_addr_src_eth, test_data_src, GRETH_TEST_DATA_LEN_BYTES, tx_descriptor_data_, &tst_greth_mac);

    greth_start_receive( base_addr_dst_eth, true );
    greth_start_transmit( base_addr_src_eth );

    TEST_ASSERT(greth_wait_receive_irq(base_addr_dst_eth, eth_handled_flag_ptr), "Receiving is failed\n");
    TEST_ASSERT(memcmp(test_data_src, test_data_dst, GRETH_TEST_DATA_LEN_BYTES)==0, "Data compare error!\n");

    memset(test_data_dst, 0, GRETH_TEST_DATA_LEN_BYTES);

    rumboot_free(tx_descriptor_data_);
    rumboot_free(rx_descriptor_data_);
    //test_event(EVENT_CHECK_STOP_HPROT_MONITOR);
}

void test_oi10_greth (){
	struct rumboot_irq_entry *tbl;
    rumboot_printf("Start test_oi10_greth. Transmit/receive checks\n");
    //test_event_send_test_id("test_oi10_greth");
    tbl = create_greth01_irq_handlers();
    prepare_test_data();
    check_transfer_via_external_loopback(GRETH_0_BASE, (uint8_t*)rumboot_virt_to_dma(test_data_im0_src), (uint8_t*)rumboot_virt_to_dma(test_data_im0_dst));
    delete_irq_handlers(tbl);
}

//-------------------------------------------------------------------------------------------------

#define SHOW_STATUS(status) switch(status){ \
                case error_alignnment: \
                case error_scatter_alignment: \
                    rumboot_printf("DMA2PLB6: Error alignment\n"); \
                    break; \
                case error_read_data_regular: \
                case error_read_data_scatter_or_resume: \
                    rumboot_printf("DMA2PLB6: Error read data\n"); \
                    break; \
                case error_read_request_regular: \
                case error_read_request_scatter_or_resume: \
                    rumboot_printf("DMA2PLB6: Error read request\n"); \
                    break; \
                case error_write_request: \
                    rumboot_printf("DMA2PLB6: Error write request\n"); \
                    break; \
                }


#define TEST_DATA_SIZE 128
#define TEST_DATA_CONTENT 0xFEDCBA9876543210

 #define stmw(base, shift)\
    asm volatile (\
        "addis 26, 0,  0x0101 \n\t"\
        "addi  26, 26, 0x0101 \n\t"\
        "addis 27, 0,  0x0101 \n\t"\
        "addi  27, 27, 0x0101 \n\t"\
        "addis 28, 0,  0x0101 \n\t"\
        "addi  28, 28, 0x0101 \n\t"\
        "addis 29, 0,  0x0101 \n\t"\
        "addi  29, 29, 0x0101 \n\t"\
        "addis 30, 0,  0x0101 \n\t"\
        "addi  30, 30, 0x0101 \n\t"\
        "addis 31, 0,  0x0202 \n\t"\
        "addi  31, 31, 0x0202 \n\t"\
        "addis 25, 0, %0 \n\t"\
        "stmw 26, %1(25) \n\t"\
        ::"i"(base), "i"(shift)\
    )

struct regpoker_checker greth_check_array[] = {
    { "CTRL              ",  REGPOKER_READ32,  CTRL              , 0x98000090, 0xFE007CFF },
    { "MDIO_CTRL         ",  REGPOKER_READ32,  MDIO_CTRL         , 0x01E10140, 0xFFFFFFCF },
    { "TRANSMIT_DESCR_PTR",  REGPOKER_READ8,  TRANSMIT_DESCR_PTR, 0x0, 0x3F8 },
    { "RECEIVER_DESCR_PTR",  REGPOKER_READ8,  RECEIVER_DESCR_PTR, 0x0, 0x3F8 },
    { "EDCL_IP           ",  REGPOKER_READ32,  EDCL_IP           , EDCLIP , ~0 },
    { "EDCL_MAC_MSB      ",  REGPOKER_READ32,  EDCL_MAC_MSB      , EDCLMAC_MSB, 0xffff },
    { "EDCL_MAC_LSB      ",  REGPOKER_READ32,  EDCL_MAC_LSB      , EDCLMAC_LSB, ~0 },
    { "EDCL_IP           ",  REGPOKER_WRITE32, EDCL_IP           , 0, ~0 },
    { "EDCL_MAC_MSB      ",  REGPOKER_WRITE32, EDCL_MAC_MSB      , 0, 0xffff },
    { "EDCL_MAC_LSB      ",  REGPOKER_WRITE32, EDCL_MAC_LSB      , 0, ~0 }
};

void regs_check(uint32_t base_addr)
{
    if (base_addr==GRETH_1_BASE)
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n", 1, base_addr);
        greth_check_array[4].expected = EDCLIP1;
        greth_check_array[6].expected |= EDCLADDRL1;
    }
    else
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n", 0, base_addr);
    }
    TEST_ASSERT(rumboot_regpoker_check_array(greth_check_array, base_addr)==0, "Failed to check GRETH registers\n");
}


static void fill(uint64_t *s, uint64_t pattern, uint32_t size_in_bytes)
{
    uint32_t i;
    for (i = 0; i < (size_in_bytes>>3); i++)
        s[i] = pattern;
}

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

static uint64_t fill_word = TEST_DATA_CONTENT;

static uint32_t compare(uint32_t s_addr, uint32_t d_addr,uint32_t size_in_bytes)
{
    uint32_t i;
    volatile uint64_t temp0, temp1;
    for (i = 0; i < size_in_bytes; i+=8){

        rumboot_printf("offcet: 0x%x\n", i);

        temp0 = ioread64(s_addr + i);
        temp1 = ioread64(d_addr + i);

        //rumboot_printf("get source data: 0x%x%x\n", (uint32_t)(temp0>>32),(uint32_t)(temp0));
        //rumboot_printf("get dest data: 0x%x%x\n", (uint32_t)(temp1>>32),(uint32_t)(temp1));

        //msync();

        if(temp0 != temp1)
        {
            rumboot_printf("compare failed, expected value: 0x%x%x\n", (uint32_t)(temp0>>32),(uint32_t)(temp0));
            rumboot_printf("read value: 0x%x%x\n", (uint32_t)(temp1>>32),(uint32_t)(temp1));
            return false;
        }
    }
    return true;
}

uint32_t check_dma2plb6_0_mem_to_mem(uint32_t source_ea, uint32_t dest_ea, uint64_t source_phys, uint64_t dest_phys)
{
    dma2plb6_setup_info dma_info;
    channel_status status = {};
    dma_info.base_addr = DCR_DMAPLB6_BASE;
    dma_info.source_adr = source_phys;
    dma_info.dest_adr = dest_phys;
    dma_info.priority = priority_medium_low;
    dma_info.striding_info.striding = striding_none;
    dma_info.tc_int_enable = false;
    dma_info.err_int_enable = false;
    dma_info.int_mode = int_mode_level_wait_clr;
    dma_info.channel = channel0;
    for (dma_info.rw_transfer_size = rw_tr_size_1q; dma_info.rw_transfer_size <= rw_tr_size_8q; dma_info.rw_transfer_size++)
    {
        rumboot_printf("rw_transfer_size = 0x%x\n", dma_info.rw_transfer_size);
        for(dma_info.transfer_width = tr_width_byte; dma_info.transfer_width <= tr_width_quadword; dma_info.transfer_width++)
        {
            rumboot_printf("transfer_width = 0x%x\n", dma_info.transfer_width);

            rumboot_printf("fill source area\n");
            fill((uint64_t *)source_ea, fill_word, TEST_DATA_SIZE);

            rumboot_printf("fill dest area\n");
            fill((uint64_t *)dest_ea, 0x0, TEST_DATA_SIZE);




            dma_info.count = TEST_DATA_SIZE/dma2plb6_get_bytesize(dma_info.transfer_width);

            rumboot_printf("run dma copy\n");
            if(dma2plb6_single_copy(&dma_info, &status) == false){
                SHOW_STATUS(status.spec_error_status);
                return false;
            } else {
                rumboot_printf("run compare data\n");
                TEST_ASSERT(compare(source_ea, dest_ea, TEST_DATA_SIZE) == true, "Memory comparison failed");
                fill_word = ~fill_word;
                rumboot_printf("done: tr_width = 0x%x\n", dma_info.transfer_width);
            }
        }
    }
    return true;
}


static void byte_plb_axi_test (uint32_t base_addr){
    uint32_t i = 0;
    uint8_t temp = 0;

    do {
        iowrite8(i,base_addr+i);
        i++;
    } while (i!=0x100);

    i=0;
    do {
        temp = ioread8(base_addr + i);
        if (i != temp){
            //rumboot_printf("[hword_plb_axi_test] Memory comparison failed exp 0x%x act 0x%x \n", i, temp);
            TEST_ASSERT(0, "");
        }
        i++;
    } while (i!=0x100);
}


static void hword_plb_axi_test (uint32_t base_addr, uint32_t offc){
    uint32_t i = 0;
    uint16_t tdata = 0x1;
    uint16_t temp = 0;

    do {
        //rumboot_printf("[hword_plb_axi_test] write data 0x%x to addr 0x%x \n", tdata, base_addr+i+offc);
        iowrite16(tdata,base_addr+i+offc);
        tdata = tdata << 1;
        i+=2;
    } while(tdata!=0x0);

    i=0;
    tdata=0x1;
    do {
        temp = ioread16(base_addr+i+offc);
        //rumboot_printf("[hword_plb_axi_test] read data 0x%x in addr 0x%x \n", temp, base_addr+i+offc);
        if (tdata != temp){
            rumboot_printf("[hword_plb_axi_test] Memory comparison failed exp 0x%x act 0x%x \n", tdata, temp);
            TEST_ASSERT(0, "");
        }

        tdata = tdata << 1;
        i+=2;
    } while (tdata!=0x0);
}

static void word_plb_axi_test (uint32_t base_addr, uint32_t offc){
    uint32_t i = 0;
    uint32_t tdata = 0x1;
    uint32_t temp = 0;

    do {
        //rumboot_printf("[word_plb_axi_test] write data 0x%x to addr 0x%x \n", tdata, base_addr+i+offc);
        iowrite32(tdata,base_addr+i+offc);
        tdata = tdata << 1;
        i+=4;
    } while(tdata!=0x0);

    i=0;
    tdata=0x1;
    do {
        temp = ioread32(base_addr+i+offc);
        //rumboot_printf("[word_plb_axi_test] read data 0x%x in addr 0x%x \n", temp, base_addr+i+offc);
        if (tdata != temp){
            rumboot_printf("[word_plb_axi_test] Memory comparison failed exp 0x%x act 0x%x \n", tdata, temp);
            TEST_ASSERT(0, "");
        }

        tdata = tdata << 1;
        i+=4;
    } while (tdata!=0x0);
}


static void dword_plb_axi_test (uint32_t base_addr, uint32_t offc){
    uint32_t i = 0;
    uint64_t tdata = 0x1;
    uint64_t temp = 0;

    do {
        //rumboot_printf("[dword_plb_axi_test] write data 0x%x%x to addr 0x%x \n", (uint32_t)(tdata>>32), (uint32_t)(tdata&0xFFFFFFFF), base_addr+i+offc);
        iowrite64(tdata,base_addr+i+offc);
        tdata = tdata << 1;
        i+=8;
    } while(tdata!=0x0);

    i=0;
    tdata=0x1;
    do {
        temp = ioread64(base_addr+i+offc);
        //rumboot_printf("[dword_plb_axi_test] read data 0x%x%x in addr 0x%x \n", (uint32_t)(temp>>32), (uint32_t)(temp&0xFFFFFFFF), base_addr+i+offc);
        if (tdata != temp){
            rumboot_printf("[dword_plb_axi_test] Memory comparison failed exp 0x%x act 0x%x \n", (uint32_t)(tdata>>32), (uint32_t)(tdata&0xFFFFFFFF) , (uint32_t)(temp>>32), (uint32_t)(temp&0xFFFFFFFF));
            TEST_ASSERT(0, "");
        }

        tdata = tdata << 1;
        i+=8;
    } while (tdata!=0x0);
}

void single_plb6_axi_test (uint32_t base_addr) {
    byte_plb_axi_test (base_addr);

    hword_plb_axi_test (base_addr, 0x0);
    hword_plb_axi_test (base_addr, 0x1);
    hword_plb_axi_test (base_addr, 0x2);
    hword_plb_axi_test (base_addr, 0x3);
    hword_plb_axi_test (base_addr, 0x4);
    hword_plb_axi_test (base_addr, 0x5);
    hword_plb_axi_test (base_addr, 0x6);
    hword_plb_axi_test (base_addr, 0x7);

    word_plb_axi_test (base_addr, 0x0);
    word_plb_axi_test (base_addr, 0x1);
    word_plb_axi_test (base_addr, 0x2);
    word_plb_axi_test (base_addr, 0x3);
    word_plb_axi_test (base_addr, 0x4);
    word_plb_axi_test (base_addr, 0x5);
    word_plb_axi_test (base_addr, 0x6);
    word_plb_axi_test (base_addr, 0x7);

    dword_plb_axi_test (base_addr, 0x0);
    dword_plb_axi_test (base_addr, 0x1);
    dword_plb_axi_test (base_addr, 0x2);
    dword_plb_axi_test (base_addr, 0x3);
    dword_plb_axi_test (base_addr, 0x4);
    dword_plb_axi_test (base_addr, 0x5);
    dword_plb_axi_test (base_addr, 0x6);
    dword_plb_axi_test (base_addr, 0x7);
}
#ifdef CHECK_AXI_PLB6_BURST
void run_hscb_transfers_via_external_loopback(hscb_instance_t* hscb_inst)
{
    int i;
    //run wdma
    for (i=0; i<4; i++)
        hscb_run_wdma((hscb_inst+i)->src_hscb_base_addr);
    //run rdma
    for (i=0; i<4; i++)
        hscb_run_rdma((hscb_inst+i)->src_hscb_base_addr);
}

void test_oi10_hscb()
{
    struct rumboot_irq_entry *tbl;
    rumboot_printf("Start test_oi10_hscb. Transmit/receive checks\n");
    //test_event_send_test_id("test_oi10_greth");
    tbl = create_greth01_irq_handlers();
    prepare_test_data();
    check_transfer_via_external_loopback(GRETH_0_BASE, (uint8_t*)rumboot_virt_to_dma(test_data_im0_src), (uint8_t*)rumboot_virt_to_dma(test_data_im0_dst));
    delete_irq_handlers(tbl);
}

void configure_hscb(hscb_instance_t* hscb_inst, hscb_axi_arwlen_t hscb_axi_arwlen, hscb_axi_arwburst_t hscb_axi_arwburst)
{
    int i;
    hscb_axi_params_cfg_t axi_params_cfg;
    rumboot_printf("\n---\nApply software reset HSCBs\n");
    for (i=0; i<4; i++)
    {
        hscb_sw_rst((hscb_inst+i)->src_hscb_base_addr);
        hscb_adma_sw_rst((hscb_inst+i)->src_hscb_base_addr);
    }

    //config
    for (i=0; i<4; i++)
    {
        hscb_config_for_receive_and_transmit(hscb_inst + i);
        hscb_get_axi_params((hscb_inst+i)->src_hscb_base_addr, &axi_params_cfg);
        axi_params_cfg.arlen   = hscb_axi_arwlen;
        axi_params_cfg.awlen   = hscb_axi_arwlen;
        axi_params_cfg.arburst = hscb_axi_arwburst;
        axi_params_cfg.awburst = hscb_axi_arwburst;
        hscb_set_axi_params((hscb_inst+i)->src_hscb_base_addr, &axi_params_cfg);
    }

    //enable
    for (i=0; i<4; i++)
        hscb_enable((hscb_inst+i)->src_hscb_base_addr);

    //wait status
    for (i=0; i<4; i++)
        TEST_ASSERT(hscb_wait_status((hscb_inst+i)->src_hscb_base_addr, HSCB_STATUS_ACTIVE_LINK_mask), "Failed to waiting ACTIVE_LINK on HSCB");
}

void hscb_memcmp(hscb_instance_t* hscb_cfg)
{
#define HSCB_TO_VAL     1000
    uint32_t timeout = 0;

    while(
           ((hscb_handler[0]==false) ||
            (hscb_handler[1]==false) ||
            (hscb_handler[2]==false) ||
            (hscb_handler[3]==false)) && (timeout++ < 1000)
            );

    for (int i=0; i<4; i++)
        rumboot_printf("hscb_handler[%d] = %d\n", i, hscb_handler[i]);

    TEST_ASSERT(timeout<HSCB_TO_VAL, "HSCB finish is timed out");

    TEST_ASSERT(memcmp((hscb_cfg + 0)->src_addr, (hscb_cfg + 1)->dst_addr, sizeof(test_hscbx_src_data))==0, "HSCB0->1 data compare error!\n");
    TEST_ASSERT(memcmp((hscb_cfg + 1)->src_addr, (hscb_cfg + 0)->dst_addr, sizeof(test_hscbx_src_data))==0, "HSCB1->0 data compare error!\n");
    TEST_ASSERT(memcmp((hscb_cfg + 2)->src_addr, (hscb_cfg + 3)->dst_addr, sizeof(test_hscbx_src_data))==0, "HSCB2->3 data compare error!\n");
    TEST_ASSERT(memcmp((hscb_cfg + 3)->src_addr, (hscb_cfg + 2)->dst_addr, sizeof(test_hscbx_src_data))==0, "HSCB3->2 data compare error!\n");

    memset((hscb_cfg + 0)->dst_addr, 0, sizeof(test_hscbx_src_data));
    memset((hscb_cfg + 1)->dst_addr, 0, sizeof(test_hscbx_src_data));
    memset((hscb_cfg + 2)->dst_addr, 0, sizeof(test_hscbx_src_data));
    memset((hscb_cfg + 3)->dst_addr, 0, sizeof(test_hscbx_src_data));
}
#endif

int main(void)
{

#ifdef CHECK_PLB6_AXI_SINGLE
    rumboot_putstring("Start test_plb6_axi_greth. Apply access to GRETH0 regs for checking PLB6->AXI convertion for single transactions\n");
    regs_check(GRETH_0_BASE);
    rumboot_printf("End check GRETH registers. \n");
    single_plb6_axi_test (IM1_BASE);
#endif


#ifdef CHECK_PLB6_AXI_BURST
    uint32_t src_im1, dst_im1;
    volatile uint64_t src_im1_physical, dst_im1_physical;

    src_im1 = (IM1_BASE);
    rumboot_printf("src_im1 = 0x%x\n", src_im1);

    dst_im1 = (IM1_BASE + 0x80);
    rumboot_printf("dst_im1 = 0x%x\n", dst_im1);

    set_mem_window(MEM_WINDOW_0);
    src_im1_physical = rumboot_virt_to_phys((uint32_t*)src_im1);
    rumboot_printf("src_im1_physical = 0x%x%x \n", src_im1_physical >> 32, src_im1_physical & 0xFFFFFFFF);

    dst_im1_physical = rumboot_virt_to_phys((uint32_t*)dst_im1);
    rumboot_printf("dst_im1_physical = 0x%x%x \n", dst_im1_physical >> 32,dst_im1_physical & 0xFFFFFFFF);

    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_im1,
                                            dst_im1,
                                            src_im1_physical,
                                            dst_im1_physical) == true, "IM1-to-IM1 failed");
#endif

#ifdef CHECK_AXI_PLB6_SINGLE
    test_oi10_greth ();
#endif

#ifdef CHECK_AXI_PLB6_BURST
    struct rumboot_irq_entry *tbl;

    #define ARWLEN_ARR_SIZE       4
    #define ARWBURST_ARR_SIZE     3

    hscb_axi_arwlen_t       hscb_axi_arwlen_arr[ARWLEN_ARR_SIZE] = {
            HSCB_ARWLEN_2,
            HSCB_ARWLEN_4,
            HSCB_ARWLEN_8,
            HSCB_ARWLEN_16
                                                     };

    hscb_axi_arwburst_t       hscb_axi_arwburst[ARWBURST_ARR_SIZE] = {
            HSCB_ARWBURST_FIXED,
            HSCB_ARWBURST_INCR,
            HSCB_ARWBURST_WRAP
                                                     };

    rumboot_printf("Start test_oi10_hscb. Transmit/receive checks\n");
    prepare_test_data();
    tbl = create_hscb_irq_handlers(hscb_cfg);

    for (int i=0; i<ARWLEN_ARR_SIZE; i++)
    {
        for (int j=0; j<ARWBURST_ARR_SIZE; j++)
        {
            rumboot_printf("Check with ARWLEN: 0x%x / ARWBURST: 0x%x\n", hscb_axi_arwlen_arr[i], hscb_axi_arwburst[j]);
            configure_hscb(hscb_cfg, hscb_axi_arwlen_arr[i], hscb_axi_arwburst[j]);
            run_hscb_transfers_via_external_loopback(hscb_cfg);
            hscb_memcmp(hscb_cfg);
        }
    }

    delete_irq_handlers(tbl);
#endif


    rumboot_putstring("TEST complete");
    return 0;
}
