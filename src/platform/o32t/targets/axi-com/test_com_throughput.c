#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/regs/regs_com.h>
#include <platform/devices.h>
#include <platform/devices/com_simple.h>


#include <arch/ppc/arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc/arch/ppc_476fp_mmu.h>

#ifndef ARR_SIZE
#define ARR_SIZE 1024
#endif

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
        (*src)[ i ] = i + ( (i) << 8 ) + ( (i) << 16 ) + ( (i) << 24 );
    msync();
}

// Crutch Until This Will Be Made For All Tests Accurately
static struct tlb_entry const em2_nospeculative_tlb_entries[] =
{
/*   MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
    {MMU_TLB_ENTRY(  0x020,  0xC0304,    0xC0304,    MMU_TLBE_DSIZ_16KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_UND,   0b1 )}
};

int comp_dma_run_thru(uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1,uint32_t count_num) {
	uint32_t result;
	uint32_t start;
	uint32_t end;
	uint32_t delta;
	uint32_t v;
	
	result =comp_dma_run_tr(src_addr,base0,0x3,count_num );
	result =comp_dma_run_rcv(dst_addr,base1,0x3,count_num );
	
	iowrite32(COM_CONTROL_EN,base0+ CSR_tr);
	
	start = rumboot_platform_get_uptime();
	rumboot_printf( "start=%d\n",start);

	
	iowrite32(COM_CONTROL_EN,base1	+ CSR_rcv);
	
	rumboot_printf( "check_run\n" );
	
	if (com_simple_wait_complete(base0, CSR_tr)) {
        rumboot_printf( "COM0_Cpl  timeout_1\n" );
        return 1;
    }
	
	end = rumboot_platform_get_uptime();
	delta = end -start;
    v = (ARR_SIZE *4)/delta;
	rumboot_printf( "end=%d\n",end);	
	rumboot_printf( "v=%d\n",v);
//rumboot_printf( "delta=%d\n",delta );

	
	if (com_simple_wait_complete(base1, CSR_rcv )) {
        rumboot_printf( "COM1_Cpl  timeout_1\n" );
        return 1;
    }
return result;
   }   




int main()
{
	uint32_t result=0; 
	uint32_t * src0;
	uint32_t * dst0;
	uint32_t * src1;
	uint32_t * dst1;
  rumboot_printf("Start COMMPORT\n");
  
  write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));
    prepare_arrays( &src0, &dst0 );	
    prepare_arrays( &src1, &dst1 );
	rumboot_printf( "ar_size=%d\n",ARR_SIZE);
//uint32_t start = rumboot_platform_get_uptime();
//rumboot_printf( "start=%d\n",start);
  
    if( (comp_dma_run_thru(rumboot_virt_to_dma(src0), rumboot_virt_to_dma(dst0),COM0_BASE,COM1_BASE,(ARR_SIZE	>>	1)) != 0)
    ) {
        result = 1;
	 }
	 
//uint32_t end = rumboot_platform_get_uptime();
//uint32_t delta = end -start;
	
//rumboot_printf( "delta=%d\n",delta );
//	rumboot_printf( "end=%d\n",end);

 	 rumboot_printf("COMMPORT0 to COMMPORT1 direction checked\n"); 
	if( (comp_dma_run_thru(rumboot_virt_to_dma(src1), rumboot_virt_to_dma(dst1),COM1_BASE,COM0_BASE,(ARR_SIZE	>>	1)) != 0)
    ) {
       return 1;
	 }
 rumboot_printf("COMMPORT1 to COMMPORT0 direction checked\n"); 	 
  return result;
}

