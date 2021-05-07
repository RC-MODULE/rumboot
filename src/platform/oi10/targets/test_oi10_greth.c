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
#include <regs/fields/mpic128.h>
#include <platform/regs/sctl.h>
#include <devices/ugly/emi.h>
#include <platform/test_assert.h>

#include <platform/devices/greth.h>

#define GRETH_TEST_DATA_LEN_BYTES    65
#define GRETH_EDCL_DATA_LEN_BYTES    16

#define GRETH_TEST_DATA_MISALIGN_IM1  0
#define GRETH_TEST_DATA_MISALIGN_IM2  0
#define GRETH_TEST_DATA_MISALIGN_EM2  0

static uintptr_t EDCL_TEST_ADDR;
//#define EDCL_TEST_ADDR_IM1  (IM1_BASE + 0x4000 + 0x100)
//#define EDCL_TEST_ADDR_IM2  (IM2_BASE + 0x4000 + 0x100)
//#define EDCL_TEST_ADDR_EM2  (EM2_BANK3_BASE)

#define EVENT_CHECK_GRETH0_RX_ER   0x00001000
#define EVENT_CHECK_GRETH1_RX_ER   0x00001001
#define EVENT_CHECK_GRETH0_RX_COL  0x00001002
#define EVENT_CHECK_GRETH1_RX_COL  0x00001003
#define EVENT_CHECK_RUN_HPROT_MONITOR     0x00001004
#define EVENT_CHECK_STOP_HPROT_MONITOR    0x00001005

//setting non-default EDCLADDRL values
#define EDCLADDRL0_TEST  0x1
#define EDCLADDRL1_TEST  0x2
#define EDCLIP0_TEST     (EDCLIP | EDCLADDRL0_TEST)
#define EDCLIP1_TEST     (EDCLIP | EDCLADDRL1_TEST)

#define TST_MAC_MSB     0xFFFF
#define TST_MAC_LSB     0xFFFFFFFF
greth_mac_t tst_greth_mac       = {TST_MAC_MSB, TST_MAC_LSB};
greth_mac_t tst_greth_edcl_mac0 = {EDCLMAC_MSB, (EDCLMAC_LSB | EDCLADDRL0_TEST) };
greth_mac_t tst_greth_edcl_mac1 = {EDCLMAC_MSB, (EDCLMAC_LSB | EDCLADDRL1_TEST) };

#ifndef SRC_HEAP_NAME
 #define SRC_HEAP_NAME "IM1"
#endif

#ifndef DST_HEAP_NAME
 #define DST_HEAP_NAME "IM1"
#endif
/*
 * Functions and variables for interrupt handling
 */
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


struct rumboot_irq_entry * create_greth01_irq_handlers()
{
    rumboot_printf( "Enable GRETH irqs\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    GRETH0_IRQ_HANDLED = 0;
    GRETH1_IRQ_HANDLED = 0;

    rumboot_irq_set_handler( tbl, ETH0_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_eth, &in[0] );
    rumboot_irq_set_handler( tbl, ETH1_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_eth, &in[1] );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( ETH0_INT );
    rumboot_irq_enable( ETH1_INT );
    rumboot_irq_sei();

    return tbl;
}

void delete_greth01_irq_handlers(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

/*
 * Test data and descriptors
 */
uint32_t edcl_seq_number = 0;
uint8_t* edcl_test_data_im1_wr;


greth_descr_t*  tx_descriptor_data_;
greth_descr_t*  rx_descriptor_data_;
uint8_t* test_data_src_;
uint8_t* test_data_dst_;

/*
 * Registers access checks
 */
struct regpoker_checker greth_check_array[] = {
    { "CTRL              ",  REGPOKER_READ32,  CTRL              , 0x9A000090, 0xFE007CFF },
    { "MDIO_CTRL         ",  REGPOKER_READ32,  MDIO_CTRL         , 0x01E10140, 0xFFFFFFCF },
    { "EDCL_IP           ",  REGPOKER_READ32,  EDCL_IP           , EDCLIP0, ~0 },
    { "EDCL_MAC_MSB      ",  REGPOKER_READ32,  EDCL_MAC_MSB      , EDCLMAC_MSB, 0xffff },
    { "EDCL_MAC_LSB      ",  REGPOKER_READ32,  EDCL_MAC_LSB      , EDCLMAC_LSB, ~0 },
    { "EDCL_IP           ",  REGPOKER_WRITE32, EDCL_IP           , 0, ~0 },
    { "EDCL_MAC_MSB      ",  REGPOKER_WRITE32, EDCL_MAC_MSB      , 0, 0xffff },
    { "EDCL_MAC_LSB      ",  REGPOKER_WRITE32, EDCL_MAC_LSB      , 0, ~0 }
};

void regs_check(uint32_t base_addr)
{
    if (base_addr==GRETH_0_BASE)
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers !!!\n", 1, base_addr);
        greth_check_array[2].expected = EDCLIP0;
        greth_check_array[4].expected |= EDCLADDRL0;
    }
    else
    if (base_addr==GRETH_1_BASE)
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n", 1, base_addr);
        greth_check_array[2].expected = EDCLIP1;
        greth_check_array[4].expected |= EDCLADDRL1;
    }
    else
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n", 0, base_addr);
    }
    TEST_ASSERT(rumboot_regpoker_check_array(greth_check_array, base_addr)==0, "Failed to check GRETH registers\n");
}
/*
 * MDIO checks
 */
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

