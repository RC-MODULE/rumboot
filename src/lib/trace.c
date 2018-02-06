#include <stdio.h> 
#include <rumboot/platform.h>
#include <rumboot/printf.h>
/* These will be used when call tracing is enabled */

void __attribute__((no_instrument_function)) __cyg_profile_func_enter(void *this_fn, void *call_site)
{
#ifdef RUMBOOT_TRACE
    rumboot_platform_runtime_info.nestlevel++;
    rumboot_platform_trace(this_fn);
#endif

#ifdef RUMBOOT_PROFILE
    rumboot_platform_perf_func(this_fn);
#endif
}

void __attribute__((no_instrument_function))
    __cyg_profile_func_exit(void *this_fn, void *call_site)
{

#ifdef RUMBOOT_TRACE
    rumboot_platform_runtime_info.nestlevel--;
#endif

#ifdef RUMBOOT_PROFILE
    rumboot_platform_perf_func(NULL);
#endif

}
