#include <stdio.h>
#include <stdint.h>

#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <platform/devices/greth.h>

uint32_t test_data_src[] __attribute__((section(".data")))__attribute__((aligned(0x4))) =
        {
                [0] = 0x00000000,
                [1] = 0x11111111,
                [2] = 0x22222222,
                [3] = 0x33333333,
                [4] = 0x44444444,
                [5] = 0x55555555,
                [6] = 0x66666666,
                [7] = 0x77777777
        };

uint32_t test_data_dst[sizeof(test_data_src)] __attribute__((section(".data")));


bool mdio_check(uint32_t base_addr)
{
    uint32_t buf;
    rumboot_printf("MDIO check for ETH%d(0x%x)\n", base_addr==GRETH_0_BASE ? 0 : 1, base_addr);
    if (!mdio_write( base_addr, 0, 0, 0xABBA)) return 1;
    buf = (uint32_t)mdio_read( base_addr, 0, 0);
    rumboot_printf("PHY(0x%x) = 0x%x\n", 0, buf);
    return 0;
}

int main(void)
{

    //if (mdio_check(GBIT_GRETH_0_BASE)) return 1;
    //if (mdio_check(GBIT_GRETH_1_BASE)) return 1;
    if (!greth_mem_copy(GRETH_0_BASE, test_data_src, test_data_dst, sizeof(test_data_dst))) return 1;
    return 0;
}
