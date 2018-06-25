#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

int main()
{
    rumboot_platform_sim_save("data.bin");
    rumboot_platform_sim_restore("data.bin");
	return 0;
}
