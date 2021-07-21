/* -*- rumboot-test-labels: oi10only -*- */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <platform/devices.h>
#include <platform/regs/regs_scrb.h>
#include <platform/devices/scrb.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <regs/fields/mpic128.h>
#include <platform/interrupts.h>


struct scrb_test_config{
    int test_number;
    int im1_enable;
    int im2_enable;
    int mode;
    int pri;
    int dont_mask_all;
    int dont_mask_sep;
    uint32_t timer_value;
    int all_errors;
    int im1_errors;
    int im2_errors;

    struct irq_struct *irq_struct_inst ;
};

struct irq_struct{
    int int0_irq;
    int int1_irq;
};


// Increase relative counter for every interrupt caught
static void handler0( int irq, void *arg ) {
    scrb_clear_interrupt_0(SCRB_BASE);
    struct irq_struct *a = ( struct irq_struct * )arg;
    a->int0_irq = a->int0_irq + 1;
}

// Increase relative counter for every interrupt caught
static void handler1( int irq, void *arg ) {
    scrb_clear_interrupt_1(SCRB_BASE);
    struct irq_struct *a = ( struct irq_struct * )arg;
    a->int1_irq = a->int1_irq + 1;
}


// Check if value of valid parts of FCNT reg is no empty
// It should be not empty at the end of scrubbing
// Also should clean it by hand if multiple scrb runs are made
bool check_if_scrubbing_done(void *arg) {
    struct scrb_test_config *a = ( struct scrb_test_config * )arg;
    bool result;
    result=true;
    if (a->im1_enable) {
        result &=(scrb_get_number_im1_errors(SCRB_BASE)>0);
    }
    if (a->im2_enable) {
        result &=(scrb_get_number_im2_errors(SCRB_BASE)>0);
    }
    return result;
}





bool test_scrb( uint32_t structure ) {

    // Config
    bool result;
    uint32_t mask_value;

    struct scrb_test_config *stru = ( struct scrb_test_config * )structure;
    struct irq_struct *irq_struct_inst = (struct irq_struct *)stru->irq_struct_inst;

    scrb_set_timer(SCRB_BASE, stru->timer_value);
    scrb_set_allcnt(SCRB_BASE, stru->all_errors);
    scrb_set_fref(SCRB_BASE, stru->im1_errors);

    mask_value = (stru->dont_mask_all<<1)|(stru->dont_mask_sep);
    scrb_set_imr(SCRB_BASE, mask_value);
    scrb_set_work_mode(SCRB_BASE, stru->mode);
    scrb_set_priority(SCRB_BASE, stru->pri);


    result = true;

    // Clear previous FCNT value so its is possible
    // to check, if scrubbing is done. If this is not done
    // You cand differentiate between current scrb run and previous
    // coz this reg is filled with nonzero from prev run
    iowrite32(0, SCRB_BASE+SCRB_REG_FCNT);


    irq_struct_inst->int0_irq=0;
    irq_struct_inst->int1_irq=0;

    // Инициализация тестбенча - ивент для записи ошибок в память
    rumboot_platform_sv_event("INSERT_MEM_ERROR");

    // Запуск
    rumboot_printf("START SCRB FROM TEST %d\n", stru->test_number);
    scrb_start_scrubber(SCRB_BASE, (stru->im2_enable)<<1|(stru->im1_enable));

    // Ожидание завершения


    do {
        for (int i = 0; i<100; i++){
        }

    } while( !check_if_scrubbing_done(stru));

    // Завершение
    rumboot_printf("Stoping scrubbing\n");
    scrb_stop_scrubbing_im1(SCRB_BASE);
    scrb_stop_scrubbing_im2(SCRB_BASE);


    int interrupts_0_caught = irq_struct_inst->int0_irq;
    int interrupts_1_caught = irq_struct_inst->int1_irq;

    int errors_fcnt_im1 = scrb_get_number_im1_errors(SCRB_BASE);
    int errors_fcnt_im2 = scrb_get_number_im2_errors(SCRB_BASE);


    // Add comment to this
    int errors_allcnt = interrupts_1_caught*(stru->all_errors+1)+(stru->all_errors-scrb_get_number_total_errors_left_in_reg(SCRB_BASE));

    int errors_im1_expected = stru->im1_enable ? 20 : 0;//%stru->im1_errors;
    int errors_im2_expected = stru->im2_enable ? 20 : 0;//%stru->im2_errors;
    int errors_all_expected = errors_im1_expected+errors_im2_expected;//%stru->all_errors;



    int interrupts_0_expected = stru->dont_mask_sep ? 1 : 0;
    int interrupts_0_other_expected = stru->dont_mask_sep ? 2 : 0;
    int interrupts_1_expected = stru->dont_mask_all ? errors_all_expected/(stru->all_errors+1) : 0;

    // Проверка числа прерываний

    // Other expected is nessesary in case that scrubbing of im1 and im2 wont finish at ther same time
    // In that case there will be 2 int0 instead of 1
    if ((interrupts_0_caught!=interrupts_0_expected)&(interrupts_0_caught!=interrupts_0_other_expected)){
        rumboot_printf("Wrong number of interrupt0 - got %d, expected - %d\n", interrupts_0_caught, interrupts_0_expected);
        result = false;
    }

    if (interrupts_1_caught!=interrupts_1_expected){
        rumboot_printf("Wrong number of interrupt1 - got %d, expected - %d\n", interrupts_1_caught, interrupts_1_expected);
        result = false;
    }


    // Проверка числа ошибок

    if (errors_fcnt_im1!=errors_im1_expected){
        rumboot_printf("Wrong number of im1 fcnt errors - got %d, expected - %d\n", errors_fcnt_im1, errors_im1_expected);
        result = false;
    }

    if (errors_fcnt_im2!=errors_im2_expected){
        rumboot_printf("Wrong number of im2 fcnt errors - got %d, expected - %d\n", errors_fcnt_im2, errors_im2_expected);
        result = false;
    }

    // Cant check allcnt errors if its interrupt is masked
    if(stru->dont_mask_all){
        if (errors_allcnt!=errors_all_expected){
            rumboot_printf("Wrong number of allcnt errors - got %d, expected - %d\n", errors_allcnt, errors_all_expected);
            result = false;
        }
    }

    // It is nessesary to have a delay after scrb run in mode == 1
    // Coz you cant control run of scrubbing as in mode==0
    // So you realy cant be sure when scrubbing stops and its safe
    // to do another run
    int temp_counter=0;
    if (stru->mode == 1){
        for (int i = 0; i<100; i++){
            scrb_get_number_im1_errors(SCRB_BASE);
            //if (i%7){
            //    temp_counter++;
            //}
        }
    }

    // Event is needed to prevent deadlock in while True loop in testbench
    rumboot_platform_sv_event("WAIT_FOR_END_OF_SCRUBBING");

    return result;
}



