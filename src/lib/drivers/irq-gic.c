#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <regs/regs_gic.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>


static inline void gic_dist_write(uint32_t reg, uint32_t value)
{
	iowrite32(value, GIC_DIST_BASE + reg);
}

static inline void gic_cpuif_write(uint32_t reg, uint32_t value)
{
	iowrite32(value, GIC_CPUIF_BASE + reg);
}

static inline uint32_t gic_dist_read(uint32_t reg)
{
	return ioread32(GIC_DIST_BASE + reg);
}

static inline uint32_t gic_cpuif_read(uint32_t reg)
{
	return ioread32(GIC_CPUIF_BASE + reg);
}

static int gic_init(const struct rumboot_irq_controller *dev)
{
	/* Enable IRQ forwarding & set PMR */
	gic_cpuif_write(GICC_REG_PMR, 0xff);
	gic_cpuif_write(GICC_REG_CTLR, 1);
	gic_dist_write(GICD_REG_CTLR, 1);
	return 0;
}

static uint32_t gic_begin(const struct rumboot_irq_controller *dev)
{
	return gic_cpuif_read(GICC_REG_IAR);
}

static void gic_end(const struct rumboot_irq_controller *dev, uint32_t irq)
{
	gic_cpuif_write(GICC_REG_EOIR, irq);
}

static void gic_configure(const struct rumboot_irq_controller *dev, int irq, uint32_t flags, int enable)
{
	/* IRQ < 16 (SW) can't be configured */
	if (irq < 16) {
		return;
	}

	gic_dist_write(GICD_REG_CTLR, 0);
	gic_cpuif_write(GICC_REG_CTLR, 0);

	uint32_t reg = enable ? GICD_REG_ISENABLER1 : GICD_REG_ICENABLER1;
	uint32_t shift = irq;

	/* TODO: Edge/Level sensivity */
	if ((irq > 31) && (irq < 64)) {
		reg = enable ? GICD_REG_ISENABLER2 : GICD_REG_ICENABLER2;
		shift = irq - 32;
	} else if ((irq > 63) && (irq < 96)) {
		reg = enable ? GICD_REG_ISENABLER3 : GICD_REG_ICENABLER3;
		shift = irq - 64;
	}

	/* Do we need moar ? */

	gic_dist_write(reg, 1 << shift);

	gic_cpuif_write(GICC_REG_PMR, 0xff);
	gic_cpuif_write(GICC_REG_CTLR, 1);
	gic_dist_write(GICD_REG_CTLR, 1);
}

static const struct rumboot_irq_controller irq_ctl = {
    .first = 0,
    .last  = 127,
    .init = gic_init,
    .begin = gic_begin,
    .end = gic_end,
    .configure = gic_configure
};


void rumboot_irq_register_gic()
{
    rumboot_irq_register_controller(&irq_ctl);
}
