#ifndef PPC_IRQ_MACROS_H
#define PPC_IRQ_MACROS_H

/* Include doxygen.h to make sure that documentation gets updated  */
#include "../../doxygen.h"

static inline void rumboot_arch_irq_enable()
{
    asm volatile ("nop");
}


static inline int rumboot_arch_irq_disable(void)
{
    int result = 0;
    asm volatile (
        "nop\n"
        );
    return result;
}

static inline int rumboot_arch_irq_setstate(int pri_mask)
{
    uint32_t tmp;
    asm volatile (
        "nop\n"
        );
    return 0;
}

#define RUMBOOT_ATOMIC_BLOCK() \
     for(int mask = rumboot_arch_irq_disable(), flag = 1; \
         flag;\
         flag = rumboot_arch_irq_setstate(mask))


#endif /* end of include guard: PPC_IRQ_MACROS_H */
