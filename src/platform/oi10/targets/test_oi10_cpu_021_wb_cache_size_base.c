#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/platform.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/devices/emi.h>
#include <rumboot/memfill.h>

                                    //MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,  IL1D,   W,     I,     M,     G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,               V   )
#define TLB_ENTRY_CACHE_WB_VALID      MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,   0b0,    0b0,   0b0,   0b1,   0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,  0b1 )
#define M_BASE SRAM0_BASE

typedef bool func();

int main(void)
{
    rumboot_printf ("EMI init\n");
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_memfill8_modelling( (void*)M_BASE, 0x10000, 0x00, 0x00 ); //anti X fix

    rumboot_printf ("Load cache test function in 0x%x\n", M_BASE);
    rumboot_platform_request_file("MBIN", M_BASE); //M_BASE defined in cmake

    rumboot_printf ("Set tlb (WB)\n");
    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_WB_VALID};
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid, 1);

    rumboot_printf("Call cache test function\n");
    func* test_func = (func*)(M_BASE);
    if ( !test_func() )
    {
        rumboot_printf ("TEST ERROR\n");
        return 1;
    }

    rumboot_printf ("TEST OK\n");
    return 0;
}
