
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/regs_com.h>
#include <platform/devices.h>
#include <devices/com_simple.h>

#include <rumboot/rumboot.h>
#include <rumboot/platform.h>

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


            return 1;
        }
    } while ( --timeout );

    return 0;
}
int comp_dma_irq_run( uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1, uint32_t COM0_Cpl,uint32_t COM1_Cpl, uint32_t count_num ) {
	uint32_t result;
	
	result = comp_dma_run_tr(src_addr,base0,0x0,count_num);	
	result = comp_dma_run_rcv(dst_addr,base1,0x0,count_num);
	//result = comp_dma_run_rcv(dst_addr,base1,0x0,count_num);
	
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
 
    return result;
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
  (( ( (COM_Status_tr >> 2) & 0x1)==1) && (((COM_Mask_tr>> 1) & 0x1)==0) ) ;
  if (( (( COM_Status_tr >> 2) & 0x1)==1) && (((COM_Mask_tr>> 1) & 0x1)==0) ) {
  COM_IRQ_TR_ERROR =1;
   rumboot_printf( "COM_IRQ_TR_ERROR= %d\n", COM_IRQ_TR_ERROR );
  }
  }
  else
  {COM_Mask_rcv = ioread32	( base + InterruptMask_rcv ); 
  COM_Status_rcv = ioread32	( base + CSR_rcv );
  result = (((( COM_Status_rcv >> 1 ) & 0x1)==1) && ((COM_Mask_rcv & 0x1) ==0)) ||
  ( ( ((COM_Status_rcv >> 2) & 0x1)==1) && (((COM_Mask_rcv>> 1) & 0x1) ==0));  
    if (( ((COM_Status_rcv >> 2)& 0x1)  == 1) && (((COM_Mask_rcv>> 1) & 0x1) ==0)) {
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
	COM_Mask_tr = ioread32( base + InterruptMask_tr );
	COM_Status_tr = ioread32( base + CSR_tr );
	 if ( (( (COM_Status_tr >> 1) & 0x1) ==1)&& ((COM_Mask_tr & 0x1) ==0))  
	iowrite32((COM_Status_tr & 0xd),base + CSR_tr); 
	if (( ( (COM_Status_tr >> 2) & 0x1)==1) && ((COM_Mask_tr>> 1) & 0x1))
	iowrite32((COM_Status_tr & 0xb),base + CSR_tr);
	iowrite32((COM_Status_tr & 0x3),base + CSR_tr); //set CLR 
 }
 if (direct ==0) {
	COM_Mask_rcv = ioread32( base + InterruptMask_rcv);
	COM_Status_rcv = ioread32	( base + CSR_rcv);
	 if (((( COM_Status_rcv >> 1 ) & 0x1)==1) && ((COM_Mask_rcv & 0x1) ==0))		 
	 {iowrite32((COM_Status_rcv & 0xd),base + CSR_rcv);
     iowrite32((COM_Status_rcv & 0x9),base + CSR_rcv);}
	 if (( ( (COM_Status_rcv >> 2) & 0x1)==1) && (((COM_Mask_rcv >> 1) & 0x1)==0))
	 {iowrite32((COM_Status_rcv & 0xb),base + CSR_rcv);
	 iowrite32((COM_Status_rcv & 0x9),base + CSR_rcv); } 
	 
 }
 
}

