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


#define COUNT_AREAS 5

/*                                          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
#define TLB_ENTRY_CACHE_L2_1_L1I_1_L1D_1    MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
static const tlb_entry em_tlb_entry_cache_on = {TLB_ENTRY_CACHE_L2_1_L1I_1_L1D_1};

BEGIN_ENUM(PSEUDO_CT_DECODING)
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L1I_i,         0    )
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L1I_n,         1    )
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L1I_mask,     FIELD_MASK32( PSEUDO_CT_DECODING_IS_L1I_i, PSEUDO_CT_DECODING_IS_L1I_n ) )

    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L2_i,         0    )
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L2_n,         1    )
    DECLARE_ENUM_VAL(   PSEUDO_CT_DECODING_IS_L2_mask,     FIELD_MASK32( PSEUDO_CT_DECODING_IS_L2_i, PSEUDO_CT_DECODING_IS_L2_n ) )
END_ENUM(PSEUDO_CT_DECODING)

bool get_way_by_addr(uint32_t pseudo_CT, void* addr, int32_t* cache_way)
{
    uint64_t    phys_addr = 0;
    uint32_t    reg_xCDBTRH;
    uint32_t    reg_xCDBTRL;
    bool        tag_valid;
    uint32_t    CT = pseudo_CT & PSEUDO_CT_DECODING_IS_L2_mask;
    bool        is_l1i = pseudo_CT & PSEUDO_CT_DECODING_IS_L1I_mask;

    phys_addr = (uint64_t)rumboot_virt_to_phys(addr);
    if(CT == 2)
    {
        return l2c_arracc_get_way_by_address(DCR_L2C_BASE,
                    (uint32_t)((phys_addr >> 32) & 0xffffffff),
                    (uint32_t)((phys_addr      ) & 0xffffffff),
                    cache_way
                    );
    } else if(CT == 0)
    {
        *cache_way = -1;
        do{
            ( int32_t )( *cache_way )++;
            msync();
            if(is_l1i)
                icread((uint32_t*)(((uint32_t)addr & (XCREAD_EA_L1I_INDEX_mask | XCREAD_EA_WORD_ADDR_mask))
                        | (((*cache_way) << XCREAD_EA_L1I_WAY_i) & XCREAD_EA_L1I_WAY_mask)));
            else
                dcread(((uint32_t*)(((uint32_t)addr & (XCREAD_EA_L1I_INDEX_mask | XCREAD_EA_WORD_ADDR_mask))
                        | (((*cache_way) << XCREAD_EA_L1I_WAY_i) & XCREAD_EA_L1I_WAY_mask))));
            isync();
            reg_xCDBTRL = spr_read(SPR_DCDBTRL);
            reg_xCDBTRH = spr_read(SPR_DCDBTRH);
            tag_valid = (reg_xCDBTRH & XCDBTRH_VALID_mask);
            rumboot_printf("\nget_way_by_addr, CT == %d, %s\ncache_way == %d\naddr == 0x%x\nicread (0x%x)\nxCDBTRH == 0x%x\nxCDBTRL == 0x%x\nvalid == %d\n",
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
/**********************************************************/
/*Here we have all testing functions for all the cache instructions*/
uint32_t test_icbi()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_icbt()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dcba()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dcbf()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dcbi()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dcbt()
{
    uint32_t    result = 0;
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
uint32_t test_ici()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dci()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_icread()
{
    uint32_t    result = 0;
    return result;
}
uint32_t test_dcread()
{
    uint32_t    result = 0;
    return result;
}
uint32_t (*test_cache_functions[])() = {
        test_icbi,
        test_icbt,
        test_dcba,
        test_dcbf,
        test_dcbi,
        test_dcbt,
        test_dcbtst,
        test_dcbz,
        test_icbtls,
        test_icblc,
        test_dcbtls,
        test_dcbtstls,
        test_dcblc,
        test_ici,
        test_dci,
        test_icread,
        test_dcread
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
    uint32_t    (*test_function_buf[COUNT_AREAS])();
    uint32_t    function_size = ((uint32_t)main) - ((uint32_t)test_function);
    uint32_t    function_area_size = ((function_size) & 0xffffff80) + 0x180;

    emi_init(DCR_EM2_EMI_BASE);
    rumboot_memfill8_modelling((void*)SRAM0_BASE,0x1000,0,0);

    write_tlb_entries(&em_tlb_entry_cache_on, 1);
    msync();
    isync();
    test_event_send_test_id("test_oi10_cpu_021_cache_op");


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

    for(uint32_t i = 0; i < sizeof(test_cache_functions)/sizeof(*test_cache_functions); ++i)
        result |= test_cache_functions[i]();

    return (result >> 24) | (result >> 16) | (result >> 8) | result;
}
