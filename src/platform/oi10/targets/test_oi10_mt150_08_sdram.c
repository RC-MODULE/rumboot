#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

#include <arch/ppc_476fp_config.h>
#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <platform/interrupts.h>

#define NOR_SELFCHECKING_DISABLE
#include <platform/devices/nor_1636RR4.h>
#undef NOR_SELFCHECKING_DISABLE

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


#define ADDR_SRAM0_SE                           SRAM0_BASE + 0x10000
#define ADDR_SRAM0_DE                           SRAM0_BASE + 0x10100

#define ADDR_SSRAM_SE                           SSRAM_BASE + 0x10000
#define ADDR_SSRAM_DE                           SSRAM_BASE + 0x10100

#define ADDR_SDRAM_SE                           SDRAM_BASE + 0x10000
#define ADDR_SDRAM_DE                           SDRAM_BASE + 0x10100

#define ADDR_NOR_SE                             NOR_BASE + 0x10000
#define ADDR_NOR_SE_2                           NOR_BASE + 0x10010
#define ADDR_NOR_DE                             NOR_BASE + 0x10100
#define ADDR_NOR_DE_2                           NOR_BASE + 0x10110

#define BABA0001_ECC                            0x7A
#define BABA0001_ECC_1bit                       0x7B
#define BABA0001_ECC_2bit                       0x7F


enum emi_error_irq_source
{
    emi_single_error_irq_source_SRAM0,
    emi_single_error_irq_source_SSRAM,
    emi_single_error_irq_source_SDRAM,
    emi_single_error_irq_source_NOR,

    emi_double_error_irq_source_SRAM0,
    emi_double_error_irq_source_SSRAM,
    emi_double_error_irq_source_SDRAM,
    emi_double_error_irq_source_NOR
};


volatile enum emi_error_irq_source   error_irq_source;

volatile bool emi_single_error_int_occured = false;
volatile bool emi_double_error_int_occured = false;

volatile bool emi_recoverable_error_int_occured = false;
volatile bool emi_unrecoverable_error_int_occured = false;

const uint32_t ERROR_WAIT_TIMEOUT = 1000;


