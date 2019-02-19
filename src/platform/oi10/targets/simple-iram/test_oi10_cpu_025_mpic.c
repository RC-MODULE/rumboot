/*
 * test_oi10_cpu_025_mpic.c
 *
 *  Created on: Feb 14, 2019
 *      Author: m.smolina
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>
#include <platform/interrupts.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/mpic128.h>
#include <platform/regs/regs_mpic128.h>
#include <platform/regs/fields/emi.h>

typedef enum
{
    EVENT_CHECK_MPIC_TIMER0_INT = TEST_EVENT_CODE_MIN,
    EVENT_CHECK_MPIC_TIMER1_INT,
    EVENT_CHECK_MPIC_TIMER2_INT,
    EVENT_CHECK_MPIC_TIMER3_INT,
	EVENT_CHECK_MPIC_IPI_INT
} TEST_OI10_CPU_025_EVENT_CODE;

enum {
	MPIC_RESET_TYPE = 0,
	MPIC_RESET = 1
};

#define DELAY                               (0x2100)   // external, MPIC
#define TEST_OI10_025_MPIC_TIMEOUT    10000000

volatile bool HANDLER_FLAG = 0;
volatile uint32_t MPIC_TIMER_NUMBER = 0;

uint32_t reset_type = 0;
uint32_t dbcr = 0;

static void mpic_ipi_generate_interrupt()
{
    rumboot_printf("Check mpic reset\n");
    test_event( EVENT_CHECK_MPIC_IPI_INT);

    dcr_write(DCR_MPIC128_BASE + MPIC128_IPID_PR, (1 << 0));

}

static void  mpic_timer_generate_interrupt (uint32_t delay)
{
	uint32_t mpic_timer = rumboot_platform_runtime_info->persistent[MPIC_RESET_TYPE];

    switch (rumboot_platform_runtime_info->persistent[MPIC_RESET_TYPE])
    {
        case 0:
            test_event(EVENT_CHECK_MPIC_TIMER0_INT);
            break;
        case 1:
            test_event(EVENT_CHECK_MPIC_TIMER1_INT);
            break;
        case 2:
            test_event(EVENT_CHECK_MPIC_TIMER2_INT);
            break;
        case 3:
            test_event(EVENT_CHECK_MPIC_TIMER3_INT);
            break;
    }

    mpic128_timer_init(DCR_MPIC128_BASE, mpic128_timer_freq_SYS_CLK);
    mpic128_start_timer(DCR_MPIC128_BASE, mpic_timer, delay);
}

static void mpic_timer_handler()
{
	HANDLER_FLAG = 1;
    rumboot_printf("Interrupt handler timer\n");
    rumboot_platform_perf("reset_system");
    mpic128_stop_timer(DCR_MPIC128_BASE, rumboot_platform_runtime_info->persistent[MPIC_RESET_TYPE]);
    reset_type = (rumboot_platform_runtime_info->persistent[MPIC_RESET_TYPE] < 3)?(rumboot_platform_runtime_info->persistent[MPIC_RESET_TYPE] + 1):3;
    dbcr = spr_read(SPR_DBCR0)|(reset_type << IBM_BIT_INDEX(64, 35));
    rumboot_platform_runtime_info->persistent[MPIC_RESET_TYPE]++;
    msync();
    spr_write(SPR_DBCR0, dbcr);
}

static void mpic_handler ()
{
	    rumboot_printf("Interrupt handler\n");
	    rumboot_platform_perf("reset_system");
	    dcr_write(DCR_MPIC128_BASE + MPIC128_IPID_PR, 0x00);
	    reset_type = (rumboot_platform_runtime_info->persistent[MPIC_RESET_TYPE] < 3)?(rumboot_platform_runtime_info->persistent[MPIC_RESET_TYPE]+1):3;
	    dbcr = spr_read(SPR_DBCR0)|(reset_type << IBM_BIT_INDEX(64, 35));
	    msync();
	    spr_write(SPR_DBCR0, dbcr);
}

static void init_handlers()
{
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    	switch (rumboot_platform_runtime_info->persistent[MPIC_RESET_TYPE])
	{
		case 0:
			if (rumboot_platform_runtime_info->persistent[MPIC_RESET] == 0)
			{
		    rumboot_irq_set_handler( tbl, MPIC128_IPI_0 , RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, mpic_handler, ( void* )0 );

		    /* Activate the table */
		    rumboot_irq_table_activate( tbl );
		    rumboot_irq_enable( MPIC128_IPI_0  );
		    rumboot_irq_sei();
			}

		    rumboot_irq_set_handler( tbl, MPIC128_TIMER_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, mpic_timer_handler, ( void* )0 );

		    /* Activate the table */
		    rumboot_irq_table_activate( tbl );
		    rumboot_irq_enable( MPIC128_TIMER_0 );
		    rumboot_irq_sei();

			break;
		case 1:
			rumboot_irq_set_handler( tbl, MPIC128_TIMER_1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, mpic_timer_handler, ( void* )0 );

			/* Activate the table */
			rumboot_irq_table_activate( tbl );
			rumboot_irq_enable( MPIC128_TIMER_1 );
			rumboot_irq_sei();
			break;
		case 2:
		    rumboot_irq_set_handler( tbl, MPIC128_TIMER_2, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, mpic_timer_handler, ( void* )0 );

		    /* Activate the table */
		    rumboot_irq_table_activate( tbl );
		    rumboot_irq_enable( MPIC128_TIMER_2 );
		    rumboot_irq_sei();
			break;
		case 3:
		    rumboot_irq_set_handler( tbl, MPIC128_TIMER_3, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, mpic_timer_handler, ( void* )0 );

		    /* Activate the table */
		    rumboot_irq_table_activate( tbl );
		    rumboot_irq_enable( MPIC128_TIMER_3 );
		    rumboot_irq_sei();
			break;
	}
}

int main ()
{
	test_event_send_test_id( "test_oi10_cpu_025_mpic");

    if (rumboot_platform_runtime_info->persistent[MPIC_RESET_TYPE] == 4)
    {
    	  return 0;
    }

    init_handlers();

	if (rumboot_platform_runtime_info->persistent[MPIC_RESET] == 0)
    {
		rumboot_platform_runtime_info->persistent[MPIC_RESET] = 1;
		mpic_ipi_generate_interrupt();
    }

	mpic_timer_generate_interrupt(DELAY);
    while (HANDLER_FLAG == 0) msync();



    return 1;
}
