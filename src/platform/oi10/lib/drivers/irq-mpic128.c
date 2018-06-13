#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>

static int mpic128_init( const struct rumboot_irq_controller *dev ) {
    return 0; /* We're good */
}

static uint32_t mpic128_begin( const struct rumboot_irq_controller *dev ) {
    return 0;
}

static void mpic128_end( const struct rumboot_irq_controller *dev, uint32_t irq ) {
    return;
}

static void mpic128_configure( const struct rumboot_irq_controller *dev, int irq, uint32_t flags, int enable ) {

}

static const struct rumboot_irq_controller irq_ctl = {
    .first = 0,
    .last = 127,
    .init = mpic128_init,
    .begin = mpic128_begin,
    .end = mpic128_end,
    .configure = mpic128_configure };

void rumboot_irq_register_mpic128_controller() {
    rumboot_irq_register_controller( &irq_ctl );
}
