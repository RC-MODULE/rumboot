#include <stdarg.h>
#include <stdint.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <devices/gic.h>
#include <unistd.h>
#include <rumboot/rumboot.h>
#include <rumboot/printf.h>
#include <platform/easynmc/easynmc.h>

void  rumboot_platform_putchar(uint8_t c)
{
        eputc(easynmc_stdout, c);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
        return egetc(easynmc_stdin);
}

uint32_t rumboot_platform_get_uptime()
{
        return 0;
}

void rumboot_platform_setup()
{

}
