
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>

#include <rumboot/irq.h>
#include <platform/interrupts.h>
#include <arch/ppc/arch/irq_macros.h>

#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/regs/regs_com.h>
#include <platform/devices/com_simple.h>

#include <arch/ppc/arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc/arch/ppc_476fp_mmu.h>


#define ARR_SIZE 128

#ifndef COM_SRC_HEAP
#define COM_SRC_HEAP "IM2"
#endif
#ifndef COM_DST_HEAP
#define COM_DST_HEAP "IM2"
#endif

void prepare_arrays( uint32_t ** src, uint32_t ** dst ) {
    *src = rumboot_malloc_from_named_heap_aligned( COM_SRC_HEAP, sizeof(uint32_t)*ARR_SIZE, sizeof(uint64_t) );
    *dst = rumboot_malloc_from_named_heap_aligned( COM_DST_HEAP, sizeof(uint32_t)*ARR_SIZE,sizeof(uint64_t) );

	
    for( uint32_t i = 0; i < ARR_SIZE; i++ )
        (*src)[ i ] = i + ( (i+1) << 8 ) + ( (i+2) << 16 ) + ( (i+3) << 24 );
    msync();
}

static volatile uint32_t COM0_Cpl_tr;
static volatile uint32_t COM0_Cpl_rcv;
static volatile uint32_t COM1_Cpl_tr;
static volatile uint32_t COM1_Cpl_rcv;
static volatile uint32_t COM0_ES_tr=0;
static volatile uint32_t COM0_ES_rcv=0;
static volatile uint32_t COM1_ES_tr=0;
static volatile uint32_t COM1_ES_rcv=0;
static volatile uint32_t COMMP0_COMMP1_IRQ = 0;
static volatile uint32_t COMMP0_COMMP1_IRQ_ERROR = 0;

static void handler1() {

	uint32_t  COM0_Mask_tr = ioread32	( COM0_BASE + InterruptMask_tr ); //Read interrupt mask
	uint32_t  COM0_Mask_rcv = ioread32	( COM0_BASE + InterruptMask_rcv ); //Read interrupt mask
	uint32_t  COM1_Mask_tr = ioread32	( COM1_BASE + InterruptMask_tr ); //Read interrupt mask
	uint32_t  COM1_Mask_rcv = ioread32	( COM1_BASE + InterruptMask_rcv ); //Read interrupt mask
	uint32_t  COM0_Status_tr = ioread32	( COM0_BASE + CSR_tr ); //Read interrupt status
	uint32_t  COM0_Status_rcv = ioread32( COM0_BASE + CSR_rcv ); //Read interrupt status
	uint32_t  COM1_Status_tr = ioread32	( COM1_BASE + CSR_tr ); //Read interrupt status
	uint32_t  COM1_Status_rcv = ioread32( COM1_BASE + CSR_rcv ); //Read interrupt status
	
	
    if( ( COM0_Status_tr >> 1 ) & ((COM0_Mask_tr & 0x1) ==0) ) {				 
       COM0_Cpl_tr = 1;
	   	rumboot_printf( "COM0_Cpl_tr= %d\n",COM0_Cpl_tr );
		//rumboot_printf( "COM0_Status_trr= %d\n",( COM0_Status_tr >> 1 ) );
		//rumboot_printf( "COM0_Status_tr= %d\n",( ( COM0_Status_tr >> 1 ) & ((COM0_Mask_tr & 0x1) ==0) ) );
		//rumboot_printf( "COM0_Mask_tr= %d\n",((COM0_Mask_tr & 0x1) ==0));		
		iowrite32((COM0_Status_tr & 0xfffffffd),COM0_BASE + CSR_tr);
		COMMP0_COMMP1_IRQ = 80;//CP1_RCV_INT;
    }
    if( ( COM0_Status_tr >> 2 ) &(((COM0_Mask_tr >> 2) & 0x1)==0)){
        COM0_ES_tr = 1;
		rumboot_printf( "COMPORT ERROR COM0_ES_tr= %d\n",COM0_ES_tr );
	   iowrite32((COM0_Status_tr & 0xfffffffb),COM0_BASE + CSR_tr);
		COMMP0_COMMP1_IRQ = 80;//CP1_RCV_INT;	
		COMMP0_COMMP1_IRQ_ERROR =1;
		rumboot_printf( "COMMP0_COMMP1_IRQ_TR_ERROR= %d\n",COMMP0_COMMP1_IRQ_ERROR );
    }
		 
if( ( COM1_Status_rcv >> 1 ) & ((COM1_Mask_rcv & 0x1) ==0)){
       COM1_Cpl_rcv = 1;	  
		rumboot_printf( "COM1_Cpl_rcv= %d\n",COM1_Cpl_rcv );
	   	iowrite32((COM1_Status_rcv & 0xfffffffd),COM1_BASE + CSR_rcv);
		COMMP0_COMMP1_IRQ = 81;// CP1_TRM_INT;

    }
	

	if( ( COM1_Status_rcv	>> 2) &	(((COM1_Mask_rcv >> 2) & 0x1)==0)) {
        COM1_ES_rcv = 1;
	   iowrite32((COM1_Status_rcv & 0xfffffffb),COM1_BASE + CSR_rcv);	   
		COMMP0_COMMP1_IRQ =81; //CP1_TRM_INT;
		COMMP0_COMMP1_IRQ_ERROR =1;
		rumboot_printf( "COMMP0_COMMP1_IRQ_RCV_ERROR= %d\n",COMMP0_COMMP1_IRQ_ERROR );
    }
			    
	
     if( ( COM1_Status_tr >> 1 ) & ((COM1_Mask_tr & 0x1) ==0 )){
       COM1_Cpl_tr = 1;
	   rumboot_printf( "COM1_Cpl_tr= %d\n",COM1_Cpl_tr );
	
		iowrite32((COM1_Status_tr & 0xfffffffd),COM1_BASE + CSR_tr);	   
		COMMP0_COMMP1_IRQ = 78;//CP0_RCV_INT; 
	 }	
    if( ( COM1_Status_tr >> 2 ) & ((COM1_Mask_tr >> 2) & 0x1 ==0)){
        COM1_ES_tr = 1;
		iowrite32((COM1_Status_tr & 0xfffffffb),COM1_BASE + CSR_tr);
		COMMP0_COMMP1_IRQ = 78;//CP0_RCV_INT;		
		COMMP0_COMMP1_IRQ_ERROR =1;
		rumboot_printf( "COMMP1_COMMP0_IRQ_TR_ERROR =%d\n",COMMP0_COMMP1_IRQ_ERROR );
    }
	
		 
	 if( ( COM0_Status_rcv >> 1 ) & ((COM0_Mask_rcv & 0x1) ==0 )){
        COM0_Cpl_rcv = 1;
		   	rumboot_printf( "COM0_Cpl_rcv= %d\n",COM0_Cpl_rcv );

	   	iowrite32((COM0_Status_rcv & 0xfffffffd),COM0_BASE + CSR_rcv);
		COMMP0_COMMP1_IRQ = 0;
    }
   if( ( COM0_Status_rcv	>> 2 ) & ((COM0_Mask_rcv >> 2) & 0x1 ==0)){
        COM0_ES_rcv  = 1;
	   iowrite32((COM0_Status_tr & 0xfffffffb),COM0_BASE + CSR_rcv);
		COMMP0_COMMP1_IRQ = 0;
		COMMP0_COMMP1_IRQ_ERROR =1;
		rumboot_printf( "COMMP1_COMMP0_IRQ_RCV_ERROR= %d\n",COMMP0_COMMP1_IRQ_ERROR );
    }
	    
	
    msync();

}



