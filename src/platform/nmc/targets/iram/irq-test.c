#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/hexdump.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <rumboot/irq.h>



int main(int argc, char** argv)
{  
        rumboot_irq_sei();
        iowrite32(0x40, 0x38002<<2);
        iowrite32(0x40, 0x38008<<2);
        rumboot_printf("Hello world!\n");
        rumboot_printf("Hello world!\n");
        rumboot_printf("Hello world!\n");
        rumboot_printf("Hello world!\n");
} 

