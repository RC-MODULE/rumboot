
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/regpoker.h>

#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>


#define EVENT_INJECT_ERR_S          TEST_EVENT_CODE_MIN + 0
#define EVENT_INJECT_ERR_D          TEST_EVENT_CODE_MIN + 1
#define EVENT_CHK_IRQ		        TEST_EVENT_CODE_MIN + 2

#define CONST_1                     0xBABA0001

#define ADDR_SE_SRAM                EM2_BASE
#define ADDR_DE_SRAM                EM2_BASE + 0x100

#define TEST_OK                     0x00000000
#define TEST_ERROR                  0x00000001


void check_emi(uint32_t base_addr, uint32_t *tpl, uint32_t val, uint32_t ev_code)
{
    int          i;
    uint8_t     *byte_tpl = (uint8_t*)&(tpl[3]);
    iowrite32(val, base_addr);
    rumboot_printf("READED DATA = 0x%X\n", ioread32(base_addr));

    for(i = 0; i < 4; i++)
    {
        iowrite8(byte_tpl[i], base_addr + i);
        rumboot_printf("writing byte, data = 0x%X\n", ioread32(base_addr));
        TEST_ASSERT(ioread32(ADDR_SE_SRAM) == tpl[i], "READ ERROR");
    }

    rumboot_putstring("done.\n");

    rumboot_putstring("Inject error by h/w event...");
    // test_event(ev_code);

    rumboot_putstring("Check data\n");
    rumboot_printf("Expected = 0x%X, Actual = 0x%X\n", tpl[3], ioread32(base_addr));
    TEST_ASSERT (ioread32(base_addr) == tpl[3], "ECC ERROR");
    rumboot_putstring("Success!\n");
}

uint32_t main(void)
{
    static
    uint32_t    long_tpl[] = {0xDEBA0001, 0xDEDA001, 0xDEDA0101, 0xDEDA0100};

    emi_init_impl (DCR_EM2_EMI_BASE, DCR_EM2_PLB6MCIF2_BASE, 0x00);
	rumboot_putstring("CHECK SINGLE BIT ERROR\n");
	check_emi(ADDR_SE_SRAM, long_tpl, CONST_1, EVENT_INJECT_ERR_S);
	rumboot_putstring("CHECK DOUBLE BIT ERROR\n");
    check_emi(ADDR_DE_SRAM, long_tpl, CONST_1, EVENT_INJECT_ERR_D);

    rumboot_putstring("TEST OK!\n");

    return TEST_OK;
}

