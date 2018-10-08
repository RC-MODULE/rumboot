#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/interrupts.h>

#define RUMBOOT_ASSERT_WARN_ONLY
#include <platform/devices/nor_1636RR4.h>
#undef RUMBOOT_ASSERT_WARN_ONLY

#include <platform/regs/regs_mclfir.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>


#define EVENT_INJECT_SRAM0_0                    TEST_EVENT_CODE_MIN + 0
#define EVENT_INJECT_SRAM0_7                    TEST_EVENT_CODE_MIN + 1

#define EVENT_INJECT_SSRAM_0                    TEST_EVENT_CODE_MIN + 2
#define EVENT_INJECT_SSRAM_7                    TEST_EVENT_CODE_MIN + 3

#define EVENT_INJECT_SDRAM_0                    TEST_EVENT_CODE_MIN + 4
#define EVENT_INJECT_SDRAM_7                    TEST_EVENT_CODE_MIN + 5

#define EVENT_INJECT_NOR_0                      TEST_EVENT_CODE_MIN + 6
#define EVENT_INJECT_NOR_7                      TEST_EVENT_CODE_MIN + 7


#define ADDR_SRAM0_SE                           SRAM0_BASE
#define ADDR_SRAM0_DE                           SRAM0_BASE + 0x100

#define ADDR_SSRAM_SE                           SSRAM_BASE
#define ADDR_SSRAM_DE                           SSRAM_BASE + 0x100

#define ADDR_SDRAM_SE                           SDRAM_BASE
#define ADDR_SDRAM_DE                           SDRAM_BASE + 0x100

#define ADDR_NOR_SE                             NOR_BASE + 0x10000
#define ADDR_NOR_DE                             NOR_BASE + 0x10100

#define BABA0001_ECC                            0x7A


static void irq_handler( int irq, void *arg ) {

    switch (irq) {
        case EMI_CNTR_INT_0:
            rumboot_putstring("EMI_CNTR_INT_0 occured.\n");

            rumboot_printf("MCLFIR_MC_ERR0 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR0));
            rumboot_printf("MCLFIR_MC_ERR1 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR1));

            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND0, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND1, 0x00000000);

            break;

        case EMI_CNTR_INT_1:
            rumboot_putstring("EMI_CNTR_INT_1 occured.\n");

            rumboot_printf("MCLFIR_MC_ERR0 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR0));
            rumboot_printf("MCLFIR_MC_ERR1 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR1));

            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND0, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND1, 0x00000000);

            break;

        case EMI_CNTR_INT_2:
            rumboot_putstring("EMI_CNTR_INT_2 occured.\n");

            rumboot_printf("MCLFIR_MC_ERR0 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR0));
            rumboot_printf("MCLFIR_MC_ERR1 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR1));

            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND0, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND1, 0x00000000);

            break;

        case EMI_CNTR_INT_3:
            rumboot_putstring("EMI_CNTR_INT_3 occured.\n");

            rumboot_printf("MCLFIR_MC_ERR0 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR0));
            rumboot_printf("MCLFIR_MC_ERR1 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR1));

            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND0, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND1, 0x00000000);

            break;

        case MCLFIR_REC_INT:
            rumboot_putstring("MCLFIR_REC_INT occured.\n");

            rumboot_printf("MCLFIR_MC_ERR0 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR0));
            rumboot_printf("MCLFIR_MC_ERR1 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR1));
            rumboot_printf("MCLFIR_MC_LFIR = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR));

            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND0, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND1, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR_AND, 0x00000000);
            break;

        case MCLFIR_UNREC_INT:
            rumboot_putstring("MCLFIR_UNREC_INT occured.\n");

            rumboot_printf("MCLFIR_MC_ERR1 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR1));
            rumboot_printf("MCLFIR_MC_LFIR = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR));
            rumboot_printf("MCLFIR_MC_ERR0 = 0x%x\n", dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR0));

            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND0, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND1, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR_AND, 0x00000000);
            break;

        default:
            rumboot_putstring("Unexpected interrupt occured!\n");
            break;
    }
}


bool check_data()
{
    uint32_t reg = 0, stat = 0;

    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00); //clear reg
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT53, 0x00); //clear reg


    rumboot_putstring("Checking SRAM0 ... \n");
    reg = ioread32(ADDR_SRAM0_SE);
    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SRAM0 (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SRAM0_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x000001, "Invalid value in STATUS_IRQ (SRAM0)");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00);

    reg = ioread32(ADDR_SRAM0_DE);
    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR SRAM0 (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SRAM0_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x000000, "Invalid value in STATUS_IRQ (SRAM)");


    rumboot_putstring("Checking SSRAM ... \n");
    reg = ioread32(ADDR_SSRAM_SE);
    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SSRAM (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SSRAM_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x10000, "Invalid value in STATUS_IRQ (SSRAM)");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00);

    reg = ioread32(ADDR_SSRAM_DE);
    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR SSRAM (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SSRAM_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x00, "Invalid value in STATUS_IRQ (SSRAM)");


    rumboot_putstring("Checking SDRAM ... \n");
    reg = ioread32(ADDR_SDRAM_SE);
    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SDRAM (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SDRAM_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x100, "Invalid value in STATUS_IRQ (SDRAM)");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00);

    reg = ioread32(ADDR_SDRAM_DE);
    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR SDRAM (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SDRAM_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x00, "Invalid value in STATUS_IRQ (SDRAM)");


    rumboot_putstring("Checking NOR ... \n");
    reg = ioread32(ADDR_NOR_SE);
    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR NOR (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_NOR_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT53);
    TEST_ASSERT(stat == 0x010000, "Invalid value in STATUS_IRQ (NOR)");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT53, 0x00);

    reg = ioread32(ADDR_NOR_DE);
    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR NOR (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_NOR_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT53);
    TEST_ASSERT(stat == 0x00, "Invalid value in STATUS_IRQ (NOR)");

    return true;
}


