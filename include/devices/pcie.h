
/* --- Peripheral Base Addresses --- */

#define DIT0_BASE      0x0102A000
#define DIT1_BASE      0x0102B000
#define DIT2_BASE      0x0102C000
#define DIT3_BASE      0x0102D000

#define TSNS_BASE      0x01019000

#define GPIO0_BASE     0x01036000
#define GPIO1_BASE     0x01037000
#define GPIO2_BASE     0x01038000

#define SCTL_BASE      0x01010000

#define PCIE_BASE      0x40000000

#define ADDR_TRANS_SLV_BASE      0x01034000
#define ADDR_TRANS_MST_BASE      0x01035000
#define PCIE_CORE_BASE           0x01058000
#define PCIE_PHY_BASE            0x01060000

/* --- I2C ------) --- */
#define I2C0_BASE      0x0102A000
#define I2C1_BASE      0x0102B000

#define I2C_PRESCALE		0x00
#define I2C_CTRL			0x04
#define I2C_TRANSMIT		0x08
#define I2C_RECEIVE			0x08
#define I2C_COMMAND			0x0C
#define I2C_STATUS			0x0C

#define SDIO_BASE		 0x01032000
#define GSPI_BASE		 0x01030000



/* --- GPIO (PL061) --- */

#define GPIO_DIR                 0x0400
#define GPIO_DATA                0x03FC



/* --- TIMER (ARM SP804) --- */

#define TIMER0_BASE    0x0
#define TIMER1_BASE    0x020

#define TIMER_LOAD     0x000
#define TIMER_VALUE    0x004
#define TIMER_CONTROL  0x008
#define TIMER_CLEAR    0x00C
#define TIMER_RIS      0x010
#define TIMER_MIS      0x014
#define TIMER_BGLOAD   0x018

#define TIMER_ITCR     0xF00
#define TIMER_ITOP     0xF04

#define TIMER_PERIPHID0  0xFE0
#define TIMER_PERIPHID1  0xFE4
#define TIMER_PERIPHID2  0xFE8
#define TIMER_PERIPHID3  0xFEC
#define TIMER_PCELLID0   0xFF0
#define TIMER_PCELLID1   0xFF4
#define TIMER_PCELLID2   0xFF8
#define TIMER_PCELLID3   0xFFC

/* --- WDOG (ARM SP805) --- */

#define WDOG_LOAD    0x000
#define WDOG_VALUE   0x004
#define WDOG_CONTROL 0x008
#define WDOG_INTCLR  0x00C
#define WDOG_RIS     0x010
#define WDOG_MIS     0x014
#define WDOG_LOCK    0xC00

#define WDOG_ITCR  0xF00
#define WDOG_ITOP  0xF04

#define WDOG_PERIPHID0  0xFE0
#define WDOG_PERIPHID1  0xFE4
#define WDOG_PERIPHID2  0xFE8
#define WDOG_PERIPHID3  0xFEC
#define WDOG_PCELLID0   0xFF0
#define WDOG_PCELLID1   0xFF4
#define WDOG_PCELLID2   0xFF8
#define WDOG_PCELLID3   0xFFC

/* --- TSNS (Module) ----*/

#define TSNS_PWDN     0x000
#define TSNS_START    0x004
#define TSNS_IR       0x008
#define TSNS_IM       0x00C
#define TSNS_CLKDIV   0x010
#define TSNS_LEVEL    0x014
#define TSNS_DATA     0x018
#define TSNS_ENZCALIB 0x01C

/* --- SCTL --- */

#define SCTL_PCIE_RST   0x400
#define SCTL_PCIE_REG_0 0x404
#define SCTL_PCIE_REG_1 0x408
#define SCTL_PCIE_REG_2 0x40C

/* --- PCIe Phy --- */

    // paddr [11] was excluded, so it paddr forms next way
