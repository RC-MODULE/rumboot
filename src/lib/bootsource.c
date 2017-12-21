#include <rumboot/bootheader.h>
#include <rumboot/bootsource.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <string.h>


static bool load_img(const struct rumboot_bootsource *src, void *pdata)
{
	int ret;
	char tmp[512];
	uint32_t *read_from = (uint32_t *) (src->base + src->offset);
	struct rumboot_bootheader* hdr = (struct rumboot_bootheader*) &tmp;
	uint32_t *data_ptr = (uint32_t *) &rumboot_platform_spl_start + 66;

	ret = src->read(pdata, &tmp, read_from);

	if (ret < 0) {
		rumboot_printf("boot: read from %s failed.\n", src->name);
		return false;
	}

	if (rumboot_bootimage_check_header(hdr, (void **)&data_ptr) < 0) {
		rumboot_printf("boot: validation of header failed!\n");
		return false;
	}

	//Here i must write IMG to IM0
	int i;
	for(i=0; i<hdr->datalen; i++) {

		ret = src->read(pdata, &tmp, read_from);
		memcpy((uint32_t*) hdr->entry_point[0], &tmp, ret);

		i += ret;
	}

	return true;
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

		rumboot_printf("boot: loaded an image from %s, okay, ", src->name);
	} else {
		rumboot_printf("boot: no valid image found in %s\n\n", src->name);
		goto deinit;
	}

	rumboot_printf("and... execute!\n");

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
