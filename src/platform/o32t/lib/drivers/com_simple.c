
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <rumboot/printf.h>

#include <rumboot/io.h>
#include <platform/regs/regs_com.h>
#include <platform/devices.h>
#include <platform/devices/com_simple.h>

 int comp_dma_run( uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1 ) {
    //write data
 iowrite32( COM_COUNT, base0 + MainCounter_tr ); //set dma total data 512 byte
	iowrite32( src_addr, base0 + Address_tr ); //dma source atart address
	iowrite32( 0x0, base0 + Bias_tr );
	iowrite32( 0x0, base0 + RowCounter_tr );
	iowrite32( 0x0, base0 + AddressMode_tr );	
	iowrite32( 0x3, base0 + InterruptMask_tr );
	
	iowrite32( COM_COUNT, base1 + MainCounter_rcv ); //set dma total data 512 byte
	iowrite32( dst_addr, base1 + Address_rcv ); //dma destination atart address	
	iowrite32( 0x0, base1 + Bias_rcv );
	iowrite32( 0x0, base1 + RowCounter_rcv );
	iowrite32( 0x0, base1 + AddressMode_rcv );	
	iowrite32( 0x3, base1 + InterruptMask_rcv );
	
	iowrite32(COM_CONTROL_EN,base0 + CSR_tr);
	// comp_delay (50);
	iowrite32(COM_CONTROL_EN,base1 + CSR_rcv);
    rumboot_printf( "check_run\n" );


	if (com_simple_wait_complete(base0, CSR_tr)) {
        rumboot_printf( "COM0_Cpl  timeout_1\n" );
        return 1;
    }
	if (com_simple_wait_complete(base1, CSR_rcv )) {
        rumboot_printf( "COM1_Cpl  timeout_1\n" );
        return 1;
    }
	return 0;
   }

void comp_delay( uint32_t timeout){
uint32_t count ;
do { count = timeout;
 rumboot_printf( "count=%d\n",count );
} while ( --timeout );	
}
static inline __attribute__((always_inline)) uint32_t wait_com_int_handled( uint32_t timeout, volatile uint32_t * const  flag ) {
    do {
        rumboot_printf( "COMPORT  wait interrupt Cpl\n" ); 
		if( *flag ) {
			// rumboot_printf( "*flag=%d\n",(*flag) );
			//  rumboot_printf( "flag=%d\n",(&flag) );
            *flag = 0;
            msync();

            return 1;
        }
    } while ( --timeout );

    return 0;
}


int comp_dma_irq_run( uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1, uint32_t COM0_Cpl,uint32_t COM1_Cpl ) {

 
	iowrite32( COM_COUNT,  base0 + MainCounter_tr ); //set dma total data 512 byte
	iowrite32( src_addr,  base0 + Address_tr ); //dma source atart address
	iowrite32( 0x0, base0 + Bias_tr );
	iowrite32( 0x0, base0 + RowCounter_tr );
	iowrite32( 0x0, base0 + AddressMode_tr );	
	iowrite32( 0x0, base0 + InterruptMask_tr );
	
	
	iowrite32( COM_COUNT, base1 + MainCounter_rcv ); //set dma total data 512 byte
	iowrite32( dst_addr, base1 + Address_rcv ); //dma destination atart address	
	iowrite32( 0x0, base1 + Bias_rcv );
	iowrite32( 0x0, base1 + RowCounter_rcv );
	iowrite32( 0x0, base1 + AddressMode_rcv );	
	iowrite32( 0x0, base1 + InterruptMask_rcv );
	

	iowrite32(COM_CONTROL_EN,base0 + CSR_tr);
	iowrite32(COM_CONTROL_EN,base1 + CSR_rcv);
    rumboot_printf( "check_irq_run\n" );
 

	if( !wait_com_int_handled( 1000, &COM0_Cpl) )
		{
			
    rumboot_printf( "COM0_Cpl interrupt timeout_1\n" );
        return 1;
    }
	 rumboot_printf( "COM0_Cpl_trm interrupt  is ended\n" );

	if( !wait_com_int_handled( 1000, &COM1_Cpl) ) {	
        rumboot_printf( "COM1_Cpl interrupt timeout_1\n" );
        return 1;
    }
	rumboot_printf( "COM1_Cpl_rcv interrupt is ended\n" );

    return 0;
} 
 

int com_simple_wait_complete(uintptr_t base, uintptr_t cntrl )
{
  uint32_t res;
  do{ res=ioread32(base+cntrl); } while(! ( res&COM_CONTROL_CPL ) );
  return 0;
}
