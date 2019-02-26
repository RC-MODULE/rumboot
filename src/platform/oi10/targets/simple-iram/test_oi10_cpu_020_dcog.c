/*
 * test_oi10_cpu_020_dcog.c
 *
 *  Created on: Feb 19, 2019
 *      Author: v.fufaev
 */
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
    EVENT_CHECK_L2C_MEM,
    EVENT_CHECK_MEM_MEM_S,
    EVENT_CHECK_MEM_L2C_S,
    EVENT_CHECK_L2C_L2C_S,
    EVENT_CHECK_L2C_MEM_S
} test_event_code;

//                               MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_SRAM0_NOCACHE  MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM1_CACHE    MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define BASE_TEST_DATA_VALUE     0x1ACCE551

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

static void check_rd_wr_hard (uint32_t code_ev, uint32_t r_addr, uint32_t w_addr, uint32_t * data)
{
    uint32_t event_arr[4];
    uint32_t r_data = 0x00;

    event_arr[0] = code_ev; event_arr[1] = r_addr, event_arr[2] = w_addr; event_arr[3] = (*data) + 1;
    rumboot_platform_event_raise(EVENT_TESTEVENT, event_arr, ARRAY_SIZE(event_arr) );

    r_data = ioread32(r_addr);
    TEST_ASSERT (r_data == *data, "TEST ASSERT: invalid data");
    (*data)++;

    iowrite32(*data, w_addr);
}

static void check_rd_wr_soft (uint32_t code_ev, uint32_t r_addr, uint32_t w_addr, uint32_t * data, bool data_l2c_expect)
{
    uint32_t event_arr[4];
    uint32_t r_data = 0x00;

    if (data_l2c_expect)
    {
       dcbt(0, r_addr);
       dcbz((void*) r_addr);
       TEST_ASSERT(ioread32(r_addr) == 0x00, "TEST_ASSERT: dcbz not set block to zero");
       dci(2);
    }

    dcbt(0, r_addr);
    check_addr_in_l2c (r_addr, data_l2c_expect);

    event_arr[0] = code_ev; event_arr[1] = r_addr; event_arr[2] = w_addr; event_arr[3] = (*data) + 1;
    rumboot_platform_event_raise(EVENT_TESTEVENT, event_arr, ARRAY_SIZE(event_arr) );

    r_data = ioread32(r_addr);
    TEST_ASSERT (r_data == *data, "TEST ASSERT: invalid data");
    (*data)++;

    iowrite32(*data, w_addr);
    dcbf((void*)w_addr);
    msync();
    dci(2);
}

int main()
{
    test_event_send_test_id("test_oi10_cpu_020_dcog");

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

    uint32_t test_data = BASE_TEST_DATA_VALUE;
    iowrite32(test_data, s0_addr0);
    dci(2);

    //Set tlb
    rumboot_printf("Set tlb\n");
    static const tlb_entry mem_tlb_entrys[] = {{TLB_ENTRY_SRAM0_NOCACHE}, {TLB_ENTRY_SRAM1_CACHE}};
    write_tlb_entries(mem_tlb_entrys, 2);

    //1. Hard check. Use lwz, stw
    rumboot_printf("HARD CHECK\n");
    //MEM -> MEM
    rumboot_printf("mem->mem\n");
    check_rd_wr_hard(EVENT_CHECK_MEM_MEM, s0_addr0, s0_addr1, &test_data);

    //MEM -> L2C
    rumboot_printf("mem->l2c\n");
    check_rd_wr_hard(EVENT_CHECK_MEM_L2C, s0_addr1, s1_addr0, &test_data);

    //L2C -> L2C
    rumboot_printf("l2c->l2c\n");
    check_rd_wr_hard(EVENT_CHECK_L2C_L2C, s1_addr0, s1_addr1, &test_data);

    //L2C -> MEM
    rumboot_printf("l2c->mem\n");
    check_rd_wr_hard(EVENT_CHECK_L2C_MEM, s1_addr1, s0_addr0, &test_data);

    //prepare for soft check
    msync();
    memset((void*)s0_addr0, 0x00, 128);
    memset((void*)s0_addr1, 0x00, 128);
    memset((void*)s1_addr0, 0x00, 128);
    memset((void*)s1_addr1, 0x00, 128);
    test_data = BASE_TEST_DATA_VALUE;
    iowrite32(test_data, s0_addr0);
    dci(2);

    //2. Soft check. Use d*
    rumboot_printf("SOFT CHECK\n");

    //MEM -> MEM
    rumboot_printf("mem->mem\n");
    check_rd_wr_soft(EVENT_CHECK_MEM_MEM_S, s0_addr0, s0_addr1, &test_data, false);

    //MEM -> L2C
    rumboot_printf("mem->l2c\n");
    check_rd_wr_soft(EVENT_CHECK_MEM_L2C_S, s0_addr1, s1_addr0, &test_data, false);

    //L2C -> L2C
    rumboot_printf("l2c->l2c\n");
    check_rd_wr_soft(EVENT_CHECK_L2C_L2C_S, s1_addr0, s1_addr1, &test_data, true);

    //L2C -> MEM
    rumboot_printf("l2c->mem\n");
    check_rd_wr_soft(EVENT_CHECK_L2C_MEM_S, s1_addr1, s0_addr0, &test_data, true);

    rumboot_printf("TEST_FINISHED\n");
    while(1);

    return 0;
}



