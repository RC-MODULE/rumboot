
//-----------------------------------------------------------------------------
//  This file contain PCIe related functions, used for testing.
//  It includes next functions:
//    - controller software reset
//    - internal loopback On
//    - simple turn On
//    - turn On with options (EndPoint mode)
//    - turn On with options (RootPort mode)
//    - switch freq ???
//    - ext_irq_gen settings TODO
//    - AT settings TODO
//
//      !!! Some functions includes PCIe Speedup for simulation !!!
//      !!!         It must be removed in real program          !!!
//
//-----------------------------------------------------------------------------
#include <basis/defs_c.h>

//-----------------------------------------------------------------------------
//  These defines must be changed for real program
//-----------------------------------------------------------------------------
#define PCIE_TEST_LIB_PLL_LOCK_DURATION    0x1000
#define PCIE_TEST_LIB_TRAINING_DURATION    0x1000


//-----------------------------------------------------------------------------
//  This function put PCIe subsystem into reset
//    Doesnt reset settings in SCTL and Sticky parameters in PCIe controller.
//    Doesnt send any reset-related messages by PCIe link.
//-----------------------------------------------------------------------------
void pcie_soft_reset ()
{
    rgSCTL_PCIE_RST = 0x0;
}

//-----------------------------------------------------------------------------
//  This function is for turning ON one of PCIe subsystem loopback modes
//    It influence only on Phy.
//    To make controller to finish training in loopback choose this option
//      in turn ON function (or set neccessasry parameter manually)
//  
//  Arguments:
//    - loopback type
//        only one mode must be chosen
//        [5:0]  0x01 - Serial Loopback Mode                  Our    after SERDES
//               0x02 - ISI Generation Loopback Mode          Our    after buffer
//               0x04 - Line Side Loopback Mode               Their  after buffer
//               0x08 - Recovered Clock Loopback Mode         Their  after CDR
//               0x10 - Near End Parallel Loopback Mode       Our    after before SERDES
//               0x20 - Far End Parallel Loopback Mode        Their  after SERDES
//        [31:6] *not used*
//  
//  
//-----------------------------------------------------------------------------
void pcie_loopback_mode_on (int loopback_mode)
{
    rgPCIe_Phy_PMA_XCVR_LPBK = loopback_mode;
}

//-----------------------------------------------------------------------------
//  This function is for PCIe simple turn On.
//  All settings are default.
//    It includes:
//      - PCIe reset Off
//      - Wait until training complete
//        - Exit with error if emergency timer overflow
//-----------------------------------------------------------------------------
int pcie_simple_turn_on ()
{
    int timer_cntr;
#ifndef PCIE_TEST_LIB_SIMSPEEDUP_OFF
    /***************************************************/
    /*    this PHY settings are only for simulation    */
    /*    and must be removed for real programm        */
    /***************************************************/
    rgPCIe_Phy_PCS_COM_LOCK_CFG1        = 0x4010;
    rgPCIe_Phy_PCS_COM_LOCK_CFG2        = 0x0810;
    rgPCIe_Phy_PCS_GEN3_EIE_LOCK_CFG    = 0x0101;
    rgPCIe_Phy_PCS_RCV_DET_INH          = 0x000A;
    /***************************************************/
#endif
    
    rgSCTL_PCIE_RST = 0x1;
        
    
    //---------------------------------------------------------------
    //  Wait until PLL locked
    //   Exit with error if emergency timer overflow
    //---------------------------------------------------------------
    timer_cntr = 0;
    while ((rgSCTL_PCIE_RST & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == PCIE_TEST_LIB_PLL_LOCK_DURATION)
            return -1;
    }
    
    //---------------------------------------------------------------
    //  Wait until training complete
    //   Exit with error if emergency timer overflow
    //---------------------------------------------------------------
    timer_cntr = 0;
    while ((rgPCIe_LocMgmt_i_pl_config_0_reg & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == PCIE_TEST_LIB_TRAINING_DURATION)
            return -2;
    }
    
    return 0;
}