static void irq_handler( int irq, void *arg ) {

    switch (irq) {
        case EMI_CNTR_INT_0:
            rumboot_putstring("EMI_CNTR_INT_0 occured.\n");

            switch (error_irq_source)
            {
                case emi_single_error_irq_source_SRAM0:

                    TEST_ASSERT(dcr_read(DCR_EM2_EMI_BASE + EMI_IRR) == 0x000001, "SRAM0: expected single error.");
                    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x0001);

                    break;


                case emi_single_error_irq_source_SSRAM:

                    TEST_ASSERT(dcr_read(DCR_EM2_EMI_BASE + EMI_IRR) == 0x000010, "SSRAM: expected single error.");
                    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x0010);

                    break;


                case emi_single_error_irq_source_SDRAM:

                    TEST_ASSERT(dcr_read(DCR_EM2_EMI_BASE + EMI_IRR) == 0x000004, "SDRAM: expected single error.");
                    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x0004);

                    break;


                case emi_single_error_irq_source_NOR:

                    TEST_ASSERT(dcr_read(DCR_EM2_EMI_BASE + EMI_IRR) == 0x000400, "NOR: expected single error.");
                    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x0400);

                    break;


                default:
                    rumboot_putstring("Unexpected EMI error irq source.");
                    break;
            }

            emi_single_error_int_occured = true;

            break;


        case EMI_CNTR_INT_1:
            rumboot_putstring("EMI_CNTR_INT_1 occured.\n");

            switch (error_irq_source)
            {
                case emi_double_error_irq_source_SRAM0:

                    TEST_ASSERT(dcr_read(DCR_EM2_EMI_BASE + EMI_IRR) == 0x000002, "SRAM0: expected single error.");
                    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x0002);

                    break;


                case emi_double_error_irq_source_SSRAM:

                    TEST_ASSERT(dcr_read(DCR_EM2_EMI_BASE + EMI_IRR) == 0x000020, "SSRAM: expected single error.");
                    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x0020);

                    break;


                case emi_double_error_irq_source_SDRAM:

                    TEST_ASSERT(dcr_read(DCR_EM2_EMI_BASE + EMI_IRR) == 0x000008, "SDRAM: expected single error.");
                    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x0008);

                    break;


                case emi_double_error_irq_source_NOR:

                    TEST_ASSERT(dcr_read(DCR_EM2_EMI_BASE + EMI_IRR) == 0x000800, "NOR: expected single error.");
                    dcr_write(DCR_EM2_EMI_BASE + EMI_IRR_RST, 0x0800);

                    break;


                default:
                    rumboot_putstring("Unexpected EMI error irq source.");
                    break;
            }

            emi_double_error_int_occured = true;

            break;


        case MCLFIR_REC_INT:
            rumboot_putstring("MCLFIR_REC_INT occured.\n");

            TEST_ASSERT((dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR) & 0x80000000) == 0x80000000, "Expected recoverable error.");

            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND0, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND1, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR_AND, 0x00000000);

            emi_recoverable_error_int_occured = true;

            break;


        case MCLFIR_UNREC_INT:
            rumboot_putstring("MCLFIR_UNREC_INT occured.\n");

            TEST_ASSERT((dcr_read(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR) & 0x40000000) == 0x40000000, "Expected unrecoverable error.");

            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND0, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_AND1, 0x00000000);
            dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR_AND, 0x00000000);

            emi_unrecoverable_error_int_occured = true;

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

    rumboot_putstring("Checking SRAM0 single error ... \n");

    error_irq_source = emi_single_error_irq_source_SRAM0;

    reg = ioread32(ADDR_SRAM0_SE);

    TEST_WAIT_ASSERT(emi_single_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SRAM0: single error interrupt waiting timeout.");
    emi_single_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x000001, "SRAM0: invalid value in STATUS_IRQ");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00);

    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SRAM0 (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SRAM0_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_recoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SRAM0: recoverable error interrupt waiting timeout.");
    emi_recoverable_error_int_occured = false;



    rumboot_putstring("Checking SRAM0 double error ... \n");

    error_irq_source = emi_double_error_irq_source_SRAM0;

    reg = ioread32(ADDR_SRAM0_DE);

    TEST_WAIT_ASSERT(emi_double_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SRAM0: double error interrupt waiting timeout.");
    emi_double_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x000000, "SRAM0: invalid value in STATUS_IRQ.");

    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR SRAM0 (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SRAM0_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_unrecoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SRAM0: unrecoverable error interrupt waiting timeout.");
    emi_unrecoverable_error_int_occured = false;



    rumboot_putstring("Checking SSRAM single error ... \n");

    error_irq_source = emi_single_error_irq_source_SSRAM;

    reg = ioread32(ADDR_SSRAM_SE);

    TEST_WAIT_ASSERT(emi_single_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SSRAM: single error interrupt waiting timeout.");
    emi_single_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x10000, "SSRAM: invalid value in STATUS_IRQ");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00);

    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SSRAM (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SSRAM_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_recoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SSRAM: recoverable error interrupt waiting timeout.");
    emi_recoverable_error_int_occured = false;



    rumboot_putstring("Checking SSRAM double error ... \n");

    error_irq_source = emi_double_error_irq_source_SSRAM;

    reg = ioread32(ADDR_SSRAM_DE);

    TEST_WAIT_ASSERT(emi_double_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SSRAM: double error interrupt waiting timeout.");
    emi_double_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x00, "SSRAM: invalid value in STATUS_IRQ.");

    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR SSRAM (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SSRAM_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_unrecoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SSRAM: unrecoverable error interrupt waiting timeout.");
    emi_unrecoverable_error_int_occured = false;



    rumboot_putstring("Checking SDRAM single error ... \n");

    error_irq_source = emi_single_error_irq_source_SDRAM;

    reg = ioread32(ADDR_SDRAM_SE);

    TEST_WAIT_ASSERT(emi_single_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SDRAM: single error interrupt waiting timeout.");
    emi_single_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x100, "SDRAM: invalid value in STATUS_IRQ");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00);

    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SDRAM (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SDRAM_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_recoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SDRAM: recoverable error interrupt waiting timeout.");
    emi_recoverable_error_int_occured = false;



    rumboot_putstring("Checking SDRAM double error ... \n");

    error_irq_source = emi_double_error_irq_source_SDRAM;

    reg = ioread32(ADDR_SDRAM_DE);

    TEST_WAIT_ASSERT(emi_double_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SDRAM: double error interrupt waiting timeout.");
    emi_double_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x00, "SDRAM: invalid value in STATUS_IRQ.");

    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR SDRAM (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SDRAM_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_unrecoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SDRAM: unrecoverable error interrupt waiting timeout.");
    emi_unrecoverable_error_int_occured = false;



    rumboot_putstring("Checking NOR single error ... \n");

    error_irq_source = emi_single_error_irq_source_NOR;

    reg = ioread32(ADDR_NOR_SE);

    TEST_WAIT_ASSERT(emi_single_error_int_occured == true, ERROR_WAIT_TIMEOUT, "NOR: single error interrupt waiting timeout.");
    emi_single_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT53);
    TEST_ASSERT(stat == 0x010000, "NOR: invalid value in STATUS_IRQ.");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT53, 0x00);

    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR NOR (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_NOR_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_recoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "NOR: recoverable error interrupt waiting timeout.");
    emi_recoverable_error_int_occured = false;



    rumboot_putstring("Checking NOR double error ... \n");

    error_irq_source = emi_double_error_irq_source_NOR;

    reg = ioread32(ADDR_NOR_DE);

    TEST_WAIT_ASSERT(emi_double_error_int_occured == true, ERROR_WAIT_TIMEOUT, "NOR: double error interrupt waiting timeout.");
    emi_double_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT53);
    TEST_ASSERT(stat == 0x00, "NOR: invalid value in STATUS_IRQ.");

    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR NOR (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_NOR_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_unrecoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "NOR: recoverable error interrupt waiting timeout.");
    emi_unrecoverable_error_int_occured = false;

    return true;
}


