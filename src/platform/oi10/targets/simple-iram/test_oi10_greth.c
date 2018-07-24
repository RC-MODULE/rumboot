#include <stdio.h>
#include <stdint.h>

#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>
#include <platform/regs/fields/mpic128.h>

#include <platform/devices/greth.h>

#define IM1_BASE_MIRROR     0xC0000000

#define GRETH_TEST_DATA_LEN 64
#define GRETH_EDCL_DATA_LEN 15

#define TST_MAC_MSB     0xFFFF
#define TST_MAC_LSB     0xFFFFFFFF
greth_mac_t tst_greth_mac      = {TST_MAC_MSB, TST_MAC_LSB};
greth_mac_t tst_greth_edcl_mac = {0x0017, 0x66000501};

/*
 * Functions and variables for interrupt handling
 */
uint32_t GRETH0_IRQ_HANDLED = 0;
uint32_t GRETH1_IRQ_HANDLED = 0;

static void handler_eth0( int irq, void *arg )
{
    uint32_t cur_status;
    uint32_t mask;
    rumboot_printf( "ETH0 IRQ arrived  \n" );
    mask = ((1 << GRETH_STATUS_IA) |
            (1 << GRETH_STATUS_TS) |
            (1 << GRETH_STATUS_TA) |
            (1 << GRETH_STATUS_RA) |
            (1 << GRETH_STATUS_TI) |
            (1 << GRETH_STATUS_TE) |
            (1 << GRETH_STATUS_RE));

    cur_status = greth_get_status(GRETH_0_BASE);
    if (cur_status & (1 << GRETH_STATUS_RI))
    {
        GRETH0_IRQ_HANDLED = 1;
        greth_clear_status_bits(GRETH_0_BASE, (1 << GRETH_STATUS_RI) );
    }
    else
    {
        greth_clear_status_bits(GRETH_0_BASE, mask);
    }
}

static void handler_eth1( int irq, void *arg )
{
    uint32_t cur_status;
    uint32_t mask;
    rumboot_printf( "ETH1 IRQ arrived  \n" );
    mask = ((1 << GRETH_STATUS_IA) |
            (1 << GRETH_STATUS_TS) |
            (1 << GRETH_STATUS_TA) |
            (1 << GRETH_STATUS_RA) |
            (1 << GRETH_STATUS_TI) |
            (1 << GRETH_STATUS_TE) |
            (1 << GRETH_STATUS_RE));

    cur_status = greth_get_status(GRETH_1_BASE);
    if (cur_status & (1 << GRETH_STATUS_RI))
    {
        GRETH1_IRQ_HANDLED = 1;
        greth_clear_status_bits(GRETH_1_BASE, (1 << GRETH_STATUS_RI) );
    }
    else
    {
        greth_clear_status_bits(GRETH_1_BASE, mask);
    }
}

bool greth_wait_receive_irq(uint32_t base_addr, uint32_t *eth_irq_handled_flag)
{
    uint32_t t = 0;

    rumboot_printf("Waiting receive..\n");
    while(!(*eth_irq_handled_flag) && (t++ < GRETH_TIMEOUT*100)){}

    if ((t==GRETH_TIMEOUT) || !(*eth_irq_handled_flag))
    {
        rumboot_printf("Waiting receive is timed out\n");
        return false;
    }
    else
    {
        rumboot_printf("Waiting receive is OK\n");
        return true;
    }
}

