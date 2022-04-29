#ifndef TIMER__
#define TIMER__

#include <stdint.h>
#include <rumboot/platform.h>

#ifndef RUMBOOT_REMOTE
static inline void udelay(uint32_t us)
{
  uint32_t start = rumboot_platform_get_uptime();
  while ((rumboot_platform_get_uptime() - start) < us)
    ;
};
#else
#include <platform/vl_api.h>
extern struct vl_instance *g_vl_instance;
extern uint64_t g_ticks_per_us;

static inline void udelay(uint32_t us)
{
  uint64_t ticks = us * g_ticks_per_us;
  vl_wait(g_vl_instance, 0, ticks);
}
#endif

static inline void mdelay(uint32_t ms)
{
  while (ms--)
    udelay(1000);
};

#define LOOP_UNTIL_TIMEOUT(timeout_us)                  \
  for (uint32_t _start = rumboot_platform_get_uptime(); \
       rumboot_platform_get_uptime() - _start < timeout_us;)

#endif
