#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <devices/gic.h>
#include <rumboot/io.h>

void handler(int irq, void *arg)
{
    rumboot_printf("IRQ arrived, arg %x\n", arg);
    uint32_t *done = arg;
    *done = 1;
    rumboot_printf("done\n");
}

int main()
{
    volatile uint32_t done = 0;

    /* Disable all interrupts */
    rumboot_irq_cli();

    /* Create an IRQ table */
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    /* Configure handler */
    rumboot_irq_set_handler(tbl, 0, 0, handler, (void *)&done);

    /* Activate the table */
    rumboot_irq_table_activate(tbl);

    /* Enable IRQ */
    rumboot_irq_enable(0);

    /* Allow interrupt handling */
    rumboot_irq_sei();

    /* Generate an interrupt */
    rumboot_printf("Firing GIC_GENSWINT0, arg 0x%x \n", &done);
    iowrite32(GIC_GENSWINT0, (GIC_DIST_BASE + GICD_REG_SGIR));

    /* Wait for it... */
    while (!done) { }

    rumboot_printf("We're back! \n");

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    return 0;
}
