/*
 * test_oi10_cpu_mem_021.c
 *
 *  Created on: Dec 21, 2018
 *      Author: a.gurov
 */


#include <stdio.h>
#include <stdint.h>
#include <string.h>

//#define RUMBOOT_ASSERT_WARN_ONLY

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/memfill.h>

#include <platform/common_macros/common_macros.h>

#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>

#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <platform/arch/ppc/ppc_476fp_l1c_fields.h>

///* Config */
#define L2C_REQUEST_TRIES           0x80
//
///* Integer constant macros */
#define TEST_OK                     0x00000000
#define BUFFERID_TAG                L2C_L2ARRACCCTL_BUFFERID_TAG_i
#define REQTYPE_RD8WOECC            L2C_L2ARRACCCTL_REQTYPE_RD8WOECC
#define MAY_NOT_BE_USED             __attribute__((unused))
#define ALWAYS_INLINE               __attribute__((always_inline))
//
///* Utility (function) macros */
#define BIT(N)                      (1 << (N))
#define GEN_REQTYPE(REQTYPE)        (REQTYPE << L2C_L2ARRACCCTL_REQTYPE_i)
#define GEN_WAY_FIELD(WAY)          ((WAY) << L2C_L2ARRACCCTL_L2WAY_i)
#define L2C_ARR_RD_TAG_RQ(WAY)      (BIT(L2C_L2ARRACCCTL_REQUEST_i)  | \
                                     BIT(BUFFERID_TAG)               | \
                                     GEN_REQTYPE(REQTYPE_RD8WOECC)   | \
                                     GEN_WAY_FIELD(WAY))


union u64_u32x2_t
{
    uint32_t    u32[2];
    uint64_t    u64;
     int64_t    i64;
};

typedef union u64_u32x2_t u64_u32x2_t;

/* Global vars */
MAY_NOT_BE_USED
/*      MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
static const tlb_entry em_tlb_entry_cache_on[] = {{
        MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
}};
MAY_NOT_BE_USED
static uint32_t l2c_cache_sizes[4] =
    {256*1024, 512*1024, 1024*1024, 128*1024};
MAY_NOT_BE_USED
static uint32_t l2c_addr_masks[4] =
    /* 256     512     1024    128 */
    {0x03FE, 0x07FE, 0x0FFE, 0x01FE};
MAY_NOT_BE_USED
static uint32_t l2c_read_req[4] =
{
        L2C_ARR_RD_TAG_RQ(0),
        L2C_ARR_RD_TAG_RQ(1),
        L2C_ARR_RD_TAG_RQ(2),
        L2C_ARR_RD_TAG_RQ(3)
};

static volatile uint32_t g_cnfg0 = 0;
static volatile uint32_t g_cnfg1 = 0;


BEGIN_ENUM(PSEUDO_CT_DECODING)
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L1I_i,         0    )
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L1I_n,         1    )
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L1I_mask,     FIELD_MASK32( PSEUDO_CT_DECODING_IS_L1I_i, PSEUDO_CT_DECODING_IS_L1I_n ) )
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L1D_val,       0    )

    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L2_i,         1    )
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L2_n,         1    )
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L2_mask,     FIELD_MASK32( PSEUDO_CT_DECODING_IS_L2_i, PSEUDO_CT_DECODING_IS_L2_n ) )
END_ENUM(PSEUDO_CT_DECODING)

