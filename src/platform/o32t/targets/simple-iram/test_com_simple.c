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

/*
com_cfg_t cfg = {
  .RD_MainCounter  =0x0,
  .RD_Address      =(uint32_t)0x0,
  .RD_Bias         =0x0,
  .RD_RowCounter   =0x0,
  .RD_AddressMode  =ONE_DIMENSION,
  .RD_InterruptMask=COM_RD_CONTROL_EN|COM_RD_CONTROL_CPL,
  .RD_IH_EN_tr         =0x0,
  .WR_MainCounter     =0x0,
  .WR_Address      =(uint32_t)0x0,
  .WR_Bias         =0x0,
  .WR_RowCounter   =0x0,
  .WR_AddressMode  =ONE_DIMENSION,
  .WR_InterruptMask   =COM_WR_CONTROL_EN|COM_WR_CONTROL_CPL,
  .WR_PHYConfig         =0x0,
  .WR_Err1CNT_rcv         =0x0,
  .WR_Err2CNT_rcv         =0x0 
};
*/
#define ARR_SIZE 128

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
        (*src)[ i ] = i + ( (i+1) << 8 ) + ( (i+1) << 16 ) + ( (i+1) << 24 );
    msync();
}

// Crutch Until This Will Be Made For All Tests Accurately
static struct tlb_entry const em2_nospeculative_tlb_entries[] =
{
/*   MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
    {MMU_TLB_ENTRY(  0x020,  0xC0304,    0xC0304,    MMU_TLBE_DSIZ_16KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_UND,   0b1 )}
};

int main()
{
	uint32_t result; 
	uint32_t * src0;
	uint32_t * dst0;
	uint32_t * src1;
	uint32_t * dst1;
  rumboot_printf("Start COMMPORT\n");
  
  write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));
    prepare_arrays( &src0, &dst0 );
    prepare_arrays( &src1, &dst1 );
    if( (comp_dma_run( rumboot_virt_to_dma(src0), rumboot_virt_to_dma(dst0),COM0_BASE,COM1_BASE ) != 0)
    || (memcmp( src0, dst0, sizeof(uint32_t)*ARR_SIZE  ) != 0) ) {
        result = 1;
	 }  
 	 rumboot_printf("COMMPORT0 to COMMPORT1 direction checked\n"); 
	if( (comp_dma_run( rumboot_virt_to_dma(src1), rumboot_virt_to_dma(dst1),COM1_BASE,COM0_BASE ) != 0)
     || (memcmp( src1, dst1, sizeof(uint32_t)*ARR_SIZE  ) != 0) ) {
        return 1;
	 }
   rumboot_printf("COMMPORT1 to COMMPORT0 direction checked\n"); 	 
  return 0;
}

