#ifndef MIVEM_MEM_MAP_H
#define MIVEM_MEM_MAP_H


#define BOOTROM_BASE            0xFFFC0000
#define BOOTROM_SIZE            0x00040000

#define BOOTROM_MIRROR_BASE     0x00000000
#define BOOTROM_MIRROR_SIZE     0x00040000

#define IM0_BASE                0x00040000
#define IM0_SIZE                0x00040000

#define EM0_BASE                0x40000000
#define EM0_SIZE                0x40000000

#define EM1_BASE                0x80000000
#define EM1_SIZE                0x40000000

#define EM2_BASE                0xC0000000
#define EM2_SIZE                0x08000000

#define EM3_BASE                0xD0000000
#define EM3_SIZE                0x10000000

//UART base address
#define UART0_BASE              0x3C05D000 
#define UART1_BASE              0x3C05E000 
#define UART2_BASE              0x3C05F000 

//I2C base address
#define I2C0_BASE               0x3C05A000 
#define I2C1_BASE               0x3C05B000 
#define I2C2_BASE               0x3C05C000 

//GMII base addresses
#define GMII0_BASE              0x3C033000
#define GRETH_GBIT0_BASE        GMII0_BASE
#define GMII1_BASE              0x3C034000
#define GRETH_GBIT1_BASE        GMII1_BASE

//RMII base address
//#define RMII_BASE               0x3C033000
//#define GRETH_BASE              RMII_BASE

//VDU base addresses
#define VDU0_MAIN_BASE          0x38011000
#define VDU0_OSD_BASE           0x38012000
#define VDU1_MAIN_BASE          0x38013000
#define VDU1_OSD_BASE           0x38014000
#define GRAB0_INT_BASE          0x38015000
#define GRAB0_EXT_BASE          0x38016000
#define GRAB1_INT_BASE          0x3801b000
#define GRAB1_EXT_BASE          0x3801c000

//CODA_V base address
#define CODA_V_BASE             0x38020000

//CODA_J base address
#define CODA_J_BASE             0x3801a000

//AUDIO_DMA base address
#define AUD_DMA_BASE            0x38017000

//I2S base address
#define I2S_BASE                0x39080000

//SPDIF base address
#define SPDIF_BASE              0x39081000

//GSPI_SDIO base address
#define SDIO_BASE               0x3C064000 
#define GSPI_BASE               0x3C063000 

#define SPI_CTRL0_BASE         0x3C061000
#define SPI_CTRL1_BASE         0x3C062000

//GPIO base address
#define GPIO0_BASE              0x3C065000
#define GPIO1_BASE              0x3C066000

#define LSIF0_MGPIO0_BASE        0x3C040000
#define LSIF0_MGPIO1_BASE        0x3C041000
#define LSIF0_MGPIO2_BASE        0x3C042000
#define LSIF0_MGPIO3_BASE        0x3C043000
#define LSIF0_MGPIO4_BASE        0x3C044000
#define LSIF0_MGPIO5_BASE        0x3C045000
#define LSIF0_MGPIO6_BASE        0x3C046000
#define LSIF0_MGPIO7_BASE        0x3C047000
#define LSIF0_MGPIO8_BASE        0x3C048000
#define LSIF0_MGPIO9_BASE        0x3C049000
#define LSIF0_MGPIO10_BASE       0x3C04A000

#define LSIF1_MGPIO0_BASE        0x3C067000
#define LSIF1_MGPIO1_BASE        0x3C068000
#define LSIF1_MGPIO2_BASE        0x3C069000
#define LSIF1_MGPIO3_BASE        0x3C06A000
#define LSIF1_MGPIO4_BASE        0x3C06B000

//System Timer (APB)
#define TIMER_BASE              0x38001000

//WD
#define WD_BASE                 0x38002000

//Sensors
#define TVSENS_BASE             0x38003000


//DDR modules base addresses
#define EM0_PLB6MCIF2_DCR_BASE  0x80010000
#define EM0_AXIMCIF2_DCR_BASE   0x80020000
#define EM0_MCLFIR_DCR_BASE     0x80030000
#define EM0_MCIF2ARB_DCR_BASE   0x80040000
#define EM0_DDR3LMC_DCR_BASE    0x80050000
#define EM0_PHY_DCR_BASE        0x3800E000

