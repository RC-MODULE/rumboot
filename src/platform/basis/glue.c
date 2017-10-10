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

	rumboot_platform_raise_event(EVENT_TERM, 1);
    while (1) { };
}

static void dump32(enum rumboot_simulation_event event, void *pc)
{
	union u32 u;

	u.u = (uint32_t)pc;
	rumboot_platform_raise_event(event, u.byte[3]);
	rumboot_platform_raise_event(event, u.byte[2]);
	rumboot_platform_raise_event(event, u.byte[1]);
	rumboot_platform_raise_event(event, u.byte[0]);
}

void rumboot_platform_trace(void *pc)
{
    dump32(EVENT_TRACE, pc);
}

void rumboot_platform_perf(const char *tag)
{
    dump32(EVENT_PERF, (void *) tag);
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


#ifdef RUMBOOT_PRINTF_ACCEL
void __attribute__((optimize("-O0"))) rumboot_printf(const char *fmt, ...)
{
	/* va-arg func sends r0-r3 registers to stack in prologue
	 * 00000384 <rumboot_sim_printf>:
	 *  384:	e92d000f 	push	{r0, r1, r2, r3}
	 *  388:	e92d4800 	push	{fp, lr}
	 *  38c:	e28db004 	add	fp, sp, #4
	 */
	dump32(EVENT_PRINTF, __builtin_frame_address(0));
}
#endif

int rumboot_platform_getchar(uint32_t timeout_us)
{
    iowrite32(0x00, DGPIO_HIGH_BASE + DIR);
    rumboot_platform_raise_event(EVENT_STDIN, 0x0);
    uint8_t ret = ioread32(DGPIO_HIGH_BASE + DATA) & 0xff;
    iowrite32(0xff, DGPIO_HIGH_BASE + DIR);
    return ret;
}

void rumboot_platform_request_file(const char *plusarg, uint32_t addr)
{
    dump32(EVENT_UPLOAD, (void *) plusarg);
    dump32(EVENT_UPLOAD, (void *) addr);
}
