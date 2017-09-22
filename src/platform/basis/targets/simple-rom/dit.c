/*
 *  DUAL-TIMER (ARM SP804)  registers
 */
 

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <basis/defs_c.h>
#include <rumboot/dit_lib.h>

#define SIZE0           4
#define SIZE1           7


void oneshot_n_cycles_0(int cycles, int count_to)
{
        rumboot_printf("Control before EVVVVV : %x",rgTIMER0_CONTROL);

    oneshot0();
    for (int i = 0; i < cycles; i++)
    {
        load_value_0(count_to);
        rumboot_printf("Control after LOAD : %x",rgTIMER0_CONTROL);
//        start_0();
        rumboot_printf("Control after START : %x",rgTIMER0_CONTROL);

        int h = 1;
        do{h = check_value_0();
        rumboot_printf("Value = %d \n", h);}
        while(h)	;
        rumboot_printf("ROUND0 %d\n",i);
    }
}

void oneshot_n_cycles_1(int cycles, int count_to)
{
    oneshot1();
    for (int i = 0; i < cycles; i++)
    {
        load_value_1(count_to);
        start_1();
        int h = 1;
        do{h = check_value_1();}
        while(h)	;
        rumboot_printf("ROUND1 %d\n",i);
    }
}







int main ()
{
    rumboot_printf("Start of test\n");

    oneshot_n_cycles_0(SIZE0,9);
//    oneshot_n_cycles_1(SIZE1,15);
    
    
    
    return 0;
    
    
}