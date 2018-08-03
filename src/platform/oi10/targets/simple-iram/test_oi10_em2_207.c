/**
 * Implemented test which described in 2.7 PPC_SRAM_SDRAM_slave0_testplan.docx
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/test_event_c.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>

int main()
{

    rumboot_printf("Start test_oi10_em2_204\n");

    emi_init();
    emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_on);

    return 0;
}

