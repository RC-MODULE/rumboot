//-----------------------------------------------------------------------------
//  This program is for achieving PCIe maximum power consumption
//  
//  if there is no opposite device, use only PCIe Phy in BIST mode
//    (then MAC wont turn if off)
//  
//  It includes:
//    - 
//
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/pcie_test_lib.h>
#include <regs/regs_pcie.h>
#include <platform/devices.h>

//-----------------------------------------------------------------------------
//  Test parameters. Can be changed, if You need other functionality.
//-----------------------------
// #define PCIE_GEN_1
#define PCIE_GEN_2
//-----------------------------------------------------------------------------

#ifdef PCIE_GEN_1
#define PCIe_Phy_PMA_ISO_MODE__VALUE 0x95
#endif
#ifdef PCIE_GEN_2
#define PCIe_Phy_PMA_ISO_MODE__VALUE 0xA5
#endif

#define ISI_LOOPBACK        0x02
#define SERIAL_LOOPBACK     0x01
#define NE_LOOPBACK         0x10

#define LANE_0_BASE         0x000
#define LANE_1_BASE         0x400
#define LANE_2_BASE         0x800
#define LANE_3_BASE         0xC00

#define COMMON_TIMEOUT  10000000

//-----------------------------------------------------------------------------
//  Common functions
//-----------------------------
void delay_cycle (volatile uint32_t delay)
{
    while (delay != 0)
        delay--;
}

//-----------------------------------------------------------------------------
void simulation_speedup ()
{
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
#endif
#endif
#endif
}

