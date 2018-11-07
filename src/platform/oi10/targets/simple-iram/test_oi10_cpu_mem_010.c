#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <platform/common_macros/common_macros.h>

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>

//                                        MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_CACHE_VALID_0           MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_VALID_1           MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define ddr_init_array  SRAM0_BASE;


#define CACHE_DATA_LINE_BY_TAG_INDEX(TAG,INDEX) \
    rumboot_printf("Address = %x\n", (0x00 | ((TAG)<<13) | ((INDEX)<<5) | ((0)<<2)) );\
    msync(); \
    /*Minimal caching size is 1 line = 32 bytes = 8 words = 4 doublewords*/ \
    iowrite64(0x00 | ((TAG)<<13) | ((INDEX)<<5) | ((0)<<2)), test_10_data_array64[0]; \
    iowrite64(0x00 | ((TAG)<<13) | ((INDEX)<<5) | ((2)<<2)), test_10_data_array64[1]; \
    iowrite64(0x00 | ((TAG)<<13) | ((INDEX)<<5) | ((4)<<2)), test_10_data_array64[2]; \
    iowrite64(0x00 | ((TAG)<<13) | ((INDEX)<<5) | ((6)<<2)), test_10_data_array64[3]; \
    msync(); \
    /*1st read initiates a caching*/ \
    TEST_ASSERT(ioread32( 0x00 | ((TAG)<<13) | ((INDEX)<<5) | ((0)<<2) ) == (uint32_t*) *test_10_data_array64, "Compare failed");\
    msync();

#define REVERSE8(X) \
    (((X & 0x01) << 7) | \
    ((X & 0x02) << 5) | \
    ((X & 0x04) << 3) | \
    ((X & 0x08) << 1) | \
    ((X & 0x10) >> 1) | \
    ((X & 0x20) >> 3) | \
    ((X & 0x40) >> 5) | \
    ((X & 0x80) >> 7) )

#define MOVE1TO6(X) (((X&(1<<1))<<5) |  ((X&0x7C)>>1) | (X & 1))
#define TAG_A_TO_INDEX(A)  (REVERSE8(MOVE1TO6(A) << 1))

//data array
uint64_t test_10_data_array64[4] =
{
        0xFFFFFFFFFFFFFFFF,
        0xFFFFFFFF00000000,
        0x00000000FFFFFFFF,
        0xF0F0F0F0F0F0F0F0
};

/*
 * erpn - 0 or 1 for em0
 * tag - 0..0x7FFFF
 * erpn_tag:
 * [28:19] = ERPN, (may be 0 or 1 only for EM0, see tlb entries)
 * [18:0] = TAG
 * [31:29] = 0
 */

void cache_data(uint32_t tag, uint32_t addr)
{
    uint32_t index = TAG_A_TO_INDEX(addr);
    rumboot_printf("index = %x\n", index);
    if(index & 1) //ODD
    {
        if(index >=128)
        {
            CACHE_DATA_LINE_BY_TAG_INDEX(tag,(index))
            CACHE_DATA_LINE_BY_TAG_INDEX(tag+1,(index))
        }
        else
        {
            CACHE_DATA_LINE_BY_TAG_INDEX(tag,(index+128))
            CACHE_DATA_LINE_BY_TAG_INDEX(tag+1,(index+128))
        }
    }
    else//EVEN
    {
        if(index >=128)
        {
            CACHE_DATA_LINE_BY_TAG_INDEX(tag,(index-128))
            CACHE_DATA_LINE_BY_TAG_INDEX(tag+1,(index-128))
        }
        else
        {
            CACHE_DATA_LINE_BY_TAG_INDEX(tag,(index))
            CACHE_DATA_LINE_BY_TAG_INDEX(tag+1,(index))
        }
    }
}

void check_dcu_tag()
{
     rumboot_printf("SRAM0\n");
     cache_data((0x3F), 0x00);
     cache_data((0x00), 0x04);
     cache_data((0x15), 0x08);
     cache_data((0x2A), 0x0B);

     rumboot_printf("SDRAM\n");
     cache_data((0x8FFF), 0x00);
     cache_data((0x8000), 0x04);
     cache_data((0x8AAA), 0x08);
     cache_data((0x8555), 0x0B);

     rumboot_printf("SSRAM\n");
     cache_data((0x100FF), 0x00);
     cache_data((0x10000), 0x04);
     cache_data((0x100AA), 0x08);
     cache_data((0x10055), 0x0B);

     rumboot_printf("PIPELINED\n");
     cache_data((0x140FF), 0x00);
     cache_data((0x14000), 0x04);
     cache_data((0x140AA), 0x08);
     cache_data((0x14055), 0x0B);

     rumboot_printf("SRAM1\n");
     cache_data((0x180FF), 0x00);
     cache_data((0x18000), 0x01);
     cache_data((0x180AA), 0x02);
     cache_data((0x18055), 0x03);
}

int main()
{
     emi_init(DCR_EM2_EMI_BASE);

     //cache on
     static const tlb_entry tlb_entry_cacheable_valid_0 = {TLB_ENTRY_CACHE_VALID_0};
     write_tlb_entries(&tlb_entry_cacheable_valid_0, 1);

     static const tlb_entry tlb_entry_cacheable_valid_1 = {TLB_ENTRY_CACHE_VALID_1};
     write_tlb_entries(&tlb_entry_cacheable_valid_1, 1);

     check_dcu_tag();

     return 0;
}

