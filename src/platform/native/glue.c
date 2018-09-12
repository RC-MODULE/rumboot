#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>

/* HACK: dummy variables */
char rumboot_platform_spl_start;
char rumboot_platform_spl_end;

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
    return 0;
}

void __attribute__((noreturn)) rumboot_platform_panic(const char *why, ...)
{
    va_list ap;
    va_start(ap, why);
    printf("PANIC: ");
    vprintf(why, ap);
    va_end(ap);
    exit(1);
}

void rumboot_platform_trace(void *pc)
{
    /* stack tracing code here */
}

void rumboot_platform_event_raise(enum rumboot_simulation_event event, uint32_t *data, uint32_t len)
{
    exit(event);
}

void rumboot_platform_putchar(uint8_t c)
{
    putc(c, stdout);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
    return (uint8_t) getc(stdin);
}

struct rumboot_runtime_info rumboot_platform_runtime_info;


uint32_t rumboot_arch_irq_disable()
{
    return 0;
}

uint32_t rumboot_arch_irq_enable()
{
    return 0;
}

void rumboot_platform_request_file(const char *plusarg, uint32_t addr)
{

}

void rumboot_platform_dump_region(const char *filename, uint32_t addr, uint32_t len)
{

}

void rumboot_platform_perf(const char *tag)
{

}

const struct rumboot_bootsource *rumboot_platform_get_bootsources()
{
    return NULL;
}

bool rumboot_platform_check_entry_points(struct rumboot_bootheader *hdr)
{
	/* Any entry point is okay */
	return true;
}

void rumboot_platform_exec(struct rumboot_bootheader *hdr)
{
	/* No-op, this chip has only one core */
}

uint32_t rumboot_virt_to_dma(volatile void *addr)
{
    return (uint32_t) addr;
}

