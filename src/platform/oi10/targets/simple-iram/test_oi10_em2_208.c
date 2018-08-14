/**
 * Implemented test which described in 2.8 PPC_SRAM_SDRAM_slave0_testplan.docx
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

#define TEST_EVENT_CHECK_EMI_ACTIVE     0x00001000
#define TEST_EVENT_CHECK_EMI_HIZ        0x00001001

void check_emi_ports()
{
    const uint32_t test_data = 0xDEADBA11;
    emi_init();

    test_event(TEST_EVENT_CHECK_EMI_ACTIVE);
    iowrite32(test_data, SRAM0_BASE);
    TEST_ASSERT(test_data==ioread32(SRAM0_BASE), "Data error");

    test_event(TEST_EVENT_CHECK_EMI_HIZ);
    emi_hiz_mode_on(DCR_EM2_EMI_BASE);

    test_event(TEST_EVENT_CHECK_EMI_ACTIVE);
    emi_hiz_mode_off(DCR_EM2_EMI_BASE);
    iowrite32(test_data, SRAM0_BASE);
    TEST_ASSERT(test_data==ioread32(SRAM0_BASE), "Data error");
}

int main()
{
    rumboot_printf("Start test_oi10_em2_208\n");
    test_event_send_test_id("test_oi10_em2_208");

    check_emi_ports();

    return 0;
}


