#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <platform/devices.h>
#include <devices/interprocessor_irq.h>

#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>

struct irq_struct{
    int int0_irq;
    int int1_irq;
};

static void nmc_handler0( int irq, void *arg ) {
    rumboot_printf("HANDLER 0 NMC\n");
    interprocessor_irq_clear_interrupt(SC_BASE, 0);

    struct irq_struct *a = ( struct irq_struct * )arg;
    a->int0_irq = a->int0_irq + 1;

    uint32_t irq_sent_by_ppc;
    irq_sent_by_ppc = 0;
    while( irq_sent_by_ppc!=0xFF) {
        irq_sent_by_ppc = interprocessor_irq_get_irq_confirmation(SC_BASE);
        //irq_sent_by_ppc = interprocessor_irq_check_irq_confirmation(SC_BASE, 0);
    }

    // Set same type interrupt back to PPC
    interprocessor_irq_set_interrupt(SC_BASE, 0);

    // Send through GP signals info that irq was set back 
    // This is done so that PPC can wait for specifiq bit in its GP reg
    // and then check int0_irq counter
    interprocessor_irq_set_irq_confirmation(SC_BASE, 0xFF);
  }


static void nmc_handler1( int irq, void *arg ) {
    rumboot_printf("HANDLER 1 NMC START\n");
    interprocessor_irq_clear_interrupt(SC_BASE, 1);

    struct irq_struct *a = ( struct irq_struct * )arg;
    a->int1_irq = a->int1_irq + 1;

    uint32_t irq_sent_by_ppc = 0;

    while( irq_sent_by_ppc!=0xFF) {
        irq_sent_by_ppc = interprocessor_irq_get_irq_confirmation(SC_BASE);
    }
 
    // Set same type interrupt back to PPC
    interprocessor_irq_set_interrupt(SC_BASE, 1);
    interprocessor_irq_set_irq_confirmation(SC_BASE, 0xFF);

    //// Send through GP signals info that irq was set back 
    //// This is done so that PPC can wait for specifiq bit in its GP reg
    //// and then check int0_irq counter
    rumboot_printf("HANDLER 1 NMC END\n");
}


static struct irq_struct nmc_irq_struct_instance = {
    .int0_irq = 0,
    .int1_irq = 0,
};


int main() {
// Set up interrupt handlers
    rumboot_printf( "NMC interprocessor irq integration test START\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, PPC_HIGH_LEVEL_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, nmc_handler0, &nmc_irq_struct_instance );
    rumboot_irq_set_handler( tbl, PPC_LOW_LEVEL_INT , RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, nmc_handler1, &nmc_irq_struct_instance );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );

    rumboot_irq_enable(PPC_HIGH_LEVEL_INT);
    rumboot_irq_enable(PPC_LOW_LEVEL_INT);
    rumboot_irq_sei();

// Run tests and return failed one
    int i = 0 ;
    while (1) {
        //rumboot_printf("Hello, my uptime is %d\n", rumboot_platform_get_uptime());
        udelay(300);
    }    

    return 1;
}