#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <arch/ppc/arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_l1c_fields.h>
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
const uint32_t way1 = (1 << XCREAD_EA_L1I_WAY_i);
const uint32_t way2 = (2 << XCREAD_EA_L1I_WAY_i);
const uint32_t way3 = (3 << XCREAD_EA_L1I_WAY_i);

const uint32_t index19 = (1 << (XCREAD_EA_L1I_INDEX_i + XCREAD_EA_L1I_INDEX_n - 1));

const uint32_t blr_instruction  = 0x4E800020;

//const uint32_t mem_size_instr   = 0x00100000;
const uint32_t mem_size_instr   = 0x00008000;
//const uint32_t mem_size_instr   = 0x00007800;

const uint32_t mem_size_zero    = 0x00000100;

const uint32_t mem_step         = 0x00002000;

//const uint32_t index_count      = 128;
const uint32_t index_count      = 10;


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


//
//static bool test_icu_lru_array()
//{
//    uint32_t addr;
//
//    for (addr = SRAM0_BASE; addr != (SRAM0_BASE + mem_size_instr); addr += mem_step)
//    {
//        rumboot_printf("Testing address 0x%x ...\n", addr);
//
//        for (uint32_t index = 0; index < index_count; index++)
//        {
//            rumboot_printf("Testing index 0x%x ...\n", index);
//
//            rumboot_putstring("Testing MEM0 block ...\n");
//
////            rumboot_printf("%d EVENT_ICU_LRU_MEM0_VALUE_34\n", index);
//
//            //test_event(EVENT_ICU_LRU_MEM0_VALUE_34);
//            ((pWay_func)(addr | (index << XCREAD_EA_L1I_INDEX_i) | way0))();
//            icread((void*)(addr | (index << XCREAD_EA_L1I_INDEX_i) | way0));
//            isync();
//
//            TEST_ASSERT(spr_read(SPR_ICDBDR0) == 0x4E800020,"Read from SPR_ICDBDR0 failed!");
//            rumboot_printf("icdbdr0 = 0x%x\n", spr_read(SPR_ICDBDR0));
//            rumboot_printf("icdbdr1 = 0x%x\n", spr_read(SPR_ICDBDR1));
//            rumboot_printf("icdbtrh = 0x%x\n", spr_read(SPR_ICDBTRH));
//            rumboot_printf("icdbtrl = 0x%x\n", spr_read(SPR_ICDBTRL));
//
////            rumboot_printf("%d EVENT_ICU_LRU_MEM0_VALUE_1E\n", index);
//
//            //test_event(EVENT_ICU_LRU_MEM0_VALUE_1E);
//            ((pWay_func)(addr | (index << XCREAD_EA_L1I_INDEX_i) | way1))();
//            icread((void*)(addr | (index << XCREAD_EA_L1I_INDEX_i) | way1));
//            isync();
//
//            TEST_ASSERT(spr_read(SPR_ICDBDR0) == 0x4E800020,"Read from SPR_ICDBDR0 failed!");
//            rumboot_printf("icdbdr0 = 0x%x\n", spr_read(SPR_ICDBDR0));
//            rumboot_printf("icdbdr1 = 0x%x\n", spr_read(SPR_ICDBDR1));
//            rumboot_printf("icdbtrh = 0x%x\n", spr_read(SPR_ICDBTRH));
//            rumboot_printf("icdbtrl = 0x%x\n", spr_read(SPR_ICDBTRL));
//
////            rumboot_printf("%d EVENT_ICU_LRU_MEM0_VALUE_07\n", index);
//
//            //test_event(EVENT_ICU_LRU_MEM0_VALUE_07);
//            ((pWay_func)(addr | (index << XCREAD_EA_L1I_INDEX_i) | way2))();
//            icread((void*)(addr | (index << XCREAD_EA_L1I_INDEX_i) | way2));
//            isync();
//
//            TEST_ASSERT(spr_read(SPR_ICDBDR0) == 0x4E800020,"Read from SPR_ICDBDR0 failed!");
//            rumboot_printf("icdbdr0 = 0x%x\n", spr_read(SPR_ICDBDR0));
//            rumboot_printf("icdbdr1 = 0x%x\n", spr_read(SPR_ICDBDR1));
//            rumboot_printf("icdbtrh = 0x%x\n", spr_read(SPR_ICDBTRH));
//            rumboot_printf("icdbtrl = 0x%x\n", spr_read(SPR_ICDBTRL));
//
////            rumboot_printf("%d EVENT_ICU_LRU_MEM0_VALUE_00\n", index);
//
//            //test_event(EVENT_ICU_LRU_MEM0_VALUE_00);
//            ((pWay_func)(addr | (index << XCREAD_EA_L1I_INDEX_i) | way3))();
//            icread((void*)(addr | (index << XCREAD_EA_L1I_INDEX_i) | way3));
//            isync();
//
//            TEST_ASSERT(spr_read(SPR_ICDBDR0) == 0x4E800020,"Read from SPR_ICDBDR0 failed!");
//            rumboot_printf("icdbdr0 = 0x%x\n", spr_read(SPR_ICDBDR0));
//            rumboot_printf("icdbdr1 = 0x%x\n", spr_read(SPR_ICDBDR1));
//            rumboot_printf("icdbtrh = 0x%x\n", spr_read(SPR_ICDBTRH));
//            rumboot_printf("icdbtrl = 0x%x\n", spr_read(SPR_ICDBTRL));
//
//
//
//            rumboot_putstring("Testing MEM1 block ...\n");
//
////            rumboot_printf("%d EVENT_ICU_LRU_MEM1_VALUE_34\n", index);
//
//            //test_event(EVENT_ICU_LRU_MEM1_VALUE_34);
//            ((pWay_func)(addr | index19 | (index << XCREAD_EA_L1I_INDEX_i) | way0))();
//            icread((void*)(addr | index19 | (index << XCREAD_EA_L1I_INDEX_i) | way0));
//            isync();
//
//            TEST_ASSERT(spr_read(SPR_ICDBDR0) == 0x4E800020,"Read from SPR_ICDBDR0 failed!");
//            rumboot_printf("icdbdr0 = 0x%x\n", spr_read(SPR_ICDBDR0));
//            rumboot_printf("icdbdr1 = 0x%x\n", spr_read(SPR_ICDBDR1));
//            rumboot_printf("icdbtrh = 0x%x\n", spr_read(SPR_ICDBTRH));
//            rumboot_printf("icdbtrl = 0x%x\n", spr_read(SPR_ICDBTRL));
//
////            rumboot_printf("%d EVENT_ICU_LRU_MEM1_VALUE_1E\n", index);
//
//            //test_event(EVENT_ICU_LRU_MEM1_VALUE_1E);
//            ((pWay_func)(addr | index19 | (index << XCREAD_EA_L1I_INDEX_i) | way1))();
//            icread((void*)(addr | index19 | (index << XCREAD_EA_L1I_INDEX_i) | way1));
//            isync();
//
//            TEST_ASSERT(spr_read(SPR_ICDBDR0) == 0x4E800020,"Read from SPR_ICDBDR0 failed!");
//            rumboot_printf("icdbdr0 = 0x%x\n", spr_read(SPR_ICDBDR0));
//            rumboot_printf("icdbdr1 = 0x%x\n", spr_read(SPR_ICDBDR1));
//            rumboot_printf("icdbtrh = 0x%x\n", spr_read(SPR_ICDBTRH));
//            rumboot_printf("icdbtrl = 0x%x\n", spr_read(SPR_ICDBTRL));
//
////            rumboot_printf("%d EVENT_ICU_LRU_MEM1_VALUE_07\n", index);
//
//            //test_event(EVENT_ICU_LRU_MEM1_VALUE_07);
//            ((pWay_func)(addr | index19 | (index << XCREAD_EA_L1I_INDEX_i) | way2))();
//            icread((void*)(addr | index19 | (index << XCREAD_EA_L1I_INDEX_i) | way2));
//            isync();
//
//            TEST_ASSERT(spr_read(SPR_ICDBDR0) == 0x4E800020,"Read from SPR_ICDBDR0 failed!");
//            rumboot_printf("icdbdr0 = 0x%x\n", spr_read(SPR_ICDBDR0));
//            rumboot_printf("icdbdr1 = 0x%x\n", spr_read(SPR_ICDBDR1));
//            rumboot_printf("icdbtrh = 0x%x\n", spr_read(SPR_ICDBTRH));
//            rumboot_printf("icdbtrl = 0x%x\n", spr_read(SPR_ICDBTRL));
//
////            rumboot_printf("%d EVENT_ICU_LRU_MEM1_VALUE_00\n", index);
//
//            //test_event(EVENT_ICU_LRU_MEM1_VALUE_00);
//            ((pWay_func)(addr | index19 | (index << XCREAD_EA_L1I_INDEX_i) | way3))();
//            icread((void*)(addr | index19 | (index << XCREAD_EA_L1I_INDEX_i) | way3));
//            isync();
//
//            TEST_ASSERT(spr_read(SPR_ICDBDR0) == 0x4E800020,"Read from SPR_ICDBDR0 failed!");
//            rumboot_printf("icdbdr0 = 0x%x\n", spr_read(SPR_ICDBDR0));
//            rumboot_printf("icdbdr1 = 0x%x\n", spr_read(SPR_ICDBDR1));
//            rumboot_printf("icdbtrh = 0x%x\n", spr_read(SPR_ICDBTRH));
//            rumboot_printf("icdbtrl = 0x%x\n", spr_read(SPR_ICDBTRL));
//        }
//    }
//
//    return true;
//}

