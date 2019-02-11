#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <rumboot/rumboot.h>
#include <platform/test_assert.h>
#include <platform/arch/ppc/ppc_476fp_l1c_fields.h>
#include <platform/test_event_c.h>


uint32_t const EVENT_TEST_DCI       = TEST_EVENT_CODE_MIN + 0;
uint32_t const EVENT_TEST_ICBI      = TEST_EVENT_CODE_MIN + 1;
uint32_t const EVENT_TEST_ICBT      = TEST_EVENT_CODE_MIN + 2;
uint32_t const EVENT_TEST_DCBT      = TEST_EVENT_CODE_MIN + 3;
uint32_t const EVENT_TEST_DCBZ      = TEST_EVENT_CODE_MIN + 4;
uint32_t const EVENT_TEST_DCBI      = TEST_EVENT_CODE_MIN + 5;
uint32_t const EVENT_TEST_DCBF      = TEST_EVENT_CODE_MIN + 6;
uint32_t const EVENT_TEST_DCBST     = TEST_EVENT_CODE_MIN + 7;

#define COUNT_AREAS 5

/*                                          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
#define TLB_ENTRY_CACHE_L2_1_L1I_1_L1D_1    MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
static const tlb_entry em_tlb_entry_cache_on = {TLB_ENTRY_CACHE_L2_1_L1I_1_L1D_1};

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
    uint32_t    reg_xCDBTRL;
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
        rumboot_printf("l2c_arracc_get_valid_way_by_address(0x%x)\n",addr);
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
                reg_xCDBTRL = spr_read(SPR_ICDBTRL);
                reg_xCDBTRH = spr_read(SPR_ICDBTRH);
            }
            else
            {
                dcread(((uint32_t*)(((uint32_t)addr & (XCREAD_EA_L1I_INDEX_mask | XCREAD_EA_WORD_ADDR_mask))
                        | (((*cache_way) << XCREAD_EA_L1I_WAY_i) & XCREAD_EA_L1I_WAY_mask))));
                isync();
                reg_xCDBTRL = spr_read(SPR_DCDBTRL);
                reg_xCDBTRH = spr_read(SPR_DCDBTRH);
            }
            tag_valid = (reg_xCDBTRH & XCDBTRH_VALID_mask);
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

uint32_t get_locks(uint32_t pseudo_CT, void* addr, uint32_t cache_way)
{
    uint32_t    result = -1;
    uint64_t    phys_addr = rumboot_virt_to_phys(addr);
    uint32_t    L2C_LRU_info;
    uint32_t    CT = pseudo_CT & PSEUDO_CT_DECODING_IS_L2_mask;
    bool        is_l1i = pseudo_CT & PSEUDO_CT_DECODING_IS_L1I_mask;

    if(CT == 2)
    {
        if(l2c_arracc_lru_info_read_by_way(DCR_L2C_BASE,
                    (uint32_t)((phys_addr >> 32) & 0xffffffff),
                    (uint32_t)((phys_addr      ) & 0xffffffff),
                    cache_way,
                    &L2C_LRU_info
                    ))
        {
            result = ( L2C_LRU_info & L2C_L2ARRACCDO0_LRU_LOCK_BITS_mask ) >> L2C_L2ARRACCDO0_LRU_LOCK_BITS_i;
        }
    } else if(CT == 0)
    {
        msync();
        if(is_l1i)
            icread((uint32_t*)((uint32_t)addr & (XCREAD_EA_L1I_INDEX_mask | XCREAD_EA_WORD_ADDR_mask)));
        else
            dcread((uint32_t*)((uint32_t)addr & (XCREAD_EA_L1I_INDEX_mask | XCREAD_EA_WORD_ADDR_mask)));
        isync();
        result = (spr_read(SPR_DCDBTRL) & DCDBTRL_LOCK_mask) >> DCDBTRL_LOCK_i;
    }
    return result;
}
/*Global data are not good, I know, but now I have no time and wish to bind test_functions to any interface.*/
uint32_t    (*test_function_buf[COUNT_AREAS])();


