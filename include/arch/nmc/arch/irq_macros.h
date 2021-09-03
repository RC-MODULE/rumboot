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

/*
 * General notes on NMC interrupt handling
 * 
 * - 'Sandwitched' groups of instructions are guaranteed to be atomic
 * - We should not touch flag bits or configs in PSWR, compiler relies on 'em
 * - We have to save the previous PSWR value to stack via a callrel
 * - Debug & Overflow should be enabled explicitly, since overflow gets 
 *   triggered every time you print are really long uin32_t
 */


static inline uint32_t rumboot_arch_irq_disable()
{
    uint32_t ret = 0;
    uint32_t new_state = 0x1f0; /* Mask ALL interrupts */
    asm volatile(
        "delayed callrel 2;\n"
        "pswr clear %1;\n"
        "nul; \n"
        "%0 = [ar7 - 1];\n"
        "ar7 = ar7 - 2;\n"
        : "=r"(ret)
        : "r"(new_state)
        );
    return ret & 0x1f0; /* Do not return flags & reserved bits */
}

static inline void __nmc_flush_pipeline()
{
    /* This will flush everything from the pipeline */
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
    asm volatile("gr0 = gr0;\n");
};

static inline uint32_t rumboot_arch_irq_setstate(uint32_t new_state)
{
    /* Assume interrupts were enabled before. Rare, but may happen. */
    uint32_t prev = rumboot_arch_irq_disable();
    /* At this point we only SET bits */ 
    asm volatile("pswr set %0;\n"
        :: "r"(new_state)
        );
    __nmc_flush_pipeline();
    return prev;
}

static inline uint32_t rumboot_arch_irq_enable()
{
    /* Assume interrupts were enabled before. Rare, but may happen. */
    uint32_t prev = rumboot_arch_irq_disable();
    /* Enable 'em, preserving other bits */
    asm volatile("pswr set 0xc0;\n"); /* Enable all except DEBUG & OVERFLOW */ 
    /* Flush the pipeline. If any IRQ was pending, it will arrive here, 
     * BEFORE user code is reached 
     */
    __nmc_flush_pipeline();
    return prev;
}

static inline void __nmc_enable_overflow_exception(int enable)
{
    if (enable) {
        asm volatile("pswr set 0x100;\n"); /* Enable OVERFLOW */         
    } else {
        asm volatile("pswr clear 0x100;\n"); /* Enable OVERFLOW */         
    }
}

static inline void __nmc_enable_debug_exception(int enable)
{
    if (enable) {
        asm volatile("pswr set 0x20;\n"); /* Enable DEBUG */         
    } else {
        asm volatile("pswr clear 0x20;\n"); /* Disable debug */         
    }
}

#define RUMBOOT_ATOMIC_BLOCK() \
     for(uint32_t old_state = rumboot_arch_irq_disable(), flag = 1; \
         flag;\
         rumboot_arch_irq_setstate(old_state), flag = 0)


#endif /* end of include guard: ARM_IRQ_MACROS_H */
