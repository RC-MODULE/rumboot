#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/mpic128.h>
#include <platform/regs/regs_mpic128.h>


void mpic128_reset( uint32_t base_address ) {
    dcr_write( base_address + MPIC128_GCF0,
               dcr_read( base_address + MPIC128_GCF0 ) | ( 1 << MPIC128_GCF0_R_i ) );
    while( dcr_read( base_address + MPIC128_GCF0 ) & ( 1 << MPIC128_GCF0_R_i ) ) {}
}

void mpic128_pass_through_disable( uint32_t base_address ) {
    dcr_write( base_address + MPIC128_GCF0,
               dcr_read( base_address + MPIC128_GCF0 ) | ( 1 << MPIC128_GCF0_P_i ) );
}

void mpic128_set_interrupt_borders( uint32_t base_address, uint32_t mc_border, uint32_t cr_border ) {
    TEST_ASSERT( mc_border >= cr_border, "See MPIC documentation" );
    dcr_write( base_address + MPIC128_VITC,
               ( mc_border << MPIC128_VITC_MCB_i ) | ( cr_border << MPIC128_VITC_CRB_i ) );
}

static int mpic128_init( const struct rumboot_irq_controller *dev ) {
    rumboot_printf("Init MPIC\n");
    mpic128_reset( DCR_MPIC128_BASE );
    mpic128_pass_through_disable( DCR_MPIC128_BASE );
    mpic128_set_interrupt_borders( DCR_MPIC128_BASE, MPIC128_PRIOR_UND, MPIC128_PRIOR_UND ); /* make all interrupts non-critical */
    dcr_write( DCR_MPIC128_BASE + MPIC128_CTP_PR, MPIC128_PRIOR_0 ); /* enable all interrupts priority */
    return 0; /* We're good */
}

static uint32_t mpic128_begin( const struct rumboot_irq_controller *dev ) {
    return dcr_read( DCR_MPIC128_BASE + MPIC128_NCIAR_PR ); /* acknowledge interrupt handlig */
}

static void mpic128_end( const struct rumboot_irq_controller *dev, uint32_t irq ) {
    if( irq != dcr_read( DCR_MPIC128_BASE + MPIC128_NCIAR_PR ) ) {
        dcr_write( DCR_MPIC128_BASE + MPIC128_NCEOI_PR, 0 ); /* signal the end of processing for non-spurious interrupt */
    }
}

static void mpic128_configure( const struct rumboot_irq_controller *dev, int irq, uint32_t flags, int enable ) {

}

static const struct rumboot_irq_controller irq_ctl = {
    .first = 0,
    .last = 127,
    .init = mpic128_init,
    .begin = mpic128_begin,
    .end = mpic128_end,
    .configure = mpic128_configure
};

void rumboot_irq_register_mpic128() {
    rumboot_irq_register_controller( &irq_ctl );
}