bool get_valid_way_by_addr(uint32_t pseudo_CT, void* addr, int32_t* cache_way)
{
    uint64_t    phys_addr = 0;
    uint32_t    reg_xCDBTRH;
#ifdef L2C_TRACE_DEBUG_MSG
    uint32_t    reg_xCDBTRL;
#endif
    bool        tag_valid;
    uint32_t    CT = pseudo_CT & PSEUDO_CT_DECODING_IS_L2_mask;
    bool        is_l1i = pseudo_CT & PSEUDO_CT_DECODING_IS_L1I_mask;
    uint32_t    tag_info;

    if(CT == 0)
    {
        if(is_l1i)
        {
            spr_write(SPR_ICDBTRL,0);
            spr_write(SPR_ICDBTRH,0);
        }
        else
        {
            spr_write(SPR_DCDBTRL,0);
            spr_write(SPR_DCDBTRH,0);
        }
    }
    phys_addr = (uint64_t)rumboot_virt_to_phys(addr);
    *cache_way = -1;
    if(CT == 2)
    {
#ifdef L2C_TRACE_DEBUG_MSG
        rumboot_printf("l2c_arracc_get_valid_way_by_address(0x%x)\n",addr);
#endif
        if( l2c_arracc_get_way_by_address(DCR_L2C_BASE,
                    (uint32_t)((phys_addr >> 32) & 0xffffffff),
                    (uint32_t)((phys_addr      ) & 0xffffffff),
                    cache_way
                    ))
        {
            if(l2c_arracc_tag_info_read_by_way( DCR_L2C_BASE,
                    (uint32_t)((phys_addr >> 32) & 0xffffffff),
                    (uint32_t)((phys_addr      ) & 0xffffffff),
                    *cache_way, &tag_info ))
                return (((tag_info & L2C_L2ARRACCDO0_TAG_CACHE_STATE_mask) >> L2C_L2ARRACCDO0_TAG_CACHE_STATE_i)
                        != L2C_L2ARRACCDO0_TAG_CACHE_STATE_INVALID);
            else
                return false;
        }else
            return false;
    } else if(CT == 0)
    {
        do{
            ( int32_t )( *cache_way )++;
            msync();
            if(is_l1i)
            {
                icread((uint32_t*)(((uint32_t)addr & (XCREAD_EA_L1I_INDEX_mask | XCREAD_EA_WORD_ADDR_mask))
                        | (((*cache_way) << XCREAD_EA_L1I_WAY_i) & XCREAD_EA_L1I_WAY_mask)));
                isync();
#ifdef L2C_TRACE_DEBUG_MSG
                reg_xCDBTRL = spr_read(SPR_ICDBTRL);
#endif
                reg_xCDBTRH = spr_read(SPR_ICDBTRH);
            }
            else
            {
                dcread(((uint32_t*)(((uint32_t)addr & (XCREAD_EA_L1I_INDEX_mask | XCREAD_EA_WORD_ADDR_mask))
                        | (((*cache_way) << XCREAD_EA_L1I_WAY_i) & XCREAD_EA_L1I_WAY_mask))));
                isync();
#ifdef L2C_TRACE_DEBUG_MSG
                reg_xCDBTRL = spr_read(SPR_DCDBTRL);
#endif
                reg_xCDBTRH = spr_read(SPR_DCDBTRH);
            }
            tag_valid = (reg_xCDBTRH & XCDBTRH_VALID_mask);
#ifdef L2C_TRACE_DEBUG_MSG
            rumboot_printf("\nget_valid_way_by_addr, CT == %d, %s\ncache_way == %d\naddr == 0x%x\nicread (0x%x)\nxCDBTRH == 0x%x\nxCDBTRL == 0x%x\nvalid == %d\n",
                    CT,
                    CT ? "" : (is_l1i ? "L1I" : "L1D"),
                    *cache_way,
                    (uint32_t)addr,
                    (uint32_t*)(((uint32_t)addr & (XCREAD_EA_L1I_INDEX_mask | XCREAD_EA_WORD_ADDR_mask))
                            | (((*cache_way) << XCREAD_EA_L1I_WAY_i) & XCREAD_EA_L1I_WAY_mask)),
                    reg_xCDBTRH,
                    reg_xCDBTRL,
                    tag_valid);
#endif
        }while( ( (*cache_way) < (L2C_COUNT_WAYS - 1))
                 && ( !( (tag_valid)
                     &&( ( ( (reg_xCDBTRH & XCDBTRH_TAG_EXT_ADDR_mask) >> XCDBTRH_TAG_EXT_ADDR_i) == (uint32_t)((phys_addr >> 32) & 0xffffffff) )
                       &&( (reg_xCDBTRH & XCDBTRH_TAG_ADDR_mask) == ( phys_addr & XCDBTRH_TAG_ADDR_mask ) )
                       )
                     ) )
              );
        if( tag_valid && ( ( ( (reg_xCDBTRH & XCDBTRH_TAG_EXT_ADDR_mask) >> XCDBTRH_TAG_EXT_ADDR_i) == (uint32_t)((phys_addr >> 32) & 0xffffffff) )
                       &&( (reg_xCDBTRH & XCDBTRH_TAG_ADDR_mask) == ( phys_addr & XCDBTRH_TAG_ADDR_mask ) )
                       ) )
            return true;
    }
    return false;
}

