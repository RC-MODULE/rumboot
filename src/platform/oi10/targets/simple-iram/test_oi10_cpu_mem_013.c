#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <arch/ppc/arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/regs_mclfir.h>
#include <platform/regs/regs_plb6mcif2.h>

#include <rumboot/io.h>
#include <rumboot/macros.h>
#include <rumboot/memfill.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>


//#define EVENT_ICU_LRU_MEM_SET_INDEX     (TEST_EVENT_CODE_MIN + 0)

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

const uint32_t blr_instruction  = 0x4E800020;

//const uint32_t mem_size_instr   = 0x00100000;
const uint32_t mem_size_instr   = 0x00008000;
//const uint32_t mem_size_instr   = 0x00007800;

const uint32_t mem_size_zero    = 0x00000100;

const uint32_t mem_step         = 0x00008000;

const uint32_t index_count      = 128;


typedef void (*pWay_func) ();



//static void fill_mem(void* dst, const uint32_t val, size_t size)
//{
//    asm("stmw 8, 96(1)\n\t"
//        "mr 8, %[val]\n\t"
//        "mr 9, %[val]\n\t"
//        "mr 10, %[val]\n\t"
//        "mr 11, %[val]\n\t"
//        "mr 12, %[val]\n\t"
//        "mr 13, %[val]\n\t"
//        "mr 14, %[val]\n\t"
//        "mr 15, %[val]\n\t"
//        "mr 16, %[val]\n\t"
//        "mr 17, %[val]\n\t"
//        "mr 18, %[val]\n\t"
//        "mr 19, %[val]\n\t"
//        "mr 20, %[val]\n\t"
//        "mr 21, %[val]\n\t"
//        "mr 22, %[val]\n\t"
//        "mr 23, %[val]\n\t"
//        "mr 24, %[val]\n\t"
//        "mr 25, %[val]\n\t"
//        "mr 26, %[val]\n\t"
//        "mr 27, %[val]\n\t"
//        "mr 28, %[val]\n\t"
//        "mr 29, %[val]\n\t"
//        "mr 30, %[val]\n\t"
//        "mr 31, %[val]\n\t"
//        "1: stmw 8, 0(%[dst])\n\t"
//        "addi %[dst], %[dst], 96\n\t"
//        "subi %[size], %[size], 96\n\t"
//        "cmpwi cr3, %[size], 0\n\t"
//        "bgt cr3, 1b\n\t"
//        "lmw 8, 96(1)\n\t"
//        :
//        :[dst] "r" (dst), [val] "r" (val), [size] "r" (size)
//        :"r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "r16", "r17", "r18", "r19",
//         "r20", "r21", "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "cr3"
//    );
//}



static bool test_icu_lru_array()
{
    uint32_t addr;

    for (addr = SRAM0_BASE; addr != (SRAM0_BASE + mem_size_instr); addr += mem_step)
    {
        rumboot_printf("addr = 0x%x\n", addr);

        for (uint32_t index = 0; index < index_count; index++)
        {
            rumboot_putstring("Testing MEM0 block ...\n");

            rumboot_printf("%d EVENT_ICU_LRU_MEM0_VALUE_34\n", index);

            test_event(EVENT_ICU_LRU_MEM0_VALUE_34);
            ((pWay_func)(addr | (index << 5) | way0))();

            rumboot_printf("%d EVENT_ICU_LRU_MEM0_VALUE_1E\n", index);

            test_event(EVENT_ICU_LRU_MEM0_VALUE_1E);
            ((pWay_func)(addr | (index << 5) | way1))();

            rumboot_printf("%d EVENT_ICU_LRU_MEM0_VALUE_07\n", index);

            test_event(EVENT_ICU_LRU_MEM0_VALUE_07);
            ((pWay_func)(addr | (index << 5) | way2))();

            rumboot_printf("%d EVENT_ICU_LRU_MEM0_VALUE_00\n", index);

            test_event(EVENT_ICU_LRU_MEM0_VALUE_00);
            ((pWay_func)(addr | (index << 5) | way3))();



            rumboot_putstring("Testing MEM1 block ...\n");

            rumboot_printf("%d EVENT_ICU_LRU_MEM1_VALUE_34\n", index);

            test_event(EVENT_ICU_LRU_MEM1_VALUE_34);
            ((pWay_func)(addr | (1 << 12) | (index << 5) | way0))();

            rumboot_printf("%d EVENT_ICU_LRU_MEM1_VALUE_1E\n", index);

            test_event(EVENT_ICU_LRU_MEM1_VALUE_1E);
            ((pWay_func)(addr | (1 << 12) | (index << 5) | way1))();

            rumboot_printf("%d EVENT_ICU_LRU_MEM1_VALUE_07\n", index);

            test_event(EVENT_ICU_LRU_MEM1_VALUE_07);
            ((pWay_func)(addr | (1 << 12) | (index << 5) | way2))();

            rumboot_printf("%d EVENT_ICU_LRU_MEM1_VALUE_00\n", index);

            test_event(EVENT_ICU_LRU_MEM1_VALUE_00);
            ((pWay_func)(addr | (1 << 12) | (index << 5) | way3))();
        }
    }

    return true;
}


int main()
{
    rumboot_putstring("Test started\n");

    test_event_send_test_id("test_oi10_cpu_mem_013");

    emi_init(DCR_EM2_EMI_BASE);
    rumboot_putstring("EM2 initialized\n");


    for (int index = 0; index < index_count; index++)
    {
        rumboot_putstring("Copy values ...\n");


        rumboot_memfill32((void*)(SRAM0_BASE | (index << 5)), (mem_size_instr >> 2), blr_instruction, 0);

//        rumboot_printf("SRAM0_BASE + 0x0 = 0x%x\n", ioread32(SRAM0_BASE | (index << 12) + 0x0));

        //rumboot_memfill32((void*)(SRAM0_BASE | (1 << 12) | (index << 5)), (mem_size_instr >> 2), blr_instruction, 0);

//        rumboot_printf("SRAM0_BASE + 0x0 = 0x%x\n", ioread32(SRAM0_BASE | (index << 12) + 0x0));


        rumboot_putstring("Copy zeroes ...\n");


        rumboot_memfill32((void*)((SRAM0_BASE | (index << 5)) + mem_size_instr), (mem_size_zero >> 2), 0x00000000, 0);

//        rumboot_printf("SRAM0_BASE + mem_size_instr + 0x0 = 0x%x\n", ioread32(SRAM0_BASE + mem_size_instr + 0x0));

    //    rumboot_memfill32((void*)((SRAM0_BASE | (1 << 12) | (index << 5)) + mem_size_instr), (mem_size_zero >> 2), 0x00000000, 0);

//        rumboot_printf("SRAM0_BASE + mem_size_instr + 0x0 = 0x%x\n", ioread32(SRAM0_BASE + mem_size_instr + 0x0));
    }


    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_VALID};
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid, 1);

    rumboot_putstring("Starting test ...\n");
    TEST_ASSERT(test_icu_lru_array(),"ICU LRU array check failed");

    test_event(EVENT_FINISHED);

    rumboot_putstring("TEST OK\n");
    return 0;
}
