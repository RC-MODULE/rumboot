#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/rumboot.h>
#include <rumboot/irq.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <platform/regs/regs_plb4ahb.h>
#include <arch/ppc_476fp_config.h>

#ifndef NON_EXISTENT_AXI_ADDR
#define NON_EXISTENT_AXI_ADDR   0xC0025000
#endif

#ifndef AXI_HEAP
#define AXI_HEAP                "IM1"
#endif

#define TEST_VALUE              0xBABADEDA

typedef enum test_event_code {
    TEC_START_CHECK_HRESP = TEST_EVENT_CODE_MIN,
    TEC_START_CHECK_CELL1,
    TEC_START_CHECK_CELL2
} test_event_code;

static volatile bool PLB6PLB41_O_BR6TO4_handled;
static volatile bool L2C0_MCHKOUT_handled;

#define TEST_OI10_CPU_025_PPC_TIMEOUT    100

static void machine_check_interrupt_handler(int id, const char *name)
{
    rumboot_printf("\nWE GOT AN EXCEPTION: %d: %s\n", id, name);

    rumboot_printf("MSR:  0x%x\n", msr_read());
    rumboot_printf("SRR0: 0x%x\n", spr_read(SPR_SRR0));
    rumboot_printf("SRR1: 0x%x\n", spr_read(SPR_SRR1));
    rumboot_printf("CSRR0: 0x%x\n", spr_read(SPR_CSRR0));
    rumboot_printf("CSRR1: 0x%x\n", spr_read(SPR_CSRR1));
    rumboot_printf("MCSRR0: 0x%x\n", spr_read(SPR_MCSRR0));
    rumboot_printf("MCSRR1: 0x%x\n", spr_read(SPR_MCSRR1));
    rumboot_printf("MCSR:  0x%x\n", spr_read(SPR_MCSR_RW));
    spr_write(SPR_MCSR_C,spr_read(SPR_MCSR_RW));
}

static void BR6TO4_interrupt_handler(int irq, void *args)
{
    rumboot_printf( "Cleaning of registers SEAR, SESR\n" );
    dcr_write( DCR_PLB4AHB_0_BASE + SEAR_ADDRUPPER, 0xF );
    dcr_write( DCR_PLB4AHB_0_BASE + SEAR_ADDRLOWER, 0xF );
    dcr_write( DCR_PLB4AHB_0_BASE + SESR, 0xF );
    PLB6PLB41_O_BR6TO4_handled = true;
}

static void L2C0_MCHKOUT_interrupt_handler(int irq, void *args)
{
    rumboot_printf( "Cleaning of register L2PLBSTAT1\n" );
    dcr_write( DCR_PLB4AHB_0_BASE + SEAR_ADDRUPPER, 0xF );
    dcr_write( DCR_PLB4AHB_0_BASE + SEAR_ADDRLOWER, 0xF );
    dcr_write( DCR_PLB4AHB_0_BASE + SESR, 0xF );
    l2c_l2_write( DCR_L2C_BASE, L2C_L2PLBSTAT1, 0xFFFFFFFF );
    L2C0_MCHKOUT_handled = true;
}

static void init_handlers()
{
rumboot_irq_cli();
struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

rumboot_irq_set_handler( tbl, PLB6PLB41_O_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, BR6TO4_interrupt_handler, NULL );
rumboot_irq_set_handler( tbl, L2C0_MCHKOUT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, L2C0_MCHKOUT_interrupt_handler, NULL );

/* Activate the table */
rumboot_irq_table_activate( tbl );
rumboot_irq_enable( PLB6PLB41_O_BR6TO4_INTR );
rumboot_irq_enable( L2C0_MCHKOUT );
rumboot_irq_sei();

rumboot_irq_set_exception_handler(machine_check_interrupt_handler);

}

int main() {
    test_event_send_test_id( "test_oi10_sys_005" );

    init_handlers();


    void * const test_addr = rumboot_malloc_from_named_heap( AXI_HEAP, 4 );
    uint32_t const test_axi_addr = rumboot_virt_to_dma( test_addr );

    rumboot_printf( "Checking access to existing devices in AXI\n" );
    uint32_t const event_start_check_cell1 [] = { TEC_START_CHECK_CELL1, test_axi_addr };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_start_check_cell1, ARRAY_SIZE(event_start_check_cell1) );
    iowrite32( TEST_VALUE, (uint32_t)test_addr );
    rumboot_printf( "Checking: OK\n" );

    rumboot_printf( "Checking write to non-existent devices in AXI\n" );
    PLB6PLB41_O_BR6TO4_handled = false;
    msync();
    uint32_t const event_start_check_hresp [] = { TEC_START_CHECK_HRESP, rumboot_virt_to_dma( (void*)NON_EXISTENT_AXI_ADDR ) };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_start_check_hresp, ARRAY_SIZE(event_start_check_hresp) );
    iowrite32( TEST_VALUE, NON_EXISTENT_AXI_ADDR );
    TEST_WAIT_ASSERT (PLB6PLB41_O_BR6TO4_handled, TEST_OI10_CPU_025_PPC_TIMEOUT, "Interrupt handle timeout!");
    rumboot_printf( "Checking: OK\n" );

    rumboot_printf( "Checking read to non-existent devices in AXI\n" );
    L2C0_MCHKOUT_handled = false;
    msync();
    uint32_t const event_start_check_hresp1 [] = { TEC_START_CHECK_HRESP, rumboot_virt_to_dma( (void*)NON_EXISTENT_AXI_ADDR ) };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_start_check_hresp1, ARRAY_SIZE(event_start_check_hresp1) );
    ioread32(NON_EXISTENT_AXI_ADDR);
    TEST_WAIT_ASSERT (L2C0_MCHKOUT_handled, TEST_OI10_CPU_025_PPC_TIMEOUT, "Interrupt handle timeout!");

    rumboot_printf( "Checking access to existing devices in AXI\n" );
    test_event( TEC_START_CHECK_CELL2 );
    uint32_t const event_start_check_cell2 [] = { TEC_START_CHECK_CELL2, test_axi_addr };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_start_check_cell2, ARRAY_SIZE(event_start_check_cell2) );
    ioread32( (uint32_t)test_addr );
    rumboot_printf( "Checking: OK\n" );

    return 0;
}

