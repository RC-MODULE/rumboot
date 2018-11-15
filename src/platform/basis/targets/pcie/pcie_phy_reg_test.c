
//-----------------------------------------------------------------------------
//  This program is for testing PCIe Phy registers.
//  It must show, that Phy APB bus works correctly.
//  Nothing writes to registers, because it can make bad things.
//  It includes:
//    - checking default values of some registers
//
//    Test duration (RTL): < 150us
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/pcie_test_lib.h>
#include <regs/regs_pcie.h>
#include <platform/devices.h>

//-------------------------------------------------------------------
//  Group: Phy Macro Top Level
//    Not all registers are used (and described here too)
//-------------------------------------------------------------------

//    Default values:
#define def_PCIe_Phy_PCS_CMN_CTRL1            0x040F
#define def_PCIe_Phy_PCS_CMN_CTRL2            0x0051
#define def_PCIe_Phy_PCS_COM_LOCK_CFG1        0x4400
#define def_PCIe_Phy_PCS_GEN3_EIE_LOCK_CFG    0x037F
#define def_PCIe_Phy_PCS_ISO_CMN_CTRL         0x0023

#define def_PCIe_Phy_PCS_ISO_TX_CTRL          0x0000
#define def_PCIe_Phy_PCS_ISO_TX_LPC_LO        0x0000
#define def_PCIe_Phy_PCS_ISO_TX_LPC_HI        0x0000
#define def_PCIe_Phy_PCS_ISO_TX_DMPH_HI       0x0000
#define def_PCIe_Phy_PCS_ISO_RX_CTRL          0x0010
#define def_PCIe_Phy_PCS_ISO_RX_DATA_HI       0x0000

#define def_PCIe_Phy_PMA_CMN_CTRL             0x2000
#define def_PCIe_Phy_PMA_SSM_STATE            0x0020
#define def_PCIe_Phy_PMA_PSM_STATE_LO         0x0000
#define def_PCIe_Phy_PMA_ISO_CMN_CTRL         0x0C01
#define def_PCIe_Phy_PMA_ISOLATION_CTRL       0x0000

#define def_PCIe_Phy_PMA_XCVR_CTRL            0x0000
#define def_PCIe_Phy_PMA_XCVR_LPBK            0x0000
#define def_PCIe_Phy_PMA_PI_POS               0x0000
#define def_PCIe_Phy_PMA_ISO_TX_LPC_LO        0x0000
#define def_PCIe_Phy_PMA_ISO_TX_FSLF          0x0000
#define def_PCIe_Phy_PMA_ISO_DATA_HI          0x0000
//-------------------------------------------------------------------
//  Group: PMA common
//    Not all registers are used (and described here too)
//-------------------------------------------------------------------

//    Default values:
#define def_PCIe_Phy_PMA_CMN_PID_TYPE               0x7364
#define def_PCIe_Phy_PMA_CMN_PID_NUM                0x0104
#define def_PCIe_Phy_PMA_CMN_PID_REV                0x0320
#define def_PCIe_Phy_PMA_CMN_PID_MFG                0x0074
#define def_PCIe_Phy_PMA_CMN_PID_NODE               0x0028
#define def_PCIe_Phy_PMA_CMN_PID_FLV                0x686D
#define def_PCIe_Phy_PMA_CMN_PID_IOV                0x0180
#define def_PCIe_Phy_PMA_CMN_PID_METAL              0x6020
#define def_PCIe_Phy_PMA_CMN_PID_LANES              0x0202
#define def_PCIe_Phy_PMA_CMN_SSM_BANDGAP_TMR        0x0019
#define def_PCIe_Phy_PMA_CMN_SSM_BIAS_TMR           0x0032
#define def_PCIe_Phy_PMA_CMN_SSM_PLLEN_TMR          0x0001
#define def_PCIe_Phy_PMA_CMN_SSM_PLLVREF_TMR        0x0001
#define def_PCIe_Phy_PMA_CMN_SSM_LANECAL_TMR        0x01B5
#define def_PCIe_Phy_PMA_CMN_SSM_A1OUT_TMR          0x0001
#define def_PCIe_Phy_PMA_CMN_PSC_A0                 0xCF7F
#define def_PCIe_Phy_PMA_CMN_PSC_A1                 0xCF23
#define def_PCIe_Phy_PMA_CMN_PSC_A2                 0xE022
#define def_PCIe_Phy_PMA_CMN_PSC_A4                 0x0080