uint32_t check_caches( uint32_t  (*array_of_addr[])(), bool expected_in_cache, uint32_t pseudo_CT, char* test_name){
    bool        found_in_cache;
    int32_t     cache_way;
    uint32_t    result = 0;
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        found_in_cache = get_valid_way_by_addr(
                pseudo_CT ,
                (void*)(array_of_addr[i]),
                &cache_way
                );

        if(found_in_cache != expected_in_cache)
        {
            rumboot_printf("%s ERROR!!! i == %d: found in %s: %s, CT == %d\n",
                    test_name,
                    i,
                    (pseudo_CT & PSEUDO_CT_DECODING_IS_L2_mask)?"L2C":
                            (pseudo_CT & PSEUDO_CT_DECODING_IS_L1I_mask)?"L1I":"L1D",
                    (found_in_cache)?"yes":"no",
                    (pseudo_CT & PSEUDO_CT_DECODING_IS_L2_mask));
            result |= 1;
        }
    }
    return result;
}

uint32_t test_data[] = {
        0x00000000, 0xFFFFFFFF,0x0000FFFF, 0xFFFF0000, 0x00FF00FF,0xFF00FF00,0x0F0F0F0F,0xF0F0F0F0,0x33333333,0xCCCCCCCC,0x55555555,0xAAAAAAAA
};

