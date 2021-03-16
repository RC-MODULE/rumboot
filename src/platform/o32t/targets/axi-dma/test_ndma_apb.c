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


ndma_cfg_t cfg_apb = {
  .RD_Address      =(uint32_t)0x12345678,
  .RD_Bias         =0xABCDEF90,
  .RD_RowCounter   =0x5555,
  .RD_AddressMode  =ONE_DIMENSION,
  .WR_Address      =(uint32_t)0x87654321,
  .WR_Bias         =0x12121212,
  .WR_RowCounter   =0xFFFF,
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
  
  
  rumboot_printf("Hello NDMA, test apb\n");
  
  write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));
 
  rumboot_printf("NDMA apb check\n");
  ndma_apb_check((uintptr_t)RCM_NDMA_BASE,&cfg_apb);
  rumboot_printf("NDMA running one check\n");
  ndma_apb_running_one((uintptr_t)RCM_NDMA_BASE);
   
  return 0;
}

