#include <rumboot/printf.h>
#include <rumboot/version.h>
static const char rumboot_logo[] =
"\n\n\n"
" ,ggggggggggg,                               ,ggggggggggg,\n"
"dP\"\"\"88\"\"\"\"\"\"Y8,                            dP\"\"\"88\"\"\"\"\"\"Y8,                        I8\n"
"Yb,  88      `8b                            Yb,  88      `8b                        I8   \n"
" `\"  88      ,8P                             `\"  88      ,8P                     88888888\n"
"     88aaaad8P\"                                  88aaaad8P\"                         I8   \n"
"     88\"\"\"\"Yb,    gg      gg   ,ggg,,ggg,,ggg,   88\"\"\"\"Y8ba    ,ggggg,    ,ggggg,   I8\n"
"     88     \"8b   I8      8I  ,8\" \"8P\" \"8P\" \"8,  88      `8b  dP\"  \"Y8gggdP\"  \"Y8gggI8\n"
"     88      `8i  I8,    ,8I  I8   8I   8I   8I  88      ,8P i8'    ,8I i8'    ,8I ,I8,  \n"
"     88       Yb,,d8b,  ,d8b,,dP   8I   8I   Yb, 88_____,d8',d8,   ,d8',d8,   ,d8',d88b, \n"
"     88        Y88P'\"Y88P\"`Y88P'   8I   8I   `Y888888888P\"  P\"Y8888P\"  P\"Y8888P\"  8P\"\"Y8\n";
                                                                                         
                                                                                         
            

static const char rumboot_host_logo[] = ""
"\n";

//static const char version[] = RUMBOOT_PLATFORM " | " CMAKE_BUILD_TYPE " | " RUMBOOT_VERSION "\n";

void rumboot_print_logo()
{
    rumboot_printf(rumboot_logo);
    const struct rumboot_version_info *inf = rumboot_version_info();
    rumboot_printf("     The Third Generation ROM Loader\n");
    rumboot_printf("     %s | %s | %s \n\n", inf->platform, CMAKE_BUILD_TYPE, inf->gitversion);
}

void rumboot_print_host_logo()
{
    rumboot_printf(rumboot_host_logo);
    rumboot_printf("Host mode\n");
}
