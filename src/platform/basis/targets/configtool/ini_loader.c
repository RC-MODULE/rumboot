#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <algo/ini.h>
#include <stdlib.h>
#include <rumboot/io.h>

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
struct pcie_barcfg {
        uintptr_t base;
        uintptr_t size;
        uintptr_t translation;
};

struct pcie_data
{
        uint32_t vid;
        uint32_t pid;
        struct pcie_barcfg bars[PCIE_NUM_BARS];
};

int pcie_handler(void *user, const char *section,
                 const char *name, const char *value,
                 int lineno)
{
        struct pcie_data *data = user;
        if (strcmp(name, "vid") == 0)
        {
                data->vid = strtoul(value, NULL, 16);
        }

        if (strcmp(name, "pid") == 0)
        {
                data->vid = strtoul(value, NULL, 16);
        }

        int bar;
        if (sscanf(name, "BAR:%d", &bar)==1) {
                rumboot_printf("Configuring bar %d\n", bar);
        }
}

void pcie_apply(void *user)
{
        struct pcie_data *data = user;
        /* pcie_configure(base, data->vid, data->pid); */
        int i;
        for (i=0; i<PCIE_NUM_BARS; i++) {
                /*pcie_add_bar() */
        }
}


int exit_handler(void *user, const char *section,
                 const char *name, const char *value,
                 int lineno)
{
        if (strcmp(name, "mode") != 0)
                return 0;

        if (strcmp(value, "loop") == 0) {
                rumboot_printf("Entering endless loop\n");
                /* Endless loop */
                while(1) ;;
        }

        if (strcmp(value, "next") == 0) {
                rumboot_printf("Returning to bootrom to boot from next device\n");
                exit(0);
        }

        if (strcmp(value, "host") == 0) {
                rumboot_printf("Will now go back to host mode\n");
                exit(-1);
        }


        if (strcmp(value, "wfiloop") == 0) {
                rumboot_printf("Entering WFI loop\n");
                while(1) {
                        asm("wfi");
                }
        }

        int jid;
        if (1==sscanf(value, "jump:%d", &jid)) {
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

        if (strlen(prevsection) && strcmp(prevsection, section) != 0)
        {
                bzero(user, USERDATA_LEN);
                rumboot_printf("Finished parsing section: %s", prevsection);
                if (curfilter && curfilter->apply)
                {
                        curfilter->apply(user);
                }

                if (strlen(section) > MAX_SECTION_LEN)
                {
                        rumboot_platform_panic("Section name %d too long", section);
                }

                strcpy(prevsection, section);
                curfilter = filters;
                do
                {
                        if (strcmp(curfilter->section, section) == 0)
                                break;
                } while (curfilter++);

                if (!curfilter)
                {
                        rumboot_printf("Ignoring unknown section: %s\n", section);
                } else {
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
