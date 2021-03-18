#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/memfill.h>
#include <rumboot/io.h>
#include <rumboot/macros.h>
#include <arch/irq_macros.h>

void __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t c)
{
	RUMBOOT_ATOMIC_BLOCK() {
		rumboot_platform_runtime_info->out.data[0] = c;
		rumboot_platform_runtime_info->out.opcode = EVENT_STDOUT;
	}
}

int __attribute__((no_instrument_function)) rumboot_platform_getchar(uint32_t timeout_us)
{
	return -1;
}
