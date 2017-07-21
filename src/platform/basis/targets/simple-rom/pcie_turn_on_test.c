
//-----------------------------------------------------------------------------
//  This program is for simple PCIe subsystem testing
//  It includes:
//    - initialisation and turning ON function
//    
//    Initialisation to:
//      EndPoint mode - default in SCTL
//      Gen 2 - default in SCTL
//      BAR0 - Memory type - first 2 GB of space
//      BAR1 - Memory type - last 2 GB of space
//      other BARs turned off
//      no address transaltion
//
//      !!! Program includes PCIe Speedup for simulation !!!
//      !!!      It must be removed in real program      !!!
//
//-----------------------------------------------------------------------------

#include <basis/defs_c.h>

#define PLL_LOCK_DURATION    0x1000
#define TRAINING_DURATION    0x1000

int PCIe_setting_ ()
{
    int timer_cntr;
    /***************************************************/
    /*    this PHY settings are only for simulation    */
    /*    and must be removed for real programm        */
    /***************************************************/
    rgPCIe_Phy_PCS_COM_LOCK_CFG1        = 0x4010;
    rgPCIe_Phy_PCS_COM_LOCK_CFG2        = 0x0810;
    rgPCIe_Phy_PCS_GEN3_EIE_LOCK_CFG    = 0x0101;
    rgPCIe_Phy_PCS_RCV_DET_INH          = 0x000A;
    /***************************************************/
    
    rgSCTL_PCIE_RST = 0x1;
    
    timer_cntr = 0;
    while ((rgSCTL_PCIE_RST & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == PLL_LOCK_DURATION)
            return -1;
    }
    
    /***************************************************************/
    /*     Enable access through controller                        */
    /***************************************************************/
    rgPCIe_EP_i_command_status = 0x07;
    /***************************************************************/
    /*     BAR 1 - what high bits of inbound address to mask       */
    /***************************************************************/
    rgPCIe_EP_i_base_addr_1 = 0x80000000;
    /***************************************************************/
    /* enable BAR 0 and BAR 1 for inbound transactions             */
    /*   - maximum BAR size selected - 2 GB                        */
    /*   - two BARs cover all memory space                         */
    /***************************************************************/
    rgPCIe_LocMgmt_i_pf_0_BAR_config_0_reg = 0x00009898;
    /***************************************************************/
    /*     BAR 1 - what to put instead of masked bits              */
    /***************************************************************/
    rgPCIe_AXI_inregion_ep_bar_1_addr_translation = 0x80000000;
    /***************************************************************/
    /*     AXI outbound addr translation setup                     */
    /*       - pass 32 bits to bridge, zero none bits              */
    /***************************************************************/
    rgPCIe_AXI_outregion_0_addr_translation_0 = 0x0000001F;
    /***************************************************************/
    /*     AXI outbound PCIe header descriptor                     */
    /*       - configure for memory RW                             */
    /***************************************************************/
    rgPCIe_AXI_outregion_0_header_descriptor_0 = 0x00000002;
    /***************************************************************/
    /*     AXI Outbound Region 0 AXI Region Base Address 0         */
    /*       - none of bits choose region                          */
    /***************************************************************/
    rgPCIe_AXI_outregion_0_region_base_addr = 0x0000001F;
        
    timer_cntr = 0;
    while ((rgPCIe_LocMgmt_i_pl_config_0_reg & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == TRAINING_DURATION)
            return -1;
    }
    
    return 0;
}

int main ()
{
    if (PCIe_setting_ () != 0)
        return -1;
    
    return 0;
}