void access_list_of_addresses(uint32_t* list_of_addresses,  uint32_t length )
{
    for(int i = 0; i < length; ++i)
        ((pWay_func)(list_of_addresses[i]))();
//        icbtls(0,list_of_addresses[i]);
    isync();
}

void lock_list_of_addresses(uint32_t* list_of_addresses,  uint32_t length )
{
    for(int i = 0; i < length; ++i)
        icbtls(0,list_of_addresses[i]);
    isync();
//        ((pWay_func)(list_of_addresses[i]))();
}

void unlock_list_of_addresses(uint32_t* list_of_addresses,  uint32_t length )
{
    for(int i = 0; i < length; ++i)
//        ((pWay_func)(list_of_addresses[i]))();
        icblc(0,list_of_addresses[i]);
//    isync();
}


void print_access_sequence(uint32_t* list_of_addresses,  uint32_t length )
{
    rumboot_printf("Set of addresses:\n");
    for(int i = 0; i < length; ++i)
        rumboot_printf("[%d] = 0x%x\n", i, list_of_addresses[i]);
}

void print_icache_values(uint32_t address)
{
    icread((void*) address);
    rumboot_printf("icdbdr0 = 0x%x\n", spr_read(SPR_ICDBDR0));
    rumboot_printf("icdbdr1 = 0x%x\n", spr_read(SPR_ICDBDR1));
    rumboot_printf("icdbtrh = 0x%x\n", spr_read(SPR_ICDBTRH));
    rumboot_printf("icdbtrl = 0x%x\n", spr_read(SPR_ICDBTRL));
    rumboot_printf("LRU val = 0x%x\n", ((spr_read(SPR_ICDBTRL) & ICDBTRL_LRU_mask) >> ICDBTRL_LRU_i));
    rumboot_printf("LOCK val = 0x%x\n", ((spr_read(SPR_ICDBTRL) & ICDBTRL_LOCK_mask) >> ICDBTRL_LOCK_i));
}

