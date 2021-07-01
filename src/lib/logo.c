#include <rumboot/printf.h>
#include <rumboot/version.h>

//static const char rumboot_logo[] =
//"▄▄▄  ▄• ▄▌• ▌  ▄·. ▄▄▄▄·             ▄▄▄▄▄\n"
//"▀▄ █·█  █▌· █ ▐███▪▐█ ▀█▪▪     ▪     •██  \n"
//"▐▀▀▄ █▌ █▌ █ ▌▐▌ █·▐█▀▀█▄ ▄█▀▄  ▄█▀▄  ▐█.▪\n"
//"▐█•█▌▐█▄█▌ █  █▌ █▌██▄▪▐█▐█▌.▐▌▐█▌.▐▌ ▐█▌·\n"
//".▀  ▀ ▀▀▀  ▀  █  ▀▀·▀▀▀▀  ▀█▄▀▪ ▀█▄▀▪ ▀▀▀ \n";

static const char rumboot_logo[] =
"\n\n\n"
"________                  ________            _____ \n"
"___  __ \\___  ________ ______  __ )_____________  /_\n"
"__  /_/ /  / / /_  __ `__ \\_  __  |  __ \\  __ \\  __/\n"
"_  _, _// /_/ /_  / / / / /  /_/ // /_/ / /_/ / /_  \n"
"/_/ |_| \\__,_/ /_/ /_/ /_//_____/ \\____/\\____/\\__/  \n";

void rumboot_print_logo()
{
    rumboot_printf(rumboot_logo);
    const struct rumboot_version_info *inf = rumboot_version_info();
    rumboot_printf("     The Third Generation ROM Loader\n");
    rumboot_printf("     %s | %s | %s \n\n", inf->platform, CMAKE_BUILD_TYPE, inf->gitversion);
}