#define def_PCIe_Phy_PMA_CMN_PLLCPICAL_CTRL         0x0004
#define def_PCIe_Phy_PMA_CMN_PLLCPICAL_OVRD         0x0000
#define def_PCIe_Phy_PMA_CMN_PLLCPICAL_INIT_TMR     0x01F4
#define def_PCIe_Phy_PMA_CMN_RXCAL_CTRL             0x0000
#define def_PCIe_Phy_PMA_CMN_RXCAL_OVRD             0x0000
#define def_PCIe_Phy_PMA_CMN_RXCAL_INIT_TMR         0x02EE
#define def_PCIe_Phy_PMA_CMN_TXPUCAL_CTRL           0x0000
#define def_PCIe_Phy_PMA_CMN_TXPUCAL_OVRD           0x0000
#define def_PCIe_Phy_PMA_CMN_TXPUCAL_INIT_TMR       0x002F
#define def_PCIe_Phy_PMA_CMN_TXPDCAL_CTRL           0x003C
#define def_PCIe_Phy_PMA_CMN_TXPDCAL_OVRD           0x0000
#define def_PCIe_Phy_PMA_CMN_TXPDCAL_INIT_TMR       0x002F

#define def_PCIe_Phy_PMA_TX_HBDC_PRE_OVRD           0x0000
#define def_PCIe_Phy_PMA_TX_HBDC_MAIN_OVRD          0x0000
#define def_PCIe_Phy_PMA_TX_HBDC_POST_OVRD          0x0000
#define def_PCIe_Phy_PMA_TX_HBDC_PRE_PREM_OVRD      0x0000
#define def_PCIe_Phy_PMA_TX_HBDC_MAIN_INITM_OVRD    0x0000
// SU
#define def_PCIe_Phy_PMA_TX_RCVDET_CTRL             0x1388
#define def_PCIe_Phy_PMA_TX_BIST_CTRL               0x0000
#define def_PCIe_Phy_PMA_TX_BIST_UDDWR              0x0000
// SU
#define def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_CTRL            0x0000
#define def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_START           0x0000
#define def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_INIT_TMR        0x02EE
#define def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_ITER_TMR        0x0080
// SU
#define def_PCIe_Phy_PMA_RX_IQPI_ILL_LOCK_CALCNT_START0  0x013F
// SU
#define def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_CTRL             0x0000
#define def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_START            0x0000
#define def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_INIT_TMR         0x02EE
#define def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_ITER_TMR         0x0080
// SU
#define def_PCIe_Phy_PMA_RX_EPI_ILL_LOCK_CALCNT_START0   0x013F
#define def_PCIe_Phy_PMA_RX_CDRLF_CNFG                   0x018E
#define def_PCIe_Phy_PMA_RX_CDRLF_CNFG2                  0x1133
#define def_PCIe_Phy_PMA_RX_CDRLF_MGN_DIAG               0x0000
#define def_PCIe_Phy_PMA_RX_SIGDET_FILT_TMR              0x000C
#define def_PCIe_Phy_PMA_RX_SIGDET_DLY_TMR               0x0000
#define def_PCIe_Phy_PMA_RX_SIGDET_MIN_TMR               0x0000