void swap(uint32_t *a, int i, int j)
{
  uint32_t s = a[i];
  a[i] = a[j];
  a[j] = s;
}

bool NextSet(uint32_t *a, uint32_t n)
{
  int j = n - 2;
  while (j != -1 && a[j] >= a[j + 1]) j--;
  if (j == -1)
    return false;
  int k = n - 1;
  while (a[j] >= a[k]) k--;
  swap(a, j, k);
  int l = j + 1, r = n - 1;
  while (l<r)
    swap(a, l++, r--);
  return true;
}

void set_different_lru_states()
{
    //initial caching of a line in all four ways
    //setting order: the least recently used is not called, the next is called first, the third is called before last, the most recently used is called last
    //may be we should use a function that accesses a list of addresses
    uint32_t list_of_addresses_zero_LRU[] = {
            SRAM0_BASE,
            SRAM0_BASE + mem_step,
            SRAM0_BASE + (2 * mem_step),
            SRAM0_BASE + (3 * mem_step),
            SRAM0_BASE,
            SRAM0_BASE + mem_step,
            SRAM0_BASE + (2 * mem_step),
            SRAM0_BASE + (3 * mem_step)
    };
    uint32_t list_of_addresses_one_LRU[] = {
            SRAM0_BASE,
            SRAM0_BASE + mem_step,
            SRAM0_BASE + (2 * mem_step),
            SRAM0_BASE + (3 * mem_step),
            SRAM0_BASE + (3 * mem_step),
            SRAM0_BASE + (2 * mem_step),
            SRAM0_BASE + mem_step,
            SRAM0_BASE
    };
    lock_list_of_addresses(list_of_addresses_zero_LRU, sizeof(list_of_addresses_zero_LRU)/( 2 * sizeof(list_of_addresses_zero_LRU[0]) ));
    print_icache_values(list_of_addresses_zero_LRU[0]);
    ici(0);

    access_list_of_addresses(list_of_addresses_zero_LRU, sizeof(list_of_addresses_zero_LRU)/sizeof(list_of_addresses_zero_LRU[0]));
    print_icache_values(list_of_addresses_zero_LRU[0]);
    //print access sequence
    print_access_sequence(list_of_addresses_zero_LRU,  sizeof(list_of_addresses_zero_LRU)/sizeof(list_of_addresses_zero_LRU[0]));
    //print lru value
    print_icache_values(list_of_addresses_zero_LRU[0]);
    unlock_list_of_addresses(list_of_addresses_zero_LRU, sizeof(list_of_addresses_zero_LRU)/sizeof(list_of_addresses_zero_LRU[0]));
    ici(0);

    access_list_of_addresses(list_of_addresses_one_LRU, sizeof(list_of_addresses_one_LRU)/sizeof(list_of_addresses_one_LRU[0]));
    print_icache_values(list_of_addresses_one_LRU[0]);
    lock_list_of_addresses(list_of_addresses_one_LRU, sizeof(list_of_addresses_one_LRU)/( 2 * sizeof(list_of_addresses_one_LRU[0]) ));
    print_icache_values(list_of_addresses_one_LRU[0]);
    //print access sequence
    print_access_sequence(list_of_addresses_one_LRU,  sizeof(list_of_addresses_one_LRU)/sizeof(list_of_addresses_one_LRU[0]));
    //print lru value
    print_icache_values(list_of_addresses_one_LRU[0]);
    unlock_list_of_addresses(list_of_addresses_one_LRU, sizeof(list_of_addresses_one_LRU)/sizeof(list_of_addresses_one_LRU[0]));
    print_icache_values(list_of_addresses_one_LRU[0]);
    ici(0);
}


