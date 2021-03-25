
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


#define ARR_SIZE  2//128

#ifndef COM_SRC_HEAP
#define COM_SRC_HEAP "IM1"
#endif
#ifndef COM_DST_HEAP
#define COM_DST_HEAP "IM1"
#endif

void prepare_arrays( uint32_t ** src, uint32_t ** dst ) {
    *src = rumboot_malloc_from_named_heap_aligned( COM_SRC_HEAP, sizeof(uint32_t)*ARR_SIZE, sizeof(uint64_t) );
    *dst = rumboot_malloc_from_named_heap_aligned( COM_DST_HEAP, sizeof(uint32_t)*ARR_SIZE,sizeof(uint64_t) );
	
    for( uint32_t i = 0; i < ARR_SIZE; i++ )
        (*src)[ i ] = i + ( (i+1) << 8 ) + ( (i+2) << 16 ) + ( (i+3) << 24 );

//    msync();
}
void prepare_arrays_light( uint32_t ** src, uint32_t ** dst ) {
    *src = rumboot_malloc_from_named_heap_aligned( COM_SRC_HEAP, sizeof(uint32_t)*ARR_SIZE, sizeof(uint64_t) );
    *dst = rumboot_malloc_from_named_heap_aligned( COM_DST_HEAP, sizeof(uint32_t)*ARR_SIZE,sizeof(uint64_t) );
	
    for( uint32_t i = 0; i < ARR_SIZE; i++ )
        (*src)[ i ] = i << 1;
	
}
 uint32_t COM0_Cpl_tr;
 uint32_t COM0_Cpl_rcv;
 uint32_t COM1_Cpl_tr;
 uint32_t COM1_Cpl_rcv;
 uint32_t COMMP0_COMMP1_IRQ = CP0_TRM_INT;


static void handler1() {


	   rumboot_printf( "COMMP0_COMMP1_IRQ= %d\n",COMMP0_COMMP1_IRQ) ;	
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
		clear_com_status(COM1_BASE,0);
		rumboot_printf( "COM1_Cpl_rcv= %d\n",COM1_Cpl_rcv );
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
static struct tlb_entry const em2_nospeculative_tlb_entries[] =
{
/*   MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
   {MMU_TLB_ENTRY(  0x020,  0xC0304,    0xC0304,    MMU_TLBE_DSIZ_16KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_UND,   0b1 )}
};

int main()
{
  int result = 0;
  uint32_t * src0 ;
  uint32_t * dst0;
  uint32_t * src1;
  uint32_t * dst1;
  rumboot_printf("Start COMMPORT\n");
  
  write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));
	prepare_arrays( &src0, &dst0 );
    prepare_arrays_light( &src1, &dst1 );
	
	
	COMMP0_COMMP1_IRQ = CP0_TRM_INT;
  struct rumboot_irq_entry * tbl = init_irq(); 

uint32_t	dst_0= 0x80003da0;
uint32_t	dst_1= 0x80003da8;	

if( (comp_dma_irq_run(rumboot_virt_to_dma(src0), (dst_0),COM0_BASE,COM1_BASE,&COM0_Cpl_tr,&COM1_Cpl_rcv,(ARR_SIZE>>1)) != 0)
 )
 {
        result = 1;  //check from BRESP[1]=1  ES interrupt COMMMPORT 1
	 } 
	 rumboot_printf("COMMPORT0 to COMMPORT1 direction checked for BRESP COMMPORT1\n"); 
	COMMP0_COMMP1_IRQ = CP1_TRM_INT;
	if( (comp_dma_irq_run(rumboot_virt_to_dma(src1),(dst_1),COM1_BASE,COM0_BASE, &COM1_Cpl_tr,&COM0_Cpl_rcv,(ARR_SIZE>>1)) != 0)
	) {
       result = 1;  //check from BRESP[1]=1  ES interrupt COMMMPORT 0
	 }
rumboot_printf("COMMPORT1 to COMMPORT0 direction checked for BRESP COMMPORT0\n"); 	 
COMMP0_COMMP1_IRQ = CP0_TRM_INT; 
if( (comp_dma_irq_run((dst_0), rumboot_virt_to_dma(dst0),COM0_BASE,COM1_BASE,&COM0_Cpl_tr,&COM1_Cpl_rcv,(ARR_SIZE>>1)) != 0)
 )
 {
        result = 1; //check from RRESP[1]=1   ES interrupt COMMMPORT 0 
	 } 
	 rumboot_printf("COMMPORT0 to COMMPORT1 direction checked RRESP COMMPORT0\n"); 
	COMMP0_COMMP1_IRQ = CP1_TRM_INT;
	if( (comp_dma_irq_run( (dst_1),rumboot_virt_to_dma(dst1),COM1_BASE,COM0_BASE, &COM1_Cpl_tr,&COM0_Cpl_rcv,(ARR_SIZE>>1)) != 0)
	) {
       result = 1; //check from RRESP[1]=1  ES interrupt COMMMPORT 1 
	 }
	
	deinit_irq(tbl);
	rumboot_printf("COMMPORT1 to COMMPORT0 direction checked RRESP COMMPORT1\n"); 
    return result;
}