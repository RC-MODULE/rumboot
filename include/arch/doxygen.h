#ifndef ARCH_DOXYGEN_H
#define ARCH_DOXYGEN_H




#ifdef __DOXYGEN__
/**
 *
 * Define a critical section. Interrupts (IRQ & FIQ) will be disabled before
 * entry and restored to their previous state upon exit.
 *
 * \code{.c}
 * RUMBOOT_ATOMIC_BLOCK() {
 *  this_core_will_not_be_interrupted();
 * }
 * \endcode
 * \ingroup irq
 *
 */
#define RUMBOOT_ATOMIC_BLOCK();
#endif


/**
 * Enable IRQ handling by CPU
 */
static inline void rumboot_arch_irq_enable();

/**
 * Disable IRQ handling by CPU and return previous IRQ state
 * @return  irq satate that can be passed to setstate
 */
static inline int rumboot_arch_irq_disable(void);

/**
 * Set IRQ handling state
 * @param  pri_mask state to set (from rumboot_arch_irq_disable())
 * @return          previous state
 */
static inline int rumboot_arch_irq_setstate(int pri_mask);

#endif /* end of include guard: ARCH_DOXYGEN_H */
