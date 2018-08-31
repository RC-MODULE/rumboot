#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <stdlib.h>

int main()
{
    rumboot_printf("spl: hello, I'm the secondary image! I shouldn't be running\n");
    exit(1);
}
