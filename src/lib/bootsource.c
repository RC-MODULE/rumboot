#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <string.h>


bool bootsource_init(const struct rumboot_bootsource *src, void *pdata)
{
        bool ret = false;

        ret = true;
        if (src->enable) {
                ret = src->enable(src, pdata);
        }

        if (ret) {
                dbg_boot(src, "Enabled successfully");
        } else {
                dbg_boot(src, "Failed to enable or media not present");
                goto gpio_deinit;
        }

        ret = true;
        if (src->plugin->init) {
                ret = src->plugin->init(src, pdata);
        }

        if (ret) {
                dbg_boot(src, "Initialized");
        } else {
                dbg_boot(src, "Initialization failed");
                goto deinit;
        }

        return true;

deinit:
        if (src->plugin->deinit) {
                src->plugin->deinit(src, pdata);
        }
gpio_deinit:
        if (src->disable) {
                src->disable(src, pdata);
        }
        return ret;
}


size_t bootsource_read(const struct rumboot_bootsource *src, void *pdata, void *dest, size_t offset, size_t len)
{
        if (src->plugin->align) {
                if (offset % src->plugin->align) {
                        dbg_boot(src, "WARN: Attempting to do misaligned read");
                        return 0;
                }
                size_t misalign = len % src->plugin->align;
                if (misalign) {
                        len -= misalign;
                        dbg_boot(src, "WARN: Reducing read size to match alignment requirements");
                }
        }
        return src->plugin->read(src, pdata, dest, offset, len);
}

void bootsource_deinit(const struct rumboot_bootsource *src, void *pdata)
{
        if (src->plugin->deinit) {
                src->plugin->deinit(src, pdata);
        }
        if (src->disable) {
                src->disable(src, pdata);
        }
}

static int load_img(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *hdr, size_t maxsize)
{
//	hdr = (struct rumboot_bootheader *) malloc(128000);
        void *data;

        size_t toread = src->plugin->align;

        if (!toread) {
                toread = sizeof(*hdr);
        }
        /* Now, carefully handle alignment. */
        if ((sizeof(*hdr) > toread)) {
                toread = sizeof(*hdr);
                size_t misalign = (toread % src->plugin->align);
                if (misalign) {
                        toread += src->plugin->align - misalign;
                }
        }

        //Read header!
        size_t count = bootsource_read(src, pdata, hdr, 0, toread);
        if (count < toread) {
                return false;
        }
        hdr->device = src;

        ssize_t len = rumboot_bootimage_check_header(hdr, &data);

        if (len < 0) {
                dbg_boot(src, "Error validating header: %s", rumboot_strerror(-len));
                return -len;
        }

        /* TODO: Read and check next image magic */
        size_t misalign = len % src->plugin->align;
        if (misalign) {
                len += src->plugin->align - misalign;
        }

        if (len + sizeof(*hdr) > maxsize) {
                dbg_boot(src, "Image exceeds maximum possible size");
                return -ETOOBIG;
        }

        bootsource_read(src, pdata, &hdr->data[count - sizeof(*hdr)], count, len);
        return 0 == rumboot_bootimage_check_data(hdr);
}

bool bootsource_try_single(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *dst, size_t maxsize)
{
        if (!bootsource_init(src, pdata)) {
                return false;
        }

        int ret = load_img(src, pdata, dst, maxsize);

        if (ret) {
                dbg_boot(src, "Image validated, executing...");
        } else {
                dbg_boot(src, "No bootable image found here");
        }

        if (ret) {
                rumboot_bootimage_exec(dst);
        }

        bootsource_deinit(src, pdata);

        return ret;
}

bool bootsource_try_chain(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *hdr, size_t maxsize)
{
        bool ret = false;

        while (src->name) {
                ret = bootsource_try_single(src, pdata, hdr, maxsize);
                src++;

                if (ret) {
                        break;
                }
        }
        return ret;
}
