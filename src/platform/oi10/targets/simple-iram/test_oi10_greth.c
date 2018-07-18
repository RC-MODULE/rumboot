#include <stdio.h>
#include <stdint.h>

#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <platform/devices/greth.h>

greth_descr_t  tx_descriptor_data_[512] __attribute__((aligned(1024)));
greth_descr_t  rx_descriptor_data_[512] __attribute__((aligned(1024)));

uint32_t test_data_im0_src[] __attribute__((section(".data")))__attribute__((aligned(0x4))) =
        {
                [0] = 0x00000000,
                [1] = 0x11111111,
                [2] = 0x22222222,
                [3] = 0x33333333,
                [4] = 0x44444444,
                [5] = 0x55555555,
                [6] = 0x66666666,
                [7] = 0x77777777
        };

uint32_t test_data_im0_dst[sizeof(test_data_im0_src)] __attribute__((section(".data")));

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

int check_transfer_from_im1_to_im1()
{
    rumboot_printf("IM1-IM1 checking\n\n");
    memcpy((uint32_t*)IM1_BASE, test_data_im0_src, sizeof(test_data_im0_src));
    greth_mem_copy(GRETH_0_BASE, (uint32_t*)IM1_BASE, (uint32_t*)(IM1_BASE + sizeof(test_data_im0_src)), sizeof(test_data_im0_src) );
    return 0;
}

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

int check_transfer_from_im0_to_im0(uint32_t base_addr_src_eth, uint32_t base_addr_dst_eth)
{
    rumboot_printf("IM0-IM0 checking\n");
    greth_configure_for_receive( base_addr_dst_eth, test_data_im0_dst, sizeof(test_data_im0_src), rx_descriptor_data_);
    greth_configure_for_transmit( base_addr_src_eth, test_data_im0_src, sizeof(test_data_im0_src), tx_descriptor_data_);

    greth_start_receive( base_addr_dst_eth );
    greth_start_transmit( base_addr_src_eth );

    TEST_ASSERT(greth_wait_receive(base_addr_dst_eth), "Receiving is failed");
    TEST_ASSERT(mem_cmp(test_data_im0_src, test_data_im0_dst, sizeof(test_data_im0_src)/sizeof(uint32_t)), "Data compare error!");
    mem_clr(test_data_im0_dst, sizeof(test_data_im0_src)/sizeof(uint32_t));
    return 0;
}

int main(void)
{

//    mdio_check(GRETH_0_BASE);
//    mdio_check(GRETH_1_BASE);
    rumboot_printf("Start test_oi10_greth\n\n");
    check_transfer_from_im0_to_im0(GRETH_0_BASE, GRETH_1_BASE);
    check_transfer_from_im0_to_im0(GRETH_1_BASE, GRETH_0_BASE);
    return 0;
}
