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


/*                            MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
#define TLB_ENTRY_CACHE_ON    MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
static const tlb_entry em_tlb_entry_cache_on = {TLB_ENTRY_CACHE_ON};

#define COUNT_AREAS 5

bool get_way_by_addr(uint32_t CT, void* addr, int32_t* cache_way)
{
    uint64_t    phys_addr = 0;
    uint32_t    reg_ICDBTRH;
    uint32_t    reg_ICDBTRL;
    bool        tag_valid;

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
            icread((uint32_t*)(((uint32_t)addr & (ICREAD_EA_L1I_INDEX_mask | ICREAD_EA_WORD_ADDR_mask))
                    | (((*cache_way) << ICREAD_EA_L1I_WAY_i) & ICREAD_EA_L1I_WAY_mask)));
            isync();
            reg_ICDBTRL = spr_read(SPR_ICDBTRL);
            reg_ICDBTRH = spr_read(SPR_ICDBTRH);
            tag_valid = (reg_ICDBTRH & ICDBTRH_VALID_mask);
            rumboot_printf("cache_way == %d\naddr == 0x%x\nicread (0x%x)\nICDBTRH == 0x%x\nICDBTRL == 0x%x\nvalid == %d\n",
                    *cache_way,
                    (uint32_t)addr,
                    (uint32_t*)(((uint32_t)addr & (ICREAD_EA_L1I_INDEX_mask | ICREAD_EA_WORD_ADDR_mask))
                            | (((*cache_way) << ICREAD_EA_L1I_WAY_i) & ICREAD_EA_L1I_WAY_mask)),
                    reg_ICDBTRH,
                    reg_ICDBTRL,
                    tag_valid);
        }while( ( (*cache_way) < (L2C_COUNT_WAYS - 1))
                 && ( !( (tag_valid)
                     &&( ( ( (reg_ICDBTRH & ICDBTRH_TAG_EXT_ADDR_mask) >> ICDBTRH_TAG_EXT_ADDR_i) == (uint32_t)((phys_addr >> 32) & 0xffffffff) )
                       &&( (reg_ICDBTRH & ICDBTRH_TAG_ADDR_mask) == ( phys_addr & ICDBTRH_TAG_ADDR_mask ) )
                       )
                     ) )
              );
        if( tag_valid && ( ( ( (reg_ICDBTRH & ICDBTRH_TAG_EXT_ADDR_mask) >> ICDBTRH_TAG_EXT_ADDR_i) == (uint32_t)((phys_addr >> 32) & 0xffffffff) )
                       &&( (reg_ICDBTRH & ICDBTRH_TAG_ADDR_mask) == ( phys_addr & ICDBTRH_TAG_ADDR_mask ) )
                       ) )
            return true;
    }
    return false;
}

uint32_t get_locks(uint32_t CT, void* addr, uint32_t cache_way)
{
    uint32_t result = -1;
    uint64_t phys_addr = rumboot_virt_to_phys(addr);
    uint32_t L2C_LRU_info;
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
        icread((uint32_t*)((uint32_t)addr & (ICREAD_EA_L1I_INDEX_mask | ICREAD_EA_WORD_ADDR_mask)));
        isync();
        result = (spr_read(SPR_ICDBTRL) & ICDBTRL_LOCK_mask) >> ICDBTRL_LOCK_i;
    }
    return result;
}

//uint32_t overlocking_detected = 0;
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
    uint32_t    (*test_function_buf[2 * COUNT_AREAS])();
    uint32_t    function_size = ((uint32_t)main) - ((uint32_t)test_function);
    uint32_t    function_area_size = ((function_size) & 0xffffff80) + 0x180;
    uint32_t    ctr_value = 0;
    /*cached_function[way] = number in test_function_buf*/
    uint32_t    cached_functions[4] = {0xffffffff,0xffffffff,0xffffffff,0xffffffff};
    int32_t     cache_way = -1;
    uint32_t    L2C_replacement_count = 0;
    int32_t     not_in_cache_function_number = -1;
    uint32_t    ct_field = 0;
    uint32_t    locks;

    emi_init(DCR_EM2_EMI_BASE);
    rumboot_memfill8_modelling((void*)SRAM0_BASE,0x1000,0,0);

    for(uint32_t i = 0; i < 2 * COUNT_AREAS; ++i)
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

    for(ct_field = 0; ct_field < 3; ct_field += 2)
    {
    rumboot_printf("Initial function calling and caching without locking, CT == %d.\n", ct_field);
    for(uint32_t i = 0; i < COUNT_AREAS; ++i)
    {
        ctr_value = test_function_buf[i]();
        rumboot_printf("function[%d] ctr == 0x%x\n", i, ctr_value);
    }
    for(uint32_t j = 0; j < L2C_COUNT_WAYS + 1; ++j)
    {
        L2C_replacement_count = 0;
        rumboot_printf("Let's check what functions are in cache.\n");
        for(uint32_t i = 0; i < COUNT_AREAS; ++i)
        {
            if(get_way_by_addr(ct_field, test_function_buf[i], &cache_way))
            {
                TEST_ASSERT(((cache_way >= 0) && (cache_way < L2C_COUNT_WAYS)),"Cache way is not in allowed range, although the function said it should be OK");
                cached_functions[cache_way] = i;
                rumboot_printf("function[%d] way == 0x%x\n", cached_functions[cache_way], cache_way);
            }
            else
            {
                ++L2C_replacement_count;
                not_in_cache_function_number = i;
                rumboot_printf("function[%d] at 0x%x is not in Cache\n", not_in_cache_function_number, (uint32_t)test_function_buf[not_in_cache_function_number]);
            }
        }
        TEST_ASSERT((L2C_replacement_count == 1), "Other than one count of functions was replaced.");
        if(j < L2C_COUNT_WAYS)
            TEST_ASSERT((not_in_cache_function_number == (j%4)), "Unexpected number of replaced function.");

        ici(0);
        dci(2);
        isync();

        rumboot_printf("Let's lock one more function in cache.\n");
        for(uint32_t i = 0; i < COUNT_AREAS; ++i)
        {
            ctr_value = test_function_buf[i]();
            if(i <= j)
                icbtls(ct_field,( void* )ctr_value);
        }
        cache_way = 0;
        rumboot_printf("after icbtls: locks == 0x%x\n", get_locks(ct_field,test_function_buf[cached_functions[cache_way]],cache_way));
    }

    rumboot_printf("Secondary function calling: with locking\n");
    for(int32_t j = L2C_COUNT_WAYS - 1; j >= 0; --j)
    {
        ici(0);
        dci(2);
        isync();
        for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
        {
            ctr_value = test_function_buf[i]();
            if(i < L2C_COUNT_WAYS)
                icbtls(ct_field,( void* )ctr_value);
        }
        isync();
        rumboot_printf("\n iteration %d: Cache prefilled.\n", j);
        L2C_replacement_count = 0;
        for(uint32_t i = 0; i < COUNT_AREAS; ++i)
        {
            if(get_way_by_addr(ct_field, test_function_buf[i], &cache_way))
            {
                TEST_ASSERT(((cache_way >= 0) && (cache_way < L2C_COUNT_WAYS)),"Cache way is not in allowed range, although the function said it should be OK");
                cached_functions[cache_way] = i;
            }
            else
            {
                ++L2C_replacement_count;
                not_in_cache_function_number = i;
                rumboot_printf("function[%d] at 0x%x is not in L2C\n", not_in_cache_function_number, (uint32_t)test_function_buf[not_in_cache_function_number]);
            }
        }
        TEST_ASSERT((L2C_replacement_count == 1), "Other than one count of functions was replaced.");

        cache_way = 0;
        rumboot_printf("before icblc: locks == 0x%x\n", get_locks(ct_field,test_function_buf[cached_functions[cache_way]],cache_way));
        icblc(ct_field, (test_function_buf[cached_functions[j]]));
        isync();
        cache_way = (j+1)%4;
        rumboot_printf("after icblc: locks == 0x%x\n", get_locks(ct_field,test_function_buf[cached_functions[cache_way]],cache_way));

        ctr_value = test_function_buf[not_in_cache_function_number]();

        if(get_way_by_addr(ct_field, test_function_buf[not_in_cache_function_number], &cache_way))
        {
            TEST_ASSERT(((cache_way >= 0) && (cache_way < L2C_COUNT_WAYS)),"Cache way is not in allowed range, although the function said it should be OK");
            cached_functions[cache_way] = not_in_cache_function_number;
            rumboot_printf("function[%d] was cached, way == %d\n", not_in_cache_function_number, cache_way);
            rumboot_printf("after recaching: locks == 0x%x\n", get_locks(ct_field,test_function_buf[cached_functions[cache_way]],cache_way));
//            TEST_ASSERT((cache_way == j), "Incorrect way was replaced");
            if(!(cache_way == j))
            {
                rumboot_printf("ERROR! Cache way lock does not work! Expected way %d to be replaced, but actually replaced way %d.\n",j,cache_way);
                result |= !(cache_way == j);
            }
        }
        else
        {
            rumboot_printf("Didn't find the function[%d] at 0x%x on the expected cache way == %d\n", not_in_cache_function_number, (uint32_t)test_function_buf[not_in_cache_function_number], j);
            ++result;
        }
    }
        for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
        {
            icblc(ct_field, (test_function_buf[cached_functions[i]]));
        }
        cache_way = 0;
        locks = get_locks(ct_field,test_function_buf[cached_functions[cache_way]],cache_way);
        rumboot_printf("after all: locks == 0x%x\n", locks);
        TEST_ASSERT((locks == 0), "After icblc on all ways all locks must be zeros!!!");
        ici(0);
        dci(2);
        isync();
    }
    return result;
}
