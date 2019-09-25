/*
 * test_oi10_cpu_mem_017.c
 *
 *  Created on: Dec 31, 2018
 *      Author: v.fufaev
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>

/*                        MMU_TLB_ENTRY(  ERPN,     RPN,         EPN,          DSIZ,              IL1I,   IL1D,    W,      I,      M,      G,           E,                 UX, UW, UR,     SX, SW, SR     DULXE,  IULXE,       TS,         TID,                WAY,              BID,             V )*/
#define TLB_ENTRY0_CACHE  MMU_TLB_ENTRY(  0x000,  0x00000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY1_CACHE  MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY2_CACHE  MMU_TLB_ENTRY(  0x000,  0x80000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY3_CACHE  MMU_TLB_ENTRY(  0x000,  0xC0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY4_CACHE  MMU_TLB_ENTRY(  0x001,  0x00000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_4,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY5_CACHE  MMU_TLB_ENTRY(  0x001,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_5,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY6_CACHE  MMU_TLB_ENTRY(  0x001,  0x80000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_6,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY7_CACHE  MMU_TLB_ENTRY(  0x001,  0xC0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_7,       MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )

#define TLB_ENTRY0_INV    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY1_INV    MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY2_INV    MMU_TLB_ENTRY(  0x000,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY3_INV    MMU_TLB_ENTRY(  0x000,  0xc0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_1,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY4_INV    MMU_TLB_ENTRY(  0x001,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY5_INV    MMU_TLB_ENTRY(  0x001,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_2,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY6_INV    MMU_TLB_ENTRY(  0x001,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY7_INV    MMU_TLB_ENTRY(  0x001,  0xc0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )


#define CACHED_PAGE            0x40000000

#define GET_WINDOW(ERPN_TAG) \
   ( 0x8000 | ((ERPN_TAG >> 19) << 2) | ((ERPN_TAG >> 17) & 0x3))

#define GET_EA_BY_TAG_INDEX(TAG, INDEX) \
    (CACHED_PAGE | ((TAG)<<13) | ((INDEX)<<5))

#define CACHE_DATA_LINE_BY_TAG_INDEX(TAG,INDEX) \
    /*rumboot_printf("TAG = 0x%X\n", TAG);*/ \
    /*rumboot_printf("Index = 0x%X\n", INDEX);*/ \
    /*rumboot_printf("EA = 0x%X\n", (CACHED_PAGE | ((TAG)<<13) | ((INDEX)<<5) | ((0)<<2)));*/ \
    /*dcbi(CACHED_PAGE | ((TAG)<<13) | ((INDEX)<<5) | ((0)<<2));*/ \
    /*msync();*/ \
    /*Minimal caching size is 1 line = 32 bytes = 8 words = 4 doublewords*/ \
    MEM64(CACHED_PAGE | ((TAG)<<13) | ((INDEX)<<5) | ((0)<<2)) = data_array64[0]; \
    MEM64(CACHED_PAGE | ((TAG)<<13) | ((INDEX)<<5) | ((2)<<2)) = data_array64[1]; \
    MEM64(CACHED_PAGE | ((TAG)<<13) | ((INDEX)<<5) | ((4)<<2)) = data_array64[2]; \
    MEM64(CACHED_PAGE | ((TAG)<<13) | ((INDEX)<<5) | ((6)<<2)) = data_array64[3]; \
    msync(); \
    /*1st read initiates a caching*/ \
    COMPARE_VALUES(MEM32(CACHED_PAGE | ((TAG)<<13) | ((INDEX)<<5) | ((0)<<2)),data_array32[0],"Compare failed");\
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

enum {
    TEC_CHECK_SRAM_AB = TEST_EVENT_CODE_MIN,
    TEC_CHECK_SRAM_CD,
    TEC_CHECK_SRAM_EF,
    TEC_CHECK_SRAM_GH
} test_event_code;

volatile uint64_t __attribute__((aligned(128))) data_array64[4] = {
    0xFFFFFFFFFFFFFFFFULL,
    0xFFFFFFFFFFFFFFFFULL,
    0xFFFFFFFFFFFFFFFFULL,
    0xFFFFFFFFFFFFFFFFULL
};

volatile uint32_t *data_array32 = (volatile uint32_t *)data_array64;

