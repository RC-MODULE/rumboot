#ifndef DEVICES_H
#define DEVICES_H


/**
 * \defgroup platform_basis_register_map Register Map
 *
 * \ingroup platform_basis
 *
 * \addtogroup platform_basis_register_map
 *
 *
 * @{
 *
 * This file contains physical addresses of devices present in SoC.
 * For developers: Please, keep the list sorted by device address
 * and formatted properly.
 *
 * WARNING: DO NOT ADD INDIVIDUAL REGISTER OFFSETS HERE
 */

 #define MDMA0_BASE               0x01010000
 #define MDMA1_BASE               0x01011000
 #define MDMA2_BASE               0x01012000
 #define MDMA3_BASE               0x01013000
 #define EXT_IRQ_GEN_BASE         0x01018000
 #define ADDR_TRANS_SLV_BASE      0x01019000
 #define ADDR_TRANS_MST_BASE      0x0101A000
 #define CAN0_BASE                0x01020000
 #define CAN1_BASE                0x01028000
 #define TSNS_BASE                0x01032000
 #define BISR_L2C                 0x0103E000
 #define UART0_BASE               0x01044000
 #define UART1_BASE               0x01045000
 #define I2C0_BASE                0x01046000
 #define I2C1_BASE                0x01047000
 #define DIT0_BASE                0x01048000
 #define DIT1_BASE                0x01049000
 #define DIT2_BASE                0x0104A000
 #define DIT3_BASE                0x0104B000
 #define CRG_DDR_BASE             0x0104D000
 #define PCIE_CORE_BASE           0x01058000
 #define PCIE_PHY_BASE            0x01060000
 #define GPIO0_BASE               0x01098000
 #define GPIO1_BASE               0x01099000
 #define GPIO2_BASE               0x0109A000
 #define GPIO3_BASE               0x0109B000
 #define GPIO4_BASE               0x0109C000
 #define SCTL_BASE                0x0109D000
 #define DDR0_BASE                0x01100000
 #define DDR1_BASE                0x01102000
 #define GIC_DIST_BASE            0x01104000
 #define GIC_CPUIF_BASE           0x01105000
 #define PCIE_BASE                0x40000000
 #define EMI0_BASE                0x80000000
 #define EMI1_BASE                0xC0000000
 #define ARINC_BASE               0x01030000
 #define GSPI0_BASE               0x01040000
 #define GSPI1_BASE               0x01041000
 #define SDIO0_BASE               0x01042000
 #define GPIO0_BASE               0x01098000
 #define GPIO1_BASE               0x01099000
 #define GPIO2_BASE               0x0109A000
 #define GPIO3_BASE               0x0109B000
 #define GPIO4_BASE               0x0109C000


 /**
 * @}
 */

#endif /* end of include guard: DEVICES_H */
