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

#define IRQN_SW0     0
#define IRQN_SW1     1

#define DIT0_TIMINT1 42
#define DIT0_TIMINT2 43

/*
 * @}
 */

#endif /* end of include guard: BASIS_IRQ_H */