static struct rumboot_irq_entry * init_irq() {
    rumboot_irq_cli();
    struct rumboot_irq_entry * const tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, COMMP0_COMMP1_IRQ, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	COMMP0_COMMP1_IRQ = 80;
    rumboot_irq_set_handler( tbl, COMMP0_COMMP1_IRQ, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	COMMP0_COMMP1_IRQ = 81;
    rumboot_irq_set_handler( tbl,COMMP0_COMMP1_IRQ, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	COMMP0_COMMP1_IRQ = 78;
    rumboot_irq_set_handler( tbl, COMMP0_COMMP1_IRQ, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
    rumboot_irq_table_activate( tbl );
	rumboot_irq_enable(COMMP0_COMMP1_IRQ);
    rumboot_irq_sei();
    return tbl;
}


static void deinit_irq( struct rumboot_irq_entry * const tbl ) {
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

// Crutch Until This Will Be Made For All Tests Accurately
static struct tlb_entry const em2_nospeculative_tlb_entries[] =
{
/*   MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
    {MMU_TLB_ENTRY(  0x020,  0xC0304,    0xC0304,    MMU_TLBE_DSIZ_16KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_UND,   0b1 )}
};

int main()
{
  int result = 0;
  uint32_t * src0;
  uint32_t * dst0;
  uint32_t * src1;
  uint32_t * dst1;
  rumboot_printf("Start COMMPORT\n");
  
  write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));
	prepare_arrays( &src0, &dst0 );
    prepare_arrays( &src1, &dst1 );
	COMMP0_COMMP1_IRQ = 79;
  struct rumboot_irq_entry * tbl = init_irq(); 
  
    if( (comp_dma_irq_run( rumboot_virt_to_dma(src0), rumboot_virt_to_dma(dst0),COM0_BASE,COM1_BASE,&COM0_Cpl_tr,&COM1_Cpl_rcv ) != 0)
 || (memcmp( src0, dst0, sizeof(uint32_t)*ARR_SIZE  ) != 0) ) {
        result = 1;
	 } 
	 rumboot_printf("COMMPORT0 to COMMPORT1 direction checked\n"); 
	COMMP0_COMMP1_IRQ = 81;

	if( (comp_dma_irq_run( rumboot_virt_to_dma(src1), rumboot_virt_to_dma(dst1),COM1_BASE,COM0_BASE, &COM1_Cpl_tr,&COM0_Cpl_rcv ) != 0)
    || (memcmp( src1, dst1, sizeof(uint32_t)*ARR_SIZE  ) != 0)) {
        result = 1;
	 } 
 	rumboot_printf( "COM0_Cpl_tr= %d\n",COM0_Cpl_tr );
	rumboot_printf( "COM1_Cpl_rcv= %d\n",COM1_Cpl_rcv ); 
	rumboot_printf( "COM1_Cpl_tr= %d\n",COM1_Cpl_tr );
	rumboot_printf( "COM0_Cpl_rcv= %d\n",COM0_Cpl_rcv ); 
	COM0_Cpl_tr =0;
	COM0_Cpl_rcv=0;
	COM1_Cpl_tr =0;
	COM1_Cpl_rcv=0;
	
   deinit_irq(tbl);
  rumboot_printf("COMMPORT1 to COMMPORT0 direction checked\n"); 
    return result;
}

