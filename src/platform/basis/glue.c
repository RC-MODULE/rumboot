#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
    return 0;
}

#define DGPIO_LOW_BASE      0x01044000
#define DGPIO_HIGH_BASE     0x01045000
#define DIR                 0x0400
#define DATA                0x03FC

void rumboot_platform_setup() {
    iowrite32(0xff, DGPIO_LOW_BASE + DIR);
    iowrite32(0x00, DGPIO_LOW_BASE + DATA);
    iowrite32(0xff, DGPIO_HIGH_BASE + DIR);
    iowrite32(0x00, DGPIO_HIGH_BASE + DATA);
    rumboot_platform_raise_event(EVENT_PERF, 0);
}

void rumboot_platform_raise_event(enum rumboot_simulation_event event, uint8_t arg)
{
    iowrite32(arg, DGPIO_HIGH_BASE + DATA);
    iowrite32(0, DGPIO_LOW_BASE + DATA);
    iowrite32((event << 1) | 1, DGPIO_LOW_BASE + DATA);
}

void rumboot_platform_putchar(uint8_t c)
{
    rumboot_platform_raise_event(EVENT_STDOUT, c);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
    iowrite32(0x00, DGPIO_HIGH_BASE + DIR);
    rumboot_platform_raise_event(EVENT_STDIN, 0x0);
    uint8_t ret = ioread32(DGPIO_HIGH_BASE + DATA) & 0xff;
    iowrite32(0xff, DGPIO_HIGH_BASE + DIR);
    return ret;
}
