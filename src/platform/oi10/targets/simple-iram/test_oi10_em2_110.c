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

#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/regs_mclfir.h>
#include <platform/interrupts.h>


#define NE_ADDR_SRAM0 0x1000000

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
uint32_t volatile  emi_irqstat_reg = 0x00;


static void trace_emi_irq_regs ()
{
    rumboot_printf("EMI_IRR = 0x%x\n", emi_irqstat_reg);
    rumboot_printf("EMI_RREQADR = 0x%x\n", dcr_read (DCR_EM2_EMI_BASE + EMI_RREQADR));
    rumboot_printf("EMI_WREQADR = 0x%x\n", dcr_read (DCR_EM2_EMI_BASE + EMI_WREQADR));
    rumboot_printf("EMI_WDADR = 0x%x\n",   dcr_read (DCR_EM2_EMI_BASE + EMI_WDADR));
    rumboot_printf("MCLFIR_ERR0 = 0x%x\n", dcr_read (DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR0));
    rumboot_printf("MCLFIR_ERR1 = 0x%x\n", dcr_read (DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR1));
}

void wait_irq ()
{
    uint32_t i = 0, timeout = 1000;
    while (IRQ_EXPECT && (i < timeout)) i++;
    //TEST_ASSERT (i < timeout, "ERRORL: wait_irq is timeout");
    if (i >= timeout)
    {
      trace_emi_irq_regs ();
      TEST_ASSERT (0, "ERRORL: wait_irq is timeout");
    }
}


//static void sram0_handler ()
//{
//    rumboot_printf("sram0_handler\n");
//    TEST_ASSERT (IRQ_EXPECT, "Not expected interrupt (SRAM0)");
//    IRQ_EXPECT = false;
//    emi_irqstat_reg = dcr_read (DCR_EM2_EMI_BASE + EMI_IRR);
//    trace_emi_irq_regs ();
//}

static void sram_handler ()
{
    rumboot_printf("sram_handler\n");
    TEST_ASSERT (IRQ_EXPECT, "Not expected interrupt (SRAM)");
    IRQ_EXPECT = false;
    emi_irqstat_reg = dcr_read (DCR_EM2_EMI_BASE + EMI_IRR);
    trace_emi_irq_regs ();
    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x00000);
}

static void emi_cntr_0_handler ()
{
    rumboot_printf("emi_cntr_0_handler\n");
    TEST_ASSERT (IRQ_EXPECT, "Not expected interrupt (EMI_CNTR_0)");
    IRQ_EXPECT = false;
    emi_irqstat_reg = dcr_read (DCR_EM2_EMI_BASE + EMI_IRR);
    trace_emi_irq_regs ();
    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x00000);
}

static void emi_cntr_1_handler ()
{
    rumboot_printf("emi_cntr_1_handler\n");
    TEST_ASSERT (IRQ_EXPECT, "Not expected interrupt (EMI_CNTR_1)");
    IRQ_EXPECT = false;
    emi_irqstat_reg = dcr_read (DCR_EM2_EMI_BASE + EMI_IRR);
    trace_emi_irq_regs ();
    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x00000);
}

static void emi_cntr_2_handler ()
{
    rumboot_printf("emi_cntr_2_handler\n");
    TEST_ASSERT (IRQ_EXPECT, "Not expected interrupt (EMI_CNTR_2)");
    IRQ_EXPECT = false;
    emi_irqstat_reg = dcr_read (DCR_EM2_EMI_BASE + EMI_IRR);
    trace_emi_irq_regs ();
    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x00000);
}

static void emi_cntr_3_handler ()
{
    rumboot_printf("emi_cntr_3_handler\n");
    TEST_ASSERT (IRQ_EXPECT, "Not expected interrupt (EMI_CNTR_3)");
    //IRQ_EXPECT = false;
    emi_irqstat_reg = dcr_read (DCR_EM2_EMI_BASE + EMI_IRR);
    trace_emi_irq_regs ();
    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x00000);
    rumboot_printf("DCR_EM2_EMI_BASE + EMI_IRR = 0x%x\n", dcr_read(DCR_EM2_EMI_BASE + EMI_IRR));
}



int main ()
{
    test_event_send_test_id("test_oi10_em2_110");

    emi_init(DCR_EM2_EMI_BASE);
    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR, 0x00000); //Clear all EMI interrupts
    dcr_write(DCR_EM2_EMI_BASE + EMI_IMR, 0x1FFFF); //Unmask all EMI interrupts


    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
//    rumboot_irq_set_handler( tbl, L2C0_MCHKOUT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, sram0_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, SRAM_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, sram_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, emi_cntr_0_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, emi_cntr_1_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_2, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, emi_cntr_2_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_3, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, emi_cntr_3_handler, ( void* )0 );
    /* Activate the table */
    rumboot_irq_table_activate( tbl );
//    rumboot_irq_enable( L2C0_MCHKOUT );
    rumboot_irq_enable( SRAM_INT );
    rumboot_irq_enable( EMI_CNTR_INT_0 );
    rumboot_irq_enable( EMI_CNTR_INT_1 );
    rumboot_irq_enable( EMI_CNTR_INT_2 );
    rumboot_irq_enable( EMI_CNTR_INT_3 );
    rumboot_irq_sei();


