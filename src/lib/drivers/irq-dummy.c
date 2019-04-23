#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>


static int dummy_init(const struct rumboot_irq_controller *dev)
{
    return 0; /* We're good */
}

static uint32_t dummy_begin(const struct rumboot_irq_controller *dev)
{
        return 0;
}

static void dummy_end(const struct rumboot_irq_controller *dev, uint32_t irq)
{
    return;
}

static void dummy_configure(const struct rumboot_irq_controller *dev, int irq, uint32_t flags, int enable)
{

}

static void dummy_swint(const struct rumboot_irq_controller *dev, uint32_t irq)
{

}

static const struct rumboot_irq_controller irq_ctl = {
    .first = 0,
    .last  = 31,
    .init = dummy_init,
    .begin = dummy_begin,
    .end = dummy_end,
    .configure = dummy_configure,
    .generate_swint = dummy_swint
};


void rumboot_irq_register_dummy_controller()
{
    rumboot_irq_register_controller(&irq_ctl);
}
