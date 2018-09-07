#ifndef DEVICES_H
#define DEVICES_H


/**
 * \defgroup platform_mpw_proto_register_map Register Map
 * \ingroup platform_mpw_proto
 * This file contains physical addresses of devices present in SoC.
 * For developers: Please, keep the list sorted by device address
 * and formatted properly.
 *
 * WARNING: DO NOT ADD INDIVIDUAL REGISTER OFFSETS HERE
 *
 * \addtogroup platform_mpw_proto_register_map
 * @{
 *
 */



#define BOOT_ROM_BASE              0xFFFC0000
#define BOOT_ROM_SIZE              0x00040000
#define BOOT_ROM_INITIAL_4K_BASE   0xFFFFF000
#define BOOT_ROM_ENTRY_BASE        0xFFFFFFFC
#define IM0_BASE                   0x00040000
#define IM0_SIZE                   0x00040000
#define SCTL_BASE                  0x38000000

#define GPIO0_BASE                 0x3C065000
#define GPIO1_BASE                 0x3C066000

#define UART0_BASE                 0x3C05D000
#define UART1_BASE                 0x3C05E000
#define UART2_BASE                 0x3C05F000


#define SYS_TIMER_BASE             0x38001000

#define SYS_WATCHDOG_BASE              0x38002000

#define TVSENS_BASE                0x38003000

#define FC1_BASE              0x3C004000
#define FC2_BASE              0x3C014000

#define SPI_CTRL0_BASE         0x3C061000
#define SPI_CTRL1_BASE         0x3C062000

#define LSIF0_MGPIO0_BASE           0x3C040000
#define LSIF0_MGPIO1_BASE           0x3C041000
#define LSIF0_MGPIO2_BASE           0x3C042000
#define LSIF0_MGPIO3_BASE           0x3C043000
#define LSIF0_MGPIO4_BASE           0x3C044000
#define LSIF0_MGPIO5_BASE           0x3C045000
#define LSIF0_MGPIO6_BASE           0x3C046000
#define LSIF0_MGPIO7_BASE           0x3C047000
#define LSIF0_MGPIO8_BASE           0x3C048000
#define LSIF0_MGPIO9_BASE           0x3C049000
#define LSIF0_MGPIO10_BASE          0x3C04A000

#define LSIF1_GPIO0_BASE           0x3C065000
#define LSIF1_GPIO1_BASE           0x3C066000

#define LSIF1_MGPIO0_BASE           0x3C067000
#define LSIF1_MGPIO1_BASE           0x3C068000
#define LSIF1_MGPIO2_BASE           0x3C069000
#define LSIF1_MGPIO3_BASE           0x3C06A000
#define LSIF1_MGPIO4_BASE           0x3C06B000



#define SPISDIO_BASE             0x3C064000

#define SDIO_BASE                0x3C064000

#define GSPI_BASE                0x3C063000

#define NAND_BASE             0x3C032000

#define GRETH_GBIT0_BASE       0x3C033000

#define GRETH_GBIT1_BASE       0x3C034000

#define GRETH0_BASE       0x3C035000
#define GRETH1_BASE       0x3C036000
#define GRETH2_BASE       0x3C037000

#define LSCB0_BASE       0x3C050000
#define LSCB1_BASE       0x3C055000

#define XHSIF0_CTRL_BASE      0x3C005000

#define XHSIF1_CTRL_BASE      0x3C015000

#define NORMC_BASE         0x3C030000

#define NORMEM_BASE        0x20000000

#define SRAMMC_BASE         0x3C031000

#define SRAMMEM_BASE       0x30000000

#define I2C0_BASE          0x3C05A000
#define I2C1_BASE          0x3C05B000
#define I2C2_BASE          0x3C05C000

#define NIC301_A_CFG_BASE   0x38100000
#define NIC301_A_CFG_SIZE   0x00100000

#define CRGCPU_BASE        0x38006000

#define CRGDDR_BASE        0x38007000

#define RFBISR_BASE      0x3800C000

//Power Management Unit
#define PMU_BASE            0x38004000

#define USB20OTG0_BASE          0x3C06D000

#define LSIF1_CTRLREG_BASE       0x3C060000

#define XHSIF0_CTRL_BASE    0x3C005000
#define XHSIF1_CTRL_BASE    0x3C015000

#define LSIF0_CONFIG_BASE       0x3C03F000

#define LSIF0_AXI32_S_BASE      0x3C700000
#define LSIF0_AXI32_M_BASE      0x3C800000

#define LSIF1_AXI32_S_BASE      0x3C900000
#define LSIF1_AXI32_M_BASE      0x3CA00000

#define HSIF_PL301_S_BASE       0x3C500000
#define HSIF_PL301_M_BASE       0x3C600000


#define HSVI0_BASE              0x3C001000
#define HSVI1_BASE              0x3C002000
#define HSVI2_BASE              0x3C003000

#define HSVI3_BASE              0x3C011000
#define HSVI4_BASE              0x3C012000
#define HSVI5_BASE              0x3C013000


#define HSVI8_BASE              0x3C021000
#define HSVI8_BASE              0x3C021000

#define HSVI8_BASE              0x3C021000

#define RMACE_BASE              0x3C024000

#define HSCB0_BASE              0x3C025000
#define HSCB1_BASE              0x3C026000

#define HSIF_CTRL_BASE          0x3C023000

#define XHSIF0_PCIe_BASE        0x3C000000

#define XHSIF1_PCIe_BASE        0x3C010000

#define SERDES1_BASE      0x3CB00000
#define SERDES2_BASE      0x3CB10000
#define SERDES3_BASE      0x3CB20000
#define SERDES4_BASE      0x3CB30000

#define SERDES5_BASE      0x3CB40000
#define SERDES6_BASE      0x3CB50000
#define SERDES7_BASE      0x3CB60000
#define SERDES8_BASE      0x3CB70000


#define EM0_PLB6MCIF2_DCR  0x80010000
#define EM0_AXIMCIF2_DCR   0x80020000
#define EM0_MCLFIR_DCR     0x80030000
#define EM0_MCIF2ARB_DCR   0x80040000
#define EM0_DDR3LMC_DCR    0x80050000

#define EM1_PLB6MCIF2_DCR  0x80100000
#define EM1_AXIMCIF2_DCR   0x80110000
#define EM1_MCLFIR_DCR     0x80120000
#define EM1_MCIF2ARB_DCR   0x80130000
#define EM1_DDR3LMC_DCR    0x80140000

#define EM2_PLB6MCIF2_DCR 0x80160000
#define EM3_PLB6MCIF2_DCR 0x80180000

#define EM0_PHY_BASE        0x3800E000
#define EM1_PHY_BASE        0x3800F000


/**
 * @}
 */

#endif
