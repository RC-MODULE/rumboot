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

int check(void)
{
    int i;
    rumboot_putstring("Hello from IM11");
    for(i = 0; i < 10; i++)
        rumboot_puthex(i);
      return 0;

    return 1;
}
