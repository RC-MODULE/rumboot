#ifndef PCIE_TEST_LIB_H
#define PCIE_TEST_LIB_H

#include <stdint.h>

#include <platform/defs_c.h>

/**
 *
 * \defgroup PCIe Example Functions
 * \ingroup devices_pcie
 *
 * \code{.c}
 * #include <rumboot/pcie_test_lib.h>
 * \endcode
 * Functions in this group are used for PCIe subsystem testing
 *
 *  @{
 */


    /**
     * This function put PCIe subsystem into reset
     *   Doesnt reset settings in SCTL and Sticky parameters in PCIe controller.
     *   Doesnt send any reset-related messages by PCIe link.
     */
    void pcie_soft_reset ();

    /**
     * This function is for turning ON one of PCIe subsystem loopback modes
     *   It influence only on Phy.
     *   To make controller to finish training in loopback choose this option
     *     in turn ON function (or set neccessasry parameter manually)
     *
     * @param loopback_mode [description]
     */
    void pcie_loopback_mode_on (uint32_t loopback_mode);

    /**
     * This function is for PCIe simple turn On.
     * All settings are default.
     */
    uint32_t pcie_simple_turn_on ();

    /**
     * This function is for PCIe turn On in EndPoint mode with options
     *   Very few options are used here. Look documentation for full list.
     *
     * @param usual_settings            [description]
     * @param sctl_base_opt             [description]
     * @param i_command_status          [description]
     * @param i_base_addr_0             [description]
     * @param i_base_addr_1             [description]
     * @param i_base_addr_2             [description]
     * @param i_base_addr_3             [description]
     * @param i_base_addr_4             [description]
     * @param i_base_addr_5             [description]
     * @param i_pf_0_BAR_config_0_reg   [description]
     * @param i_pf_0_BAR_config_1_reg   [description]
     * @param ep_bar_0_addr_translation [description]
     * @param ep_bar_1_addr_translation [description]
     * @param ep_bar_2_addr_translation [description]
     * @param ep_bar_3_addr_translation [description]
     * @param ep_bar_4_addr_translation [description]
     * @param ep_bar_5_addr_translation [description]
     * @return pass
     */
    uint32_t pcie_turn_on_with_options_ep
    (
        uint8_t usual_settings              ,
        uint32_t sctl_base_opt              ,
        uint32_t i_command_status           ,
        uint32_t i_base_addr_0              ,
        uint32_t i_base_addr_1              ,
        uint32_t i_base_addr_2              ,
        uint32_t i_base_addr_3              ,
        uint32_t i_base_addr_4              ,
        uint32_t i_base_addr_5              ,
        uint32_t i_pf_0_BAR_config_0_reg    ,
        uint32_t i_pf_0_BAR_config_1_reg    ,
        uint32_t ep_bar_0_addr_translation  ,
        uint32_t ep_bar_1_addr_translation  ,
        uint32_t ep_bar_2_addr_translation  ,
        uint32_t ep_bar_3_addr_translation  ,
        uint32_t ep_bar_4_addr_translation  ,
        uint32_t ep_bar_5_addr_translation
    );

    /**
     * This function is for PCIe turn On in RootPort mode with options
     *   Very few options are used here. Look documentation for full list.
     *
     * @param usual_settings            [description]
     * @param high_lvl_loopback_mode    [description]
     * @param sctl_base_opt             [description]
     * @param i_command_status          [description]
     * @param i_RC_BAR_0                [description]
     * @param i_RC_BAR_1                [description]
     * @param i_rc_BAR_config_reg       [description]
     * @param rc_bar_0_addr_translation [description]
     * @param rc_bar_1_addr_translation [description]
     * @return pass
     */
    uint32_t pcie_turn_on_with_options_rc
    (
        uint8_t usual_settings              ,
        uint8_t high_lvl_loopback_mode      ,
        uint32_t sctl_base_opt              ,
        uint32_t i_command_status           ,
        uint32_t i_RC_BAR_0                 ,
        uint32_t i_RC_BAR_1                 ,
        uint32_t i_rc_BAR_config_reg        ,
        uint32_t rc_bar_0_addr_translation  ,
        uint32_t rc_bar_1_addr_translation
    );

    /**
     * This function is for Outbound Address Translator simple configuration
     *   Very few options are used here. Look documentation for full list.
     *
     * @param config_type            [description]
     */
    void addr_trans_slv_config
    (
        uint8_t config_type
    );

    /**
     * This function is for Inbound Address Translator simple configuration
     *   Very few options are used here. Look documentation for full list.
     *
     * @param config_type            [description]
     */
    void addr_trans_mst_config
    (
        uint8_t config_type
    );

    /**
     * This function is for External IRQ Generator configuration
     *   Very few options are used here. Look documentation for full list.
     *
     * @param Ctrl            [description]
     * @param Global_IRQ_Mask_h            [description]
     * @param Global_IRQ_Mask_l            [description]
     */
    void ext_irq_gen_config
    (
        uint32_t Ctrl                  ,
        uint32_t Global_IRQ_Mask_h     ,
        uint32_t Global_IRQ_Mask_l
    );

