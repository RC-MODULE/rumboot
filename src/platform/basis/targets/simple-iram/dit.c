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
#include <platform/interrupts.h>


#define TIMER0_CYCLES           1
#define TIMER1_CYCLES           2

struct s804_instance{
    int timer0_irq;
    int timer1_irq;
    int base_addr;
    int dit_index;
};

static void handler0(int irq, void *arg)
{
    struct s804_instance *a = (struct s804_instance *) arg;
    a->timer0_irq = a->timer0_irq +1 ;
	rumboot_printf("IRQ 0 arrived  \n");
    
	rumboot_printf("sp804_%d timer 0 INT # %d  \n",a->dit_index,a->timer0_irq);
    sp804_clrint(a->base_addr,0);
}

static void handler1(int irq, void *arg)
{
    struct s804_instance *a = (struct s804_instance *) arg;
    a->timer1_irq = a->timer1_irq+1;
	rumboot_printf("IRQ 1 arrived  \n");
	rumboot_printf("sp804_%d timer 0 INT # %d  \n",a->dit_index,a->timer1_irq);
    sp804_clrint(a->base_addr,1);

}


bool test_dit_4_timers (uint32_t structure)
{
    int c = 0;
    int d = 0;
   
    struct s804_instance *stru = (struct s804_instance *) structure;
    int base_addr = stru->base_addr ;

    
    struct sp804_conf config_0 = {
    .mode             = ONESHOT,
    .interrupt_enable = 1      ,
    .clock_division   = 1      ,
    .width            = 32     ,
    .load             = 100    ,
    .bgload           = 0      
    };
    
    struct sp804_conf config_1 = {
    .mode             = ONESHOT,
    .interrupt_enable = 1      ,
    .clock_division   = 1      ,
    .width            = 32     ,
    .load             = 200    ,
    .bgload           = 0      
    };

    for (int i =0; i<TIMER0_CYCLES+stru->dit_index; i++)
    {
        sp804_config(base_addr,&config_0,0);
        sp804_enable(base_addr,0); 
        while(sp804_get_value(base_addr,0)){};
        c++;
    }
    
    for (int i =0; i<TIMER1_CYCLES+stru->dit_index; i++)
    {
        sp804_config(base_addr,&config_1,1);
        sp804_enable(base_addr,1); 
        while(sp804_get_value(base_addr,1)){};
        d++;
    }    
    
    
    
    
    if(stru->timer0_irq==TIMER0_CYCLES+stru->dit_index){
            rumboot_printf("Timer 0 test OK \n");
    }
    else
    {
            rumboot_printf("ERROR in Timer 0 test \n");
            rumboot_printf("Interrupts came == %d, should be %d \n", stru->timer0_irq,TIMER0_CYCLES+stru->dit_index);
            return false;
    }

    if(stru->timer1_irq==TIMER1_CYCLES+stru->dit_index){
            rumboot_printf("Timer 1 test OK \n");
    }
    else
    {
            rumboot_printf("ERROR in Timer 1 test \n");
            rumboot_printf("Interrupts came == %d, should be %d \n", stru->timer1_irq,TIMER1_CYCLES+stru->dit_index);
            return false;
    }
 
    return true;
}


static struct s804_instance in[] = {
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
    rumboot_printf("SP804 test START\n");
	rumboot_irq_cli();
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    
    rumboot_irq_set_handler(tbl, DIT0_TIMINT1, 0, handler0, &in[0] );
    rumboot_irq_set_handler(tbl, DIT0_TIMINT2, 0, handler1, &in[0] );
    rumboot_irq_set_handler(tbl, DIT1_TIMINT1, 0, handler0, &in[1] );
    rumboot_irq_set_handler(tbl, DIT1_TIMINT2, 0, handler1, &in[1] );
    rumboot_irq_set_handler(tbl, DIT2_TIMINT1, 0, handler0, &in[2] );
    rumboot_irq_set_handler(tbl, DIT2_TIMINT2, 0, handler1, &in[2] );
    rumboot_irq_set_handler(tbl, DIT3_TIMINT1, 0, handler0, &in[3] );
    rumboot_irq_set_handler(tbl, DIT3_TIMINT2, 0, handler1, &in[3] );



	/* Activate the table */
	rumboot_irq_table_activate(tbl);
	rumboot_irq_enable(DIT0_TIMINT1);
	rumboot_irq_enable(DIT0_TIMINT2);
	rumboot_irq_enable(DIT1_TIMINT1);
	rumboot_irq_enable(DIT1_TIMINT2);
	rumboot_irq_enable(DIT2_TIMINT1);
	rumboot_irq_enable(DIT2_TIMINT2);
	rumboot_irq_enable(DIT3_TIMINT1);
	rumboot_irq_enable(DIT3_TIMINT2);
	rumboot_irq_sei();

// Run tests and return failed one    
    
    int ret = test_suite_run(NULL, &dit_testlist);
    rumboot_printf("%d tests from suite failed\n", ret);
    return ret;
}
