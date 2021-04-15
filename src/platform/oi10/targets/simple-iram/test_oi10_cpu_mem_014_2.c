#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <arch/ppc/arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_l1c_fields.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/regs_mclfir.h>

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

#define MEM_STEP          0x00002000

//const uint32_t index_count      = 128;
const uint32_t index_count      = 10;


typedef void (*pWay_func) ();

uint32_t list_of_addresses_zero_LRU[] = {
        SRAM0_BASE,
        SRAM0_BASE + MEM_STEP,
        SRAM0_BASE + (2 * MEM_STEP),
        SRAM0_BASE + (3 * MEM_STEP),
        SRAM0_BASE,
        SRAM0_BASE + MEM_STEP,
        SRAM0_BASE + (2 * MEM_STEP),
        SRAM0_BASE + (3 * MEM_STEP)
};
uint32_t list_of_addresses_one_LRU[] = {
        SRAM0_BASE,
        SRAM0_BASE + MEM_STEP,
        SRAM0_BASE + (2 * MEM_STEP),
        SRAM0_BASE + (3 * MEM_STEP),
        SRAM0_BASE + (3 * MEM_STEP),
        SRAM0_BASE + (2 * MEM_STEP),
        SRAM0_BASE + MEM_STEP,
        SRAM0_BASE
};


void touch_list_of_addresses(uint32_t* list_of_addresses,  uint32_t length, uint32_t offset )
{
    for(int i = 0; i < length; ++i)
        dcbt(0,list_of_addresses[i] + offset);
    isync();
}

void access_list_of_addresses(uint32_t* list_of_addresses,  uint32_t length, uint32_t offset )
{
    for(int i = 0; i < length; ++i)
    {
        isync();
        iowrite32(~(ioread32(list_of_addresses[i] + offset)), list_of_addresses[i] + offset);
    }
}

void lock_list_of_addresses(uint32_t* list_of_addresses,  uint32_t length, uint32_t offset )
{
    for(int i = 0; i < length; ++i)
        dcbtls(0,list_of_addresses[i] + offset);
    isync();
}

void unlock_list_of_addresses(uint32_t* list_of_addresses,  uint32_t length, uint32_t offset )
{
    for(int i = 0; i < length; ++i)
        dcblc(0,list_of_addresses[i] + offset);
    isync();
}


//void print_access_sequence(uint32_t* list_of_addresses,  uint32_t length, uint32_t offset )
//{
//    rumboot_printf("Set of addresses:\n");
//    for(int i = 0; i < length; ++i)
//        rumboot_printf("[%d] = 0x%x\n", i, list_of_addresses[i] + offset);
//}

void print_icache_values(uint32_t address)
{
    msync();
    isync();
    dcread((void*) address);
    msync();
    isync();
    uint32_t dcdbtrl = spr_read(SPR_DCDBTRL);
    msync();
    isync();
    rumboot_printf("\nPrinting: address = 0x%x\nicdbdr0 = 0x%x\nicdbdr1 = 0x%x\nicdbtrh = 0x%x\ndcdbtrl = 0x%x\nLRU val = 0x%x, LOCK val = 0x%x, Validity bits = 0x%x\n",
            address,
            spr_read(SPR_ICDBDR0),
            spr_read(SPR_ICDBDR1),
            spr_read(SPR_DCDBTRH),
            dcdbtrl,
            ((dcdbtrl & ICDBTRL_LRU_mask)  >> ICDBTRL_LRU_i),
            ((dcdbtrl & ICDBTRL_LOCK_mask) >> ICDBTRL_LOCK_i),
            ((dcdbtrl & ICDBTRL_LRUV_mask) >> ICDBTRL_LRUV_i));
}

uint32_t print_and_check_icache_values(
        uint32_t    address,
        bool        check_LRU,
        uint32_t    expected_LRU,
        bool        check_LOCK,
        uint32_t    expected_LOCK,
        bool        check_LRUV,
        uint32_t    expected_LRUV
        )
{
    msync();
    isync();
    dcread((void*) address);
    msync();
    isync();
    uint32_t dcdbtrl = spr_read(SPR_DCDBTRL);
    msync();
    isync();
    uint32_t LRU_values = ((dcdbtrl & ICDBTRL_LRU_mask)  >> ICDBTRL_LRU_i);
    uint32_t LOCK_VALUES = ((dcdbtrl & ICDBTRL_LOCK_mask) >> ICDBTRL_LOCK_i);
    uint32_t LRUV_VALUES = ((dcdbtrl & ICDBTRL_LRUV_mask) >> ICDBTRL_LRUV_i);
    uint32_t result = 0;
    if (((check_LRU) && (LRU_values != expected_LRU) )
            || ((check_LOCK) && (LOCK_VALUES != expected_LOCK))
            || ((check_LRUV) && (LRUV_VALUES != expected_LRUV)))
    {
        result |= 1;
        rumboot_printf("\nPrinting: address = 0x%x\nicdbtrh = 0x%x\ndcdbtrl = 0x%x\nLRU val = 0x%x, LOCK val = 0x%x, Validity bits = 0x%x\n",
            address,
            spr_read(SPR_DCDBTRH),
            dcdbtrl,
            LRU_values,
            LOCK_VALUES,
            LRUV_VALUES);
    }
    return result;
}

