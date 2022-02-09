#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/macros.h>
#include <rumboot/xmodem.h>
#include <rumboot/hexdump.h>
#include <algo/crc32.h>

__attribute__((no_instrument_function)) void rumboot_platform_trace(void *pc)
{
//	uint32_t data[] = { (uint32_t) (pc), rumboot_platform_runtime_info->nestlevel};
//	rumboot_platform_event_raise(EVENT_TRACE, data, ARRAY_SIZE(data));
}

__attribute__((no_instrument_function)) void rumboot_platform_perf(const char *tag)
{
//	rumboot_printf("PERF: %s @ %u us\n", tag, rumboot_platform_get_uptime());
}

__attribute__((no_instrument_function)) void rumboot_platform_perf_func(void *addr)
{
	rumboot_printf("PERF_FUNC: 0x%x @ %u us\n", addr, rumboot_platform_get_uptime());
}

struct mcpy_desc {
	uint32_t crc32; 
	void *to;
	size_t length;
	size_t maxlength;
};

static void memcpy_cb(size_t curpos, void *data, size_t length, void *arg)
{
	struct mcpy_desc *dsc = arg;
	char *to = dsc->to;
	size_t tocopy = (length <= dsc->maxlength) ? length : dsc->maxlength;
	dsc->crc32 = crc32(dsc->crc32, data, tocopy);
	dsc->length += tocopy;
	dsc->maxlength -= tocopy;
	memcpy(&to[curpos], data, tocopy);
}
#include <rumboot/timer.h>

uint32_t rumboot_platform_request_file_ex(const char *plusarg, uint32_t addr, uint32_t bufsize)
{
        rumboot_printf("UPLOAD: %s to 0x%x. 'X' for X-modem, 'E' for EDCL\n", plusarg, addr);
 		struct mcpy_desc dsc = {
			 .crc32 = 0,
			 .to = (void *) addr,
			 .length = 0,
			 .maxlength = (bufsize > 0) ? bufsize : -1
		 };
		while(1) {
			int tmp = rumboot_platform_getchar(500);
			if (tmp < 0)
				continue;
			if (tmp == 'X') {
				xmodem_get_async(-1, memcpy_cb, &dsc);
				uint32_t crc = crc32(0, (void *) addr, dsc.length);
				if (crc != dsc.crc32) {
					mdelay(1000);
					rumboot_platform_panic("FATAL: Data upload CRC32 mismatch!");
				}
				break;
			}

			if (tmp == 'E') {
				break;
			}
		}
		return dsc.length;
}


void rumboot_platform_request_file(const char *plusarg, uint32_t addr)
{
	rumboot_platform_request_file_ex(plusarg, addr, 0);
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
	rumboot_printf("DOWNLOAD: %u bytes from 0x%x to %s. 'X' for X-modem, 'E' for EDCL\n", len, addr, filename);
	while(1) {
		char tmp = rumboot_platform_getchar(500);
		if (tmp == 'X') {
			xmodem_send((void *) addr, len);
			break;
		}

		if (tmp == 'E') {
			break;
		}
	}
}

void rumboot_platform_store_gcda(const char *filename, uint32_t addr, uint32_t len)
{
	rumboot_printf("DOWNLOAD: %u bytes from 0x%x to %s\n", len, addr, filename);
	rumboot_platform_dump_region(filename, addr, len);
}

void rumboot_platform_relocate_runtime(uint32_t addr)
{
	rumboot_printf("RUNTIME: Relocated to 0x%x \n", addr);
}

void rumboot_platform_sv_event(const char *name)
{
	/* No - op */
}