uint32_t check_caches( uint32_t    (*array_of_addr[])(), bool expected_in_cache, uint32_t pseudo_CT, char* test_name){
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

/**********************************************************/
/*Here we have all testing functions for all the cache instructions*/

/*  test_ici - let's call several functions and check that they are in the cache.
 * Then let's invalidate the cache and check that all these functions are not more in the cache for L1 and nothing is done for L2*/
uint32_t test_ici()
{
    uint32_t    result = 0;
    char        test_name[] = "test_ici";
    rumboot_printf("%s start\n",test_name);
    for(uint32_t CT = 0; CT < 3; CT += 2)
    {
        rumboot_printf("caching for CT == %d\n", CT);
        for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
        {
            test_function_buf[i]();
        }
        rumboot_printf("Checking, that everything is cached.\n", CT);
        result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L1I_mask | CT ,test_name);
        msync();
        isync();
        if(CT & PSEUDO_CT_DECODING_IS_L2_mask)
            ici(2);
        if(!(CT & PSEUDO_CT_DECODING_IS_L2_mask))
            ici(0);
        msync();
        isync();

        result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L1I_mask | PSEUDO_CT_DECODING_IS_L2_mask,test_name);
        result |= check_caches(test_function_buf,(bool)CT,PSEUDO_CT_DECODING_IS_L1I_mask ,test_name);
    }
    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}
uint32_t test_dci()
{
    uint32_t    result = 0;
    uint32_t    first_word;
    char        test_name[] = "test_dci";
    rumboot_printf("%s start\n",test_name);
    for(uint32_t CT = 0; CT < 3; CT += 2)
    {
        rumboot_printf("caching for CT == %d\nnext are coming first words from data areas, all equal.\n", CT);
        for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
        {
            first_word = ioread32((uint32_t)test_function_buf[i]);
            rumboot_puthex(first_word);
        }
        msync();
        rumboot_printf("Checking, that everything is cached., CT == %d\n", CT);
        result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
        result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L1D_val,test_name);
        test_event(EVENT_TEST_DCI);
        msync();
        if(CT & PSEUDO_CT_DECODING_IS_L2_mask)
            dci(2);
        else
            dci(0);
        msync();
        isync();
        /* in the FPUG table 10-3 Cache operations we have information that dci(2) invalidates both
         * L1D and L2C, however in section 5.5.12 it is specified that
         * "it is only permissible to perform a dci instruction to the L1 and L2, in that order, followed by an isync."
         * */
        result |= check_caches(test_function_buf,!((bool)CT),PSEUDO_CT_DECODING_IS_L2_mask,test_name);
        result |= check_caches(test_function_buf, ((bool)CT),PSEUDO_CT_DECODING_IS_L1D_val,test_name);
    }
    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}
uint32_t test_icbi()
{
    uint32_t    result = 0;
    char        test_name[] = "test_icbi";
    rumboot_printf("%s start\n",test_name);
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        test_function_buf[i]();
    }
    rumboot_printf("Checking, that everything is cached.\n");
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L1I_mask,test_name);

    test_event(EVENT_TEST_ICBI);
    msync();
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        icbi(test_function_buf[i]);
        isync();
    }

    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf,false,PSEUDO_CT_DECODING_IS_L1I_mask,test_name);
    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}
uint32_t test_dcba()
{
    uint32_t    result = 0;
    char        test_name[] = "test_dcba";
    rumboot_printf("%s start\n",test_name);

    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        test_function_buf[i]();
        dcbt(0,test_function_buf[i]);
    }
    rumboot_printf("Checking, that dcba does nothing for cached functions.\n");

    result |= check_caches(test_function_buf, true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf, true,PSEUDO_CT_DECODING_IS_L1D_val,test_name);
    result |= check_caches(test_function_buf, true,PSEUDO_CT_DECODING_IS_L1I_mask,test_name);
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        dcba(test_function_buf[i]);
    }
    msync();
    isync();

    result |= check_caches(test_function_buf, true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf, true,PSEUDO_CT_DECODING_IS_L1D_val,test_name);
    result |= check_caches(test_function_buf, true,PSEUDO_CT_DECODING_IS_L1I_mask,test_name);
    msync();
    dci(0);
    ici(0);
    dci(2);
    msync();
    isync();
    rumboot_printf("Checking, that dcba does nothing for not yet cached functions.\n");

    result |= check_caches(test_function_buf, false,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf, false,PSEUDO_CT_DECODING_IS_L1D_val,test_name);
    result |= check_caches(test_function_buf, false,PSEUDO_CT_DECODING_IS_L1I_mask,test_name);
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        dcba(test_function_buf[i]);
    }
    msync();
    isync();

    result |= check_caches(test_function_buf, false,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf, false,PSEUDO_CT_DECODING_IS_L1D_val,test_name);
    result |= check_caches(test_function_buf, false,PSEUDO_CT_DECODING_IS_L1I_mask,test_name);
    msync();
    dci(0);
    dci(2);
    msync();
    isync();

    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}
