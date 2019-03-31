/*
 * test_oi10_em2_110.c
 *
 *  Created on: Aug 1, 2018
 *      Author: s.chernousov
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/macros.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>

#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/regs_mclfir.h>
#include <platform/interrupts.h>
#include <platform/devices/l2c.h>

#define NE_ADDR_SRAM0 (SRAM0_BASE + SRAM0_SIZE)

uint32_t const EVENT_ERR_ADR_RS_PTY       = TEST_EVENT_CODE_MIN + 0;
uint32_t const EVENT_REL_ADR_RS_PTY       = TEST_EVENT_CODE_MIN + 1;
uint32_t const EVENT_ERR_BE_PTY           = TEST_EVENT_CODE_MIN + 2;
uint32_t const EVENT_REL_BE_PTY           = TEST_EVENT_CODE_MIN + 3;
uint32_t const EVENT_ERR_CMD_PTY          = TEST_EVENT_CODE_MIN + 4;
uint32_t const EVENT_REL_CMD_PTY          = TEST_EVENT_CODE_MIN + 5;
uint32_t const EVENT_ERR_COMP_CMD_PTY     = TEST_EVENT_CODE_MIN + 6;
uint32_t const EVENT_REL_COMP_CMD_PTY     = TEST_EVENT_CODE_MIN + 7;
uint32_t const EVENT_ERR_WR_DATA_PAR      = TEST_EVENT_CODE_MIN + 8;
uint32_t const EVENT_REL_WR_DATA_PAR      = TEST_EVENT_CODE_MIN + 9;
uint32_t const EVENT_ERR_RD_DATA_PAR      = TEST_EVENT_CODE_MIN + 10;
uint32_t const EVENT_REL_RD_DATA_PAR      = TEST_EVENT_CODE_MIN + 11;

bool     volatile  IRQ_EXPECT = false;
uint32_t volatile  rel_event_code = 0x00;

static void irq_expect ()
{
    IRQ_EXPECT = true;
    msync();
}

static void trace_emi_irq_regs ()
{
    /*
    rumboot_printf("trace:\n");
    rumboot_printf("EMI_IRR = %x\n", dcr_read (DCR_EM2_EMI_BASE + EMI_IRR));
    rumboot_printf("EMI_RREQADR = %x\n", dcr_read (DCR_EM2_EMI_BASE + EMI_RREQADR));
    rumboot_printf("EMI_WREQADR = %x\n", dcr_read (DCR_EM2_EMI_BASE + EMI_WREQADR));
    rumboot_printf("EMI_WDADR = %x\n",   dcr_read (DCR_EM2_EMI_BASE + EMI_WDADR));
    rumboot_printf("MCLFIR_ERR0 = %x\n", dcr_read (DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR0));
    rumboot_printf("MCLFIR_ERR1 = %x\n", dcr_read (DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR1));
    */
}

void release_signal ()
{
    rumboot_printf("Release\n");
    test_event(rel_event_code);
}

void emi_irq_unmask ()
{
    dcr_write(DCR_EM2_EMI_BASE + EMI_IMR, 0x1FFFF);
}

void emi_irq_clear ()
{
    rumboot_printf("irq_clear\n");
    release_signal();
    iowrite32(0x00, SRAM0_BASE+0x4);
    msync();
    TEST_ASSERT (ioread32(SRAM0_BASE+0x4) == 0,"TEST_ERROR: write_data != read_data (emi_irq_clear)");
    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR, 0x00);
    TEST_ASSERT(dcr_read (DCR_EM2_EMI_BASE + EMI_IRR) == 0x00, "TEST_ERROR: EMI_IRR != 0, irq not clear ");
}

void wait_irq ()
{
    uint32_t i = 0, timeout = 100;
    while (IRQ_EXPECT && (i < timeout)) i++;
    if (i >= timeout)
      TEST_ASSERT (0, "ERROR: wait_irq is timeout");
}

