#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/irq.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/interrupts.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <platform/regs/regs_l2c_l2.h>
#include <rumboot/rumboot.h>

enum {
    EVENT_CHECK_MEM_MEM = TEST_EVENT_CODE_MIN,
    EVENT_CHECK_MEM_L2C,
    EVENT_CHECK_L2C_L2C,
    EVENT_CHECK_L2C_MEM
} test_event_code;

typedef void func();

//                               MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_SRAM0_NOCACHE  MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM1_CACHE    MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define CODE_BLR 0x4E800020

static void check_addr_in_l2c (uint32_t addr, bool expected)
{
    int32_t cache_way = -1;
    uint64_t phys_addr = rumboot_virt_to_phys((void*) addr);
    uint32_t tag_data = 0;
    if (l2c_arracc_get_way_by_address( DCR_L2C_BASE, (uint32_t)(phys_addr >> 32), (uint32_t) phys_addr, &cache_way )){
        bool res = l2c_arracc_tag_info_read_by_way (DCR_L2C_BASE, (uint32_t)(phys_addr >> 32), (uint32_t) phys_addr, cache_way, &tag_data);
        if(res && !expected)   TEST_ASSERT((tag_data & 0xE0000000) == 0, "Error: data no found in l2c (not expect)");
        if (!res && expected ) TEST_ASSERT(0, "Error: data no found in l2c (not expect)");
    }
    else TEST_ASSERT(!expected, "ERROR: can't get L2C way");
}

static void check_func (uint32_t code_ev, uint32_t r_addr, uint32_t w_addr, bool data_l2c_expect)
{
    uint32_t event_arr[4];

    icbt(2, r_addr);

    event_arr[0] = code_ev; event_arr[1] = r_addr; event_arr[2] = w_addr; event_arr[3] = CODE_BLR;
    rumboot_platform_event_raise(EVENT_TESTEVENT, event_arr, ARRAY_SIZE(event_arr) );

    func* f = (func*)(r_addr);
    f();

    iowrite32(CODE_BLR, w_addr);

    check_addr_in_l2c (w_addr, data_l2c_expect);
    dcbf((void*)w_addr);
    dci(2);
}

int main()
{
    test_event_send_test_id("test_oi10_cpu_020_icog");

    emi_init(DCR_EM2_EMI_BASE);

    //init data
    rumboot_printf("init data\n");
    uint32_t s0_addr0 = (uint32_t) rumboot_malloc_from_named_heap_aligned("SRAM0", 128, 128);
    uint32_t s0_addr1 = (uint32_t) rumboot_malloc_from_named_heap_aligned("SRAM0", 128, 128);
    uint32_t s1_addr0 = (uint32_t) rumboot_malloc_from_named_heap_aligned("SRAM1", 128, 128);
    uint32_t s1_addr1 = (uint32_t) rumboot_malloc_from_named_heap_aligned("SRAM1", 128, 128);
    memset((void*)s0_addr0, 0x00, 128);
    memset((void*)s0_addr1, 0x00, 128);
    memset((void*)s1_addr0, 0x00, 128);
    memset((void*)s1_addr1, 0x00, 128);
    iowrite32(CODE_BLR, s0_addr0);
    msync(); dci(2);

    //Set tlb
    rumboot_printf("Set tlb\n");
    static const tlb_entry mem_tlb_entrys[] = {{TLB_ENTRY_SRAM0_NOCACHE}, {TLB_ENTRY_SRAM1_CACHE}};
    write_tlb_entries(mem_tlb_entrys, 2);

    //MEM -> MEM
    rumboot_printf("mem->mem\n");
    check_func (EVENT_CHECK_MEM_MEM, s0_addr0, s0_addr1, false);

    //MEM -> L2C
    rumboot_printf("mem->l2c\n");
    check_func (EVENT_CHECK_MEM_L2C, s0_addr1, s1_addr0, true);

    //L2C -> L2C
    rumboot_printf("l2c->l2c\n");
    check_func (EVENT_CHECK_L2C_L2C, s1_addr0, s1_addr1, true);

    //L2C -> MEM
    rumboot_printf("l2c->mem\n");
    check_func (EVENT_CHECK_L2C_MEM, s1_addr1, s0_addr0, false);

    rumboot_printf("TEST_FINISHED\n");
    while(1);

    return 0;
}



