
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

#include <regs/regs_pcie.h>
#include <platform/devices.h>
#include <rumboot/io.h>

#include <rumboot/printf.h>

//-----------------------------------------------------------------------------
//  These defines must be changed for real program
//-----------------------------------------------------------------------------
#ifndef CMAKE_BUILD_TYPE_POSTPRODUCTION
#define PCIE_TEST_LIB_PLL_LOCK_TIMEOUT    0x1000
#define PCIE_TEST_LIB_TRAINING_TIMEOUT    0x1000
#else
#define PCIE_TEST_LIB_PLL_LOCK_TIMEOUT    0x1000000
#define PCIE_TEST_LIB_TRAINING_TIMEOUT    0x1000000
#endif



//-----------------------------------------------------------------------------
//  This function put PCIe subsystem into reset
//    Doesnt reset settings in SCTL and Sticky parameters in PCIe controller.
//    Doesnt send any reset-related messages by PCIe link.
//-----------------------------------------------------------------------------
void pcie_soft_reset ()
{
    iowrite32 (0x0, SCTL_BASE + SCTL_PCIE_RST);
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
#ifndef RUMBOOT_BUILD_TYPE_POSTPRODUCTION
#ifndef CMAKE_BUILD_TYPE_POSTPRODUCTION
    /***************************************************/
    /*    this PHY settings are only for simulation    */
    /*    and must be removed for real programm        */
    /***************************************************/
    //-------------------------------------------------------------------------
    //  These settings may be replaced with corresponding key on RTL simulation
    //    And they are neccessary in Netlist simulation
    //-------------------------------------------------------------------------
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
            return -1;
    }

    //---------------------------------------------------------------
    //  Wait until training complete
    //   Exit with error if emergency timer overflow
    //---------------------------------------------------------------
    timer_cntr = 0;
    while ((ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pl_config_0_reg) & 0x1) == 0)
    {
        timer_cntr++;
        if (timer_cntr == PCIE_TEST_LIB_TRAINING_TIMEOUT)
            return -2;
    }

    return 0;
}

