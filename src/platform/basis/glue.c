#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <arch/irq_macros.h>
#include <platform/devices.h>
#include <devices/sp804.h>
#include <rumboot/rumboot.h>
#include <string.h>
#include <rumboot/pcie_test_lib.h>
#include <rumboot/ddr_test_lib.h>
#include <devices/gic.h>
#include <devices/irq-proxy-gic-cdnpcie.h>
#include <rumboot/irq.h>
#include <stdbool.h>

int64_t rumboot_virt_to_phys(volatile void *addr)
{
        int64_t ret = (uint32_t)addr;

        return ret;
}

uint32_t rumboot_virt_to_dma(volatile void *addr)
{
        return (uint32_t)addr;
}

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
        return (0xFFFFFFFF - sp804_get_value(DIT3_BASE, 1)) / 6.25;
}


extern char rumboot_im0_heap_start;
extern char rumboot_im0_heap_end;
extern char rumboot_im1_heap_start;
extern char rumboot_im1_heap_end;

extern char rumboot_ddr1_heap_start;
extern char rumboot_ddr1_heap_end;
extern char rumboot_ddr0_heap_start;
extern char rumboot_ddr0_heap_end;

/* Comes from startup.S */
extern char rumboot_default_irq_vectors;

void setup_mirrored_heap(int id)
{
        uint32_t len = rumboot_malloc_heap_length(id);
        void *ptr = rumboot_malloc_from_heap(id, len / 2);
        char *name = rumboot_malloc_from_heap(0, 16);

        strcpy(name, "PCIE-");
        strcat(name, rumboot_malloc_heap_name(id));

        /* Trashy code, but will do for now */
        if (rumboot_platform_runtime_info->heaps[id].start >= (void *)0xC0000000) {
                rumboot_malloc_register_heap(name,
                                             (ptr - 0xC0000000) + 0x60000000,
                                             (ptr + (len / 2) - 0xC0000000) + 0x60000000);
        } else if ((rumboot_platform_runtime_info->heaps[id].start >= (void *)0x80000000)) {
                rumboot_malloc_register_heap(name,
                                             ptr - 0x80000000 + 0x50000000,
                                             ptr + (len / 2) - 0x80000000 + 0x50000000);
        } else {
                rumboot_malloc_register_heap(name,
                                             BASIS_PHYS(ptr),
                                             BASIS_PHYS(ptr + (len / 2)));
        }
};

void rumboot_platform_setup()
{
        /*
         * Make sure VBAR is where we need it.
         * Needed for handling IRQs in secondary image
         */
        arm_vbar_set((uint32_t)&rumboot_default_irq_vectors);
        rumboot_irq_register_gic();

        rumboot_malloc_register_heap("IM0",
                                     &rumboot_im0_heap_start, &rumboot_im0_heap_end);
        rumboot_malloc_register_heap("IM1",
                                     &rumboot_im1_heap_start, &rumboot_im1_heap_end);

#ifdef RUMBOOT_BASIS_ENABLE_DDR
        /* DDR Should be already initialized via bootrom stub */
        rumboot_malloc_register_heap("DDR0",
                                     &rumboot_ddr0_heap_start, &rumboot_ddr0_heap_end);
        rumboot_malloc_register_heap("DDR1",
                                     &rumboot_ddr1_heap_start, &rumboot_ddr1_heap_end);
#endif


#ifdef RUMBOOT_BASIS_ENABLE_MIRROR
        struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
        rumboot_irq_table_activate(tbl);
        rumboot_irq_register_msix_proxy_controller();
        int heaps = rumboot_malloc_num_heaps();
        int i;

#if  defined(RUMBOOT_BASIS_ENABLE_DDR) && defined(RUMBOOT_BASIS_DIRECT_IRAM)
        for (i = 2; i < heaps; i++)
                setup_mirrored_heap(i);
#else
        for (i = 0; i < heaps; i++)
                setup_mirrored_heap(i);
#endif
#endif

        /* Fire timer subsystem */
        struct sp804_conf conf_str;

        conf_str.mode = FREERUN;
        conf_str.interrupt_enable = 1;
        conf_str.clock_division = 16;
        conf_str.width = 32;
        conf_str.load = 0;
        conf_str.bgload = 0;

        sp804_config(DIT3_BASE, &conf_str, 1);
        sp804_enable(DIT3_BASE, 1);
}
