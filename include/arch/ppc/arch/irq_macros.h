#ifndef PPC_IRQ_MACROS_H
#define PPC_IRQ_MACROS_H

/* Include doxygen.h to make sure that documentation gets updated  */
#include "../../doxygen.h"

static inline uint32_t rumboot_arch_irq_enable()
{
    uint32_t result = 0;
    asm volatile (
        "nop\n"
    );
    return result;
}


static inline uint32_t rumboot_arch_irq_disable()
{
    uint32_t result = 0;
    asm volatile (
        "nop\n"
    );
    return result;
}

static inline uint32_t rumboot_arch_irq_setstate(uint32_t new_state){
    uint32_t result = 0;
    asm volatile (
        "nop\n"
    );
    return result;
}

#define RUMBOOT_ATOMIC_BLOCK() \
     for(uint32_t old_state = rumboot_arch_irq_disable(), flag = 1; \
         flag;\
         rumboot_arch_irq_setstate(old_state), flag = 0)


#endif /* end of include guard: PPC_IRQ_MACROS_H */