#define PCIe_Phy_PCS_CMN_CTRL1             0xC000*2 + 0x000*4
#define PCIe_Phy_PCS_CMN_CTRL2             0xC000*2 + 0x001*4
#define PCIe_Phy_PCS_COM_LOCK_CFG1         0xC000*2 + 0x002*4
#define PCIe_Phy_PCS_COM_LOCK_CFG2         0xC000*2 + 0x003*4
#define PCIe_Phy_PCS_GEN3_EIE_LOCK_CFG     0xC000*2 + 0x004*4
#define PCIe_Phy_PCS_LANE_DSBL             0xC000*2 + 0x005*4
#define PCIe_Phy_PCS_RCV_DET_INH           0xC000*2 + 0x006*4
#define PCIe_Phy_PCS_ISO_CMN_CTRL          0xC000*2 + 0x008*4
#define PCIe_Phy_EQ_TRAIN_CTRL             0xC000*2 + 0x008*4
#define PCIe_Phy_ISO_LN_CTRL               0xD000*2 + 0x000*4
#define PCIe_Phy_EQ_COEF_TRAIN_CTRL        0xD000*2 + 0x001*4
#define PCIe_Phy_PCS_ISO_TX_CTRL           0xD000*2 + 0x000*4
#define PCIe_Phy_PCS_ISO_TX_LPC_LO         0xD000*2 + 0x001*4
#define PCIe_Phy_PCS_ISO_TX_LPC_HI         0xD000*2 + 0x002*4
#define PCIe_Phy_PCS_ISO_TX_DMPH_LO        0xD000*2 + 0x003*4
#define PCIe_Phy_PCS_ISO_TX_DMPH_HI        0xD000*2 + 0x004*4
#define PCIe_Phy_PCS_ISO_TX_FSLF           0xD000*2 + 0x005*4
#define PCIe_Phy_PCS_ISO_TX_DATA_LO        0xD000*2 + 0x006*4
#define PCIe_Phy_PCS_ISO_TX_DATA_HI        0xD000*2 + 0x007*4
#define PCIe_Phy_PCS_ISO_RX_CTRL           0xD000*2 + 0x008*4
#define PCIe_Phy_PCS_ISO_RX_EQ_EVAL        0xD000*2 + 0x009*4
#define PCIe_Phy_PCS_ISO_RX_DATA_LO        0xD000*2 + 0x00E*4
#define PCIe_Phy_PCS_ISO_RX_DATA_HI        0xD000*2 + 0x00F*4
#define PCIe_Phy_PMA_CMN_CTRL              0xE000*2 + 0x000*4
#define PCIe_Phy_PMA_SSM_STATE             0xE000*2 + 0x001*4
#define PCIe_Phy_PMA_PSM_STATE_LO          0xE000*2 + 0x002*4
#define PCIe_Phy_PMA_PSM_STATE_HI          0xE000*2 + 0x003*4
#define PCIe_Phy_PMA_ISO_CMN_CTRL          0xE000*2 + 0x004*4
#define PCIe_Phy_PMA_ISO_MODE              0xE000*2 + 0x005*4
#define PCIe_Phy_PMA_ISO_PWRST_CTRL        0xE000*2 + 0x006*4
#define PCIe_Phy_PMA_ISOLATION_CTRL        0xE000*2 + 0x00F*4
#define PCIe_Phy_PMA_PMA_CMN_SSM_DIAG      0x0000*2 + 0x039*4
#define PCIe_Phy_PMA_XCVR_CTRL             0xF000*2 + 0x000*4
#define PCIe_Phy_PMA_XCVR_LPBK             0xF000*2 + 0x001*4
#define PCIe_Phy_PMA_PI_POS                0xF000*2 + 0x002*4
#define PCIe_Phy_PMA_ISO_XCVR_CTRL         0xF000*2 + 0x003*4
#define PCIe_Phy_PMA_ISO_TX_LPC_LO         0xF000*2 + 0x004*4
#define PCIe_Phy_PMA_ISO_TX_LPC_HI         0xF000*2 + 0x005*4
#define PCIe_Phy_PMA_ISO_TX_DMPH_LO        0xF000*2 + 0x006*4
#define PCIe_Phy_PMA_ISO_TX_DMPH_HI        0xF000*2 + 0x007*4
#define PCIe_Phy_PMA_ISO_TX_FSLF           0xF000*2 + 0x008*4
#define PCIe_Phy_PMA_ISO_TX_MGN            0xF000*2 + 0x009*4
#define PCIe_Phy_PMA_ISO_RX_EQ_CTRL        0xF000*2 + 0x00D*4
#define PCIe_Phy_PMA_ISO_DATA_LO           0xF000*2 + 0x00E*4
#define PCIe_Phy_PMA_ISO_DATA_HI           0xF000*2 + 0x00F*4
#define PCIe_Phy_PMA_TX_BIST_CTRL          0x4000*2 + 0x040*4
#define PCIe_Phy_PMA_TX_BIST_UDDWR         0x4000*2 + 0x041*4
#define PCIe_Phy_PMA_RX_BIST_CTRL          0x8000*2 + 0x120*4
#define PCIe_Phy_PMA_RX_BIST_SYNCCNT       0x8000*2 + 0x121*4
#define PCIe_Phy_PMA_RX_BIST_UDDWR         0x8000*2 + 0x122*4
#define PCIe_Phy_PMA_RX_BIST_ERRCNT        0x8000*2 + 0x123*4
#define PCIe_Phy_PMA_CMN_PID_TYPE          0x0000*2 + 0x000*4
#define PCIe_Phy_PMA_CMN_PID_NUM           0x0000*2 + 0x003*4
#define PCIe_Phy_PMA_CMN_PID_REV           0x0000*2 + 0x004*4
#define PCIe_Phy_PMA_CMN_PID_MFG           0x0000*2 + 0x008*4
#define PCIe_Phy_PMA_CMN_PID_NODE          0x0000*2 + 0x009*4
#define PCIe_Phy_PMA_CMN_PID_FLV           0x0000*2 + 0x00A*4
#define PCIe_Phy_PMA_CMN_PID_IOV           0x0000*2 + 0x00C*4
#define PCIe_Phy_PMA_CMN_PID_METAL         0x0000*2 + 0x00D*4
#define PCIe_Phy_PMA_CMN_PID_LANES         0x0000*2 + 0x010*4
#define PCIe_Phy_RX_EYESURF_CTRL           0x8000*2 + 0x0E0*4
#define PCIe_Phy_RX_EYESURF_TMR_DELLOW     0x8000*2 + 0x0E4*4
#define PCIe_Phy_RX_EYESURF_TMR_DELHIGH    0x8000*2 + 0x0E5*4
#define PCIe_Phy_RX_EYESURF_TMR_TESTLOW    0x8000*2 + 0x0E6*4
#define PCIe_Phy_RX_EYESURF_TMR_TESTHIGH   0x8000*2 + 0x0E7*4
#define PCIe_Phy_RX_EYESURF_NS_COORD       0x8000*2 + 0x0E8*4
#define PCIe_Phy_RX_EYESURF_EW_COORD       0x8000*2 + 0x0E9*4
#define PCIe_Phy_RX_EYESURF_ERRCNT         0x8000*2 + 0x0EA*4