int check_read_default_value ()
{
    if (
        //-------------------------------------------------------------------
        //  Group: Phy Macro Top Level
        //    Not all registers are used (and described here too)
        //-------------------------------------------------------------------
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PCS_CMN_CTRL1         ) != def_PCIe_Phy_PCS_CMN_CTRL1         ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PCS_CMN_CTRL2         ) != def_PCIe_Phy_PCS_CMN_CTRL2         ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PCS_COM_LOCK_CFG1     ) != def_PCIe_Phy_PCS_COM_LOCK_CFG1     ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PCS_GEN3_EIE_LOCK_CFG ) != def_PCIe_Phy_PCS_GEN3_EIE_LOCK_CFG ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PCS_ISO_CMN_CTRL      ) != def_PCIe_Phy_PCS_ISO_CMN_CTRL      ) ||

        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_CTRL     + 0x000 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_CTRL       ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_LPC_LO   + 0x000 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_LPC_LO     ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_LPC_HI   + 0x000 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_LPC_HI     ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_DMPH_HI  + 0x000 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_DMPH_HI    ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_RX_CTRL     + 0x000 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_RX_CTRL       ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_RX_DATA_HI  + 0x000 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_RX_DATA_HI    ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_CTRL     + 0x400 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_CTRL       ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_LPC_LO   + 0x400 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_LPC_LO     ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_LPC_HI   + 0x400 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_LPC_HI     ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_DMPH_HI  + 0x400 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_DMPH_HI    ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_RX_CTRL     + 0x400 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_RX_CTRL       ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_RX_DATA_HI  + 0x400 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_RX_DATA_HI    ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_CTRL     + 0x800 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_CTRL       ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_LPC_LO   + 0x800 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_LPC_LO     ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_LPC_HI   + 0x800 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_LPC_HI     ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_DMPH_HI  + 0x800 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_DMPH_HI    ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_RX_CTRL     + 0x800 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_RX_CTRL       ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_RX_DATA_HI  + 0x800 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_RX_DATA_HI    ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_CTRL     + 0xC00 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_CTRL       ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_LPC_LO   + 0xC00 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_LPC_LO     ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_LPC_HI   + 0xC00 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_LPC_HI     ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_TX_DMPH_HI  + 0xC00 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_TX_DMPH_HI    ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_RX_CTRL     + 0xC00 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_RX_CTRL       ) ||
        (*(volatile int*)(PCIe_Phy_PCS_ISO_RX_DATA_HI  + 0xC00 + PCIE_PHY_BASE) != def_PCIe_Phy_PCS_ISO_RX_DATA_HI    ) ||

        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_CTRL       ) != def_PCIe_Phy_PMA_CMN_CTRL          ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_SSM_STATE      ) != def_PCIe_Phy_PMA_SSM_STATE         ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_PSM_STATE_LO   ) != def_PCIe_Phy_PMA_PSM_STATE_LO      ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_CMN_CTRL   ) != def_PCIe_Phy_PMA_ISO_CMN_CTRL      ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_ISOLATION_CTRL ) != def_PCIe_Phy_PMA_ISOLATION_CTRL    ) ||

        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_CTRL     ) != def_PCIe_Phy_PMA_XCVR_CTRL         ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_XCVR_LPBK     ) != def_PCIe_Phy_PMA_XCVR_LPBK         ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_PI_POS        ) != def_PCIe_Phy_PMA_PI_POS            ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_TX_LPC_LO ) != def_PCIe_Phy_PMA_ISO_TX_LPC_LO     ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_TX_FSLF   ) != def_PCIe_Phy_PMA_ISO_TX_FSLF       ) ||  default value not correspond spec
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_ISO_DATA_HI   ) != def_PCIe_Phy_PMA_ISO_DATA_HI       ) ||


        //-------------------------------------------------------------------
        //  Group: PMA common
        //    Not all registers are used (and described here too)
        //-------------------------------------------------------------------

        //    Registers addresses:
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PID_TYPE        ) != def_PCIe_Phy_PMA_CMN_PID_TYPE             ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PID_NUM         ) != def_PCIe_Phy_PMA_CMN_PID_NUM              ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PID_REV         ) != def_PCIe_Phy_PMA_CMN_PID_REV              ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PID_MFG         ) != def_PCIe_Phy_PMA_CMN_PID_MFG              ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PID_NODE        ) != def_PCIe_Phy_PMA_CMN_PID_NODE             ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PID_FLV         ) != def_PCIe_Phy_PMA_CMN_PID_FLV              ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PID_IOV         ) != def_PCIe_Phy_PMA_CMN_PID_IOV              ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PID_METAL       ) != def_PCIe_Phy_PMA_CMN_PID_METAL            ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PID_LANES       ) != def_PCIe_Phy_PMA_CMN_PID_LANES            ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_SSM_BANDGAP_TMR ) != def_PCIe_Phy_PMA_CMN_SSM_BANDGAP_TMR      ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_SSM_BIAS_TMR    ) != def_PCIe_Phy_PMA_CMN_SSM_BIAS_TMR         ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_SSM_PLLEN_TMR   ) != def_PCIe_Phy_PMA_CMN_SSM_PLLEN_TMR        ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_SSM_PLLVREF_TMR ) != def_PCIe_Phy_PMA_CMN_SSM_PLLVREF_TMR      ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_SSM_LANECAL_TMR ) != def_PCIe_Phy_PMA_CMN_SSM_LANECAL_TMR      ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_SSM_A1OUT_TMR   ) != def_PCIe_Phy_PMA_CMN_SSM_A1OUT_TMR        ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PSC_A0          ) != def_PCIe_Phy_PMA_CMN_PSC_A0               ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PSC_A1          ) != def_PCIe_Phy_PMA_CMN_PSC_A1               ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PSC_A2          ) != def_PCIe_Phy_PMA_CMN_PSC_A2               ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PSC_A4          ) != def_PCIe_Phy_PMA_CMN_PSC_A4               ) ||

        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PLLCPICAL_CTRL    ) != def_PCIe_Phy_PMA_CMN_PLLCPICAL_CTRL       ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PLLCPICAL_OVRD    ) != def_PCIe_Phy_PMA_CMN_PLLCPICAL_OVRD       ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_PLLCPICAL_INIT_TMR) != def_PCIe_Phy_PMA_CMN_PLLCPICAL_INIT_TMR   ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_RXCAL_CTRL        ) != def_PCIe_Phy_PMA_CMN_RXCAL_CTRL           ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_RXCAL_OVRD        ) != def_PCIe_Phy_PMA_CMN_RXCAL_OVRD           ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_RXCAL_INIT_TMR    ) != def_PCIe_Phy_PMA_CMN_RXCAL_INIT_TMR       ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_TXPUCAL_CTRL      ) != def_PCIe_Phy_PMA_CMN_TXPUCAL_CTRL         ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_TXPUCAL_OVRD      ) != def_PCIe_Phy_PMA_CMN_TXPUCAL_OVRD         ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_TXPUCAL_INIT_TMR  ) != def_PCIe_Phy_PMA_CMN_TXPUCAL_INIT_TMR     ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_TXPDCAL_CTRL      ) != def_PCIe_Phy_PMA_CMN_TXPDCAL_CTRL         ) ||
        // (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_TXPDCAL_OVRD      ) != def_PCIe_Phy_PMA_CMN_TXPDCAL_OVRD         ) ||
        (ioread32 (PCIE_PHY_BASE + PCIe_Phy_PMA_CMN_TXPDCAL_INIT_TMR  ) != def_PCIe_Phy_PMA_CMN_TXPDCAL_INIT_TMR     ) ||


        //-------------------------------------------------------------------
        //  Group: PMA Tx
        //    Not all registers are used (and described here too)
        //-------------------------------------------------------------------

        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_PRE_OVRD        + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_PRE_OVRD         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_MAIN_OVRD       + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_MAIN_OVRD        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_POST_OVRD       + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_POST_OVRD        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_PRE_PREM_OVRD   + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_PRE_PREM_OVRD    ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_MAIN_INITM_OVRD + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_MAIN_INITM_OVRD  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_RCVDET_CTRL          + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_RCVDET_CTRL           ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_BIST_CTRL            + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_BIST_CTRL             ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_BIST_UDDWR           + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_BIST_UDDWR            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_PRE_OVRD        + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_PRE_OVRD         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_MAIN_OVRD       + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_MAIN_OVRD        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_POST_OVRD       + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_POST_OVRD        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_PRE_PREM_OVRD   + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_PRE_PREM_OVRD    ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_MAIN_INITM_OVRD + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_MAIN_INITM_OVRD  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_RCVDET_CTRL          + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_RCVDET_CTRL           ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_BIST_CTRL            + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_BIST_CTRL             ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_BIST_UDDWR           + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_BIST_UDDWR            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_PRE_OVRD        + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_PRE_OVRD         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_MAIN_OVRD       + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_MAIN_OVRD        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_POST_OVRD       + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_POST_OVRD        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_PRE_PREM_OVRD   + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_PRE_PREM_OVRD    ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_MAIN_INITM_OVRD + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_MAIN_INITM_OVRD  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_RCVDET_CTRL          + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_RCVDET_CTRL           ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_BIST_CTRL            + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_BIST_CTRL             ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_BIST_UDDWR           + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_BIST_UDDWR            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_PRE_OVRD        + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_PRE_OVRD         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_MAIN_OVRD       + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_MAIN_OVRD        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_POST_OVRD       + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_POST_OVRD        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_PRE_PREM_OVRD   + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_PRE_PREM_OVRD    ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_HBDC_MAIN_INITM_OVRD + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_HBDC_MAIN_INITM_OVRD  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_RCVDET_CTRL          + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_RCVDET_CTRL           ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_BIST_CTRL            + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_BIST_CTRL             ) ||
        (*(volatile int*)(PCIe_Phy_PMA_TX_BIST_UDDWR           + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_TX_BIST_UDDWR            ) ||


        //-------------------------------------------------------------------
        //  Group: PMA Rx
        //    Not all registers are used (and described here too)
        //-------------------------------------------------------------------

        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_CTRL            + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_CTRL            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_START           + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_START           ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_INIT_TMR        + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_INIT_TMR        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_ITER_TMR        + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_ITER_TMR        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_LOCK_CALCNT_START0  + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_LOCK_CALCNT_START0  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_CTRL             + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_CTRL             ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_START            + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_START            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_INIT_TMR         + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_INIT_TMR         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_ITER_TMR         + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_ITER_TMR         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_LOCK_CALCNT_START0   + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_LOCK_CALCNT_START0   ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_CNFG                   + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_CNFG                   ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_CNFG2                  + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_CNFG2                  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_MGN_DIAG               + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_MGN_DIAG               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_FILT_TMR              + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_FILT_TMR              ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_DLY_TMR               + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_DLY_TMR               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_MIN_TMR               + 0x0000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_MIN_TMR               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_CTRL            + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_CTRL            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_START           + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_START           ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_INIT_TMR        + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_INIT_TMR        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_ITER_TMR        + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_ITER_TMR        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_LOCK_CALCNT_START0  + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_LOCK_CALCNT_START0  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_CTRL             + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_CTRL             ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_START            + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_START            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_INIT_TMR         + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_INIT_TMR         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_ITER_TMR         + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_ITER_TMR         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_LOCK_CALCNT_START0   + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_LOCK_CALCNT_START0   ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_CNFG                   + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_CNFG                   ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_CNFG2                  + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_CNFG2                  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_MGN_DIAG               + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_MGN_DIAG               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_FILT_TMR              + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_FILT_TMR              ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_DLY_TMR               + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_DLY_TMR               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_MIN_TMR               + 0x0800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_MIN_TMR               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_CTRL            + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_CTRL            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_START           + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_START           ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_INIT_TMR        + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_INIT_TMR        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_ITER_TMR        + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_ITER_TMR        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_LOCK_CALCNT_START0  + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_LOCK_CALCNT_START0  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_CTRL             + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_CTRL             ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_START            + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_START            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_INIT_TMR         + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_INIT_TMR         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_ITER_TMR         + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_ITER_TMR         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_LOCK_CALCNT_START0   + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_LOCK_CALCNT_START0   ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_CNFG                   + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_CNFG                   ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_CNFG2                  + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_CNFG2                  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_MGN_DIAG               + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_MGN_DIAG               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_FILT_TMR              + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_FILT_TMR              ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_DLY_TMR               + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_DLY_TMR               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_MIN_TMR               + 0x1000 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_MIN_TMR               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_CTRL            + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_CTRL            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_START           + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_START           ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_INIT_TMR        + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_INIT_TMR        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_CAL_ITER_TMR        + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_CAL_ITER_TMR        ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_IQPI_ILL_LOCK_CALCNT_START0  + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_IQPI_ILL_LOCK_CALCNT_START0  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_CTRL             + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_CTRL             ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_START            + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_START            ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_INIT_TMR         + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_INIT_TMR         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_CAL_ITER_TMR         + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_CAL_ITER_TMR         ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_EPI_ILL_LOCK_CALCNT_START0   + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_EPI_ILL_LOCK_CALCNT_START0   ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_CNFG                   + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_CNFG                   ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_CNFG2                  + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_CNFG2                  ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_CDRLF_MGN_DIAG               + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_CDRLF_MGN_DIAG               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_FILT_TMR              + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_FILT_TMR              ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_DLY_TMR               + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_DLY_TMR               ) ||
        (*(volatile int*)(PCIe_Phy_PMA_RX_SIGDET_MIN_TMR               + 0x1800 + PCIE_PHY_BASE) != def_PCIe_Phy_PMA_RX_SIGDET_MIN_TMR               )
    )
        return -1;

    return 0;
}


int main ()
{
    if (check_read_default_value () != 0)
        return -1;

    return 0;
}
