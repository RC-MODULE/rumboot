#ifndef PLATFORM_SDIO__
#define PLATFORM_SDIO__

#define PL061_AFSEL 0x420

#define SDIO_CLK_DIV_400KHz   124
#define SDIO_CLK_DIV_10MHz    4
#define SDIO_CLK_DIV_50MHz    0

#include <rumboot/io.h>
#include <platform/devices.h>

/**
 * [enable_gpio_for_SDIO description]
 */
void enable_gpio_for_SDIO()
{
    uint8_t afsel = ioread32(LSIF1_GPIO7__ + PL061_AFSEL) | 0b11110;

    iowrite32(afsel, LSIF1_GPIO7__ + PL061_AFSEL);
}




#endif