//#define GRETH_SRC_DATA_SEL  true
//#define GRETH_DST_DATA_SEL  false
//
//uint8_t * get_src_dst_data_ptr(bool src_dst, uint32_t bank_num)
//{
//    uint8_t* data_ptr = (uint8_t *) 0;
//    switch (bank_num)
//    {
//        case 1:
//            data_ptr = (src_dst==GRETH_SRC_DATA_SEL) ? test_data_src : test_data_dst;
//            break;
//        case 2:
//            data_ptr = (src_dst==GRETH_SRC_DATA_SEL) ? test_data_im2_src : test_data_im2_dst;
//            break;
//        case 3:
//            data_ptr = (src_dst==GRETH_SRC_DATA_SEL) ? test_data_em2_src : test_data_em2_dst;
//            break;
//        default:
//            TEST_ASSERT(0, "Wrong bank index");
//            break;
//    }
//    return data_ptr;
//}

void prepare_test_data()
{
    uint32_t i=0;

#ifdef INIT_EMI
    {
        rumboot_putstring("Init EMI");
        emi_init(DCR_EM2_EMI_BASE);
    }
#endif

    rumboot_putstring("Preparing data");
    tx_descriptor_data_ = (greth_descr_t*) rumboot_malloc_from_heap_aligned(1, 3*sizeof(greth_descr_t), 1024);
    rx_descriptor_data_ = (greth_descr_t*) rumboot_malloc_from_heap_aligned(1, 3*sizeof(greth_descr_t), 1024);

    rumboot_printf("\rumboot_virt_to_dma(ntx_descriptor_data_) = 0x%X\n", rumboot_virt_to_dma(tx_descriptor_data_));
    rumboot_putstring("Allocate mem for src");
    test_data_src_ = (uint8_t* )rumboot_malloc_from_named_heap_misaligned(SRC_HEAP_NAME, GRETH_TEST_DATA_LEN_BYTES, 16, GRETH_TEST_DATA_MISALIGN_IM1);
    
    rumboot_putstring("Allocate mem for dst");
    test_data_dst_ = (uint8_t* )rumboot_malloc_from_named_heap_misaligned(DST_HEAP_NAME, GRETH_TEST_DATA_LEN_BYTES, 16, GRETH_TEST_DATA_MISALIGN_IM1);

    rumboot_putstring("Fill im1 src array");
    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x00FF0000) >> 16);
    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0xFF000000) >> 24);
    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x000000FF) >>  0);
    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x0000FF00) >>  8);

    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x000000FF) >>  0);
    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_msb & 0x0000FF00) >>  8);
    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x000000FF) >>  0);
    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x0000FF00) >>  8);

    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x000000FF) >>  0);
    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x0000FF00) >>  8);
    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0x00FF0000) >> 16);
    test_data_src_[i++] = (uint8_t)((tst_greth_mac.mac_lsb & 0xFF000000) >> 24);

    test_data_src_[i++] = (uint8_t)(((GRETH_TEST_DATA_LEN_BYTES - 14) & 0x00FF) >> 0);
    test_data_src_[i++] = (uint8_t)(((GRETH_TEST_DATA_LEN_BYTES - 14) & 0xFF00) >> 8);
    test_data_src_[i++] = (uint8_t)(0);
    test_data_src_[i++] = (uint8_t)(0);

    while(i<GRETH_TEST_DATA_LEN_BYTES)
    {
        test_data_src_[i] = i;
        i++;
    }

    rumboot_putstring("Preparing data finished");
}

