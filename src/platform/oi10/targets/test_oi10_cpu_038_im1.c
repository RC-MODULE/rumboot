#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/platform.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>

int main(void) 
{
    rumboot_platform_request_file("IM1BIN", IM1_BASE);

    int (*check_function)();
    check_function = (void *)(IM1_BASE);
    rumboot_putstring("starting code from IM1\n");

      return check_function();

    return 1;
}