struct rumboot_irq_entry * create_greth01_irq_handlers()
{
    rumboot_printf( "Enable GRETH irqs\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, ETH0_INT, (int_sense_edge << MPIC128_VP_S_i) | (int_pol_pos << MPIC128_VP_POL_i), handler_eth0, (void *)0 );
    rumboot_irq_set_handler( tbl, ETH1_INT, (int_sense_edge << MPIC128_VP_S_i) | (int_pol_pos << MPIC128_VP_POL_i), handler_eth1, (void *)0 );

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
typedef struct edcl_test_data_struct
{
    uint32_t  dst_ip;
    uint32_t  edcl_seq_number;
    uint32_t  addr;
    uint32_t* data;
    uint32_t  len;
    bool      wrrd;
}edcl_test_data_struct_t;

uint32_t edcl_seq_number = 0;
uint32_t test_edcl_data = 0xBABADEDA;
edcl_test_data_struct_t edcl_cfg = {
                                        0xC0A80035,
                                        0,
                                        IM1_BASE_MIRROR + 0x10000,
                                        &test_edcl_data,
                                        0x4,
                                        true,
                                    };

greth_descr_t  tx_descriptor_data_[GRETH_MAX_DESCRIPTORS_COUNT] __attribute__((aligned(1024)));
greth_descr_t  rx_descriptor_data_[GRETH_MAX_DESCRIPTORS_COUNT] __attribute__((aligned(1024)));

uint32_t test_data_im0_src[GRETH_TEST_DATA_LEN]  __attribute__((section(".data")));
uint32_t test_data_im0_dst[GRETH_TEST_DATA_LEN]  __attribute__((section(".data")));
uint32_t test_edcl_packet_im0[GRETH_EDCL_DATA_LEN] __attribute__((section(".data")));
uint32_t test_edcl_rcv_packet_im0[GRETH_EDCL_DATA_LEN] __attribute__((section(".data")));


uint32_t* test_data_im1_src  = (uint32_t *)( IM1_BASE_MIRROR );
uint32_t* test_data_im1_dst  = (uint32_t *)( IM1_BASE_MIRROR +   sizeof(test_data_im0_src) );
uint32_t* test_edcl_packet_im1 = (uint32_t *)( IM1_BASE_MIRROR + 2*sizeof(test_data_im0_src) );
uint32_t* test_edcl_rcv_packet_im1 = (uint32_t *)( IM1_BASE_MIRROR + 2*sizeof(test_data_im0_src) + sizeof(test_edcl_packet_im0));
uint32_t* test_edcl_data_im1 = (uint32_t *)( IM1_BASE_MIRROR + 2*sizeof(test_data_im0_src) + 2*sizeof(test_edcl_packet_im0));

/*
 * MDIO checks
 */
bool mdio_check(uint32_t base_addr)
{
    rumboot_printf("MDIO check for ETH%d(0x%x)\n", base_addr==GRETH_0_BASE ? 0 : 1, base_addr);
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_CTRL      )==ETH_PHY_CTRL_DEFAULT      , "Error at mdio reading ETH_PHY_CTRL       register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_STATUS    )==ETH_PHY_STATUS_DEFAULT    , "Error at mdio reading ETH_PHY_STATUS     register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_ID0       )==ETH_PHY_ID0_DEFAULT       , "Error at mdio reading ETH_PHY_ID0        register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_ID1       )==ETH_PHY_ID1_DEFAULT       , "Error at mdio reading ETH_PHY_ID1        register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_ANEGADV   )==ETH_PHY_ANEGADV_DEFAULT   , "Error at mdio reading ETH_PHY_ANEGADV    register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_ANEGLP    )==ETH_PHY_ANEGLP_DEFAULT    , "Error at mdio reading ETH_PHY_ANEGLP     register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_ANEGXP    )==ETH_PHY_ANEGXP_DEFAULT    , "Error at mdio reading ETH_PHY_ANEGXP     register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_ANEGNPTX  )==ETH_PHY_ANEGNPTX_DEFAULT  , "Error at mdio reading ETH_PHY_ANEGNPTX   register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_ANEGNPLP  )==ETH_PHY_ANEGNPLP_DEFAULT  , "Error at mdio reading ETH_PHY_ANEGNPLP   register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_MSTSLVCTRL)==ETH_PHY_MSTSLVCTRL_DEFAULT, "Error at mdio reading ETH_PHY_MSTSLVCTRL register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_MSTSLVSTAT)==ETH_PHY_MSTSLVSTAT_DEFAULT, "Error at mdio reading ETH_PHY_MSTSLVSTAT register");
    TEST_ASSERT(mdio_read(base_addr, 0, ETH_PHY_EXTSTATUS )==ETH_PHY_EXTSTATUS_DEFAULT , "Error at mdio reading ETH_PHY_EXTSTATUS  register");
    return 0;
}

/*
 * Memory functions
 */
bool mem_cmp(void volatile * const src, void volatile * const dst, uint32_t len)
{
    uint32_t * src_addr;
    uint32_t * dst_addr;
    bool data_ok = true;
    src_addr = (uint32_t *)src; dst_addr = (uint32_t *)dst;
    for (uint32_t i=0; i<len; i++)
    {
        if (*src_addr!=*dst_addr)
        {
            rumboot_printf("Data error:\nsrc[0x%X]: 0x%x\ndst[0x%X]: 0x%x\n", src_addr, *src_addr, dst_addr, *dst_addr);
            data_ok = false;
        }
        src_addr++; dst_addr++;
    }
    return data_ok;
}

