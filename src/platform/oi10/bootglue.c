#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/ppc470s/mmu.h>
#include <platform/devices/mpic128.h>
#include <rumboot/boot.h>


void rumboot_platform_read_config(struct rumboot_config *conf)
{

}

void rumboot_platform_selftest(struct rumboot_config *conf)
{
        /* Execute selftest routines */
}

const struct rumboot_bootsource *rumboot_platform_get_bootsources()
{
    return NULL;
}

bool rumboot_platform_check_entry_points(struct rumboot_bootheader *hdr)
{
	/* Any entry point is okay */
	return true;
}

int rumboot_platform_exec(struct rumboot_bootheader *hdr)
{
	/* No-op, this chip has only one core */
    return 0;
}

void *rumboot_platform_get_spl_area(size_t *size)
{
  *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
  return (void *) &rumboot_platform_spl_start;
}
