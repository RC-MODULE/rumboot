#ifndef BASIS_IRQ_H
#define BASIS_IRQ_H


/**
 * \defgroup platform_basis_irq_map IRQ Map
 * \ingroup platform_basis
 *
 * \addtogroup platform_basis_irq_map
 * @{
 * This file contains gic irq numbers present in SoC.
 * For developers: Please, keep the list sorted by irq number
 * and formatted properly.
 *
 * WARNING: DO NOT ADD ANYTHING ELSE HERE, ONLY IRQ NUMBERS
 */

#define RUMBOOT_PLATFORM_NUM_IRQS 256

#define IRQN_SW0     0
#define IRQN_SW1     1

#define IRQ_MIRROR_BASE 128

#ifdef RUMBOOT_BASIS_ENABLE_MIRROR
        #define IRQ_REAL(n) (IRQ_MIRROR_BASE - 32 + n)
	#define IRQ_VIRT(n) (n - IRQ_MIRROR_BASE + 32)
#else
        #define IRQ_REAL(n) (n)
	#define IRQ_VIRT(n) (n)
#endif

#define TSNS_INTR                   IRQ_REAL(35)
#define MDIO0_INT                   IRQ_REAL(36)
#define MDIO1_INT                   IRQ_REAL(37)
#define MDIO2_INT                   IRQ_REAL(38)
#define MDIO3_INT                   IRQ_REAL(39)
#define MDIO_GPIO0_INT              IRQ_REAL(40)
#define MDIO_GPIO1_INT              IRQ_REAL(41)
#define DIT0_TIMINT1                IRQ_REAL(42)
#define DIT0_TIMINT2                IRQ_REAL(43)
#define DIT1_TIMINT1                IRQ_REAL(44)
#define DIT1_TIMINT2                IRQ_REAL(45)
#define DIT2_TIMINT1                IRQ_REAL(46)
#define DIT2_TIMINT2                IRQ_REAL(47)
#define DIT3_TIMINT1                IRQ_REAL(48)
#define DIT3_TIMINT2                IRQ_REAL(49)
#define MGETH0_IRQ                  IRQ_REAL(51)
#define MGETH1_IRQ                  IRQ_REAL(52)
#define MGETH2_IRQ                  IRQ_REAL(53)
#define MGETH3_IRQ                  IRQ_REAL(54)
#define ADTRANS_SLV_IRQ_SPI         IRQ_REAL(61)
#define WDT_INT                     IRQ_REAL(63)
#define MDMA0_IRQ                   IRQ_REAL(64)
#define MDMA1_IRQ                   IRQ_REAL(65)
#define MDMA2_IRQ                   IRQ_REAL(66)
#define MDMA3_IRQ                   IRQ_REAL(67)
#define MKIO0_IRQ                   IRQ_REAL(68)
#define MKIO1_IRQ                   IRQ_REAL(69)
#define GSPI_SDIO0_IRQ              IRQ_REAL(72)
#define GSPI_SDIO1_IRQ              IRQ_REAL(73)
#define I2C0_IRQ                    IRQ_REAL(74)
#define I2C1_IRQ                    IRQ_REAL(75)
#define UART0_INTR                  IRQ_REAL(76)
#define UART1_INTR                  IRQ_REAL(77)
#define GPIO1_INTR                  IRQ_REAL(79)
#define GPIO2_INTR                  IRQ_REAL(80)
#define GPIO3_INTR                  IRQ_REAL(81)
#define GPIO4_INTR                  IRQ_REAL(82)
#define CAN0_INT                    IRQ_REAL(83)
#define CAN1_INT                    IRQ_REAL(84)
#define ARINC_INT                   IRQ_REAL(85)
#define SGMII_INT                   IRQ_REAL(86)


/**
 * @}
 */

#endif /* end of include guard: BASIS_IRQ_H */
