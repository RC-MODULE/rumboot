
//-----------------------------------------------------------------------------
//  This file contain PCIe functions for secondary loader (Basis project)
//  It includes next functions:
//    - pcie_init
//    - pcie_add_bar
//    - pcie_link_up
//    - 
//    - 
//    - 
//    - 
//    - 
//    - 
//
//      !!! Some functions includes PCIe Speedup for simulation !!!
//      !!!         It must be removed in real program          !!!
//
//-----------------------------------------------------------------------------
#include <stdint.h>

#include <regs/regs_pcie.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/devices.h>


//-----------------------------------------------------------------------------
//  These defines must be changed for real program
//  Unproven values!!
//-----------------------------------------------------------------------------
#ifdef CMAKE_BUILD_TYPE_DEBUG
#define PCIE_TEST_LIB_PLL_LOCK_TIMEOUT    0x1000
#define PCIE_TEST_LIB_TRAINING_TIMEOUT    0x1000
#else
#define PCIE_TEST_LIB_PLL_LOCK_TIMEOUT    0x1000000
#define PCIE_TEST_LIB_TRAINING_TIMEOUT    0x1000000
#endif

void pcie_sim_speedup ()
{
#ifndef PCIE_TEST_LIB_SIMSPEEDUP_OFF
#ifdef RUMBOOT_BUILD_TYPE_DEBUG
#ifdef CMAKE_BUILD_TYPE_DEBUG
    /***************************************************/
    /*    this PHY settings are only for simulation    */
    /*    and must be removed for real programm        */
    /***************************************************/
    //-------------------------------------------------------------------------
    //  These settings may be replaced with corresponding key on RTL simulation
    //    And they are neccessary in Netlist simulation
    //-------------------------------------------------------------------------
    rumboot_printf ("\n  Warning: Simulation speedup On\n\n");
    iowrite32 (0b0000000011111010, PCIE_PHY_BASE + PCIe_Phy_MASSWR_TX_RCVDET_CTRL                );
    iowrite32 (0b0000000000000001, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_BANDGAP_TMR                  );
    iowrite32 (0b0000000000000001, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_BIAS_TMR                     );
    iowrite32 (0b0000000000000001, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_PLLEN_TMR                    );
    iowrite32 (0b0000000000000001, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_PLLPRE_TMR                   );
    iowrite32 (0b0000000000000001, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_PLLVREF_TMR                  );
    iowrite32 (0b0000000000000010, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_PLLLOCK_TMR                  );
    iowrite32 (0b0000000000000001, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_PLLCLKDIS_TMR                );
    iowrite32 (0b0000000000000001, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_LANECAL_TMR                  );
    iowrite32 (0b0000000000000001, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_A0IN_TMR                     );
    iowrite32 (0b0000000000000100, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_A1IN_TMR                     );
    iowrite32 (0b0000000000000100, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_A2IN_TMR                     );
    iowrite32 (0b0000000000000100, PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_A3IN_TMR                     );
    iowrite32 (0b1000000000000000, PCIE_PHY_BASE + PCIe_Phy_CMN_BGCAL_OVRD                       );
    iowrite32 (0b1000000000000000, PCIE_PHY_BASE + PCIe_Phy_CMN_PLL_CPI_OVRD                     );
    iowrite32 (0b1000000000000000, PCIE_PHY_BASE + PCIe_Phy_MASSWR_CMN_SDCAL0_OVRD               );
    iowrite32 (0b1000000000000000, PCIE_PHY_BASE + PCIe_Phy_MASSWR_CMN_SDCAL1_OVRD               );
    iowrite32 (0b1000000000010010, PCIE_PHY_BASE + PCIe_Phy_CMN_PLL_VCOCAL_OVRD                  );
    iowrite32 (0b0000000000010000, PCIE_PHY_BASE + PCIe_Phy_CMN_PLL_VCOCAL_CNT_START             );
    iowrite32 (0b0000000000010000, PCIE_PHY_BASE + PCIe_Phy_CMN_PLL_LOCK_CNT_START               );
    iowrite32 (0b1000000000000010, PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_IQPI_ILL_CAL_OVRD          );
    iowrite32 (0b0000000000010000, PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_IQPI_ILL_LOCK_REFCNT_START );
    iowrite32 (0b0000000000010000, PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_IQPI_ILL_LOCK_CALCNT_START0);
    iowrite32 (0b0000000000010000, PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_IQPI_ILL_LOCK_CALCNT_START1);
    iowrite32 (0b1000000000000010, PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_EPI_ILL_CAL_OVRD           );
    iowrite32 (0b0000000000010000, PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_EPI_ILL_LOCK_REFCNT_START  );
    iowrite32 (0b0000000000010000, PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_EPI_ILL_LOCK_CALCNT_START0 );
    iowrite32 (0b0000000000010000, PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_EPI_ILL_LOCK_CALCNT_START1 );
    // iowrite32 (0b00_0000_1000, PCIE_PHY_BASE + PCIe_Phy_LANE_CAL_RESET_TIME_VALUE         );
    iowrite32 (0b1000000000100010, PCIE_PHY_BASE + PCIe_Phy_CMN_TXPUCAL_OVRD                     );
    iowrite32 (0b1000000000100010, PCIE_PHY_BASE + PCIe_Phy_CMN_TXPDCAL_OVRD                     );
    iowrite32 (0b1000000000000011, PCIE_PHY_BASE + PCIe_Phy_CMN_RXCAL_OVRD                       );
    iowrite32 (0b0000000000000001, PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_SLC_INIT_TMR               );
    iowrite32 (0b0000000000000001, PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_SLC_RUN_TMR                );
    // iowrite32 (0b0000000000010000, PCIE_PHY_BASE + PCIe_Phy_CMN_PRPLL_LOCK_CNT_START          );
    
    //-------------------------------------------------------------------------
    //  These settings must be used in any type of simulation
    //-------------------------------------------------------------------------
    iowrite32 (0x4010, PCIE_PHY_BASE + PCIe_Phy_PCS_COM_LOCK_CFG1      );
    iowrite32 (0x0810, PCIE_PHY_BASE + PCIe_Phy_PCS_COM_LOCK_CFG2      );
    iowrite32 (0x0101, PCIE_PHY_BASE + PCIe_Phy_PCS_GEN3_EIE_LOCK_CFG  );
    iowrite32 (0x000A, PCIE_PHY_BASE + PCIe_Phy_PCS_RCV_DET_INH        );
#endif
#endif
#endif
}

