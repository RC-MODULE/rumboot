.extern _rumboot_main
.extern _rumboot_platform_stack_start

.global start
.set start, the_start

.section .text.init
the_start:
    gr0 = _rumboot_platform_stack_start;
    gr0 = _rumboot_platform_stack_start;
    gr0 = _rumboot_platform_stack_start;

//    ar7 = _rumboot_platform_stack_start;
	delayed goto _rumboot_main;
	nul 32;


#if 0
halt_code:
.align 3
	delayed halt;
	gr0 -= gr0;
	[0h] = gr0;
#endif
dead_loop:
	delayed skip dead_loop;
	gr0 = gr0;
	gr0 = gr0;