//-----------------------------------------------------------------------------
//  
//-----------------------------------------------------------------------------
void pcie_display_regs ()
{
    rumboot_printf ("\n  debug pcie_display_regs\n");
    rumboot_printf ("    SCTL_PCIE_REG_0  0x%08x\n", ioread32 (SCTL_BASE + SCTL_PCIE_REG_0));
    rumboot_printf ("    SCTL_PCIE_REG_1  0x%08x\n", ioread32 (SCTL_BASE + SCTL_PCIE_REG_1));
    rumboot_printf ("    SCTL_PCIE_REG_2  0x%08x\n", ioread32 (SCTL_BASE + SCTL_PCIE_REG_2));
    rumboot_printf ("    PCIe_EP_i_vendor_id_device_id              0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_vendor_id_device_id            ));
    rumboot_printf ("    PCIe_EP_i_command_status                   0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_command_status                 ));
    rumboot_printf ("    PCIe_EP_i_revision_id_class_code           0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_revision_id_class_code         ));
    rumboot_printf ("    PCIe_EP_i_bist_header_latency_cache_line   0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_bist_header_latency_cache_line ));
    rumboot_printf ("    PCIe_EP_i_base_addr_0                      0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_0                    ));
    rumboot_printf ("    PCIe_EP_i_base_addr_1                      0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_1                    ));
    rumboot_printf ("    PCIe_EP_i_base_addr_2                      0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_2                    ));
    rumboot_printf ("    PCIe_EP_i_base_addr_3                      0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_3                    ));
    rumboot_printf ("    PCIe_EP_i_base_addr_4                      0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_4                    ));
    rumboot_printf ("    PCIe_EP_i_base_addr_5                      0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_5                    ));
    rumboot_printf ("    PCIe_EP_i_subsystem_vendor_id_subsystem_i  0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_subsystem_vendor_id_subsystem_i));
    rumboot_printf ("    PCIe_EP_i_capabilities_pointer             0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_capabilities_pointer           ));
    rumboot_printf ("    PCIe_EP_i_intrpt_line_intrpt_pin           0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_intrpt_line_intrpt_pin         ));
    rumboot_printf ("    PCIe_EP_i_pwr_mgmt_cap                     0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_pwr_mgmt_cap                   ));
    rumboot_printf ("    PCIe_EP_i_pwr_mgmt_ctrl_stat_rep           0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_pwr_mgmt_ctrl_stat_rep         ));
    rumboot_printf ("    PCIe_EP_i_msi_ctrl_reg                     0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msi_ctrl_reg                   ));
    rumboot_printf ("    PCIe_EP_i_msi_msg_low_addr                 0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msi_msg_low_addr               ));
    rumboot_printf ("    PCIe_EP_i_msi_msg_hi_addr                  0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msi_msg_hi_addr                ));
    rumboot_printf ("    PCIe_EP_i_msi_msg_data                     0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msi_msg_data                   ));
    rumboot_printf ("    PCIe_EP_i_msi_mask                         0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msi_mask                       ));
    rumboot_printf ("    PCIe_EP_i_msi_pending_bits                 0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msi_pending_bits               ));
    rumboot_printf ("    PCIe_EP_i_msix_ctrl                        0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl                      ));
    rumboot_printf ("    PCIe_EP_i_msix_tbl_offset                  0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_tbl_offset                ));
    rumboot_printf ("    PCIe_EP_i_msix_pending_intrpt              0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_pending_intrpt            ));
    rumboot_printf ("    PCIe_EP_i_pcie_cap_list                    0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_pcie_cap_list                  ));
    rumboot_printf ("    PCIe_EP_i_pcie_dev_cap                     0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_pcie_dev_cap                   ));
    rumboot_printf ("    PCIe_EP_i_pcie_dev_ctrl_status             0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_pcie_dev_ctrl_status           ));
    rumboot_printf ("    PCIe_EP_i_link_cap                         0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_link_cap                       ));
    rumboot_printf ("    PCIe_EP_i_link_ctrl_status                 0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_link_ctrl_status               ));
    rumboot_printf ("    PCIe_EP_i_pcie_dev_cap_2                   0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_pcie_dev_cap_2                 ));
    rumboot_printf ("    PCIe_EP_i_pcie_dev_ctrl_status_2           0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_pcie_dev_ctrl_status_2         ));
    rumboot_printf ("    PCIe_EP_i_link_cap_2_reg                   0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_link_cap_2_reg                 ));
    rumboot_printf ("    PCIe_EP_i_link_ctrl_status_2               0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_link_ctrl_status_2             ));
    rumboot_printf ("    PCIe_EP_i_AER_enhanced_cap_hdr             0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_AER_enhanced_cap_hdr           ));
    rumboot_printf ("    PCIe_EP_i_uncorr_err_status                0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_uncorr_err_status              ));
    rumboot_printf ("    PCIe_EP_i_uncorr_err_mask                  0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_uncorr_err_mask                ));
    rumboot_printf ("    PCIe_EP_i_uncorr_err_severity              0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_uncorr_err_severity            ));
    rumboot_printf ("    PCIe_EP_i_corr_err_status                  0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_corr_err_status                ));
    rumboot_printf ("    PCIe_EP_i_corr_err_mask                    0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_corr_err_mask                  ));
    rumboot_printf ("    PCIe_EP_i_advcd_err_cap_ctrl               0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_advcd_err_cap_ctrl             ));
    rumboot_printf ("    PCIe_EP_i_hdr_log_0                        0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_hdr_log_0                      ));
    rumboot_printf ("    PCIe_EP_i_hdr_log_1                        0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_hdr_log_1                      ));
    rumboot_printf ("    PCIe_EP_i_hdr_log_2                        0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_hdr_log_2                      ));
    rumboot_printf ("    PCIe_EP_i_hdr_log_3                        0x%08x\n", ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_hdr_log_3                      ));
    rumboot_printf ("\n");
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
    volatile uint32_t timer_cntr;
    volatile uint32_t rdata;


#ifndef PCIE_TEST_LIB_SIMSPEEDUP_OFF
#ifndef RUMBOOT_BUILD_TYPE_POSTPRODUCTION
#ifndef CMAKE_BUILD_TYPE_POSTPRODUCTION
    /***************************************************/
    /*    this PHY settings are only for simulation    */
    /*    and must be removed for real programm        */
    /***************************************************/
    //-------------------------------------------------------------------------
    //  These settings may be replaced with corresponding key on RTL simulation
    //    And they are neccessary in Netlist simulation
    //-------------------------------------------------------------------------
    rumboot_printf ("\n  Error: Simulation speedup On\n\n");
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


    //---------------------------------------------------------------
    //  Disable Link Training and set Gen1
    //---------------------------------------------------------------
    rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) & 0xFFFFFFEE;
    iowrite32 (rdata, SCTL_BASE + SCTL_PCIE_REG_0);
    //---------------------------------------------------------------
    //  Enable function change power auto-acknowledge
    //---------------------------------------------------------------
    rumboot_printf ("  Enable function change power auto-acknowledge (write 0x4 to SCTL_PCIE_REG_1)\n");
    rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_1);
    iowrite32 (rdata | (1 << 2), SCTL_BASE + SCTL_PCIE_REG_1);

    //---------------------------------------------------------------
    //  Set base options in SCTL
    //---------------------------------------------------------------
    if (usual_settings == 0)
    {
        rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) & 0xFFFFFFF0;
        iowrite32 (rdata | (sctl_base_opt & 0x0000000D), SCTL_BASE + SCTL_PCIE_REG_0);
    }

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
    rumboot_printf ("  PCIe PLL locked\n");


    //---------------------------------------------------------------
    //  Set options in PCIe controller
    //---------------------------------------------------------------
    if (usual_settings == 0)
    {
        //------------------------------------------------------
        //  Enable access through controller
        //------------------------------------------------------
        iowrite32 (i_command_status, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_command_status);
        //------------------------------------------------------
        //  BAR {x} - what high bits of inbound address to mask
        //------------------------------------------------------
        iowrite32 (i_base_addr_0, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_0);
        iowrite32 (i_base_addr_1, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_1);
        iowrite32 (i_base_addr_2, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_2);
        iowrite32 (i_base_addr_3, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_3);
        iowrite32 (i_base_addr_4, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_4);
        iowrite32 (i_base_addr_5, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_5);
        //------------------------------------------------------
        //  Enable BAR {x} for inbound transactions
        //  also set BAR {x} size
        //------------------------------------------------------
        iowrite32 (i_pf_0_BAR_config_0_reg, PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
        iowrite32 (i_pf_0_BAR_config_1_reg, PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_1_reg);
        //------------------------------------------------------
        //  BAR {x} - what to put instead of masked bits
        //------------------------------------------------------
        iowrite32 (ep_bar_0_addr_translation, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_0_addr_translation);
        iowrite32 (ep_bar_1_addr_translation, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_1_addr_translation);
        iowrite32 (ep_bar_2_addr_translation, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_2_addr_translation);
        iowrite32 (ep_bar_3_addr_translation, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_3_addr_translation);
        iowrite32 (ep_bar_4_addr_translation, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_4_addr_translation);
        iowrite32 (ep_bar_5_addr_translation, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_5_addr_translation);
    }
    else
    {
        //------------------------------------------------------
        //  Enable access through controller
        //------------------------------------------------------
        // iowrite32 (0x07, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_command_status);
        //------------------------------------------------------
        //  BAR 1 - what high bits of inbound address to mask
        //------------------------------------------------------
        // iowrite32 (0x80000000, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_base_addr_1);
        //------------------------------------------------------
        //  Enable BAR 0 and BAR 1 for inbound transactions
        //   - maximum BAR size selected - 2 GB
        //   - two BARs cover all memory space
        //------------------------------------------------------
        // iowrite32 (0x00009898, PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
        iowrite32 (0x00009292, PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pf_0_BAR_config_0_reg);
        //------------------------------------------------------
        //  BAR 1 - what to put instead of masked bits
        //------------------------------------------------------
        // iowrite32 (0x80000000, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_ep_bar_1_addr_translation);
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
    iowrite32 (0x0000001F, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_0);
    //------------------------------------------------------
    //     AXI outbound PCIe header descriptor
    //       - configure for memory RW
    //------------------------------------------------------
    iowrite32 (0x00000002, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_0);
    //------------------------------------------------------
    //     AXI Outbound Region 0 AXI Region Base Address 0
    //       - none of bits choose region
    //------------------------------------------------------
    iowrite32 (0x0000001F, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_region_base_addr);


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
            // break;
        }
    }
    rumboot_printf ("  PCIe link up\n");
    
    
    
    
    
    
    pcie_display_regs ();
    
    rumboot_printf ("  start debugging PCIe link. Read status 10_000_000 times. Report, if it changed.\n");
    
    rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pl_config_0_reg);
    rumboot_printf ("    initial status  (PCIe_LocMgmt_i_pl_config_0_reg)  0x%08x\n", rdata);
    
    volatile uint32_t rdata_1;
    volatile uint32_t link_status_changed = 0;
    
    for (uint32_t i = 0; i < 10000000; i++)
    {
        rdata_1 = ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pl_config_0_reg);
        
        if (rdata != rdata_1)
        {
            rumboot_printf ("    status changed to  0x%08x    time  %d\n", rdata_1, rumboot_platform_get_uptime());
            rdata = rdata_1;
            link_status_changed++;
            if (link_status_changed == 100)
            {
                rumboot_printf ("\n  Link status changed too many times. Exit this cycle.\n\n");
                break;
            }
        }
    }
    
    if (link_status_changed == 0)
        rumboot_printf ("    PCIe status looks stable\n");
    
    
    rumboot_printf ("    Manual ACK of function power state change (write 0x6 to SCTL_PCIE_REG_1)\n");
    rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_1);
    iowrite32 (rdata | (1 << 2) | (1 << 1), SCTL_BASE + SCTL_PCIE_REG_1);
    
    
    
    
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
    uint32_t rdata;


#ifndef PCIE_TEST_LIB_SIMSPEEDUP_OFF
#ifndef RUMBOOT_BUILD_TYPE_POSTPRODUCTION
#ifndef CMAKE_BUILD_TYPE_POSTPRODUCTION
    /***************************************************/
    /*    this PHY settings are only for simulation    */
    /*    and must be removed for real programm        */
    /***************************************************/
    //-------------------------------------------------------------------------
    //  These settings may be replaced with corresponding key on RTL simulation
    //    And they are neccessary in Netlist simulation
    //-------------------------------------------------------------------------
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


    //---------------------------------------------------------------
    //  Disable Link Training
    //---------------------------------------------------------------
    rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) & 0xFFFFFFEF;
    iowrite32 (rdata, SCTL_BASE + SCTL_PCIE_REG_0);

    //---------------------------------------------------------------
    //  Set base options in SCTL
    //---------------------------------------------------------------
    if (usual_settings == 0)
    {
        rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) & 0xFFFFFFF0;
        iowrite32 (rdata | (sctl_base_opt & 0x0000000F) | 0x2, SCTL_BASE + SCTL_PCIE_REG_0);
    }
    else
    {
        rdata = ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) | 0x2;
        iowrite32 (rdata, SCTL_BASE + SCTL_PCIE_REG_0);
    }

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
            return -1;
    }


    //---------------------------------------------------------------
    //  Turning high level loopback, if required
    //---------------------------------------------------------------
    if (high_lvl_loopback_mode != 0)
    {
        rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pl_config_0_reg) | 0x80000000;
        iowrite32 (rdata, PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_pl_config_0_reg);
    }
    //---------------------------------------------------------------
    //  Set options in PCIe controller
    //---------------------------------------------------------------
    if (usual_settings == 0)
    {
        //------------------------------------------------------
        //  Enable access through controller
        //------------------------------------------------------
        iowrite32 (i_command_status, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_RP_i_command_status);
        //------------------------------------------------------
        //  BAR {x} - what high bits of inbound address to mask
        //------------------------------------------------------
        iowrite32 (i_RC_BAR_0, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_RP_i_RC_BAR_0);
        iowrite32 (i_RC_BAR_1, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_RP_i_RC_BAR_1);
        //------------------------------------------------------
        //  Enable BAR {x} for inbound transactions
        //  also set BAR {x} size
        //------------------------------------------------------
        iowrite32 (i_rc_BAR_config_reg, PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_rc_BAR_config_reg);
        //------------------------------------------------------
        //  BAR {x} - what to put instead of masked bits
        //------------------------------------------------------
        iowrite32 (rc_bar_0_addr_translation, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_rc_bar_0_addr_translation);
        iowrite32 (rc_bar_1_addr_translation, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_inregion_rc_bar_1_addr_translation);
    }
    else
    {
        //------------------------------------------------------
        //  Enable access through controller
        //------------------------------------------------------
        iowrite32 (0x07, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_RP_i_command_status);
        //------------------------------------------------------
        //  Enable BAR 0 for inbound transactions
        //   - BAR size selected - 4 GB
        //   - one BAR 0 cover all memory space
        //------------------------------------------------------
        iowrite32 (0x0000011E, PCIE_CORE_BASE + PCIe_Core_LocalMgmt + PCIe_LocMgmt_i_rc_BAR_config_reg);
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
    iowrite32 (0x0000001F, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_0);
    //------------------------------------------------------
    //     AXI outbound PCIe header descriptor
    //       - configure for memory RW
    //------------------------------------------------------
    iowrite32 (0x00000002, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_0);
    //------------------------------------------------------
    //     AXI Outbound Region 0 AXI Region Base Address 0
    //       - none of bits choose region
    //------------------------------------------------------
    iowrite32 (0x0000001F, PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_region_base_addr);


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
        case 0 : iowrite32 (0, ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_ctrl); break;
        case 1 :
        {
            iowrite32 (0, ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_ctrl);
            for (uint32_t i = 0; i < 128; i++)
            {
                *(addr_pointer++)  = (uint32_t) 0x02000000*i | 0x1;
                *(addr_pointer+=2) = (uint32_t) 0x02000000*i + 0x01FFF000;
            }
            break;
        }
        case 2 :
        {
            iowrite32 (0, ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_ctrl);
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
        case 0 : iowrite32 (0, ADDR_TRANS_MST_BASE + ADDR_TRANS_MST_ctrl); break;
        case 1 :
        {
            iowrite32 (0x00000000 | 0x1, ADDR_TRANS_MST_BASE + ADDR_TRANS_MST_region0_base);
            iowrite32 (0x3FFFF000, ADDR_TRANS_MST_BASE + ADDR_TRANS_MST_region0_end);

            iowrite32 (0, ADDR_TRANS_MST_BASE + ADDR_TRANS_MST_ctrl);
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
//  This function must be used after PCIe turn On, otherwise APB bus will buzz
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
    uint32_t rdata;

    iowrite32 (Ctrl             , EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl             );
    iowrite32 (Global_IRQ_Mask_l, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
    iowrite32 (Global_IRQ_Mask_h, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
    
    rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
    if (Ctrl & 0x2)
        iowrite32 (rdata | 0x80000000, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
    else
        iowrite32 (rdata & ~0x80000000, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
    
    rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
    if (Ctrl & 0x4)
        iowrite32 (rdata | 0x40000000, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
    else
        iowrite32 (rdata & ~0x40000000, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
    
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

//-----------------------------------------------------------------------------
//  This function configures PCIe for functional tests "from HOST".
//    But here HOST is our SoC and all transactions go through external mirror.
//-----------------------------------------------------------------------------
uint32_t pcie_mirror_tests_setup ()
{
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
        return -1;
    
    //-----------------------------------------------------------------
    //  Setup Outbound Address Translator next way:
    //
    //    0x4000_0000 .. 0x43FF_FFFF    maps to    0x0000_0000 .. 0x03FF_FFFF
    //      it covers 64 MByte - eSRAM0, eSRAM1 and Periph registers
    //    
    //    0x5000_0000 .. 0x5FFF_FFFF    maps to    0x8000_0000 .. 0x8FFF_FFFF
    //      it covers 256 MByte - DDR0
    //
    //    0x6000_0000 .. 0x6FFF_FFFF    maps to    0xC000_0000 .. 0xCFFF_FFFF
    //      it covers 256 MByte - DDR1
    //
    //                         !!!  Attention !!!
    //    transactions to not defined addresses in range 
    //    0x4000_0000 .. 0x7FFF_FFFF will cause interruption
    //-----------------------------------------------------------------
    iowrite32 (0x40000000 | 0x1, ADDR_TRANS_MST_BASE + ((0 * 3 + 1) << 2)) ;  //  base address, enable
    iowrite32 (0x43FFFFFF      , ADDR_TRANS_MST_BASE + ((0 * 3 + 2) << 2)) ;  //  end address
    iowrite32 (0xC0000000      , ADDR_TRANS_MST_BASE + ((0 * 3 + 3) << 2)) ;  //  translate address
    
    iowrite32 (0x50000000 | 0x1, ADDR_TRANS_MST_BASE + ((1 * 3 + 1) << 2)) ;  //  base address, enable
    iowrite32 (0x5FFFFFFF      , ADDR_TRANS_MST_BASE + ((1 * 3 + 2) << 2)) ;  //  end address
    iowrite32 (0x30000000      , ADDR_TRANS_MST_BASE + ((1 * 3 + 3) << 2)) ;  //  translate address
    
    iowrite32 (0x60000000 | 0x1, ADDR_TRANS_MST_BASE + ((2 * 3 + 1) << 2)) ;  //  base address, enable
    iowrite32 (0x6FFFFFFF      , ADDR_TRANS_MST_BASE + ((2 * 3 + 2) << 2)) ;  //  end address
    iowrite32 (0x60000000      , ADDR_TRANS_MST_BASE + ((2 * 3 + 3) << 2)) ;  //  translate address
    
    iowrite32 (0, ADDR_TRANS_MST_BASE);
    
    return 0;
}

