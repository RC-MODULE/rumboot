#include <stdarg.h>
#include <stdint.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <devices/gic.h>
#include <unistd.h>
#include <rumboot/rumboot.h>
#include <rumboot/printf.h>

void  rumboot_platform_putchar(uint8_t c)
{

}

int rumboot_platform_getchar(uint32_t timeout_us)
{
        return 0;
}



uint32_t rumboot_platform_get_uptime()
{
        return 0;
}

void rumboot_platform_setup()
{

}
