#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <rumboot/timer.h>
#include <rumboot/io.h>
#include <rumboot/hexdump.h>
#include <rumboot/xmodem.h>
#include <rumboot/boot.h>

#define DSP_MEM_BASE 0x39000000


static void hostmode_loop(struct rumboot_config *conf, void *pdata)
{
        size_t maxsize = 128*1024;
        struct rumboot_bootheader *hdr = rumboot_platform_get_spl_area(&maxsize);
        dbg_boot(NULL, "Host Mode, please upload SPL to 0x%x", (uintptr_t) hdr);
        rumboot_platform_enter_host_mode(conf);
        dbg_boot(NULL, "Hit 'X' for xmodem upload");
        void *data;
        int ret;

        while (1) {
                rumboot_platform_sv_event("HOST");
                #ifdef __PPC__
                /* FixMe: Use cross-platform barrier sync functions here */
                asm("msync");
                #endif

                int c = rumboot_getchar(10000);
                if (c == 'X') {
                        int ret = xmodem_get((void *) hdr, maxsize);
                        mdelay(250);
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
                        dbg_boot(NULL, "M'aiq the Liar: There are absolutely no easter eggs in bootrom code.");
                        rumboot_platform_sv_event("EASTER_EGG");
                }

                if (hdr->device) {
                        hdr->device = NULL;
                }
                ssize_t len = rumboot_bootimage_check_header(hdr, &data);
                if (len == -EBADMAGIC) {
                        continue;
                }
                if (len < 0) {
                        dbg_boot(NULL, "Header error: %s\n", rumboot_strerror(len));
                        hdr->magic = -len;
                        continue;
                }
                if (0 == rumboot_bootimage_check_data(hdr)) {
                        hdr->magic = 0x0;
                        ret = rumboot_platform_exec(hdr);
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
    int i; 
    char pdata[128];
    rumboot_platform_init_loader(NULL);

    for (i=0; i<1024; i++) {
        iowrite8(i, DSP_MEM_BASE + i);
    }

    struct rumboot_bootheader *hdr = (struct rumboot_bootheader *) DSP_MEM_BASE;
    

    hostmode_loop(NULL, pdata);
}
