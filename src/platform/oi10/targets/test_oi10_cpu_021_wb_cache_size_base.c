#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/platform.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>
#include <arch/ppc_476fp_mmu.h>
#include <devices/ugly/emi.h>
#include <rumboot/memfill.h>


typedef bool func( void );

int main(void) {
    rumboot_printf( "EMI init\n" );
    emi_init( DCR_EM2_EMI_BASE );

    rumboot_printf( "Load cache test function in 0x%x\n", SRAM0_BASE );
    rumboot_platform_request_file( "MBIN", SRAM0_BASE );

    rumboot_printf("Call cache test function\n");
    func* test_func = (func*)(SRAM0_BASE);
    if ( !test_func() ) {
        rumboot_printf( "TEST ERROR\n" );
        return 1;
    }

    rumboot_printf( "TEST OK\n" );
    return 0;
}