static void emi_cntr_0_handler ()
{
    rumboot_printf("emi_cntr_0_handler\n");
    TEST_ASSERT (IRQ_EXPECT, "Not expected interrupt (EMI_CNTR_0)");
    IRQ_EXPECT = false;
    trace_emi_irq_regs ();
    emi_irq_clear ();
}

static void emi_cntr_1_handler ()
{
    rumboot_printf("emi_cntr_1_handler\n");
    TEST_ASSERT (IRQ_EXPECT, "Not expected interrupt (EMI_CNTR_1)");
    IRQ_EXPECT = false;
    trace_emi_irq_regs ();
    emi_irq_clear ();
}

static void emi_cntr_2_handler ()
{
    rumboot_printf("emi_cntr_2_handler\n");
    TEST_ASSERT (IRQ_EXPECT, "Not expected interrupt (EMI_CNTR_2)");
    IRQ_EXPECT = false;
    trace_emi_irq_regs ();
    emi_irq_clear ();
}

static void emi_cntr_3_handler ()
{
    rumboot_printf("emi_cntr_3_handler\n");
    TEST_ASSERT (IRQ_EXPECT, "Not expected interrupt (EMI_CNTR_3)");
    IRQ_EXPECT = false;
    trace_emi_irq_regs ();
    emi_irq_clear ();
}

static void exception_handler(int id, const char *name)
{
    rumboot_printf("Not worry, is expected MCHKOUT\n");

    spr_write(SPR_MCSR_C, ~0);
    l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBSTAT1, ~0);
}

int main ()
{
    test_event_send_test_id("test_oi10_em2_110");

    emi_init(DCR_EM2_EMI_BASE);
    emi_irq_unmask ();

    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, emi_cntr_0_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, emi_cntr_1_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_2, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, emi_cntr_2_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_3, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, emi_cntr_3_handler, ( void* )0 );
    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( EMI_CNTR_INT_0 );
    rumboot_irq_enable( EMI_CNTR_INT_1 );
    rumboot_irq_enable( EMI_CNTR_INT_2 );
    rumboot_irq_enable( EMI_CNTR_INT_3 );
    rumboot_irq_sei();

    rumboot_irq_set_exception_handler(exception_handler);
 /*   uint32_t const msr_old_value = msr_read();

    msr_write( msr_old_value & ~(

                               (0b1 << ITRPT_XSR_ME_i)
                               ));
*/
    msync();
//*******************************************************************************************************
    rumboot_printf ("10.1 Check write/read in non-existent address\n");
    uint32_t ne_addr = NE_ADDR_SRAM0,
             test_data = 0xBABADEDA;

    iowrite32 (0x00, SRAM0_BASE);
    iowrite32 (test_data, ne_addr);
    msync();

    trace_emi_irq_regs ();
    TEST_ASSERT (ioread32(SRAM0_BASE)    == test_data, "Not expected EMI behavior");
    TEST_ASSERT (ioread32(NE_ADDR_SRAM0) == test_data, "Not expected EMI behavior");
    rumboot_printf ("Done! Behavior of the EMI is correct\n");

//*******************************************************************************************************

//*******************************************************************************************************

    rumboot_printf ("10.2 Error in M_ADR_RS_PTY\n");
    iowrite32(0x00, SRAM0_BASE + 0xC); //Init memory
    rel_event_code = EVENT_REL_ADR_RS_PTY;

    rumboot_printf("Inject err in ADR_RS_PTY\n");
    test_event(EVENT_ERR_ADR_RS_PTY);

    rumboot_printf("Write transaction\n");
    irq_expect();
    iowrite32(test_data, SRAM0_BASE + 0xC);
    wait_irq();

    rumboot_printf("Inject err in ADR_RS_PTY\n");
    test_event(EVENT_ERR_ADR_RS_PTY);

    rumboot_printf("Read transaction\n");
    irq_expect();
    rumboot_printf("read data = %x\n", ioread32(SRAM0_BASE + 0xC));
    wait_irq();

