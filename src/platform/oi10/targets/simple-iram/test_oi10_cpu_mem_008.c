/*
 * test_oi10_cpu_mem_008_base.c
 *
 *  Created on: Oct 9, 2018
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/dma2plb6.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/devices/dma2plb6.h>

//                                      MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_CACHE_VALID_0           MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_VALID_1           MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

typedef void func();

static volatile uint64_t test_array[]={
        0x4E80002060000000,
        0x6000000060000000,
};

#define GET_ADDR_BY_TAG_INDEX_WORD(TAG,INDEX,WORD) \
        ( (0x00) | ((TAG) << 13) | ((INDEX) << 5) | ((WORD) << 2) )

#define CODE_COPY_RUN(ADDRESS) \
    do{ \
        rumboot_printf("address = %x\n", (uint32_t) ADDRESS); \
        write_blr_l2_line_by_addr(ADDRESS); \
        func* f = (func*)(ADDRESS); \
        f(); \
        icread((void *)ADDRESS); \
        isync(); \
        TEST_ASSERT( (ADDRESS >> 13) == (spr_read(SPR_ICDBTRH) >> 13), "Wrong value in SPR_ICDBTRH" ); \
    }while (0);

//rumboot_printf("SPR = %x\n",spr_read(SPR_ICDBTRH)) ;

void write_blr_l2_line_by_addr(uint32_t volatile address)
{
    //write 1 line
    iowrite64(test_array[0], address);
    iowrite64(test_array[1], address+8);
    msync();
}

#define GET_WINDOW(ERPN_TAG) \
   ( 0x8000 | (((ERPN_TAG) >> 19) << 2) | (((ERPN_TAG) >> 17) & 0x3))

uint32_t get_EA_tag(uint32_t erpn_tag)
{
    uint32_t mem_window_number = GET_WINDOW(erpn_tag) & 0x7;
    if(mem_window_number > 3)
       return (erpn_tag & 0x7FFFF)-((0x40000000 * (mem_window_number - 4)) >> 13);
    else
       return (erpn_tag & 0x7FFFF)-((0x40000000 * mem_window_number) >> 13);
}

void cache_icu_tag(uint32_t erpn_tag, uint32_t index)
{
    rumboot_printf("ERPN_TAG = %x\n", erpn_tag);
    uint32_t tag = get_EA_tag(erpn_tag);
    rumboot_printf("TAG = %x\n", tag);
    //set_mem_window(GET_WINDOW(erpn_tag));

    CODE_COPY_RUN(GET_ADDR_BY_TAG_INDEX_WORD(tag,index,0));
    CODE_COPY_RUN(GET_ADDR_BY_TAG_INDEX_WORD(tag+1,index,0));
    rumboot_printf("\n");
}

bool test_icu_tag_array()
{
    rumboot_printf("Start test icu tag array\n");

//                   E                 TAG   +   WAY
    cache_icu_tag((0b111111111111111111 << 2) | 0b00,0x0);//array 0
    cache_icu_tag((0b000000000000000000 << 2) | 0b00,0x2);//array 0
    cache_icu_tag((0b001111111111111111 << 2) | 0b00,0x4);//array 0
    cache_icu_tag((0b110000000000000000 << 2) | 0b00,0x6);//array 0
    cache_icu_tag((0b001111111100000000 << 2) | 0b00,0x8);//array 0
    cache_icu_tag((0b110000000011111111 << 2) | 0b00,0xA);//array 0
    cache_icu_tag((0b001111000011110000 << 2) | 0b00,0xC);//array 0
    cache_icu_tag((0b110000111100001111 << 2) | 0b00,0xE);//array 0
    cache_icu_tag((0b001100110011001100 << 2) | 0b00,0x10);//array 0
    cache_icu_tag((0b110011001100110011 << 2) | 0b00,0x12);//array 0
    cache_icu_tag((0b101010101010101010 << 2) | 0b00,0x14);//array 0
    cache_icu_tag((0b010101010101010101 << 2) | 0b00,0x16);//array 0
    //test_event(EVENT_CACHING_DONE);
//                   E                 TAG   +   WAY
    cache_icu_tag((0b111111111111111111 << 2) | 0b00,0x80);//array 0
    cache_icu_tag((0b000000000000000000 << 2) | 0b00,0x82);//array 0
    cache_icu_tag((0b001111111111111111 << 2) | 0b00,0x84);//array 0
    cache_icu_tag((0b110000000000000000 << 2) | 0b00,0x86);//array 0
    cache_icu_tag((0b001111111100000000 << 2) | 0b00,0x88);//array 0
    cache_icu_tag((0b110000000011111111 << 2) | 0b00,0x8A);//array 0
    cache_icu_tag((0b001111000011110000 << 2) | 0b00,0x8C);//array 0
    cache_icu_tag((0b110000111100001111 << 2) | 0b00,0x8E);//array 0
    cache_icu_tag((0b001100110011001100 << 2) | 0b00,0x90);//array 0
    cache_icu_tag((0b110011001100110011 << 2) | 0b00,0x92);//array 0
    cache_icu_tag((0b101010101010101010 << 2) | 0b00,0x94);//array 0
    cache_icu_tag((0b010101010101010101 << 2) | 0b00,0x96);//array 0
    //test_event(EVENT_CACHING_DONE);

//                   E                 TAG   +   WAY
    cache_icu_tag((0b111111111111111111 << 2) | 0b00,0x0);//array 0
    cache_icu_tag((0b000000000000000000 << 2) | 0b10,0x2);//array 0
    cache_icu_tag((0b001111111111111111 << 2) | 0b10,0x4);//array 0
    cache_icu_tag((0b110000000000000000 << 2) | 0b10,0x6);//array 0
    cache_icu_tag((0b001111111100000000 << 2) | 0b10,0x8);//array 0
    cache_icu_tag((0b110000000011111111 << 2) | 0b10,0xA);//array 0
    cache_icu_tag((0b001111000011110000 << 2) | 0b10,0xC);//array 0
    cache_icu_tag((0b110000111100001111 << 2) | 0b10,0xE);//array 0
    cache_icu_tag((0b001100110011001100 << 2) | 0b10,0x10);//array 0
    cache_icu_tag((0b110011001100110011 << 2) | 0b10,0x12);//array 0
    cache_icu_tag((0b101010101010101010 << 2) | 0b10,0x14);//array 0
    cache_icu_tag((0b010101010101010101 << 2) | 0b10,0x16);//array 0
    //test_event(EVENT_CACHING_DONE);
//                   E                 TAG   +   WAY
    cache_icu_tag((0b111111111111111111 << 2) | 0b10,0x80);//array 0
    cache_icu_tag((0b000000000000000000 << 2) | 0b10,0x82);//array 0
    cache_icu_tag((0b001111111111111111 << 2) | 0b10,0x84);//array 0
    cache_icu_tag((0b110000000000000000 << 2) | 0b10,0x86);//array 0
    cache_icu_tag((0b001111111100000000 << 2) | 0b10,0x88);//array 0
    cache_icu_tag((0b110000000011111111 << 2) | 0b10,0x8A);//array 0
    cache_icu_tag((0b001111000011110000 << 2) | 0b10,0x8C);//array 0
    cache_icu_tag((0b110000111100001111 << 2) | 0b10,0x8E);//array 0
    cache_icu_tag((0b001100110011001100 << 2) | 0b10,0x90);//array 0
    cache_icu_tag((0b110011001100110011 << 2) | 0b10,0x92);//array 0
    cache_icu_tag((0b101010101010101010 << 2) | 0b10,0x94);//array 0
    cache_icu_tag((0b010101010101010101 << 2) | 0b10,0x96);//array 0
    //test_event(EVENT_CACHING_DONE);

    return true;
}

int main(void)
{
    emi_init(DCR_EM2_EMI_BASE);

    //write valid cacheable for MEM
    static const tlb_entry tlb_entry_cacheable_valid_0 = {TLB_ENTRY_CACHE_VALID_0};
    write_tlb_entries(&tlb_entry_cacheable_valid_0, 1);

    static const tlb_entry tlb_entry_cacheable_valid_1 = {TLB_ENTRY_CACHE_VALID_1};
    write_tlb_entries(&tlb_entry_cacheable_valid_1, 1);

    //START TEST (call func in test_oi10_cpu_mem_008.c)
    bool result = test_icu_tag_array();
    if (!result)
    {
        rumboot_printf("TEST_ERROR\n");
        return 1;
    }

    rumboot_printf("TEST_OK\n");
    return 0;
}
