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

void report_data_error(uint32_t addr, uint32_t exp, uint32_t act)
{
    rumboot_printf("Data compare error at addr 0x%X!\n Expected: 0x%X\nActual: 0x%X\n", addr, exp, act);
}

void check_wrrd8(uint32_t addr, uint8_t data)
{
    uint8_t rd;
    rumboot_printf("Check Write/Read operation. Width: 1 byte\n");

    iowrite8(data, addr);

    rd = ioread8(addr);
    if (rd != data)
    {
        report_data_error(addr, data, rd);
        TEST_ASSERT(0, "Data error (1 byte)");
    }
}

void check_wrrd16(uint32_t addr, uint16_t data)
{
    uint16_t rd;
    rumboot_printf("Check Write/Read operation. Width: 2 bytes\n");

    iowrite16(data, addr);

    rd = ioread16(addr);
    if (rd != data)
    {
        report_data_error(addr, data, rd);
        TEST_ASSERT(0, "Data error (2 bytes)");
    }
}

void check_wrrd32(uint32_t addr, uint16_t data)
{
    uint32_t rd;
    rumboot_printf("Check Write/Read operation. Width: 4 bytes\n");

    iowrite32(data, addr);

    rd = ioread32(addr);
    if (rd != data)
    {
        report_data_error(addr, data, rd);
        TEST_ASSERT(0, "Data error (4 bytes)");
    }
}

void check_wrrd64(uint32_t addr, uint64_t data)
{
    uint64_t rd;
    rumboot_printf("Check Write/Read operation. Width: 8 bytes\n");

    iowrite64(data, addr);

    rd = ioread64(addr);
    if (rd != data)
    {
        report_data_error(addr, data, rd);
        TEST_ASSERT(0, "Data error (8 bytes)");
    }
}

void check8(uint32_t addr)
{

    for (int i=0; i<16; i++)
    {
        if (i<8)
            check_wrrd8(addr + i, (1 << i) );
        else
            check_wrrd8(addr + i, ~(1 << (i - 8) ));
    }
}

int main()
{

    rumboot_printf("Start test_oi10_em2_204\n");

    emi_init();
    emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_on);

    check8(SRAM0_BASE);

    return 0;
}