void prepare_test_edcl_data(edcl_test_data_struct_t* edcl_cfg, uint32_t* test_edcl_packet)
{
    uint32_t     i = 0;
    uint32_t     j = 0;
    uint32_t     Version          = 4;
    uint32_t     IHL              = 5;
    uint32_t     DSCP             = 0;
    uint32_t     ECN              = 0;
    uint32_t     Total_Length     = 0;
    uint32_t     Id               = 0;
    uint32_t     Time_To_Live     = 1;
    uint32_t     Protocol         = 0x11;
    uint32_t     Header_Checksum  = 0;
    uint32_t     Source_Port      = 0;
    uint32_t     Dest_Port        = 0;
    uint32_t     Data_Length      = 64 + (edcl_cfg->wrrd==EDCL_WR) ? edcl_cfg->len : 0; //header_length + offset_length + control_word_length + address_length + data_length = 8 + 2 + 4 + 4 + len = 18 + len
    uint32_t     Checksum         = 0;
    uint32_t     len;

    test_edcl_packet[i++] = (edcl_cfg->dst_mac.mac_msb << 16) | ((edcl_cfg->dst_mac.mac_lsb & 0xFFFF0000) >> 16);  //DST MAC [48:16]
    test_edcl_packet[i++] = ((edcl_cfg->dst_mac.mac_lsb & 0xFFFF) << 16) | edcl_cfg->src_mac.mac_msb;              //DST MAC [15:0] SRC MAC [48:32]
    test_edcl_packet[i++] = edcl_cfg->src_mac.mac_lsb;                                                         //SRC MAC [31:0]
    test_edcl_packet[i++] = (0x0800 << 16) | Version << 12 |  IHL << 8 | DSCP << 2 | ECN;
    test_edcl_packet[i++] = (Total_Length << 16) | Id;
    test_edcl_packet[i++] = (Time_To_Live << 8 | Protocol);
    test_edcl_packet[i++] = (Header_Checksum << 16) | (edcl_cfg->src_ip & 0xFFFF0000) >> 16;
    test_edcl_packet[i++] = (edcl_cfg->src_ip & 0xFFFF) | (edcl_cfg->dst_ip & 0xFFFF0000);
    test_edcl_packet[i++] = (edcl_cfg->dst_ip & 0xFFFF) << 16;
    test_edcl_packet[i++] = Source_Port << 16 | Dest_Port                                        ;
    test_edcl_packet[i++] = Data_Length << 16 | Checksum                                         ;
    test_edcl_packet[i++] = edcl_cfg->edcl_seq_number << 18 | edcl_cfg->wrrd << 17 | edcl_cfg->len << 7; //CTRL_WORD
    test_edcl_packet[i++] = edcl_cfg->addr;
    if (edcl_cfg->wrrd==EDCL_WR)
    {
        len = edcl_cfg->len;
        while (len > 3)
        {
            test_edcl_packet[i++] = (((uint32_t)(*(edcl_cfg->data + j + 3) <<  0) & 0x000000FF) |
                                     ((uint32_t)(*(edcl_cfg->data + j + 2) <<  8) & 0x0000FF00) |
                                     ((uint32_t)(*(edcl_cfg->data + j + 1) << 16) & 0x00FF0000) |
                                     ((uint32_t)(*(edcl_cfg->data + j + 0) << 24) & 0xFF000000));
            j+=4;
            len-=4;
        }
        switch (len)
        {
            case 1:
                test_edcl_packet[i++] = ((uint32_t)(*(edcl_cfg->data + j) << 24) & 0xFF000000);
                break;
            case 2:
                test_edcl_packet[i++] = (((uint32_t)(*(edcl_cfg->data + j + 1) << 16) & 0x00FF0000) |
                                         ((uint32_t)(*(edcl_cfg->data + j + 0) << 24) & 0xFF000000));
                break;
            case 3:
                test_edcl_packet[i++] = (((uint32_t)(*(edcl_cfg->data + j + 2) <<  8) & 0x0000FF00) |
                                         ((uint32_t)(*(edcl_cfg->data + j + 1) << 16) & 0x00FF0000) |
                                         ((uint32_t)(*(edcl_cfg->data + j + 0) << 24) & 0xFF000000));
                break;
            default: break;
        }
    }
    test_edcl_packet[i++] = 0xDEADBEAF;
    test_edcl_packet[i++] = 0xDEADBEAF;
    test_edcl_packet[i++] = 0xDEADBEAF;
    test_edcl_packet[i++] = 0xDEADBEAF;
    test_edcl_packet[i++] = 0xDEADBEAF;
    test_edcl_packet[i++] = 0xDEADBEAF;
    test_edcl_packet[i++] = 0xDEADBEAF;
}

