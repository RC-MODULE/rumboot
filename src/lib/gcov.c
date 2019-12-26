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
	rumboot_printf("FLUSG!\n");
	__gcov_flush();
	rumboot_printf("FLUSG!\n");
#endif
}

#if 0
struct gcov_info;
static unsigned int gcov_version;
void __gcov_init(struct gcov_info *info)
{
	if (gcov_version == 0) {
			/*
			 * Printing gcc's version magic may prove useful for debugging
			 * incompatibility reports.
			 */
			gcov_version = gcov_info_version(info);
			rumboot_printf("gcov: version magic: 0x%x\n", gcov_version);
	}
	gcov_info_link(info);
}

#endif