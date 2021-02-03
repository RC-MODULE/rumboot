#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <devices/irq-nmc.h>
#include <platform/devices.h>
#include <rumboot/io.h>

#define INTC_IRRL(base)      (base + 0x0)
#define INTC_IRRL_SET(base)  (base + 0x8)
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


static uint32_t read_intr() {
    uint32_t ret;
    asm volatile("%0 = intr;\n"
    : "=r"(ret)
    );
    return ret;
}

static void clear_intr(uint32_t intr) {
    asm volatile("intr clear %0;\n"
    :: "r"(intr));
}

static int nmc_intc_init(const struct rumboot_irq_controller *dev)
{
    return 0; /* We're good */
}

static uint32_t nmc_intc_begin(const struct rumboot_irq_controller *dev)
{
        uint32_t intr = read_intr();
        if ((intr & (1<<7)) == 0) {
            rumboot_printf("INTC: WARN: Got external IRQ, but no EI is not set (intr == %x)\n", intr);
        };
        uint32_t ivec = intr & 0x3f; /* Bits 0-5 are interrupt vector number */
        uint32_t irqn = ioread32(INTC_IRRL(dev->base0));
        rumboot_printf("IVEC: %x IRRL: %x\n", ivec, irqn);
        return ivec;
}

static void nmc_intc_end(const struct rumboot_irq_controller *dev, uint32_t irq)
{
    /* Clear the interrupt */
    iowrite32(1<<irq, INTC_IRRL_CLR(dev->base0));
    return;
}

static void nmc_intc_configure(const struct rumboot_irq_controller *dev, int irq, uint32_t flags, int enable)
{

}

static void nmc_intc_swint(const struct rumboot_irq_controller *dev, uint32_t irq)
{

}

static const struct rumboot_irq_controller irq_ctl = {
    .first = 0,
    .last  = 31,
    .init = nmc_intc_init,
    .begin = nmc_intc_begin,
    .end = nmc_intc_end,
    .configure = nmc_intc_configure,
    .generate_swint = nmc_intc_swint,
    .base0 = INTC_BASE
};


void rumboot_irq_register_nmc_intc_controller()
{
    rumboot_irq_register_controller(&irq_ctl);
}
