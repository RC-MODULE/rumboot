#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <rumboot/rumboot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <string.h>

void rumboot_putstring(const char *string)
{
#ifndef RUMBOOT_PRINTF_ACCEL
	while (*string)
		rumboot_putchar(*string++);
#else
	rumboot_printf(string);
#endif
}