/* --- This registers are used for netlist simspeedup --- */
#define PCIe_Phy_MASSWR_TX_RCVDET_CTRL                    0x6000*2 + 0x020*4
#define PCIe_Phy_CMN_SSM_BANDGAP_TMR                      0x0000*2 + 0x020*4
#define PCIe_Phy_CMN_SSM_BIAS_TMR                         0x0000*2 + 0x021*4
#define PCIe_Phy_CMN_SSM_PLLEN_TMR                        0x0000*2 + 0x022*4
#define PCIe_Phy_CMN_SSM_PLLPRE_TMR                       0x0000*2 + 0x023*4
#define PCIe_Phy_CMN_SSM_PLLVREF_TMR                      0x0000*2 + 0x024*4
#define PCIe_Phy_CMN_SSM_PLLLOCK_TMR                      0x0000*2 + 0x025*4
#define PCIe_Phy_CMN_SSM_PLLCLKDIS_TMR                    0x0000*2 + 0x026*4
#define PCIe_Phy_CMN_SSM_LANECAL_TMR                      0x0000*2 + 0x028*4
#define PCIe_Phy_CMN_SSM_A0IN_TMR                         0x0000*2 + 0x029*4
#define PCIe_Phy_CMN_SSM_A1IN_TMR                         0x0000*2 + 0x02A*4
#define PCIe_Phy_CMN_SSM_A2IN_TMR                         0x0000*2 + 0x02B*4
#define PCIe_Phy_CMN_SSM_A3IN_TMR                         0x0000*2 + 0x02C*4
#define PCIe_Phy_CMN_BGCAL_OVRD                           0x0000*2 + 0x061*4
#define PCIe_Phy_CMN_PLL_CPI_OVRD                         0x0000*2 + 0x081*4
#define PCIe_Phy_MASSWR_CMN_SDCAL0_OVRD                   0xA000*2 + 0x181*4
#define PCIe_Phy_MASSWR_CMN_SDCAL1_OVRD                   0xA000*2 + 0x191*4
#define PCIe_Phy_CMN_PLL_VCOCAL_OVRD                      0x0000*2 + 0x0A2*4
#define PCIe_Phy_CMN_PLL_VCOCAL_CNT_START                 0x0000*2 + 0x0A7*4
#define PCIe_Phy_CMN_PLL_LOCK_CNT_START                   0x0000*2 + 0x0A5*4
#define PCIe_Phy_MASSWR_RX_IQPI_ILL_CAL_OVRD              0xA000*2 + 0x022*4
#define PCIe_Phy_MASSWR_RX_IQPI_ILL_LOCK_REFCNT_START     0xA000*2 + 0x025*4
#define PCIe_Phy_MASSWR_RX_IQPI_ILL_LOCK_CALCNT_START0    0xA000*2 + 0x026*4
#define PCIe_Phy_MASSWR_RX_IQPI_ILL_LOCK_CALCNT_START1    0xA000*2 + 0x027*4
#define PCIe_Phy_MASSWR_RX_EPI_ILL_CAL_OVRD               0xA000*2 + 0x042*4
#define PCIe_Phy_MASSWR_RX_EPI_ILL_LOCK_REFCNT_START      0xA000*2 + 0x045*4
#define PCIe_Phy_MASSWR_RX_EPI_ILL_LOCK_CALCNT_START0     0xA000*2 + 0x046*4
#define PCIe_Phy_MASSWR_RX_EPI_ILL_LOCK_CALCNT_START1     0xA000*2 + 0x047*4
/* #define PCIe_Phy_LANE_CAL_RESET_TIME_VALUE            register wasn`t found */
#define PCIe_Phy_CMN_TXPUCAL_OVRD                         0x0000*2 + 0x0C1*4
#define PCIe_Phy_CMN_TXPDCAL_OVRD                         0x0000*2 + 0x0E1*4
#define PCIe_Phy_CMN_RXCAL_OVRD                           0x0000*2 + 0x101*4
#define PCIe_Phy_MASSWR_RX_SLC_INIT_TMR                   0xA000*2 + 0x14D*4
#define PCIe_Phy_MASSWR_RX_SLC_RUN_TMR                    0xA000*2 + 0x14E*4
/* #define PCIe_Phy_CMN_PRPLL_LOCK_CNT_START             register wasn`t found */

