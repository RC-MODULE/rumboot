/*
 *  DUAL-TIMER (ARM SP804)  registers
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/gic.h>
#include <rumboot/irq.h>

#include <platform/devices.h>
#include <devices/dit.h>
#include <rumboot/dit_lib.h>

#define SIZE0           1
#define SIZE1           7



static void handler(int irq, void *arg)
{

	rumboot_printf("IRQ arrived  \n");
}

int main ()
{
	rumboot_irq_cli();
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, 42, 0, handler, NULL);
	/* Activate the table */
	rumboot_irq_table_activate(tbl);
	rumboot_irq_enable(42);
	rumboot_irq_sei();

    rumboot_printf("Start of test \n");
    oneshot_n_cycles_0(SIZE0,66);

	int i = 100;
	while (i--) {
		asm volatile("nop");
	}

 //   oneshot_n_cycles_1(SIZE0,30);

 //   periodic_n_cycles_0(SIZE0,20);
 //   freerun_0(1000);
 //   freerun_0(1000);
 //   freerun_1(900);
 //     periodic_0(1100,800);
 //     periodic_1(5100,500);


    return 0;
}
