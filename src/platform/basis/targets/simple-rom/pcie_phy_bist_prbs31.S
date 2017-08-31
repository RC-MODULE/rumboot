
//-----------------------------------------------------------------------------
//  This program is for PCIe Phy testing by BIST
//
//    Test duration (RTL): < 170us
//-----------------------------------------------------------------------------

#include <basis/defs.h>

.global main

main:
    PUSH {R1-R7, LR}
    
/**********************************************************
 *  This is PCIe (or ELC) Phy bist test                   *
 *********************************************************/
PCIe_bist:
    /*tube_hex 00000000*/
    
    /***************************************************/
    /*    this PMA settings are only for simulation    */
    /*    on netlist, equal define SIM_SPEEDUP on RTL  */
    /***************************************************/
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_TX_RCVDET_CTRL
    LDR R2, =0x00FA
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_BANDGAP_TMR
    LDR R2, =0x0001
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_BIAS_TMR
    LDR R2, =0x0001
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_PLLEN_TMR
    LDR R2, =0x0001
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_PLLPRE_TMR
    LDR R2, =0x0001
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_PLLVREF_TMR
    LDR R2, =0x0001
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_PLLLOCK_TMR
    LDR R2, =0x0002
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_PLLCLKDIS_TMR
    LDR R2, =0x0001
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_LANECAL_TMR
    LDR R2, =0x0001
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_A0IN_TMR
    LDR R2, =0x0001
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_A1IN_TMR
    LDR R2, =0x0004
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_A2IN_TMR
    LDR R2, =0x0004
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_SSM_A3IN_TMR
    LDR R2, =0x0004
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_BGCAL_OVRD
    LDR R2, =0x8000
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_PLL_CPI_OVRD
    LDR R2, =0x8000
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_CMN_SDCAL0_OVRD
    LDR R2, =0x8000
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_CMN_SDCAL1_OVRD
    LDR R2, =0x8000
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_PLL_VCOCAL_OVRD
    LDR R2, =0x8012
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_PLL_VCOCAL_CNT_START
    LDR R2, =0x0010
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_PLL_LOCK_CNT_START
    LDR R2, =0x0010
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_IQPI_ILL_CAL_OVRD
    LDR R2, =0x8002
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_IQPI_ILL_LOCK_REFCNT_START
    LDR R2, =0x0010
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_IQPI_ILL_LOCK_CALCNT_START0
    LDR R2, =0x0010
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_IQPI_ILL_LOCK_CALCNT_START1
    LDR R2, =0x0010
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_EPI_ILL_CAL_OVRD
    LDR R2, =0x8002
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_EPI_ILL_LOCK_REFCNT_START
    LDR R2, =0x0010
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_EPI_ILL_LOCK_CALCNT_START0
    LDR R2, =0x0010
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_EPI_ILL_LOCK_CALCNT_START1
    LDR R2, =0x0010
    STR R2, [R1]
    
    /* LDR R1, =PCIE_PHY_BASE + PCIe_Phy_LANE_CAL_RESET_TIME_VALUE  */
    /* LDR R2, =                                              */
    /* STR R2, [R1]                                           */
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_TXPUCAL_OVRD
    LDR R2, =0x8022
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_TXPDCAL_OVRD
    LDR R2, =0x8022
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_RXCAL_OVRD
    LDR R2, =0x8003
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_SLC_INIT_TMR
    LDR R2, =0x0001
    STR R2, [R1]
    
    LDR R1, =PCIE_PHY_BASE + PCIe_Phy_MASSWR_RX_SLC_RUN_TMR
    LDR R2, =0x0001
    STR R2, [R1]

    /* LDR R1, =PCIE_PHY_BASE + PCIe_Phy_CMN_PRPLL_LOCK_CNT_START  */
    /* LDR R2, =                                             */
    /* STR R2, [R1]                                          */
    /***************************************************/
    /***************************************************/

    LDR R3, =0x0
    LDR R4, =0x0
pcie_bist_lane_cycle:
        /********** Initiate PMA reset **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_CMN_CTRL
        LDR R2, =0x0
        STR R2, [R1]
        
        /********** Set standard mode **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_MODE
        LDR R2, =0x95
        STR R2, [R1]
        
        /********** Setup transceiver controls to enable lanes but in electrical idle **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_XCVR_CTRL
        LDR R2, =0x1103
        STR R2, [R1]
        STR R2, [R1, #0x0400]
        STR R2, [R1, #0x0800]
        STR R2, [R1, #0x0C00]
        
        /********** Write to PMA lanes TD to set a repeating pattern **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_DATA_LO + 0x000
        ADD R1, R1, R3
        LDR R2, =0xCCCC
        STR R2, [R1]
        STR R2, [R1, #4]
        
        /********** Enable loopback for selected lane **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_LPBK + 0x000
        ADD R1, R1, R3
        LDR R2, =0x2
        STR R2, [R1]
        
        /********** Put everything into isolation mode (PMA) **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_ISOLATION_CTRL
        LDR R2, =0xD3FF
        STR R2, [R1]
        
        /********** Release reset and set up refclk selection **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_CMN_CTRL
        LDR R2, =0x0E33
        STR R2, [R1]
        
        /*tube_hex 00000001*/
        
        /********** Poll until PMA asserts cmn_ready **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_CTRL
Wait_pcie_pma_cmn_ready:
        LDR R2, [R1]
        AND R2, R2, #0x1
        CMP R2, #0x1
        BNE Wait_pcie_pma_cmn_ready
        
        /*tube_hex 00000002*/
        
        /********** Set power state request **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_PWRST_CTRL
        LDR R2, =0x1
        STR R2, [R1]
        
        /********** Poll until PMA responds with the power state acknowledge **********/
