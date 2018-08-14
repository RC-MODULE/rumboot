#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <string.h>



static bool load_img(const struct rumboot_bootsource *src, void *pdata)
{
	uint8_t *read_from = (uint8_t *)(src->base + src->offset);
	uint8_t *write_to = (uint8_t *)&rumboot_platform_spl_start;
	char tmp[512];

	//Read header!
	size_t count = src->read(pdata, &tmp, read_from);
	if(count < 0)
		return false;

	struct rumboot_bootheader *hdr = (struct rumboot_bootheader *)&tmp;
	size_t img_size = hdr->datalen + 66;

	//Reed data image
	while (img_size > 0) {

		count = src->read(pdata, &tmp, read_from);

		if (count < 0) {
			rumboot_printf("boot: read from %s failed.\n", src->name);
			break;
		}
		memcpy(write_to, &tmp, count);

		img_size	-= count;
		write_to	+= count;
		read_from	+= count;
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