/*
 * Checks transfers which uses different memory blocks
 */
void check_transfer_via_external_loopback(uint32_t base_addr_src_eth,  uint8_t * test_data_src, uint8_t * test_data_dst)
{
    uint32_t base_addr_dst_eth;
    uint32_t volatile* eth_handled_flag_ptr;

    test_event(EVENT_CHECK_RUN_HPROT_MONITOR);//checking switch u_nic400_oi10_axi32.hprot_eth_1(0)_s from 0x3 to 0xF (by request from JIRA-78)
    dcr_write(DCR_SCTL_BASE + SCTL_IFSYS_ETH_HPROT, 0x3F3F3F3F);

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
    test_event(EVENT_CHECK_STOP_HPROT_MONITOR);
}

void check_edcl_wr_via_external_loopback(uint32_t base_addr_dst_eth, uint32_t wr_mem_addr, void* data, uint32_t len)
{
    uint32_t * edcl_packet = (uint32_t* )rumboot_malloc_from_heap(1, 64 + len );
    edcl_test_data_struct_t edcl_cfg;
    uint32_t base_addr_src_eth;
    uint32_t edcl_packet_len;
    uint32_t volatile* eth_handled_flag_ptr;
    uint32_t* test_data_resp = (uint32_t* )rumboot_malloc_from_heap(1, GRETH_TEST_DATA_LEN_BYTES);

    TEST_ASSERT((base_addr_dst_eth==GRETH_1_BASE)||(base_addr_dst_eth==GRETH_0_BASE), "Wrong GRETH base address\n");
    if (base_addr_dst_eth==GRETH_1_BASE)
    {
        edcl_cfg.src_ip   = EDCLIP1_TEST;
        edcl_cfg.src_mac  = tst_greth_edcl_mac1;
        edcl_cfg.dst_ip   = EDCLIP1_TEST;
        edcl_cfg.dst_mac  = tst_greth_edcl_mac1;
        GRETH0_IRQ_HANDLED = 0;
        base_addr_src_eth = GRETH_0_BASE;
        eth_handled_flag_ptr = &GRETH0_IRQ_HANDLED;
    }
    else
    {
        edcl_cfg.src_ip   = EDCLIP0_TEST;
        edcl_cfg.src_mac  = tst_greth_edcl_mac0;
        edcl_cfg.dst_ip   = EDCLIP0_TEST;
        edcl_cfg.dst_mac  = tst_greth_edcl_mac0;
        GRETH1_IRQ_HANDLED = 0;
        base_addr_src_eth = GRETH_1_BASE;
        eth_handled_flag_ptr = &GRETH1_IRQ_HANDLED;
    }

    //check edcl write
    edcl_cfg.edcl_seq_number = edcl_seq_number++;
    edcl_cfg.wrrd = EDCL_WR;
    edcl_cfg.addr = rumboot_virt_to_dma((uint32_t*) wr_mem_addr);
    edcl_cfg.data = (uint8_t*)data;
    edcl_cfg.len  = len;
    edcl_packet_len = (len + 64);
    prepare_test_edcl_data(&edcl_cfg, edcl_packet);
    rumboot_printf("\nChecking EDCL write transfer %d bytes to 0x%X\n", len, wr_mem_addr);

    greth_configure_for_receive(  base_addr_src_eth, (uint8_t*)rumboot_virt_to_dma(test_data_resp), edcl_packet_len, rx_descriptor_data_, &tst_greth_mac);
    greth_configure_for_transmit( base_addr_src_eth, (uint8_t*)rumboot_virt_to_dma(edcl_packet), edcl_packet_len, tx_descriptor_data_, &tst_greth_mac);

    greth_start_edcl_transfer( base_addr_src_eth );

    TEST_ASSERT(greth_wait_receive_irq(base_addr_src_eth, eth_handled_flag_ptr), "Receiving EDCL response failed\n");
    TEST_ASSERT(memcmp(data, (uint8_t*) wr_mem_addr, len)==0, "Data error at EDCL WR operation!");
}

