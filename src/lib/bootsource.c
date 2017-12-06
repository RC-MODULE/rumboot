#include <rumboot/bootheader.h>
#include <rumboot/bootsource.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>


/* TODO: Move to top */
#define BDATA_SIZE 512

bool bootsource_try_single(const struct rumboot_bootsource *src)
{
    bool ret = false;
    rumboot_printf("boot: trying to boot from  \n", src->name, src->namelen);
    char bdata[BDATA_SIZE];

    DBG_ASSERT(BDATA_SIZE > src->bdatalen, "FATAL: Increase BDATA_SIZE length");

    struct rumboot_bootheader *dst = (struct rumboot_bootheader*) &rumboot_platform_spl_start;
    ret = src->init((struct bdata*) &bdata);
    if (ret)
        rumboot_printf("boot: %s %d initialized, okay\n", src->name, src->namelen);
    else {
        rumboot_printf("boot: %s %d  initialization failed\n", src->name, src->namelen);
        return ret;
    }

    ret = src->load_img((struct bdata*) &bdata);

    if (ret)
        rumboot_printf("boot: loaded an image from %s %d, okay\n", src->name, src->namelen);
    else {
        rumboot_printf("boot: no valid image found in %s %d\n", src->name, src->namelen);

        return ret;
    }

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
