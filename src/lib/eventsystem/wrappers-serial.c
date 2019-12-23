#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/memfill.h>
#include <rumboot/io.h>
#include <rumboot/macros.h>
#include <rumboot/xmodem.h>
#include <rumboot/hexdump.h>

__attribute__((no_instrument_function)) void rumboot_platform_trace(void *pc)
{
	uint32_t data[] = { (uint32_t) (pc), rumboot_platform_runtime_info->nestlevel};
	rumboot_platform_event_raise(EVENT_TRACE, data, ARRAY_SIZE(data));
}

__attribute__((no_instrument_function)) void rumboot_platform_perf(const char *tag)
{
	rumboot_printf("PERF: %s @ %u us\n", tag, rumboot_platform_get_uptime());
}

__attribute__((no_instrument_function)) void rumboot_platform_perf_func(void *addr)
{
	rumboot_printf("PERF_FUNC: 0x%x @ %u us\n", addr, rumboot_platform_get_uptime());
}

void rumboot_platform_request_file(const char *plusarg, uint32_t addr)
{
        rumboot_printf("UPLOAD: %s to 0x%x\n", plusarg, addr);
        xmodem_get((void *) addr, -1);
}

void rumboot_platform_sim_save(const char *filename)
{
	/* No - op */
}

void rumboot_platform_sim_restore(const char *filename)
{
	/* No - op */
}

void rumboot_platform_dump_region(const char *filename, uint32_t addr, uint32_t len)
{
	rumboot_printf("DOWNLOAD: %u bytes from 0x%x to %s\n", len, addr, filename);
}

void rumboot_platform_store_gcda(const char *filename, uint32_t addr, uint32_t len)
{
	rumboot_printf("DOWNLOAD: %u bytes from 0x%x to %s\n", len, addr, filename);
	rumboot_hexdump(addr, 512);
	xmodem_send(addr, len);
}

void rumboot_platform_relocate_runtime(uint32_t addr)
{
	rumboot_printf("RUNTIME: Relocated to 0x%x \n", addr);
}

void rumboot_platform_sv_event(const char *name)
{
	/* No - op */
}
