#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <stdlib.h>

#ifdef RUMBOOT_NATIVE
    #include <unistd.h>
    #include <signal.h>
#endif

#ifndef SIGNAL
    #define SIGNAL SIGUSR1
#endif

int main()
{
	rumboot_printf("spl: hello, I'm the secondary image\n");
    rumboot_printf("spl: I will exit with %d!\n", EXITCODE);

#ifdef TERMINATE_SIMULATION
    #ifndef RUMBOOT_NATIVE
        exit(EXITCODE);
    #else
	   rumboot_printf("spl: Will now send parent a signal\n");
       kill(getppid(), SIGNAL);
    #endif
#endif
    return EXITCODE;
}
