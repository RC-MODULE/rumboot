#include <stdio.h>
#include <stdint.h>

#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <platform/devices/greth.h>

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

void dump_plb4xahb()
{
    rumboot_printf("Dumping PLB4XAHB1 regs\n");
    /*
    rumboot_printf("PLB4XAHB0_REVID          = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+REVID         ));
    rumboot_printf("PLB4XAHB0_SEAR_ADDRUPPER = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+SEAR_ADDRUPPER));
    rumboot_printf("PLB4XAHB0_SEAR_ADDRLOWER = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+SEAR_ADDRLOWER));
    rumboot_printf("PLB4XAHB0_SESR           = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+SESR          ));
    rumboot_printf("PLB4XAHB0_TOP_ADDR       = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+TOP_ADDR      ));
    rumboot_printf("PLB4XAHB0_BOT_ADDR       = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+BOT_ADDR      ));
    rumboot_printf("PLB4XAHB0_ATTRS          = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+ATTRS         ));
    */
    rumboot_printf("PLB4XAHB0_CONTROL        = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+CONTROL       ));
    rumboot_printf("PLB4XAHB0_ERR_ST         = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+ERR_ST        ));
    rumboot_printf("PLB4XAHB0_ERR_ADDR       = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+ERR_ADDR      ));
    rumboot_printf("PLB4XAHB0_INT_MSK        = 0x%x\n", dcr_read(DCR_PLB4AHB_0_BASE+INT_MSK       ));
}



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

int main(void)
{

//    mdio_check(GRETH_0_BASE);
//    mdio_check(GRETH_1_BASE);
    rumboot_printf("Start test_oi10_greth\n\n");

    iowrite32(0xBABADEDA, 0xC0000000);
    rumboot_printf("MEM[0xC0000000] 0x%x\n\n", ioread32(0xC0000000));

    memcpy((uint32_t*)IM1_BASE, test_data_im0_src, sizeof(test_data_im0_src));
    for (uint32_t i = 0; i<8; i++)
    {
        rumboot_printf("test_data_im1_src[%d] = 0x%x\n\n", i, ioread32(IM1_BASE+(i<<2)) );
    }

/*
    dump_plb4xahb();
    if (!greth_mem_copy(GRETH_0_BASE, test_data_src, test_data_dst, sizeof(test_data_dst)))
    {
        dump_plb4xahb();
        return 1;
    }
*/
    return 0;
}