//-----------------------------------------------------------------------------
//  This function is for PCIe turn On in EndPoint mode with options
//    Very few options are used here. Look documentation for full list.
//    It includes:
//      - Disable link training
//      - Set base options in SCTL
//      - PCIe reset Off
//      - Wait until PLL locked complete
//        - Exit with error if emergency timer overflow
//      - Set options in PCIe controller
//      - Set address translation in PCIe controller
//      - Enable link training
//      - Wait until Link Trained
//        - Exit with error if emergency timer overflow
//
//
//  Arguments:
//    - usual_settings
//        1:
//          other arguments are not used
//          EP, Gen2, 4 lanes, 0 and 1 BARs cover all 4 Gb, no address translation
//        0:
//          other arguments are used to make neccessary configuration
//
//    - sctl_base_opt
//        [0]     pcie_generation_sel
//        [1]     *value not used here, always EP*
//        [3:2]   lane_count_in
//        [31:4]  *value not used here*
//      
//    - i_command_status
//        [0]    IO-Space Enable
//        [1]    Mem-Space Enable
//        [2]    Bus-Master Enable
//        [31:3] *not used here*
//      
//    - i_base_addr_{x}
//        standard PCIe base address registers
//      
//    - i_pf_0_BAR_config_{x}_reg
//        types and sizes of different BARs
//      
//    - bar_{x}_addr_translation
//        parameters for inbound address translation
//      
//-----------------------------------------------------------------------------
int pcie_turn_on_with_options_ep
(
    char usual_settings             ,
    int  sctl_base_opt              ,
    int  i_command_status           ,
    int  i_base_addr_0              ,
    int  i_base_addr_1              ,
    int  i_base_addr_2              ,
    int  i_base_addr_3              ,
    int  i_base_addr_4              ,
    int  i_base_addr_5              ,
    int  i_pf_0_BAR_config_0_reg    ,
    int  i_pf_0_BAR_config_1_reg    ,
    int  ep_bar_0_addr_translation  ,
    int  ep_bar_1_addr_translation  ,
    int  ep_bar_2_addr_translation  ,
    int  ep_bar_3_addr_translation  ,
    int  ep_bar_4_addr_translation  ,
    int  ep_bar_5_addr_translation

)
{
    int timer_cntr;
    
    
#ifndef PCIE_TEST_LIB_SIMSPEEDUP_OFF
    /***************************************************/
    /*    this PHY settings are only for simulation    */
    /*    and must be removed for real programm        */
    /***************************************************/
    rgPCIe_Phy_PCS_COM_LOCK_CFG1        = 0x4010;
    rgPCIe_Phy_PCS_COM_LOCK_CFG2        = 0x0810;
    rgPCIe_Phy_PCS_GEN3_EIE_LOCK_CFG    = 0x0101;
    rgPCIe_Phy_PCS_RCV_DET_INH          = 0x000A;
    /***************************************************/
#endif
    
    
    //---------------------------------------------------------------
    //  Disable Link Training
    //---------------------------------------------------------------
    rgSCTL_PCIE_REG_0 &= 0xFFFFFFEF;
    
    //---------------------------------------------------------------
    //  Set base options in SCTL
    //---------------------------------------------------------------
    if (usual_settings == 0)
        rgSCTL_PCIE_REG_0 = (rgSCTL_PCIE_REG_0 & 0xFFFFFFF0) | (sctl_base_opt & 0x0000000D);
    
    //---------------------------------------------------------------
    //  PCIe reset Off
    //---------------------------------------------------------------
    rgSCTL_PCIE_RST = 0x1;
    
    //---------------------------------------------------------------
    //  Wait until PLL locked
    //   Exit with error if emergency timer overflow
    //---------------------------------------------------------------
    timer_cntr = 0;
    while ((rgSCTL_PCIE_RST & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == PCIE_TEST_LIB_PLL_LOCK_DURATION)
            return -1;
    }
    
    
    //---------------------------------------------------------------
    //  Set options in PCIe controller
    //---------------------------------------------------------------
    if (usual_settings == 0)
    {
        //------------------------------------------------------
        //  Enable access through controller
        //------------------------------------------------------
        rgPCIe_EP_i_command_status = i_command_status;
        //------------------------------------------------------
        //  BAR {x} - what high bits of inbound address to mask
        //------------------------------------------------------
        rgPCIe_EP_i_base_addr_0 = i_base_addr_0;
        rgPCIe_EP_i_base_addr_1 = i_base_addr_1;
        rgPCIe_EP_i_base_addr_2 = i_base_addr_2;
        rgPCIe_EP_i_base_addr_3 = i_base_addr_3;
        rgPCIe_EP_i_base_addr_4 = i_base_addr_4;
        rgPCIe_EP_i_base_addr_5 = i_base_addr_5;
        //------------------------------------------------------
        //  Enable BAR {x} for inbound transactions
        //  also set BAR {x} size
        //------------------------------------------------------
        rgPCIe_LocMgmt_i_pf_0_BAR_config_0_reg = i_pf_0_BAR_config_0_reg;
        rgPCIe_LocMgmt_i_pf_0_BAR_config_1_reg = i_pf_0_BAR_config_1_reg;
        //------------------------------------------------------
        //  BAR {x} - what to put instead of masked bits
        //------------------------------------------------------
        rgPCIe_AXI_inregion_ep_bar_0_addr_translation = ep_bar_0_addr_translation;
        rgPCIe_AXI_inregion_ep_bar_1_addr_translation = ep_bar_1_addr_translation;
        rgPCIe_AXI_inregion_ep_bar_2_addr_translation = ep_bar_2_addr_translation;
        rgPCIe_AXI_inregion_ep_bar_3_addr_translation = ep_bar_3_addr_translation;
        rgPCIe_AXI_inregion_ep_bar_4_addr_translation = ep_bar_4_addr_translation;
        rgPCIe_AXI_inregion_ep_bar_5_addr_translation = ep_bar_5_addr_translation;
    }
    else
    {
        //------------------------------------------------------
        //  Enable access through controller
        //------------------------------------------------------
        rgPCIe_EP_i_command_status = 0x07;
        //------------------------------------------------------
        //  BAR 1 - what high bits of inbound address to mask
        //------------------------------------------------------
        rgPCIe_EP_i_base_addr_1 = 0x80000000;
        //------------------------------------------------------
        //  Enable BAR 0 and BAR 1 for inbound transactions
        //   - maximum BAR size selected - 2 GB
        //   - two BARs cover all memory space
        //------------------------------------------------------
        rgPCIe_LocMgmt_i_pf_0_BAR_config_0_reg = 0x00009898;
        //------------------------------------------------------
        //  BAR 1 - what to put instead of masked bits
        //------------------------------------------------------
        rgPCIe_AXI_inregion_ep_bar_1_addr_translation = 0x80000000;
    }
    
    
    //---------------------------------------------------------------
    //  Probably, it will be better to make outbound address
    //    translation parameters as function arguments too.
    //  There are 32 regions, but only one is described here
    //---------------------------------------------------------------
    //------------------------------------------------------
    //     AXI outbound addr translation setup
    //       - pass 32 bits to bridge, zero none bits
    //------------------------------------------------------
    rgPCIe_AXI_outregion_0_addr_translation_0 = 0x0000001F;
    //------------------------------------------------------
    //     AXI outbound PCIe header descriptor
    //       - configure for memory RW
    //------------------------------------------------------
    rgPCIe_AXI_outregion_0_header_descriptor_0 = 0x00000002;
    //------------------------------------------------------
    //     AXI Outbound Region 0 AXI Region Base Address 0
    //       - none of bits choose region
    //------------------------------------------------------
    rgPCIe_AXI_outregion_0_region_base_addr = 0x0000001F;
    
    
    //---------------------------------------------------------------
    //  Enable Link Training
    //---------------------------------------------------------------
    rgSCTL_PCIE_REG_0 |= 0x00000010;
    
    //---------------------------------------------------------------
    //  Wait until training complete
    //   Exit with error if emergency timer overflow
    //---------------------------------------------------------------
    timer_cntr = 0;
    while ((rgPCIe_LocMgmt_i_pl_config_0_reg & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == PCIE_TEST_LIB_TRAINING_DURATION)
            return -1;
    }
    
    return 0;
}

