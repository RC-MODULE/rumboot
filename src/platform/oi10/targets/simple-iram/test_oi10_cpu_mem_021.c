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

/* Config */
#define CHECK_BASE                  SRAM1_BASE
#define CHECK_OFFSET                0x0000      //0x0200
#define L2C_LINE_BYTES              128
#define L2C_REQUEST_TRIES           0x80

/* Integer constant macros */
#define TEST_OK                     0x00000000
#define TEST_ERROR                  !TEST_OK
#define BYTE_MASK                   0xFF
#define NOTHING                     0x00000000
#define NO                          NOTHING
#define YES                         (!NO)
#define FAIL_VALUE                  ~NOTHING
#define PPC_EXT_ADDR_WIDTH              42
#define MACHINE_BITS                (sizeof(uintptr_t) << 3)
/* + L2C specific constants */
#define L2C_LINE_WORDS              (L2C_LINE_BYTES / sizeof(uintptr_t))
#define BUFFERID_LRU                L2C_L2ARRACCCTL_BUFFERID_LRU_i
#define BUFFERID_TAG                L2C_L2ARRACCCTL_BUFFERID_TAG_i
#define BUFFERID_DATA               L2C_L2ARRACCCTL_BUFFERID_DATA_i
#define REQTYPE_RD8WOECC            L2C_L2ARRACCCTL_REQTYPE_RD8WOECC
#define REQTYPE_WR8WTECC            L2C_L2ARRACCCTL_REQTYPE_WR8WTECC
#define REQTYPE_WR8WOECC            L2C_L2ARRACCCTL_REQTYPE_WR8WOECC
#define L2C_TAG_ADDR_SHIFT          6
#define L2C_TAG_STAT_BITS           3
#define L2C_TAG_STAT_MASK           0x07
#define L2C_TAG_ADDR_MASK           0x1FFFFFFF
#define L2C_TAG_STAT_SHIFT          (32 - L2C_TAG_STAT_BITS)
#define L2C_TAG_ENTRY_SHIFT_LO      ((PPC_EXT_ADDR_WIDTH       \
                                        - MACHINE_BITS)        \
                                        + L2C_TAG_STAT_BITS)
#define L2C_TAG_ENTRY_SHIFT_HI      (     L2C_TAG_STAT_BITS)

/* ++ L2C Cache State (debug) Modes */
#define L2C_CSM_DEFAULT             0x00
#define L2C_CSM_NO_SL               0x01
#define L2C_CSM_NO_MU               0x02
#define L2C_CSM_NO_T                0x04
#define L2C_CSM_MESI_ONLY           0x07

/* ++ L2C Tag Array Cache State */
#define L2C_STATE_I                 0b000   /* Invalid              */
#define L2C_STATE_U                 0b001   /* Undefined (not used) */
#define L2C_STATE_S                 0b010   /* Shared               */
#define L2C_STATE_SL                0b011   /* Shared Last          */
#define L2C_STATE_E                 0b100   /* Exclusive            */
#define L2C_STATE_T                 0b101   /* Tagged               */
#define L2C_STATE_M                 0b110   /* Modified             */
#define L2C_STATE_MU                0b111   /* Modified Unsolicited */


#define MAY_NOT_BE_USED             __attribute__((unused))
#define ALWAYS_INLINE               __attribute__((always_inline))

/* Utility (function) macros */
#define BIT(N)                      (1 << (N))
#define CAST_ADDR(VAL)              ((void*)(VAL))
#define GEN_REQTYPE(REQTYPE)        (REQTYPE << L2C_L2ARRACCCTL_REQTYPE_i)
#define GEN_WAY_FIELD(WAY)          ((WAY) << L2C_L2ARRACCCTL_L2WAY_i)
#define CHECK_ADDR(OFFT)            (CHECK_BASE + CHECK_OFFSET + (OFFT))
#define GET_BYTE(VAL,BYTE)          (((VAL) >> ((BYTE) << 3)) & BYTE_MASK)
#define L2C_TAG_XTR_ST(TD)          (((TD) >> L2C_TAG_STAT_SHIFT)      \
                                        & L2C_TAG_STAT_MASK)
#define L2C_READ(REG)               l2c_l2_read(DCR_L2C_BASE,          \
                                        (L2C_L2REG)(REG))
