#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/xmodem.h>
#include <rumboot/timer.h>
#include <rumboot/irq.h>
#include <platform/bootheader.h>

static inline void dump_parameter(const char *fmt, int param)
{
        if (param < 0) {
                return;
        }
        rumboot_printf(fmt, (param == 0) ? "disabled" : "enabled");
}

void rumboot_platform_dump_config(struct rumboot_config *conf, size_t maxsize) {
        rumboot_printf("--- RumBoot Configuration ---\n");
        dump_parameter("Force Host Mode: %s\n", conf->hostmode);
        dump_parameter("Secure Boot:     %s\n", conf->secure_boot);
        dump_parameter("Selftest:        %s\n", conf->selftest);
        dump_parameter("EDCL/RMAP:       %s\n", conf->edcl);
        rumboot_printf("UART speed:      %d bps\n", conf->baudrate);
        rumboot_printf("Max SPL size:    %d bytes\n", maxsize);
        rumboot_platform_print_summary(conf);
        rumboot_printf("---          ---          ---\n");
}


static void hostmode_loop(struct rumboot_config *conf, void *pdata)
{
        size_t maxsize;
        struct rumboot_bootheader *hdr = rumboot_platform_get_spl_area(&maxsize);
        dbg_boot(NULL, "Host Mode, please upload SPL to 0x%x", (uintptr_t) hdr);
        rumboot_platform_enter_host_mode(conf);
        dbg_boot(NULL, "Hit 'X' for X-Modem upload");
        void *data;
        int ret;

        while (1) {
                rumboot_platform_sv_event("HOST");
                #ifdef __PPC__
                /* FixMe: Use cross-platform barrier sync functions here */
                asm("msync");
                #endif

                int c = rumboot_getchar(100);
                if (c == 'X') {
                        int ret = xmodem_get((void *) hdr, maxsize);
                        rumboot_printf("\n\n\n");
                        if (ret > 0) {
                                dbg_boot(NULL, "Received %d bytes, executing in 100ms", ret);
                                mdelay(100);
                        } else {
                                dbg_boot(NULL, "Upload failed, code %d", ret);
                                hdr->magic = -ret;

                        }
                }

                if (c == 'e') {
                        #ifndef RUMBOOT_PLATFORM_EASTER_EGG
                        dbg_boot(NULL, "M'aiq the Liar: There are absolutely no easter eggs in bootrom code.");
                        #else
                        dbg_boot(NULL, RUMBOOT_PLATFORM_EASTER_EGG);
                        #endif
                        rumboot_platform_sv_event("EASTER_EGG");
                }

                if (hdr->device) {
                        hdr->device = 0;
                }
                ssize_t len = rumboot_bootimage_check_header(NULL, hdr, &data);
                if (len == -EBADMAGIC) {
                        continue;
                }
                if (len < 0) {
                        dbg_boot(NULL, "Header error: %s\n", rumboot_strerror(len));
                        hdr->magic = -len;
                        continue;
                }
                if (0 == rumboot_bootimage_check_data(NULL, hdr)) {
                        ret = rumboot_bootimage_execute(hdr, NULL);
                        dbg_boot(NULL, "Back in rom, code %d", ret);
                        if (ret > 0) {
                                bootsource_try_by_id(ret - 1, pdata, hdr, maxsize);
                        }
                } else {
                        dbg_boot(NULL, "Data CRC32 mismatch\n");
                        hdr->magic = EBADDATACRC;
                }
        }
}


int main()
{
        size_t maxsize;

        /* Set our own handler */
        rumboot_irq_set_exception_handler(rumboot_arch_exception);

        struct rumboot_bootheader *hdr = rumboot_platform_get_spl_area(&maxsize);
        /* Initialize SPL execution counter */
        rumboot_platform_runtime_info->persistent[0] = 0;
        rumboot_platform_perf("Config");
        #define PDATA_SIZE 128
        char pdata[PDATA_SIZE];

        struct rumboot_config conf;
        rumboot_platform_read_config(&conf);
        rumboot_platform_init_loader(&conf);
        rumboot_print_logo();
        rumboot_platform_dump_config(&conf, maxsize);

        rumboot_platform_perf(NULL);

        if (conf.selftest > 0) {
                int ret;
                rumboot_platform_perf("Selftest");
                ret = rumboot_platform_selftest(&conf);
                rumboot_platform_perf(NULL);
                rumboot_printf("selftest: %d test(s) from suite failed\n", ret);
        }

        if (conf.hostmode) {
                hostmode_loop(&conf, pdata);
        }


        rumboot_platform_perf("Boot");
        bootsource_try_chain(pdata, hdr, maxsize);
        rumboot_platform_perf(NULL);

        hostmode_loop(&conf, pdata);
        /* Never reached. Throw an error if it does */
        return 1;
}
