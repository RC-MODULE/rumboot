#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>

int main()
{
    rumboot_printf("Hello, printing something every 100 us\n");
    int i = 0 ;
    while (1) {
        rumboot_printf("Hello, my uptime is %d\n", rumboot_platform_get_uptime());
        udelay(300);
    }    
}