//convert RA tag to EA tag
uint32_t get_EA_tag(uint32_t erpn_tag)
{
    uint32_t mem_window_number = GET_WINDOW(erpn_tag) & 0x7;
    if(mem_window_number > 3)
       return (erpn_tag & 0x7FFFF)-((0x40000000 * (mem_window_number - 4)) >> 13);
    else
       return (erpn_tag & 0x7FFFF)-((0x40000000 * mem_window_number) >> 13);
}

/*
 * erpn - 0 or 1 for em0
 * tag - 0..0x7FFFF
 * erpn_tag:
 * [28:19] = ERPN, (may be 0 or 1 only for EM0, see tlb entries)
 * [18:0] = TAG
 * [31:29] = 0
 */
void cache_tag_abcd(uint32_t erpn_tag, uint32_t addr)
{
    uint32_t index = TAG_A_TO_INDEX(addr);
    uint32_t window = GET_WINDOW(erpn_tag);
    uint32_t tag = get_EA_tag(erpn_tag);
    set_mem_window(window);
    //rumboot_printf("====CACHING AB/CD====\n");
    //rumboot_printf("Window 0x%X\n", window);
    //rumboot_printf("Sram.A=0x%X\n", addr);
    //rumboot_printf("Index (RAW)=0x%X\n", index);
    if(index & 1) //ODD
    {
        if(index >=128)
        {
            CACHE_DATA_LINE_BY_TAG_INDEX(tag,(index))//mem[]MSB
            CACHE_DATA_LINE_BY_TAG_INDEX(tag+1,(index))//mem[]LSB
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

void cache_tag_efgh(uint32_t erpn_tag, uint32_t addr)
{
    uint32_t index = TAG_A_TO_INDEX(addr);
    uint32_t window = GET_WINDOW(erpn_tag);
    uint32_t tag = get_EA_tag(erpn_tag);
    set_mem_window(window);
    //rumboot_printf("====CACHING EF/GH====\n");
    //rumboot_printf("Window 0x%X\n", window);
    //rumboot_printf("Sram.A=0x%X\n", addr);
    //rumboot_printf("Index (RAW)=9x%X\n", index);
    if(index & 1) //ODD
    {
        if(index >=128)
        {
            CACHE_DATA_LINE_BY_TAG_INDEX(tag,(index-128))//mem[]MSB
            CACHE_DATA_LINE_BY_TAG_INDEX(tag+1,(index-128))//mem[]LSB
        }
        else
        {
            CACHE_DATA_LINE_BY_TAG_INDEX(tag,(index))
            CACHE_DATA_LINE_BY_TAG_INDEX(tag+1,(index))
        }
    }
    else//EVEN
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
}

int main()
{
    test_event_send_test_id("test_oi10_cpu_mem_017");

    static const tlb_entry tlb_entries[] =
       {
        {TLB_ENTRY0_INV}, {TLB_ENTRY1_INV}, {TLB_ENTRY2_INV}, {TLB_ENTRY3_INV},
        {TLB_ENTRY4_INV}, {TLB_ENTRY5_INV}, {TLB_ENTRY6_INV}, {TLB_ENTRY7_INV},
        {TLB_ENTRY0_CACHE}, {TLB_ENTRY1_CACHE}, {TLB_ENTRY2_CACHE}, {TLB_ENTRY3_CACHE},
        {TLB_ENTRY4_CACHE}, {TLB_ENTRY5_CACHE}, {TLB_ENTRY6_CACHE}, {TLB_ENTRY7_CACHE},
       };

    emi_init(DCR_EM2_EMI_BASE);

    TEST_ASSERT(l2c_l2_read (DCR_L2C_BASE, L2C_L2ISTAT),"L2C Array Initialization Complete Event did not occur!");

    rumboot_printf("Start TLB entries initialization... ");
    write_tlb_entries(tlb_entries, ARRAY_SIZE(tlb_entries));
    msync();
    isync();
    rumboot_printf("done\n");

    //invalidate all. emi_init causes a caching, we must clear it
    dci(0);
    dci(2);
    msync();

     //START TEST
    rumboot_printf("DCU TAG Memory initialization..\n");
    //load initial value (zero)
    cache_tag_abcd((0b000000000000000000 << 2) | 0b00,0x00);//AB A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b00,0x3F);//AB A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b00,0x40);//AB A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b00,0x7F);//AB A=
//                     E                 TAG   +   WAY
    cache_tag_abcd((0b000000000000000000 << 2) | 0b10,0x00);//CD A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b10,0x3F);//CD A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b10,0x40);//CD A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b10,0x7F);//CD A=
//                     E                 TAG   +   WAY
    cache_tag_efgh((0b000000000000000000 << 2) | 0b00,0x00);//EF A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b00,0x3F);//EF A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b00,0x40);//EF A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b00,0x7F);//EF A=
//                     E                 TAG   +   WAY
    cache_tag_efgh((0b000000000000000000 << 2) | 0b10,0x00);//GH A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b10,0x3F);//GH A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b10,0x40);//GH A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b10,0x7F);//GH A=

    //clear 'valid' for initial valuies
    dci(0);
    dci(2);
    msync();

    rumboot_printf("DCU TAG Memory checking:\n");

//                     E                 TAG   +   WAY
    test_event(TEC_CHECK_SRAM_AB);
    cache_tag_abcd((0b111111111111111111 << 2) | 0b00,0x00);//AB A=
    cache_tag_abcd((0b111111111111111111 << 2) | 0b00,0x3F);//AB A=
    cache_tag_abcd((0b111111111111111111 << 2) | 0b00,0x40);//AB A=
    cache_tag_abcd((0b111111111111111111 << 2) | 0b00,0x7F);//AB A=

//                     E                 TAG   +   WAY
    test_event(TEC_CHECK_SRAM_CD);
    cache_tag_abcd((0b111111111111111111 << 2) | 0b10,0x00);//CD A=
    cache_tag_abcd((0b111111111111111111 << 2) | 0b10,0x3F);//CD A=
    cache_tag_abcd((0b111111111111111111 << 2) | 0b10,0x40);//CD A=
    cache_tag_abcd((0b111111111111111111 << 2) | 0b10,0x7F);//CD A=

//                     E                 TAG   +   WAY
    test_event(TEC_CHECK_SRAM_EF);
    cache_tag_efgh((0b111111111111111111 << 2) | 0b00,0x00);//EF A=
    cache_tag_efgh((0b111111111111111111 << 2) | 0b00,0x3F);//EF A=
    cache_tag_efgh((0b111111111111111111 << 2) | 0b00,0x40);//EF A=
    cache_tag_efgh((0b111111111111111111 << 2) | 0b00,0x7F);//EF A=

//                     E                 TAG   +   WAY
    test_event(TEC_CHECK_SRAM_GH);
    cache_tag_efgh((0b111111111111111111 << 2) | 0b10,0x00);//GH A=
    cache_tag_efgh((0b111111111111111111 << 2) | 0b10,0x3F);//GH A=
    cache_tag_efgh((0b111111111111111111 << 2) | 0b10,0x40);//GH A=
    cache_tag_efgh((0b111111111111111111 << 2) | 0b10,0x7F);//GH A=

    //clear 'valid' for initial valuies
    dci(0);
    dci(2);
    msync();

    test_event(TEC_CHECK_SRAM_AB);
    cache_tag_abcd((0b000000000000000000 << 2) | 0b00,0x00);//AB A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b00,0x3F);//AB A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b00,0x40);//AB A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b00,0x7F);//AB A=

    test_event(TEC_CHECK_SRAM_CD);
    cache_tag_abcd((0b000000000000000000 << 2) | 0b10,0x00);//CD A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b10,0x3F);//CD A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b10,0x40);//CD A=
    cache_tag_abcd((0b000000000000000000 << 2) | 0b10,0x7F);//CD A=

    test_event(TEC_CHECK_SRAM_EF);
    cache_tag_efgh((0b000000000000000000 << 2) | 0b00,0x00);//EF A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b00,0x3F);//EF A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b00,0x40);//EF A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b00,0x7F);//EF A=

    test_event(TEC_CHECK_SRAM_GH);
    cache_tag_efgh((0b000000000000000000 << 2) | 0b10,0x00);//GH A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b10,0x3F);//GH A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b10,0x40);//GH A=
    cache_tag_efgh((0b000000000000000000 << 2) | 0b10,0x7F);//GH A=

    return 0;
}
