/*
 *  DUAL-TIMER (ARM SP804)  registers
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <devices/dit.h>
#include <rumboot/dit_lib.h>

#define SIZE0           4
#define SIZE1           7





int main ()
{
    rumboot_printf("Start of test \n");
 //   oneshot_n_cycles_0(SIZE0,66);
 //   oneshot_n_cycles_1(SIZE0,30);
 //   periodic_n_cycles_0(SIZE0,20);
 //   freerun_0(1000);
 //   freerun_0(1000);
 //   freerun_1(900);
 //     periodic_0(1100,800);
      periodic_1(5100,500);


    return 0;   
}