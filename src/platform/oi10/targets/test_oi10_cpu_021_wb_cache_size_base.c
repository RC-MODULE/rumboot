#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/platform.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/devices/emi.h>
#include <rumboot/memfill.h>

#define M_BASE SRAM0_BASE

typedef bool func();

int main(void)
{
    test_event_send_test_id("test_oi10_cpu_021_wb_cache_size");

    rumboot_printf ("EMI init\n");
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_memfill8_modelling( (void*)M_BASE, 0x8000, 0x00, 0x00 ); //anti X fix

    rumboot_printf ("Load cache test function in 0x%x\n", M_BASE);
    rumboot_platform_request_file("MBIN", M_BASE); //M_BASE defined in cmake

    rumboot_printf("Call cache test function\n");
    func* test_func = (func*)(M_BASE);
    if ( !test_func() )
    {
        rumboot_printf ("TEST ERROR\n");
        return 1;
    }

    rumboot_printf ("TEST OK\n");
    return 0;
}
