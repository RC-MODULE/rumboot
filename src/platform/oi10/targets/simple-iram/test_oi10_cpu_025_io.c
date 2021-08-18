/*
 * test_oi10_cpu_025_io.c
 *
 *  Created on: Feb 19, 2019
 *      Author: m.smolina
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>

#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/interrupts.h>

#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <platform/arch/ppc/test_macro.h>
#include <arch/mem_window.h>

#include <devices/ugly/emi.h>
#include <platform/devices/l2c.h>
#include <arch/dma2plb6.h>

#include <arch/regs/dma2plb6.h>
#include <platform/regs/regs_plb4plb6.h>
#include <platform/regs/regs_plb4ahb.h>
#include <arch/regs/fields/plb6bc.h>
#include <platform/regs/regs_dcrarb.h>
#include <arch/regs/regs_p64.h>
#include <regs/fields/mpic128.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c.h>
#include <platform/regs/regs_l2c_pmu.h>


#define EVENT_GENERATE_INT_PLB4XAHB1_INTR            (TEST_EVENT_CODE_MIN + 0)
#define EVENT_CLEAR_INT_PLB4XAHB1_INTR               (TEST_EVENT_CODE_MIN + 1)

#define EVENT_GENERATE_INT_P6BC_O_SYSTEM_HUNG        (TEST_EVENT_CODE_MIN + 2)
#define EVENT_CLEAR_INT_P6BC_O_SYSTEM_HUNG           (TEST_EVENT_CODE_MIN + 3)

#define EVENT_GENERATE_INT_P6P4_0_BR6TO4_INTR        (TEST_EVENT_CODE_MIN + 4)
#define EVENT_CLEAR_INT_P6P4_0_BR6TO4_INTR           (TEST_EVENT_CODE_MIN + 5)

#define EVENT_GENERATE_INT_P6P4_1_BR6TO4_INTR        (TEST_EVENT_CODE_MIN + 6)
#define EVENT_CLEAR_INT_P6P4_1_BR6TO4_INTR           (TEST_EVENT_CODE_MIN + 7)

#define EVENT_GENERATE_INT_ARB_SYSDCRERR             (TEST_EVENT_CODE_MIN + 8)
#define EVENT_CLEAR_INT_ARB_SYSDCRERR                (TEST_EVENT_CODE_MIN + 9)

#define TEST_OI10_CPU_025_IO_TIMEOUT    400

#define UNEXIST_DCR_ADDR                0x00

#define NON_EXISTENT_AXI_ADDR   0xC0025000
#define TEST_VALUE              0xBABADEDA

struct rumboot_irq_entry *tbl;
volatile uint32_t MC_HANDLED = 0;

static void exception_handler(int const id, char const * const name ) {
    rumboot_printf( "Exception: %s\n", name );

    rumboot_printf("MSR:  0x%x\n", msr_read());
    rumboot_printf("SRR0: 0x%x\n", spr_read(SPR_SRR0));
    rumboot_printf("SRR1: 0x%x\n", spr_read(SPR_SRR1));
    rumboot_printf("CSRR0: 0x%x\n", spr_read(SPR_CSRR0));
    rumboot_printf("CSRR1: 0x%x\n", spr_read(SPR_CSRR1));
    rumboot_printf("MCSRR0: 0x%x\n", spr_read(SPR_MCSRR0));
    rumboot_printf("MCSRR1: 0x%x\n", spr_read(SPR_MCSRR1));
    rumboot_printf("MCSR:  0x%x\n", spr_read(SPR_MCSR_RW));

    TEST_ASSERT( (id == RUMBOOT_IRQ_MACHINE_CHECK)
              && (spr_read(SPR_MCSR_RW) == ( (0b1 << ITRPT_MCSR_MCS_i) )),
                 "Unexpected exception" );
    spr_write( SPR_MCSR_C, spr_read(SPR_MCSR_RW) );

    isync();
    msync();
}

void generate_BR6TO4_INTR(uint32_t base_address)
{
    rumboot_printf("Check BR6TO4_INTR generation\n");
    dcr_write(base_address + P64_TESR, 0b1 << ESR_TESR_P6WPE_i);
}

void generate_PLB4AHB_INTR()
{
    rumboot_printf("Check PLB4AHB_INTR generation\n");
    iowrite32( TEST_VALUE, NON_EXISTENT_AXI_ADDR );
}

void generate_ARB_SYSDCRERR()
{
    dcr_write(DCR_ARB_BASE + DCRARB_DACR, (0x1 << DCRARB_DACR_TOCNT_i));
    dcr_read(UNEXIST_DCR_ADDR);
}

void wait_interrupt(uint32_t interrupt_number)
{
    uint32_t t = 0;
    while (!(MC_HANDLED & (1 << interrupt_number)) && t++<TEST_OI10_CPU_025_IO_TIMEOUT) msync();
    TEST_ASSERT((MC_HANDLED & (1 << interrupt_number)), "Interrupt handle timeout!");
}

void P6P4_0_BR6TO4_INTR_inj_int_handler()
{
    uint32_t p64_esr;
    rumboot_printf("P6P4_0_BR6TO4_INTR_inj_int_handler\n");
    SET_BIT(MC_HANDLED, PLB6PLB40_O_0_BR6TO4_INTR);
    rumboot_printf("P64_0.ESR=0x%X\n",
            p64_esr = dcr_read(DCR_PLB6PLB4_0_BASE + P64_ESR));
    TEST_ASSERT(p64_esr & (1 << ESR_TESR_P4MWE_i),
            "Wrong value in P6P4_0 ESR reg. Expected ESR[P4MWE]=1\n");
    test_event(EVENT_CLEAR_INT_P6P4_0_BR6TO4_INTR);
    dcr_write(DCR_PLB6PLB4_0_BASE + P64_ESR, p64_esr);
}

void P6P4_1_BR6TO4_INTR_inj_int_handler()
{
    uint32_t p64_esr;
    rumboot_printf("P6P4_1_BR6TO4_INTR_inj_int_handler\n");
    SET_BIT(MC_HANDLED, PLB6PLB41_O_BR6TO4_INTR);
    rumboot_printf("P64_1.ESR=0x%X\n",
            p64_esr = dcr_read(DCR_PLB6PLB4_1_BASE + P64_ESR));
    TEST_ASSERT(p64_esr & (1 << ESR_TESR_P4MWE_i),
            "Wrong value in P6P4_1 ESR reg. Expected ESR[P4MWE]=1\n");
    test_event(EVENT_CLEAR_INT_P6P4_1_BR6TO4_INTR);
    dcr_write(DCR_PLB6PLB4_1_BASE + P64_ESR, p64_esr);
}

void ARB_SYSDCRERR_inj_int_handler()
{
    uint32_t daesr_reg;
    rumboot_printf("ARB_SYSDCRERR_inj_int_handler\n");
    SET_BIT(MC_HANDLED, ARB_SYSDCRERR);
    daesr_reg = dcr_read(DCR_ARB_BASE + DCRARB_DAESR);
    TEST_ASSERT((daesr_reg & (0b1 << DCRARB_DAESR_TE_i)) && (daesr_reg & (0b1 << DCRARB_DAESR_EV_i)), "Failed to set DCRARB DAESR[TE] and DAESR[EV] bit");
    test_event(EVENT_CLEAR_INT_ARB_SYSDCRERR);
    dcr_write(DCR_ARB_BASE + DCRARB_DAESR, 0b1<<DCRARB_DAESR_EV_i); //dcrarb_clear_daesr
}

void PLB4AHB_INTR_inj_int_handler()
{
    rumboot_printf("PLB4AHB_INTR_inj_int_handler\n");
    SET_BIT(MC_HANDLED, PLB4XAHB1_INTR);
    test_event(EVENT_CLEAR_INT_PLB4XAHB1_INTR);
//    dcr_write( DCR_PLB4AHB_0_BASE + SEAR_ADDRUPPER, 0xF );
//    dcr_write( DCR_PLB4AHB_0_BASE + SEAR_ADDRLOWER, 0xF );
//    dcr_write( DCR_PLB4AHB_0_BASE + SESR, 0xF );
}

void P6P4_0_BR6TO4_INTR_reg_int_handler()
{
    rumboot_printf("P6P4_0_BR6TO4_INTR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, PLB6PLB40_O_0_BR6TO4_INTR);
    TEST_ASSERT(dcr_read(DCR_PLB6PLB4_0_BASE + P64_ESR) & (0b1 << ESR_TESR_P6WPE_i), "Wrong value in P6P4_0 ESR reg. Expected ESR[P6BPE]=1\n");
    dcr_write(DCR_PLB6PLB4_0_BASE + P64_ESR, (0b1 << ESR_TESR_P6WPE_i));
}

void P6P4_1_BR6TO4_INTR_reg_int_handler()
{
    rumboot_printf("P6P4_1_BR6TO4_INTR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, PLB6PLB41_O_BR6TO4_INTR);
    TEST_ASSERT(dcr_read(DCR_PLB6PLB4_1_BASE + P64_ESR) & (0b1 << ESR_TESR_P6WPE_i), "Wrong value in P6P4_1 ESR reg. Expected ESR[P6BPE]=1\n");
    dcr_write(DCR_PLB6PLB4_1_BASE + P64_ESR, (0b1 << ESR_TESR_P6WPE_i));
}

void PLB4AHB_INTR_reg_int_handler()
{
    rumboot_printf("PLB4AHB_INTR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, PLB4XAHB1_INTR);
    dcr_write( DCR_PLB4AHB_0_BASE + SEAR_ADDRUPPER, 0xF );
    dcr_write( DCR_PLB4AHB_0_BASE + SEAR_ADDRLOWER, 0xF );
    dcr_write( DCR_PLB4AHB_0_BASE + SESR, 0xF );
}

void ARB_SYSDCRERR_reg_int_handler()
{
    uint32_t daesr_reg;
    rumboot_printf("ARB_SYSDCRERR_reg_int_handler\n");
    SET_BIT(MC_HANDLED, ARB_SYSDCRERR);
    daesr_reg = dcr_read(DCR_ARB_BASE + DCRARB_DAESR);
    TEST_ASSERT((daesr_reg & (0b1 << DCRARB_DAESR_TE_i)) && (daesr_reg & (0b1 << DCRARB_DAESR_EV_i)), "Failed to set DCRARB DAESR[TE] and DAESR[EV] bit");
    dcr_write(DCR_ARB_BASE + DCRARB_DAESR, 0b1 << DCRARB_DAESR_EV_i); //dcrarb_clear_daesr
}

void test_setup( uint32_t interrupt_number )
{

    switch (interrupt_number)
    {
    case PLB6PLB40_O_0_BR6TO4_INTR:
                        rumboot_printf("Set handler for PLB6PLB40_O_0_BR6TO4_INTR \n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB40_O_0_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, P6P4_0_BR6TO4_INTR_reg_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB40_O_0_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB6PLB41_O_BR6TO4_INTR:
                        rumboot_printf("Set handler for PLB6PLB41_O_BR6TO4_INTR\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB41_O_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, P6P4_1_BR6TO4_INTR_reg_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB41_O_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case ARB_SYSDCRERR:
                        rumboot_printf("Set handler for ARB_SYSDCRERR\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, ARB_SYSDCRERR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, ARB_SYSDCRERR_reg_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( ARB_SYSDCRERR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB4XAHB1_INTR:
                        rumboot_printf("Set handler for PLB4XAHB1_INTR\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB41_O_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, PLB4AHB_INTR_reg_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB41_O_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    }
}

#ifdef USE_HARDWARE_PARTS
void test_setup_inj( uint32_t interrupt_number )
{

    switch (interrupt_number)
    {
    case PLB6PLB40_O_0_BR6TO4_INTR:
                        rumboot_printf("Set handler for PLB6PLB40_O_0_BR6TO4_INTR \n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB40_O_0_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, P6P4_0_BR6TO4_INTR_inj_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB40_O_0_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB6PLB41_O_BR6TO4_INTR:
                        rumboot_printf("Set handler for PLB6PLB41_O_BR6TO4_INTR\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB41_O_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, P6P4_1_BR6TO4_INTR_inj_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB41_O_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case ARB_SYSDCRERR:
                        rumboot_printf("Set handler for ARB_SYSDCRERR\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, ARB_SYSDCRERR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, ARB_SYSDCRERR_inj_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( ARB_SYSDCRERR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    case PLB4XAHB1_INTR:
                        rumboot_printf("Set handler for PLB4XAHB1_INTR\n");
                        rumboot_irq_cli();
                        tbl = rumboot_irq_create( NULL );
                        rumboot_irq_set_handler( tbl, PLB6PLB41_O_BR6TO4_INTR, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, PLB4AHB_INTR_inj_int_handler, NULL );
                        rumboot_irq_table_activate( tbl );
                        rumboot_irq_enable( PLB6PLB41_O_BR6TO4_INTR );
                        rumboot_irq_sei();

                        MC_HANDLED = 0;
    break;
    }
}
#endif

void check_interrupt_from_io_dev_reg(uint32_t interrupt_number)
{
    test_setup( interrupt_number );

    switch (interrupt_number)
    {
        case PLB6PLB40_O_0_BR6TO4_INTR:
                                            rumboot_printf("Check software generation P6P4_0_BR6TO4_INTR\n");
                                            generate_BR6TO4_INTR(DCR_PLB6PLB4_0_BASE);
                                            break;
        case PLB6PLB41_O_BR6TO4_INTR:
                                            rumboot_printf("Check software generation P6P4_1_BR6TO4_INTR\n");
                                            generate_BR6TO4_INTR(DCR_PLB6PLB4_1_BASE);
                                            break;
        case ARB_SYSDCRERR:
                                            rumboot_printf("Check ARB_SYSDCRERR generation\n");
                                            generate_ARB_SYSDCRERR();
                                            break;
        case PLB4XAHB1_INTR:
                                            rumboot_printf("Check PLB4XAHB1_INTR generation\n");
                                            generate_PLB4AHB_INTR();
                                            break;

        default: rumboot_printf("Get wrong interrupt id for injection\n");
                 test_event(EVENT_ERROR);
                 break;
    }

    wait_interrupt(interrupt_number);
}

#ifdef USE_HARDWARE_PARTS
void check_interrupt_from_io_dev_inj(uint32_t interrupt_number)
{
    test_setup_inj( interrupt_number );

    switch (interrupt_number)
    {
        case PLB4XAHB1_INTR:
                                            test_event(EVENT_GENERATE_INT_PLB4XAHB1_INTR);
                                            break;

        case PLB6PLB40_O_0_BR6TO4_INTR:
                                            test_event(EVENT_GENERATE_INT_P6P4_0_BR6TO4_INTR);
                                            break;

        case PLB6PLB41_O_BR6TO4_INTR:
                                            test_event(EVENT_GENERATE_INT_P6P4_1_BR6TO4_INTR);
                                            break;


        case ARB_SYSDCRERR:
                                            dcr_write(DCR_ARB_BASE + DCRARB_DACR, (0b1 << DCRARB_DACR_TOCNT_i));
                                            test_event(EVENT_GENERATE_INT_ARB_SYSDCRERR);
                                            break;

        default: rumboot_printf("Get wrong interrupt id for injection\n");
                 test_event(EVENT_ERROR);
                 break;
    }

    wait_interrupt(interrupt_number);
}
#endif

int main()
{
#ifdef USE_HARDWARE_PARTS
    test_event_send_test_id( "test_oi10_cpu_025_io" );
#endif

    rumboot_irq_set_exception_handler(exception_handler);

    MC_HANDLED = 0;

    rumboot_printf("P64_0.ESR=0x%X\n",
            dcr_read(DCR_PLB6PLB4_0_BASE + P64_ESR));
    rumboot_printf("P64_1.ESR=0x%X\n",
            dcr_read(DCR_PLB6PLB4_1_BASE + P64_ESR));




#ifdef USE_HARDWARE_PARTS
    check_interrupt_from_io_dev_inj(PLB4XAHB1_INTR);
    check_interrupt_from_io_dev_inj(PLB6PLB40_O_0_BR6TO4_INTR);
    check_interrupt_from_io_dev_inj(PLB6PLB41_O_BR6TO4_INTR);
    check_interrupt_from_io_dev_inj(ARB_SYSDCRERR);
#endif

    check_interrupt_from_io_dev_reg(PLB4XAHB1_INTR);
    check_interrupt_from_io_dev_reg(PLB6PLB40_O_0_BR6TO4_INTR);
    check_interrupt_from_io_dev_reg(PLB6PLB40_O_0_BR6TO4_INTR);
    check_interrupt_from_io_dev_reg(PLB6PLB41_O_BR6TO4_INTR);
    check_interrupt_from_io_dev_reg(ARB_SYSDCRERR);

    return 0;
}