uint32_t test_icbt()
{
    uint32_t    result = 0;
    char        test_name[] = "test_icbt";
    rumboot_printf("%s start\n",test_name);
    msync();
    ici(0);
    dci(2);
    msync();
    isync();
    for(uint32_t CT = 0; CT < 3; CT += 2)
    {
        test_event(EVENT_TEST_ICBT);
        msync();
        for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
            icbt(CT,test_function_buf[i]);
        isync();
        msync();
        result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
        result |= check_caches(test_function_buf,!((bool)CT),PSEUDO_CT_DECODING_IS_L1I_mask,test_name);
        msync();
        ici(0);
        dci(2);
        msync();
        isync();
    }
    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}
uint32_t test_dcbi_dcbf(uint32_t is_dcbi, char* test_name)
{
    uint32_t    result = 0;
    msync();
    dci(0);
    dci(2);
    msync();
    isync();
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        ioread32((uint32_t)test_function_buf[i]);
        iowrite32(0,(uint32_t)test_function_buf[i]);
    }
    msync();
    rumboot_printf("Checking, that everything is cached.\n");
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L1D_val,test_name);

    test_event((is_dcbi)?EVENT_TEST_DCBI : EVENT_TEST_DCBF);
    msync();
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        if(is_dcbi)
            dcbi(test_function_buf[i]);
        else
            dcbf(test_function_buf[i]);
    }
    msync();

    result |= check_caches(test_function_buf,false,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf,false,PSEUDO_CT_DECODING_IS_L1D_val,test_name);
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        result |= !(ioread32((uint32_t)test_function_buf[i]));
    }

    test_event((is_dcbi)?EVENT_TEST_DCBI : EVENT_TEST_DCBF);
    msync();
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        if(is_dcbi)
            dcbi(test_function_buf[i]);
        else
            dcbf(test_function_buf[i]);
    }
    msync();

    result |= check_caches(test_function_buf,false,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf,false,PSEUDO_CT_DECODING_IS_L1D_val,test_name);
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        result |= !(ioread32((uint32_t)test_function_buf[i]));
    }
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        ioread32((uint32_t)test_function_buf[i]);
    }
    msync();
    rumboot_printf("Checking, that everything is cached.\n");
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L1D_val,test_name);

    test_event((is_dcbi)?EVENT_TEST_DCBI : EVENT_TEST_DCBF);
    msync();
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        if(is_dcbi)
            dcbi(test_function_buf[i]);
        else
            dcbf(test_function_buf[i]);
    }
    msync();
    msync();
    dci(0);
    dci(2);
    msync();
    isync();
    return result;
}
uint32_t test_dcbf()
{
    uint32_t    result = 0;
    char        test_name[] = "test_dcbf";
    rumboot_printf("%s start\n",test_name);
    result = test_dcbi_dcbf(0, test_name);
    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}
uint32_t test_dcbi()
{
    uint32_t    result = 0;
    char        test_name[] = "test_dcbi";
    rumboot_printf("%s start\n",test_name);
    result = test_dcbi_dcbf(1, test_name);
    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}
uint32_t test_dcbt()
{
    uint32_t    result = 0;
    char        test_name[] = "test_dcbt";
    rumboot_printf("%s start\n",test_name);
    msync();
    ici(0);
    dci(2);
    msync();
    isync();
    for(uint32_t CT = 0; CT < 3; CT += 2)
    {
        test_event(EVENT_TEST_DCBT);
        msync();
        for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
            dcbt(CT,test_function_buf[i]);
        isync();
        msync();
        result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
        result |= check_caches(test_function_buf,!((bool)CT),PSEUDO_CT_DECODING_IS_L1D_val,test_name);
        msync();
        dci(0);
        dci(2);
        msync();
        isync();
    }
    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}