//*******************************************************************************************************

//*******************************************************************************************************
    rumboot_printf ("10.3 Error in M_BE_PTY\n");
    iowrite32(0x00, SRAM0_BASE + 0xC); //Init memory
    rel_event_code = EVENT_REL_BE_PTY;

    rumboot_printf("Inject err in BE_PTY\n");
    test_event(EVENT_ERR_BE_PTY);

    rumboot_printf("Write transaction\n");
    irq_expect();
    iowrite32(test_data, SRAM0_BASE + 0xC);
    wait_irq();

    rumboot_printf("Inject err in BE_PTY\n");
    test_event(EVENT_ERR_BE_PTY);

    rumboot_printf("Read transaction\n");
    irq_expect();
    rumboot_printf("read data = %x\n", ioread32(SRAM0_BASE + 0xC));
    wait_irq();

//*******************************************************************************************************

//*******************************************************************************************************

    rumboot_printf ("10.4 Error in M_CMD_PTY\n");
    iowrite32(0x00, SRAM0_BASE + 0xC); //Init memory
    rel_event_code = EVENT_REL_CMD_PTY;

    rumboot_printf("Inject err in CMD_PTY\n");
    test_event(EVENT_ERR_CMD_PTY);

    rumboot_printf("Write transaction\n");
    irq_expect();
    iowrite32(test_data, SRAM0_BASE + 0xC);
    wait_irq();

    rumboot_printf("Inject err in CMD_PTY\n");
    test_event(EVENT_ERR_CMD_PTY);

    rumboot_printf("Read transaction\n");
    irq_expect();
    rumboot_printf("read data = %x\n", ioread32(SRAM0_BASE + 0xC));
    wait_irq();

//*******************************************************************************************************

//*******************************************************************************************************
    rumboot_printf ("10.5 Error in M_COMP_CMD_PTY\n");
    iowrite32(0x00, SRAM0_BASE + 0xC); //Init memory
    rel_event_code = EVENT_REL_COMP_CMD_PTY;

    rumboot_printf("Inject err in COMP_CMD_PTY\n");
    test_event(EVENT_ERR_COMP_CMD_PTY);

    rumboot_printf("Write transaction\n");
    iowrite32(test_data, SRAM0_BASE + 0xC);
    msync();

    rumboot_printf("Inject err in COMP_CMD_PTY\n");
    test_event(EVENT_ERR_COMP_CMD_PTY);

    rumboot_printf("Read transaction\n");
    rumboot_printf("read data = %x\n", ioread32(SRAM0_BASE + 0xC));
    msync();

//*******************************************************************************************************

//*******************************************************************************************************
    rumboot_printf ("10.6 Error in M_WR_DATA_PAR\n");
    iowrite32(0x00, SRAM0_BASE + 0xC); //Init memory
    rel_event_code = EVENT_REL_WR_DATA_PAR;

    rumboot_printf("Inject err in WR_DATA_PAR\n");
    test_event(EVENT_ERR_WR_DATA_PAR);

    rumboot_printf("Write transaction\n");
    irq_expect();
    iowrite32(test_data, SRAM0_BASE + 0xC);
    wait_irq();
//*******************************************************************************************************

//*******************************************************************************************************
    rumboot_printf ("10.7 Error in M_RD_DATA_PAR\n");
    iowrite32(test_data, SRAM0_BASE + 0xC); //Init memory
    rel_event_code = EVENT_REL_RD_DATA_PAR;

    rumboot_printf("Inject err in RD_DATA_PAR\n");
    test_event(EVENT_ERR_RD_DATA_PAR);

    rumboot_printf("Read transaction\n");
    rumboot_printf("read data = %x\n", ioread32(SRAM0_BASE + 0xC));
    msync();
//*******************************************************************************************************

    return 0;
}
