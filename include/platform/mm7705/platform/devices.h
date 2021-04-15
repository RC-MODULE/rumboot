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

//UART base address
#define UART0_BASE              0x3C034000
#define UART1_BASE              0x3C035000
#define UART2_BASE              0x3C05C000
#define UART3_BASE              0x3C05D000

//I2C base address
#define I2C0_BASE               0x3C03D000
#define I2C1_BASE               0x3C05B000
#define I2C2_BASE               0x3C05D000

//GMII base addresses
#define GMII0_BASE              0x3C050000
#define GRETH_GBIT0_BASE        GMII0_BASE
#define GMII1_BASE              0x3C051000
#define GRETH_GBIT1_BASE        GMII1_BASE

//RMII base address
//#define RMII_BASE               0x3C033000
//#define GRETH_BASE              RMII_BASE

//VDU base addresses
#define VDU0_MAIN_BASE          0x03011000
#define VDU0_OSD_BASE           0x03012000
#define VDU1_MAIN_BASE          0x03013000
#define VDU1_OSD_BASE           0x03014000

//GRABBER base address
#define GRABBER0_BASE           0x03015000
#define GRABBER1_BASE           0x03016000

//SCALER base address
#define SCALER_BASE             0x03017000

//CODA_V base address
#define CODA_V_BASE             0x03019000

//CODA_J base address
#define CODA_J_BASE             0x0301a000

//AUDIO_DMA base address
#define AUDIO_DMA_BASE          0x03018000

//I2S base address
#define I2S_BASE                0x01080000

//SPDIF base address
#define SPDIF_BASE              0x01081000

//GSPI_SDIO base address
#define SDIO_BASE          0x3C06A000
#define GSPI_BASE               0x3C059000

//GPIO base address
#define GPIO0_BASE              0x3C03B000
#define GPIO1_BASE              0x3C03C000

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

#define LSIF1_MGPIO0_BASE        0x3C060000
#define LSIF1_MGPIO1_BASE        0x3C061000
#define LSIF1_MGPIO2_BASE        0x3C062000
#define LSIF1_MGPIO3_BASE        0x3C063000
#define LSIF1_MGPIO4_BASE        0x3C064000
#define LSIF1_MGPIO5_BASE        0x3C065000
#define LSIF1_MGPIO6_BASE        0x3C066000
#define LSIF1_MGPIO7_BASE        0x3C067000
#define LSIF1_MGPIO8_BASE        0x3C068000
#define LSIF1_MGPIO9_BASE        0x3C069000

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
#define EM0_PHY_DCR_BASE        0x80060000

#define EM1_PLB6MCIF2_DCR_BASE  0x80100000
#define EM1_AXIMCIF2_DCR_BASE   0x80110000
#define EM1_MCLFIR_DCR_BASE     0x80120000
#define EM1_MCIF2ARB_DCR_BASE   0x80130000
#define EM1_DDR3LMC_DCR_BASE    0x80140000
#define EM1_PHY_DCR_BASE        0x80150000

#define NAND_BASE               0x3C032000

#define L2C0_DCR_BASE           0x80000600
#define L2C1_DCR_BASE           0x80000700

#define PMU0_DCR_BASE           0x80000600 //need to set
#define PMU1_DCR_BASE           0x80000700 //need to set

#define DCR_MPIC128_BASE              0xFFC00000

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
#define SPI_CTRL0_BASE          0x3C03E000
#define SPI_CTRL1_BASE          0x3C05A000

//CRG_CPU
#define CRG_CPU_BASE            0x38006000

//CRG_DDR
#define CRG_DDR_BASE            0x38007000

//Power Management Unit
#define PMU_BASE                0x38004000

//USB 2.0 OTG
#define USB_0_BASE          0x3C052000

//USB PHY
#define USBPHY_CFG_BASE       0x3800a000

//AXILSIF1 CTRL
#define AXI32LSIFS1_CTRL_BASE   0x3C900000
#define AXI32LSIFM1_CTRL_BASE   0x3CA00000

//LSIF1_CTRLREG
#define LSIF1_CTRLREG_BASE      0x3C05E000

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
#define LSIF0_LSCB0_BASE        0x3C036000
//#define LSIF0_LSCB1_BASE      0x3C037000
//#define LSIF0_LSCB2_BASE      0x3C038000
//#define LSIF0_LSCB3_BASE      0x3C039000
//#define LSIF0_LSCB4_BASE      0x3C03A000

#define LSIF1_LSCB0_BASE        0x3C054000
//#define LSIF1_LSCB1_BASE      0x3C055000
//#define LSIF1_LSCB2_BASE      0x3C056000
//#define LSIF1_LSCB3_BASE      0x3C057000
//#define LSIF1_LSCB4_BASE      0x3C058000

//LSIF0_CONFIG
#define LSIF0_CONFIG_BASE       0x3C03F000

//IFSYS_AXI32
#define LSIF0_MATRIX_BASE       0x3C700000
#define LSIF1_AXI32_S_BASE      0x3C900000
#define LSIF1_AXI32_M_BASE      0x3CA00000
#define HSIF_PL301_S_BASE       0x3C500000
#define HSIF_PL301_M_BASE       0x3C600000

//HSVI8
#define HSVI8_BASE              0x3C021000

//RMACE
#define RMACE_BASE              0x3C024000

//HSCB
#define HSCB0_BASE              0x3C025000
#define HSCB1_BASE              0x3C026000

//HSIF_CTRL
#define HSIF_CTRL_BASE          0x3C023000

//XHSIF_PCIe
#define XHSIF0_PCIe_BASE        0x3C000000
#define XHSIF1_PCIe_BASE        0x3C010000

//XHSIF_SERDES
#define XHSIF0_SERDES_BASE      0x3CB00000
#define XHSIF1_SERDES_BASE      0x3CB40000

//PLB4AHB
#define PLB4AHB_BASE            0x00000050

#define DCR_PLB6_BC_BASE            0x80000200

#endif //MIVEM_MEM_MAP_H
