#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <algo/ini.h>
#include <stdlib.h>
#include <rumboot/io.h>
#include <rumboot/pcie_lib.h>

#define MAX_SECTION_LEN 128
#define USERDATA_LEN 1024

struct sectionfilter
{
        const char *section;
        int (*handler)(void *user, const char *section,
                       const char *name, const char *value,
                       int lineno);
        void (*apply)(void *user);
};

int iomem_handler(void *user, const char *section,
                  const char *name, const char *value,
                  int lineno)
{
        uintptr_t dest = strtoul(name, NULL, 16);
        uint32_t v = strtoul(value, NULL, 16);
        rumboot_printf("[%x] <= %x\n", dest, v);
        iowrite32(v, dest);
        return 0;
}

#define PCIE_NUM_BARS 8
struct pcie_barcfg
{
        uintptr_t axi_start;
        uintptr_t aperture;
        uintptr_t type;
        int valid;
};

struct pcie_data
{
        uint32_t speed;
        uint32_t device_id;
        uint32_t revision_id;
        uint32_t subclass_code;
        uint32_t class_code;
        uint32_t state_auto_ack;
        struct pcie_barcfg bars[PCIE_NUM_BARS];
};

#define fill_hex(nm)                                 \
        if (strcmp(name, #nm) == 0)                  \
        {                                            \
                data->nm = strtoul(value, NULL, 16); \
        }

#define fill_bar(bar, nm)                                      \
        if (strcmp(barparam, #nm) == 0)                        \
        {                                                      \
                data->bars[bar].nm = strtoul(value, NULL, 16); \
                data->bars[bar].valid = 1;                     \
        }

int pcie_handler(void *user, const char *section,
                 const char *name, const char *value,
                 int lineno)
{
        struct pcie_data *data = user;

        fill_hex(speed);
        fill_hex(device_id);
        fill_hex(revision_id);
        fill_hex(subclass_code);
        fill_hex(class_code);
        fill_hex(state_auto_ack);

        int bar;
        char tmp[16];
        if (!sscanf(name, "BAR[%d]", &bar))
                return 0;
        int pos = sprintf(NULL, "BAR[%d].", bar);
        char *barparam = &name[pos];
        fill_bar(bar, aperture);
        fill_bar(bar, axi_start);
        fill_bar(bar, type);
}

void pcie_apply(void *user)
{
        struct pcie_data *data = user;
        int ret;
        ret = pcie_init(
            data->speed,
            data->device_id,
            data->revision_id,
            data->subclass_code,
            data->class_code,
            data->state_auto_ack);
        if (ret)
        {
                rumboot_printf("WARNING: PCIe init faied: %d\n", ret);
                return;
        }

        int i;
        for (i = 0; i < PCIE_NUM_BARS; i++)
        {
                if (data->bars[i].valid)
                {
                        pcie_add_bar(i,
                                     data->bars[i].axi_start,
                                     data->bars[i].aperture,
                                     data->bars[i].type);
                }
        }

        if (pcie_link_up()) {
                rumboot_printf("Warning: Failed to bring up link\n");
        }
}

int exit_handler(void *user, const char *section,
                 const char *name, const char *value,
                 int lineno)
{
        if (strcmp(name, "mode") != 0)
                return 0;

        if (strcmp(value, "loop") == 0)
        {
                rumboot_printf("Entering endless loop\n");
                /* Endless loop */
                while (1)
                        ;
                ;
        }

        if (strcmp(value, "next") == 0)
        {
                rumboot_printf("Returning to bootrom to boot from next device\n");
                exit(0);
        }

        if (strcmp(value, "host") == 0)
        {
                rumboot_printf("Will now go back to host mode\n");
                exit(-1);
        }

        if (strcmp(value, "wfiloop") == 0)
        {
                rumboot_printf("Entering WFI loop\n");
                while (1)
                {
                        asm("wfi");
                }
        }

        int jid;
        if (1 == sscanf(value, "jump:%d", &jid))
        {
                rumboot_printf("Jumping to boot device %d\n", jid);
                exit(jid);
        }
}

struct sectionfilter filters[] = {
    {
        .section = "EXIT",
        .handler = exit_handler,
    },
    {
        .section = "PCIe",
        .handler = pcie_handler,
        .apply = pcie_apply,
    },
    {
        .section = "IOMEM",
        .handler = iomem_handler,
        .apply = NULL,
    },
    {/* Sentinel */},
};

static char prevsection[MAX_SECTION_LEN];
static struct sectionfilter *curfilter;

int theini_handler(void *user, const char *section,
                   const char *name, const char *value,
                   int lineno)
{
        rumboot_printf("[%s] : %s=%s\n", section, name, value);

        if (!strlen(prevsection) || strcmp(prevsection, section) != 0)
        {
                bzero(user, USERDATA_LEN);
                if (strlen(prevsection))
                {
                        rumboot_printf("Finished parsing section: %s\n", prevsection);
                }
                if (curfilter && curfilter->apply)
                {
                        curfilter->apply(user);
                }

                if (strlen(section) > MAX_SECTION_LEN)
                {
                        rumboot_platform_panic("Section name %d too long\n", section);
                }

                strcpy(prevsection, section);
                curfilter = filters;
                do
                {
                        if (strcmp(curfilter->section, section) == 0)
                                break;
                        curfilter++;
                } while (curfilter->section);

                if (!curfilter->section)
                {
                        rumboot_printf("Ignoring unknown section: %s\n", section);
                }
                else
                {
                        rumboot_printf("Current filter: %s\n", curfilter->section);
                }
        }

        if (curfilter && curfilter->handler)
                curfilter->handler(user, section, name, value, lineno);
}

int main()
{
        rumboot_printf("Parsing ini at 0x%x\n", &rumboot_platform_spl_end);
        char userdata[USERDATA_LEN];

        ini_parse_string(&rumboot_platform_spl_end, theini_handler, userdata);
        /* Return 0, test passed */
        exit(0);
}
