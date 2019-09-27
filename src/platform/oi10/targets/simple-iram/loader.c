#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <rumboot/boot.h>
#include <rumboot/xmodem.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/macros.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>


static void hostmode_loop(struct rumboot_config *conf, void *pdata)
{
        size_t maxsize = 256 * 1024;
        struct rumboot_bootheader *hdr = (void *) IM0_BASE;
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
                        #ifndef RUMBOOT_PLATFORM_EASTER_EGG
                        dbg_boot(NULL, "M'aiq the Liar: There are absolutely no easter eggs in bootrom code.");
                        #else
                        dbg_boot(NULL, RUMBOOT_PLATFORM_EASTER_EGG);
                        #endif
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
    
        static const tlb_entry sram0_tlb_entry[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    write_tlb_entries(sram0_tlb_entry, ARRAY_SIZE(sram0_tlb_entry));

    
    rumboot_printf("Chain-booting from IM0 \n");
  #define PDATA_SIZE 128
    char pdata[PDATA_SIZE];

        struct rumboot_config conf;
        rumboot_platform_read_config(&conf);
        rumboot_platform_init_loader(&conf);
        rumboot_print_logo();
        hostmode_loop(&conf, pdata);


    return 0;
}
