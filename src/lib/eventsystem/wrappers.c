#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/memfill.h>
#include <rumboot/io.h>
#include <rumboot/macros.h>

__attribute__((no_instrument_function)) void rumboot_platform_trace(void *pc)
{
	uint32_t data[] = { (uint32_t) (pc), rumboot_platform_runtime_info->nestlevel};
	rumboot_platform_event_raise(EVENT_TRACE, data, ARRAY_SIZE(data));
}

__attribute__((no_instrument_function)) void rumboot_platform_perf(const char *tag)
{
	uint32_t data[] = { (uint32_t) tag };
	rumboot_platform_event_raise(EVENT_PERF, data, ARRAY_SIZE(data));
}

__attribute__((no_instrument_function)) void rumboot_platform_perf_func(void *addr)
{
	uint32_t data[] = { (uint32_t) addr };
	rumboot_platform_event_raise(EVENT_PERF_FUNC, data, ARRAY_SIZE(data));
}

void rumboot_platform_request_file(const char *plusarg, uint32_t addr)
{
	uint32_t data[] = { (uint32_t) plusarg, addr };
	rumboot_platform_event_raise(EVENT_UPLOAD, data, ARRAY_SIZE(data));
}

void rumboot_platform_sim_save(const char *filename)
{
	uint32_t data[] = { (uint32_t) filename };
	rumboot_platform_event_raise(EVENT_SIM_SAVE, data, ARRAY_SIZE(data));
}

void rumboot_platform_sim_restore(const char *filename)
{
	uint32_t data[] = { (uint32_t) filename };
	rumboot_platform_event_raise(EVENT_SIM_RESTORE, data, ARRAY_SIZE(data));
}

void rumboot_platform_dump_region(const char *filename, uint32_t addr, uint32_t len)
{
	uint32_t data[] = { (uint32_t) filename, addr, len };
	rumboot_platform_event_raise(EVENT_DOWNLOAD, data, ARRAY_SIZE(data));
}

void rumboot_platform_store_gcda(const char *filename, uint32_t addr, uint32_t len)
{
	uint32_t data[] = { (uint32_t) filename, addr, len };
	rumboot_platform_event_raise(EVENT_GCDA, data, ARRAY_SIZE(data));
}

void rumboot_platform_relocate_runtime(uint32_t addr)
{
    rumboot_platform_event_raise(EVENT_RELOCATE_RUNTIME, &addr, 1);
}