void mem_clr(void volatile * const ptr, uint32_t len)
{
    uint32_t * addr = (uint32_t *) ptr;
    for (uint32_t i=0; i<len; i++)
    {
        *addr = 0xFFFFFFFF; addr++;
    }
}

void prepare_test_data()
{
    uint32_t i=0;
    bool toggle = true;
    test_data_im0_src[i++] = (tst_greth_mac.mac_msb << 16) | ((tst_greth_mac.mac_lsb & 0xFFFF0000) >> 16);//DST MAC [48:16]
    test_data_im0_src[i++] = ((tst_greth_mac.mac_lsb & 0xFFFF) << 16) | tst_greth_mac.mac_msb;//DST MAC [15:0] SRC MAC [48:32]
    test_data_im0_src[i++] = tst_greth_mac.mac_lsb;//SRC MAC [31:0]
    test_data_im0_src[i++] = ((GRETH_TEST_DATA_LEN * sizeof(uint32_t) - 14) << 16);//Len and 0x0000 data
    while(i<GRETH_TEST_DATA_LEN)
    {
        test_data_im0_src[i] = toggle ? (1 << i) : ~(1 << i);
        i++; toggle = !toggle;
    }
    memcpy(test_data_im1_src, test_data_im0_src, sizeof(test_data_im0_src));
}

void prepare_test_edcl_data(edcl_test_data_struct_t* edcl_cfg)
{
    uint32_t     i = 0;
    uint32_t     j = 0;
    uint32_t     Version          = 4;
    uint32_t     IHL              = 5;
    uint32_t     DSCP             = 0;
    uint32_t     ECN              = 0;
    uint32_t     Total_Length     = 0;
    uint32_t     Id               = 0;
//    uint32_t     Flags            = 0;
//    uint32_t     Fragment_Offset  = 0;
    uint32_t     Time_To_Live     = 1;
    uint32_t     Protocol         = 0x11;
    uint32_t     Header_Checksum  = 0;
    uint32_t     Source_IP        = 0;
    // UDP Header
    uint32_t     Source_Port      = 0;
    uint32_t     Dest_Port        = 0;
    uint32_t     Data_Length      = 18 + edcl_cfg->len; //header_length + offset_length + control_word_length + address_length + data_length = 8 + 2 + 4 + 4 + len = 18 + len
    uint32_t     Checksum         = 0;

    test_edcl_packet_im0[i++] = (tst_greth_edcl_mac.mac_msb << 16) | ((tst_greth_edcl_mac.mac_lsb & 0xFFFF0000) >> 16);  //DST MAC [48:16]
    test_edcl_packet_im0[i++] = ((tst_greth_edcl_mac.mac_lsb & 0xFFFF) << 16) | tst_greth_edcl_mac.mac_msb;              //DST MAC [15:0] SRC MAC [48:32]
    test_edcl_packet_im0[i++] = tst_greth_edcl_mac.mac_lsb;                                                         //SRC MAC [31:0]
    test_edcl_packet_im0[i++] = (0x0800 << 16) | Version << 12 |  IHL << 8 | DSCP << 2 | ECN;
    test_edcl_packet_im0[i++] = (Total_Length << 16) | Id;
    test_edcl_packet_im0[i++] = (Time_To_Live << 8 | Protocol);
    test_edcl_packet_im0[i++] = (Header_Checksum << 16) | (Source_IP & 0xFFFF0000) >> 16;
    test_edcl_packet_im0[i++] = (Source_IP & 0xFFFF) | (edcl_cfg->dst_ip & 0xFFFF0000);
    test_edcl_packet_im0[i++] = (edcl_cfg->dst_ip & 0xFFFF) << 16;
    test_edcl_packet_im0[i++] = Source_Port << 16 | Dest_Port                                        ;
    test_edcl_packet_im0[i++] = Data_Length << 16 | Checksum                                         ;
    test_edcl_packet_im0[i++] = edcl_cfg->edcl_seq_number << 18 | edcl_cfg->wrrd << 17 | 0x4 << 7; //CTRL_WORD
    test_edcl_packet_im0[i++] = edcl_cfg->addr;
    if (edcl_cfg->wrrd==EDCL_WR)
    {
        while (j < (edcl_cfg->len / sizeof(uint32_t)))
        {
            //change byte order
            test_edcl_packet_im0[i++] = *(edcl_cfg->data + j);
            j++;
        }
    }
    test_edcl_packet_im0[i++] = 0xDEADBEAF;
    memcpy(test_edcl_packet_im1, test_edcl_packet_im0, sizeof(test_edcl_packet_im0));
    memcpy(test_edcl_data_im1, &test_edcl_data, sizeof(test_edcl_data));
}


