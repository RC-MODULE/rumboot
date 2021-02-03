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

#define INTC_IRR      (INTC_BASE + 0x0)
#define INTC_IRR_SET  (INTC_BASE + 0x8 )
#define INTC_IRR_CLR  (INTC_BASE + 0x10)
#define INTC_IMR      (INTC_BASE + 0x20)
#define INTC_IMR_SET  (INTC_BASE + 0x28)
#define INTC_IMR_CLR  (INTC_BASE + 0x30)
#define INTC_IMR_NULL (INTC_BASE + 0x38)


#define INTC_IRP      (INTC_BASE + 0x10)
#define INTC_IRP_SET  (INTC_BASE + 0x12)
#define INTC_IRP_CLR  (INTC_BASE + 0x14)
#define INTC_IRP_NULL (INTC_BASE + 0x16)
#define INTC_IAS      (INTC_BASE + 0x18)
#define INTC_IAS_CLR  (INTC_BASE + 0x1C)


int main(int argc, char** argv)
{  
        rumboot_irq_sei();
        iowrite32(0xffffffff, 0x38000);
        iowrite32(0xffffffff, 0x38008);
        rumboot_printf("Hello world!\n");
        rumboot_printf("Hello world!\n");
        rumboot_printf("Hello world!\n");
        rumboot_printf("Hello world!\n");
} 

