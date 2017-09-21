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

    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, 0, 0, handler, (void *)&done);
    rumboot_irq_set_handler(tbl, 1, 0, handler, (void *)&done);

    rumboot_irq_enable(0);
    rumboot_irq_enable(1);
    
    rumboot_irq_table_activate(tbl);

#if 0
    rumboot_printf("Firing swi IRQ, arg %x\n", &done);
    asm("swi #3");
    while (!done) { }
    rumboot_printf("And we got back...\n");
    return 0;
#endif

    rumboot_irq_sei();
    iowrite32(GIC_GENSWINT0, (GIC_DIST_BASE + GICD_REG_SGIR));
    rumboot_printf("FIRE! \n");
//    iowrite32(GIC_GENSWINT1, GIC_DIST_BASE + GICD_REG_SGIR);
    /* Wait for the interrupt */


    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    return 0;
}
