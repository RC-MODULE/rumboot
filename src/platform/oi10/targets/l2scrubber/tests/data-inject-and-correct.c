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
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/interrupts.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <math.h>
#include <arch/l2scrub.h>

#include <rumboot/timer.h>
#include "../src/platform/oi10/targets/l2scrubber/glue.h"
#define NUMCOR 1

int main()
{   
    struct l2_scrubber *scr = l2scrubber_tests_init();
    uintptr_t ptr = (uintptr_t) l2scrubber_tests_get_cachable_area();

    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);

    int i,j; 
    int num_irqs;
    for (j=0; j < layout.l2size_bytes / 8; j++) {
        rumboot_printf("cell %d/%d\n", j, layout.l2size_bytes / 8);
        for (i=0; i<8; i++) {
            num_irqs = scr->irq_count;
            l2_inject_data_ecc_fault(DCR_L2C_BASE, j, i);
            memcpy(ptr, ptr, 256*1024);
//            ioread32( (ptr + j*8 + 4));
//            ioread32( (ptr + j*8));
            if (num_irqs == scr->irq_count) {
                rumboot_printf("FATAL: IRQ didn't arrive: bit %d cell %d\n", i, j);
                l2_dump_data(j-5, 10);
                return 1;
            }
        }
        for (i=0; i<64; i++) {
            l2_inject_data_fault(DCR_L2C_BASE, j, i);
            ioread32( (ptr + j*8 + 4));
            ioread32( (ptr + j*8));
            if (num_irqs == scr->irq_count) {
                rumboot_printf("FATAL: IRQ didn't arrive: bit %d cell %d\n", i, j);
                l2_dump_data(j-5, 10);
                return 1;
            }
        }
    }
    rumboot_printf("TOTAL: %d DATA errors fixed, %d TAG errors fixed %d us wasted\n", scr->corrected_data_errors, scr->corrected_tag_errors, scr->time_wasted);
    return 0;
//    cache_dump_line(NUMCOR);
//    l2_inject_data_fault(DCR_L2C_BASE, NUMCOR, 1);
//    l2_inject_data_ecc_fault(DCR_L2C_BASE, NUMCOR, 1);
//    cache_dump_line(NUMCOR);

//    rumboot_printf("STATUS: %x\n", l2c_l2_read(DCR_L2C_BASE, L2C_L2INT));

//    l2_inject_data_fault(DCR_L2C_BASE, 1, 1);
//    l2_inject_data_fault(DCR_L2C_BASE, 2, 2);

//    cache_dump_tags();
//    l2_inject_tag_fault(DCR_L2C_BASE, 0, 29);
//    cache_dump_tags();



#if 0
    int i; 
    int idx;
    for (int way=0; i<4; i++) {
        idx = 0;
    for (i=0; i<64; i++) {
        //inject_index = idx;
        //bit_id = i;
        l2_inject_tag_fault(DCR_L2C_BASE, idx, i);
//        idx++;
        ioread32( (idx*8 + 4));
        ioread32( (idx*8));
        ioread32( (idx*8 + 4));
        ioread32( (idx*8));

    }
    for (i=0; i<16; i++) {
        //inject_index = idx;
        //bit_id = 64 + i;
        l2_inject_tag_ecc_fault(DCR_L2C_BASE, idx, i);
        //idx++;
        ioread32( (idx*8 + 4));
        ioread32( (idx*8));
        ioread32( (idx*8 + 4));
        ioread32( (idx*8));

    }

    }


#endif


    for (i=0; i<16384; i=i+4) {
        ioread32(i);
        mdelay(500);
    }

    rumboot_printf("%x %x\n", *((uint32_t *) (NUMCOR*8)), *((uint32_t *) (NUMCOR*8 + 4)));
    rumboot_printf("%x %x\n", *((uint32_t *) (NUMCOR*8)), *((uint32_t *) (NUMCOR*8 + 4)));
    rumboot_printf("%x %x\n", *((uint32_t *) (NUMCOR*8)), *((uint32_t *) (NUMCOR*8 + 4)));
    rumboot_printf("%x %x\n", *((uint32_t *) (NUMCOR*8)), *((uint32_t *) (NUMCOR*8 + 4)));
    rumboot_printf("%x %x\n", *((uint32_t *) (NUMCOR*8)), *((uint32_t *) (NUMCOR*8 + 4)));

//    cache_dump_data(16);
    return 0;
}
