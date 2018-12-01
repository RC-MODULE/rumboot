#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <algo/ini.h>
#include <stdlib.h>
#include <rumboot/io.h>



int theini_handler(void *user, const char *section,
                   const char *name, const char *value,
                   int lineno)
{
        if (strcmp(section, "IOMEM") == 0) {
                uintptr_t dest = strtoul(name, NULL, 16);
                uint32_t v     = strtoul(value, NULL, 16);
                rumboot_printf("[%x] <= %x\n", dest, v);
                iowrite32(v, dest);
        } else {
                rumboot_printf("Unsupported option: %d: %s.%s=%s\n", lineno, section, name, value);
        }
        return 0;
}

int main()
{
        rumboot_printf("Parsing ini at 0x%x\n", &rumboot_platform_spl_end);
        rumboot_printf("%s\n", &rumboot_platform_spl_end);
        ini_parse_string(&rumboot_platform_spl_end, theini_handler, NULL);
        /* Return 0, test passed */
        exit(0);
}