uint32_t check_LRU(uint32_t address, uint32_t expected_value)
{
    return ((spr_read(SPR_DCDBTRL) & ICDBTRL_LRU_mask) >> ICDBTRL_LRU_i) == expected_value;
}

uint32_t check_LRUV(uint32_t address, uint32_t expected_value)
{
    return ((spr_read(SPR_DCDBTRL) & ICDBTRL_LRUV_mask) >> ICDBTRL_LRUV_i) == expected_value;
}


//void swap(uint32_t *a, int i, int j)
//{
//  uint32_t s = a[i];
//  a[i] = a[j];
//  a[j] = s;
//}
//
//bool NextSet(uint32_t *a, uint32_t n)
//{
//  int j = n - 2;
//  while (j != -1 && a[j] >= a[j + 1]) j--;
//  if (j == -1)
//    return false;
//  int k = n - 1;
//  while (a[j] >= a[k]) k--;
//  swap(a, j, k);
//  int l = j + 1, r = n - 1;
//  while (l<r)
//    swap(a, l++, r--);
//  return true;
//}
//
//void set_different_lru_states()
//{
//    //initial caching of a line in all four ways
//    //setting order: the least recently used is not called, the next is called first, the third is called before last, the most recently used is called last
//    //may be we should use a function that accesses a list of addresses
////    uint32_t list_of_addresses_zero_LRU[] = {
////            SRAM0_BASE,
////            SRAM0_BASE + mem_step,
////            SRAM0_BASE + (2 * mem_step),
////            SRAM0_BASE + (3 * mem_step),
////            SRAM0_BASE,
////            SRAM0_BASE + mem_step,
////            SRAM0_BASE + (2 * mem_step),
////            SRAM0_BASE + (3 * mem_step)
////    };
////    uint32_t list_of_addresses_one_LRU[] = {
////            SRAM0_BASE,
////            SRAM0_BASE + mem_step,
////            SRAM0_BASE + (2 * mem_step),
////            SRAM0_BASE + (3 * mem_step),
////            SRAM0_BASE + (3 * mem_step),
////            SRAM0_BASE + (2 * mem_step),
////            SRAM0_BASE + mem_step,
////            SRAM0_BASE
////    };
//    lock_list_of_addresses(list_of_addresses_zero_LRU, sizeof(list_of_addresses_zero_LRU)/( 2 * sizeof(list_of_addresses_zero_LRU[0]) ), 0);
//    print_icache_values(list_of_addresses_zero_LRU[0]);
//    dci(0);
//
//    access_list_of_addresses(list_of_addresses_zero_LRU, sizeof(list_of_addresses_zero_LRU)/sizeof(list_of_addresses_zero_LRU[0]), 0);
//    print_icache_values(list_of_addresses_zero_LRU[0]);
//    //print access sequence
//    print_access_sequence(list_of_addresses_zero_LRU,  sizeof(list_of_addresses_zero_LRU)/sizeof(list_of_addresses_zero_LRU[0]), 0);
//    //print lru value
//    print_icache_values(list_of_addresses_zero_LRU[0]);
//    unlock_list_of_addresses(list_of_addresses_zero_LRU, sizeof(list_of_addresses_zero_LRU)/sizeof(list_of_addresses_zero_LRU[0]), 0);
//    dci(0);
//
//    access_list_of_addresses(list_of_addresses_one_LRU, sizeof(list_of_addresses_one_LRU)/sizeof(list_of_addresses_one_LRU[7]), 0);
//    print_icache_values(list_of_addresses_one_LRU[7]);
//    lock_list_of_addresses(list_of_addresses_one_LRU, sizeof(list_of_addresses_one_LRU)/( 2 * sizeof(list_of_addresses_one_LRU[7]) ), 0);
//    print_icache_values(list_of_addresses_one_LRU[7]);
//    //print access sequence
//    print_access_sequence(list_of_addresses_one_LRU,  sizeof(list_of_addresses_one_LRU)/sizeof(list_of_addresses_one_LRU[7]), 0);
//    //print lru value
//    print_icache_values(list_of_addresses_one_LRU[7]);
//    unlock_list_of_addresses(list_of_addresses_one_LRU, sizeof(list_of_addresses_one_LRU)/sizeof(list_of_addresses_one_LRU[7]), 0);
//    print_icache_values(list_of_addresses_one_LRU[7]);
//    dci(0);
//}

