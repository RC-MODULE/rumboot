#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>


static inline const char *bool_param(bool param)
{
        return param ? "enabled" : "disabled";
}

void rumboot_platform_dump_config(struct rumboot_config *conf) {
        rumboot_printf("--- RumBoot Configuration ---\n");
        rumboot_printf("Force Host Mode: %s\n", bool_param(conf->hostmode));
        rumboot_printf("Selftest:        %s\n", bool_param(conf->selftest));
        if (conf->has_edcl) {
                rumboot_printf("EDCL/RMAP:       %s\n", bool_param(conf->edcl));
        }
        if (conf->legacyboot) {
                rumboot_printf("Legacy BBP boot: enabled\n");
        }
        rumboot_printf("UART speed:      %d bps\n", conf->baudrate);
        rumboot_printf("---          ---          ---\n");
}


static void hostmode_loop()
{
        struct rumboot_bootheader *hdr = (struct rumboot_bootheader *) &rumboot_platform_spl_start;
        rumboot_printf("boot: Entering host mode loop\n");
        rumboot_platform_request_file("HOSTMOCK", (uint32_t) hdr);
        void *data;
        while (1) {
                ssize_t len = rumboot_bootimage_check_header(hdr, &data);
                if (len == -EBADMAGIC) {
                        continue;
                }
                if (len < 0) {
                        rumboot_printf("boot: validation failed: %s\n", rumboot_strerror(len));
                        hdr->magic = 0;
                        continue;
                }
                if (0 == rumboot_bootimage_check_data(hdr)) {
                        rumboot_bootimage_exec(hdr);
                        rumboot_printf("boot: We're back in rom!\n");
                } else {
                        rumboot_printf("boot: Data CRC32 mismatch\n");
                }
        }
}


int main()
{
        rumboot_platform_perf("Config printout");

        int ret;
        #define PDATA_SIZE 128
        char pdata[PDATA_SIZE];

        rumboot_print_logo();
        struct rumboot_config conf;
        rumboot_platform_read_config(&conf);
        rumboot_platform_dump_config(&conf);

        rumboot_platform_perf(NULL);

        if (conf.selftest) {
                rumboot_platform_perf("Selftest");
                rumboot_platform_selftest(&conf);
                rumboot_platform_perf(NULL);
        }

        if (conf.hostmode) {
                hostmode_loop();
        }


        rumboot_platform_perf("Boot chain");
        const struct rumboot_bootsource *sources = rumboot_platform_get_bootsources();
        ret = bootsource_try_chain(sources, (void*) &pdata);
        if (ret) {
                rumboot_printf("boot: We got back from secondary image\n");
        } else {
                rumboot_printf("boot: Failed to boot secondary image\n");
        }
        rumboot_platform_perf(NULL);
        hostmode_loop();
        /* Never reached. Throw an error if it does */
        return 1;
}