void check_edcl_rd_via_external_loopback(uint32_t base_addr_dst_eth, uint32_t rd_mem_addr, uint32_t len)
{
    uint32_t * edcl_packet = (uint32_t* )rumboot_malloc_from_heap(1, 64 + len );
    edcl_test_data_struct_t edcl_cfg;
    uint32_t base_addr_src_eth;
    uint32_t edcl_packet_len;
    uint32_t* test_data_resp = (uint32_t* )rumboot_malloc_from_heap(1, GRETH_TEST_DATA_LEN_BYTES);
    uint32_t volatile* eth_handled_flag_ptr;

    TEST_ASSERT((base_addr_dst_eth==GRETH_1_BASE)||(base_addr_dst_eth==GRETH_0_BASE), "Wrong GRETH base address\n");
    if (base_addr_dst_eth==GRETH_1_BASE)
    {
        edcl_cfg.src_ip   = EDCLIP1_TEST;
        edcl_cfg.src_mac  = tst_greth_edcl_mac1;
        edcl_cfg.dst_ip   = EDCLIP1_TEST;
        edcl_cfg.dst_mac  = tst_greth_edcl_mac1;
        GRETH1_IRQ_HANDLED = 0;
        base_addr_src_eth = GRETH_0_BASE;
    }
    else
    {
        edcl_cfg.src_ip   = EDCLIP0_TEST;
        edcl_cfg.src_mac  = tst_greth_edcl_mac0;
        edcl_cfg.dst_ip   = EDCLIP0_TEST;
        edcl_cfg.dst_mac  = tst_greth_edcl_mac0;
        GRETH0_IRQ_HANDLED = 0;
        base_addr_src_eth = GRETH_1_BASE;
    }

    //check edcl read
    edcl_cfg.edcl_seq_number = edcl_seq_number++;
    edcl_cfg.wrrd = EDCL_RD;
    edcl_cfg.addr = rumboot_virt_to_dma((uint32_t*) rd_mem_addr);
    edcl_cfg.len  = len;
    edcl_packet_len = (len + 64);
    prepare_test_edcl_data(&edcl_cfg, edcl_packet);
    rumboot_printf("\nChecking EDCL read transfer %d bytes from 0x%X\n", len, rd_mem_addr);

    greth_configure_for_receive(  base_addr_src_eth, (uint8_t*)rumboot_virt_to_dma(test_data_resp), edcl_packet_len, rx_descriptor_data_, &tst_greth_mac);
    greth_configure_for_transmit( base_addr_src_eth, (uint8_t*)rumboot_virt_to_dma(edcl_packet), edcl_packet_len, tx_descriptor_data_, &tst_greth_mac);

    greth_start_edcl_transfer( base_addr_src_eth );

    eth_handled_flag_ptr = (base_addr_src_eth==GRETH_0_BASE) ? &GRETH0_IRQ_HANDLED : &GRETH1_IRQ_HANDLED;
    TEST_ASSERT(greth_wait_receive_irq(base_addr_src_eth, eth_handled_flag_ptr), "Receiving EDCL response failed\n");
    TEST_ASSERT(memcmp((uint8_t*) rd_mem_addr, (uint8_t*)(&test_data_resp[13]), len)==0, "Data compare error at EDCL RD operation!\n");
}

