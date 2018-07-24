#ifndef PPC_IRQ_MACROS_H
#define PPC_IRQ_MACROS_H


#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/regs/fields/mpic128.h>


/* Include doxygen.h to make sure that documentation gets updated  */
#include "../../doxygen.h"


#define RUMBOOT_IRQ_EDGE    (int_sense_edge << MPIC128_VP_S_i)
#define RUMBOOT_IRQ_LEVEL   (int_sense_level << MPIC128_VP_S_i)
#define RUMBOOT_IRQ_POS     (int_pol_pos << MPIC128_VP_POL_i)
#define RUMBOOT_IRQ_NEG     (int_pol_neg << MPIC128_VP_POL_i)
#define RUMBOOT_IRQ_HIGH    (int_pol_high << MPIC128_VP_POL_i)
#define RUMBOOT_IRQ_LOW     (int_pol_low << MPIC128_VP_POL_i)


static inline uint32_t rumboot_arch_irq_enable() {
    uint32_t state = msr_read();
    msr_write( state | ( 1 << ITRPT_XSR_EE_i ) );
    return state;
}

static inline uint32_t rumboot_arch_irq_disable() {
    uint32_t state = msr_read();
    msr_write( state & ~( 1 << ITRPT_XSR_EE_i ) );
    return state;
}

static inline uint32_t rumboot_arch_irq_setstate( uint32_t new_state ) {
    uint32_t state = msr_read();
    msr_write( new_state );
    return state;
}

#define RUMBOOT_ATOMIC_BLOCK() \
     for(uint32_t old_state = rumboot_arch_irq_disable(), flag = 1; \
         flag;\
         rumboot_arch_irq_setstate(old_state), flag = 0)


#endif /* end of include guard: PPC_IRQ_MACROS_H */