int main(void) 
{
    uint32_t test_result = 0;

    test_event_send_test_id("test_oi10_em2_205");

    emi_init(DCR_EM2_EMI_BASE);
    emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_on);


    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_2, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_3, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );

    rumboot_irq_set_handler( tbl, MCLFIR_REC_INT,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, MCLFIR_UNREC_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( EMI_CNTR_INT_0 );
    rumboot_irq_enable( EMI_CNTR_INT_1 );
    rumboot_irq_enable( EMI_CNTR_INT_2 );
    rumboot_irq_enable( EMI_CNTR_INT_3 );
    rumboot_irq_enable( MCLFIR_REC_INT );
    rumboot_irq_enable( MCLFIR_UNREC_INT );
    rumboot_irq_sei();


    //Unmask EMI memory error interrupts
    dcr_write(DCR_EM2_EMI_BASE + EMI_IMR, 0xFFF);


    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_MSK_AND0, 0x00000000);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_MSK_AND1, 0x0100003F);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR_MSK_AND, 0x3FFFFFFF);


    rumboot_putstring("WRITE SRAM0\n");
    test_event(EVENT_INJECT_SRAM0_0); //DATA_SE = 0xBABA0000
    iowrite32(0xBABA0001, ADDR_SRAM0_SE);
    isync();

    test_event(EVENT_INJECT_SRAM0_7); //DATA_DE = 0xBABA0007
    iowrite32(0xBABA0001, ADDR_SRAM0_DE);
    isync();


    rumboot_putstring("WRITE SSRAM\n");
    test_event(EVENT_INJECT_SSRAM_0); //DATA_SE = 0xBABA0000
    iowrite32(0xBABA0001, ADDR_SSRAM_SE);
    isync();

    test_event(EVENT_INJECT_SSRAM_7); //DATA_SE = 0xBABA0007
    iowrite32(0xBABA0001, ADDR_SSRAM_DE);
    isync();


    rumboot_putstring("WRITE SDRAM\n");
    test_event(EVENT_INJECT_SDRAM_0); //DATA_SE = 0xBABA0000
    iowrite32(0xBABA0001, ADDR_SDRAM_SE);
    isync();

    test_event(EVENT_INJECT_SDRAM_7); //DATA_SE = 0xBABA0007
    iowrite32(0xBABA0001, ADDR_SDRAM_DE);
    isync();


    //Software error injection for NOR
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECCWRR, BABA0001_ECC);
    dcr_write(DCR_EM2_EMI_BASE + EMI_FLCNTRL,  (dcr_read(DCR_EM2_EMI_BASE + EMI_FLCNTRL) & 0x1C)  | ECC_CTRL_CNT_ECCWRR);

    rumboot_putstring("WRITE NOR\n");
    rumboot_putstring("Next assertion is expected behaviour!");
    nor_write32(0xBABA0000, ADDR_NOR_SE);
    isync();

    nor_write32(0xBABA0007, ADDR_NOR_DE);
    isync();

    udelay(5);

    rumboot_putstring("CHECK\n");
    if (!check_data())
        test_result = 1;

    rumboot_irq_table_activate( NULL );
    rumboot_irq_free( tbl );

    if (!test_result)
        rumboot_putstring("Test has been finished successfully.");

    return test_result;
}