#define EM1_PLB6MCIF2_DCR_BASE  0x80100000
#define EM1_AXIMCIF2_DCR_BASE   0x80110000
#define EM1_MCLFIR_DCR_BASE     0x80120000
#define EM1_MCIF2ARB_DCR_BASE   0x80130000
#define EM1_DDR3LMC_DCR_BASE    0x80140000
#define EM1_PHY_DCR_BASE        0x3800F000

#define EM2_SRAM_PLB6MCIF2_DCR_BASE      0x80160000
#define EM2_SRAM_DCRAPB_DCR_BASE         0x80170000
#define EM3_NOR_PLB6MCIF2_DCR_BASE       0x80180000
#define EM3_NOR_DCRAPB_DCR_BASE          0x80190000

#define NAND_BASE               0x3C032000 

#define L2C0_DCR_BASE           0x80000600
#define L2C1_DCR_BASE           0x80000700

#define PMU0_DCR_BASE           0x80000600 //need to set
#define PMU1_DCR_BASE           0x80000700 //need to set

#define DCR_MPIC128_BASE        0xFFC00000

#define PLB6PLB4_0_BASE         0x80000300
#define PLB6PLB4_1_BASE         0x80000400
#define PLB6PLB4_2_BASE         0x80000E00
#define PLB6PLB4_3_BASE         0x80000F00


#define SRAMMC2PLB4_0_BASE      0x00000030
#define SRAMMC2PLB4_1_BASE      0x00000040

//PLB6 BUS CTRL
#define PLB6_BC_BASE            0x80000200

//PLB4ARB
#define PLB4ARB8M_0_BASE        0x00000010
#define PLB4ARB8M_1_BASE        0x00000020
#define PLB4ARB8M_2_BASE        0x00000060
#define PLB4ARB8M_3_BASE        0x00000070
#define PLB4ARB8M_4_BASE        0x00000080
#define PLB4ARB8M_5_BASE        0x00000090
#define PLB4ARB8M_6_BASE        0x000000A0

//PLB6DMA
#define DMA2PLB6_0_BASE         0x80000100
#define DMA2PLB6_1_BASE         0x80000D00

//PLB4PLB6
#define PLB4PLB6_0_BASE         0x80000500
#define PLB4PLB6_1_BASE         0x80001000
#define PLB4PLB6_2_BASE         0x80001100

//PLB4AHB/AHBPLB4
#define PLB4AHB_0_BASE          0x00000050
#define PLB4AHB_1_BASE          0x000000B0
#define PLB4AHB_2_BASE          0x000000C0

//DCRARB
#define DCR_ARB_BASE            0x80000800

//ITRACE
#define ITRACE_BASE             0x80000900

//LTRACE
#define LTRACE0_BASE            0x80000B00
#define LTRACE1_BASE            0x80000C00

//NIC301_A_CFG
#define NIC301_A_CFG_BASE       0x38100000

//FC
#define FCx_BASE                0x3C020000

//SSP
#define SSP_CTRL0_BASE          0x3C03E000
#define SSP_CTRL1_BASE          0x3C05A000

//CRG
#define CRG_CPU_BASE            0x38006000
#define CRG_DDR_BASE            0x38007000
#define CRG_VDU_BASE            0x38008000
#define CRG_AUD_BASE            0x38009000

//Power Management Unit
#define PMU_BASE                0x38004000

//DSP Internal memory Banks
#define DSP0_NM0                                0x39000000
#define DSP0_NM1                                0x39020000
#define DSP1_NM0                                0x39040000
#define DSP1_NM1                                0x39060000

//USB 2.0 OTG
#define USB_0_BASE              0x3C06D000 

//USB PHY
#define USBPHY_CFG_BASE         0x3C06C000

//AXILSIF1 CTRL
#define AXI32LSIFS1_CTRL_BASE   0x3C900000
#define AXI32LSIFM1_CTRL_BASE   0x3CA00000

