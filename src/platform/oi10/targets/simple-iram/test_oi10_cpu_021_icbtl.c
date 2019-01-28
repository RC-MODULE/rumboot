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


/*                            MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
#define TLB_ENTRY_CACHE_ON    MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
static const tlb_entry em_tlb_entry_cache_on = {TLB_ENTRY_CACHE_ON};

#define COUNT_AREAS 5

uint32_t compare_cached_functions(uint32_t* prev, uint32_t* curr){
    return 0;
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
    uint32_t    cached_functions_prev[4] = {0xffffffff,0xffffffff,0xffffffff,0xffffffff};
    uint64_t    phys_addr = 0;
    int32_t     cache_way = -1;
    uint32_t    L2C_replacement_count = 0;
    int32_t     L2C_replacement_way = -1;
    uint32_t    ct_field = 2;

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

    rumboot_printf("Initial function calling and caching without locking\n");
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
            phys_addr = (uint64_t)rumboot_virt_to_phys(test_function_buf[i]);
            cached_functions_prev[cache_way] = cached_functions[cache_way];
            if(l2c_arracc_get_way_by_address(DCR_L2C_BASE,
                    (uint32_t)((phys_addr >> 32) & 0xffffffff),
                    (uint32_t)((phys_addr      ) & 0xffffffff),
                    &cache_way
                    ))
            {
                TEST_ASSERT(((cache_way >= 0) && (cache_way < L2C_COUNT_WAYS)),"Cache way is not in allowed range, although the function said it should be OK");
                cached_functions[cache_way] = i;
                rumboot_printf("function[%d] way == 0x%x\n", cached_functions[cache_way], cache_way);
            }
            else
            {
                ++L2C_replacement_count;
                L2C_replacement_way = i;
                rumboot_printf("function[%d] at 0x%x is not in L2C\n", L2C_replacement_way, (uint32_t)test_function_buf[L2C_replacement_way]);
            }
        }
        TEST_ASSERT((L2C_replacement_count == 1), "Other than one count of functions was replaced.");
        TEST_ASSERT((L2C_replacement_way == (j%4)), "Unexpected number of replaced function.");

        compare_cached_functions(cached_functions_prev, cached_functions);
        ici(0);
        dci(2);
        isync();

    //    cached_functions_prev[]
        rumboot_printf("Let's lock one more function in cache.\n");
        for(uint32_t i = 0; i < COUNT_AREAS; ++i)
        {
            ctr_value = test_function_buf[i]();
            if(i <= j)
                icbtls(ct_field,( void* )ctr_value);
        }
    }

    rumboot_printf("Secondary function calling without locking\n");
    for(int32_t j = L2C_COUNT_WAYS - 1; j >= 0; --j)
    {
        ici(0);
        dci(2);
        isync();
        for(uint32_t i = 0; i < L2C_COUNT_WAYS; ++i)
        {
            ctr_value = test_function_buf[i]();
            rumboot_printf("function[%d] ctr == 0x%x\n", i, ctr_value);
            if(i < L2C_COUNT_WAYS)
                icbtls(ct_field,( void* )ctr_value);
        }
        rumboot_printf("Let's check what functions are in cache.\n");
        L2C_replacement_count = 0;
        for(uint32_t i = 0; i < COUNT_AREAS; ++i)
        {
            phys_addr = (uint64_t)rumboot_virt_to_phys(test_function_buf[i]);
            cached_functions_prev[cache_way] = cached_functions[cache_way];
            if(l2c_arracc_get_way_by_address(DCR_L2C_BASE,
                    (uint32_t)((phys_addr >> 32) & 0xffffffff),
                    (uint32_t)((phys_addr      ) & 0xffffffff),
                    &cache_way
                    ))
            {
                TEST_ASSERT(((cache_way >= 0) && (cache_way < L2C_COUNT_WAYS)),"Cache way is not in allowed range, although the function said it should be OK");
                cached_functions[cache_way] = i;
                rumboot_printf("function[%d] way == 0x%x\n", cached_functions[cache_way], cache_way);
            }
            else
            {
                ++L2C_replacement_count;
                L2C_replacement_way = i;
                rumboot_printf("function[%d] at 0x%x is not in L2C\n", L2C_replacement_way, (uint32_t)test_function_buf[L2C_replacement_way]);
            }
        }
        TEST_ASSERT((L2C_replacement_count == 1), "Other than one count of functions was replaced.");
        rumboot_printf("L2C_replacement_way == %d, j == %d", L2C_replacement_way, j);
//        TEST_ASSERT((L2C_replacement_way == (j%4)), "Unexpected way of replaced function.");

        icblc(ct_field, (test_function_buf[cached_functions[j]]));

        ctr_value = test_function_buf[L2C_replacement_way]();
        rumboot_printf("function[%d] ctr == 0x%x\n", L2C_replacement_way, ctr_value);

        phys_addr = (uint64_t)rumboot_virt_to_phys(test_function_buf[L2C_replacement_way]);
        if(l2c_arracc_get_way_by_address(DCR_L2C_BASE,
                (uint32_t)((phys_addr >> 32) & 0xffffffff),
                (uint32_t)((phys_addr      ) & 0xffffffff),
                &cache_way
                ))
        {
            TEST_ASSERT(((cache_way >= 0) && (cache_way < L2C_COUNT_WAYS)),"Cache way is not in allowed range, although the function said it should be OK");
            rumboot_printf("function[%d] way == 0x%x\n", cached_functions[cache_way], cache_way);
        }
        else
        {
            rumboot_printf("Didn't find the function[%d] at 0x%x on the expected cache way == %d\n", L2C_replacement_way, (uint32_t)test_function_buf[L2C_replacement_way], j);
            ++result;
        }
//        compare_cached_functions(cached_functions_prev, cached_functions);
    }
    return result;
}
