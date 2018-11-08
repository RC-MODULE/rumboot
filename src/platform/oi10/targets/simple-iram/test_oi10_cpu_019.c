/*
 * test_oi10_cpu_019.c
 *
 *  Created on: Nov 1, 2018
 *      Author: a.gurov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RUMBOOT_ASSERT_WARN_ONLY


#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/common_macros/common_macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/dma2plb6.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>

#include <platform/test_event_codes.h>
#include <platform/interrupts.h>

#define TEST_STRING_ID              "test_oi10_cpu_019"

/* Constants macros */
#define NOTHING                     0x00000000
#define TEST_OK                     NOTHING
#define TEST_ERROR                  !TEST_OK
#define DISABLED                    NOTHING
#define ENABLED                     !DISABLED
#define YES                         ENABLED
#define NO                          DISABLED
#define IRQ_WAIT_TIMEOUT            100         /* us */
#define IRQ_MIN                     32
#define IRQ_MAX                     (IRQ_MIN + 96)
#define IRQ_CNT                     (IRQ_MAX - IRQ_MIN)
#define IRQ_FLAGS                   (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)
#define IRQ_ARR_SZ                  (IRQ_CNT / (sizeof(uint32_t) * 8))
#define IRQ_LIST_END                0xFF
#define EVERYTHING                  (~0)

/* Other macros */
#define MAY_BE_NOT_USED             __attribute__((unused))
#define _GET_UPTIME                 rumboot_platform_get_uptime
#define RESULT_TPL                  "TEST %s!\n"
#define EVENT_CLR_EXT_INT           EVENT_CLEAR_EXT_INT

/* Utility and function macros */
#define CAST_ADDR(VAL)              ((void*)    (VAL))
#define CAST_UINT(VAL)              ((uint32_t) (VAL))
#define FORCE_USE(VAR)              VAR ^= 0
#define IRQ_IDX(IRQ_NUM)            ((IRQ_NUM) - IRQ_MIN)
#define BIT(B)                      (1 << (B))
#define RESULT_STRING(ST)           (!(ST) ? "OK" : "ERROR")


/* Types and structures definitions */
typedef struct rumboot_irq_entry    rumboot_irq_entry;
typedef volatile uint32_t           irq_flags_t;

/* Global vars */
static irq_flags_t IRQ[IRQ_ARR_SZ];
static irq_flags_t ext_int_raised;
static rumboot_irq_entry *irq_table = NULL;


//--------------------------------------------------------

inline static uint32_t print_result(uint32_t test_status)
{
    rumboot_printf(RESULT_TPL, RESULT_STRING(test_status));
    return test_status;
}

inline static void set_interrupt(irq_flags_t *irqs, uint32_t irq_num)
{
    irqs[IRQ_IDX(irq_num) >> 0x05]
         |= BIT(IRQ_IDX(irq_num) & 0x1F);
}

inline static uint32_t get_interrupt(irq_flags_t *irqs, uint32_t irq_num)
{
    return !!(irqs[IRQ_IDX(irq_num) >> 0x05]
           & BIT(IRQ_IDX(irq_num) & 0x1F));
}

inline static void clear_interrupt(irq_flags_t *irqs, uint32_t irq_num)
{
    irqs[IRQ_IDX(irq_num) >> 0x05] &=
            ~BIT(IRQ_IDX(irq_num) & 0x1F);
}

void clear_all_interrupts(irq_flags_t *irqs)
{
    int i;
    for(i = 0; i < IRQ_ARR_SZ; i++)
        irqs[i] = NOTHING;
}

uint32_t wait_irq(uint32_t usecs, uint32_t irq)
{
    volatile
    uint32_t    tmp;

    tmp = _GET_UPTIME();
    rumboot_printf("Wait for interrupt #%d...\n", irq);
    while(((_GET_UPTIME() - tmp) < (usecs)) && ! get_interrupt(IRQ, irq))
        ;
    rumboot_printf((_GET_UPTIME() - tmp) < (usecs)
            ?"HAVE WAITED IRQ!\n":"IRQ WAIT TIMEOUT!\n");

    return usecs - (_GET_UPTIME() - tmp);
}

void irq_handler( int irq_num, void *arg )
{
    rumboot_printf("IRQ #%d received (Return address: 0x%X).\n",
            irq_num, spr_read(SPR_SRR0));
    /* Clear external interrupt signal
       and increment interrupt number   */
    test_event(EVENT_CLR_EXT_INT);
    ext_int_raised = DISABLED;
    set_interrupt(IRQ, irq_num);
}

void init_interrupts(void)
{
    uint32_t    irq;
    rumboot_putstring ("\tStart IRQ initialization...\n");

    clear_all_interrupts(IRQ);
    rumboot_irq_cli();
    ext_int_raised = NO;
    irq_table = rumboot_irq_create(NULL);
    for(irq = IRQ_MIN; irq < IRQ_MAX; irq++)
        rumboot_irq_set_handler(irq_table, irq,
            IRQ_FLAGS, irq_handler, NULL);

    /* Activate the table */
    rumboot_irq_table_activate(irq_table);
    for(irq = IRQ_MIN; irq < IRQ_MAX; irq++)
        rumboot_irq_enable(irq);
    rumboot_irq_sei();

    rumboot_putstring ("\tIRQ have been initialized.\n");
}

uint32_t check_external_interrupts(void)
{
    uint32_t         status = TEST_OK,
                     result = 0;
    uint32_t         irq;

    for(irq = IRQ_MIN; irq < IRQ_MAX; irq++)
    {
        ext_int_raised = YES;
        /* Set external interrupt signal */
        test_event(EVENT_SET_EXT_INT);
        wait_irq(IRQ_WAIT_TIMEOUT, irq);
        if(ext_int_raised)
        {
            test_event(EVENT_CLR_EXT_INT);
            ext_int_raised = NO;
        }
        status |= !(result = get_interrupt(IRQ, irq));
        clear_interrupt(IRQ, irq);
        if(!result)
            rumboot_printf("While waiting interrupt #%d...\n", irq);
        TEST_ASSERT(result, "EXTERNAL INTERRUPT NOT RECEIVED!\n");
    }

    return status;
}

uint32_t main(void)
{
    test_event_send_test_id(TEST_STRING_ID);
    /* msr_write(msr_read() | BIT(ITRPT_XSR_CE_i) | BIT(ITRPT_XSR_EE_i)); */
    init_interrupts();

    return print_result(check_external_interrupts());
}




