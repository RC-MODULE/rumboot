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
        uint32_t *ptr = (void *) src->base;
        uint32_t i = ptr[0];
        rumboot_printf("%d\n", i);
        /* We can't do memcpy here, since we risk of getting MCE */
        memcpy(to, (void *) (uintptr_t) (src->offset + offset), length);
        return length;
}

const struct rumboot_bootmodule g_bootmodule_physmap = {
        .init		= physmap_init,
        .deinit		= physmap_deinit,
        .read		= physmap_read,
        .privdatalen	= sizeof(struct physmap_private_data)
};
