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

#define RUMBOOT_PLATFORM_NUM_IRQS 128

#define IRQN_SW0     0
#define IRQN_SW1     1
#define MDIO0_INT    36
#define MDIO1_INT    37
#define MDIO2_INT    38
#define MDIO3_INT    39
#define DIT0_TIMINT1 42
#define DIT0_TIMINT2 43
#define DIT1_TIMINT1 44
#define DIT1_TIMINT2 45
#define DIT2_TIMINT1 46
#define DIT2_TIMINT2 47
#define DIT3_TIMINT1 48
#define DIT3_TIMINT2 49
#define MGETH0_IRQ   51
#define MGETH1_IRQ   52
#define MGETH2_IRQ   53
#define MGETH3_IRQ   54
#define WDT_INT      63
#define MDMA0_IRQ    64
#define MDMA1_IRQ    65
#define MDMA2_IRQ    66
#define MDMA3_IRQ    67
#define I2C0_IRQ     74
#define I2C1_IRQ     75
#define UART0_INTR   76
#define UART1_INTR   77
#define ARINC_INT    85
#define CAN0_INT     83
#define CAN1_INT     84

/**
 * @}
 */

#endif /* end of include guard: BASIS_IRQ_H */