#define PCIe_Phy_PMA_CMN_PID_TYPE                     0x0000*2 + 0x000*4
#define PCIe_Phy_PMA_CMN_PID_NUM                      0x0000*2 + 0x003*4
#define PCIe_Phy_PMA_CMN_PID_REV                      0x0000*2 + 0x004*4
#define PCIe_Phy_PMA_CMN_PID_MFG                      0x0000*2 + 0x008*4
#define PCIe_Phy_PMA_CMN_PID_NODE                     0x0000*2 + 0x009*4
#define PCIe_Phy_PMA_CMN_PID_FLV                      0x0000*2 + 0x00A*4
#define PCIe_Phy_PMA_CMN_PID_IOV                      0x0000*2 + 0x00C*4
#define PCIe_Phy_PMA_CMN_PID_METAL                    0x0000*2 + 0x00D*4
#define PCIe_Phy_PMA_CMN_PID_LANES                    0x0000*2 + 0x010*4
#define PCIe_Phy_PMA_CMN_SSM_BANDGAP_TMR              0x0000*2 + 0x020*4
#define PCIe_Phy_PMA_CMN_SSM_BIAS_TMR                 0x0000*2 + 0x021*4
#define PCIe_Phy_PMA_CMN_SSM_PLLEN_TMR                0x0000*2 + 0x022*4
#define PCIe_Phy_PMA_CMN_SSM_PLLVREF_TMR              0x0000*2 + 0x024*4
#define PCIe_Phy_PMA_CMN_SSM_LANECAL_TMR              0x0000*2 + 0x028*4
#define PCIe_Phy_PMA_CMN_SSM_A1OUT_TMR                0x0000*2 + 0x021*4
#define PCIe_Phy_PMA_CMN_PSC_A0                       0x0000*2 + 0x040*4
#define PCIe_Phy_PMA_CMN_PSC_A1                       0x0000*2 + 0x041*4
#define PCIe_Phy_PMA_CMN_PSC_A2                       0x0000*2 + 0x042*4
#define PCIe_Phy_PMA_CMN_PSC_A4                       0x0000*2 + 0x044*4
#define PCIe_Phy_PMA_CMN_PLLCPICAL_CTRL               0x0000*2 + 0x080*4
#define PCIe_Phy_PMA_CMN_PLLCPICAL_OVRD               0x0000*2 + 0x081*4
#define PCIe_Phy_PMA_CMN_PLLCPICAL_INIT_TMR           0x0000*2 + 0x082*4
#define PCIe_Phy_PMA_CMN_RXCAL_CTRL                   0x0000*2 + 0x100*4
#define PCIe_Phy_PMA_CMN_RXCAL_OVRD                   0x0000*2 + 0x101*4
#define PCIe_Phy_PMA_CMN_RXCAL_INIT_TMR               0x0000*2 + 0x102*4
#define PCIe_Phy_PMA_CMN_TXPUCAL_CTRL                 0x0000*2 + 0x0C0*4
#define PCIe_Phy_PMA_CMN_TXPUCAL_OVRD                 0x0000*2 + 0x0C1*4
#define PCIe_Phy_PMA_CMN_TXPUCAL_INIT_TMR             0x0000*2 + 0x0C2*4
#define PCIe_Phy_PMA_CMN_TXPDCAL_CTRL                 0x0000*2 + 0x0E0*4
#define PCIe_Phy_PMA_CMN_TXPDCAL_OVRD                 0x0000*2 + 0x0E1*4
#define PCIe_Phy_PMA_CMN_TXPDCAL_INIT_TMR             0x0000*2 + 0x0E2*4
#define PCIe_Phy_PMA_TX_HBDC_PRE_OVRD                 0x4000*2 + 0x000*4
#define PCIe_Phy_PMA_TX_HBDC_MAIN_OVRD                0x4000*2 + 0x001*4
#define PCIe_Phy_PMA_TX_HBDC_POST_OVRD                0x4000*2 + 0x002*4
#define PCIe_Phy_PMA_TX_HBDC_PRE_PREM_OVRD            0x4000*2 + 0x004*4
#define PCIe_Phy_PMA_TX_HBDC_MAIN_INITM_OVRD          0x4000*2 + 0x008*4
#define PCIe_Phy_PMA_TX_RCVDET_CTRL                   0x4000*2 + 0x020*4
#define PCIe_Phy_PMA_TX_BIST_CTRL                     0x4000*2 + 0x040*4
#define PCIe_Phy_PMA_TX_BIST_UDDWR                    0x4000*2 + 0x041*4
#define PCIe_Phy_PMA_RX_IQPI_ILL_CAL_CTRL             0x8000*2 + 0x020*4
#define PCIe_Phy_PMA_RX_IQPI_ILL_CAL_START            0x8000*2 + 0x021*4
#define PCIe_Phy_PMA_RX_IQPI_ILL_CAL_INIT_TMR         0x8000*2 + 0x023*4
#define PCIe_Phy_PMA_RX_IQPI_ILL_CAL_ITER_TMR         0x8000*2 + 0x024*4
#define PCIe_Phy_PMA_RX_IQPI_ILL_LOCK_CALCNT_START0   0x8000*2 + 0x026*4
#define PCIe_Phy_PMA_RX_EPI_ILL_CAL_CTRL              0x8000*2 + 0x040*4
#define PCIe_Phy_PMA_RX_EPI_ILL_CAL_START             0x8000*2 + 0x041*4
#define PCIe_Phy_PMA_RX_EPI_ILL_CAL_INIT_TMR          0x8000*2 + 0x043*4
#define PCIe_Phy_PMA_RX_EPI_ILL_CAL_ITER_TMR          0x8000*2 + 0x044*4
#define PCIe_Phy_PMA_RX_EPI_ILL_LOCK_CALCNT_START0    0x8000*2 + 0x046*4
#define PCIe_Phy_PMA_RX_CDRLF_CNFG                    0x8000*2 + 0x080*4
#define PCIe_Phy_PMA_RX_CDRLF_CNFG2                   0x8000*2 + 0x081*4
#define PCIe_Phy_PMA_RX_CDRLF_MGN_DIAG                0x8000*2 + 0x082*4
#define PCIe_Phy_PMA_RX_SIGDET_FILT_TMR               0x8000*2 + 0x160*4
#define PCIe_Phy_PMA_RX_SIGDET_DLY_TMR                0x8000*2 + 0x161*4
#define PCIe_Phy_PMA_RX_SIGDET_MIN_TMR                0x8000*2 + 0x162*4
/* --- PCIe Core --- */

#define PCIe_Core_FuncRPConfig  0x00000000
#define PCIe_Core_LocalMgmt     0x00001000
#define PCIe_Core_AXIConfig     0x00004000
#define PCIe_Core_DMAConfig     0x00006000

#define PCIe_EP_i_vendor_id_device_id                    0x0000
#define PCIe_EP_i_command_status                         0x0004
#define PCIe_EP_i_revision_id_class_code                 0x0008
#define PCIe_EP_i_bist_header_latency_cache_line         0x000C
#define PCIe_EP_i_base_addr_0                            0x0010
#define PCIe_EP_i_base_addr_1                            0x0014
#define PCIe_EP_i_base_addr_2                            0x0018
#define PCIe_EP_i_base_addr_3                            0x001C
#define PCIe_EP_i_base_addr_4                            0x0020
#define PCIe_EP_i_base_addr_5                            0x0024
#define PCIe_EP_i_subsystem_vendor_id_subsystem_i        0x002C
#define PCIe_EP_i_capabilities_pointer                   0x0034
#define PCIe_EP_i_intrpt_line_intrpt_pin                 0x003C
#define PCIe_EP_i_pwr_mgmt_cap                           0x0080
#define PCIe_EP_i_pwr_mgmt_ctrl_stat_rep                 0x0084
#define PCIe_EP_i_msi_ctrl_reg                           0x0090
#define PCIe_EP_i_msi_msg_low_addr                       0x0094
#define PCIe_EP_i_msi_msg_hi_addr                        0x0098
#define PCIe_EP_i_msi_msg_data                           0x009C
#define PCIe_EP_i_msi_mask                               0x00A0
#define PCIe_EP_i_msi_pending_bits                       0x00A4
#define PCIe_EP_i_msix_ctrl                              0x00B0
#define PCIe_EP_i_msix_tbl_offset                        0x00B4
#define PCIe_EP_i_msix_pending_intrpt                    0x00B8
#define PCIe_EP_i_pcie_cap_list                          0x00C0
#define PCIe_EP_i_pcie_dev_cap                           0x00C4
#define PCIe_EP_i_pcie_dev_ctrl_status                   0x00C8
#define PCIe_EP_i_link_cap                               0x00CC
#define PCIe_EP_i_link_ctrl_status                       0x00D0
#define PCIe_EP_i_pcie_dev_cap_2                         0x00E4
#define PCIe_EP_i_pcie_dev_ctrl_status_2                 0x00E8
#define PCIe_EP_i_link_cap_2_reg                         0x00EC
#define PCIe_EP_i_link_ctrl_status_2                     0x00F0
#define PCIe_EP_i_AER_enhanced_cap_hdr                   0x0100
#define PCIe_EP_i_uncorr_err_status                      0x0104
#define PCIe_EP_i_uncorr_err_mask                        0x0108
#define PCIe_EP_i_uncorr_err_severity                    0x010C
#define PCIe_EP_i_corr_err_status                        0x0110
#define PCIe_EP_i_corr_err_mask                          0x0114
#define PCIe_EP_i_advcd_err_cap_ctrl                     0x0118
#define PCIe_EP_i_hdr_log_0                              0x011C
#define PCIe_EP_i_hdr_log_1                              0x0120
#define PCIe_EP_i_hdr_log_2                              0x0124
#define PCIe_EP_i_hdr_log_3                              0x0128

