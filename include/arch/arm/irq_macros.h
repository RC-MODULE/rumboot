#ifndef ARM_IRQ_MACROS_H
#define ARM_IRQ_MACROS_H

#if 0
static inline void rumboot_arch_irq_dump_cpsr()
{
    int result = 0;
    asm volatile (
    "mrs %0, cpsr\n" : "=r" (result) );
    rumboot_printf("cpsr == 0x%x\n", result);
}

#endif

static inline void rumboot_arch_irq_enable()
{
    asm volatile ("cpsie i" : : : "memory");
}


static inline int rumboot_arch_irq_disable(void)
{
    int result = 0;
    int tmp = 0;
s    asm volatile (
        "mrs %0, cpsr\n"
        "orr %1, %0, #0xC0\n"
        "msr cpsr_c, %1\n"
        : "=r" (result), "+r" (tmp)
        );
    return result;
}

static inline int rumboot_arch_irq_setstate(int pri_mask)
{
    uint32_t tmp;
    asm volatile (
        "mrs  %0, cpsr\n"
        "and  %0, #0x3f\n"
        "orr  %1, %0\n"
        "msr cpsr_c, %1\n"
        : "=r" (tmp)
        : "r" (pri_mask)
        );
    return 0;
}

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
#define RUMBOOT_ATOMIC_BLOCK() \
     for(int mask = rumboot_arch_irq_disable(), flag = 1; \
         flag;\
         flag = rumboot_arch_irq_setstate(mask))


#endif /* end of include guard: ARM_IRQ_MACROS_H */
