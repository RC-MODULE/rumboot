#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
    return 0;
}

#define DGPIO_LOW_BASE      0x01036000
#define DGPIO_HIGH_BASE     0x01037000
#define DIR                 0x0400
#define DATA                0x03FC

void rumboot_platform_setup() {
    iowrite32(0xff, DGPIO_LOW_BASE + DIR);
    iowrite32(0x00, DGPIO_LOW_BASE + DATA);
    iowrite32(0xff, DGPIO_HIGH_BASE + DIR);
    iowrite32(0x00, DGPIO_HIGH_BASE + DATA);
    rumboot_platform_raise_event(EVENT_PERF, 0);
}

union u32 {
    uint32_t u;
    char byte[4];
};

void void __attribute__((noreturn)) rumboot_platform_panic(const char *why, ...)
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
    /* Let's walk the stack all the way down */
	while (depth--) {
		uint32_t pc = *(((uint32_t*)fp) + 0);
		uint32_t lr = *(((uint32_t*)fp) + 1);
	 	fp = (uint32_t *) (lr);
        rumboot_platform_trace((void *) (pc - 4 ));
		if (fp == 0)
			break;
	}

	rumboot_platform_raise_event(EVENT_CRASH, 0);
    while (1) { };
}

void rumboot_platform_trace(void *pc)
{
    union u32 u;
    u.u = (uint32_t) pc;
    rumboot_platform_raise_event(EVENT_TRACE, u.byte[3]);
    rumboot_platform_raise_event(EVENT_TRACE, u.byte[2]);
    rumboot_platform_raise_event(EVENT_TRACE, u.byte[1]);
    rumboot_platform_raise_event(EVENT_TRACE, u.byte[0]);
}

void rumboot_platform_raise_event(enum rumboot_simulation_event event, uint8_t arg)
{
    iowrite32(arg, DGPIO_HIGH_BASE + DATA);
    iowrite32(0, DGPIO_LOW_BASE + DATA);
    iowrite32((event << 1) | 1, DGPIO_LOW_BASE + DATA);
}

void rumboot_platform_putchar(uint8_t c)
{
    rumboot_platform_raise_event(EVENT_STDOUT, c);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
    iowrite32(0x00, DGPIO_HIGH_BASE + DIR);
    rumboot_platform_raise_event(EVENT_STDIN, 0x0);
    uint8_t ret = ioread32(DGPIO_HIGH_BASE + DATA) & 0xff;
    iowrite32(0xff, DGPIO_HIGH_BASE + DIR);
    return ret;
}
