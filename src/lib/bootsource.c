#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <string.h>


static size_t round_up_to_align(size_t value, size_t align)
{
        if (!align) {
                return value;
        }
        size_t misalign = value % align;
        if (misalign) {
                value += align - misalign;
        }
        return value;
}

static const char *describe_next_step(int code)
{
        if (code > 0) {
                return "boot from another device";
        } else if (code == 0) {
                return "boot from next device";
        } else {
                return "switch to host mode";
        }
}
bool bootsource_init(const struct rumboot_bootsource *src, void *pdata)
{
        bool ret = false;

        ret = true;
        if (src->enable) {
                ret = src->enable(src, pdata);
        }

        if (!ret) {
                dbg_boot(src, "Failed to enable or media not present");
                goto gpio_deinit;
        }

        ret = true;
        if (src->plugin->init) {
                ret = src->plugin->init(src, pdata);
        }

        if (!ret) {
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

int bootsource_load_img(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *hdr, size_t maxsize, size_t *offset)
{
        void *data;

        size_t toread = src->plugin->align;

        if (!toread) {
                toread = sizeof(*hdr);
        }
        /* Now, carefully handle alignment. */
        if ((sizeof(*hdr) > toread)) {
                toread = sizeof(*hdr);
                toread = round_up_to_align(toread, src->plugin->align);
        }

        //Read header!
        size_t count = bootsource_read(src, pdata, hdr, *offset, toread);
        if (count < toread) {
                return -EIO;
        }
        hdr->device = src;
        *offset += count;

        ssize_t len = rumboot_bootimage_check_header(hdr, &data);

        if (len < 0) {
                return len;
        }

        len = round_up_to_align(len, src->plugin->align);

        if (len + sizeof(*hdr) > maxsize) {
                return -ETOOBIG;
        }


        bootsource_read(src, pdata, &hdr->data[count - sizeof(*hdr)], *offset, len);
        *offset += len;
        /* No error handling here, let crc32 sort everything out */
        return rumboot_bootimage_check_data(hdr);
}

int bootsource_try_source_once(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *dst, size_t maxsize, size_t *offset)
{
        int ret = 0;

        if (!bootsource_init(src, pdata)) {
                return false;
        }

        *offset = round_up_to_align(*offset, src->plugin->align);
        dbg_boot(src, "Reading image from offset %ld", *offset);
        ret = bootsource_load_img(src, pdata, dst, maxsize, offset);
        if (ret != 0) {
                dbg_boot(src, "Error: %s (%d)", rumboot_strerror(-ret), -ret);
                ret = -EBADSOURCE;
                goto bailout;
        }

        dbg_boot(src, "Image validated, executing...");
        ret = rumboot_platform_exec(dst);
        dbg_boot(src, "Back in rom, code %d, will now %s", ret, describe_next_step(ret));
bailout:
        bootsource_deinit(src, pdata);
        return ret;
}

int bootsource_try_single(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *dst, size_t maxsize, size_t *offset)
{
        int ret;

        do {
                ret = bootsource_try_source_once(src, pdata, dst, maxsize, offset);
        } while (ret == 0);
        return ret;
}

static const struct rumboot_bootsource *find_source_by_id(int bootid)
{
        const struct rumboot_bootsource *src = rumboot_platform_get_bootsources();
        /* We can't index, or we risk getting out-of-bounds */
        int i = 0;
        do {
                if (i++ == bootid) {
                        break;
                }
                src++;
        } while (src->name);
        if (!src->name) {
                return NULL;
        }
        return src;
}

void bootsource_try_chain(void *pdata, struct rumboot_bootheader *hdr, size_t maxsize)
{
        int ret;
        const struct rumboot_bootsource *src = rumboot_platform_get_bootsources();

        while (src && src->name) {
                size_t offset = src->offset;
                ret = 0;
                ret = bootsource_try_single(src, pdata, hdr, maxsize, &offset);

                if (ret > 0) {
                        src = find_source_by_id(ret - 1);
                        continue;
                }

                if ((ret < 0) && (ret != -EBADSOURCE)) {
                        break;
                }

                src++;
        }
}


int bootsource_try_by_id(int bootid, void *pdata, struct rumboot_bootheader *hdr, size_t maxsize)
{
        const struct rumboot_bootsource *src = find_source_by_id(bootid);

        if (src) {
                size_t offset = src->offset;
                return bootsource_try_single(src, pdata, hdr, maxsize, &offset);
        }
        return -EBADSOURCE;
}