/*
 * Checks transfers which uses different memory blocks
 */
int check_transfer_via_external_loopback(uint32_t base_addr_src_eth,  uint32_t * test_data_src, uint32_t * test_data_dst)
{
    uint32_t base_addr_dst_eth;
    base_addr_dst_eth = base_addr_src_eth==GRETH_0_BASE ? GRETH_1_BASE : GRETH_0_BASE;

/*    uint32_t * eth_hadled_flag_ptr;

    if (base_addr_dst_eth==GRETH_0_BASE)
    {
        GRETH0_IRQ_HANDLED = 0;
        eth_hadled_flag_ptr = &GRETH0_IRQ_HANDLED;
    }
    else
    {
        GRETH1_IRQ_HANDLED = 0;
        eth_hadled_flag_ptr = &GRETH1_IRQ_HANDLED;
    }
*/
    rumboot_printf("\nChecking transfer from 0x%X to 0x%x\n", (uint32_t)test_data_src, (uint32_t) test_data_dst);

    greth_configure_for_receive( base_addr_dst_eth, test_data_dst, sizeof(test_data_im0_src), rx_descriptor_data_, &tst_greth_mac);
    greth_configure_for_transmit( base_addr_src_eth, test_data_src, sizeof(test_data_im0_src), tx_descriptor_data_, &tst_greth_mac);

    greth_start_receive( base_addr_dst_eth );
    greth_start_transmit( base_addr_src_eth );

//    TEST_ASSERT(greth_wait_receive_irq(base_addr_dst_eth, eth_hadled_flag_ptr), "Receiving is failed");
    TEST_ASSERT(greth_wait_receive(base_addr_dst_eth), "Receiving is failed");
    TEST_ASSERT(mem_cmp(test_data_src, test_data_dst, sizeof(test_data_im0_src)/sizeof(uint32_t)), "Data compare error!");

    mem_clr(test_data_dst, sizeof(test_data_im0_src)/sizeof(uint32_t));
    return 0;
}

