#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <devices/irq-nmc.h>
#include <platform/devices.h>
#include <rumboot/io.h>

#define INTC_IRRL(base)      (base + 0x00)
#define INTC_IRRL_SET(base)  (base + 0x08)
#define INTC_IRRL_CLR(base)  (base + 0x10)
#define INTC_IMRL(base)      (base + 0x20)
#define INTC_IMRL_SET(base)  (base + 0x28)
#define INTC_IMRL_CLR(base)  (base + 0x30)
#define INTC_IMRL_NULL(base) (base + 0x38)
#define INTC_IRPL(base)      (base + 0x40)
#define INTC_IRPL_SET(base)  (base + 0x48)
#define INTC_IRPL_CLR(base)  (base + 0x50)
#define INTC_IRPL_NULL(base) (base + 0x58)
#define INTC_IASL(base)      (base + 0x60)
#define INTC_IASL_CLR(base)  (base + 0x70)

#define INTC_IRRH(base)      (base + 0x100)
#define INTC_IRRH_SET(base)  (base + 0x108)
#define INTC_IRRH_CLR(base)  (base + 0x110)
#define INTC_IMRH(base)      (base + 0x120)
#define INTC_IMRH_SET(base)  (base + 0x128)
#define INTC_IMRH_CLR(base)  (base + 0x130)
#define INTC_IMRH_NULL(base) (base + 0x138)
#define INTC_IRPH(base)      (base + 0x140)
#define INTC_IRPH_SET(base)  (base + 0x148)
#define INTC_IRPH_CLR(base)  (base + 0x150)
#define INTC_IRPH_NULL(base) (base + 0x158)
#define INTC_IASH(base)      (base + 0x160)
#define INTC_IASH_CLR(base)  (base + 0x170)

static int nmc_intc_init(const struct rumboot_irq_controller *dev)
{
    return 0; /* We're good here */
}

static uint32_t nmc_intc_begin(const struct rumboot_irq_controller *dev, int id)
{
    /* Surprisingly nothing to clear here */
    return id;
}

static void nmc_intc_end(const struct rumboot_irq_controller *dev, int irq)
{
    /* If that was a level interrupt (assume it was), we need to clear it */
    uintptr_t reg, shift;
	if (irq > 31) {
		reg = INTC_IASH_CLR(dev->base0);
		shift = irq - 32;
	} else {
		reg = INTC_IASL_CLR(dev->base0);
		shift = irq;
	}
    iowrite32(1<<shift, reg);
    return;
}

static void nmc_intc_configure(const struct rumboot_irq_controller *dev, int irq, uint32_t flags, int enable)
{
    uintptr_t reg, shift;
	if (irq > 31) {
		reg = enable ? INTC_IMRH_SET(dev->base0) : INTC_IMRH_CLR(dev->base0);
		shift = irq - 32;
	} else {
		reg = enable ? INTC_IMRL_SET(dev->base0) : INTC_IMRL_CLR(dev->base0);
		shift = irq;
	}
    iowrite32(1<<shift, reg);
}

static void nmc_intc_swint(const struct rumboot_irq_controller *dev, int irq)
{
        uintptr_t reg;
    	if (irq > 31) {
		    reg = INTC_IRRH_SET(dev->base0);
		    irq = irq - 32;
        } else {
		    reg = INTC_IRRL_SET(dev->base0);
        }
        iowrite32(1<<irq, reg);
}

void nmc_intc_adjust_priority( struct rumboot_irq_controller const * const dev, int irq, int const priority)
{
    uintptr_t reg;
    if (irq > 31) {
	    reg = priority ? INTC_IRPH_SET(dev->base0) : INTC_IRPH_CLR(dev->base0);
	    irq = irq - 32;
    } else {
	    reg = priority ? INTC_IRPL_SET(dev->base0) : INTC_IRPL_CLR(dev->base0);
    }
    iowrite32(1<<irq, reg);
}

static const struct rumboot_irq_controller irq_ctl = {
    .first = 0,
    .last  = 63,
    .init = nmc_intc_init,
    .begin = nmc_intc_begin,
    .priority_max = 0,
    .priority_min = 0,
    .end = nmc_intc_end,
    .configure = nmc_intc_configure,
    .generate_swint = nmc_intc_swint,
    .base0 = INTC_BASE
};


void rumboot_irq_register_nmc_intc_controller()
{
    rumboot_irq_register_controller(&irq_ctl);
}