#define PCIe_RP_i_vendor_id_device_id                    0x0000
#define PCIe_RP_i_command_status                         0x0004
#define PCIe_RP_i_revision_id_class_code                 0x0008
#define PCIe_RP_i_bist_header_latency_cache_line         0x000C
#define PCIe_RP_i_RC_BAR_0                               0x0010
#define PCIe_RP_i_RC_BAR_1                               0x0014
#define PCIe_RP_i_pcie_bus_numbers                       0x0018
#define PCIe_RP_i_pcie_io_base_limit                     0x001C
#define PCIe_RP_i_pcie_mem_base_limit                    0x0020
#define PCIe_RP_i_pcie_prefetch_base_limit               0x0024
#define PCIe_RP_i_pcie_prefetch_base_upper               0x0028
#define PCIe_RP_i_pcie_prefetch_limit_upper              0x002C
#define PCIe_RP_i_pcie_io_base_limit_upper               0x0030
#define PCIe_RP_i_capabilities_pointer                   0x0034
#define PCIe_RP_i_intrpt_line_intrpt_pin                 0x003C
#define PCIe_RP_i_pwr_mgmt_cap                           0x0080
#define PCIe_RP_i_pwr_mgmt_ctrl_stat_rep                 0x0084
#define PCIe_RP_i_msi_ctrl_reg                           0x0090
#define PCIe_RP_i_msi_msg_low_addr                       0x0094
#define PCIe_RP_i_msi_msg_hi_addr                        0x0098
#define PCIe_RP_i_msi_msg_data                           0x009C
#define PCIe_RP_i_msi_mask                               0x00A0
#define PCIe_RP_i_msi_pending_bits                       0x00A4
#define PCIe_RP_i_msix_ctrl                              0x00B0
#define PCIe_RP_i_msix_tbl_offset                        0x00B4
#define PCIe_RP_i_msix_pending_intrpt                    0x00B8
#define PCIe_RP_i_pcie_cap_list                          0x00C0
#define PCIe_RP_i_pcie_cap                               0x00C4
#define PCIe_RP_i_pcie_dev_ctrl_status                   0x00C8
#define PCIe_RP_i_link_cap                               0x00CC
#define PCIe_RP_i_link_ctrl_status                       0x00D0
#define PCIe_RP_i_slot_ctrl_status                       0x00D8
#define PCIe_RP_i_root_ctrl_cap                          0x00DC
#define PCIe_RP_i_root_status                            0x00E0
#define PCIe_RP_i_pcie_cap_2                             0x00E4
#define PCIe_RP_i_pcie_dev_ctrl_status_2                 0x00E8
#define PCIe_RP_i_link_cap_2                             0x00EC
#define PCIe_RP_i_link_ctrl_status_2                     0x00F0
#define PCIe_RP_i_AER_enhncd_cap                         0x0100
#define PCIe_RP_i_uncorr_err_status                      0x0104
#define PCIe_RP_i_uncorr_err_mask                        0x0108
#define PCIe_RP_i_uncorr_err_severity                    0x010C
#define PCIe_RP_i_corr_err_status                        0x0110
#define PCIe_RP_i_corr_err_mask                          0x0114
#define PCIe_RP_i_adv_err_cap_ctl                        0x0118
#define PCIe_RP_i_hdr_log_0                              0x011C
#define PCIe_RP_i_hdr_log_1                              0x0120
#define PCIe_RP_i_hdr_log_2                              0x0124
#define PCIe_RP_i_hdr_log_3                              0x0128
#define PCIe_RP_i_root_err_cmd                           0x012C
#define PCIe_RP_i_root_err_stat                          0x0130
#define PCIe_RP_i_err_src_id                             0x0134

