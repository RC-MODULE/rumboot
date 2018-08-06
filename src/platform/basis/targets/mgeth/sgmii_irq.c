#include <stdio.h>
#include <stdlib.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <regs/regs_global_timers.h>
#include <regs/regs_mgeth.h>
#include <devices/mgeth.h>

struct rumboot_irq_entry *tbl;
volatile unsigned int int_cnt = 0;

static void isr(int irq, void *arg)
{
	rumboot_printf("isr start; irq = %d; arg = %d.\n", irq, arg);

	rumboot_printf("PIN_OVRDEN[0] = 0x%X\n", ioread32(SGMII_PHY + 0x1128));
	rumboot_printf("PIN_OVRDVAL[0] = 0x%X\n", ioread32(SGMII_PHY + 0x1130));

	iowrite32(0x0, SGMII_PHY + 0x1130); // PIN_OVRDVAL[0]
	iowrite32(0x0, SGMII_PHY + 0x1128); // PIN_OVRDEN[0]

	rumboot_printf("PIN_OVRDEN[0] = 0x%X\n", ioread32(SGMII_PHY + 0x1128));
	rumboot_printf("PIN_OVRDVAL[0] = 0x%X\n", ioread32(SGMII_PHY + 0x1130));

	int_cnt++;

	rumboot_printf("isr end.\n");
}

int main()
{
	rumboot_printf("================================================================================\n");

	tbl = rumboot_irq_create(NULL);
	rumboot_irq_set_handler(tbl, SGMII_INT, 0x0, isr, (void *)11);
	rumboot_irq_table_activate(tbl);
	rumboot_irq_enable(SGMII_INT);
	rumboot_irq_sei();

	rumboot_printf("Waiting SGMII initialization...\n");

	if (mgeth_init_sgmii(SGMII_PHY, SCTL_BASE))
	{
		rumboot_printf("ERROR: SGMII initialization ERROR!\n");
		return 1;
	}

	rumboot_printf("SGMII initialized.\n");

	rumboot_printf("PIN_OVRDEN[0] = 0x%X\n", ioread32(SGMII_PHY + 0x1128));
	rumboot_printf("PIN_OVRDVAL[0] = 0x%X\n", ioread32(SGMII_PHY + 0x1130));

	iowrite32(0x8, SGMII_PHY + 0x1128); // PIN_OVRDEN[0]
	iowrite32(0x8, SGMII_PHY + 0x1130); // PIN_OVRDVAL[0]

	rumboot_printf("Waiting SGMII interrupt...\n");

	while (!int_cnt);

	rumboot_printf("Interrupt caught (int_cnt = %u).\n", int_cnt);

	rumboot_printf("PIN_OVRDEN[0] = 0x%X\n", ioread32(SGMII_PHY + 0x1128));
	rumboot_printf("PIN_OVRDVAL[0] = 0x%X\n", ioread32(SGMII_PHY + 0x1130));

	if (int_cnt != 1)
	{
		rumboot_printf("ERROR: Wrong interrupts count!\n");
		return 1;
	}

	rumboot_printf("Test PASS.\n");

	return 0;
}