//-----------------------------------------------------------------------------
//  This function setup initial PCIe parameters and waits PLL lock
//    
//  Arguments:
//    - speed
//        0 - Gen1  2.5 Gbps
//        1 - Gen1  2.5 Gbps    or    Gen2  5.0 Gbps
//    - device_id
//        change this parameter if you want to differentiate few PCBs
//    - revision_id
//        change this parameter if you want to differentiate few revisions
//    - subclass_code
//    - class_code
//        0x00 - unclassified - may cause problems with driver
//        0xFF - unassigned
//        all other - wath in PCIe doc or in internet
//    - state_auto_ack
//        if not set, configuration requests from host after D state change 
//        will require manual completion
//-----------------------------------------------------------------------------
uint32_t pcie_init
(
    uint32_t speed              ,
    uint32_t device_id          ,
    uint32_t revision_id        ,
    uint32_t subclass_code      ,
    uint32_t class_code         ,
    uint32_t state_auto_ack
)
{
    uint32_t rdata;
    uint32_t timer_cntr;
    
    rumboot_printf ("\n  pcie_init start\n");
    
    pcie_sim_speedup ();
    
    //---------------------------------------------------------------
    //  Disable Link Training
    //---------------------------------------------------------------
    rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) & 0xFFFFFFEF;
    iowrite32 (rdata, SCTL_BASE + SCTL_PCIE_REG_0);

    //---------------------------------------------------------------
    //  Setup initial strap parameters
    //  And turn off PCIe MBIST
    //---------------------------------------------------------------
    rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_0);
    iowrite32 (rdata & 0xF87FFFFE | speed, SCTL_BASE + SCTL_PCIE_REG_0);
    rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_1);
    iowrite32 (rdata & 0xFFFFFFFB | (state_auto_ack << 2), SCTL_BASE + SCTL_PCIE_REG_1);

    //---------------------------------------------------------------
    //  PCIe reset Off
    //---------------------------------------------------------------
    iowrite32 (0x1, SCTL_BASE + SCTL_PCIE_RST);

    //---------------------------------------------------------------
    //  Wait until PLL locked
    //   Exit with error if emergency timer overflow
    //---------------------------------------------------------------
    timer_cntr = 0;
    while ((ioread32 (SCTL_BASE + SCTL_PCIE_RST) & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == PCIE_TEST_LIB_PLL_LOCK_TIMEOUT)
        {
            rumboot_printf ("\n  Warning: PCIe PLL lock timeout.\n\n");
            return -1;
        }
    }
    
    rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_vendor_id_device_id);
    iowrite32 (rdata & 0x0000FFFF | (device_id << 16), PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_vendor_id_device_id);
    
    rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_revision_id_class_code);
    iowrite32 (rdata & 0x0000FF00 | revision_id | (subclass_code << 16) | (class_code << 24), PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_revision_id_class_code);
    
    //  TODO: probably, this action must be done from the host. Not sure.
    // iowrite32 (0x07, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_command_status);
    
    return 0;
}

