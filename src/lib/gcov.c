#include <stdint.h>
#include <string.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <stdlib.h>
#include <gcov.h>

void _init()
{
#ifdef RUMBOOT_COVERAGE
	rumboot_printf("gcov: Linking cov info structures, this may take a while.\n");
#endif
}

void _fini()
{
#if (!defined(RUMBOOT_ONLY_STACK)) && defined(RUMBOOT_COVERAGE)
	rumboot_printf("gcov: Dumping coverage...\n");
	struct gcov_info *info = gcov_info_next(NULL);
	char tmp[8192];
	uint32_t len;
	while(info) {
		len = gcov_convert_to_gcda(tmp, info);
		const char *file = gcov_info_filename(info);
		rumboot_platform_dump_region(file, (uint32_t) tmp, len);
		info = gcov_info_next(info);
	}
#endif
}

struct gcov_info;
void __gcov_init(struct gcov_info *info)
{
	static unsigned int gcov_version;
	if (gcov_version == 0) {
			gcov_version = gcov_info_version(info);
			/*
			 * Printing gcc's version magic may prove useful for debugging
			 * incompatibility reports.
			 */
			rumboot_printf("gcov: version magic: 0x%x\n", gcov_version);
	}
	rumboot_printf("%s\n", gcov_info_filename(info));
	gcov_info_link(info);
}