uint32_t test_dcbtst()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dcbz()
{
    uint32_t    result = 0;
    char        test_name[] = "test_dcbz";
    rumboot_printf("%s start\n",test_name);
    msync();
    dci(0);
    dci(2);
    msync();
    isync();
    test_event(EVENT_TEST_DCBZ);
    msync();

    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        dcbz(test_function_buf[i]);
    }
    msync();

    for(uint32_t CT = 0; CT < 3; CT += 2)
    {
        for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
            for(uint32_t addr = 0; addr < L2C_LINE_SIZE; addr += 4)
                if(ioread32(((uint32_t)test_function_buf[i]) + addr))
                {
                    result |= ioread32(((uint32_t)test_function_buf[i]) + addr);
                    rumboot_printf("%s failed on address 0x%x, CT == %d.\n", test_name, (((uint32_t)test_function_buf[i]) + addr), CT);
                }
        dci(0);
        msync();
    }

    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}
uint32_t test_icbtls()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_icblc()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dcbtls()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dcbtstls()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dcblc()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dcbst()
{
    uint32_t    result = 0;
    uint32_t    data;
    char        test_name[] = "test_dcbst";
    rumboot_printf("%s start\n",test_name);
    msync();
    dci(0);
    dci(2);
    msync();
    isync();
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        data = ioread32((uint32_t)test_function_buf[i]);
    }
    msync();
    rumboot_printf("Checking, that everything is cached.\n");
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L1D_val,test_name);
    test_event(EVENT_TEST_DCBST);
    msync();
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        dcbst(test_function_buf[i]);
    }
    msync();
    rumboot_printf("Checking, that everything non-modified is cached.\n");
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L1D_val,test_name);
    msync();
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        data = ioread32((uint32_t)test_function_buf[i]);
        iowrite32(data,(uint32_t)test_function_buf[i]);
    }
    msync();
    test_event(EVENT_TEST_DCBST);
    msync();
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        dcbst(test_function_buf[i]);
    }
    msync();
    rumboot_printf("Checking, that everything modified is cached.\n");
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L2_mask,test_name);
    result |= check_caches(test_function_buf,true,PSEUDO_CT_DECODING_IS_L1D_val,test_name);
    msync();
    dci(0);
    dci(2);
    msync();
    isync();

    test_event(EVENT_TEST_DCBST);
    msync();
    for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
    {
        dcbst(test_function_buf[i]);
    }
    msync();
    rumboot_printf("%s finished: %s\n",test_name,(result)?"FAIL":"OK");
    return result;
}
/* test_icread and test_dcread separate functions are meaningless -
 * the corresponding instructions affect only L1* caches and were used quite often during the test
 */
uint32_t (*test_cache_functions[])() = {
//        test_ici,
//        test_dci,
//        test_icbi,
//        test_dcba,
//        test_icbt,
//        test_dcbt,
        test_dcbi,
//        test_dcbtst,
//        test_dcbz,
//        test_icbtls,
//        test_icblc,
//        test_dcbtls,
//        test_dcbtstls,
//        test_dcblc,
        test_dcbst,
        test_dcbf
};
/**********************************************************/

/*leave this function just before main function*/
uint32_t test_function()
{
    uint32_t function_address = 0;
    asm(
            "mfctr %0\n\t"
            : "=r"(function_address)
    );
    return function_address;
}

int main()
{
    uint32_t    result = 0;
    uint32_t    function_size = ((uint32_t)main) - ((uint32_t)test_function);
    uint32_t    function_area_size = ((function_size) & 0xffffff80) + 0x180;

    emi_init(DCR_EM2_EMI_BASE);
    rumboot_memfill8_modelling((void*)SRAM0_BASE,0x1000,0,0);


    for(uint32_t i = 0; i < COUNT_AREAS; ++i)
    {
        /*aligned size + 3 cache lines*/
        test_function_buf[i] = rumboot_malloc_from_named_heap_aligned("SSRAM", function_area_size, 64*1024);
        if((test_function_buf[i] == NULL)){
            rumboot_printf("Cannot allocate memory from SSRAM\n");
            return 1;
        }
        rumboot_memfill8(test_function_buf[i],function_area_size,0,0);
        memcpy(test_function_buf[i], test_function, function_size);
    }

    write_tlb_entries(&em_tlb_entry_cache_on, 1);
    msync();
    isync();
    test_event_send_test_id("test_oi10_cpu_021_cache_op");

    for(uint32_t i = 0; i < sizeof(test_cache_functions)/sizeof(*test_cache_functions); ++i)
        result |= test_cache_functions[i]();

    return ((result >> 24) | (result >> 16) | (result >> 8) | result);
}
