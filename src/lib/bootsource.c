#include <rumboot/bootheader.h>
#include <rumboot/bootsource.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

/* TODO: Move to top */
#define PRIVDATA_SIZE 128

bool bootsource_try_single(const struct rumboot_bootsource *src)
{
    bool ret = false;

    rumboot_printf("boot: trying to boot from  \n", src->name);

    char pdata[PRIVDATA_SIZE];
    DBG_ASSERT(PRIVDATA_SIZE < src->privdatalen, "FATAL: Increase PRIVDATA_SIZE length");

    ret = src->init(src, (struct pdata*) pdata);
    if (ret) {
      rumboot_printf("boot: %s initialized, okay\n", src->name);
    }
    else {
        rumboot_printf("boot: %s initialization failed\n", src->name);
        return ret;
    }

    ret = src->load_img((struct pdata*) pdata);

    if (ret) {
      rumboot_printf("boot: loaded an image from %s, okay\n", src->name);
    }
    else {
        rumboot_printf("boot: no valid image found in %s\n", src->name);
        return ret;
    }

    struct rumboot_bootheader *dst = (struct rumboot_bootheader*) &rumboot_platform_spl_start;

    /* At this point we have a valid image in IM0 */
    rumboot_bootimage_exec(dst);

    /* Should not happen */
    return false;
}

bool bootsource_try_chain(const struct rumboot_bootsource *src)
{
    bool ret = false;

    while (src->name) {
        ret = bootsource_try_single(src);
        src++;
        if (ret)
            break;
    }
    return ret;
}
