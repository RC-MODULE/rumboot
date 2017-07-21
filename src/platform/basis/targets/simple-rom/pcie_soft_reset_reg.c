
//-----------------------------------------------------------------------------
//  This program is for testing PCIe RG, placed in SCTL.
//  It shows, that PCIe can be put into reset and waked up several times.
//  PCIe consider turned on, if link training complete,
//    so it needs response device in Root Port mode.
//  It includes:
//      - reset off, wait until PCIe link up
//      - reset on
//    - repeat several times
//
//-----------------------------------------------------------------------------
#include <basis/defs_c.h>

#define PLL_LOCK_DURATION    0x1000
#define TRAINING_DURATION    0x1000

void pcie_reset ()
{
    rgSCTL_PCIE_RST = 0x0;
}

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
        
    timer_cntr = 0;
    while ((rgPCIe_LocMgmt_i_pl_config_0_reg & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == TRAINING_DURATION)
            return -2;
    }
    
    return 0;
}

int main ()
{
    pcie_reset ();
    if (PCIe_setting_ () != 0)
        return -1;
        
    pcie_reset ();
    if (PCIe_setting_ () != 0)
        return -1;
        
    pcie_reset ();
    if (PCIe_setting_ () != 0)
        return -1;
        
    return 0;
}