#define PCIe_LocMgmt_i_pl_config_0_reg                         0x0000
#define PCIe_LocMgmt_i_pl_config_1_reg                         0x0004
#define PCIe_LocMgmt_i_dll_tmr_config_reg                      0x0008
#define PCIe_LocMgmt_i_rcv_cred_lim_0_reg                      0x000C
#define PCIe_LocMgmt_i_rcv_cred_lim_1_reg                      0x0010
#define PCIe_LocMgmt_i_transm_cred_lim_0_reg                   0x0014
#define PCIe_LocMgmt_i_transm_cred_lim_1_reg                   0x0018
#define PCIe_LocMgmt_i_transm_cred_update_int_config_0_reg     0x001C
#define PCIe_LocMgmt_i_transm_cred_update_int_config_1_reg     0x0020
#define PCIe_LocMgmt_i_L0S_timeout_limit_reg                   0x0024
#define PCIe_LocMgmt_i_transmit_tlp_count_reg                  0x0028
#define PCIe_LocMgmt_i_transmit_tlp_payload_dword_count_reg    0x002C
#define PCIe_LocMgmt_i_receive_tlp_count_reg                   0x0030
#define PCIe_LocMgmt_i_receive_tlp_payload_dword_count_reg     0x0034
#define PCIe_LocMgmt_i_compln_tmout_lim_0_reg                  0x0038
#define PCIe_LocMgmt_i_compln_tmout_lim_1_reg                  0x003C
#define PCIe_LocMgmt_i_L1_st_reentry_delay_reg                 0x0040
#define PCIe_LocMgmt_i_vendor_id_reg                           0x0044
#define PCIe_LocMgmt_i_aspm_L1_entry_tmout_delay_reg           0x0048
#define PCIe_LocMgmt_i_pme_turnoff_ack_delay_reg               0x004C
#define PCIe_LocMgmt_i_linkwidth_control_reg                   0x0050
#define PCIe_LocMgmt_i_sris_control_reg                        0x0074
#define PCIe_LocMgmt_i_shdw_hdr_log_0_reg                      0x0100
#define PCIe_LocMgmt_i_shdw_hdr_log_1_reg                      0x0104
#define PCIe_LocMgmt_i_shdw_hdr_log_2_reg                      0x0108
#define PCIe_LocMgmt_i_shdw_hdr_log_3_reg                      0x010C
#define PCIe_LocMgmt_i_shdw_func_num_reg                       0x0110
#define PCIe_LocMgmt_i_shdw_ur_err_reg                         0x0114
#define PCIe_LocMgmt_i_debug_dllp_count_gen1_reg               0x0144
#define PCIe_LocMgmt_i_debug_dllp_count_gen2_reg               0x0148
#define PCIe_LocMgmt_i_debug_dllp_count_gen3_reg               0x014C
#define PCIe_LocMgmt_i_negotiated_lane_map_reg                 0x0200
#define PCIe_LocMgmt_i_receive_fts_count_reg                   0x0204
#define PCIe_LocMgmt_i_debug_mux_control_reg                   0x0208
#define PCIe_LocMgmt_i_local_error_status_register             0x020C
#define PCIe_LocMgmt_i_local_intrpt_mask_reg                   0x0210
#define PCIe_LocMgmt_i_lcrc_err_count_reg                      0x0214
#define PCIe_LocMgmt_i_ecc_corr_err_count_reg                  0x0218
#define PCIe_LocMgmt_i_pme_service_timeout_delay_reg           0x0224
#define PCIe_LocMgmt_i_root_port_requestor_id_reg              0x0228
#define PCIe_LocMgmt_i_ep_bus_device_number_reg                0x022C
#define PCIe_LocMgmt_i_pf_0_BAR_config_0_reg                   0x0240
#define PCIe_LocMgmt_i_pf_0_BAR_config_1_reg                   0x0244
#define PCIe_LocMgmt_i_pf_config_reg                           0x02C0
#define PCIe_LocMgmt_i_rc_BAR_config_reg                       0x0300
#define PCIe_LocMgmt_i_ecc_corr_err_count_reg_axi              0x0C80
#define PCIe_LocMgmt_low_power_debug_and_control0              0x0C88
#define PCIe_LocMgmt_low_power_debug_and_control1              0x0C8C
#define PCIe_LocMgmt_tl_internal_control                       0x0C94
#define PCIe_LocMgmt_i_ld_ctrl                                 0x0DA0
#define PCIe_LocMgmt_rx_elec_idle_filter_control               0x0DA4

