/*
 * test)oi10_em2_104.c
 *
 *  Created on: Aug 1, 2018
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/regpoker.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/mem_window.h>
#include <devices/ugly/emi.h>
#include <platform/devices/nor_1636RR4.h>

#define TLB_ENTRY_CACHE_WT   MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_WB   MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

void init_nor ()
{
    uint32_t data_arr32[] =
    {
        [0] = 0x10101010,
        [1] = 0x01234567,
        [2] = 0x89ABCDEF,
        [3] = 0x00000000,
        [4] = 0xFFFFFFFF,
        [5] = 0x55555555,
        [6] = 0xCCCCCCCC,
        [7] = 0x77777777
    };

    for (int i = 0; i < 8; i++)
        nor_write32 (data_arr32 [i], NOR_BASE + 4*i);
}
void check_nor ()
{
    struct regpoker_checker check_array8[] = {
          { "Data0_8",  REGPOKER_READ8, 0x0, 0x10, 0xff },
          { "Data1_8",  REGPOKER_READ8, 0x1, 0x10, 0xff },
          { "Data2_8",  REGPOKER_READ8, 0x2, 0x10, 0xff },
          { "Data3_8",  REGPOKER_READ8, 0x3, 0x10, 0xff },
          { "Data4_8",  REGPOKER_READ8, 0x4, 0x01, 0xff },
          { "Data5_8",  REGPOKER_READ8, 0x5, 0x23, 0xff },
          { "Data6_8",  REGPOKER_READ8, 0x6, 0x45, 0xff },
          { "Data7_8",  REGPOKER_READ8, 0x7, 0x67, 0xff },
          { "Data8_8",  REGPOKER_READ8, 0x8, 0x89, 0xff },
          { "Data9_8",  REGPOKER_READ8, 0x9, 0xAB, 0xff },
          { "Data10_8",  REGPOKER_READ8, 0xA, 0xCD, 0xff },
          { "Data11_8",  REGPOKER_READ8, 0xB, 0xEF, 0xff },
          { "Data12_8",  REGPOKER_READ8, 0xC, 0x00, 0xff },
          { "Data13_8",  REGPOKER_READ8, 0xD, 0x00, 0xff },
          { "Data14_8",  REGPOKER_READ8, 0xE, 0x00, 0xff },
          { "Data15_8",  REGPOKER_READ8, 0xF, 0x00, 0xff },
          { "Data16_8",  REGPOKER_READ8, 0x10, 0xff, 0xff },
          { "Data17_8",  REGPOKER_READ8, 0x11, 0xff, 0xff },
          { "Data18_8",  REGPOKER_READ8, 0x12, 0xff, 0xff },
          { "Data19_8",  REGPOKER_READ8, 0x13, 0xff, 0xff },
          { "Data20_8",  REGPOKER_READ8, 0x14, 0x55, 0xff },
          { "Data21_8",  REGPOKER_READ8, 0x15, 0x55, 0xff },
          { "Data22_8",  REGPOKER_READ8, 0x16, 0x55, 0xff },
          { "Data23_8",  REGPOKER_READ8, 0x17, 0x55, 0xff },
          { "Data24_8",  REGPOKER_READ8, 0x18, 0xCC, 0xff },
          { "Data25_8",  REGPOKER_READ8, 0x19, 0xCC, 0xff },
          { "Data26_8",  REGPOKER_READ8, 0x1A, 0xCC, 0xff },
          { "Data27_8",  REGPOKER_READ8, 0x1B, 0xCC, 0xff },
          { "Data28_8",  REGPOKER_READ8, 0x1C, 0x77, 0xff },
          { "Data29_8",  REGPOKER_READ8, 0x1D, 0x77, 0xff },
          { "Data30_8",  REGPOKER_READ8, 0x1E, 0x77, 0xff },
          { "Data31_8",  REGPOKER_READ8, 0x1F, 0x77, 0xff },
          { /* Sentinel */ }
        };

    struct regpoker_checker check_array16[] = {
          { "Data0_16",  REGPOKER_READ16, 0x0, 0x1010, 0xffff },
          { "Data1_16",  REGPOKER_READ16, 0x2, 0x1010, 0xffff },
          { "Data2_16",  REGPOKER_READ16, 0x4, 0x0123, 0xffff },
          { "Data3_16",  REGPOKER_READ16, 0x6, 0x4567, 0xffff },
          { "Data4_16",  REGPOKER_READ16, 0x8, 0x89AB, 0xffff },
          { "Data5_16",  REGPOKER_READ16, 0xA, 0xCDEF, 0xffff },
          { "Data6_16",  REGPOKER_READ16, 0xC, 0x0000, 0xffff },
          { "Data7_16",  REGPOKER_READ16, 0xE, 0x0000, 0xffff },
          { "Data8_16",  REGPOKER_READ16, 0x10, 0xffff, 0xffff },
          { "Data9_16",  REGPOKER_READ16, 0x12, 0xffff, 0xffff },
          { "Data10_16",  REGPOKER_READ16, 0x14, 0x5555, 0xffff },
          { "Data11_16",  REGPOKER_READ16, 0x16, 0x5555, 0xffff },
          { "Data12_16",  REGPOKER_READ16, 0x18, 0xCCCC, 0xffff },
          { "Data13_16",  REGPOKER_READ16, 0x1A, 0xCCCC, 0xffff },
          { "Data14_16",  REGPOKER_READ16, 0x1C, 0x7777, 0xffff },
          { "Data15_16",  REGPOKER_READ16, 0x1E, 0x7777, 0xffff },
          { /* Sentinel */ }
        };

    struct regpoker_checker check_array32[] = {
          { "Data0_32",  REGPOKER_READ32, 0x0, 0x10101010, 0xffffffff },
          { "Data1_32",  REGPOKER_READ32, 0x4, 0x01234567, 0xffffffff },
          { "Data2_32",  REGPOKER_READ32, 0x8, 0x89ABCDEF, 0xffffffff },
          { "Data3_32",  REGPOKER_READ32, 0xC, 0x00000000, 0xffffffff },
          { "Data4_32",  REGPOKER_READ32, 0x10, 0xffffffff, 0xffffffff },
          { "Data5_32",  REGPOKER_READ32, 0x14, 0x55555555, 0xffffffff },
          { "Data6_32",  REGPOKER_READ32, 0x18, 0xCCCCCCCC, 0xffffffff },
          { "Data7_32",  REGPOKER_READ32, 0x1C, 0x77777777, 0xffffffff },

          { /* Sentinel */ }
        };

    struct regpoker_checker check_array64[] = {
          { "Data0_64",  REGPOKER_READ64, 0x0, 0x1010101001234567, 0xffffffffffffffff },
          { "Data1_64",  REGPOKER_READ64, 0x8, 0x89ABCDEF00000000, 0xffffffffffffffff },
          { "Data2_64",  REGPOKER_READ64, 0x10, 0xffffffff55555555, 0xffffffffffffffff },
          { "Data3_64",  REGPOKER_READ64, 0x18, 0xCCCCCCCC77777777, 0xffffffffffffffff },
          { /* Sentinel */ }
        };

    TEST_ASSERT (rumboot_regpoker_check_array(check_array8, NOR_BASE) == 0, "ERROR: read8 from NOR is fail" );
    dci(2);
    TEST_ASSERT (rumboot_regpoker_check_array(check_array16, NOR_BASE) == 0, "ERROR: read16 from NOR is fail" );
    dci(2);
    TEST_ASSERT (rumboot_regpoker_check_array(check_array32, NOR_BASE) == 0, "ERROR: read32 from NOR is fail" );
    dci(2);
    TEST_ASSERT (rumboot_regpoker_check_array(check_array64, NOR_BASE) == 0, "ERROR: read64 from NOR is fail" );
    dci(2);
}

int main ()
{
    emi_init (DCR_EM2_EMI_BASE);
    init_nor ();

    rumboot_printf ("CACHE OFF\n");
    check_nor ();

    rumboot_printf ("CACHE WT\n");
    static const tlb_entry sram0_tlb_entry_wt = {TLB_ENTRY_CACHE_WT};
    write_tlb_entries(&sram0_tlb_entry_wt,1);
    check_nor ();

    rumboot_printf ("CACHE WB\n");
    static const tlb_entry sram0_tlb_entry_wb = {TLB_ENTRY_CACHE_WB};
    write_tlb_entries(&sram0_tlb_entry_wb,1);
    check_nor ();

    rumboot_printf ("TEST_OK\n");
    return 0;
}
