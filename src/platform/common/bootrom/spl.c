#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <stdlib.h>
#include <rumboot/boot.h>

#ifdef RUMBOOT_NATIVE
#include <unistd.h>
#include <signal.h>
#endif


#ifdef OVERSIZED
const char __attribute__((used)) __attribute__((section(".data"))) huge_data[8 * 1024 * 1024];
#endif

void do_exit(int code) {
        #ifndef RUMBOOT_NATIVE
		uint32_t c = code;
                rumboot_platform_event_raise(EVENT_TERM, &c, 1);
        #else
                rumboot_printf("spl: Will now send parent a signal: %d\n", code ? SIGUSR2 : SIGUSR1);
                kill(getppid(), code ? SIGUSR2 : SIGUSR1);
                usleep(1000);
                exit(0);
        #endif
}

int main()
{
        rumboot_platform_runtime_info->persistent[0]++;
        rumboot_printf("spl: executing spl #%d\n",
                       rumboot_platform_runtime_info->persistent[0]
                       );
        rumboot_printf("spl: I will exit with %d!\n", EXITCODE);

#ifdef SPL_COUNT_CHECK
        if (rumboot_platform_runtime_info->persistent[0] != SPL_COUNT_CHECK) {
                rumboot_printf("spl: Execution counter check failed!\n");
                do_exit(1);
        }
#endif

#ifdef CRASH_NPE
        volatile uint32_t *ptr = (uint32_t *) 0;
        *ptr = 0xdeadc0de;
#endif

#ifdef TERMINATE_SIMULATION
        do_exit(EXITCODE);
#endif
        return EXITCODE;
}
