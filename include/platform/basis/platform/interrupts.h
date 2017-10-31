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

#define DIT0_TIMINT1 42
#define DIT0_TIMINT2 43
#define DIT1_TIMINT1 44
#define DIT1_TIMINT2 45
#define DIT2_TIMINT1 46
#define DIT2_TIMINT2 47
#define DIT3_TIMINT1 48
#define DIT3_TIMINT2 49

/**
 * @}
 */

#endif /* end of include guard: BASIS_IRQ_H */
