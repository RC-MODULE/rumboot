#ifndef ARM_IRQ_MACROS_H
#define ARM_IRQ_MACROS_H

/* Include doxygen.h to make sure that documentation gets updated  */
#include "../../doxygen.h"

#define RUMBOOT_IRQ_EDGE    (1 << 1)
#define RUMBOOT_IRQ_LEVEL   (0 << 1)
#define RUMBOOT_IRQ_POS     (0)
#define RUMBOOT_IRQ_NEG     (0)
#define RUMBOOT_IRQ_HIGH    (0)
#define RUMBOOT_IRQ_LOW     (0)


#if 0
static inline void rumboot_arch_irq_dump_cpsr()
{
    int result = 0;
    asm volatile (
    "mrs %0, cpsr\n" : "=r" (result) );
    rumboot_printf("cpsr == 0x%x\n", result);
}

#endif

static inline uint32_t rumboot_arch_irq_enable()
{
    return 0;
}


static inline uint32_t rumboot_arch_irq_disable()
{
    return 0;
}

static inline uint32_t rumboot_arch_irq_setstate(uint32_t new_state)
{
    return 0;
}


#define RUMBOOT_ATOMIC_BLOCK() \
     for(uint32_t old_state = rumboot_arch_irq_disable(), flag = 1; \
         flag;\
         rumboot_arch_irq_setstate(old_state), flag = 0)


#endif /* end of include guard: ARM_IRQ_MACROS_H */
