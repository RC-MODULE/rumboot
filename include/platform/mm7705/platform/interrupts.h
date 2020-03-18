#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/**
 * \defgroup platform_mpw_proto_irq_map IRQ Map
 * \ingroup platform_mpw_proto
 *
 * \addtogroup platform_mpw_proto_irq_map
 * @{
 * This file contains irq numbers present in SoC.
 * For developers: Please, keep the list sorted by irq number
 * and formatted properly.
 *
 * WARNING: DO NOT ADD ANYTHING ELSE HERE, ONLY IRQ NUMBERS
 */

#define RUMBOOT_PLATFORM_NUM_IRQS 256


#define MPIC128_TIMER_0             (128)
#define MPIC128_TIMER_1             (129)
#define MPIC128_TIMER_2             (130)
#define MPIC128_TIMER_3             (131)
#define MPIC128_IPI_0               (132)
#define MPIC128_IPI_1               (133)
#define MPIC128_IPI_2               (134)
#define MPIC128_IPI_3               (135)

 /**
  * @}
  */

#endif /* end of include guard: INTERRUPTS_H */
