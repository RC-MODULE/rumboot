#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <platform/easynmc/easynmc.h>
#include <stdint.h>
#include <devices/greth_edcl.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>


void check_addr(void *ptr)
{
        uint32_t pp = (uintptr_t) ptr;
        rumboot_printf("addr 0x%x traslates to 0x%x\n", 
                pp, rumboot_virt_to_dma(pp)
        );
}

int main(int argc, char** argv)
{  
        rumboot_printf("Checking address translation\n");
        uint32_t tmp;
        void *theptr = rumboot_malloc_from_named_heap("IM2", 10);
        if (!theptr) {
                rumboot_platform_panic("Compiler bug spotted. This won't work");
        }

        check_addr(rumboot_malloc_from_named_heap("IM1", 10));
        check_addr(rumboot_malloc_from_named_heap("IM2", 10));
        check_addr(rumboot_malloc_from_named_heap("IM3", 10));
        check_addr(rumboot_malloc_from_named_heap("SRAM", 10));

        check_addr(rumboot_malloc_from_named_heap("IM1", 10));
        check_addr(rumboot_malloc_from_named_heap("IM2", 10));
        check_addr(rumboot_malloc_from_named_heap("IM3", 10));
        check_addr(rumboot_malloc_from_named_heap("SRAM", 10));

        return 0;
} 

