global start  : label;	// Стандартная точка входа.
global _exit  : label;	// Функция exit.
global __exit : label;	// Функция _exit.


weak _sync : label; // функция sync для варианта программного эмулятора

extern stop  : label; // Внешняя функция остановки программы.

extern _rumboot_main :label;
extern _easynmc_argc: word;
extern _easynmc_argv: word;
extern _rumboot_platform_stack_start: word;

begin text_init

         // Identification string
    CompilerString : long[4] =
            'NeuroMatrix(r) NM640x SDK v3.00';

        // место в памяти для хранения кода возврата программы
    result_code : word = -1;
    dummy : word = -1;

// Точка входа программы
<start>
    sp = _rumboot_platform_stack_start;								//  start +0
    gr7 = [_easynmc_argv];     /* argv */
    push gr7;      
    gr7 = [_easynmc_argc];     /* argc */
    push gr7;     
    call _rumboot_main;
    push ar7,gr7;

<terminate>
 //   call dtor;
    pop ar5,gr5;
    gr0 = gr5 with gr7 = gr5;
    [ result_code ] = gr7;
    goto stop;

//<_exit>
//    sp -= 2;
//    goto terminate;
//
//<__exit>
//    sp -= 2;
//    pop ar5,gr5;
//    gr0 = gr5 with gr7 = gr5;
//    [ result_code ] = gr7;
//    goto int_stop;



global _easynmc_send_LPINT: label;
global _easynmc_send_HPINT: label;
global stop: label;
	
<_easynmc_send_LPINT>
	push gr7;
	gr7 = 24h;
	nmscu = gr7;
	pop gr7;
	return;
	
<_easynmc_send_HPINT>
	push gr7;
	gr7 = 48h;
	nmscu = gr7;
	pop gr7;
	return;

<stop>
	goto 0h;

end text_init;