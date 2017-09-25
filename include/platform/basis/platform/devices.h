#ifndef DEVICES_H
#define DEVICES_H

/*
 * This file should contain only base addresses of devices.
 * Keep the list sorted in by device address and formatted.
 * WARNING: DO NOT ADD INDIVIDUAL REGISTER OFFSETS HERE
 */

 /**
  * \defgroup platform_basis_register_map Register Map
  * \ingroup platform_basis
  *
  * \addtogroup platform_basis_register_map
  * @{
  */

 #define MDMA0_BASE               0x01010000
 #define MDMA1_BASE               0x01011000
 #define MDMA2_BASE               0x01012000
 #define MDMA3_BASE               0x01013000
 #define EXT_IRQ_GEN_BASE         0x01018000
 #define TSNS_BASE                0x01019000
 #define I2C0_BASE                0x01023000
 #define I2C1_BASE                0x01024000
 #define SCTL_BASE                0x01028000
 #define DIT0_BASE                0x0102A000
 #define DIT1_BASE                0x0102B000
 #define DIT2_BASE                0x0102C000
 #define DIT3_BASE                0x0102D000
 #define CRG_DDR_BASE             0x0102F000
 #define ADDR_TRANS_SLV_BASE      0x01034000
 #define ADDR_TRANS_MST_BASE      0x01035000
 #define GPIO0_BASE               0x01036000
 #define GPIO1_BASE               0x01037000
 #define GPIO2_BASE               0x01038000
 #define PCIE_CORE_BASE           0x01058000
 #define PCIE_PHY_BASE            0x01060000
 #define DDR0_BASE                0x01080000
 #define DDR1_BASE                0x01082000
 #define GIC_DIST_BASE            0x01084000
 #define GIC_CPUIF_BASE           0x01085000
 #define PCIE_BASE                0x40000000
 #define EMI0_BASE                0x80000000
 #define EMI1_BASE                0xC0000000

 /**
 * @}
 */

#endif /* end of include guard: DEVICES_H */