bool check_data_ECC()
{
    uint32_t reg = 0, stat = 0;

    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00); //clear reg
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT53, 0x00); //clear reg

    rumboot_putstring("Checking SRAM0 single error ... \n");

    error_irq_source = emi_single_error_irq_source_SRAM0;

    reg = ioread32(ADDR_SRAM0_SE);

    TEST_WAIT_ASSERT(emi_single_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SRAM0: single error interrupt waiting timeout.");
    emi_single_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x000001, "SRAM0: invalid value in STATUS_IRQ");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00);

    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SRAM0 (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SRAM0_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_recoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SRAM0: recoverable error interrupt waiting timeout.");
    emi_recoverable_error_int_occured = false;



    rumboot_putstring("Checking SRAM0 double error ... \n");

    error_irq_source = emi_double_error_irq_source_SRAM0;

    reg = ioread32(ADDR_SRAM0_DE);

    TEST_WAIT_ASSERT(emi_double_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SRAM0: double error interrupt waiting timeout.");
    emi_double_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x000000, "SRAM0: invalid value in STATUS_IRQ.");

    if (reg != 0xBABA0001) //see .svh
    {
        rumboot_putstring("ECC ERROR SRAM0 (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SRAM0_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_unrecoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SRAM0: unrecoverable error interrupt waiting timeout.");
    emi_unrecoverable_error_int_occured = false;



    rumboot_putstring("Checking SSRAM single error ... \n");

    error_irq_source = emi_single_error_irq_source_SSRAM;

    reg = ioread32(ADDR_SSRAM_SE);

    TEST_WAIT_ASSERT(emi_single_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SSRAM: single error interrupt waiting timeout.");
    emi_single_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x10000, "SSRAM: invalid value in STATUS_IRQ");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00);

    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SSRAM (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SSRAM_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_recoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SSRAM: recoverable error interrupt waiting timeout.");
    emi_recoverable_error_int_occured = false;



    rumboot_putstring("Checking SSRAM double error ... \n");

    error_irq_source = emi_double_error_irq_source_SSRAM;

    reg = ioread32(ADDR_SSRAM_DE);

    TEST_WAIT_ASSERT(emi_double_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SSRAM: double error interrupt waiting timeout.");
    emi_double_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x00, "SSRAM: invalid value in STATUS_IRQ.");

    if (reg != 0xBABA0001) //see .svh
    {
        rumboot_putstring("ECC ERROR SSRAM (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SSRAM_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_unrecoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SSRAM: unrecoverable error interrupt waiting timeout.");
    emi_unrecoverable_error_int_occured = false;



    rumboot_putstring("Checking SDRAM single error ... \n");

    error_irq_source = emi_single_error_irq_source_SDRAM;

    reg = ioread32(ADDR_SDRAM_SE);

    TEST_WAIT_ASSERT(emi_single_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SDRAM: single error interrupt waiting timeout.");
    emi_single_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x100, "SDRAM: invalid value in STATUS_IRQ");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00);

    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SDRAM (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SDRAM_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_recoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SDRAM: recoverable error interrupt waiting timeout.");
    emi_recoverable_error_int_occured = false;



    rumboot_putstring("Checking SDRAM double error ... \n");

    error_irq_source = emi_double_error_irq_source_SDRAM;

    reg = ioread32(ADDR_SDRAM_DE);

    TEST_WAIT_ASSERT(emi_double_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SDRAM: double error interrupt waiting timeout.");
    emi_double_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x00, "SDRAM: invalid value in STATUS_IRQ.");

    if (reg != 0xBABA0001) //see .svh
    {
        rumboot_putstring("ECC ERROR SDRAM (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SDRAM_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_unrecoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "SDRAM: unrecoverable error interrupt waiting timeout.");
    emi_unrecoverable_error_int_occured = false;



    rumboot_putstring("Checking NOR single error ... \n");

    error_irq_source = emi_single_error_irq_source_NOR;

    reg = ioread32(ADDR_NOR_SE_2);

    TEST_WAIT_ASSERT(emi_single_error_int_occured == true, ERROR_WAIT_TIMEOUT, "NOR: single error interrupt waiting timeout.");
    emi_single_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT53);
    TEST_ASSERT(stat == 0x010000, "NOR: invalid value in STATUS_IRQ.");
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT53, 0x00);

    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR NOR (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_NOR_SE_2);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_recoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "NOR: recoverable error interrupt waiting timeout.");
    emi_recoverable_error_int_occured = false;



    rumboot_putstring("Checking NOR double error ... \n");

    error_irq_source = emi_double_error_irq_source_NOR;

    reg = ioread32(ADDR_NOR_DE_2);

    TEST_WAIT_ASSERT(emi_double_error_int_occured == true, ERROR_WAIT_TIMEOUT, "NOR: double error interrupt waiting timeout.");
    emi_double_error_int_occured = false;

    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT53);
    TEST_ASSERT(stat == 0x00, "NOR: invalid value in STATUS_IRQ.");

    if (reg != 0xBABA0001) //see .svh
    {
        rumboot_putstring("ECC ERROR NOR (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_NOR_DE_2);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }

    TEST_WAIT_ASSERT(emi_unrecoverable_error_int_occured == true, ERROR_WAIT_TIMEOUT, "NOR: recoverable error interrupt waiting timeout.");
    emi_unrecoverable_error_int_occured = false;

    return true;
}


static void exception_handler(int const id, char const * const name ) {
    rumboot_printf( "Exception: %s\n", name );
    TEST_ASSERT( (id == RUMBOOT_IRQ_MACHINE_CHECK)
              && (spr_read(SPR_MCSR_RW) == ( (0b1 << ITRPT_MCSR_MCS_i)
                                           | (0b1 << ITRPT_MCSR_L2_i) ))
              && (l2c_l2_read(DCR_L2C_BASE, L2C_L2FERR) == (0b1 << 9)),
                 "Unexpected exception" );

    rumboot_printf("L2MCK = 0x%x\n", l2c_l2_read(DCR_L2C_BASE, L2C_L2MCK));
    rumboot_printf("L2FERR = 0x%x\n", l2c_l2_read(DCR_L2C_BASE, L2C_L2FERR));
    rumboot_printf("L2INT = 0x%x\n", l2c_l2_read(DCR_L2C_BASE, L2C_L2INT));

    rumboot_printf("L2C_L2PLBSTAT0 = 0x%x\n", l2c_l2_read(DCR_L2C_BASE, L2C_L2PLBSTAT0));
    rumboot_printf("L2C_L2PLBSTAT1 = 0x%x\n", l2c_l2_read(DCR_L2C_BASE, L2C_L2PLBSTAT1));

    l2c_l2_write( DCR_L2C_BASE, L2C_L2PLBSTAT0, 0xFFFFFFFF );
    l2c_l2_write( DCR_L2C_BASE, L2C_L2PLBSTAT1, 0xFFFFFFFF );

    spr_write( SPR_MCSR_C, spr_read(SPR_MCSR_RW) );
}


int main(void)
{   
    rumboot_printf("start!\n");
    uint32_t test_result = 0;

    rumboot_irq_set_exception_handler(exception_handler);

    memset((uint8_t*)SRAM0_BASE, 0x00, 0x1000);

    emi_init(DCR_EM2_EMI_BASE);
//    emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_on);
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

if (1==0) {
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, EMI_CNTR_INT_1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );

    rumboot_irq_set_handler( tbl, MCLFIR_REC_INT,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, MCLFIR_UNREC_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, ( void* )0 );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( EMI_CNTR_INT_0 );
    rumboot_irq_enable( EMI_CNTR_INT_1 );
    rumboot_irq_enable( MCLFIR_REC_INT );
    rumboot_irq_enable( MCLFIR_UNREC_INT );
    rumboot_irq_sei();
}

    //Unmask EMI memory error interrupts
    dcr_write(DCR_EM2_EMI_BASE + EMI_IMR, 0xFFF);

    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_MSK_AND0, 0x00000000);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_MSK_AND1, 0x0100003F);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_LFIR_MSK_AND, 0x3FFFFFFF);

    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_ACTION0, 0x00000420);
    dcr_write(DCR_EM2_MCLFIR_BASE + MCLFIR_MC_ERR_ACTION1, 0x00000000);

