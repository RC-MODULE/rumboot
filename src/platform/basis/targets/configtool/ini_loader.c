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

struct ddr_data
{
    int slot;
    int identifier;
    int memtest;
    uint32_t speed;
    uint32_t ecc;
    char name[32];
};



#define fill_hex(nm)                                 \
        if (strcmp(name, #nm) == 0)                  \
        {                                            \
                data->nm = strtoul(value, NULL, 16); \
        }

#define fill_dec(nm)                                 \
        if (strcmp(name, #nm) == 0)                  \
        {                                            \
                data->nm = strtoul(value, NULL, 10); \
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

static int the_exitcode = 0;
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
                the_exitcode = 0;
                return 1;
        }

        if (strcmp(value, "host") == 0)
        {
                rumboot_printf("Will now go back to host mode\n");
                the_exitcode = -1;
                return 1;
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
                the_exitcode = jid;
                return 1;
        }
}


extern int ddr_do_init (int slot, int identifier, int speed, int ecc);
extern int ddrlib_find_memtype (const char *type, uint32_t speed);

int ddr_handler(void *user, const char *section,
                 const char *name, const char *value,
                 int lineno)
{
    struct ddr_data *data = user;
    int slot;

    if (sscanf(name, "slot[%d]", &slot) && strstr(name, "name"))
        strncpy(data[slot].name, value, ARRAY_SIZE(data[slot].name));

    if (sscanf(name, "slot[%d]", &slot) && strstr(name, "speed"))
        data[slot].speed = strtoul(value, NULL, 10);

    if (sscanf(name, "slot[%d]", &slot) && strstr(name, "ecc"))
        data[slot].ecc = strtoul(value, NULL, 10);

    if (sscanf(name, "slot[%d]", &slot) && strstr(name, "memtest"))
        data[slot].memtest = strtoul(value, NULL, 10);
    
    /*
    if (strcmp(name, "turn_on") == 0) {
        data->turn_on = strtoul(value, NULL, 16);
        
        if (data->turn_on) {
            if (data->identifier != -1) {
                rumboot_printf("DDR: slot %d  memory %s  speed %d  ecc %d\n", data->slot, data->name, data->speed, data->ecc);
                int ret = ddr_do_init(data->slot, data->identifier, data->speed, data->ecc);
                if (ret) {
                        rumboot_printf("    DDR initialisation of slot %d type %s failed\n", data->slot, value);
                        rumboot_printf("    System halted\n");
                        while(1);
                }
                return 1;
            }
            else
                rumboot_printf("ERROR: unknown DDR memory type\n");
        }
        else
            rumboot_printf("DDR slot %d turn OFF\n", data->slot);
    }
*/
        
        
}

void ddr_apply(void *user)
{        
        struct ddr_data *data = user;
        int slot; 
        for (slot=0; slot<2; slot++) {
                data[slot].name[31] = 0x0;
                if (strcmp(data[slot].name, "disabled") == 0) {
                        rumboot_printf("DDR: slot %d is disabled\n", slot);
                        continue;
                }
                int id = ddrlib_find_memtype(data[slot].name, data[slot].speed);

                if (id >= 0) {
                        rumboot_printf("DDR: slot %d memory %s (%d) speed %d ecc %s\n", slot, data[slot].name, id, data[slot].speed, data[slot].ecc ? "enabled" : "disabled");
                        int ret = ddr_do_init(slot, id, data[slot].speed, data[slot].ecc);
                } else {
                        rumboot_printf("DDR: slot %d contains invalid name: %s, skipping\n", slot, data[slot].name);
                }
        }
};

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
    {
        .section = "IOMEM",
        .handler = iomem_handler,
        .apply = NULL,
    },
    {
        .section = "DDR",
        .handler = ddr_handler,
        .apply = ddr_apply,
    },
    {/* Sentinel */},
};

static char prevsection[MAX_SECTION_LEN];
static struct sectionfilter *curfilter;

int theini_handler(void *user, const char *section,
                   const char *name, const char *value,
                   int lineno)
{

        if (!strlen(prevsection) || strcmp(prevsection, section) != 0)
        {
                if (strlen(prevsection))
                {
                        //rumboot_printf("Finished parsing section: %s\n", prevsection);
                }
                if (curfilter && curfilter->apply)
                {
                        curfilter->apply(user);
                }
                bzero(user, USERDATA_LEN);
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
                        //rumboot_printf("Current filter: %s\n", curfilter->section);
                }
        }

        if (curfilter && curfilter->handler)
                curfilter->handler(user, section, name, value, lineno);
}

int main()
{
        rumboot_platform_runtime_info->silent = 0;
        rumboot_printf("Parsing ini at 0x%x\n", &rumboot_platform_spl_end);
        char userdata[USERDATA_LEN];
        int ret = ini_parse_string(&rumboot_platform_spl_end, theini_handler, userdata);
        if (ret != 0) {
                rumboot_printf("Failed to parse config, problem on line %d\n", ret);
                while(1);;
        }
        rumboot_printf("Done parsing config, code %d \n", the_exitcode);
        return the_exitcode;
}
