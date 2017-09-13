
//-----------------------------------------------------------------------------
//  This file contain PCIe related functions, used for testing.
//  It includes next functions:
//    - controller software reset
//    - internal loopback On
//    - simple turn On
//    - turn On with options (EndPoint mode)
//    - turn On with options (RootPort mode)
//    - switch freq ???
//    - slave AT settings
//    - master AT settings
//    - ext_irq_gen settings
//
//      !!! Some functions includes PCIe Speedup for simulation !!!
//      !!!         It must be removed in real program          !!!
//
//-----------------------------------------------------------------------------
#include <stdint.h>

#include <basis/defs_c.h>

//-----------------------------------------------------------------------------
//  These defines must be changed for real program
//-----------------------------------------------------------------------------
#define PCIE_TEST_LIB_PLL_LOCK_TIMEOUT    0x1000
#define PCIE_TEST_LIB_TRAINING_TIMEOUT    0x1000


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
void pcie_loopback_mode_on (uint32_t loopback_mode)
{
    (*(uint32_t*) (PCIe_Phy_PMA_XCVR_LPBK + 0x000 + PCIE_PHY_BASE)) = loopback_mode;
    (*(uint32_t*) (PCIe_Phy_PMA_XCVR_LPBK + 0x400 + PCIE_PHY_BASE)) = loopback_mode;
    (*(uint32_t*) (PCIe_Phy_PMA_XCVR_LPBK + 0x800 + PCIE_PHY_BASE)) = loopback_mode;
    (*(uint32_t*) (PCIe_Phy_PMA_XCVR_LPBK + 0xC00 + PCIE_PHY_BASE)) = loopback_mode;
}