Wait_pcie_a0_power_state:
        LDR R2, [R1]
        AND R2, R2, #0x100
        CMP R2, #0x100
        BNE Wait_pcie_a0_power_state
        
        /*tube_hex 00000003*/
        
        /********** De-assert transceiver control **********/
        /********** electrical idle to begin transmission **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_XCVR_CTRL + 0x000
        ADD R1, R1, R3
        LDR R2, =0x0103
        STR R2, [R1]
        
        /********** Activate BIST testing **********/
        
        /********** Clear the BIST engines **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + 0x000
        ADD R1, R1, R4
        LDR R2, =0x0002
        STR R2, [R1]
        
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + 0x000
        ADD R1, R1, R4
        LDR R2, =0x0012
        STR R2, [R1]
        
        /********** Check the BIST error counter register **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_ERRCNT + 0x000
        ADD R1, R1, R4
        LDR R2, [R1]
        CMP R2, #0x0
        BNE pcie_test_failed
        
        /********** Activate BIST on TX side **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + 0x000
        ADD R1, R1, R4
        LDR R2, =0x0B01
        STR R2, [R1]
        
        /********** Check that BIST not yet synchronised **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_CTRL + 0x000
        ADD R1, R1, R3
        LDR R2, [R1]
        AND R2, R2, #0xE
        CMP R2, #0x0
        BNE pcie_test_failed
        
        /********** Activate BIST on RX side **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_SYNCCNT + 0x000
        ADD R1, R1, R4
        LDR R2, =0x00FA
        STR R2, [R1]
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + 0x000
        ADD R1, R1, R4
        LDR R2, =0x0B01
        STR R2, [R1]
        
        /********** Wait for BIST to synchronise **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_CTRL
        ADD R1, R1, R3
Wait_pcie_bist_synch:
        LDR R2, [R1]
        AND R2, R2, #0x2
        CMP R2, #0x2
        BNE Wait_pcie_bist_synch
        
        /*tube_hex 00000004*/
        
        /********** Clear BIST error counters **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + 0x000
        ADD R1, R1, R4
        LDR R2, =0x0B11
        STR R2, [R1]
        LDR R2, =0x0B01
        STR R2, [R1]
        
        /********** Check that BIST pins indicate no errors **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_CTRL + 0x000
        ADD R1, R1, R3
        LDR R2, [R1]
        AND R2, R2, #0xE
        CMP R2, #0x2
        BNE pcie_test_failed
        
        /********** Check the BIST error counter register **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_ERRCNT + 0x000
        ADD R1, R1, R4
        LDR R2, [R1]
        CMP R2, #0x0
        BNE pcie_test_failed
         
        /********** Force a BIST error **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + 0x000
        ADD R1, R1, R4
        LDR R2, =0x0B11
        STR R2, [R1]
        LDR R2, =0x0B01
        STR R2, [R1]
        
        /********** Check that an error is detected **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_CTRL
        ADD R1, R1, R3
Wait_pcie_error_detect:
        LDR R2, [R1]
        CMP R2, #0x0
        BEQ Wait_pcie_error_detect
        
        /*tube_hex 00000005*/
        
        /********** Check the BIST error counter register **********/
        /********** it must contain errors **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_ERRCNT + 0x000
        ADD R1, R1, R4
        LDR R2, [R1]
        CMP R2, #0x0
        BEQ pcie_test_failed
       
        /********** Now clear BIST errors and check status **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + 0x000
        ADD R1, R1, R4
        LDR R2, =0x0B11
        STR R2, [R1]
        LDR R2, =0x0B01
        STR R2, [R1]
        
        /********** Check that BIST pins indicate no errors **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_CTRL + 0x000
        ADD R1, R1, R3
        LDR R2, [R1]
        AND R2, R2, #0xE
        CMP R2, #0x2
        BNE pcie_test_failed
        
        /********** Check the BIST error counter register **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_ERRCNT + 0x000
        ADD R1, R1, R4
        LDR R2, [R1]
        CMP R2, #0x0
        BNE pcie_test_failed
       
        /********** De-activate isolation mode **********/
        LDR R1, =PCIE_PHY_BASE + PCIe_Phy_PMA_ISOLATION_CTRL
        LDR R2, =0x0000
        STR R2, [R1]
        
        /********** Test passed **********/
        LDR R0, =0x00000000
        
    ADD R3, R3, #0x400
    ADD R4, R4, #0x800
    CMP R3, #0x1000
    BEQ pcie_test_exit
    B pcie_bist_lane_cycle
    
pcie_test_exit:
    POP {R1-R7, PC}

pcie_test_failed:
    /*tube_hex 00000006*/
    LDR R0, =0xFFFFFFFF
    B pcie_test_exit
    