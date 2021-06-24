#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>
#include <platform/devices.h>

#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>

#include <devices/interprocessor_irq.h>

struct irq_struct{
    int int0_irq;
    int int1_irq;
};


// Increase relative counter for every interrupt caught
static void ppc_handler0( int irq, void *arg ) {
    rumboot_printf("HANDLER 0 PPC\n");
    interprocessor_irq_clear_interrupt(DCR_IIRQ_BASE, 0);

    struct irq_struct *a = ( struct irq_struct * )arg;
    a->int0_irq = a->int0_irq + 1;
}

static void ppc_handler1( int irq, void *arg ) {
    rumboot_printf("HANDLER 1 PPC\n");
    interprocessor_irq_clear_interrupt(DCR_IIRQ_BASE, 1);

    struct irq_struct *a = ( struct irq_struct * )arg;
    a->int1_irq = a->int1_irq + 1;
}


static struct irq_struct ppc_irq_struct_instance = {
    .int0_irq = 0,
    .int1_irq = 0,
};


struct interprocessor_irq_test_config{
    int test_number;
    int test_index;
    struct irq_struct *ppc_irq_struct_inst ;
};

static struct interprocessor_irq_test_config in_new[ ] = {
    // 0
    {
        .test_number    = 0,
        .test_index     = 0,
        .ppc_irq_struct_inst = &ppc_irq_struct_instance,
    },
    // 1
    {
        .test_number    = 1,
        .test_index     = 1,
        .ppc_irq_struct_inst = &ppc_irq_struct_instance,
    },
};


bool test_interprocessor_irq( uint32_t structure ) {

    // Make return error by default
    bool result = 1;

    struct interprocessor_irq_test_config *stru = ( struct interprocessor_irq_test_config * )structure;
    struct irq_struct *ppc_irq_struct_instance = (struct irq_struct *)stru->ppc_irq_struct_inst;
    rumboot_printf("START INTERPROCESSOR IRQ TEST %d\n", stru->test_number);

    int test_index = stru->test_index;

    ppc_irq_struct_instance->int0_irq=0;
    ppc_irq_struct_instance->int1_irq=0;


    interprocessor_irq_set_interrupt(DCR_IIRQ_BASE, test_index);
    interprocessor_irq_set_irq_confirmation(DCR_IIRQ_BASE, 0xFF);

    rumboot_printf("WAIT FOR CONFIRM FROM NMC\n");
    // Whait for confirmation from NMC that is sent irq to PPC
    uint32_t irq_sent_by_nmc = 0;
    while( irq_sent_by_nmc!=0xFF) {
        irq_sent_by_nmc=interprocessor_irq_get_irq_confirmation(DCR_IIRQ_BASE);
        //irq_sent_by_nmc = interprocessor_irq_check_irq_confirmation(DCR_IIRQ_BASE, test_index*4);
    }

    if (test_index == 0){
        result = ppc_irq_struct_instance->int0_irq==1;

    } else if (test_index == 1){
        result = ppc_irq_struct_instance->int1_irq==1;
    }
    return result;
};



TEST_SUITE_BEGIN(interprocessor_irq_testlist, "Interprocessor IRQ TEST")
    TEST_ENTRY("INTERPROCESSOR_IRQ_0", test_interprocessor_irq, (uint32_t) &in_new[0]),
    TEST_ENTRY("INTERPROCESSOR_IRQ_1", test_interprocessor_irq, (uint32_t) &in_new[1]),

TEST_SUITE_END();

int main() {
// Set up interrupt handlers
    rumboot_printf( "interprocessor irq integration test START\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, NMC4_INT_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, ppc_handler0, &ppc_irq_struct_instance );
    rumboot_irq_set_handler( tbl, NMC4_INT_1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, ppc_handler1, &ppc_irq_struct_instance );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( NMC4_INT_0 );
    rumboot_irq_enable( NMC4_INT_1 );
    rumboot_irq_sei();

    // Run tests and return failed one
    int ret = test_suite_run( NULL, &interprocessor_irq_testlist );
    rumboot_printf( "%d tests from suite failed\n", ret );
    return ret;
}