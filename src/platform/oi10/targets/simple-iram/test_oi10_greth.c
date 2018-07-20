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

#define GRETH_TEST_DATA_LEN 69
#define TST_MAC_MSB     0xFFFF
#define TST_MAC_LSB     0xFFFFFFFF
greth_mac_t tst_greth_mac = {TST_MAC_MSB, TST_MAC_LSB};

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
greth_descr_t  tx_descriptor_data_[GRETH_MAX_DESCRIPTORS_COUNT] __attribute__((aligned(1024)));
greth_descr_t  rx_descriptor_data_[GRETH_MAX_DESCRIPTORS_COUNT] __attribute__((aligned(1024)));

uint32_t test_data_im0_src[GRETH_TEST_DATA_LEN] __attribute__((section(".data")))__attribute__((aligned(0x4)));
uint32_t test_data_im0_dst[GRETH_TEST_DATA_LEN] __attribute__((section(".data")))__attribute__((aligned(0x4)));

uint32_t* test_data_im1_src = (uint32_t *) IM1_BASE_MIRROR;
uint32_t* test_data_im1_dst = (uint32_t *)(IM1_BASE_MIRROR + sizeof(test_data_im0_src));

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
            rumboot_printf("Data error:\nsrc: 0x%x\ndst: 0x%x\n", *src_addr, *dst_addr);
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
    uint32_t j=0;
    test_data_im0_src[i++] = (tst_greth_mac.mac_msb << 16) | ((tst_greth_mac.mac_lsb & 0xFFFF0000) >> 16);//DST MAC [48:16]
    test_data_im0_src[i++] = ((tst_greth_mac.mac_lsb & 0xFFFF) << 16) | tst_greth_mac.mac_msb;//DST MAC [15:0] SRC MAC [48:32]
    test_data_im0_src[i++] = tst_greth_mac.mac_lsb;//SRC MAC [31:0]
    test_data_im0_src[i++] = ((GRETH_TEST_DATA_LEN * sizeof(uint32_t) - 14) << 16);//Len and 0x0000 data
    while(j<32)
    {
        test_data_im0_src[i] = (1 << j);
        i++; j++;
    }
    test_data_im0_src[i++] = 0xFFFFFFFF;
    j=0;
    while(j<32)
    {
        test_data_im0_src[i] = ~(1 << j);
        i++; j++;
    }
    memcpy(test_data_im1_src, test_data_im0_src, sizeof(test_data_im0_src));
}

/*
 * Checks transfers which uses different memory blocks
 */
int check_transfer_via_external_loopback(uint32_t base_addr_src_eth, uint32_t base_addr_dst_eth, uint32_t * test_data_src, uint32_t * test_data_dst)
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

/*
 * test_oi10_greth
 */
int main(void)
{
    struct rumboot_irq_entry *tbl;

    rumboot_printf("Start test_oi10_greth\n\n");

//    mdio_check(GRETH_0_BASE);
//    mdio_check(GRETH_1_BASE);

    tbl = create_greth01_irq_handlers();

    prepare_test_data(test_data_im1_src);

                                            //TX          //RX          src addr            dst addr
    check_transfer_via_external_loopback(GRETH_0_BASE, GRETH_1_BASE, test_data_im0_src, test_data_im0_dst);
    check_transfer_via_external_loopback(GRETH_1_BASE, GRETH_0_BASE, test_data_im0_src, test_data_im0_dst);
    check_transfer_via_external_loopback(GRETH_0_BASE, GRETH_1_BASE, test_data_im1_src, test_data_im1_dst);
    check_transfer_via_external_loopback(GRETH_1_BASE, GRETH_0_BASE, test_data_im1_src, test_data_im1_dst);
    check_transfer_via_external_loopback(GRETH_0_BASE, GRETH_1_BASE, test_data_im0_src, test_data_im1_dst);
    check_transfer_via_external_loopback(GRETH_1_BASE, GRETH_0_BASE, test_data_im0_src, test_data_im1_dst);
    check_transfer_via_external_loopback(GRETH_0_BASE, GRETH_1_BASE, test_data_im1_src, test_data_im0_dst);
    check_transfer_via_external_loopback(GRETH_1_BASE, GRETH_0_BASE, test_data_im1_src, test_data_im0_dst);

    delete_greth01_irq_handlers(tbl);
    return 0;
}