#define L2C_WRITE(REG,VAL)          l2c_l2_write(DCR_L2C_BASE,         \
                                        (L2C_L2REG)(REG),(VAL))
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


MAY_NOT_BE_USED
static uint32_t mem_read32(uint32_t ptr)
{
    uint32_t readed;
    rumboot_printf("Reading from 0x%X...\n", CHECK_ADDR(ptr));
    readed = ioread32(CHECK_ADDR(ptr));
    g_cnfg1 = L2C_READ(L2C_L2CNFG1);
    rumboot_printf("Readed: 0x%X (%d).\n", readed, readed);
    return readed;
}

MAY_NOT_BE_USED
static void mem_write32(uint32_t val, uint32_t ptr)
{
    rumboot_printf("Writing 0x%X (%d) to 0x%X...\n",
            val, val, CHECK_ADDR(ptr));
    iowrite32(val, CHECK_ADDR(ptr));
    g_cnfg1 = L2C_READ(L2C_L2CNFG1);
}

MAY_NOT_BE_USED
static int32_t l2c_read_lru(uintptr_t       addr,
                            u64_u32x2_t    *lru_addr,
                            int32_t        *way_out)
{
    uint32_t way, tries, result, l2c_addr32;
    int32_t  l2c_stat;
    int64_t  l2c_addr64;
    l2c_addr64  = rumboot_virt_to_phys(CAST_ADDR(addr));
    l2c_addr32  = (uint32_t)((l2c_addr64 >> L2C_TAG_ADDR_SHIFT)
                    & l2c_addr_masks[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]);
    L2C_WRITE(L2C_L2ARRACCADR, l2c_addr32);
    for (way = 0; way < 4; way++)
    {
        result = 0;
        l2c_stat = 0;
        L2C_WRITE(L2C_L2ARRACCCTL, l2c_read_req[way]);
        for(tries = L2C_REQUEST_TRIES;
            (L2C_READ(L2C_L2ARRACCCTL) & L2C_L2ARRACCCTL_REQUEST_mask)
                    && tries;
            tries--) {;} /* No body for this 'for' */
        result = L2C_READ(L2C_L2ARRACCDO0);
        l2c_stat = L2C_TAG_XTR_ST(result);
        if(tries && l2c_stat)
        {
            *way_out = way;
            lru_addr->u64 = ((uint64_t)(result & L2C_TAG_ADDR_MASK))
                            << L2C_TAG_ENTRY_SHIFT_LO;
            return l2c_stat;
        } else if(!tries)
                rumboot_printf(
                    "L2C tag array request timeout! "
                    "(way%d, tag=0x%X)\n",
                    way, result);
    }
    return -1;
}


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
                               + ((index < start_test_data_index)? ((index + 1) << 2 ) : 0))
                               ^ ((index >= 12) ? (1 << 8) : 0)))
            & l2c_addr_masks[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]);
}
static inline uint32_t get_addr_by_some_more_sophisticated_rule(uint32_t index, uint32_t way)
{
    return (SDRAM_BASE + get_addr_by_some_sophisticated_rule(index) + way * l2c_cache_sizes[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]/L2C_COUNT_WAYS);
}
uint32_t l2arracc_lru_check(uint32_t base)
{
    uint32_t result = 0;
    char test_name[] = "l2arracc_lru_check";
    rumboot_printf("%s started\n", test_name);
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
                rumboot_printf("3rd stage: index =%0d, i = %0d, j = %0d, k = %0d\n", i,j,k);
                dcbt(0, get_addr_by_some_more_sophisticated_rule (index,i));
                //SDRAM_BASE + get_addr_by_some_sophisticated_rule(index) + i * l2c_cache_sizes[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]/L2C_COUNT_WAYS);
                dcbt(0, get_addr_by_some_more_sophisticated_rule (index,j));
                //SDRAM_BASE + get_addr_by_some_sophisticated_rule(index) + j * l2c_cache_sizes[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]/L2C_COUNT_WAYS);
                dcbt(0, get_addr_by_some_more_sophisticated_rule (index,k));
//                SDRAM_BASE + get_addr_by_some_sophisticated_rule(index) + k * l2c_cache_sizes[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]/L2C_COUNT_WAYS);
                index++;
            }
    isync();
    msync();
    rumboot_memfill8(lru_values_array,sizeof(lru_values_array),initial_value,0);
    for (uint32_t i = 0; i < LRU_VALUES_COUNT; ++i)
    {
        for(uint32_t j = 0; j < L2C_COUNT_WAYS; ++j)
        {
            addr = get_addr_by_some_more_sophisticated_rule (i,j);
//            SDRAM_BASE + get_addr_by_some_sophisticated_rule(i) + j * l2c_cache_sizes[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]/L2C_COUNT_WAYS;
            array_of_addrs[j] = addr;
        }
        /*4th stage: checking that everything is still cached*/
        result |= check_caches((uint32_t (**)())array_of_addrs, true,PSEUDO_CT_DECODING_IS_L2_mask,"lru_value_based_lru_check");
        /*5th stage: checking that every set of cached areas has a unique LRU value*/
//        result |= check_unique_lru_value()
        if(l2c_arracc_lru_info_read_by_way(DCR_L2C_BASE,
                rumboot_virt_to_phys((void *)addr) >> 32,
                rumboot_virt_to_phys((void *)addr) & 0xFFFFFFFF,
                0,&lru_info))
        {
            lru_value = ((lru_info & L2C_L2ARRACCDO0_LRU_LRU_mask) >> L2C_L2ARRACCDO0_LRU_LRU_i);
            if(lru_values_array[lru_value] == initial_value)
                lru_values_array[lru_value] = lru_value;
            else
            {
                rumboot_printf("ERROR!!! LRU value == 0x%x already occurred for addr == 0x%x\n",lru_value,addr);
                result |= 1;
            }
        }
        else
        {
            rumboot_printf("ERROR!!! I did not find any entry that corresponds with this addr == 0x%x\n",addr);
            result |= 1;
        }
    }

    rumboot_printf("Just for information: valid values of LRU:\n");
    for (uint32_t i = 0; i < LRU_VALUES_COUNT; ++i)
    {
        if(lru_values_array[i] != initial_value)
        rumboot_printf("0x%x\n", lru_values_array[i]);
    }

    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}

