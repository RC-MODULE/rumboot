
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <rumboot/printf.h>

#include <rumboot/io.h>
#include <platform/regs/regs_com.h>
#include <platform/devices.h>
#include <platform/devices/com_simple.h>


int comp_dma_run_tr( uint32_t addr, uint32_t base, uint32_t mask,	uint32_t count_num) {
    //write data
	iowrite32( count_num, base + MainCounter_tr ); //set dma total data 512 byte
	iowrite32( addr, base + Address_tr ); //dma source atart address
	iowrite32( 0x0, base + Bias_tr );
	iowrite32( 0x0, base + RowCounter_tr );
	iowrite32( 0x0, base + AddressMode_tr );	
	iowrite32( mask, base + InterruptMask_tr );
	
//iowrite32(COM_CONTROL_EN,base + CSR_tr);
return 0;
   }

int comp_dma_run_rcv( uint32_t addr, uint32_t base, uint32_t mask,	uint32_t count_num) {
    //write data
	iowrite32( count_num, base + MainCounter_rcv ); //set dma total data 512 byte
	iowrite32( addr, base + Address_rcv ); //dma destination atart address
	iowrite32( 0x0, base + Bias_rcv );
	iowrite32( 0x0, base + RowCounter_rcv );
	iowrite32( 0x0, base + AddressMode_rcv );	
	iowrite32( mask, base + InterruptMask_rcv );
	
	//iowrite32(COM_CONTROL_EN,base + CSR_rcv);
return 0;
   }
   
void comp_delay( uint32_t timeout){
uint32_t count ;
do { count = timeout;
 rumboot_printf( "count=%d\n",count );
} while ( --timeout );	
} 
  
int comp_dma_run( uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1,uint32_t count_num) {
	uint32_t result;
	uint32_t start;
	uint32_t end;
	uint32_t delta;
	
	result =comp_dma_run_tr(src_addr,base0,0x3,count_num );
	result =comp_dma_run_rcv(dst_addr,base1,0x3,count_num );
	
	iowrite32(COM_CONTROL_EN,base0+ CSR_tr);
	
//	#ifdef COM_DO_DELAY
//	comp_delay (25);
//	#endif
	
	iowrite32(COM_CONTROL_EN,base1	+ CSR_rcv);
	
	rumboot_printf( "check_run\n" );
	
	if (com_simple_wait_complete(base0, CSR_tr)) {
        rumboot_printf( "COM0_Cpl  timeout_1\n" );
        return 1;
    }
	
	
	if (com_simple_wait_complete(base1, CSR_rcv )) {
        rumboot_printf( "COM1_Cpl  timeout_1\n" );
        return 1;
    }
return result;
   }   


static inline __attribute__((always_inline)) uint32_t wait_com_int_handled( uint32_t timeout, volatile uint32_t * const  flag ) {
    do {
        rumboot_printf( "COMPORT  wait interrupt Cpl\n" ); 
		if( *flag ) {		
            *flag = 0;
            msync();

            return 1;
        }
    } while ( --timeout );

    return 0;
}
int comp_dma_irq_run( uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1, uint32_t COM0_Cpl,uint32_t COM1_Cpl, uint32_t count_num ) {
	uint32_t result;
	
	result = comp_dma_run_tr(src_addr,base0,0x0,count_num);	
	result = comp_dma_run_rcv(dst_addr,base1,0x0,count_num);
	
	iowrite32(COM_CONTROL_EN,base0 + CSR_tr);
	iowrite32(COM_CONTROL_EN,base1	+ CSR_rcv);	
	
	rumboot_printf( "check_run\n" );

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
int com_status(uint32_t base, uint32_t direct )
{ uint32_t result;

  uint32_t  COM_Mask_tr;
  uint32_t  COM_Mask_rcv;
  uint32_t  COM_Status_tr;
  uint32_t  COM_Status_rcv;
  uint32_t  COM_IRQ_TR_ERROR;
  uint32_t  COM_IRQ_RCV_ERROR;

  if (direct ==1) {
   COM_Mask_tr = ioread32	( base + InterruptMask_tr );
   COM_Status_tr = ioread32	( base + CSR_tr );
   result = ( (( (COM_Status_tr >> 1) & 0x1) ==1)&& ((COM_Mask_tr & 0x1) ==0)) ||
  (( ( (COM_Status_tr >> 2) & 0x1)==1) && (((COM_Mask_tr>> 2) & 0x1)==0) ) ;
  if (( ( COM_Status_tr & 0x4)==4) && (((COM_Mask_tr>> 2) & 0x1)==0) ) {
  COM_IRQ_TR_ERROR =1;
   rumboot_printf( "COM_IRQ_TR_ERROR= %d\n", COM_IRQ_TR_ERROR );
  }
  }
  else
  {COM_Mask_rcv = ioread32	( base + InterruptMask_rcv ); 
  COM_Status_rcv = ioread32	( base + CSR_rcv );
  result = (((( COM_Status_rcv >> 1 ) & 0x1)==1) && ((COM_Mask_rcv & 0x1) ==0)) ||
  ( ( ((COM_Status_rcv >> 2) & 0x1)==1) && (((COM_Mask_rcv>> 2) & 0x1) ==0));  
    if (( (COM_Status_rcv &0x4)  == 4) && (((COM_Mask_rcv>> 2) & 0x1) ==0)) {
	COM_IRQ_RCV_ERROR =1;
	rumboot_printf( "COM_IRQ_RCV_ERROR= %d\n", COM_IRQ_RCV_ERROR );
	}
  }
  
  return result;
} 
void clear_com_status(uint32_t base, uint32_t direct ) {
  uint32_t  COM_Mask_tr;
  uint32_t  COM_Mask_rcv;
  uint32_t  COM_Status_tr;
  uint32_t  COM_Status_rcv;	
 
      
 if (direct ==1) {
	  COM_Mask_tr = ioread32	( base + InterruptMask_tr );
	COM_Status_tr = ioread32	( base + CSR_tr );
	 if ( (( (COM_Status_tr >> 1) & 0x1) ==1)&& ((COM_Mask_tr & 0x1) ==0))  
	iowrite32((COM_Status_tr & 0xfffffffd),base + CSR_tr); 
	else if (( ( (COM_Status_tr & 0x4) & 0x1)==1) && ((COM_Mask_tr>> 2) & 0x1))
	iowrite32((COM_Status_rcv & 0xfffffffb),base + CSR_rcv);

 }
 if (direct ==0) {
  COM_Mask_rcv = ioread32	( base + InterruptMask_rcv);
	COM_Status_rcv = ioread32	( base + CSR_rcv);
	 if (((( COM_Status_rcv >> 1 ) & 0x1)==1) && ((COM_Mask_rcv & 0x1) ==0))
	iowrite32((COM_Status_rcv & 0xfffffffd),base + CSR_rcv); 
	else if (( ( (COM_Status_rcv & 0x4) & 0x1)==1) && (((COM_Mask_rcv>> 2) & 0x1)==0))
	iowrite32((COM_Status_rcv & 0xfffffffb),base + CSR_rcv);
 }
}