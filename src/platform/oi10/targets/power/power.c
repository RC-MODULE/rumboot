#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>

#include <regs/regs_mkio.h>
#include <regs/regs_spi.h>

#include <devices/mkio.h>
#include <devices/gpio_pl061.h>
#include <devices/pl022.h>
#include <devices/pl022_flash.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/regs/fields/dma2plb6.h>
#include <platform/regs/regs_dma2plb6.h>
#include <platform/devices/hscb.h>
#include <platform/devices/dma2plb6.h>
#include <platform/devices/emi.h>
#include <platform/test_assert.h>
#include <platform/devices/greth.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <rumboot/hexdump.h>

extern void test_oi10_power();

int main(void)
{
    rumboot_printf("Running power test\n");
    uint32_t start = rumboot_platform_get_uptime();
    test_oi10_power();
    uint32_t end = rumboot_platform_get_uptime();
    rumboot_printf("Iteration took %u us\n", end - start);

    return 0;
}