uint32_t main(void)
{
    uint32_t     status  = TEST_OK;
//                 revid   = 0,
//                 idx     = 0;
//    uint32_t    addr;

    rumboot_printf("Init EMI...\n");
    emi_init(DCR_EM2_EMI_BASE);
//    revid = L2C_READ(L2C_L2REVID);
    g_cnfg0 = L2C_READ(L2C_L2CNFG0);
    g_cnfg1 = L2C_READ(L2C_L2CNFG1);
//    /*initializing memories under the specified cache lines*/
//    for(uint32_t i = 0; i < 4; ++i)
//    {
//        addr = SSRAM_BASE + (i * l2c_cache_sizes[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]/L2C_COUNT_WAYS);
//        rumboot_memfill32(
//                SSRAM_BASE + (i * l2c_cache_sizes[g_cnfg0 & L2C_L2CNFG0_L2SIZE_mask]/L2C_COUNT_WAYS),
//                L2C_LINE_SIZE,
//                addr, 0);
//    }
    status |= l2arracc_lru_check(DCR_L2C_BASE);
//    for(idx = 0; idx < (128 / sizeof(uint32_t)); idx++)
//        iowrite32(0, CHECK_ADDR(idx * sizeof(uint32_t)));
//    mem_write32(0x5555AAAA, 0);
//    rumboot_printf("Init done.\n");
//    rumboot_printf("Write TLB entries...\n");
    write_tlb_entries(em_tlb_entry_cache_on, 1);
//    tlbsync();
    msync();
    isync();
    status |= lru_value_based_lru_check(DCR_L2C_BASE);
//    rumboot_printf("Start checking L2C at DCR base 0x%X (ver:%d, rev:%d)...\n",
//            DCR_L2C_BASE, revid >> 8 & 0x0FFF, GET_BYTE(revid,0));
//    rumboot_printf("L2C_L2CNFG0=0x%X, L2C_L2CNFG1=0x%X,%s\n", g_cnfg0, g_cnfg1);
//    rumboot_printf("Cache size is %dKB\n",
//            (l2c_cache_sizes[g_cnfg0 & 0x03]) >> 10);
//
//    status |= 0;

    rumboot_printf("TEST %s!\n", !status ? "OK" : "ERROR");

    return status;
}





