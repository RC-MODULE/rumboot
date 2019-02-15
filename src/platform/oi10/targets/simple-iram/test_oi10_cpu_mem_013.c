#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/regs/regs_plb6mcif2.h>
#include <platform/regs/regs_mclfir.h>
#include <platform/regs/regs_emi.h>


#define EVENT_ICU_LRU_MEM0_VALUE_00     (TEST_EVENT_CODE_MIN + 0)
#define EVENT_ICU_LRU_MEM0_VALUE_34     (TEST_EVENT_CODE_MIN + 1)
#define EVENT_ICU_LRU_MEM0_VALUE_1E     (TEST_EVENT_CODE_MIN + 2)
#define EVENT_ICU_LRU_MEM0_VALUE_07     (TEST_EVENT_CODE_MIN + 3)

#define EVENT_ICU_LRU_MEM1_VALUE_00     (TEST_EVENT_CODE_MIN + 4)
#define EVENT_ICU_LRU_MEM1_VALUE_34     (TEST_EVENT_CODE_MIN + 5)
#define EVENT_ICU_LRU_MEM1_VALUE_1E     (TEST_EVENT_CODE_MIN + 6)
#define EVENT_ICU_LRU_MEM1_VALUE_07     (TEST_EVENT_CODE_MIN + 7)



#define TLB_ENTRY_CACHE_VALID    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )



#define GET_ADDR_BY_TAG_INDEX_WORD(TAG,INDEX,WORD) \
        ( (EM0_CACHED_PAGE) | ((TAG) << 13) | ((INDEX) << 5) | ((WORD) << 2) )

#define GET_WINDOW(ERPN_TAG) \
        ( 0x8000 | (((ERPN_TAG) >> 19) << 2) | (((ERPN_TAG) >> 17) & 0x3))


////convert RA tag to EA tag
//static uint32_t get_EA_tag(uint32_t erpn_tag)
//{
//    uint32_t mem_window_number = GET_WINDOW(erpn_tag) & 0x7;
//    if(mem_window_number > 3)
//       return (erpn_tag & 0x7FFFF)-((0x40000000 * (mem_window_number - 4)) >> 13);
//    else
//       return (erpn_tag & 0x7FFFF)-((0x40000000 * mem_window_number) >> 13);
//}


const uint32_t way0 = 0;
const uint32_t way1 = (1 << 13);
const uint32_t way2 = (1 << 14);
const uint32_t way3 = ((1 << 13) | (1 << 14));

const uint32_t index19 = (1 << 12);

typedef void (*pWay_func) ();


static bool test_icu_lru_array()
{
    uint32_t addr;

    for (addr = SRAM0_BASE; addr != (SRAM0_BASE + 0x00100000); addr += /*0x00010000*/ 0x00100000)
    {
        ((pWay_func)(addr | way0))();
        test_event(EVENT_ICU_LRU_MEM0_VALUE_34);

        ((pWay_func)(addr | way1))();
        test_event(EVENT_ICU_LRU_MEM0_VALUE_1E);

        ((pWay_func)(addr | way2))();
        test_event(EVENT_ICU_LRU_MEM0_VALUE_07);

        ((pWay_func)(addr | way3))();
        test_event(EVENT_ICU_LRU_MEM0_VALUE_00);


        ((pWay_func)(addr | (index19 | way0)))();
        test_event(EVENT_ICU_LRU_MEM1_VALUE_34);

        ((pWay_func)(addr | (index19 | way1)))();
        test_event(EVENT_ICU_LRU_MEM1_VALUE_1E);

        ((pWay_func)(addr | (index19 | way2)))();
        test_event(EVENT_ICU_LRU_MEM1_VALUE_07);

        ((pWay_func)(addr | (index19 | way3)))();
        test_event(EVENT_ICU_LRU_MEM1_VALUE_00);
    }

//    ((pWay_func)(0x40000040 | way0))();
//    ((pWay_func)(0x40000080 | way0))();
//    ((pWay_func)(0x40000100 | way0))();
//    ((pWay_func)(0x40000200 | way0))();
//    ((pWay_func)(0x40000400 | way0))();
//    ((pWay_func)(0x40000800 | way0))();

    return true;
}


static void fill_mem(void* dst, const uint32_t val, size_t size)
{
    asm("stmw 8, 96(1)\n\t"
        "mr 8, %[val]\n\t"
        "mr 9, %[val]\n\t"
        "mr 10, %[val]\n\t"
        "mr 11, %[val]\n\t"
        "mr 12, %[val]\n\t"
        "mr 13, %[val]\n\t"
        "mr 14, %[val]\n\t"
        "mr 15, %[val]\n\t"
        "mr 16, %[val]\n\t"
        "mr 17, %[val]\n\t"
        "mr 18, %[val]\n\t"
        "mr 19, %[val]\n\t"
        "mr 20, %[val]\n\t"
        "mr 21, %[val]\n\t"
        "mr 22, %[val]\n\t"
        "mr 23, %[val]\n\t"
        "mr 24, %[val]\n\t"
        "mr 25, %[val]\n\t"
        "mr 26, %[val]\n\t"
        "mr 27, %[val]\n\t"
        "mr 28, %[val]\n\t"
        "mr 29, %[val]\n\t"
        "mr 30, %[val]\n\t"
        "mr 31, %[val]\n\t"
        "1: stmw 8, 0(%[dst])\n\t"
        "addi %[dst], %[dst], 96\n\t"
        "subi %[size], %[size], 96\n\t"
        "cmpwi cr3, %[size], 0\n\t"
        "bgt cr3, 1b\n\t"
        "lmw 8, 96(1)\n\t"
        :
        :[dst] "r" (dst), [val] "r" (val), [size] "r" (size)
        :
    );
}

int main()
{
    rumboot_putstring("Test started\n");

    test_event_send_test_id("test_oi10_cpu_mem_013");

    emi_init(DCR_EM2_EMI_BASE);
    rumboot_putstring("EM2 initialized\n");

    rumboot_putstring("Copy values ...\n");
//    fill_mem((void*)SRAM0_BASE, 0x4e800020, 1048576);
    fill_mem((void*)SRAM0_BASE, 0x4e800020, 30720);



    rumboot_putstring("Copy zeroes ...\n");
//    fill_mem((void*)(SRAM0_BASE + 1048576), 0x00000000, 256);
    fill_mem((void*)(SRAM0_BASE + 30720), 0x00000000, 256);


    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_VALID};
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid, 1);

    rumboot_putstring("Starting test ...\n");
    //START TEST
    TEST_ASSERT(test_icu_lru_array(),"ICU LRU array check failed");

    test_event(EVENT_FINISHED);

    rumboot_putstring("TEST OK\n");
    return 0;
}
