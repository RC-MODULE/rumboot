/**
 * Implemented test which described in 2.3 PPC_SRAM_SDRAM_slave0_testplan.docx
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/test_assert.h>
#include <platform/devices/emi.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>
#include <platform/regs/fields/mpic128.h>


#define TEST_BANK_IDX    emi_b0_sram0
#define TEST_BANK_BASE   SRAM0_BASE
#define TEST_LEN         3
#define TEST_TRDY        (0x400 - 0x258)

#define TEST_EVENT_MAKE_PULSE_ON_RDY_An     0x0000003E

static uint32_t volatile EMI_SRAM0_IRQ_HANDLED = 0;
#define TEST_EMI_IRQ_WR     1
#define TEST_EMI_IRQ_RD     2

uint32_t test_addr[TEST_LEN] = {
                                    TEST_BANK_BASE,
                                    TEST_BANK_BASE + 0x100,
                                    TEST_BANK_BASE + 0x1000,
                                };

uint32_t test_data[TEST_LEN] = {
                                    0xFFFFFFFF,
                                    0x55555555,
                                    0x00000000
                                };

static void handler_emi( int irq, void *arg )
{
    emi_irr_cfg irr;
    rumboot_printf( "EMI IRQ arrived\n");
    emi_get_irr(DCR_EM2_EMI_BASE, &irr);
    if (irr.IRDYW==true)
    {
        EMI_SRAM0_IRQ_HANDLED = TEST_EMI_IRQ_WR;
        rumboot_printf( "TRDY interrupt request at writing\n");
    }
    else if (irr.IRDYR==true)
    {
        EMI_SRAM0_IRQ_HANDLED = TEST_EMI_IRQ_RD;
        rumboot_printf( "TRDY interrupt request at reading\n");
    }
    emi_clear_irr(DCR_EM2_EMI_BASE, &irr);
    rumboot_printf("Exit from handler\n");
}

struct rumboot_irq_entry * create_sram_irq_handler()
{
    rumboot_printf( "Enable SRAM irq\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    EMI_SRAM0_IRQ_HANDLED = 0;

    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_2, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_emi, NULL );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( EMI_CNTR_INT_2 );
    rumboot_irq_sei();

    return tbl;
}

void delete_sram_irq_handler(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

void check_wr_ext_rdy()
{
    for (int i = 0; i<TEST_LEN; i++)
    {
        test_event(TEST_EVENT_MAKE_PULSE_ON_RDY_An);
        iowrite32(test_data[i], test_addr[i]);
    }
}

void check_rd_ext_rdy()
{
    uint32_t rd;
    for (int i = 0; i<TEST_LEN; i++)
    {
        test_event(TEST_EVENT_MAKE_PULSE_ON_RDY_An);
        rd = ioread32(test_addr[i]);
        if (rd!=test_data[i])
        {
            rumboot_printf("Data error!\nExpected: 0x%X\nActual: 0x%X\n", test_data[i], rd);
            TEST_ASSERT(0, "test_oi10_em2_203 is failed!\n");
        }
    }
}

void wait_sram_irq(uint32_t exp_status)
{
    #define EMI_IRQ_TIMEOUT 100
    uint32_t timeout_cnt = 0;
    while ( (EMI_SRAM0_IRQ_HANDLED!=exp_status) && (timeout_cnt++ < EMI_IRQ_TIMEOUT) );
    TEST_ASSERT(EMI_SRAM0_IRQ_HANDLED==exp_status, "IRQ is timed out!");
}

void check_irq_by_trdy_wr()
{
    iowrite32(test_data[0], test_addr[0]);
    rumboot_printf("Waiting TRDY IRQ at writing\n");
    wait_sram_irq(TEST_EMI_IRQ_WR);
}

void check_irq_by_trdy_rd()
{
    uint32_t rd;
    rd = ioread32(test_addr[0]);
    wait_sram_irq(TEST_EMI_IRQ_RD);
    if (rd!=test_data[0])
    {
        rumboot_printf("Data error!\nExpected: 0x%X\nActual: 0x%X\n", test_data[0], rd);
        TEST_ASSERT(0, "test_oi10_em2_203 is failed!\n");
    }
}

void disable_mcheck()
{
    uint32_t const msr_old_value = msr_read();
    msr_write( msr_old_value & ~(0b1 << ITRPT_XSR_ME_i));
}

int main()
{
    emi_imr_cfg mask;
    struct rumboot_irq_entry *tbl;

    rumboot_printf("Start test_oi10_em2_203\n");
    test_event_send_test_id("test_oi10_em2_203");

    emi_init(DCR_EM2_EMI_BASE);
    emi_set_trdy(DCR_EM2_EMI_BASE, TEST_BANK_IDX, TEST_TRDY);
    emi_enable_ext_rdy(DCR_EM2_EMI_BASE, TEST_BANK_IDX);
    emi_get_int_mask(DCR_EM2_EMI_BASE, &mask);
    mask.MRDYR = true;
    mask.MRDYW = true;
    emi_set_int_mask(DCR_EM2_EMI_BASE, &mask);
    tbl = create_sram_irq_handler();

    check_wr_ext_rdy();//2.3.1 PPC_SRAM_SDRAM_slave0_testplan.docx
    check_rd_ext_rdy();//2.3.1 PPC_SRAM_SDRAM_slave0_testplan.docx

    disable_mcheck();//disabling due to mcheck appears earlier than interrupt from EMI
    check_irq_by_trdy_wr();//2.3.2 PPC_SRAM_SDRAM_slave0_testplan.docx
    check_irq_by_trdy_rd();//2.3.2 PPC_SRAM_SDRAM_slave0_testplan.docx

    delete_sram_irq_handler(tbl);

    return 0;
}

