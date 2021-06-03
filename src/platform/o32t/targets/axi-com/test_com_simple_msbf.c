#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <regs/regs_com.h>
#include <platform/devices.h>
#include <devices/com_simple.h>

#ifdef __PPC__
#include <arch/ppc/arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc/arch/ppc_476fp_mmu.h>
#endif

#define ARR_SIZE 4 //2//128

#ifndef COM_SRC_HEAP
#define COM_SRC_HEAP "IM1"
#endif
#ifndef COM_DST_HEAP
#define COM_DST_HEAP "IM1"
#endif

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
	uint32_t result =0;
	uint32_t * src0;
	uint32_t * dst0;
	uint32_t * src1;
	uint32_t * dst1;
  rumboot_printf("Start COMMPORT\n");
 #ifdef __PPC__ 
  write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));
#endif
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
	//initial data is reversed by opposite endian
	iowrite32( 0x20, COM0_BASE + PHYConfig ); //set big_little endian bit
   if( (comp_dma_run( dsrc0, ddst0,COM0_BASE,COM1_BASE,(ARR_SIZE>>	1)) != 0)
		||	memcmp( src0, dst0, sizeof(uint32_t)*ARR_SIZE  ) == 0) {
	 
	// rumboot_printf( "src0= %d\n",src0 );
	// rumboot_printf( "dst0= %d\n",dst0 );
        result = 1;
	 }

	  //destination  data is reversed by opposite endian as initial data
	iowrite32( 0x20, COM1_BASE + PHYConfig );
	if( (comp_dma_run( dsrc0, ddst0,COM0_BASE,COM1_BASE,(ARR_SIZE>>	1)) != 0)
		||	memcmp( src0, dst0, sizeof(uint32_t)*ARR_SIZE  ) != 0) {
	 
	// rumboot_printf( "src0= %d\n",src0 );
	// rumboot_printf( "dst0= %d\n",dst0 );
        result = 1;
	 } 
 	 rumboot_printf("COMMPORT0 to COMMPORT1 direction checked\n");	
	 iowrite32( 0x0, COM0_BASE + PHYConfig );
	 
	 //initial data for COMMPORT1 are reversed	 

	if( (comp_dma_run( dsrc1,ddst1,COM1_BASE,COM0_BASE,(ARR_SIZE>>	1)) != 0)
	 ||	(memcmp( src1, dst1, sizeof(uint32_t)*ARR_SIZE  ) == 0) 
	){
        return 1;
	 }
	iowrite32( 0x20, COM0_BASE + PHYConfig );
	if( (comp_dma_run(dsrc1,ddst1,COM1_BASE,COM0_BASE,(ARR_SIZE>>	1)) != 0)
	 ||	(memcmp( src1, dst1, sizeof(uint32_t)*ARR_SIZE  ) != 0) 
	){
        return 1;
	 }
 
 
   rumboot_printf("COMMPORT1 to COMMPORT0 direction checked\n"); 
  return result;
}

