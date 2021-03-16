#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>

#include <platform/devices/ndma_simple.h>


#include <arch/ppc/arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc/arch/ppc_476fp_mmu.h>


ndma_cfg_t cfg = {
  .RD_Address      =(uint32_t)0x0,
  .RD_Bias         =0x0,
  .RD_RowCounter   =0x0,
  .RD_AddressMode  =ONE_DIMENSION,
  .WR_Address      =(uint32_t)0x0,
  .WR_Bias         =0x0,
  .WR_RowCounter   =0x0,
  .WR_AddressMode  =ONE_DIMENSION,
  .MainCounter     =0x0,
  .InterruptMask   =NDMA_CONTROL_EN|NDMA_CONTROL_CPL
};


#define ARR_SIZE 128

// Crutch Until This Will Be Made For All Tests Accurately
static struct tlb_entry const em2_nospeculative_tlb_entries[] =
{
/*   MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
    {MMU_TLB_ENTRY(  0x020,  0xC0304,    0xC0304,    MMU_TLBE_DSIZ_16KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_UND,   0b1 )}
};

int main()
{
  uint32_t * src;
  uint32_t * dst;
  uint32_t seed=0;
  uint32_t diff=0x00010001u;
  uint32_t patt=0x55555555u;
  uint32_t acc;
  
  
  
  rumboot_printf("Hello NDMA\n");
  
  write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));
  
  src = (uint32_t*)rumboot_malloc_from_named_heap_aligned("IM1", ARR_SIZE * sizeof(uint32_t), sizeof(uint64_t));
  dst = (uint32_t*)rumboot_malloc_from_named_heap_aligned("IM1", ARR_SIZE * sizeof(uint32_t), sizeof(uint64_t));
  
  acc = seed;
  for(int i=0;i<ARR_SIZE;i++)
  {
    src[i] = acc;
    dst[i] = patt;
    acc = acc + diff;
  }
  
  cfg.RD_Address = rumboot_virt_to_dma( src );
  cfg.WR_Address = rumboot_virt_to_dma( dst );
  cfg.MainCounter = ARR_SIZE >> 1; //Size In 64-bit Words
  
  rumboot_printf("Run NDMA from %x to %x, size %d\n",cfg.RD_Address,cfg.WR_Address,cfg.MainCounter);
  
  ndma_simple_memcpy((uintptr_t)RCM_NDMA_BASE,&cfg);
  
  rumboot_printf("Checking\n");
  acc = seed;
  for(int i=0;i<ARR_SIZE;i++)
  {
    if(dst[i]!=acc)
    {
      rumboot_printf("ERROR: i=%d, value=%x, acc=%x\n",i,dst[i],acc);
      return 1;
    }
    
    acc = acc + diff;
  }
  
  return 0;
}