//LSIF1_CTRLREG
#define LSIF1_CTRLREG_BASE      0x3C060000 

//XHSIF
#define XHSIF0_CTRL_BASE        0x3C005000
#define XHSIF1_CTRL_BASE        0x3C015000

//SCTL
#define SCTL_BASE               0x38000000

//CLDCR
#define CLDCR_DCR_BASE          0x80001200

//SRAM_NOR
#define SRAM_NOR_0_BASE         0x3C030000
#define SRAM_NOR_1_BASE         0x3C031000

//LSCB
#define LSCB0_BASE              0x3C050000
#define LSCB1_BASE              0x3C055000

//LSIF0_CONFIG
#define LSIF0_CONFIG_BASE       0x3C03F000

//IFSYS_AXI32
#define LSIF0_MATRIX_BASE       0x3C700000
#define LSIF1_AXI32_S_BASE      0x3C900000
#define LSIF1_AXI32_M_BASE      0x3CA00000
#define HSIF_PL301_S_BASE       0x3C500000
#define HSIF_PL301_M_BASE       0x3C600000

//HSVIx
#define HSVI0_BASE              0x3C001000
#define HSVI1_BASE              0x3C002000
#define HSVI2_BASE              0x3C003000
#define HSVI3_BASE              0x3C011000
#define HSVI4_BASE              0x3C012000
#define HSVI5_BASE              0x3C013000
#define HSVI6_BASE              0x3C021000
#define XHSIF0_HSVI0_BASE       HSVI0_BASE
#define XHSIF0_HSVI1_BASE       HSVI1_BASE
#define XHSIF0_HSVI2_BASE       HSVI2_BASE
#define XHSIF1_HSVI0_BASE       HSVI3_BASE
#define XHSIF1_HSVI1_BASE       HSVI4_BASE
#define XHSIF1_HSVI2_BASE       HSVI5_BASE
#define HSIF_HSVI_BASE          HSVI6_BASE

//SERDESx
#define SERDES0_BASE            0x3CB80000
#define SERDES1_BASE            0x3CB00000
#define SERDES2_BASE            0x3CB10000
#define SERDES3_BASE            0x3CB20000
#define SERDES4_BASE            0x3CB30000
#define SERDES5_BASE            0x3CB40000
#define SERDES6_BASE            0x3CB50000
#define SERDES7_BASE            0x3CB60000
#define SERDES8_BASE            0x3CB70000
#define HSIF_SERDES_BASE        SERDES0_BASE
#define XHSIF0_SERDES0_BASE     SERDES1_BASE
#define XHSIF0_SERDES1_BASE     SERDES2_BASE
#define XHSIF0_SERDES2_BASE     SERDES3_BASE
#define XHSIF0_SERDES3_BASE     SERDES4_BASE
#define XHSIF1_SERDES0_BASE     SERDES5_BASE
#define XHSIF1_SERDES1_BASE     SERDES6_BASE
#define XHSIF1_SERDES2_BASE     SERDES7_BASE
#define XHSIF1_SERDES3_BASE     SERDES8_BASE

//RMACE
#define RMACE_BASE              0x3C024000

//HSCB
#define HSCB0_BASE              0x3C025000
#define HSCB1_BASE              0x3C026000

//HSIF_CTRL
#define HSIF_CTRL_BASE          0x3C023000

//XHSIF0_CTRL
#define XHSIF0_CTRL_BASE        0x3C005000
//XHSIF1_CTRL
#define XHSIF1_CTRL_BASE        0x3C015000

//XHSIF_PCIe
#define XHSIF0_PCIe_BASE        0x3C000000
#define XHSIF1_PCIe_BASE        0x3C010000

#define XHSIF0_PCIe_AXI_BASE    0x10000000
#define XHSIF1_PCIe_AXI_BASE    0x18000000

//XHSIF_SERDES
#define XHSIF0_SERDES_BASE      0x3CB00000
#define XHSIF1_SERDES_BASE      0x3CB40000

//PLB4AHB
#define PLB4AHB_BASE            0x00000050

#define DCR_PLB6_BC_BASE            0x80000200

#endif //MIVEM_MEM_MAP_H
