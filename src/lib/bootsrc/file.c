#include <rumboot/bootsrc/file.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/macros.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>

static bool file_init(const struct rumboot_bootsource *src, void *pdata)
{
        struct file_private_data *fpd = pdata;
        fpd->filefd = fopen(src->name, "r");
        return (fpd->filefd);
}

static void file_deinit(const struct rumboot_bootsource *src, void *pdata)
{
        struct file_private_data *fpd = pdata;
        if (fpd->filefd)
            fclose(fpd->filefd);
}

static size_t file_read(const struct rumboot_bootsource *src, void *pdata, void *to, size_t offset, size_t length)
{
        struct file_private_data *fpd = pdata;
        return fread(to, 1, length, fpd->filefd);
}

struct rumboot_bootmodule g_bootmodule_file = {
        .align          = 128,
        .init		= file_init,
        .deinit		= file_deinit,
        .read		= file_read,
        .privdatalen	= sizeof(struct file_private_data)
};
