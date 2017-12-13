#include <rumboot/bootheader.h>
#include <rumboot/bootsource.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

static bool load_img(const struct rumboot_bootsource *src, void *pdata)
{
	//If src is sd card we will read for 512 byte
	uint32_t *from = (uint32_t *)(src->base + src->offset);
	struct rumboot_bootheader hdr;

	rumboot_printf("READ FROM: %x TO: %x\n", from, &hdr);

	bool ret = src->read((uint32_t*) &hdr, from, pdata);

#if 0
	if (!rumboot_bootimage_check_header(hdr, ))
		return false;
#endif

	return ret;
}

bool bootsource_try_single(const struct rumboot_bootsource *src, void *pdata)
{
	bool ret = false;

	rumboot_printf("boot: trying to boot from  %s\n", src->name);

	//DBG_ASSERT(PRIVDATA_SIZE < src->privdatalen, "FATAL: Increase PRIVDATA_SIZE length");

	struct rumboot_bootheader *dst = (struct rumboot_bootheader *)&rumboot_platform_spl_start;

	ret = src->init_gpio_mux(pdata);
	if (ret) {
		rumboot_printf("boot: %s, gpio initialized, okay\n", src->name);
	} else {
		rumboot_printf("boot: %s, gpio initialization failed\n", src->name);
		goto gpio_deinit;
	}

	ret = src->init(src, pdata);
	if (ret) {
		rumboot_printf("boot: %s initialized, okay\n", src->name);
	} else {
		rumboot_printf("boot: %s initialization failed\n", src->name);
		goto deinit;
	}

	ret = load_img(src, pdata);
	if (ret) {
		rumboot_printf("boot: loaded an image from %s, okay\n", src->name);
	} else {
		rumboot_printf("boot: no valid image found in %s\n\n", src->name);
		goto deinit;
	}

	/* At this point we have a valid image in IM0 */
	rumboot_bootimage_exec(dst);

  return true;

deinit:
	src->deinit(pdata);
gpio_deinit:
	src->deinit_gpio_mux(pdata);
	return false;
}

bool bootsource_try_chain(const struct rumboot_bootsource *src, void *pdata)
{
	bool ret = false;

	while (src->name) {
		ret = bootsource_try_single(src, pdata);
		src++;

		if (ret)
			break;
	}
	return ret;
}
