#include <rumboot/bootsrc/physmap.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/macros.h>
#include <string.h>
#include <stddef.h>

static bool physmap_init(const struct rumboot_bootsource *src, void *pdata)
{
        return true;
}

static void physmap_deinit(const struct rumboot_bootsource *src, void *pdata)
{
}

static size_t physmap_read(const struct rumboot_bootsource *src, void *pdata, void *to, size_t offset, size_t length)
{
        uint64_t *dest = to;
        volatile uint64_t *from = (void *) (src->base + offset);
        size_t i;
        for (i = offset; i < offset + length; i = i + 8) {
                *dest++ = *from++;
        }
        return length;
}

const struct rumboot_bootmodule g_bootmodule_physmap = {
        .init		= physmap_init,
        .deinit		= physmap_deinit,
        .read		= physmap_read,
        .align		= 8,
        .privdatalen	= sizeof(struct physmap_private_data)
};