void check_rx_er(uint32_t base_addr)
{
    uint32_t base_addr_dst_eth;
    uint32_t base_addr_src_eth;
    uint32_t volatile* eth_handled_flag_ptr;
    uint32_t event_code;

    TEST_ASSERT((base_addr==GRETH_1_BASE)||(base_addr==GRETH_0_BASE), "Wrong GRETH base address\n");
    if (base_addr==GRETH_1_BASE)
    {
        base_addr_dst_eth = GRETH_1_BASE;
        base_addr_src_eth = GRETH_0_BASE;
        GRETH1_IRQ_HANDLED = 0;
        eth_handled_flag_ptr = &GRETH1_IRQ_HANDLED;
        event_code = EVENT_CHECK_GRETH1_RX_ER;
    }
    else
    {
        base_addr_dst_eth = GRETH_0_BASE;
        base_addr_src_eth = GRETH_1_BASE;
        GRETH0_IRQ_HANDLED = 0;
        eth_handled_flag_ptr = &GRETH0_IRQ_HANDLED;
        event_code = EVENT_CHECK_GRETH0_RX_ER;
    }

    prepare_test_data();

    greth_configure_for_receive( base_addr_dst_eth, (uint8_t*)rumboot_virt_to_dma(test_data_dst_), GRETH_TEST_DATA_LEN_BYTES, rx_descriptor_data_, &tst_greth_mac);
    greth_configure_for_transmit( base_addr_src_eth, (uint8_t*)rumboot_virt_to_dma(test_data_src_), GRETH_TEST_DATA_LEN_BYTES, tx_descriptor_data_, &tst_greth_mac);

    test_event(event_code);
    greth_start_receive( base_addr_dst_eth, true );
    greth_start_transmit( base_addr_src_eth );

    TEST_ASSERT(greth_wait_receive_err_irq(base_addr_dst_eth, eth_handled_flag_ptr), "Receiving error is failed\n");

    rumboot_free(tx_descriptor_data_);
    rumboot_free(rx_descriptor_data_);
    rumboot_free(test_data_dst_);
    rumboot_free(test_data_src_);
}

void check_rx_col(uint32_t base_src_addr)
{
    uint32_t base_addr_dst_eth;
    uint32_t volatile* eth_handled_flag_ptr;
    uint32_t event_code;

    if (base_src_addr==GRETH_0_BASE)
    {
        base_addr_dst_eth = GRETH_1_BASE;
        GRETH1_IRQ_HANDLED = 0;
        eth_handled_flag_ptr = &GRETH1_IRQ_HANDLED;
        event_code = EVENT_CHECK_GRETH0_RX_COL;
    }
    else
    {
        base_addr_dst_eth = GRETH_0_BASE;
        GRETH0_IRQ_HANDLED = 0;
        eth_handled_flag_ptr = &GRETH0_IRQ_HANDLED;
        event_code = EVENT_CHECK_GRETH1_RX_COL;
    }

    prepare_test_data();

    greth_configure_for_transmit( base_src_addr, (uint8_t*)rumboot_virt_to_dma(test_data_src_), GRETH_TEST_DATA_LEN_BYTES, tx_descriptor_data_, &tst_greth_mac);
    greth_configure_for_receive( base_addr_dst_eth, (uint8_t*)rumboot_virt_to_dma(test_data_dst_), GRETH_TEST_DATA_LEN_BYTES, rx_descriptor_data_, &tst_greth_mac);

    test_event(event_code);

    greth_start_receive(base_addr_dst_eth, true);
    greth_start_transmit(base_src_addr);

    TEST_ASSERT(greth_wait_receive_irq(base_addr_dst_eth, eth_handled_flag_ptr), "Receiving error is failed\n");

    rumboot_free(tx_descriptor_data_);
    rumboot_free(rx_descriptor_data_);
    rumboot_free(test_data_dst_);
    rumboot_free(test_data_src_);
}


