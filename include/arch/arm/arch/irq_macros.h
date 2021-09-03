#ifndef ARM_IRQ_MACROS_H
#define ARM_IRQ_MACROS_H

/* Include doxygen.h to make sure that documentation gets updated  */
#include "../../doxygen.h"

#define RUMBOOT_IRQ_EDGE    (1 << 1)
#define RUMBOOT_IRQ_LEVEL   (0 << 1)
#define RUMBOOT_IRQ_POS     (0)
#define RUMBOOT_IRQ_NEG     (1)
#define RUMBOOT_IRQ_HIGH    (0)
#define RUMBOOT_IRQ_LOW     (1)


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
    uint32_t result;
    asm volatile ("MRS %0, cpsr" : "=r" (result) );
    asm volatile ("cpsie i" : : : "memory");
    return result;
}


static inline uint32_t rumboot_arch_irq_disable()
{
    uint32_t result;
    asm volatile ("MRS %0, cpsr" : "=r" (result) );
    asm volatile ("cpsid i" : : : "memory");
    return result;

}

static inline uint32_t rumboot_arch_irq_setstate(uint32_t new_state)
{
    uint32_t result;

    asm volatile ("MRS %0, cpsr" : "=r" (result) );
    asm volatile ("MSR cpsr, %0" : : "r" (new_state) : "memory");

    return result;
}

static inline void arm_vbar_set(uint32_t addr)
{
    /* Write Secure or Non-secure Vector Base Address Register */
    asm("MCR p15, 0, %0, c12, c0, 0\n"
        :
        : "r" (addr)
    );
}

static inline uint32_t arm_vbar_get()
{
    uint32_t ret;
    /* Read Secure or Non-secure Vector Base Address Register */
    asm(
        "MRC p15, 0, %0, c12, c0, 0\n"
        : "=r" (ret)
    );
    return ret;
}

#define RUMBOOT_ATOMIC_BLOCK() \
     for(uint32_t old_state = rumboot_arch_irq_disable(), flag = 1; \
         flag;\
         rumboot_arch_irq_setstate(old_state), flag = 0)


#endif /* end of include guard: ARM_IRQ_MACROS_H */