//-----------------------------------------------------------------------------
//  This function setup initial BAR parameters
//    
//  Arguments:
//    - bar_number
//        0-5 BARs are available for EndPoint mode
//    - bar_axi_start
//        AXI start address for selected BAR.
//        Here internal PCIe address transactor is used.
//        Must be aligned to bar_aperture
//    - bar_aperture
//        Size of selected BAR in 128 Byte units
//        0x00 - 128 B
//        0x01 - 256 B
//        ...
//        0x05 - 4 KB
//        ...
//        0x13 - 64 MB
//        ...
//        0x18 - 2 GB
//        0x19 - 4 GB   (64bit Memory only)
//        ...           (64bit Memory only)
//        0x1F - 256 GB (64bit Memory only)
//        
//    - bar_type
//        0x0 - Disabled
//        0x1 - IO
//        0x4 - 32bit Memory, non prefetch
//        0x5 - 32bit Memory, prefetchable
//        0x6 - 64bit Memory, non prefetch
//        0x7 - 64bit Memory, prefetchable
//        Other - reserved
//
//-----------------------------------------------------------------------------
void pcie_add_bar (uint32_t bar_number, uint32_t bar_axi_start, uint32_t bar_aperture, uint32_t bar_type)
{
    uint32_t rdata;
    
    //------------------------------------------------------
    //  Enable BAR {x} for inbound transactions
    //  also set BAR {x} size
    //------------------------------------------------------
    switch (bar_number)
    {
        case 0 : 
            rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
            iowrite32 (rdata & 0xFFFFFF00 | (bar_type <<  5) | (bar_aperture <<  0), PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
            iowrite32 (bar_axi_start, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_0_addr_translation);
            break;
        case 1 : 
            rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
            iowrite32 (rdata & 0xFFFF00FF | (bar_type << 13) | (bar_aperture <<  8), PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
            iowrite32 (bar_axi_start, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_1_addr_translation);
            break;
        case 2 : 
            rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
            iowrite32 (rdata & 0xFF00FFFF | (bar_type << 21) | (bar_aperture << 16), PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
            iowrite32 (bar_axi_start, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_2_addr_translation);
            break;
        case 3 : 
            rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
            iowrite32 (rdata & 0x00FFFFFF | (bar_type << 27) | (bar_aperture << 24), PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
            iowrite32 (bar_axi_start, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_3_addr_translation);
            break;
        case 4 : 
            rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_1_reg);
            iowrite32 (rdata & 0xFFFFFF00 | (bar_type <<  5) | (bar_aperture <<  0), PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_1_reg);
            iowrite32 (bar_axi_start, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_4_addr_translation);
            break;
        case 5 : 
            rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_1_reg);
            iowrite32 (rdata & 0xFFFF00FF | (bar_type << 13) | (bar_aperture <<  8), PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_1_reg);
            iowrite32 (bar_axi_start, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_5_addr_translation);
            break;
        default :
            rumboot_printf ("\n  Warning: Invalid PCIe BAR number\n\n");
    }
}

//-----------------------------------------------------------------------------
//  This function waits, until PCIe link is up
//-----------------------------------------------------------------------------
uint32_t pcie_link_up ()
{
    uint32_t rdata;
    uint32_t timer_cntr;
    //---------------------------------------------------------------
    //  Enable Link Training
    //---------------------------------------------------------------
    rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) | 0x00000010;
    iowrite32 (rdata, SCTL_BASE + SCTL_PCIE_REG_0);

    //---------------------------------------------------------------
    //  Wait until training complete
    //   Exit with error if emergency timer overflow
    //---------------------------------------------------------------
    timer_cntr = 0;
    while ((ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pl_config_0_reg) & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == PCIE_TEST_LIB_TRAINING_TIMEOUT)
        {
            rumboot_printf ("\n  Warning: PCIe link training timeout.\n\n");
            return -1;
        }
    }
    rumboot_printf ("  PCIe link up\n");

    rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pl_config_0_reg);
    rumboot_printf ("    negotiated link width:  %d\n", (1 << ((rdata >> 1) & 0x3)));
#ifdef PRODUCTION_TESTING
    if ((1 << ((rdata >> 1) & 0x3)) != 4)
    {
        rumboot_printf ("    ERROR: not all 4 lanes are active\n");
        return -1;
    }
#endif
    if (((rdata >> 3) & 0x3) == 0)
        rumboot_printf ("    negotiated link speed:  2.5GTs\n");
    if (((rdata >> 3) & 0x3) == 1)
        rumboot_printf ("    negotiated link speed:  5.0GTs\n");

    if (((rdata >> 5) & 0x1) == 0)
        rumboot_printf ("    mode:  upstream (Endpoint)\n");
    else
        rumboot_printf ("    mode:  downstream (RootPort)\n");
    
    return 0;
}