/*
 * test_oi10_greth
 */
int main(void)
{
    rumboot_printf("Start\n");
#ifndef CHECK_REG_AND_MDIO
    struct rumboot_irq_entry *tbl;
#endif
#ifdef CHECK_REG_AND_MDIO
    rumboot_printf("Start test_oi10_greth. MDIO and registers access checks for GRETH%d (0x%X)\n", GET_GRETH_IDX(GRETH_BASE), GRETH_BASE);
    regs_check(GRETH_BASE);
    mdio_check(GRETH_BASE);
#elif defined(EDCL_TEST_BANK)
    EDCL_TEST_ADDR = (uintptr_t ) rumboot_malloc_from_named_heap_aligned(__stringify(EDCL_TEST_BANK), 8192, 8);
    rumboot_printf("Start test_oi10_greth. EDCL checks \n");
    tbl = create_greth01_irq_handlers();
    edcl_seq_number = 0;
    if ((EDCL_TEST_ADDR>=EM2_BANK0_BASE) && ((EDCL_TEST_ADDR<(EM2_BANK5_BASE+NOR_SIZE))))
        emi_init(DCR_EM2_EMI_BASE);

    edcl_test_data_im1_wr = (uint8_t*) rumboot_malloc_from_heap(1, GRETH_EDCL_DATA_LEN_BYTES);

    for (int i=0; i<GRETH_EDCL_DATA_LEN_BYTES; i++)

    rx_descriptor_data_ = (greth_descr_t*) rumboot_malloc_from_heap_aligned(1, 3*sizeof(greth_descr_t), 1024);
    tx_descriptor_data_ = (greth_descr_t*) rumboot_malloc_from_heap_aligned(1, 3*sizeof(greth_descr_t), 1024);

    dcr_write(DCR_SCTL_BASE + SCTL_IFSYS_ETH_CTRL, (uint32_t) (EDCLADDRL0_TEST | (EDCLADDRL1_TEST << 16)) );//setting non-default edcladdrl values
    check_edcl_wr_via_external_loopback(GRETH_BASE, EDCL_TEST_ADDR, edcl_test_data_im1_wr, GRETH_EDCL_DATA_LEN_BYTES);
    check_edcl_rd_via_external_loopback(GRETH_BASE, EDCL_TEST_ADDR, GRETH_EDCL_DATA_LEN_BYTES);

    delete_greth01_irq_handlers(tbl);
#elif CHECK_RX_ER
    rumboot_printf("Start test_oi10_greth. Checks connection RX_ER signal for GRETH%d (0x%X)\n", GET_GRETH_IDX(GRETH_BASE), GRETH_BASE);
    test_event_send_test_id("test_oi10_greth");
    tbl = create_greth01_irq_handlers();
    check_rx_er(GRETH_BASE);
    delete_greth01_irq_handlers(tbl);
#elif CHECK_RX_COL
    rumboot_printf("Start test_oi10_greth. Checks connection RX_COL signal for GRETH%d (0x%X)\n", GET_GRETH_IDX(GRETH_BASE), GRETH_BASE);
    test_event_send_test_id("test_oi10_greth");
    tbl = create_greth01_irq_handlers();
    check_rx_col(GRETH_BASE);
    delete_greth01_irq_handlers(tbl);
#else
    rumboot_printf("Start test_oi10_greth. Transmit/receive checks\n");
    test_event_send_test_id("test_oi10_greth");
    tbl = create_greth01_irq_handlers();
    prepare_test_data();
    check_transfer_via_external_loopback(GRETH_BASE, (uint8_t*)rumboot_virt_to_dma(test_data_src_), (uint8_t*)rumboot_virt_to_dma(test_data_dst_));
    delete_greth01_irq_handlers(tbl);
#endif

    return 0;
}
