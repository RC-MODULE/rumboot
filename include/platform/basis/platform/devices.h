#ifndef DEVICES_H
#define DEVICES_H


/**
 * \defgroup platform_basis_register_map Register Map
 * \ingroup platform_basis
 * This file contains physical addresses of devices present in SoC.
 * For developers: Please, keep the list sorted by device address
 * and formatted properly.
 *
 * WARNING: DO NOT ADD INDIVIDUAL REGISTER OFFSETS HERE
 *
 * \addtogroup platform_basis_register_map
 * @{
 *
 */


 #define PCIE_BASE                0x40000000
 #define EMI0_BASE                0x80000000
 #define EMI1_BASE                0xC0000000


 #ifndef RUMBOOT_BASIS_ENABLE_MIRROR
    #define BASIS_PHYS(addr) (addr)
 #else
    #define BASIS_PHYS(addr) (addr + PCIE_BASE)
 #endif

 #define MDMA0_BASE               BASIS_PHYS(0x01010000)
 #define MDMA1_BASE               BASIS_PHYS(0x01011000)
 #define MDMA2_BASE               BASIS_PHYS(0x01012000)
 #define MDMA3_BASE               BASIS_PHYS(0x01013000)
 #define ETH0_BASE                BASIS_PHYS(0x01014000)
 #define ETH1_BASE                BASIS_PHYS(0x01015000)
 #define ETH2_BASE                BASIS_PHYS(0x01016000)
 #define ETH3_BASE                BASIS_PHYS(0x01017000)
 #define EXT_IRQ_GEN_BASE         BASIS_PHYS(0x01018000)
 #define ADDR_TRANS_SLV_BASE      BASIS_PHYS(0x01019000)
 #define ADDR_TRANS_MST_BASE      BASIS_PHYS(0x0101A000)
 #define CAN0_BASE                BASIS_PHYS(0x01020000)
 #define CAN1_BASE                BASIS_PHYS(0x01028000)
 #define ARINC_BASE               BASIS_PHYS(0x01030000)
 #define TSNS_BASE                BASIS_PHYS(0x01032000)
 #define BISR_L2C                 BASIS_PHYS(0x0103E000)
 #define GSPI0_BASE               BASIS_PHYS(0x01040000)
 #define SDIO0_BASE               BASIS_PHYS(0x01042000)
 #define UART0_BASE               BASIS_PHYS(0x01044000)
 #define UART1_BASE               BASIS_PHYS(0x01045000)
 #define I2C0_BASE                BASIS_PHYS(0x01046000)
 #define I2C1_BASE                BASIS_PHYS(0x01047000)
 #define DIT0_BASE                BASIS_PHYS(0x01048000)
 #define DIT1_BASE                BASIS_PHYS(0x01049000)
 #define DIT2_BASE                BASIS_PHYS(0x0104A000)
 #define DIT3_BASE                BASIS_PHYS(0x0104B000)
 #define CRG_SYS_BASE             BASIS_PHYS(0x0104C000)
 #define CRG_DDR_BASE             BASIS_PHYS(0x0104D000)
 #define PCIE_CORE_BASE           BASIS_PHYS(0x01058000)
 #define PCIE_PHY_BASE            BASIS_PHYS(0x01060000)
 #define MDIO0_BASE               BASIS_PHYS(0x01080000)
 #define MDIO1_BASE               BASIS_PHYS(0x01081000)
 #define MDIO2_BASE               BASIS_PHYS(0x01082000)
 #define MDIO3_BASE               BASIS_PHYS(0x01083000)
 #define MGPIO0_BASE              BASIS_PHYS(0x01084000)
 #define MGPIO1_BASE              BASIS_PHYS(0x01085000)
 #define GPIO0_BASE               BASIS_PHYS(0x01088000)
 #define GPIO1_BASE               BASIS_PHYS(0x01089000)
 #define GPIO2_BASE               BASIS_PHYS(0x0108A000)
 #define GPIO3_BASE               BASIS_PHYS(0x0108B000)
 #define GPIO4_BASE               BASIS_PHYS(0x0108C000)
 #define GPIO5_BASE               BASIS_PHYS(0x01084000)
 #define GPIO6_BASE               BASIS_PHYS(0x01085000)
 #define SCTL_BASE                BASIS_PHYS(0x0108D000)
 #define GLOBAL_TIMERS            BASIS_PHYS(0x0108E040)
 #define WDT_BASE                 BASIS_PHYS(0x0108F000)
 #define DDR0_BASE                BASIS_PHYS(0x01500000)
 #define DDR1_BASE                BASIS_PHYS(0x01502000)
 #define GIC_DIST_BASE            BASIS_PHYS(0x01504000)
 #define GIC_CPUIF_BASE           BASIS_PHYS(0x01505000)
 #define L2C_BASE                 BASIS_PHYS(0x01506000)


 /**
 * @}
 */

#endif /* end of include guard: DEVICES_H */
