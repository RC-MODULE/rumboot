#include <rumboot/irq.h>
.extern _rumboot_main
.extern __main
.extern _rumboot_platform_stack_start

.global _start
.set _start, the_start

.section .text.init

the_start:
    ar7 = _rumboot_platform_stack_start; /* Stack grown UP, ar7 increases on every push */
    ar0=0;
    ar1=0;
    ar2=0;
    ar3=0;
    ar4=0;
    ar5=0;
    ar6=0;
    gr0=0;
    gr1=0;
    gr2=0;
    gr3=0;
    gr4=0;
    gr5=0;
    gr6=0;
    gr7=0;
    /* Fire up our main code loop */
#if defined(RUMBOOT_NOINIT)
	call __main;
#else
	call _rumboot_main;
#endif
    /* Should never get here */

dead_loop:
	delayed skip dead_loop;
	gr0 = gr0;
	gr0 = gr0;