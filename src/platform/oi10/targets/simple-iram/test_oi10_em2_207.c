/**
 * Implemented test which described in 2.7 PPC_SRAM_SDRAM_slave0_testplan.docx
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
#include <platform/test_event_c.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>

void report_data_error(uint32_t addr, uint64_t exp, uint64_t act)
{
    rumboot_printf("Data compare error at addr 0x%X!\n Expected: 0x%X\nActual: 0x%X\n", addr, exp, act);
}

void check_wrrd8(uint32_t addr, uint8_t data)
{
    uint8_t rd;
    rumboot_printf("Check Write/Read operation (0x%X). Width: 1 byte\n", addr);

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
    rumboot_printf("Check Write/Read operation (0x%X). Width: 2 bytes\n", addr);

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
    rumboot_printf("Check Write/Read operation (0x%X). Width: 4 bytes\n", addr);

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
    rumboot_printf("Check Write/Read operation (0x%X). Width: 8 bytes\n", addr);

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

void check16(uint32_t addr)
{
    for (int i=0; i<8; i++)
    {
        check_wrrd16(addr + i*2, (~(1 << (i+8)) & 0xFF00) | (1 << i) );
    }
}

void check32(uint32_t addr)
{
    const uint32_t tdata[4] = {
                                0xFFFFFFFF,
                                0xAAAAAAAA,
                                0x00000000,
                                0x55555555
                               };
    for (int i=0; i<4; i++)
    {
        check_wrrd32(addr + i*4, tdata[i]);
    }
}

void check64(uint32_t addr)
{
    check_wrrd64(addr, 0x0011223344556677);
}

void mem_set(uint32_t* addr, uint32_t val, uint32_t size)
{
    for (int i=0; i<(size/sizeof(uint32_t)); i++)
    {
        iowrite32(val, (uint32_t)(addr + i));
    }
}

void check_bank(uint32_t addr)
{
    rumboot_printf("Start checking bank 0x%X\n", addr);
    mem_set((uint32_t *) addr, 0x00, 0x100);
    check8(addr);
    check16(addr);
    check32(addr);
    check64(addr);
}

void run_check()
{
    check_bank(SRAM0_BASE);
    check_bank(SDRAM_BASE);
    check_bank(SSRAM_BASE);
    check_bank(PIPELINED_BASE);
    check_bank(SRAM1_BASE);
}

int main()
{
    rumboot_printf("Start test_oi10_em2_207\n");

    emi_init(DCR_EM2_EMI_BASE);

    emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_on);
    run_check();

    emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_off);
    run_check();

    return 0;
}

