#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <regs/regs_gic.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>

static inline void gic_dist_write(const struct rumboot_irq_controller *dev, uint32_t reg, uint32_t value)
{
	iowrite32(value, dev->base0 + reg);
}

static inline void gic_cpuif_write(const struct rumboot_irq_controller *dev, uint32_t reg, uint32_t value)
{
	iowrite32(value, dev->base1 + reg);
}

static inline uint32_t gic_dist_read(const struct rumboot_irq_controller *dev, uint32_t reg)
{
	return ioread32(dev->base0 + reg);
}

static inline uint32_t gic_cpuif_read(const struct rumboot_irq_controller *dev, uint32_t reg)
{
	return ioread32(dev->base1 + reg);
}

static int gic_init(const struct rumboot_irq_controller *dev)
{
	/* Enable IRQ forwarding & set PMR */
	gic_cpuif_write(dev, GICC_REG_PMR, 0xff);
	gic_cpuif_write(dev, GICC_REG_CTLR, 1);
	gic_dist_write(dev, GICD_REG_CTLR, 1);
	return 0;
}

static uint32_t gic_begin(const struct rumboot_irq_controller *dev, int id)
{
	uint32_t irq = gic_cpuif_read(dev, GICC_REG_IAR);

	if (!irq && dev->slave_save)
		dev->slave_save();

	return irq;
}

static void gic_end(const struct rumboot_irq_controller *dev, int irq)
{
	gic_cpuif_write(dev, GICC_REG_EOIR, irq);

	if (!irq && dev->slave_restore)
		dev->slave_restore();

	return;
}

static void gic_configure(const struct rumboot_irq_controller *dev, int irq, uint32_t flags, int enable)
{
	/* IRQ < 16 (SW) can't be configured */
	if (irq < 16) {
		return;
	}

	gic_dist_write(dev, GICD_REG_CTLR, 0);
	gic_cpuif_write(dev, GICC_REG_CTLR, 0);

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

	gic_dist_write(dev, reg, 1 << shift);

	gic_cpuif_write(dev, GICC_REG_PMR, 0xff);
	gic_cpuif_write(dev, GICC_REG_CTLR, 1);
	gic_dist_write(dev, GICD_REG_CTLR, 1);
}

static void gic_swint(const struct rumboot_irq_controller *dev, int irq)
{
	if (irq == 0) {
		iowrite32(GIC_GENSWINT0, (dev->base0 + GICD_REG_SGIR));
	} else {
		rumboot_platform_panic("gic: Can't generate sw irq on line %d", irq);
	}
}

static const struct rumboot_irq_controller irq_ctl = {
	.name     = "GIC",
	.first		= 0,
	.last		= 127,
	.init		= gic_init,
	.begin		= gic_begin,
	.end		= gic_end,
	.configure	= gic_configure,
	.generate_swint = gic_swint,
	.slave_save	= NULL,
	.slave_restore	= NULL,
	.base0 = GIC_DIST_BASE,
	.base1 = GIC_CPUIF_BASE
};


void rumboot_irq_register_gic()
{
	rumboot_irq_register_controller(&irq_ctl);
}