//*******************************************************************************************************
    rumboot_printf ("10.1 Check write/read in non-existent address\n");
    uint32_t ne_addr = NE_ADDR_SRAM0,
             test_data = 0xBABADEDA;
    uint32_t read_data;

    iowrite32 (0x00, SRAM0_BASE);
    iowrite32 (test_data, ne_addr);
    TEST_ASSERT (ioread32(SRAM0_BASE)    == test_data, "Not expected EMI behavior");
    TEST_ASSERT (ioread32(NE_ADDR_SRAM0) == test_data, "Not expected EMI behavior");
    rumboot_printf ("Done! Behavior of the EMI is correct\n");
//*******************************************************************************************************


//*******************************************************************************************************
    rumboot_printf ("10.2 Error in M_ADR_RS_PTY\n");

    //iowrite32(0xFFFFFFFF, SRAM0_BASE + 0xC); //Init memory

    rumboot_printf("Inject err in ADR_RS_PTY\n");
    test_event(EVENT_ERR_ADR_RS_PTY);

    rumboot_printf("Write transaction\n");
    IRQ_EXPECT = true;
    iowrite32(test_data, SRAM0_BASE + 0xC);
    msync();
    wait_irq();

    rumboot_printf("Read transaction\n");
    IRQ_EXPECT = true;
    read_data = ioread32(SRAM0_BASE + 0xC);
    rumboot_printf("read data = %x\n", read_data);
    TEST_ASSERT(read_data != test_data, "Unexpected data has been read from SRAM0.");

    msync();
    wait_irq();

    rumboot_printf("Release ADR_RS_PTY\n");
    test_event(EVENT_REL_ADR_RS_PTY);
//*******************************************************************************************************


//*******************************************************************************************************
    rumboot_printf ("10.3 Error in M_BE_PTY\n");

    rumboot_printf("Inject err in BE_PTY\n");
    test_event(EVENT_ERR_BE_PTY);

    rumboot_printf("Write transaction\n");
    IRQ_EXPECT = true;
    iowrite32(test_data, SRAM0_BASE + 0xC);
    msync();
    wait_irq();

    rumboot_printf("Read transaction\n");
    IRQ_EXPECT = true;
    read_data = ioread32(SRAM0_BASE + 0xC);
    rumboot_printf("read data = %x\n", read_data);
    TEST_ASSERT(read_data != test_data, "Unexpected data has been read from SRAM0.");
    msync();
    wait_irq();

    rumboot_printf("Release BE_PTY\n");
    test_event(EVENT_REL_BE_PTY);
//*******************************************************************************************************


//*******************************************************************************************************
    rumboot_printf ("10.4 Error in M_CMD_PTY\n");

    rumboot_printf("Inject err in CMD_PTY\n");
    test_event(EVENT_ERR_CMD_PTY);

    rumboot_printf("Write transaction\n");
    IRQ_EXPECT = true;
    iowrite32(test_data, SRAM0_BASE + 0xC);
    msync();
    wait_irq();

    rumboot_printf("Read transaction\n");
    IRQ_EXPECT = true;
    read_data = ioread32(SRAM0_BASE + 0xC);
    rumboot_printf("read data = %x\n", read_data);
    TEST_ASSERT(read_data != test_data, "Unexpected data has been read from SRAM0.");
    msync();
    wait_irq();

    rumboot_printf("Release CMD_PTY\n");
    test_event(EVENT_REL_CMD_PTY);
//*******************************************************************************************************


//*******************************************************************************************************
    rumboot_printf ("10.5 Error in M_COMP_CMD_PTY\n");

    rumboot_printf("Inject err in COMP_CMD_PTY\n");
    test_event(EVENT_ERR_COMP_CMD_PTY);

    rumboot_printf("Write transaction\n");
    IRQ_EXPECT = true;
    iowrite32(test_data, SRAM0_BASE + 0xC);
    msync();
    wait_irq();

    rumboot_printf("Read transaction\n");
    IRQ_EXPECT = true;
    read_data = ioread32(SRAM0_BASE + 0xC);
    rumboot_printf("read data = %x\n", read_data);
    TEST_ASSERT(read_data != test_data, "Unexpected data has been read from SRAM0.");
    msync();
    wait_irq();

    rumboot_printf("Release COMP_CMD_PTY\n");
    test_event(EVENT_REL_COMP_CMD_PTY);
//*******************************************************************************************************


//*******************************************************************************************************
    rumboot_printf ("10.6 Error in M_WR_DATA_PAR\n");

    rumboot_printf("Inject err in WR_DATA_PAR\n");
    test_event(EVENT_ERR_WR_DATA_PAR);

    rumboot_printf("Write transaction\n");
    IRQ_EXPECT = true;
    iowrite32(test_data, SRAM0_BASE + 0xC);
    msync();
    wait_irq();

    rumboot_printf("Release WR_DATA_PAR\n");
    test_event(EVENT_REL_WR_DATA_PAR);
//*******************************************************************************************************


//*******************************************************************************************************
    rumboot_printf ("10.7 Error in M_RD_DATA_PAR\n");

    rumboot_printf("Inject err in RD_DATA_PAR\n");
    test_event(EVENT_ERR_RD_DATA_PAR);

    rumboot_printf("Read transaction\n");
    IRQ_EXPECT = true;
    read_data = ioread32(SRAM0_BASE + 0xC);
    rumboot_printf("read data = %x\n", read_data);
    TEST_ASSERT(read_data != test_data, "Unexpected data has been read from SRAM0.");
    msync();
    wait_irq();

    rumboot_printf("Release RD_DATA_PAR\n");
    test_event(EVENT_REL_RD_DATA_PAR);
//*******************************************************************************************************


    rumboot_printf ("TEST OK");
    return 0;
}