//-----------------------------------------------------------------------------
//  This function is for PCIe turn On in RootPort mode with options
//    Very few options are used here. Look documentation for full list.
//    It includes:
//      - Disable link training
//      - Set base options in SCTL
//      - PCIe reset Off
//      - Wait until PLL locked complete
//        - Exit with error if emergency timer overflow
//      - Set options in PCIe controller
//      - Set address translation in PCIe controller
//      - Enable link training
//      - Wait until Link Trained
//        - Exit with error if emergency timer overflow
//
//
//  Arguments:
//    - usual_settings
//        1:
//          other arguments are not used
//          RC, Gen2, 4 lanes, 0 and 1 BARs cover all 4 Gb, no address translation
//        0:
//          other arguments are used to make neccessary configuration
//
//    - sctl_base_opt
//        [0]     pcie_generation_sel
//        [1]     *value not used here, always RC*
//        [3:2]   lane_count_in
//        [31:4]  *value not used here*
//      
//    - i_command_status
//        [0]    IO-Space Enable
//        [1]    Mem-Space Enable
//        [2]    Bus-Master Enable
//        [31:3] *not used here*
//      
//    - i_base_addr_{x}
//        standard PCIe base address registers
//      
//    - i_pf_0_BAR_config_{x}_reg
//        types and sizes of different BARs
//      
//    - bar_{x}_addr_translation
//        parameters for inbound address translation
//      
//-----------------------------------------------------------------------------
int pcie_turn_on_with_options_rc
(
    char usual_settings             ,
    char high_lvl_loopback_mode     ,
    int  sctl_base_opt              ,
    int  i_command_status           ,
    int  i_RC_BAR_0                 ,
    int  i_RC_BAR_1                 ,
    int  i_rc_BAR_config_reg        ,
    int  rc_bar_0_addr_translation  ,
    int  rc_bar_1_addr_translation

)
{
    int timer_cntr;
    
    
#ifndef PCIE_TEST_LIB_SIMSPEEDUP_OFF
    /***************************************************/
    /*    this PHY settings are only for simulation    */
    /*    and must be removed for real programm        */
    /***************************************************/
    rgPCIe_Phy_PCS_COM_LOCK_CFG1        = 0x4010;
    rgPCIe_Phy_PCS_COM_LOCK_CFG2        = 0x0810;
    rgPCIe_Phy_PCS_GEN3_EIE_LOCK_CFG    = 0x0101;
    rgPCIe_Phy_PCS_RCV_DET_INH          = 0x000A;
    /***************************************************/
#endif
    
    
    //---------------------------------------------------------------
    //  Disable Link Training
    //---------------------------------------------------------------
    rgSCTL_PCIE_REG_0 &= 0xFFFFFFEF;
    
    //---------------------------------------------------------------
    //  Set base options in SCTL
    //---------------------------------------------------------------
    if (usual_settings == 0)
        rgSCTL_PCIE_REG_0 = (rgSCTL_PCIE_REG_0 & 0xFFFFFFF0) | (sctl_base_opt & 0x0000000F) | 0x2;
    
    //---------------------------------------------------------------
    //  PCIe reset Off
    //---------------------------------------------------------------
    rgSCTL_PCIE_RST = 0x1;
    
    //---------------------------------------------------------------
    //  Wait until PLL locked
    //   Exit with error if emergency timer overflow
    //---------------------------------------------------------------
    timer_cntr = 0;
    while ((rgSCTL_PCIE_RST & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == PCIE_TEST_LIB_PLL_LOCK_DURATION)
            return -1;
    }
    
    
    //---------------------------------------------------------------
    //  Set options in PCIe controller
    //---------------------------------------------------------------
    if (usual_settings == 0)
    {
        if (high_lvl_loopback_mode != 0)
            rgPCIe_LocMgmt_i_pl_config_0_reg |= 0x80000000;
        //------------------------------------------------------
        //  Enable access through controller
        //------------------------------------------------------
        rgPCIe_RP_i_command_status = i_command_status;
        //------------------------------------------------------
        //  BAR {x} - what high bits of inbound address to mask
        //------------------------------------------------------
        rgPCIe_RP_i_RC_BAR_0 = i_RC_BAR_0;
        rgPCIe_RP_i_RC_BAR_1 = i_RC_BAR_1;
        //------------------------------------------------------
        //  Enable BAR {x} for inbound transactions
        //  also set BAR {x} size
        //------------------------------------------------------
        rgPCIe_LocMgmt_i_rc_BAR_config_reg = i_rc_BAR_config_reg;
        //------------------------------------------------------
        //  BAR {x} - what to put instead of masked bits
        //------------------------------------------------------
        rgPCIe_AXI_inregion_rc_bar_0_addr_translation = rc_bar_0_addr_translation;
        rgPCIe_AXI_inregion_rc_bar_1_addr_translation = rc_bar_1_addr_translation;
    }
    else
    {
        //------------------------------------------------------
        //  Enable access through controller
        //------------------------------------------------------
        rgPCIe_RP_i_command_status = 0x07;
        //------------------------------------------------------
        //  Enable BAR 0 for inbound transactions
        //   - BAR size selected - 4 GB
        //   - one BAR 0 cover all memory space
        //------------------------------------------------------
        rgPCIe_LocMgmt_i_rc_BAR_config_reg = 0x0000011E;
    }
    
    
    //---------------------------------------------------------------
    //  Probably, it will be better to make outbound address
    //    translation parameters as function arguments too.
    //  There are 32 regions, but only one is described here
    //---------------------------------------------------------------
    //------------------------------------------------------
    //     AXI outbound addr translation setup
    //       - pass 32 bits to bridge, zero none bits
    //------------------------------------------------------
    rgPCIe_AXI_outregion_0_addr_translation_0 = 0x0000001F;
    //------------------------------------------------------
    //     AXI outbound PCIe header descriptor
    //       - configure for memory RW
    //------------------------------------------------------
    rgPCIe_AXI_outregion_0_header_descriptor_0 = 0x00000002;
    //------------------------------------------------------
    //     AXI Outbound Region 0 AXI Region Base Address 0
    //       - none of bits choose region
    //------------------------------------------------------
    rgPCIe_AXI_outregion_0_region_base_addr = 0x0000001F;
    
    
    //---------------------------------------------------------------
    //  Enable Link Training
    //---------------------------------------------------------------
    rgSCTL_PCIE_REG_0 |= 0x00000010;
    
    //---------------------------------------------------------------
    //  Wait until training complete
    //   Exit with error if emergency timer overflow
    //---------------------------------------------------------------
    timer_cntr = 0;
    while ((rgPCIe_LocMgmt_i_pl_config_0_reg & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == PCIE_TEST_LIB_TRAINING_DURATION)
            return -1;
    }
    
    return 0;
}



