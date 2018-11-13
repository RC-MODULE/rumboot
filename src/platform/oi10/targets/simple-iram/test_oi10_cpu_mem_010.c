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


//tag array
uint32_t test_tag_array[] =
{
    //sram
    0x00,
    0x04,
    0x08,
    0x10,
    0x20,
    //sdram
    0x10000,
    0x10040,
    0x10080,
    0x10100,
    0x10200,
    0x10400,
    0x10800,
    0x10FFC,
    //ssram
    0x10000,
    //pipe
    0x28000,
    //sram1
    0x30000,
};
uint32_t LEN_TAG_ARR = sizeof(test_tag_array) / sizeof (uint32_t);

//data array
uint64_t test_data_array[] =
{
    0x1234ABCDFFFFFFFF,
    0x1111111122222222,
    0x3333333344444444,
    0x5555555566666666
};

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

/*
 * erpn - 0 or 1 for em0
 * tag - 0..0x7FFFF
 * erpn_tag:
 * [28:19] = ERPN, (may be 0 or 1 only for EM0, see tlb entries)
 * [18:0] = TAG
 * [31:29] = 0
 */
uint32_t calc_addr (uint32_t tag, uint32_t index)
{
    return (0x00 | ((tag)<<13) | ((index)<<5) | (0<<2));
}

void CACHE_DATA_LINE_BY_TAG_INDEX (uint32_t TAG, uint32_t INDEX)
{
    uint32_t addr = calc_addr(TAG, INDEX);
    rumboot_printf("Address = 0x%x (tag = 0x%x)\n", addr, TAG );
    msync();
    /*Minimal caching size is 1 line = 32 bytes = 8 words = 4 doublewords*/
    for (int i = 0; i < 4; i++)
        iowrite64(test_data_array[i], addr + i*8 );
    msync();
    /*1st read initiates a caching*/
    TEST_ASSERT(ioread32(addr) == ioread32((uint32_t)test_data_array), "Compare failed");


    rumboot_printf("dcread...\n");
    msync();
    uint32_t dcread_data = dcread (addr & 0x7FFC);
    isync();
    TEST_ASSERT(dcread_data == ioread32((uint32_t)test_data_array), "Compare failed");
    uint32_t reg = spr_read(SPR_DCDBTRH);
    rumboot_printf("SPR_DCDBTRH = %x, TAG = %x\n\n",reg,  reg >> 13);
    TEST_ASSERT((reg >> 13) == TAG, "TEST_ERROR: wrong tag in SPR_DCDBTRH");
}

void cache_data(uint32_t tag, uint32_t addr)
{
    uint32_t index = TAG_A_TO_INDEX(addr);
    if((index & 1) && (index < 128)) index+=128;
    if( (!(index & 1)) && (index >=128) ) index -= 128;

    CACHE_DATA_LINE_BY_TAG_INDEX(tag,(index));
}

void check_dcu_tag()
{
    rumboot_printf("\nCheck_dcu_tag\n");
    uint32_t i = 0;
    for (i = 0; i < LEN_TAG_ARR; i++)
    {
        cache_data(test_tag_array[i], i);
    }
}

int main()
{
     rumboot_printf("Start test\n");
     rumboot_printf("EMI init\n");
     emi_init(DCR_EM2_EMI_BASE);

     rumboot_printf("cache on\n");
     static const tlb_entry tlb_entry_cacheable_valid_0 = {TLB_ENTRY_CACHE_VALID_0};
     write_tlb_entries(&tlb_entry_cacheable_valid_0, 1);

     static const tlb_entry tlb_entry_cacheable_valid_1 = {TLB_ENTRY_CACHE_VALID_1};
     write_tlb_entries(&tlb_entry_cacheable_valid_1, 1);

     check_dcu_tag();
     rumboot_printf("TEST OK\n");
     return 0;
}

