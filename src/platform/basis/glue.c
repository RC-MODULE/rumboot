#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <arch/arm/irq_macros.h>
#include <platform/devices.h>
#include <devices/sp804.h>

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
    return (0xFFFFFFFF-sp804_get_value(DIT3_BASE,1))/6.25;
}

/* Comes from startup.S */
extern char rumboot_default_irq_vectors;
void rumboot_platform_setup()
{
	struct sp804_conf conf_str;

	conf_str.mode = FREERUN;
	conf_str.interrupt_enable = 1;
	conf_str.clock_division = 16;
	conf_str.width = 32;
	conf_str.load = 0;
	conf_str.bgload = 0;

	sp804_config(DIT3_BASE, &conf_str, 1);
	sp804_enable(DIT3_BASE, 1);
	/*
	 * Make sure VBAR is where we need it.
	 * Needed for handling IRQs in secondary image
	 */
	arm_vbar_set((uint32_t)&rumboot_default_irq_vectors);
}
