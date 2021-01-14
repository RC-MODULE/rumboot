#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>

int main()
{
    rumboot_printf("Hello world from IRAM!\n");
    return 0;
}