//-----------------------------------------------------------------------------
//  This function is for PCIe simple turn On.
//  All settings are default.
//    It includes:
//      - PCIe reset Off
//      - Wait until training complete
//        - Exit with error if emergency timer overflow
//-----------------------------------------------------------------------------
uint32_t pcie_simple_turn_on ()
{
    uint32_t timer_cntr;
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
        if (timer_cntr == PCIE_TEST_LIB_PLL_LOCK_TIMEOUT)
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
        if (timer_cntr == PCIE_TEST_LIB_TRAINING_TIMEOUT)
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
uint32_t pcie_turn_on_with_options_ep
(
    uint8_t usual_settings               ,
    uint32_t  sctl_base_opt              ,
    uint32_t  i_command_status           ,
    uint32_t  i_base_addr_0              ,
    uint32_t  i_base_addr_1              ,
    uint32_t  i_base_addr_2              ,
    uint32_t  i_base_addr_3              ,
    uint32_t  i_base_addr_4              ,
    uint32_t  i_base_addr_5              ,
    uint32_t  i_pf_0_BAR_config_0_reg    ,
    uint32_t  i_pf_0_BAR_config_1_reg    ,
    uint32_t  ep_bar_0_addr_translation  ,
    uint32_t  ep_bar_1_addr_translation  ,
    uint32_t  ep_bar_2_addr_translation  ,
    uint32_t  ep_bar_3_addr_translation  ,
    uint32_t  ep_bar_4_addr_translation  ,
    uint32_t  ep_bar_5_addr_translation

)
{
    uint32_t timer_cntr;
    
    
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
        if (timer_cntr == PCIE_TEST_LIB_PLL_LOCK_TIMEOUT)
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
        if (timer_cntr == PCIE_TEST_LIB_TRAINING_TIMEOUT)
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
uint32_t pcie_turn_on_with_options_rc
(
    uint8_t usual_settings               ,
    uint8_t high_lvl_loopback_mode       ,
    uint32_t  sctl_base_opt              ,
    uint32_t  i_command_status           ,
    uint32_t  i_RC_BAR_0                 ,
    uint32_t  i_RC_BAR_1                 ,
    uint32_t  i_rc_BAR_config_reg        ,
    uint32_t  rc_bar_0_addr_translation  ,
    uint32_t  rc_bar_1_addr_translation

)
{
    uint32_t timer_cntr;
    
    
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
    else
        rgSCTL_PCIE_REG_0 |= 0x2;
    
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
        if (timer_cntr == PCIE_TEST_LIB_PLL_LOCK_TIMEOUT)
            return -1;
    }
    
    
    //---------------------------------------------------------------
    //  Turning high level loopback, if required
    //---------------------------------------------------------------
    if (high_lvl_loopback_mode != 0)
        rgPCIe_LocMgmt_i_pl_config_0_reg |= 0x80000000;
    //---------------------------------------------------------------
    //  Set options in PCIe controller
    //---------------------------------------------------------------
    if (usual_settings == 0)
    {
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
        if (timer_cntr == PCIE_TEST_LIB_TRAINING_TIMEOUT)
            return -1;
    }
    
    return 0;
}

//-----------------------------------------------------------------------------
//  This function is for Outbound Address Translator simple configuration
//    It includes:
//      - 
//
//  Arguments:
//    - config_type
//        0:
//          bypass disable, interrupts enable, no other settings
//          all transactions should be error
//        1:
//          bypass disable, interrupts enable, 
//          devide all 32 bit space to equal regions, no address change
//        2:
//          bypass disable, interrupts enable
//          0 region cover all space, except last 4K, that could be used to
//          generate interruptions. Address -= 1G (to have access to eSRAM).
//          Usefull mode for debug purposes.
//      
//-----------------------------------------------------------------------------
void addr_trans_slv_config
(
    uint8_t config_type
)
{
    volatile uint32_t* addr_pointer = (uint32_t*) (ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_region0_base);
    
    switch (config_type)
    {
        case 0 : rgADDR_TRANS_SLV_ctrl = 0; break;
        case 1 : 
        {
            rgADDR_TRANS_SLV_ctrl = 0;
            for (uint32_t i = 0; i < 128; i++)
            {
                *(addr_pointer++)  = (uint32_t) 0x02000000*i | 0x1;
                *(addr_pointer+=2) = (uint32_t) 0x02000000*i + 0x01FFF000;
            }
            break;
        }
        case 2 : 
        {
            rgADDR_TRANS_SLV_ctrl = 0;
            *(addr_pointer++) = (uint32_t) 0x40000000 | 0x1;
            *(addr_pointer++) = (uint32_t) 0x7FFFE000;
            *(addr_pointer++) = (uint32_t) 0xC0000000;
        }
    }
}

//-----------------------------------------------------------------------------
//  This function is for Inbound Address Translator simple configuration
//    It includes:
//      - 
//
//  Arguments:
//    - config_type
//        0:
//          bypass disable, interrupts enable, no other settings
//          all transactions should be error
//        1:
//          bypass disable, interrupts enable, 
//          first 2 Gb to region 0
//          second 2 Gb to (region 1 .. region 8) equal size
//          no address change
//      
//-----------------------------------------------------------------------------
void addr_trans_mst_config
(
    uint8_t config_type
)
{
    volatile uint32_t* addr_pointer = (uint32_t*) (ADDR_TRANS_MST_BASE + ADDR_TRANS_MST_region1_base);
    
    switch (config_type)
    {
        case 0 : rgADDR_TRANS_MST_ctrl = 0; break;
        case 1 : 
        {
            rgADDR_TRANS_MST_region0_base = (uint32_t) 0x00000000 | 0x1; 
            rgADDR_TRANS_MST_region0_end =  (uint32_t) 0x3FFFF000;
            
            rgADDR_TRANS_MST_ctrl = 0;
            for (uint32_t i = 0; i < 8; i++)
            {
                *(addr_pointer++)  = (uint32_t) 0x02000000*i | 0x1;
                *(addr_pointer+=2) = (uint32_t) 0x02000000*i + 0x01FFF000;
            }
            break;
        }
    }
}

//-----------------------------------------------------------------------------
//  This function is for External IRQ Generator configuration
//  Now this function realise only default MSIX mapping
//  Now this function realise only hardcoded MSIX vectors
//    MSIX vectors: addr           data
//                  0x4005_0000    1
//                  0x4005_0004    2
//                      ...
//                  0x4005_0100    64
//      Such addresses make access to eSRAM through PCIe with mirrored
//      external Root Port
//      
//  Arguments:
//    - Ctrl
//        [0]    Ext_Int_En 
//        [1]    MSIX_Int_En
//        [2]    MSIX_Int_Mask
//        [3]    Legacy_Int_En 
//        [4]    Legacy_Int_Mask 
//        [5]    Legacy_Int_Ack 
//        [31:6] Reserved
//        
//    - Global_IRQ_Mask_h
//    - Global_IRQ_Mask_l
//      
//-----------------------------------------------------------------------------
void ext_irq_gen_config
(
    uint32_t Ctrl                  ,
    uint32_t Global_IRQ_Mask_h     ,
    uint32_t Global_IRQ_Mask_l
)
{
    rgEXT_IRQ_GEN_Ctrl              = Ctrl              ;
    rgEXT_IRQ_GEN_Global_IRQ_Mask_l = Global_IRQ_Mask_h ;
    rgEXT_IRQ_GEN_Global_IRQ_Mask_h = Global_IRQ_Mask_l ;
    if (Ctrl & 0x2)
        rgPCIe_EP_i_msix_ctrl |= 0x80000000;
    else
        rgPCIe_EP_i_msix_ctrl &= ~0x80000000;
    if (Ctrl & 0x4)
        rgPCIe_EP_i_msix_ctrl |= 0x40000000;
    else
        rgPCIe_EP_i_msix_ctrl &= ~0x40000000;
    
    volatile uint32_t* addr_pointer = (uint32_t*) (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address);
    for (int i = 0; i < 64; i++)
    {
        *addr_pointer = 0x40050000 + (i << 2);
        addr_pointer+=2;
        *addr_pointer = i;
        addr_pointer++;
        *addr_pointer = 0;
        addr_pointer++;
    }
}



