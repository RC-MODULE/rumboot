#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <platform/easynmc/easynmc.h>
#include <stdint.h>
#include <devices/greth_edcl.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>


static const __attribute__((packed)) __attribute__((used)) __attribute__((section(".header")))
struct rumboot_bootheader the_head = {
	.magic		= RUMBOOT_HEADER_MAGIC,
	.version	= RUMBOOT_HEADER_VERSION,
	.chip_id	= RUMBOOT_PLATFORM_CHIPID,
	.chip_rev	= RUMBOOT_PLATFORM_CHIPREV,
};

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
        dump_parameter("Selftest:        %s\n", conf->selftest);
        dump_parameter("EDCL/RMAP:       %s\n", conf->edcl);
        rumboot_printf("UART speed:      %d bps\n", conf->baudrate);
        rumboot_printf("Max SPL size:    %d bytes\n", maxsize);
        rumboot_platform_print_summary(conf);
        rumboot_printf("---          ---          ---\n");
}


int main(int argc, char** argv)
{  
       size_t maxsize;


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

	rumboot_bootimage_check_header(&the_head, NULL);
	return argc; 
} 

