#include <rumboot/printf.h>
#include <rumboot/version.h>
static const char rumboot_logo[] =
"▄▄▄  ▄• ▄▌• ▌ ▄ ·. ▄▄▄▄·             ▄▄▄▄▄\n"
"▀▄ █·█  █▌· █ ▐███▪▐█ ▀█▪▪     ▪     •██  \n"
"▐▀▀▄ █▌ █▌ █ ▌▐▌ █·▐█▀▀█▄ ▄█▀▄  ▄█▀▄  ▐█.▪\n"
"▐█•█▌▐█▄█▌ █  █▌ █▌██▄▪▐█▐█▌.▐▌▐█▌.▐▌ ▐█▌·\n"
".▀  ▀ ▀▀▀  ▀  █  ▀▀·▀▀▀▀  ▀█▄▀▪ ▀█▄▀▪ ▀▀▀ \n";

void rumboot_print_logo()
{
    rumboot_printf(rumboot_logo);
    const struct rumboot_version_info *inf = rumboot_version_info();
    rumboot_printf("     The Third Generation ROM Loader\n");
    rumboot_printf("     %s | %s | %s \n\n", inf->platform, CMAKE_BUILD_TYPE, inf->gitversion);
}

