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

#include <rumboot/timer.h>
#include "../src/platform/oi10/targets/l2scrubber/glue.h"

int main()
{
    struct l2_scrubber *scr = l2scrubber_tests_init(0);
    uintptr_t ptr = (uintptr_t) l2scrubber_tests_get_cachable_area();

    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);


    int i,j,w;
    int injected = 0;
    int index;

for (w=0; w<4; w++) {
    rumboot_printf("%d\n", w);
    int index = l2_tag_address_encode(&layout, w, 0);
    l2_dump_tag(&layout, index);
    l2_inject_tag_ecc_fault(DCR_L2C_BASE, index, 1);    
}

for (j=0; j < layout.l2size_bytes; j = j + 8) {
    ioread64(ptr + j);
}

#if 0

for (index = 0; index < 2048; index++ ) {
    l2_inject_tag_ecc_fault(DCR_L2C_BASE, index, 1);
}
for (j=0; j < layout.l2size_bytes; j = j + 8) {
    ioread64(ptr + j);
}
rumboot_printf("%d irqs\n", scr->irq_count);
return 0;
#endif

#if 0
    for (i=1; i<8; i++) {
        scr->injected_bit = 64 + i;
        l2_dump_tag(&scr->layout, index);
        int index = l2_tag_address_encode(&layout, 2, i);
        l2_inject_tag_ecc_fault(DCR_L2C_BASE, index, i);
        for (j=0; j < layout.l2size_bytes; j = j + 8) {
            ioread64(ptr + j);
        }
        for (j=0; j < layout.l2size_bytes; j = j + 8) {
            ioread64(ptr + j);
        }
        for (j=0; j < layout.l2size_bytes; j = j + 8) {
            ioread64(ptr + j);
        }
    }
    if (scr->irq_count != 7) {
        return 1;
    }
    for (j=0; j < layout.l2size_bytes; j = j + 8) {
        ioread64(ptr + j);
    }

    for (i=0; i<32; i++) {
        scr->injected_bit = i;
        l2_dump_tag(&scr->layout, index);
        int index = l2_tag_address_encode(&layout, 2, i);
        l2_inject_tag_fault(DCR_L2C_BASE, index, i);
        for (j=0; j < layout.l2size_bytes; j = j + 8) {
            ioread64(ptr + j);
        }
        for (j=0; j < layout.l2size_bytes; j = j + 8) {
            ioread64(ptr + j);
        }
        for (j=0; j < layout.l2size_bytes; j = j + 8) {
            ioread64(ptr + j);
        }
    }
    rumboot_printf("%d\n", scr->irq_count);
    if (scr->irq_count != 36) {
        return 1;
    }

    for (j=0; j < layout.l2size_bytes; j = j + 8) {
        ioread64(ptr + j);
    }
    return 0;
        
#endif

#if 0
#if 0
    i=1;
    RUMBOOT_ATOMIC_BLOCK() {
        index = l2_tag_address_encode(&layout, 1, 0);
        l2_dump_tag(&layout, index);
        l2_inject_tag_ecc_fault(DCR_L2C_BASE, index, i);
        l2_dump_tag(&layout, index);

        index = l2_tag_address_encode(&layout, 1, 0);
        l2_inject_tag_ecc_fault(DCR_L2C_BASE, index, i);
    }
    for (j=0; j < layout.l2size_bytes; j = j + 8) {
        ioread64(ptr + j);
    }

    for (j=0; j < layout.l2size_bytes; j = j + 8) {
        ioread64(ptr + j);
    }

    return 0;

#else
    for (i=1; i<8; i++) {
        injected = 0;
        scr->irq_count = 0;
        rumboot_printf("Injecting tag ecc error in bit %d\n", i);
        for (w=1; w<4; w++) {
             for (j=500; j < layout.tag_array_size / 4; j++) {
                int index = l2_tag_address_encode(&layout, w, j);
                l2_inject_tag_ecc_fault(DCR_L2C_BASE, index, i);
                rumboot_printf("%d %d\n", w, j);
                injected++;
            }
        }
        rumboot_printf("Running readback, this will take a while\n");
        for (j=0; j < layout.l2size_bytes; j = j + 8) {
            ioread64(ptr + j);
        }

        if (injected != scr->irq_count) {
            rumboot_printf("FATAL: Expected %d IRQs, got %d\n", injected, scr->irq_count);
            //return 1;
        }
    }

    return 0;

    for (i=0; i<64; i++) {
        injected = 0;
        scr->irq_count = 0;
        rumboot_printf("Injecting tag ecc error in bit %d\n", i);
        for (w=0; w<4;w++) {
            for (j=0; j < layout.tag_array_size / 4; j++) {
                int index = l2_tag_address_encode(&layout, w, j);
                l2_inject_tag_fault(DCR_L2C_BASE, index, i);
                injected++;
            }
            for (j=0; j < layout.l2size_bytes; j = j + 8) {
                ioread64(ptr + j);
            }
        }
        rumboot_printf("Running readback, this will take a while\n");

        if (injected != scr->irq_count) {
            rumboot_printf("FATAL: Expected %d IRQs, got %d\n", injected, scr->irq_count);
            //return 1;
        }
    }
return 0;
#endif
#endif
}