static inline uint32_t get_addr_by_some_sophisticated_rule(uint32_t index)
{
    uint32_t start_test_data_index = 2;
    return (((((test_data[index % 12] & l2c_addr_masks[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask])
                               + (((index % 12) < start_test_data_index)? ((index + 1) << 2 ) : 0))
                               ^ ((index >= 12) ? (1 << 8) : 0)))
            & l2c_addr_masks[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]);
}
static inline uint32_t get_addr_by_some_more_sophisticated_rule(uint32_t index, uint32_t way)
{
    return (SDRAM_BASE + get_addr_by_some_sophisticated_rule(index)*L2C_LINE_SIZE/2 + way * l2c_cache_sizes[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]);
}
uint32_t l2arracc_lru_check(uint32_t base)
{
    uint32_t result = 0;
    uint32_t default_data = 0;
    char test_name[] = "l2arracc_lru_check";
    rumboot_printf("%s started\n", test_name);

    l2c_l2_write( base, L2C_L2ARRACCADR, get_addr_by_some_sophisticated_rule(0) );
    l2c_l2_write( base, L2C_L2ARRACCCTL,
                  L2C_L2ARRACCCTL_REQUEST_mask
                | L2C_L2ARRACCCTL_BUFFERID_LRU_mask
                | (L2C_L2ARRACCCTL_REQTYPE_RD8WOECC << L2C_L2ARRACCCTL_REQTYPE_i)
        );
    for(uint32_t tries = L2C_REQUEST_TRIES;
        (l2c_l2_read(base,L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_REQUEST_mask)
                && tries;
        --tries); /* No body for this 'for' */
    default_data = l2c_l2_read( base, L2C_L2ARRACCDO0) & L2C_L2ARRACCDO0_LRU_LAYOUT_mask;

    /*write cycle*/
    for(uint32_t i = 0; i < ARRAY_SIZE(test_data); ++i)
    {
        l2c_l2_write( base, L2C_L2ARRACCADR, get_addr_by_some_sophisticated_rule(i));
        l2c_l2_write( base, L2C_L2ARRACCDI0, test_data[i] & L2C_L2ARRACCDO0_LRU_LAYOUT_mask);
        l2c_l2_write( base, L2C_L2ARRACCCTL,
                      L2C_L2ARRACCCTL_REQUEST_mask
                    | L2C_L2ARRACCCTL_BUFFERID_LRU_mask
                    | (L2C_L2ARRACCCTL_REQTYPE_WR8WOECC << L2C_L2ARRACCCTL_REQTYPE_i)
            );
        for(uint32_t tries = L2C_REQUEST_TRIES;
            (l2c_l2_read(base,L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_REQUEST_mask)
                    && tries;
            --tries); /* No body for this 'for' */
    }
    /*read cycle*/
    for(uint32_t i = 0, data = 0; i < ARRAY_SIZE(test_data); ++i)
    {
        l2c_l2_write( base, L2C_L2ARRACCADR, get_addr_by_some_sophisticated_rule(i) );
        l2c_l2_write( base, L2C_L2ARRACCCTL,
                      L2C_L2ARRACCCTL_REQUEST_mask
                    | L2C_L2ARRACCCTL_BUFFERID_LRU_mask
                    | (L2C_L2ARRACCCTL_REQTYPE_RD8WOECC << L2C_L2ARRACCCTL_REQTYPE_i)
            );
        for(uint32_t tries = L2C_REQUEST_TRIES;
            (l2c_l2_read(base,L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_REQUEST_mask)
                    && tries;
            --tries); /* No body for this 'for' */
        data = l2c_l2_read( base, L2C_L2ARRACCDO0) & L2C_L2ARRACCDO0_LRU_LAYOUT_mask;
        if((data & L2C_L2ARRACCDO0_LRU_LAYOUT_mask) != (test_data[i] & L2C_L2ARRACCDO0_LRU_LAYOUT_mask))
        {
            rumboot_printf("ERROR!!! Written data to LRU does not match the written data. Written == 0x%x, read == 0x%x, address == 0x%x\n",
                    test_data[i] & L2C_L2ARRACCDO0_LRU_LAYOUT_mask, data, get_addr_by_some_sophisticated_rule(i));
            result |= 1;
        }
//        else
//            rumboot_printf("Written data to LRU matches the written data. Written == 0x%x, read == 0x%x, address == 0x%x\n",
//                    test_data[i] & L2C_L2ARRACCDO0_LRU_LAYOUT_mask, data, get_addr_by_some_sophisticated_rule(i));
    }
    /*clear cycle*/
    for(uint32_t i = 0; i < ARRAY_SIZE(test_data); ++i)
    {
        l2c_l2_write( base, L2C_L2ARRACCADR, get_addr_by_some_sophisticated_rule(i));
        l2c_l2_write( base, L2C_L2ARRACCDI0, default_data);
        l2c_l2_write( base, L2C_L2ARRACCCTL,
                      L2C_L2ARRACCCTL_REQUEST_mask
                    | L2C_L2ARRACCCTL_BUFFERID_LRU_mask
                    | (L2C_L2ARRACCCTL_REQTYPE_WR8WOECC << L2C_L2ARRACCCTL_REQTYPE_i)
            );
        for(uint32_t tries = L2C_REQUEST_TRIES;
            (l2c_l2_read(base,L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_REQUEST_mask)
                    && tries;
            --tries); /* No body for this 'for' */
    }

    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;

}

DECLARE_CONST(LRU_VALUES_ARRAY_SIZE,64)
DECLARE_CONST(LRU_VALUES_COUNT,24)
uint32_t  lru_value_based_lru_check(uint32_t base)
{
    uint32_t update_j_index(uint32_t index_i,uint32_t index_j)
    {
        do ++index_j; while(index_i == index_j);
        return index_j;
    }
    uint32_t update_k_index(uint32_t index_i,uint32_t index_j,uint32_t index_k)
    {
        do ++index_k; while((index_i == index_k) || (index_j == index_k));
        return index_k;
    }
    uint32_t result = 0;
    uint32_t addr;
    uint32_t index = 0;
    uint32_t array_of_addrs[4];
    uint32_t lru_values_array[LRU_VALUES_ARRAY_SIZE];
    uint32_t lru_info;
    uint32_t lru_value;
    uint32_t initial_value = 0xffffffff;

    char test_name[] = "lru_value_based_lru_check";
    rumboot_printf("%s started\n", test_name);
    for (uint32_t i = 0; i < LRU_VALUES_COUNT; ++i)
    {
        for(uint32_t j = 0; j < L2C_COUNT_WAYS; ++j)
        {
            /*1st stage: initializing necessary data areas*/
            addr = get_addr_by_some_more_sophisticated_rule (i,j);
            rumboot_memfill32((void*)addr,L2C_LINE_SIZE/4,addr,4);
            /*2nd stage: caching initialized data areas; */
            dcbt(0, addr);
            array_of_addrs[j] = addr;
        }
        isync();
        msync();
        result |= check_caches((uint32_t (**)())array_of_addrs, true,PSEUDO_CT_DECODING_IS_L2_mask,"initial caching");
    }
    /*3rd stage: accessing cached areas in a specific order, making all LRU values unique*/
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
        for(uint32_t j = update_j_index(i,-1); j < L2C_COUNT_WAYS; j = update_j_index(i,j))
            for(uint32_t k = update_k_index(i,j,-1); k < L2C_COUNT_WAYS; k = update_k_index(i,j,k))
            {
                rumboot_printf("3rd stage: index =%d, i = %d, j = %d, k = %d\naddr[i] == 0x%x; addr[j] == 0x%x; addr[k] == 0x%x\n",
                        index,i,j,k,
                        get_addr_by_some_more_sophisticated_rule (index,i),
                        get_addr_by_some_more_sophisticated_rule (index,j),
                        get_addr_by_some_more_sophisticated_rule (index,k));
                dcbt(2, get_addr_by_some_more_sophisticated_rule (index,i));
                dcbt(2, get_addr_by_some_more_sophisticated_rule (index,j));
                dcbt(2, get_addr_by_some_more_sophisticated_rule (index,k));
                index++;
            }
//loop_exit:
    isync();
    msync();
    rumboot_memfill8(lru_values_array,sizeof(lru_values_array),initial_value,0);
    for (uint32_t i = 0; i < LRU_VALUES_COUNT; ++i)
    {
        for(uint32_t j = 0; j < L2C_COUNT_WAYS; ++j)
        {
            addr = get_addr_by_some_more_sophisticated_rule (i,j);
            array_of_addrs[j] = addr;
        }
        /*4th stage: checking that everything is still cached*/
        result |= check_caches((uint32_t (**)())array_of_addrs, true,PSEUDO_CT_DECODING_IS_L2_mask,"lru_value_based_lru_check");
        /*5th stage: checking that every set of cached areas has a unique LRU value*/
        if(l2c_arracc_lru_info_read_by_way(DCR_L2C_BASE,
                (uint32_t)(rumboot_virt_to_phys((void *)addr) >> 32) & 0xFFFFFFFF,
                (uint32_t)(rumboot_virt_to_phys((void *)addr) & 0xFFFFFFFF),
                0,&lru_info))
        {
            lru_value = ((lru_info & L2C_L2ARRACCDO0_LRU_LRU_mask) >> L2C_L2ARRACCDO0_LRU_LRU_i);
            rumboot_printf("addr == 0x%x, lru_info == 0x%x, lru_value == 0x%x\n",addr,lru_info,lru_value);
            if(lru_values_array[lru_value] == initial_value)
                lru_values_array[lru_value] = lru_value;
            else
            {
                rumboot_printf("ERROR!!! LRU value == 0x%x already occurred for addr == 0x%x\nlru_info == 0x%x\n",lru_value,addr,lru_info);
                result |= 1;
            }
        }
        else
        {
            rumboot_printf("ERROR!!! I did not find any entry that corresponds with this addr == 0x%x\n",addr);
            result |= 1;
        }
    }

//    rumboot_printf("Just for information: valid values of LRU:\n");
//    for (uint32_t i = 0; i < LRU_VALUES_ARRAY_SIZE; ++i)
//    {
////        if(lru_values_array[i] != initial_value)
//        rumboot_printf("0x%x\n", lru_values_array[i]);
//    }

    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}

uint32_t main(void)
{
    uint32_t     status  = TEST_OK;

    rumboot_printf("Init EMI...\n");
    emi_init(DCR_EM2_EMI_BASE);
    g_cnfg0 = l2c_l2_read(DCR_L2C_BASE,L2C_L2CNFG0);
    g_cnfg1 = l2c_l2_read(DCR_L2C_BASE,L2C_L2CNFG1);
    status |= l2arracc_lru_check(DCR_L2C_BASE);
    write_tlb_entries(em_tlb_entry_cache_on, 1);
    msync();
    isync();
    status |= lru_value_based_lru_check(DCR_L2C_BASE);

    rumboot_printf("TEST %s!\n", !status ? "OK" : "ERROR");

    return status;
}


