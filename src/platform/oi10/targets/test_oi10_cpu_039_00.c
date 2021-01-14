/*
 * test_oi10_cpu_039_00.c
 *
 *  Created on: Dec 21, 2018
 *      Author: m.smolina
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/platform.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>
#include <arch/ppc_476fp_mmu.h>
#include <platform/devices/emi.h>

typedef uint8_t func();

                        //MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,             IL1I,            IL1D,           W,         I,    M,    G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,           V   )
//#define TLB_ENTRY_WT    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_WT    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )


int main(void)
{
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Init mem... ");
    memset((void*)(SRAM0_BASE), 0x00, 0x28000);
    rumboot_printf("done\n");

    rumboot_printf("Load bin from 0x%x\n", M_BASE);
    rumboot_platform_request_file("MBIN", M_BASE);

    rumboot_printf("Init tlb (set WT)\n");
    static const tlb_entry sram0_tlb_entry_wt = {TLB_ENTRY_WT};
    write_tlb_entries(&sram0_tlb_entry_wt,1);

    rumboot_printf("Starting test code from another memory\n");
    func* f = (func*)(M_BASE);
    uint8_t result = f();

    if (result == 0)
      rumboot_printf("TEST_OK\n");
    else
      rumboot_printf("TEST_ERROR\n");

    return result;
}