static struct irq_struct irq_struct_instance = {
    .int0_irq = 0,
    .int1_irq = 0,
};



// Тесты
//     | Who         | Mode | Pri | Mask all | Mask sep | Timer     | All errors | Err im1 | Err im2
//  0  | im1         | 1    | 0   | 0        | 1        | 1<<18 бит | 4          | 3       | 3      
//  1  | im2         | 1    | 1   | 1        | 1        | 1<<20 бит | 4          | 3       | 3      
//  2  | im2+im1     | 0    | 1   | 0        | 0        | 0 бит     | 4          | 3       | 3      



static struct scrb_test_config in[ ] = {
    // 0
    {
        .test_number    = 0,
        .im1_enable     = 1,
        .im2_enable     = 0,
        .mode           = 1,
        .pri            = 0,
        .dont_mask_all  = 1,
        .dont_mask_sep  = 0,
        .timer_value    = 1<<18,
        .all_errors     = 4,
        .im1_errors     = 3,
        .im2_errors     = 3,
        .irq_struct_inst = &irq_struct_instance,
    },
    // 1
    {
        .test_number    = 1,
        .im1_enable     = 0,
        .im2_enable     = 1,
        .mode           = 1,
        .pri            = 1,
        .dont_mask_all  = 0,
        .dont_mask_sep  = 0,
        .timer_value    = 1<<20,
        .all_errors     = 4,
        .im1_errors     = 3,
        .im2_errors     = 3,
        .irq_struct_inst = &irq_struct_instance,
    },
    // 2
    {
        .test_number    = 2,
        .im1_enable     = 1,
        .im2_enable     = 1,
        .mode           = 0,
        .pri            = 1,
        .dont_mask_all  = 1,
        .dont_mask_sep  = 1,
        .timer_value    = 0,
        .all_errors     = 4,
        .im1_errors     = 3,
        .im2_errors     = 3,
        .irq_struct_inst = &irq_struct_instance,
    },

};




TEST_SUITE_BEGIN(scrb_testlist, "SCRB TEST")
    TEST_ENTRY("SCRB_0", test_scrb, (uint32_t) &in[0]),
    TEST_ENTRY("SCRB_1", test_scrb, (uint32_t) &in[1]),
    TEST_ENTRY("SCRB_2", test_scrb, (uint32_t) &in[2]),

TEST_SUITE_END();

int main() {
// Set up interrupt handlers
    rumboot_printf( "SCRB test START\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, SCRB_INT_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler0, &irq_struct_instance );
    rumboot_irq_set_handler( tbl, SCRB_INT_1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, &irq_struct_instance );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( SCRB_INT_0 );
    rumboot_irq_enable( SCRB_INT_1 );
    rumboot_irq_sei();

// Run tests and return failed one

    int ret = test_suite_run( NULL, &scrb_testlist );
    rumboot_printf( "%d tests from suite failed\n", ret );
    return ret;
}
