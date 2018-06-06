//#include <mivem_test.h>

//#include <trace.h>
//#include <plb6bc.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/macros.h>
//#include <rumboot/trace.S.h>

//#include <test_event_c.h>
//#include <test_event_codes.h>

static bool check_cpu_ppc_im0_0() 
{
    //test_event( EVENT_CHECK_CPU_PPC_IM0_0 );
//    register uint32_t mem32_im0_base = ioread32( IM0_BASE );

    register uint32_t mem32_im0_base = ioread32( IM0_BASE );

    rumboot_printf("actual ioread32( IM0_BASE ) = \n");
    
    rumboot_printf(" Result %x", mem32_im0_base);
    
    rumboot_printf("expected ioread32( IM0_BASE ) = \n");
    //trace_msg( "expected MEM32( IM0_BASE ) = " ); 
    rumboot_printf("Result: %x", 0x7E57DA7A); 
    
    if( mem32_im0_base == 0x7E57DA7A ) return RESULT_OK;
    
    rumboot_putstring("IM0_0 is failed\n");
   ///rumboot_print_logo();
    return RESULT_ERROR;
}

static bool check_cpu_ppc_im0_1() {
    //test_event( EVENT_CHECK_CPU_PPC_IM0_1 );
    //iowrite32( IM0_BASE ) = 0xDEADBEAF;
    iowrite32(0xDEADBEAF, IM0_BASE);
    
    //register uint32_t mem32_im0_base = ioread32( IM0_BASE );

    register uint32_t mem32_im0_base = ioread32( IM0_BASE );

    rumboot_printf("actual MEM32( IM0_BASE ) = \n" ); 
    
    rumboot_printf("Result %x", mem32_im0_base );
    
    rumboot_printf("expected ioread32( IM0_BASE ) =  \n" ); 
    
    rumboot_printf( "Result: %x",  0x600DDA7A );
    
    if( mem32_im0_base == 0x600DDA7A ) return RESULT_OK;
    
    rumboot_printf( "IM0_1 is failed\n" );
    //rumboot_print_logo();
    return RESULT_ERROR;
}

int main(void) {
    if( check_cpu_ppc_im0_0() && check_cpu_ppc_im0_1() ) return RESULT_OK;
  //rumboot_print_logo();
  //rumboot_putstring("Helo\n");
  return RESULT_ERROR;
}