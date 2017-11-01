/*
 *  DUAL-TIMER (ARM SP804)  registers
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/gic.h>
#include <rumboot/irq.h>

#include <platform/devices.h>
#include <devices/dit.h>
#include <rumboot/dit_lib.h>
#include <rumboot/platform.h>



#define TIMER0_CYCLES           1
#define TIMER1_CYCLES           2

struct irq_number{
    int timer0_irq;
    int timer1_irq;
    int base_addr;
    int dit_index;
};

static void handler0(int irq, void *arg)
{

    struct irq_number *a = (struct irq_number *) arg;
//    *((int*)arg) +=1;
    a->timer0_irq = a->timer0_irq +1 ;
	rumboot_printf("IRQ 0 arrived  \n");
    
	rumboot_printf("DIT %d TIMER 0 INT # == %d  \n",a->dit_index,a->timer0_irq);
    sp804_clrint(0,a->base_addr);
}

static void handler1(int irq, void *arg)
{
//    *((int*)arg) +=1;
    struct irq_number *a = (struct irq_number *) arg;
    a->timer1_irq = a->timer1_irq+1;
	rumboot_printf("IRQ 1 arrived  \n");
	rumboot_printf("DIT %d TIMER 1 INT # == %d  \n",a->dit_index,a->timer1_irq);
    sp804_clrint(1,a->base_addr);

}


bool test_dit_4_timers (uint32_t structure)
{
    int c = 0;
    int d = 0;
   
    struct irq_number *stru = (struct irq_number *) structure;
    int base_addr = stru->base_addr ;

    
    struct sp804_conf config_0;
    config_0.mode             = ONESHOT    ;
    config_0.interrupt_enable = 1          ;
    config_0.clock_division   = 1          ;
    config_0.width            = 32         ;
    config_0.load             = 100        ;
    config_0.bgload           = 0          ;
    
    struct sp804_conf config_1;
    config_1.mode             = ONESHOT    ;
    config_1.interrupt_enable = 1          ;
    config_1.clock_division   = 1          ;
    config_1.width            = 32         ;
    config_1.load             = 200        ;
    config_1.bgload           = 0          ;

    for (int i =0; i<TIMER0_CYCLES+stru->dit_index; i++)
    {
        sp804_config(config_0,0,base_addr);
        sp804_enable(0,base_addr); 
        while(sp804_get_value(0,base_addr)){};
        rumboot_printf("TIMER0 CYCLE # %d \n", i);
        c++;
    }
    
    for (int i =0; i<TIMER1_CYCLES+stru->dit_index; i++)
    {
        sp804_config(config_1,1,base_addr);
        sp804_enable(1,base_addr); 
        while(sp804_get_value(1,base_addr)){};
        rumboot_printf("TIMER1 CYCLE # %d \n", i);
        d++;
    }    
    
    
    
    
    if(stru->timer0_irq==TIMER0_CYCLES+stru->dit_index){
            rumboot_printf("TEST 0 OK \n");
    }
    else
    {
            rumboot_printf("ERROR IN TEST 0 \n");
            rumboot_printf("INTERRUPTS CAME == %d, SHOULD BE %d \n", stru->timer0_irq,TIMER0_CYCLES+stru->dit_index);
            return false;
    }

    if(stru->timer1_irq==TIMER1_CYCLES+stru->dit_index){
            rumboot_printf("TEST 1 OK \n");
    }
    else
    {
            rumboot_printf("ERROR IN TEST 1 \n");
            rumboot_printf("INTERRUPTS CAME == %d, SHOULD BE %d \n", stru->timer1_irq,TIMER1_CYCLES+stru->dit_index);
            return false;
    }
 
    return true;
}


static struct irq_number in[] = {
    {
        .base_addr = DIT0_BASE,
        .dit_index = 0
    },
    {
        .base_addr = DIT1_BASE,
        .dit_index = 1
    },
    {
        .base_addr = DIT2_BASE,
        .dit_index = 2
    },
    {
        .base_addr = DIT3_BASE,
        .dit_index = 3
    }   
};

TEST_SUITE_BEGIN(dit_testlist, "SP804 IRQ TEST")
TEST_ENTRY("SP804_0", test_dit_4_timers, (uint32_t) &in[0]),
TEST_ENTRY("SP804_1", test_dit_4_timers, (uint32_t) &in[1]),
TEST_ENTRY("SP804_2", test_dit_4_timers, (uint32_t) &in[2]),
TEST_ENTRY("SP804_3", test_dit_4_timers, (uint32_t) &in[3]),
TEST_SUITE_END();



int main ()
{
// Set up interrupt handlers    
	rumboot_irq_cli();
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, 42, 0, handler0, &in[0] );
    rumboot_irq_set_handler(tbl, 43, 0, handler1, &in[0] );
    rumboot_irq_set_handler(tbl, 44, 0, handler0, &in[1] );
    rumboot_irq_set_handler(tbl, 45, 0, handler1, &in[1] );
    rumboot_irq_set_handler(tbl, 46, 0, handler0, &in[2] );
    rumboot_irq_set_handler(tbl, 47, 0, handler1, &in[2] );
    rumboot_irq_set_handler(tbl, 48, 0, handler0, &in[3] );
    rumboot_irq_set_handler(tbl, 49, 0, handler1, &in[3] );



	/* Activate the table */
	rumboot_irq_table_activate(tbl);
	rumboot_irq_enable(42);
	rumboot_irq_enable(43);
	rumboot_irq_enable(44);
	rumboot_irq_enable(45);
	rumboot_irq_enable(46);
	rumboot_irq_enable(47);
	rumboot_irq_enable(48);
	rumboot_irq_enable(49);
	rumboot_irq_sei();

// Run tests and return failed one    
    
    int ret = test_suite_run(NULL, &dit_testlist);
    rumboot_printf("%d tests from suite failed\n", ret);
    return ret;
}