#define PCIe_AXI_outregion_0_addr_translation_0                0x0000
#define PCIe_AXI_outregion_0_addr_translation_1                0x0004
#define PCIe_AXI_outregion_0_header_descriptor_0               0x0008
#define PCIe_AXI_outregion_0_header_descriptor_1               0x000C
#define PCIe_AXI_outregion_0_region_base_addr                  0x0018
#define PCIe_AXI_outregion_1_addr_translation_0                0x0020
#define PCIe_AXI_outregion_1_addr_translation_1                0x0024
#define PCIe_AXI_outregion_1_header_descriptor_0               0x0028
#define PCIe_AXI_outregion_1_header_descriptor_1               0x002C
#define PCIe_AXI_outregion_1_region_base_addr                  0x0038
#define PCIe_AXI_outregion_2_addr_translation_0                0x0040
#define PCIe_AXI_outregion_2_addr_translation_1                0x0044
#define PCIe_AXI_outregion_2_header_descriptor_0               0x0048
#define PCIe_AXI_outregion_2_header_descriptor_1               0x004C
#define PCIe_AXI_outregion_2_region_base_addr                  0x0058
#define PCIe_AXI_outregion_3_addr_translation_0                0x0060
#define PCIe_AXI_outregion_3_addr_translation_1                0x0064
#define PCIe_AXI_outregion_3_header_descriptor_0               0x0068
#define PCIe_AXI_outregion_3_header_descriptor_1               0x006C
#define PCIe_AXI_outregion_3_region_base_addr                  0x0078
#define PCIe_AXI_outregion_4_addr_translation_0                0x0080
#define PCIe_AXI_outregion_4_addr_translation_1                0x0084
#define PCIe_AXI_outregion_4_header_descriptor_0               0x0088
#define PCIe_AXI_outregion_4_header_descriptor_1               0x008C
#define PCIe_AXI_outregion_4_region_base_addr                  0x0098
#define PCIe_AXI_outregion_5_addr_translation_0                0x00A0
#define PCIe_AXI_outregion_5_addr_translation_1                0x00A4
#define PCIe_AXI_outregion_5_header_descriptor_0               0x00A8
#define PCIe_AXI_outregion_5_header_descriptor_1               0x00AC
#define PCIe_AXI_outregion_5_region_base_addr                  0x00B8
#define PCIe_AXI_outregion_6_addr_translation_0                0x00C0
#define PCIe_AXI_outregion_6_addr_translation_1                0x00C4
#define PCIe_AXI_outregion_6_header_descriptor_0               0x00C8
#define PCIe_AXI_outregion_6_header_descriptor_1               0x00CC
#define PCIe_AXI_outregion_6_region_base_addr                  0x00D8
#define PCIe_AXI_outregion_7_addr_translation_0                0x00E0
#define PCIe_AXI_outregion_7_addr_translation_1                0x00E4
#define PCIe_AXI_outregion_7_header_descriptor_0               0x00E8
#define PCIe_AXI_outregion_7_header_descriptor_1               0x00EC
#define PCIe_AXI_outregion_7_region_base_addr                  0x00F8
#define PCIe_AXI_outregion_8_addr_translation_0                0x0100
#define PCIe_AXI_outregion_8_addr_translation_1                0x0104
#define PCIe_AXI_outregion_8_header_descriptor_0               0x0108
#define PCIe_AXI_outregion_8_header_descriptor_1               0x010C
#define PCIe_AXI_outregion_8_region_base_addr                  0x0118
#define PCIe_AXI_outregion_9_addr_translation_0                0x0120
#define PCIe_AXI_outregion_9_addr_translation_1                0x0124
#define PCIe_AXI_outregion_9_header_descriptor_0               0x0128
#define PCIe_AXI_outregion_9_header_descriptor_1               0x012C
#define PCIe_AXI_outregion_9_region_base_addr                  0x0138
#define PCIe_AXI_outregion_10_addr_translation_0               0x0140
#define PCIe_AXI_outregion_10_addr_translation_1               0x0144
#define PCIe_AXI_outregion_10_header_descriptor_0              0x0148
#define PCIe_AXI_outregion_10_header_descriptor_1              0x014C
#define PCIe_AXI_outregion_10_region_base_addr                 0x0158
#define PCIe_AXI_outregion_11_addr_translation_0               0x0160
#define PCIe_AXI_outregion_11_addr_translation_1               0x0164
#define PCIe_AXI_outregion_11_header_descriptor_0              0x0168
#define PCIe_AXI_outregion_11_header_descriptor_1              0x016C
#define PCIe_AXI_outregion_11_region_base_addr                 0x0178
#define PCIe_AXI_outregion_12_addr_translation_0               0x0180
#define PCIe_AXI_outregion_12_addr_translation_1               0x0184
#define PCIe_AXI_outregion_12_header_descriptor_0              0x0188
#define PCIe_AXI_outregion_12_header_descriptor_1              0x018C
#define PCIe_AXI_outregion_12_region_base_addr                 0x0198
#define PCIe_AXI_outregion_13_addr_translation_0               0x01A0
#define PCIe_AXI_outregion_13_addr_translation_1               0x01A4
#define PCIe_AXI_outregion_13_header_descriptor_0              0x01A8
#define PCIe_AXI_outregion_13_header_descriptor_1              0x01AC
#define PCIe_AXI_outregion_13_region_base_addr                 0x01B8
#define PCIe_AXI_outregion_14_addr_translation_0               0x01C0
#define PCIe_AXI_outregion_14_addr_translation_1               0x01C4
#define PCIe_AXI_outregion_14_header_descriptor_0              0x01C8
#define PCIe_AXI_outregion_14_header_descriptor_1              0x01CC
#define PCIe_AXI_outregion_14_region_base_addr                 0x01D8
#define PCIe_AXI_outregion_15_addr_translation_0               0x01E0
#define PCIe_AXI_outregion_15_addr_translation_1               0x01E4
#define PCIe_AXI_outregion_15_header_descriptor_0              0x01E8
#define PCIe_AXI_outregion_15_header_descriptor_1              0x01EC
#define PCIe_AXI_outregion_15_region_base_addr                 0x01F8
#define PCIe_AXI_outregion_16_addr_translation_0               0x0200
#define PCIe_AXI_outregion_16_addr_translation_1               0x0204
#define PCIe_AXI_outregion_16_header_descriptor_0              0x0208
#define PCIe_AXI_outregion_16_header_descriptor_1              0x020C
#define PCIe_AXI_outregion_16_region_base_addr                 0x0218
#define PCIe_AXI_outregion_17_addr_translation_0               0x0220
#define PCIe_AXI_outregion_17_addr_translation_1               0x0224
#define PCIe_AXI_outregion_17_header_descriptor_0              0x0228
#define PCIe_AXI_outregion_17_header_descriptor_1              0x022C
#define PCIe_AXI_outregion_17_region_base_addr                 0x0238
#define PCIe_AXI_outregion_18_addr_translation_0               0x0240
#define PCIe_AXI_outregion_18_addr_translation_1               0x0244
#define PCIe_AXI_outregion_18_header_descriptor_0              0x0248
#define PCIe_AXI_outregion_18_header_descriptor_1              0x024C
#define PCIe_AXI_outregion_18_region_base_addr                 0x0258
#define PCIe_AXI_outregion_19_addr_translation_0               0x0260
#define PCIe_AXI_outregion_19_addr_translation_1               0x0264
#define PCIe_AXI_outregion_19_header_descriptor_0              0x0268
#define PCIe_AXI_outregion_19_header_descriptor_1              0x026C
#define PCIe_AXI_outregion_19_region_base_addr                 0x0278
#define PCIe_AXI_outregion_20_addr_translation_0               0x0280
#define PCIe_AXI_outregion_20_addr_translation_1               0x0284
#define PCIe_AXI_outregion_20_header_descriptor_0              0x0288
#define PCIe_AXI_outregion_20_header_descriptor_1              0x028C
#define PCIe_AXI_outregion_20_region_base_addr                 0x0298
#define PCIe_AXI_outregion_21_addr_translation_0               0x02A0
#define PCIe_AXI_outregion_21_addr_translation_1               0x02A4
#define PCIe_AXI_outregion_21_header_descriptor_0              0x02A8
#define PCIe_AXI_outregion_21_header_descriptor_1              0x02AC
#define PCIe_AXI_outregion_21_region_base_addr                 0x02B8
#define PCIe_AXI_outregion_22_addr_translation_0               0x02C0
#define PCIe_AXI_outregion_22_addr_translation_1               0x02C4
#define PCIe_AXI_outregion_22_header_descriptor_0              0x02C8
#define PCIe_AXI_outregion_22_header_descriptor_1              0x02CC
#define PCIe_AXI_outregion_22_region_base_addr                 0x02D8
#define PCIe_AXI_outregion_23_addr_translation_0               0x02E0
#define PCIe_AXI_outregion_23_addr_translation_1               0x02E4
#define PCIe_AXI_outregion_23_header_descriptor_0              0x02E8
#define PCIe_AXI_outregion_23_header_descriptor_1              0x02EC
#define PCIe_AXI_outregion_23_region_base_addr                 0x02F8
#define PCIe_AXI_outregion_24_addr_translation_0               0x0300
#define PCIe_AXI_outregion_24_addr_translation_1               0x0304
#define PCIe_AXI_outregion_24_header_descriptor_0              0x0308
#define PCIe_AXI_outregion_24_header_descriptor_1              0x030C
#define PCIe_AXI_outregion_24_region_base_addr                 0x0318
#define PCIe_AXI_outregion_25_addr_translation_0               0x0320
#define PCIe_AXI_outregion_25_addr_translation_1               0x0324
#define PCIe_AXI_outregion_25_header_descriptor_0              0x0328
#define PCIe_AXI_outregion_25_header_descriptor_1              0x032C
#define PCIe_AXI_outregion_25_region_base_addr                 0x0338
#define PCIe_AXI_outregion_26_addr_translation_0               0x0340
#define PCIe_AXI_outregion_26_addr_translation_1               0x0344
#define PCIe_AXI_outregion_26_header_descriptor_0              0x0348
#define PCIe_AXI_outregion_26_header_descriptor_1              0x034C
#define PCIe_AXI_outregion_26_region_base_addr                 0x0358
#define PCIe_AXI_outregion_27_addr_translation_0               0x0360
#define PCIe_AXI_outregion_27_addr_translation_1               0x0364
#define PCIe_AXI_outregion_27_header_descriptor_0              0x0368
#define PCIe_AXI_outregion_27_header_descriptor_1              0x036C
#define PCIe_AXI_outregion_27_region_base_addr                 0x0378
#define PCIe_AXI_outregion_28_addr_translation_0               0x0380
#define PCIe_AXI_outregion_28_addr_translation_1               0x0384
#define PCIe_AXI_outregion_28_header_descriptor_0              0x0388
#define PCIe_AXI_outregion_28_header_descriptor_1              0x038C
#define PCIe_AXI_outregion_28_region_base_addr                 0x0398
#define PCIe_AXI_outregion_29_addr_translation_0               0x03A0
#define PCIe_AXI_outregion_29_addr_translation_1               0x03A4
#define PCIe_AXI_outregion_29_header_descriptor_0              0x03A8
#define PCIe_AXI_outregion_29_header_descriptor_1              0x03AC
#define PCIe_AXI_outregion_29_region_base_addr                 0x03B8
#define PCIe_AXI_outregion_30_addr_translation_0               0x03C0
#define PCIe_AXI_outregion_30_addr_translation_1               0x03C4
#define PCIe_AXI_outregion_30_header_descriptor_0              0x03C8
#define PCIe_AXI_outregion_30_header_descriptor_1              0x03CC
#define PCIe_AXI_outregion_30_region_base_addr                 0x03D8
#define PCIe_AXI_outregion_31_addr_translation_0               0x03E0
#define PCIe_AXI_outregion_31_addr_translation_1               0x03E4
#define PCIe_AXI_outregion_31_header_descriptor_0              0x03E8
#define PCIe_AXI_outregion_31_header_descriptor_1              0x03EC
#define PCIe_AXI_outregion_31_region_base_addr                 0x03F8
#define PCIe_AXI_inregion_rc_bar_0_addr_translation            0x0800
#define PCIe_AXI_inregion_rc_bar_1_addr_translation            0x0808
#define PCIe_AXI_inregion_rc_bar_2_addr_translation            0x0810
#define PCIe_AXI_l0                                            0x0824
#define PCIe_AXI_inregion_ep_bar_0_addr_translation            0x0840
#define PCIe_AXI_inregion_ep_bar_1_addr_translation            0x0848
#define PCIe_AXI_inregion_ep_bar_2_addr_translation            0x0850
#define PCIe_AXI_inregion_ep_bar_3_addr_translation            0x0858
#define PCIe_AXI_inregion_ep_bar_4_addr_translation            0x0860
#define PCIe_AXI_inregion_ep_bar_5_addr_translation            0x0868
#define PCIe_AXI_inregion_ep_bar_6_addr_translation            0x0870