uint32_t pcie_turn_on_bist_all_lanes (uint32_t mode)
{
    volatile uint32_t timeout_cntr;
    
    rumboot_printf ("  pcie_turn_on_bist_all_lanes start\n");
    //---------------------------------------------------------------------------------------------
    /********** Initiate PMA reset **********/
    iowrite32 (0x0, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_CMN_CTRL);
    
    /********** Set standard mode **********/
    iowrite32 (PCIe_Phy_PMA_ISO_MODE__VALUE, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_MODE);  //  GEN2
    
    iowrite32 (0x00, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_TX_DMPH_LO);  //  -6.0dB
    // iowrite32 (0x01, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_TX_DMPH_LO);  //  -3.5dB
    // iowrite32 (0x02, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_TX_DMPH_LO);  //  0dB
    
    /********** Setup transceiver controls to enable lanes but in electrical idle **********/
    iowrite32 (0x1103, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_XCVR_CTRL + LANE_0_BASE);
    iowrite32 (0x1103, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_XCVR_CTRL + LANE_1_BASE);
    iowrite32 (0x1103, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_XCVR_CTRL + LANE_2_BASE);
    iowrite32 (0x1103, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_XCVR_CTRL + LANE_3_BASE);
    
    /********** Write to PMA lanes TD to set a repeating pattern **********/
    iowrite32 (0xCCCC, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_DATA_LO + LANE_0_BASE);
    iowrite32 (0xCCCC, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_DATA_LO + LANE_1_BASE);
    iowrite32 (0xCCCC, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_DATA_LO + LANE_2_BASE);
    iowrite32 (0xCCCC, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_DATA_LO + LANE_3_BASE);
    
    iowrite32 (0xCCCC, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_DATA_HI + LANE_0_BASE);
    iowrite32 (0xCCCC, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_DATA_HI + LANE_1_BASE);
    iowrite32 (0xCCCC, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_DATA_HI + LANE_2_BASE);
    iowrite32 (0xCCCC, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_DATA_HI + LANE_3_BASE);
    
    /********** Enable loopback for selected lane **********/
    iowrite32 (mode, PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_LPBK + LANE_0_BASE);
    iowrite32 (mode, PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_LPBK + LANE_1_BASE);
    iowrite32 (mode, PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_LPBK + LANE_2_BASE);
    iowrite32 (mode, PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_LPBK + LANE_3_BASE);
    
    /********** Put everything into isolation mode (PMA) **********/
    iowrite32 (0xD3FF, PCIE_PHY_BASE + PCIe_Phy_PMA_ISOLATION_CTRL);
    
    /********** Release reset and set up refclk selection **********/
    iowrite32 (0x0E33, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_CMN_CTRL);
    
    /********** Poll until PMA asserts cmn_ready **********/
    timeout_cntr = 0;
    while ((ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_CTRL) & 0x1) != 0x1)
    {
        timeout_cntr++;
        if (timeout_cntr == COMMON_TIMEOUT)
        {
            rumboot_printf ("    ERROR: timeout_0\n");
            return -1;
        }
    }
    
    /********** Set power state request **********/
    iowrite32 (0x1, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_PWRST_CTRL);
    
    /********** Poll until PMA responds with the power state acknowledge **********/
    timeout_cntr = 0;
    while ((ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_PWRST_CTRL) & 0x100) != 0x100)
    {
        timeout_cntr++;
        if (timeout_cntr == COMMON_TIMEOUT)
        {
            rumboot_printf ("    ERROR: timeout_1\n");
            return -1;
        }
    }
    
    /********** De-assert transceiver control **********/
    /********** electrical idle to begin transmission **********/
    iowrite32 (0x0103, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_XCVR_CTRL + LANE_0_BASE);
    iowrite32 (0x0103, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_XCVR_CTRL + LANE_1_BASE);
    iowrite32 (0x0103, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_XCVR_CTRL + LANE_2_BASE);
    iowrite32 (0x0103, PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_XCVR_CTRL + LANE_3_BASE);
    
    //---------------------------------------------------------------------------------------------
    /********** Activate BIST testing **********/

    /********** Clear the BIST engines **********/
    iowrite32 (0x0002, PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + (LANE_0_BASE << 1));
    iowrite32 (0x0002, PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + (LANE_1_BASE << 1));
    iowrite32 (0x0002, PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + (LANE_2_BASE << 1));
    iowrite32 (0x0002, PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + (LANE_3_BASE << 1));
    
    iowrite32 (0x0012, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + (LANE_0_BASE << 1));
    iowrite32 (0x0012, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + (LANE_1_BASE << 1));
    iowrite32 (0x0012, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + (LANE_2_BASE << 1));
    iowrite32 (0x0012, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + (LANE_3_BASE << 1));
    
    /********** Activate BIST on TX side **********/
    iowrite32 (0x0B01, PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + (LANE_0_BASE << 1));
    iowrite32 (0x0B01, PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + (LANE_1_BASE << 1));
    iowrite32 (0x0B01, PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + (LANE_2_BASE << 1));
    iowrite32 (0x0B01, PCIE_PHY_BASE + PCIe_Phy_PMA_TX_BIST_CTRL + (LANE_3_BASE << 1));
    
    /********** Activate BIST on RX side **********/
    iowrite32 (0x00FA, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_SYNCCNT + (LANE_0_BASE << 1));
    iowrite32 (0x00FA, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_SYNCCNT + (LANE_1_BASE << 1));
    iowrite32 (0x00FA, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_SYNCCNT + (LANE_2_BASE << 1));
    iowrite32 (0x00FA, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_SYNCCNT + (LANE_3_BASE << 1));
    iowrite32 (0x0B01, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + (LANE_0_BASE << 1));
    iowrite32 (0x0B01, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + (LANE_1_BASE << 1));
    iowrite32 (0x0B01, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + (LANE_2_BASE << 1));
    iowrite32 (0x0B01, PCIE_PHY_BASE + PCIe_Phy_PMA_RX_BIST_CTRL + (LANE_3_BASE << 1));
}

int main ()
{
    simulation_speedup ();
    
    pcie_turn_on_bist_all_lanes (ISI_LOOPBACK);
    
    //  This delay can be removed.
    //    It present only to show, that PCIe Phy works infinite time.
    delay_cycle (200);
    
    return 0;
}
