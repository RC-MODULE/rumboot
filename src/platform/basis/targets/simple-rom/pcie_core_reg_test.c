
//-----------------------------------------------------------------------------
//  This program is for testing PCIe Core registers
//  It must show, that Core APB bus works correctly.
//  It includes:
//    - checking default values of some registers
//    - read/write check of some registers
//
//    Test duration (RTL): < 310us
//-----------------------------------------------------------------------------
#include <platform/defs_c.h>

#define def_PCIe_EP_i_vendor_id_device_id                  0x010017CD
#define def_PCIe_EP_i_command_status                       0x00100007
#define def_PCIe_EP_i_revision_id_class_code               0x00000000
#define def_PCIe_EP_i_base_addr_0                          0x00000000
#define def_PCIe_EP_i_base_addr_4                          0x00000000
#define def_PCIe_EP_i_pwr_mgmt_cap                         0x5A039001
#define def_PCIe_EP_i_AER_enhanced_cap_hdr                 0x00020001

#define def_PCIe_LocMgmt_i_pl_config_1_reg                 0x40808000
#define def_PCIe_LocMgmt_i_dll_tmr_config_reg              0x00000000
#define def_PCIe_LocMgmt_i_rcv_cred_lim_1_reg              0x00000020
#define def_PCIe_LocMgmt_i_L1_st_reentry_delay_reg         0x00000000
#define def_PCIe_LocMgmt_i_shdw_hdr_log_0_reg              0x00000000
#define def_PCIe_LocMgmt_i_negotiated_lane_map_reg         0x0000000F
#define def_PCIe_LocMgmt_i_ecc_corr_err_count_reg_axi      0x00000000

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

int check_read_default_value ()
{
    if (
        (rgPCIe_EP_i_vendor_id_device_id                      != def_PCIe_EP_i_vendor_id_device_id                      ) ||
        (rgPCIe_EP_i_command_status                           != def_PCIe_EP_i_command_status                           ) ||
        (rgPCIe_EP_i_revision_id_class_code                   != def_PCIe_EP_i_revision_id_class_code                   ) ||
        (rgPCIe_EP_i_base_addr_0                              != def_PCIe_EP_i_base_addr_0                              ) ||
        (rgPCIe_EP_i_base_addr_4                              != def_PCIe_EP_i_base_addr_4                              ) ||
        (rgPCIe_EP_i_pwr_mgmt_cap                             != def_PCIe_EP_i_pwr_mgmt_cap                             ) ||
        (rgPCIe_EP_i_AER_enhanced_cap_hdr                     != def_PCIe_EP_i_AER_enhanced_cap_hdr                     ) ||
        (rgPCIe_LocMgmt_i_pl_config_1_reg                     != def_PCIe_LocMgmt_i_pl_config_1_reg                     ) ||
        (rgPCIe_LocMgmt_i_dll_tmr_config_reg                  != def_PCIe_LocMgmt_i_dll_tmr_config_reg                  ) ||
        (rgPCIe_LocMgmt_i_rcv_cred_lim_1_reg                  != def_PCIe_LocMgmt_i_rcv_cred_lim_1_reg                  ) ||
        (rgPCIe_LocMgmt_i_L1_st_reentry_delay_reg             != def_PCIe_LocMgmt_i_L1_st_reentry_delay_reg             ) ||
        (rgPCIe_LocMgmt_i_shdw_hdr_log_0_reg                  != def_PCIe_LocMgmt_i_shdw_hdr_log_0_reg                  ) ||
        (rgPCIe_LocMgmt_i_negotiated_lane_map_reg             != def_PCIe_LocMgmt_i_negotiated_lane_map_reg             ) ||
        (rgPCIe_LocMgmt_i_ecc_corr_err_count_reg_axi          != def_PCIe_LocMgmt_i_ecc_corr_err_count_reg_axi          )
    )
        return -1;
    return 0;
}

int check_write_read_value ()
{
    for (int i = 0x03E0; i >= 0x0000; i-=0x20)
    {
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_0  + i) = 0xFFFFFFFF;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_1  + i) = 0xFFFFFFFF;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_0 + i) = 0xFFFFFFFF;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_1 + i) = 0xFFFFFFFF;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_region_base_addr    + i) = 0xFFFFFFFF;
    }
    for (int i = 0x03E0; i >= 0x0000; i-=0x20)
    {
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_0  + i) != 0xFFFFFF3F) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_1  + i) != 0xFFFFFFFF) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_0 + i) != 0xFFFFFFFF) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_1 + i) != 0xFFFFFFFF) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_region_base_addr    + i) != 0xFFFFFFFF) return -1;
    }
    for (int i = 0x0000; i <= 0x03E0; i+=0x20)
    {
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_0  + i) = 0x55555555;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_1  + i) = 0x55555555;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_0 + i) = 0x55555555;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_1 + i) = 0x55555555;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_region_base_addr    + i) = 0x55555555;
    }
    for (int i = 0x03E0; i >= 0x0000; i-=0x20)
    {
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_0  + i) != 0x55555515) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_1  + i) != 0x55555555) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_0 + i) != 0x55555555) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_1 + i) != 0x55555555) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_region_base_addr    + i) != 0x55555555) return -1;
    }
    for (int i = 0x03E0; i >= 0x0000; i-=0x20)
    {
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_0  + i) = 0xAAAAAAAA;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_1  + i) = 0xAAAAAAAA;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_0 + i) = 0xAAAAAAAA;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_1 + i) = 0xAAAAAAAA;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_region_base_addr    + i) = 0xAAAAAAAA;
    }
    for (int i = 0x0000; i <= 0x03E0; i+=0x20)
    {
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_0  + i) != 0xAAAAAA2A) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_1  + i) != 0xAAAAAAAA) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_0 + i) != 0xAAAAAAAA) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_1 + i) != 0xAAAAAAAA) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_region_base_addr    + i) != 0xAAAAAAAA) return -1;
    }
    for (int i = 0x0000; i <= 0x03E0; i+=0x20)
    {
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_0  + i) = 0x00000000;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_1  + i) = 0x00000000;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_0 + i) = 0x00000000;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_1 + i) = 0x00000000;
        *(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_region_base_addr    + i) = 0x00000000;
    }
    for (int i = 0x0000; i <= 0x03E0; i+=0x20)
    {
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_0  + i) != 0x00000000) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_addr_translation_1  + i) != 0x00000000) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_0 + i) != 0x00000000) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_header_descriptor_1 + i) != 0x00000000) return -1;
        if(*(volatile int*) (PCIE_CORE_BASE + PCIe_Core_AXIConfig + PCIe_AXI_outregion_0_region_base_addr    + i) != 0x00000000) return -1;
    }
    return 0;
}

int main ()
{
    PCIe_setting_ ();
    if (check_read_default_value () != 0)
        return -1;
    if (check_write_read_value () != 0)
        return -1;

    return 0;
}
