#include <rumboot/bootsrc/boilerplate.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/macros.h>
#include <stddef.h>

#define DEBUG

static bool boilerplate_init(const struct rumboot_bootsource *src, void *pdata)
{
        return true;
}

static void boilerplate_deinit(const struct rumboot_bootsource *src, void *pdata)
{
}

static size_t boilerplate_read(const struct rumboot_bootsource *src, void *pdata, void *to, size_t offset, size_t length)
{
        return 0;
}

const struct rumboot_bootmodule g_bootmodule_boilerplate = {
        .init		= boilerplate_init,
        .deinit		= boilerplate_deinit,
        .read		= boilerplate_read,
        .privdatalen	= sizeof(struct boilerplate_private_data)
};
