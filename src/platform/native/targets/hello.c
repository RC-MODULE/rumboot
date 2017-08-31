#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

void handler(void *arg)
{
    rumboot_platform_panic("Not implemented!\n");
}

int main()
{

    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, 3, handler, 123);
    rumboot_irq_activate_table(tbl);

    /* IIRQ ... */

    rumboot_irq_activate_table(NULL);
    rumboot_irq_free(tbl);

    rumboot_printf("Hello world: %d !\n", 1);
    return 0;
}
