#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>

#include <arch/ppc_476fp_mmu.h>
#include <arch/common_macros.h>
#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <arch/dma2plb6.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>
#include <platform/devices/itrace.h>


/*                               MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
#define TLB_ENTRY_CACHE_VALID    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )


static void irq_handler( int irq, void *arg )
{
    TEST_ASSERT((itrace_get_status(DCR_ITRACE_BASE) & ITRACE_STATUS_TERMINATED) == ITRACE_STATUS_TERMINATED, "Tracing does not terminated!");
    itrace_clear_status(DCR_ITRACE_BASE, ITRACE_STATUS_TERMINATED);
}


int main(void)
{
    rumboot_putstring("Starting test test_oi10_cpu_027 ...\n");

    test_event_send_test_id( "test_oi10_cpu_027" );

    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, ITRACE_COMPLETE, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( ITRACE_COMPLETE );
    rumboot_irq_sei();


    emi_init(DCR_EM2_EMI_BASE);

    rumboot_platform_request_file("SRAM0BIN", SRAM0_BASE);
    rumboot_putstring("Starting test code from another memory ...\n");

    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_VALID};
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid, 1);

    ((void (*)())(SRAM0_BASE))();

    rumboot_putstring("Test has been finished successfully.\n");

    return 0;
}
