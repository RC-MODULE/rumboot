#ifndef TIMER__
#define TIMER__

#include <stdint.h>
#include <rumboot/platform.h>

static inline void udelay(uint32_t us) {

    uint32_t start = rumboot_platform_get_uptime();
    while ((rumboot_platform_get_uptime() - start) < us);
};

static inline void mdelay(uint32_t ms) {

  udelay(ms * 1000);
};

#endif