int check_edcl_via_external_loopback(uint32_t base_addr_src_eth, uint32_t * test_data_src, uint32_t * test_data_dst, uint32_t * test_data_resp)
{
    /*
    uint32_t * eth_hadled_flag_ptr;
    if (base_addr_dst_eth==GRETH_0_BASE)
    {
        GRETH0_IRQ_HANDLED = 0;
        eth_hadled_flag_ptr = &GRETH0_IRQ_HANDLED;
    }
    else
    {
        GRETH1_IRQ_HANDLED = 0;
        eth_hadled_flag_ptr = &GRETH1_IRQ_HANDLED;
    }
    */
    uint32_t base_addr_dst_eth;

    base_addr_dst_eth = base_addr_src_eth==GRETH_0_BASE ? GRETH_1_BASE : GRETH_0_BASE;

    //check edcl write
    edcl_cfg.edcl_seq_number = edcl_seq_number++;
    edcl_cfg.wrrd = EDCL_WR;
    edcl_cfg.addr = IM1_BASE_MIRROR + 0x1000;
    edcl_cfg.data = &test_edcl_data;
    edcl_cfg.len  = sizeof(uint32_t);
    prepare_test_edcl_data(&edcl_cfg);
    rumboot_printf("\nChecking edcl write transfer (%d) %d bytes from 0x%X to 0x%x\n", edcl_cfg.edcl_seq_number, edcl_cfg.len, (uint32_t) edcl_cfg.data, edcl_cfg.addr);

    greth_configure_for_receive( base_addr_dst_eth, test_data_dst, sizeof(test_edcl_packet_im0), rx_descriptor_data_, &tst_greth_mac);
    greth_configure_for_transmit( base_addr_src_eth, test_data_src, sizeof(test_edcl_packet_im0), tx_descriptor_data_, &tst_greth_mac);

    greth_start_receive( base_addr_dst_eth );
    greth_start_transmit( base_addr_src_eth );

//    TEST_ASSERT(greth_wait_receive_irq(base_addr_dst_eth, eth_hadled_flag_ptr), "Receiving is failed");
    TEST_ASSERT(greth_wait_receive(base_addr_dst_eth), "Receiving is failed");
    TEST_ASSERT(mem_cmp(&test_edcl_data, (uint32_t *)edcl_cfg.addr, edcl_cfg.len / sizeof(uint32_t)), "Data compare error!");
    mem_clr((uint32_t *) edcl_cfg.addr, edcl_cfg.len / sizeof(uint32_t));
    rumboot_printf("Checking edcl write is OK\n");

    //check edcl read data from cell &test_edcl_data
    edcl_cfg.edcl_seq_number = edcl_seq_number++;
    edcl_cfg.wrrd = EDCL_RD;
    edcl_cfg.addr = (uint32_t)test_edcl_data_im1;
    prepare_test_edcl_data(&edcl_cfg);
    rumboot_printf("\nChecking edcl read transfer (%d) %d bytes from 0x%X to 0x%x\n", edcl_cfg.edcl_seq_number, edcl_cfg.len, edcl_cfg.addr, (uint32_t)test_data_resp);
    greth_configure_for_receive(  base_addr_dst_eth, test_data_dst,  sizeof(test_edcl_packet_im0), rx_descriptor_data_, &tst_greth_mac);
    greth_configure_for_receive(  base_addr_src_eth, test_data_resp, sizeof(test_edcl_packet_im0), rx_descriptor_data_, &tst_greth_mac);
    greth_configure_for_transmit( base_addr_src_eth, test_data_src,  sizeof(test_edcl_packet_im0), tx_descriptor_data_, &tst_greth_mac);

    greth_start_receive( base_addr_dst_eth );
    greth_start_edcl_rd( base_addr_src_eth );

//    TEST_ASSERT(greth_wait_receive_irq(base_addr_dst_eth, eth_hadled_flag_ptr), "Receiving EDCL request is failed");
//    TEST_ASSERT(greth_wait_receive_irq(base_addr_src_eth, eth_hadled_flag_ptr), "Receiving EDCL response failed");
    TEST_ASSERT(greth_wait_receive(base_addr_dst_eth), "Receiving EDCL request is failed");
    TEST_ASSERT(greth_wait_receive(base_addr_src_eth), "Receiving EDCL response failed");
    TEST_ASSERT(mem_cmp(test_edcl_data_im1, &test_data_resp[13], edcl_cfg.len / sizeof(uint32_t)), "Data compare error!");
    mem_clr(test_data_resp, sizeof(test_edcl_packet_im0));

    rumboot_printf("Checking edcl read is OK\n");

    return 0;
}
/*
 * test_oi10_greth
 */
int main(void)
{
    struct rumboot_irq_entry *tbl;

    rumboot_printf("Start test_oi10_greth\n\n");

    mdio_check(GRETH_0_BASE);
    mdio_check(GRETH_1_BASE);

    tbl = create_greth01_irq_handlers();

    prepare_test_data();
                                            //TX          src addr            dst addr
    //check_transfer_via_external_loopback(GRETH_0_BASE, test_data_im0_src, test_data_im0_dst);
    //check_transfer_via_external_loopback(GRETH_1_BASE, test_data_im0_src, test_data_im0_dst);
    check_transfer_via_external_loopback(GRETH_0_BASE, test_data_im1_src, test_data_im1_dst);
    check_transfer_via_external_loopback(GRETH_1_BASE, test_data_im1_src, test_data_im1_dst);
    //check_transfer_via_external_loopback(GRETH_0_BASE, test_data_im0_src, test_data_im1_dst);
    //check_transfer_via_external_loopback(GRETH_1_BASE, test_data_im0_src, test_data_im1_dst);
    //check_transfer_via_external_loopback(GRETH_0_BASE, test_data_im1_src, test_data_im0_dst);
    //check_transfer_via_external_loopback(GRETH_1_BASE, test_data_im1_src, test_data_im0_dst);

    edcl_seq_number = 0;
    check_edcl_via_external_loopback(GRETH_0_BASE, test_edcl_packet_im1, test_data_im1_dst, test_edcl_rcv_packet_im1);
    delete_greth01_irq_handlers(tbl);
    return 0;
}
