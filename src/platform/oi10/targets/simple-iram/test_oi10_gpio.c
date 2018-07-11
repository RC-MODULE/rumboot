#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

static int check_gpio_regs(uint32_t base_addr)
{
}

int main(void)
{
    rumboot_putstring("Check registers GPIO_0 \n");
    check_gpio_regs(GPIO_0_BASE);

    return 0;
}
