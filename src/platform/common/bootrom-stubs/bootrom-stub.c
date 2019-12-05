#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/boot.h>

#ifdef RUMBOOT_MIRROR_STUB
        #include <rumboot/pcie_test_lib.h>
        #include <rumboot/ddr_test_lib.h>
#endif

int main()
{
#ifndef RUMBOOT_PRINTF_ACCEL
        struct rumboot_config conf;
        rumboot_platform_read_config(&conf);
        rumboot_platform_init_loader(&conf);
#endif
        #ifndef RUMBOOT_STUB_NOLOGO
        rumboot_print_logo();
        #endif

        struct rumboot_bootheader *hdr = (struct rumboot_bootheader *) &rumboot_platform_spl_start;

#ifdef RUMBOOT_MIRROR_STUB
        /* Set up mirror mode, cut half of memory for it */
        rumboot_printf("STUB: Configuring PCIe, this will take a while...\n");
        pcie_mirror_tests_setup();
        rumboot_printf("Setting up DDR. Go grab a coffee, this will take a while\n");
        ddr0_ddr1_init();
        rumboot_printf("Done!\n");
#endif

        int ret=1;
        while(1) {
                /* Save simulation state here */
                rumboot_platform_sim_save("testbench.simulator_state");
                rumboot_platform_request_file("IM0BIN", (uint32_t)hdr);

#ifdef __PPC__
                asm ("msync");
#endif
                int i = rumboot_bootimage_check_magic(hdr->magic);
                if (i < 0) {
                        rumboot_printf("STUB: No more images to boot\n");
                        break;
                }

                rumboot_printf("STUB: Executing SPL image from %x. Magic: 0x%x Entry: 0x%x\n",
                        (uint32_t) hdr, hdr->magic, hdr->entry_point[0]);
                ret = rumboot_bootimage_execute(hdr, NULL);
                if (ret != 0)
                        break;
        }
        uint32_t code = ret;
        rumboot_platform_event_raise(EVENT_TERM, &code, 1);
        return ret;
}