/*
    rumboot_putstring("WRITE SRAM0\n");

    iowrite32(0xBABA0001, ADDR_SRAM0_SE);
    iowrite32(0xBABA0001, ADDR_SRAM0_DE);
    msync();
    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x10);

    iowrite32(0xBABA0000, ADDR_SRAM0_SE);
    msync();

    iowrite32(0xBABA0007, ADDR_SRAM0_DE);
    msync();

    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x00);


    rumboot_putstring("WRITE SSRAM\n");

    iowrite32(0xBABA0001, ADDR_SSRAM_SE);
    iowrite32(0xBABA0001, ADDR_SSRAM_DE);
    msync();
    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x10);

    iowrite32(0xBABA0000, ADDR_SSRAM_SE);
    msync();

    iowrite32(0xBABA0007, ADDR_SSRAM_DE);
    msync();

    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x00);
*/

    rumboot_putstring("WRITE SDRAM\n");
/*    
    for (int i=0; i<32; i++) {
        iowrite32(1<<i, SDRAM_BASE+(i*4));
    }
    iowrite32(0x12345678, SDRAM_BASE+0x04000004);
    
    for (int i=0; i<32; i++) {
        rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),ioread32(SDRAM_BASE+(i*4)));
    }
    
    rumboot_printf("read_data(0x%x) = 0x%x\n", (SDRAM_BASE+0x04000004),ioread32(SDRAM_BASE+0x04000004));
*/    
    
    uint32_t read_data;
    
    rumboot_printf("Read emi sd1 \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0xc);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0xc,read_data);
    rumboot_printf("Read emi rfc \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0x30);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x30,read_data);
    rumboot_printf("Read EMI_HSTSR \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0x34);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x34,read_data);
    rumboot_printf("Read EMI_WECR \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0x58);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x58,read_data);
    rumboot_printf("------------------- \n");
    rumboot_printf("Write emi sd1 \n");
    dcr_write((DCR_EM2_EMI_BASE + 0xc), 0x9622);
    rumboot_printf("Read emi sd1 \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0xc);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0xc,read_data);
    
    rumboot_printf("Write EMI_HSTSR \n");
    dcr_write((DCR_EM2_EMI_BASE + 0x34), 0x2);
    rumboot_printf("Read EMI_HSTSR \n");
    read_data = dcr_read(DCR_EM2_EMI_BASE + 0x34);
    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x34,read_data);
        
//    rumboot_printf("Write EMI_WECR \n");
//    dcr_write((DCR_EM2_EMI_BASE + 0x58), 0x20);
//    rumboot_printf("Read EMI_WECR \n");
//    read_data = dcr_read(DCR_EM2_EMI_BASE + 0x58);
//    rumboot_printf("read_data(0x%x) = 0x%x\n", DCR_EM2_EMI_BASE + 0x58,read_data);

/*
        rumboot_printf("Write mem <<1 \n");
        iowrite32(0, SDRAM_BASE);
        read_data = ioread32(SDRAM_BASE);
        rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE,read_data);
        rumboot_printf("-------------\n");
        for (int i=1; i<8388608; i=i<<1) {
            iowrite32(i*4, SDRAM_BASE+(i*4));
            read_data = ioread32(SDRAM_BASE+(i*4));
            rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
        }
//        mdelay(1000);
        rumboot_printf("-------------\n");
        read_data = ioread32(SDRAM_BASE);
        rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE,read_data);
        for (int i=1; i<8388608; i=i<<1) {
            read_data = ioread32(SDRAM_BASE+(i*4));
            rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
            if (read_data != i*4) {
                rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
                rumboot_printf("error data! \n");
                test_result = 1;
//                break;
            }
        }
*/
       
    rumboot_printf("Write all mem  \n");
    for (int i=0; i<8388608; i++) {
        iowrite32(i*4, SDRAM_BASE+(i*4));
    }
    
    rumboot_printf("Read all mem  \n");
    for (int i=0; i<8388608; i++) {
        read_data = ioread32(SDRAM_BASE+(i*4));
//        rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
        if (read_data != i*4) {
            rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
            rumboot_printf("error data! \n");
            test_result = 1;
            break;
        }
        if ((i&0x00fffff) == 0xfffff) {
            rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
        }
    }


/*    
    rumboot_printf("Write all mem  \n");
    for (int j=0; j<4; j++) {
        for (int i=2097152*j; i<2097152*(j+1); i++) {
            iowrite32(i*4, SDRAM_BASE+(i*4));
        }
    }
    
    rumboot_printf("Read all mem  \n");
    for (int j=0; j<4; j++) {
        for (int i=2097152*j; i<2097152*(j+1); i++) {
            read_data = ioread32(SDRAM_BASE+(i*4));
//            rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
            if (read_data != i*4) {
                rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
                rumboot_printf("error data! \n");
                test_result = 1;
                break;
            }
            if ((i&0x0000fff) == 0xfff) {
                rumboot_printf("read_data(0x%x) = 0x%x\n", SDRAM_BASE+(i*4),read_data);
            }
        }
    }
*/    
/*    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x10);

    iowrite32(0xBABA0000, ADDR_SDRAM_SE);
    msync();

    iowrite32(0xBABA0007, ADDR_SDRAM_DE);
    msync();

    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x00);


    //Software error injection for NOR
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECCWRR, BABA0001_ECC);
    dcr_write(DCR_EM2_EMI_BASE + EMI_FLCNTRL,  (dcr_read(DCR_EM2_EMI_BASE + EMI_FLCNTRL) & 0x1C)  | ECC_CTRL_CNT_ECCWRR);

    rumboot_putstring("WRITE NOR\n");
    nor_write32(0xBABA0000, ADDR_NOR_SE);
    msync();

    nor_write32(0xBABA0007, ADDR_NOR_DE);
    msync();


    rumboot_putstring("CHECK\n");
    if (!check_data())
        test_result = 1;



    rumboot_putstring("WRITE SRAM0 ECC\n");

    iowrite32(0xBABA0001, ADDR_SRAM0_SE);
    iowrite32(0xBABA0001, ADDR_SRAM0_DE);
    msync();
    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x2F);

    iowrite32(0x7E000000, ADDR_SRAM0_SE);
    msync();

    iowrite32(0x1A000000, ADDR_SRAM0_DE);
    msync();

    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x00);


    rumboot_putstring("WRITE SSRAM ECC\n");

    iowrite32(0xBABA0001, ADDR_SSRAM_SE);
    iowrite32(0xBABA0001, ADDR_SSRAM_DE);
    msync();
    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x2F);

    iowrite32(0x7E000000, ADDR_SSRAM_SE);
    msync();

    iowrite32(0x1A000000, ADDR_SSRAM_DE);
    msync();

    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x00);


    rumboot_putstring("WRITE SDRAM ECC\n");

    iowrite32(0xBABA0001, ADDR_SDRAM_SE);
    iowrite32(0xBABA0001, ADDR_SDRAM_DE);
    msync();
    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x2F);

    iowrite32(0x7E000000, ADDR_SDRAM_SE);
    msync();

    iowrite32(0x1A000000, ADDR_SDRAM_DE);
    msync();

    dcr_write((DCR_EM2_EMI_BASE + EMI_WECR), 0x00);


    //Software error injection for NOR ECC
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECCWRR, BABA0001_ECC_1bit);
    dcr_write(DCR_EM2_EMI_BASE + EMI_FLCNTRL,  (dcr_read(DCR_EM2_EMI_BASE + EMI_FLCNTRL) & 0x1C)  | ECC_CTRL_CNT_ECCWRR);

    rumboot_putstring("WRITE NOR ECC\n");
    nor_write32(0xBABA0001, ADDR_NOR_SE_2);
    msync();

    dcr_write(DCR_EM2_EMI_BASE + EMI_ECCWRR, BABA0001_ECC_2bit);
    dcr_write(DCR_EM2_EMI_BASE + EMI_FLCNTRL,  (dcr_read(DCR_EM2_EMI_BASE + EMI_FLCNTRL) & 0x1C)  | ECC_CTRL_CNT_ECCWRR);

    nor_write32(0xBABA0001, ADDR_NOR_DE_2);
    msync();


    rumboot_putstring("CHECK ECC\n");
    if (!check_data_ECC())
        test_result = 1;


*/
    rumboot_irq_table_activate( NULL );
    rumboot_irq_free( tbl );


    if (!test_result)
        rumboot_putstring("Test has been finished successfully.");

    return test_result;
}
