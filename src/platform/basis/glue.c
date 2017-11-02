#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <arch/arm/irq_macros.h>
#include <platform/devices.h>
#include <rumboot/dit_lib.h>

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
    return (0xFFFFFFFF-sp804_get_value(DIT3_BASE,1))/6.25;
}

#define DGPIO_LOW_BASE      0x01098000
#define DGPIO_HIGH_BASE     0x01099000

#define DATA                0x000C
#define DIR                 0x0008
#define SOURCE              0x0024


/* Comes from startup.S */
extern char rumboot_default_irq_vectors;
void rumboot_platform_setup() {
    iowrite32(0xff, DGPIO_LOW_BASE + DIR);
    iowrite32(0xff, DGPIO_LOW_BASE + SOURCE);
    iowrite32(0x00, DGPIO_LOW_BASE + DATA);
    iowrite32(0xff, DGPIO_HIGH_BASE + DIR);
    iowrite32(0xff, DGPIO_HIGH_BASE + SOURCE);
    iowrite32(0x00, DGPIO_HIGH_BASE + DATA);
//    sp804_freerun_1(0,1,16,1);

  struct sp804_conf conf_str;
  conf_str.mode             = FREERUN    ;
  conf_str.interrupt_enable = 1          ;
  conf_str.clock_division   = 16         ;
  conf_str.width            = 32         ;
  conf_str.load             = 0          ;
  conf_str.bgload           = 0          ;
//
  sp804_config(DIT3_BASE,&conf_str,1);
  sp804_enable(DIT3_BASE,1);  
    /*
     * Make sure VBAR is where we need it.
     * Needed for handling IRQs in secondary image
     */
    arm_vbar_set((uint32_t) &rumboot_default_irq_vectors);
}

union u32 {
    uint32_t u;
    char byte[4];
};

void __attribute__((noreturn)) rumboot_platform_panic(const char *why, ...)
{
	uint32_t *fp = __builtin_frame_address(0);
	int depth = 32;

	if (why) {
        va_list ap;
        rumboot_printf("PANIC: ");
    	va_start(ap, why);
        rumboot_vprintf(why, ap);
    	va_end(ap);
    }

	while (depth--) {
        /* All the stack frames should be in the stack area and we should check it
         * unless we want to be trappe in a recursive fault
         */
        if ((fp < (uint32_t *) &rumboot_platform_stack_area_start) ||
            (fp > (uint32_t *) &rumboot_platform_stack_area_end)) {
                rumboot_printf("PANIC: No more valid frames\n");
                break;
            }
		uint32_t pc = *(((uint32_t*)fp) + 0);
		uint32_t lr = *(((uint32_t*)fp) + 1);
	 	fp = (uint32_t *) (lr);
        rumboot_platform_trace((void *) (pc - 4 ));
		if (fp == 0)
			break;
	}

    exit(1);
    while (1) { };
}
