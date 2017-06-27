#include <rumboot/printf.h>
static const char rumboot_logo[] =
"\n\n\n"
"    RC Module's          __                __ \n"
"   _______  ______ ___  / /_  ____  ____  / /_\n"
"  / ___/ / / / __ `__ \\/ __ \\/ __ \\/ __ \\/ __/\n"
" / /  / /_/ / / / / / / /_/ / /_/ / /_/ / /_  \n"
"/_/   \\__,_/_/ /_/ /_/_.___/\\____/\\____/\\__/  \n";

static const char rumboot_host_logo[] = ""
"\n";

static const char version[] = ROMBOOT_PLATFORM " | " ROMBOOT_BUILD_TYPE " | " ROMBOOT_VERSION "\n";

void rumboot_print_logo()
{
    rumboot_putstring(rumboot_logo);
    rumboot_putstring(version);
}
