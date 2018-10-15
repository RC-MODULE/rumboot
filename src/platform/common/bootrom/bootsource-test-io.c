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

__attribute__((packed)) union udata  {
	uint32_t u32;
	uint8_t  u8[4];
} ;

static int check_data(void *ptr, size_t offset, size_t len)
{
    int i;
	int errors = 0;
	int pos = 0;
	uint8_t *p = ptr;
    for (i = offset; i < offset + len; i++)
	{
		union udata u;
		u.u32 = i / 4;

		if (p[pos] != u.u8[i % 4]) {
			rumboot_printf("bytecheck: expected: %x got %x at pos %d (offset %d)\n", u.u8[i % 4], p[pos], pos, i);
			errors++;
		}
		pos++;
	}

	return errors;
}

static int check_read(const struct rumboot_bootsource *src, void *pdata, void *dst, size_t offset, size_t length, size_t expected)
{
	dbg_boot(src, "reading %d from offset %d, expecting %d", length, offset, expected);
	if (!bootsource_init(src, pdata)) {
		dbg_boot(src, "Initialization failed ;(");
		return 1;
	}

	size_t ret = bootsource_read(src, pdata, dst, src->offset + offset, length);
	if (ret == 0 && offset % src->plugin->align) {
		/* Caught misaligned read */
	 	return 0;
	}

	if (ret != expected) {
		dbg_boot(src, "Expected to read %d, but read only %d bytes", expected, ret);
		return 1;
	}

	int errors = check_data(dst, offset, ret);

	bootsource_deinit(src, pdata);
	return errors;
}


int check_once_from_offset(const struct rumboot_bootsource *src, void *pdata, void *hdr, size_t offset)
{
	int errors = 0;
	if (!src->plugin->align) {
		/* For plugins with no fixed blocksize we just do some reads and check the data */
		errors += check_read(src, pdata, hdr, offset, 16, 16);
		errors += check_read(src, pdata, hdr, offset, 17, 17);
		errors += check_read(src, pdata, hdr, offset, 1, 1);
		errors += check_read(src, pdata, hdr, offset, 2, 2);
		errors += check_read(src, pdata, hdr, offset, 3, 3);
		errors += check_read(src, pdata, hdr, offset, 4, 4);
	} else {
		/* For plugins with fixed blocksize we test handling of incorrect alignment in calls*/
		errors += check_read(src, pdata, hdr, offset, src->plugin->align, src->plugin->align);
		errors += check_read(src, pdata, hdr, offset, src->plugin->align - 1, 0);
		errors += check_read(src, pdata, hdr, offset, src->plugin->align + 1, src->plugin->align);
		errors += check_read(src, pdata, hdr, offset, src->plugin->align*2 + 1, src->plugin->align*2);
		errors += check_read(src, pdata, hdr, offset, src->plugin->align * 2, src->plugin->align * 2);
		errors += check_read(src, pdata, hdr, offset, src->plugin->align * 2 - 1, src->plugin->align);
	}
	return errors;
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

	/* Normal aligned reads */
	errors += check_once_from_offset(src, pdata, hdr, 0);
	/* Check misaligned reads */
	errors += check_once_from_offset(src, pdata, hdr, src->plugin->align + 1);
	/* Random reads */
	errors += check_once_from_offset(src, pdata, hdr, (src->plugin->align ? src->plugin->align : 33));

	/* TODO: Check reads without init */
	rumboot_printf("total errors: %d\n", errors);
	/* Return the number of failed test cases */
	return (errors == 0) ? 0 : 1;
}
