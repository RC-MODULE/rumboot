#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/memfill.h>

#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/regs/regs_plb6mcif2.h>
#include <platform/regs/regs_mclfir.h>
#include <platform/regs/regs_emi.h>


#define EVENT_DCU_LRU_MEM0_VALUE_00     (TEST_EVENT_CODE_MIN + 0)
#define EVENT_DCU_LRU_MEM0_VALUE_34     (TEST_EVENT_CODE_MIN + 1)
#define EVENT_DCU_LRU_MEM0_VALUE_1E     (TEST_EVENT_CODE_MIN + 2)
#define EVENT_DCU_LRU_MEM0_VALUE_07     (TEST_EVENT_CODE_MIN + 3)

#define EVENT_DCU_LRU_MEM1_VALUE_00     (TEST_EVENT_CODE_MIN + 4)
#define EVENT_DCU_LRU_MEM1_VALUE_34     (TEST_EVENT_CODE_MIN + 5)
#define EVENT_DCU_LRU_MEM1_VALUE_1E     (TEST_EVENT_CODE_MIN + 6)
#define EVENT_DCU_LRU_MEM1_VALUE_07     (TEST_EVENT_CODE_MIN + 7)

/*
#define GET_ADDR_BY_TAG_INDEX_WORD(TAG,INDEX,WORD) \
    ( (EM0_CACHED_PAGE) | ((TAG) << 13) | ((INDEX) << 5) | ((WORD) << 2) )

#define GET_WINDOW(ERPN_TAG) \
    ( 0x8000 | (((ERPN_TAG) >> 19) << 2) | (((ERPN_TAG) >> 17) & 0x3))
*/

#define TLB_ENTRY_CACHE_VALID    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )



////convert RA tag to EA tag
//uint32_t get_EA_tag(uint32_t erpn_tag)
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


bool test_dcu_lru_array()
{
    uint32_t addr;

    for (addr = SRAM0_BASE; addr != (SRAM0_BASE + 0x00100000); addr += /*0x00010000*/ 0x00100000)
    {
        rumboot_putstring("EVENT_DCU_LRU_MEM0_VALUE_34");
        (void)ioread32(addr | way0);
        test_event(EVENT_DCU_LRU_MEM0_VALUE_34);

        rumboot_putstring("EVENT_DCU_LRU_MEM0_VALUE_1E");
        (void)ioread32(addr | way1);
        test_event(EVENT_DCU_LRU_MEM0_VALUE_1E);

        rumboot_putstring("EVENT_DCU_LRU_MEM0_VALUE_07");
        (void)ioread32(addr | way2);
        test_event(EVENT_DCU_LRU_MEM0_VALUE_07);

        rumboot_putstring("EVENT_DCU_LRU_MEM0_VALUE_00");
        (void)ioread32(addr | way3);
        test_event(EVENT_DCU_LRU_MEM0_VALUE_00);


        rumboot_putstring("EVENT_DCU_LRU_MEM0_VALUE_34 (index19)");
        (void)ioread32(addr | (index19 | way0));
        test_event(EVENT_DCU_LRU_MEM1_VALUE_34);

        rumboot_putstring("EVENT_DCU_LRU_MEM0_VALUE_1E (index19)");
        (void)ioread32(addr | (index19 | way1));
        test_event(EVENT_DCU_LRU_MEM1_VALUE_1E);

        rumboot_putstring("EVENT_DCU_LRU_MEM0_VALUE_07 (index19)");
        (void)ioread32(addr | (index19 | way2));
        test_event(EVENT_DCU_LRU_MEM1_VALUE_07);

        rumboot_putstring("EVENT_DCU_LRU_MEM0_VALUE_00 (index19)");
        (void)ioread32(addr | (index19 | way3));
        test_event(EVENT_DCU_LRU_MEM1_VALUE_00);
    }

//    (void)ioread32((SRAM0_BASE + 0x00000040) | way0);
//    (void)ioread32((SRAM0_BASE + 0x00000080) | way0);
//    (void)ioread32((SRAM0_BASE + 0x00000100) | way0);
//    (void)ioread32((SRAM0_BASE + 0x00000200) | way0);
//    (void)ioread32((SRAM0_BASE + 0x00000400) | way0);
//    (void)ioread32((SRAM0_BASE + 0x00000800) | way0);

    return true;
}


int main()
{
    rumboot_putstring("Test started\n");

    test_event_send_test_id("test_oi10_cpu_mem_014");

    emi_init(DCR_EM2_EMI_BASE);
    rumboot_putstring("EM2 initialized\n");

    rumboot_putstring("Copy values ...\n");
//    rumboot_memfill32((void*)SRAM0_BASE, 1048576, 0x4e800020, 0);
    rumboot_memfill32((void*)SRAM0_BASE, 30720, 0x4e800020, 0);

    rumboot_putstring("Copy zeroes ...\n");
//    rumboot_memfill32((void*)(SRAM0_BASE + 1048576), 256, 0x00000000, 0);
    rumboot_memfill32((void*)(SRAM0_BASE + 30720), 256, 0x00000000, 0);


    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_VALID};
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid, 1);

    rumboot_putstring("Starting test ...\n");
    //START TEST
    TEST_ASSERT(test_dcu_lru_array(),"DCU LRU array check failed");


    rumboot_putstring("TEST OK\n");

    test_event(EVENT_FINISHED);

    return 0;
}
