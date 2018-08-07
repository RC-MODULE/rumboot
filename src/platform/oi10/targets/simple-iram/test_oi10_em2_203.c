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

#define TEST_BANK_IDX    0 //SRAM0
#define TEST_BANK_BASE   SRAM0_BASE
#define TEST_LEN         3
#define TEST_TRDY        (0x400 - 0x258)

#define TEST_EVENT_MAKE_PULSE_ON_RDY_An     0x0000003E

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

void emi_enable_ext_rdy()
{
    emi_bank_cfg bank_cfg;
    emi_get_bank_cfg(DCR_EM2_EMI_BASE, TEST_BANK_IDX, &bank_cfg);
    bank_cfg.ssx_cfg.SRDY = SRDY_EXT_RDY_USE;
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, TEST_BANK_IDX, &bank_cfg);
}

void emi_set_trdy(uint16_t trdy)
{
    emi_bank_cfg bank_cfg;
    emi_get_bank_cfg(DCR_EM2_EMI_BASE, TEST_BANK_IDX, &bank_cfg);
    bank_cfg.ssx_cfg.T_RDY = trdy;
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, TEST_BANK_IDX, &bank_cfg);
}

int main()
{
    uint32_t rd;

    rumboot_printf("Start test_oi10_em2_203\n");
    test_event_send_test_id("test_oi10_em2_203");

    emi_init();

    emi_enable_ext_rdy();
    check_wr_ext_rdy();//2.3.1 PPC_SRAM_SDRAM_slave0_testplan.docx
    check_rd_ext_rdy();//2.3.1 PPC_SRAM_SDRAM_slave0_testplan.docx

    //2.3.2 PPC_SRAM_SDRAM_slave0_testplan.docx
    emi_set_trdy(TEST_TRDY);
    iowrite32(test_data[0], test_addr[0]);
    rd = ioread32(test_addr[0]);
    if (rd!=test_data[0])
    {
        rumboot_printf("Data error!\nExpected: 0x%X\nActual: 0x%X\n", test_data[0], rd);
        TEST_ASSERT(0, "test_oi10_em2_203 is failed!\n");
    }


    return 0;
}

