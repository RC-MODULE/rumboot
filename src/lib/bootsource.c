#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <string.h>



static bool load_img(const struct rumboot_bootsource *src, void *pdata)
{
	struct rumboot_bootheader *hdr = (struct rumboot_bootheader *) &rumboot_platform_spl_start;
	void *data;

	size_t toread = src->plugin->align;
	if (!toread)
		toread = sizeof(*hdr);

	//Read header!
	size_t count = src->plugin->read(src, pdata, hdr, 0, toread);
	if(count != toread) {
		return false;
	}
	hdr->device = src;

	ssize_t len = rumboot_bootimage_check_header(hdr, &data);

	if (len > 0) {
		src->plugin->read(src, pdata, hdr->data, count, len);
	} else {
        dbg_boot(src, "Error validating header: %s", rumboot_strerror(-len));
		return false;
	}

	return (0 == rumboot_bootimage_check_data(hdr));
}

bool bootsource_try_single(const struct rumboot_bootsource *src, void *pdata)
{
	bool ret = false;

	//DBG_ASSERT(PRIVDATA_SIZE < src->privdatalen, "FATAL: Increase PRIVDATA_SIZE length");

	struct rumboot_bootheader *dst = (struct rumboot_bootheader *)&rumboot_platform_spl_start;

	ret = true;
    if (src->enable)
		ret = src->enable(src, pdata);

	if (ret) {
		dbg_boot(src, "Enabled successfully");
	} else {
		dbg_boot(src, "Failed to enable or media not present");
		goto gpio_deinit;
	}

	ret = true;
    if (src->plugin->init)
		ret = src->plugin->init(src, pdata);

	if (ret) {
		dbg_boot(src, "Initialized");
	} else {
		dbg_boot(src, "Initialization failed");
		goto deinit;
	}

	ret = load_img(src, pdata);
	if (ret) {
		dbg_boot(src, "Image validated, executing...");
	} else {
		dbg_boot(src, "No bootable image found here");
	}

deinit:
	if (src->plugin->deinit)
		src->plugin->deinit(src, pdata);
gpio_deinit:
	if (src->disable)
		src->disable(src, pdata);

	if (ret) {
		rumboot_bootimage_exec(dst);
	}

	return ret;
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
