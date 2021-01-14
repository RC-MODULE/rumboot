#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <stdio.h>
#include <inttypes.h>
#include <arch/ppc_476fp_mmu.h>
#include <platform/interrupts.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <math.h>
#include <arch/l2scrub.h>

#include <rumboot/timer.h>
#include "../src/platform/oi10/targets/l2scrubber/glue.h"


static void readback(struct l2_scrubber *scr, int mode)
{
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    for (int j=0; j < layout.l2size_bytes; j = j + 8) {
        ioread64(j);
        if (mode == L2SCRUB_MODE_POLLING) {
            l2_scrubber_scrub_once(scr);
        }
    }
}

int main()
{
    int i,j;
    int num_irqs;
    int mode = L2SCRUB_MODE_POLLING;
    struct l2_scrubber *scr = l2scrubber_tests_init(mode);
    uintptr_t ptr = (uintptr_t) l2scrubber_tests_get_cachable_area();

    /* This magic bullet will fix everything */
    l2_scrubber_enable_pattern_scrubbing(scr, 0x5a5a5a5a5a5a5a5aUL);

    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);


    for (i=0; i<8; i++) {
        l2_scrubber_reset_stats(scr);
        rumboot_printf("Injecting data ecc error in bit %d\n", i);
        for (j=0; j < layout.l2size_bytes / 8; j++) {
            l2_inject_data_ecc_fault(DCR_L2C_BASE, j, i);
        }
        l2_inject_tag_fault(DCR_L2C_BASE, i, 1);

        num_irqs = j;

        rumboot_printf("Running readback #1, this will take a while\n");
        readback(scr, mode);

        rumboot_printf("Fixed %d errors so far\n", scr->corrected_data_errors);

        rumboot_printf("Running readback #2, this will take a while\n");
        readback(scr, mode);

        rumboot_printf("Fixed %d errors so far\n", scr->corrected_data_errors);

        if (num_irqs != scr->corrected_data_errors) {
            rumboot_printf("FATAL: Expected %d corrected, but got %d\n", j, scr->corrected_data_errors);
            l2_scrubber_dump_stats(scr);
            return 1;
        }
        l2_scrubber_dump_stats(scr);
    }


    for (i=0; i<64; i++) {
        l2_scrubber_reset_stats(scr);
        rumboot_printf("Injecting data error in bit %d\n", i);
        for (j=0; j < layout.l2size_bytes / 8; j++) {
            l2_inject_data_fault(DCR_L2C_BASE, j, i);
        }
        num_irqs = j;
        rumboot_printf("Running readback, this will take a while\n");

        readback(scr, mode);
        readback(scr, mode);
        readback(scr, mode);

        if (num_irqs != scr->corrected_data_errors) {
            rumboot_printf("FATAL: Expected %d corrected, but got %d\n", j, scr->corrected_data_errors);
            l2_scrubber_dump_stats(scr);
            return 1;
        }
    }
return 0;
}
