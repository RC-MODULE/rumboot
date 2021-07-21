#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>

#include <rumboot/irq.h>
#include <platform/interrupts.h>

#include <rumboot/io.h>
#include <platform/devices.h>
#include <regs/regs_com.h>
#include <devices/com_simple.h>

#ifdef __PPC__
#include <arch/ppc/arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc/arch/ppc_476fp_mmu.h>
#endif

#ifndef ARR_SIZE
#define ARR_SIZE 128
#endif

#ifndef COM_SRC_HEAP
#define COM_SRC_HEAP "IM1"
#endif
#ifndef COM_DST_HEAP
#define COM_DST_HEAP "IM1"
#endif

static volatile uint32_t COM0_Cpl_tr;
static volatile uint32_t COM0_Cpl_rcv;
static volatile uint32_t COM1_Cpl_tr;
static volatile uint32_t COM1_Cpl_rcv;
static volatile uint32_t COMMP0_COMMP1_IRQ = CP0_TRM_INT;


static void handler1() {


	   rumboot_printf( "COMMP0_COMMP1_IRQ= %d\n",COMMP0_COMMP1_IRQ) ;	
	   rumboot_printf( "com_status= %d\n",com_status(COM0_BASE, 1) );	  
	if (COMMP0_COMMP1_IRQ == CP0_TRM_INT){
		if (com_status(COM0_BASE, 1)== 1) {
		COM0_Cpl_tr = 1;
		clear_com_status(COM0_BASE,1);
		rumboot_printf( "COM0_Cpl_tr= %d\n",COM0_Cpl_tr );
		COMMP0_COMMP1_IRQ = CP1_RCV_INT;
		COM0_Cpl_tr = 0;
		}
	  }
	if (COMMP0_COMMP1_IRQ == CP1_RCV_INT) {
		if (com_status(COM1_BASE, 0)== 1) {
		COM1_Cpl_rcv = 1;	  
		rumboot_printf( "COM1_Cpl_rcv= %d\n",COM1_Cpl_rcv );
		clear_com_status(COM1_BASE,0);
		COMMP0_COMMP1_IRQ = CP1_TRM_INT;
		COM1_Cpl_rcv = 0;
		}
	}
	    
	if (COMMP0_COMMP1_IRQ == CP1_TRM_INT) {
		if (com_status(COM1_BASE, 1)== 1) {
		COM1_Cpl_tr = 1;
		clear_com_status(COM1_BASE,1); 
		rumboot_printf( "COM1_Cpl_tr= %d\n",COM1_Cpl_tr );		   
		COMMP0_COMMP1_IRQ = CP0_RCV_INT;
		COM1_Cpl_tr = 0;
		}	
	}
  
	if (COMMP0_COMMP1_IRQ == CP0_RCV_INT){ 
		if (com_status(COM0_BASE, 0)== 1) {	 	
        COM0_Cpl_rcv = 1;
		clear_com_status(COM0_BASE,0); 	  
		rumboot_printf( "COM0_Cpl_rcv= %d\n",COM0_Cpl_rcv );
		COMMP0_COMMP1_IRQ = 0;
		COM0_Cpl_rcv = 0;
		}
	}
	

}

static struct rumboot_irq_entry * init_irq() {
    rumboot_irq_cli();
    struct rumboot_irq_entry * const tbl = rumboot_irq_create( NULL );
	COMMP0_COMMP1_IRQ = CP0_TRM_INT;
    rumboot_irq_set_handler( tbl, COMMP0_COMMP1_IRQ, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	COMMP0_COMMP1_IRQ = CP1_RCV_INT;
    rumboot_irq_set_handler( tbl, COMMP0_COMMP1_IRQ, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	COMMP0_COMMP1_IRQ = CP1_TRM_INT;
    rumboot_irq_set_handler( tbl,COMMP0_COMMP1_IRQ, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, NULL );
	COMMP0_COMMP1_IRQ = CP0_TRM_INT;
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
#ifdef __PPC__
static struct tlb_entry const em2_nospeculative_tlb_entries[] =
{
/*   MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
    {MMU_TLB_ENTRY(  0x020,  0xC0304,    0xC0304,    MMU_TLBE_DSIZ_16KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_UND,   0b1 )}
};
#endif

int main()
{
  int result = 0;
  uint32_t * src0;
  uint32_t * dst0;
  uint32_t * src1;
  uint32_t * dst1;
  
  rumboot_printf("Start COMMPORT\n");
#ifdef __PPC__  
  write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));
#endif

	COMMP0_COMMP1_IRQ = CP0_TRM_INT; /*CP0_TRM_INT */
	struct rumboot_irq_entry * tbl = init_irq(); 
  
	src0 = (uint32_t*)rumboot_malloc_from_named_heap(COM_SRC_HEAP, sizeof(uint32_t)*ARR_SIZE) ;
	dst0 = (uint32_t*)rumboot_malloc_from_named_heap(COM_DST_HEAP, sizeof(uint32_t)*ARR_SIZE );

	src1 = (uint32_t*)rumboot_malloc_from_named_heap(COM_SRC_HEAP, sizeof(uint32_t)*ARR_SIZE) ;
	dst1 = (uint32_t*)rumboot_malloc_from_named_heap(COM_DST_HEAP, sizeof(uint32_t)*ARR_SIZE );
 	
	for( uint32_t i = 0; i < ARR_SIZE; i++ )
	{	src0[ i ] = i + ( (i+1) << 8 ) + ( (i+2) << 16 ) + ( (i+3) << 24 );	
		src1[ i ] = i + ( (i+1) << 8 ) + ( (i+2) << 16 ) + ( (i+3) << 24 );
	}
	uintptr_t dsrc0 = rumboot_virt_to_dma(src0);
	uintptr_t ddst0 = rumboot_virt_to_dma(dst0);
	uintptr_t dsrc1 = rumboot_virt_to_dma(src1);
	uintptr_t ddst1 = rumboot_virt_to_dma(dst1);

   if( (comp_dma_irq_run( /*rumboot_virt_to_dma(src0)*/dsrc0, /*rumboot_virt_to_dma(dst0)*/ddst0,COM0_BASE,COM1_BASE,&COM0_Cpl_tr,&COM1_Cpl_rcv,(ARR_SIZE>>1)) != 0)
	||	memcmp( src0, dst0, sizeof(uint32_t)*ARR_SIZE  ) != 0){
	  return 1;
	 } 
	 	
	if( (comp_dma_irq_run( /*rumboot_virt_to_dma(src1))*/dsrc1, /*rumboot_virt_to_dma(dst1)*/ddst1,COM1_BASE,COM0_BASE,&COM1_Cpl_tr,&COM0_Cpl_rcv,(ARR_SIZE>>1)) != 0)
	||	memcmp( src1, dst1, sizeof(uint32_t)*ARR_SIZE  ) != 0){
        return 1;
	}
	rumboot_printf("sizeof=%x\n", sizeof(uint32_t)*ARR_SIZE);
	rumboot_printf("ARR_SIZE=%x\n", ARR_SIZE);
 	rumboot_printf( "COM0_Cpl_tr= %d\n",COM0_Cpl_tr );
	rumboot_printf( "COM1_Cpl_rcv= %d\n",COM1_Cpl_rcv ); 
	rumboot_printf( "COM1_Cpl_tr= %d\n",COM1_Cpl_tr );
	rumboot_printf( "COM0_Cpl_rcv= %d\n",COM0_Cpl_rcv ); 

	deinit_irq(tbl);
	
    return result;
}
