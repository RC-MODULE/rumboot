#ifndef PCIE_TEST_LIB_H
#define PCIE_TEST_LIB_H

/**
 *
 * \defgroup PCIe test library
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
    void pcie_loopback_mode_on (int loopback_mode);
    
    /**
     * This function is for PCIe simple turn On.
     * All settings are default.
     */
    int pcie_simple_turn_on ();
    
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
     */
    int pcie_turn_on_with_options_ep
    (
        char usual_settings            ,
        int sctl_base_opt              ,
        int i_command_status           ,
        int i_base_addr_0              ,
        int i_base_addr_1              ,
        int i_base_addr_2              ,
        int i_base_addr_3              ,
        int i_base_addr_4              ,
        int i_base_addr_5              ,
        int i_pf_0_BAR_config_0_reg    ,
        int i_pf_0_BAR_config_1_reg    ,
        int ep_bar_0_addr_translation  ,
        int ep_bar_1_addr_translation  ,
        int ep_bar_2_addr_translation  ,
        int ep_bar_3_addr_translation  ,
        int ep_bar_4_addr_translation  ,
        int ep_bar_5_addr_translation
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
     */
    int pcie_turn_on_with_options_rc
    (
        char usual_settings            ,
        char high_lvl_loopback_mode    ,
        int sctl_base_opt              ,
        int i_command_status           ,
        int i_RC_BAR_0                 ,
        int i_RC_BAR_1                 ,
        int i_rc_BAR_config_reg        ,
        int rc_bar_0_addr_translation  ,
        int rc_bar_1_addr_translation
    );

/** @}*/


#endif /* end of include guard: PCIE_TEST_LIB_H */