/** @}*/

typedef volatile struct __attribute__ ((__packed__))
{
    //-----------------------------------------------------
    //      [31:0]    AXI Base Address
    //-----------------------------------------------------
    uint32_t   axi_base_addr         : 32;
    //-----------------------------------------------------
    //  AXI Address Phase (AR or AW) controls
    //      [31:8]   Reserved
    //      [7]      AxLOCK
    //      [6:3]    AxCACHE
    //      [2:0]    AxPROT
    //-----------------------------------------------------
    uint32_t   axi_addr_phase        : 32;
    //-----------------------------------------------------
    //      [63:0] PCIe Base Address
    //-----------------------------------------------------
    uint64_t   pcie_base_addr        : 64;
    //-----------------------------------------------------
    //  PCIe TLP Header Attributes
    //      [31:26]  Reserved
    //      [25:10]  Requester ID
    //      [9]      Use 25:10 as Requester ID
    //      [8:6]    Reserved
    //      [5:3]    PCIe Transfer Class
    //      [2]      ID-Based Ordering
    //      [1]      Relaxed Ordering
    //      [0]      No Snoop
    //-----------------------------------------------------
    uint32_t   tlp_header_att        : 32;
    //-----------------------------------------------------
    //      [23:0] Length of transfer in bytes (0 indicates
    //         maximum length transfer 2^24 bytes)
    //-----------------------------------------------------
    uint32_t   transfer_length       : 24;
    //-----------------------------------------------------
    //  Control Byte
    //      [7:6]    Reserved
    //      [5]      Continue to execute linked-list
    //      [4:3]    Reserved
    //      [2:1]    Continuity (bulk | scatter/gather)
    //      [0]      Interrupt after execution of descriptor
    //-----------------------------------------------------
    uint8_t    control_byte          :  8;
    //-----------------------------------------------------
    //  AXI Bus Status
    //      [7:3]    Reserved
    //      [2]      Internal data integrity error detected
    //      [1:0]    BRESP[1:0] or RRESP[1:0]
    //-----------------------------------------------------
    uint8_t    axi_bus_status        :  8;
    //-----------------------------------------------------
    //  PCIe Bus Status
    //      [7:4]    Reserved
    //      [3:0]    TLP Completion status or internal error
    //        0x0      Normal completion
    //        !=0      Completion with error (look spec)
    //-----------------------------------------------------
    uint8_t    pcie_bus_status       :  8;
    //-----------------------------------------------------
    //  Channel Status
    //      [7]      Buffer Not Empty
    //      [6]      Buffer Underflow
    //      [5]      Buffer Overflow
    //      [4]      Descriptor Error
    //      [3]      Data Integrity Error
    //      [2]      AXI transfer completed early with incomplete data
    //      [1]      PCIe transfer completed early with incomplete data
    //      [0]      Descriptor action completed
    //-----------------------------------------------------
    uint8_t    channel_status        :  8;
    //-----------------------------------------------------
    //      [7:0] Reserved
    //-----------------------------------------------------
    uint8_t    reserved              :  8;
    //-----------------------------------------------------
    //      [31:0] Pointer to Next Descriptor in linked-list
    //-----------------------------------------------------
    uint32_t   next_desc_pointer     : 32;
} PCIe_DMA_desc;


#endif /* end of include guard: PCIE_TEST_LIB_H */