int com_simple_write(uintptr_t base, com_cfg_t * cfg)
{
  iowrite32(cfg->MainCounter_WR    	,base+MainCounter_tr );
  iowrite32(cfg->Address_WR       	,base+Address_tr    );
  iowrite32(cfg->Bias_WR			,base+Bias_tr  );
  iowrite32(cfg->RowCounter_WR		,base+RowCounter_tr );
  iowrite32(cfg->AddressMode_WR    	,base+AddressMode_tr );
  iowrite32(cfg->InterruptMask_WR   ,base+InterruptMask_tr );
  iowrite32(cfg->MainCounter_RD    	,base+MainCounter_rcv );
  iowrite32(cfg->Address_RD       	,base+Address_rcv    );
  iowrite32(cfg->Bias_RD 			,base+Bias_rcv  );
  iowrite32(cfg->RowCounter_RD		,base+RowCounter_rcv );
  iowrite32(cfg->AddressMode_RD    	,base+AddressMode_rcv );
  iowrite32(cfg->InterruptMask_RD  	,base+InterruptMask_rcv );
  
  return 0;
}
int com_apb_check(uintptr_t base, com_cfg_t * cfg)
{
   uint32_t result;
    uint32_t tmp;
 
   result = com_simple_write( base, cfg);
    if (result) {
		 rumboot_printf("COMMPORT_WRITE  ERROR\n");
		return 1;
	}

  if (ioread32(base+MainCounter_tr) != cfg->MainCounter_WR)
  {
		rumboot_printf("COMMPORT_MainCounter_tr read ERROR\n");
		tmp= (cfg->MainCounter_WR);
		rumboot_printf("COMMPORT_MainCounter_write =%x\n",tmp); 
		rumboot_printf("COMMPORT_MainCounter read =%x\n",ioread32(base+MainCounter_tr) );
	  
      return 1;
  }
  if (ioread32(base+Address_tr) != cfg->Address_WR)
  {
		rumboot_printf("COMMPORT_Address_tr read ERROR\n");
		tmp= (cfg->Address_WR);
		rumboot_printf("COMMPORT_Address_tr_write =%x\n",tmp);
		rumboot_printf("COMMPORT_Address_tr read =%d\n",ioread32(base+Address_tr) );
      return 1;
  }
  if (ioread32(base+Bias_tr) != cfg->Bias_WR)
  {
		rumboot_printf("COMMPORT_Bias_tr read ERROR\n");
		tmp= (cfg->Bias_WR);
		rumboot_printf("COMMPORT_Bias_WR_write =%x\n",tmp); 
		rumboot_printf("COMMPORT_Bias_tr =%d\n",ioread32(base+Bias_tr) );
      return 1;   
  }      
  if (ioread32(base+RowCounter_tr) != cfg->RowCounter_WR)
  {
      rumboot_printf("COMMPORT_RowCounter_tr read ERROR\n");
	  rumboot_printf("COMMPORT_RowCounter_tr =%d\n",ioread32(base+RowCounter_tr) );
      return 1;
  }
 if (ioread32(base+AddressMode_tr) != cfg->AddressMode_WR)
  {
      rumboot_printf("COMMPORT_AddressMode_tr read ERROR\n");
       rumboot_printf("COMMPORT_AddressMode_tr  =%d\n",ioread32(base+AddressMode_tr) );
	  return 1;
  }
  if (ioread32(base+InterruptMask_tr) != cfg->InterruptMask_WR)
  {
      rumboot_printf("COMMPORT_InterruptMask_tr read ERROR\n");
      return 1;
  }

 if (ioread32(base+MainCounter_rcv) != cfg->MainCounter_RD)
  {
      rumboot_printf("COMMPORT_MainCounter_rcv read ERROR\n");
      return 1;
  }
  if (ioread32(base+Address_rcv) != cfg->Address_RD)
  {
      rumboot_printf("COMMPORT_Address_rcv read ERROR\n");
      return 1;
  }
  if (ioread32(base+Bias_rcv) != cfg->Bias_RD)
  {
      rumboot_printf("COMMPORT_Bias_rcv read ERROR\n");
      return 1;   
  }      
  if (ioread32(base+RowCounter_tr) != cfg->RowCounter_RD)
  {
      rumboot_printf("COMMPORT_RowCounter_rcv read ERROR\n");
      return 1;
  }
 if (ioread32(base+AddressMode_rcv) != cfg->AddressMode_RD)
  {
      rumboot_printf("COMMPORT_RowCounter_rcv read ERROR\n");
      return 1;
  }
  if (ioread32(base+InterruptMask_rcv) != cfg->InterruptMask_RD)
  {
      rumboot_printf("COMMPORT_InterruptMask_tr read ERROR\n");
      return 1;
  }
  
  return 0;
}

int com_apb_running_one(uintptr_t base, uint32_t COM_Address, uint32_t mask, int end_number )
{
    uint32_t data = 1;

	
    for ( int i = 0; i<end_number; i++)
    {
        iowrite32((data & mask)    ,base+COM_Address);
        if (ioread32(base+COM_Address) != (data & mask))
        {
            rumboot_printf("COMMPORT apb running 1 ERROR, data %x", data);
            return 1;
        }
        data = (data << 1);
    }
    return 0;
}
