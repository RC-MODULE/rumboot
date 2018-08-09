#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/pcie_test_lib.h>
#include <platform/interrupts.h>
#include <platform/devices.h>
#include <regs/regs_gic.h>
#include <regs/regs_pcie.h>


static void msix_set_vector(uint32_t vector_number, uint32_t addr, uint32_t data, uint32_t mask)
{
    uint32_t addr_pointer = EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address;
    addr_pointer += (vector_number << 4);
    iowrite32 (addr, addr_pointer);
    addr_pointer += 8;
    iowrite32 (data, addr_pointer);
    addr_pointer += 4;
    iowrite32 (mask, addr_pointer);
}


#define ADTRANS_SLV_IRQ_SPI_SHORT (ADTRANS_SLV_IRQ_SPI - 32)

static int msix_proxy_init(const struct rumboot_irq_controller *dev)
{
	/* Enable MSI-X */
	iowrite32(0x80000000, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
	return 0; /* We're good */
}

static void msix_proxy_save(void)
{
	/* Prohibit MSI-X */
	iowrite32(0xC0000000, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);

	return;
}

static void msix_proxy_restore(void)
{
	/* Resolve MSI-X */
	iowrite32(0x80000000, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);

	return;
}

static uint32_t msix_proxy_begin(const struct rumboot_irq_controller *dev)
{
	int irq_start = dev->first;
	uint32_t tmp;

	tmp = ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l);
	tmp &= ~ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
	tmp &= ~ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_l); // See Global_IRQ_Map_X!!!

	if (tmp)
		return irq_start + ffs(tmp) - 1;

	tmp = ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h);
	tmp &= ~ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
	tmp &= ~ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_h);

	if (tmp)
		return irq_start + 32 + ffs(tmp) - 1;

	rumboot_platform_panic("irq-proxy: WARN: Interrupt with no status bit\n");
	return 0;
}

static void msix_proxy_end(const struct rumboot_irq_controller *dev, uint32_t irq)
{
	uint32_t tmp;

	tmp = ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l);
	tmp &= ~ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
	tmp &= ~ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_l);

	iowrite32(tmp, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l);

	tmp = ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h);
	tmp &= ~ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
	tmp &= ~ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_PBA_Entries_h);

	iowrite32(tmp, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h);

	return;
}

static void msix_proxy_configure(const struct rumboot_irq_controller *dev, int irq, uint32_t flags, int enable)
{
	uint32_t rdata;
	int mirq = irq;
	if (irq <= 31) {
		rdata = ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
		if (!enable) {
			iowrite32(rdata | ((1 << mirq)), EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
		} else {
			iowrite32(rdata & (~(1 << mirq)), EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
		}
	} else if (irq <= 63) {
		mirq -= 32;
		rdata = ioread32(EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
		if (!enable) {
			iowrite32(rdata | ((1 << mirq)), EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
		} else {
			iowrite32(rdata & (~(1 << mirq)), EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
		}
	} else {
		rumboot_platform_panic("IRQ %d out of bounds\n", irq);
	}

	msix_set_vector(irq, GIC_DIST_BASE + GICD_REG_SGIR, GIC_GENSWINT0, 0);
}

static const struct rumboot_irq_controller irq_ctl = {
	.first		= 128,
	.last		= 255,
	.init		= msix_proxy_init,
	.begin		= msix_proxy_begin,
	.end		= msix_proxy_end,
	.configure	= msix_proxy_configure
};


static void irq_proxy_handler(int irq, void *arg)
{
	rumboot_irq_core_dispatch(1, RUMBOOT_IRQ_TYPE_NORMAL, 0);
}

void rumboot_irq_register_msix_proxy_controller()
{
	struct rumboot_irq_controller *ctl = (struct rumboot_irq_controller *)rumboot_irq_controller_by_id(0);

	rumboot_irq_set_handler(NULL, 0, 0, irq_proxy_handler, NULL);
	ctl->slave_save = msix_proxy_save;
	ctl->slave_restore = msix_proxy_restore;

	rumboot_irq_register_controller(&irq_ctl);
}
