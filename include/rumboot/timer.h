#ifndef TIMER__
#define TIMER__

#include <stdint.h>
#include <rumboot/platform.h>

inline void udelay(uint32_t udelay) {

    uint32_t start = rumboot_platform_get_uptime();
    while ((rumboot_platform_get_uptime() - start) < udelay);
};

inline void mdelay(uint32_t mdelay) {

  udelay(mdelay * 1000);
};

#endif