#define PCIe_DMA_channel_0_ctrl                                0x0000
#define PCIe_DMA_channel_0_sp_l                                0x0004
#define PCIe_DMA_channel_0_sp_u                                0x0008
#define PCIe_DMA_channel_0_attr_l                              0x000C
#define PCIe_DMA_channel_0_attr_u                              0x0010
#define PCIe_DMA_channel_1_ctrl                                0x0014
#define PCIe_DMA_channel_1_sp_l                                0x0018
#define PCIe_DMA_channel_1_sp_u                                0x001C
#define PCIe_DMA_channel_1_attr_l                              0x0020
#define PCIe_DMA_channel_1_attr_u                              0x0024
#define PCIe_DMA_common_udma_int                               0x00A0
#define PCIe_DMA_common_udma_int_ena                           0x00A4
#define PCIe_DMA_common_udma_int_dis                           0x00A8
#define PCIe_DMA_common_udma_ib_ecc_uncorrectable_errors       0x00AC
#define PCIe_DMA_common_udma_ib_ecc_correctable_errors         0x00B0
#define PCIe_DMA_common_udma_ob_ecc_uncorrectable_errors       0x00B4
#define PCIe_DMA_common_udma_ob_ecc_correctable_errors         0x00B8
#define PCIe_DMA_common_udma_cap_ver                           0x00F8
#define PCIe_DMA_common_udma_config                            0x00FC
