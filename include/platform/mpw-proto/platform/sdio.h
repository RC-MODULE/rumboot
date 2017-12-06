#ifndef PLATFORM_SDIO__
#define PLATFORM_SDIO__

//offset of afsel reg
#define PL061_AFSEL 0x420

#define SDIO_CLK_FREQ 100

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
