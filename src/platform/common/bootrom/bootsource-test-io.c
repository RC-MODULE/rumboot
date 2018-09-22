#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <stdbool.h>
#include <alloca.h>


#ifndef SIZE
	#define SIZE 4096 * 1024
#endif

int check_read(const struct rumboot_bootsource *src, void *pdata, void *dst, size_t offset, size_t length, size_t expected)
{
	dbg_boot(src, "reading %d from offset %d, expecting %d", length, offset, expected);
	if (!bootsource_init(src, pdata)) {
		dbg_boot(src, "Initialization failed ;(");
		return 1;
	}

	size_t ret = bootsource_read(src, pdata, dst, offset, length);
	if (ret != expected) {
		dbg_boot(src, "Expected to read %d, but read only %d bytes", expected, ret);
		return 1;
	}

	/*
	rumboot_platform_check_random(dst, ret);
	*/

	bootsource_deinit(src, pdata);
	return 0;
}

int main()
{
	char pdata[512];
	#ifndef RUMBOOT_NATIVE
    	struct rumboot_bootheader *hdr = (struct rumboot_bootheader *)&rumboot_platform_spl_start;
	#else
		struct rumboot_config conf;
		rumboot_platform_read_config(&conf);
    	struct rumboot_bootheader *hdr = malloc(SIZE);
	#endif

	const struct rumboot_bootsource *src = &rumboot_platform_get_bootsources()[SOURCE];
	rumboot_printf("Hello, I'm bootsource io test for %d\n", SOURCE);
	rumboot_printf("This source blocksize is %d \n", src->plugin->align);

	int errors = 0;

	if (!src->plugin->align) {
		/* For plugins with no fixed blocksize we just do some reads and check the data */
		errors += check_read(src, pdata, hdr, 0, 16, 16);
		errors += check_read(src, pdata, hdr, 0, 17, 17);
		errors += check_read(src, pdata, hdr, 0, 1, 1);
		errors += check_read(src, pdata, hdr, 0, 2, 2);
		errors += check_read(src, pdata, hdr, 0, 3, 3);
		errors += check_read(src, pdata, hdr, 0, 4, 4);
	} else {
		/* For plugins with fixed blocksize we test handling of incorrect alignment in calls*/
		errors += check_read(src, pdata, hdr, 0, src->plugin->align, src->plugin->align);
		errors += check_read(src, pdata, hdr, 0, src->plugin->align - 1, 0);
		errors += check_read(src, pdata, hdr, 0, src->plugin->align + 1, src->plugin->align);
		errors += check_read(src, pdata, hdr, 0, src->plugin->align*2 + 1, src->plugin->align*2);
		errors += check_read(src, pdata, hdr, 0, src->plugin->align * 2, src->plugin->align * 2);
		errors += check_read(src, pdata, hdr, 0, src->plugin->align * 2 - 1, src->plugin->align);
	}

	/* TODO: Check reads without init */

	/* Return the number of failed test cases */
	return errors;
}