int main()
{
    rumboot_putstring("Test started\n");

//    test_event_send_test_id("test_oi10_cpu_mem_013");

    emi_init(DCR_EM2_EMI_BASE);
    rumboot_putstring("EM2 initialized\n");

    rumboot_memfill32((void*)(SRAM0_BASE ), mem_size_instr + 0x40, blr_instruction, 0);
//
//    for (int index = 0; index < index_count; index++)
//    {
//        rumboot_putstring("Copy values ...\n");
//
//
//        rumboot_memfill32((void*)(SRAM0_BASE | (index << XCREAD_EA_L1I_INDEX_i)), (mem_size_instr >> 2), blr_instruction, 0);
//
////        rumboot_printf("SRAM0_BASE + 0x0 = 0x%x\n", ioread32(SRAM0_BASE | (index << 12) + 0x0));
//
//        rumboot_memfill32((void*)(SRAM0_BASE | index19 | (index << XCREAD_EA_L1I_INDEX_i)), (mem_size_instr >> 2), blr_instruction, 0);
//
////        rumboot_printf("SRAM0_BASE + 0x0 = 0x%x\n", ioread32(SRAM0_BASE | (index << 12) + 0x0));
//
//
//        rumboot_putstring("Copy zeroes ...\n");
//
//
//        rumboot_memfill32((void*)((SRAM0_BASE | (index << XCREAD_EA_L1I_INDEX_i)) + mem_size_instr), (mem_size_zero >> 2), 0x00000000, 0);
//
////        rumboot_printf("SRAM0_BASE + mem_size_instr + 0x0 = 0x%x\n", ioread32(SRAM0_BASE + mem_size_instr + 0x0));
//
//        rumboot_memfill32((void*)((SRAM0_BASE | index19 | (index << XCREAD_EA_L1I_INDEX_i)) + mem_size_instr), (mem_size_zero >> 2), 0x00000000, 0);
//
////        rumboot_printf("SRAM0_BASE + mem_size_instr + 0x0 = 0x%x\n", ioread32(SRAM0_BASE + mem_size_instr + 0x0));
//    }


    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_VALID};
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid, 1);

    rumboot_putstring("Starting test ...\n");
//    TEST_ASSERT(test_icu_lru_array(),"ICU LRU array check failed");
    set_different_lru_states();

    test_event(EVENT_FINISHED);

    rumboot_putstring("TEST OK\n");
    return 0;
}