void touch_address_range(uint32_t offset_from, uint32_t offset_to)
{
    for(uint32_t offset = offset_from; offset <= offset_to; offset += 0x20)
        touch_list_of_addresses(
                list_of_addresses_zero_LRU,
                sizeof(list_of_addresses_zero_LRU)/sizeof(list_of_addresses_zero_LRU[7]),
                offset );

}

uint32_t set_lru_to_zero_and_print()
{
    uint32_t result = 0;
    dci(0);
    msync();
    isync();
//    for(int i = 0; i < mem_size_instr/4; i += 0x20)
//    {
//        touch_list_of_addresses(list_of_addresses_zero_LRU + 4, sizeof(list_of_addresses_zero_LRU)/( 2 * sizeof(list_of_addresses_zero_LRU[0]) ), i);
//        print_icache_values(list_of_addresses_zero_LRU[7] + i);
//    }
//    dci(0);
//    msync();
//    isync();
    for(int i = 0; i < mem_size_instr/4; i += 0x20)
    {
        result |= print_and_check_icache_values(
                list_of_addresses_zero_LRU[7] + i,
                true,
                0x0,
                true,
                0x0,
                true,
                0x0);
    }
    return result;
}


uint32_t set_lru_to_one_and_print()
{
    uint32_t result = 0;
    for(int i = 0; i < mem_size_instr/4; i += 0x20)
    {
        access_list_of_addresses(list_of_addresses_one_LRU, sizeof(list_of_addresses_one_LRU)/sizeof(list_of_addresses_one_LRU[7]), i);
//        print_icache_values(list_of_addresses_one_LRU[7] + i);
    }
    msync();
    isync();
    for(int i = 0; i < mem_size_instr/4; i += 0x20)
    {
        result |= print_and_check_icache_values(
                list_of_addresses_one_LRU[7] + i,
                true,
                0x3f,
                true,
                0x0,
                true,
                0xf);
    }
    return result;
}

uint32_t set_lock_to_one_and_print()
{
    uint32_t result = 0;
    dci(0);
    msync();
    isync();
    for(int i = 0; i < mem_size_instr/4; i += 0x20)
    {
        lock_list_of_addresses(list_of_addresses_one_LRU, sizeof(list_of_addresses_one_LRU)/sizeof(list_of_addresses_one_LRU[7]), i);
//        print_icache_values(list_of_addresses_one_LRU[7] + i);
    }
    msync();
    isync();
    for(int i = 0; i < mem_size_instr/4; i += 0x20)
    {
        result |= print_and_check_icache_values(
                list_of_addresses_zero_LRU[7] + i,
                false,
                0x0,
                true,
                0xf,
                true,
                0xf);
    }
    for(int i = 0; i < mem_size_instr/4; i += 0x20)
    {
        unlock_list_of_addresses(list_of_addresses_one_LRU, 8, i);
    }
    return result;
}

int main()
{
    uint32_t result = 0;
    rumboot_putstring("Test started\n");

//    test_event_send_test_id("test_oi10_cpu_mem_013");

    emi_init(DCR_EM2_EMI_BASE);
    rumboot_putstring("EM2 initialized\n");

    rumboot_memfill32((void*)(SRAM0_BASE ), mem_size_instr + 0x40, 0, 1);


    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_VALID};
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid, 1);

    rumboot_putstring("Starting test ...\n");
    touch_address_range(0x8000, 0x10000);
    dci(0);
    dci(2);
    msync();
    isync();
    set_lru_to_zero_and_print();
    result |= set_lru_to_zero_and_print();
    rumboot_printf("after set_lru_to_zero_and_print result == 0x%x\n", result);
    result |= set_lru_to_one_and_print();
    rumboot_printf("after set_lru_to_one_and_print result == 0x%x\n", result);
    result |= set_lock_to_one_and_print();
    rumboot_printf("after set_lock_to_one_and_print result == 0x%x\n", result);
    result |= set_lru_to_zero_and_print();
    rumboot_printf("after second set_lru_to_zero_and_print result == 0x%x\n", result);
//    set_different_lru_states();

    return result;
}
