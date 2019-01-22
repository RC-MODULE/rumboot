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
        //TODO: Figure out proper clock
        return (0xFFFFFFFF - sp804_get_value(DIT_Base, 1)) / 8.32;
}


/* Comes from startup.S */
extern char rumboot_default_irq_vectors;


void rumboot_platform_setup()
{
        /*
         * Make sure VBAR is where we need it.
         * Needed for handling IRQs in secondary image
         */
        arm_vbar_set((uint32_t)&rumboot_default_irq_vectors);
        rumboot_irq_register_gic();


        /* Fire timer subsystem */
        struct sp804_conf conf_str;

        conf_str.mode = FREERUN;
        conf_str.interrupt_enable = 1;
        conf_str.clock_division = 16;
        conf_str.width = 32;
        conf_str.load = 0;
        conf_str.bgload = 0;

        sp804_config(DIT_Base, &conf_str, 1);
        sp804_enable(DIT_Base, 1);
}
