#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>

#include <platform/devices.h>
#include <devices/gp_timers.h>
#include <rumboot/platform.h>
#include <platform/interrupts.h>

#include <regs/regs_gp_timers.h>


static void irq_handler(int irq, void *arg)
{
    int  *a = (int *) arg;
    *a = *a +1 ;
	rumboot_printf("IRQ  arrived  \n");
	rumboot_printf(" INT # %d  \n",*a);
    iowrite32 (0x0, GLOBAL_TIMERS + TMR_9_STATE);
}



  static int num_of_interrupts = 0;

 
void run_gp_global_event_timer_once (int limit)
{
    gp_timer_turn_on ();
    iowrite32 (limit, GLOBAL_TIMERS + TMR_9_LIMIT);
    iowrite32 (0x2, GLOBAL_TIMERS + TMR_9_STATE);
}


int main ()
{
    rumboot_printf("GP_Global_Timers test START\n");
	rumboot_irq_cli();
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, GP_GLOB_INT, 0, irq_handler, &num_of_interrupts );

	rumboot_irq_table_activate(tbl);
	rumboot_irq_enable(GP_GLOB_INT);
	rumboot_irq_sei();
    
    run_gp_global_event_timer_once(0x3);

    for(int a = 0; a < 100; a = a + 1 ){
         asm volatile ("nop"::);
    }
    if (num_of_interrupts == 1){
        rumboot_printf("GP_Global_Timers test OK\n");
        return 0;
    }
    else{
        rumboot_printf("ERROR\n");
        return 1;
    }
    
   return -1;
}