#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
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
    rumboot_irq_enable(0);
    rumboot_irq_table_activate(tbl);

    asm volatile("swi #0");
    while (!done) { }

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    return 0;
}
