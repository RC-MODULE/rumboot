#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/ppc_476fp_lib_c.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/memfill.h>
#include <rumboot/timer.h>

#include <arch/ppc_476fp_mmu.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/regs/regs_mclfir.h>
#include <platform/regs/regs_emi.h>


enum
{
    EVENT_DCU_LRU_MEM_EVEN_VALUE_00 = TEST_EVENT_CODE_MIN,
    EVENT_DCU_LRU_MEM_EVEN_VALUE_34,
    EVENT_DCU_LRU_MEM_EVEN_VALUE_1E,
    EVENT_DCU_LRU_MEM_EVEN_VALUE_07,

    EVENT_DCU_LRU_MEM_ODD_VALUE_00,
    EVENT_DCU_LRU_MEM_ODD_VALUE_34,
    EVENT_DCU_LRU_MEM_ODD_VALUE_1E,
    EVENT_DCU_LRU_MEM_ODD_VALUE_07
};

#define TLB_ENTRY_CACHE_VALID


enum DCU_LRU_ADDR_FEILD
{
    DCU_LRU_ADDR_IDX_i  = 5,
    DCU_LRU_ADDR_WAY_i = 13,
};

enum test_data_value
{
    TEST_DATA_VALUE_0   = 0x55555555,
    TEST_DATA_VALUE_1   = 0xAAAAAAAA,
};

static const uint32_t mem_size_instr    = 0x00008000;
static const uint32_t index_count       = 128;
static const uint32_t mem_step          = 0x00008000;

static void init_test_data(void)
{
    rumboot_putstring("Init DCU LRU test data");

    for (int index = 0; index < index_count; index++)
    {
        rumboot_memfill32((void*)(SRAM0_BASE | (index << DCU_LRU_ADDR_IDX_i)), mem_size_instr/sizeof(uint32_t), TEST_DATA_VALUE_0, 0);

        rumboot_memfill32((void*)((SRAM0_BASE | (index << DCU_LRU_ADDR_IDX_i)) + mem_size_instr), mem_size_instr/sizeof(uint32_t), TEST_DATA_VALUE_1, 0);
    }
}

static inline void test_event_dcu_lru_check( uint32_t const event_code, uint32_t const index )
{
    uint32_t const event [] = { event_code, index };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event, ARRAY_SIZE(event) );
}

static void check_dcu_lru_array(void)
{
    rumboot_putstring("Check DCU LRU array");

    uint32_t data;

    for (uint32_t addr = SRAM0_BASE; addr != (SRAM0_BASE + mem_size_instr); addr += mem_step)
    {
        rumboot_putstring("Addr:");
        rumboot_puthex(addr);

        for (uint32_t index = 0; index < index_count; index++)
        {
            rumboot_putstring("Index:");
            rumboot_puthex(index);

            test_event_dcu_lru_check((index%2) ? EVENT_DCU_LRU_MEM_ODD_VALUE_34 : EVENT_DCU_LRU_MEM_EVEN_VALUE_34, index);
            data = ioread32(addr | (index << DCU_LRU_ADDR_IDX_i) | (0 << DCU_LRU_ADDR_WAY_i));
            msync();
            TEST_ASSERT (data == TEST_DATA_VALUE_0, "Wrong test data");

            test_event_dcu_lru_check((index%2) ? EVENT_DCU_LRU_MEM_EVEN_VALUE_34 : EVENT_DCU_LRU_MEM_ODD_VALUE_34, index);
            data = ioread32(addr | (1 << 12) | (index << DCU_LRU_ADDR_IDX_i) | (0 << DCU_LRU_ADDR_WAY_i));
            msync();
            TEST_ASSERT (data == TEST_DATA_VALUE_0, "Wrong test data");

            test_event_dcu_lru_check((index%2) ? EVENT_DCU_LRU_MEM_ODD_VALUE_1E : EVENT_DCU_LRU_MEM_EVEN_VALUE_1E, index);
            data = ioread32(addr | (index << DCU_LRU_ADDR_IDX_i) | (1 << DCU_LRU_ADDR_WAY_i));
            msync();
            TEST_ASSERT (data == TEST_DATA_VALUE_0, "Wrong test data");

            test_event_dcu_lru_check((index%2) ? EVENT_DCU_LRU_MEM_EVEN_VALUE_1E : EVENT_DCU_LRU_MEM_ODD_VALUE_1E, index);
            data = ioread32(addr | (1 << 12) | (index << DCU_LRU_ADDR_IDX_i) | (1 << DCU_LRU_ADDR_WAY_i));
            msync();
            TEST_ASSERT (data == TEST_DATA_VALUE_0, "Wrong test data");

            test_event_dcu_lru_check((index%2) ? EVENT_DCU_LRU_MEM_ODD_VALUE_07 : EVENT_DCU_LRU_MEM_EVEN_VALUE_07, index);
            data = ioread32(addr | (index << DCU_LRU_ADDR_IDX_i) | (2 << DCU_LRU_ADDR_WAY_i));
            msync();
            TEST_ASSERT (data == TEST_DATA_VALUE_0, "Wrong test data");

            test_event_dcu_lru_check((index%2) ? EVENT_DCU_LRU_MEM_EVEN_VALUE_07 : EVENT_DCU_LRU_MEM_ODD_VALUE_07, index);
            data = ioread32(addr | (1 << 12) | (index << DCU_LRU_ADDR_IDX_i) | (2 << DCU_LRU_ADDR_WAY_i));
            msync();
            TEST_ASSERT (data == TEST_DATA_VALUE_0, "Wrong test data");

            test_event_dcu_lru_check((index%2) ? EVENT_DCU_LRU_MEM_ODD_VALUE_00 : EVENT_DCU_LRU_MEM_EVEN_VALUE_00, index);
            data = ioread32(addr | (index << DCU_LRU_ADDR_IDX_i) | (3 << DCU_LRU_ADDR_WAY_i));
            msync();
            TEST_ASSERT (data == TEST_DATA_VALUE_0, "Wrong test data");

            test_event_dcu_lru_check((index%2) ? EVENT_DCU_LRU_MEM_EVEN_VALUE_00 : EVENT_DCU_LRU_MEM_ODD_VALUE_00, index);
            data = ioread32(addr | (1 << 12) | (index << DCU_LRU_ADDR_IDX_i) | (3 << DCU_LRU_ADDR_WAY_i));
            msync();
            TEST_ASSERT (data == TEST_DATA_VALUE_0, "Wrong test data");
        }
    }
}

int main()
{
    test_event_send_test_id("test_oi10_cpu_mem_014");

    emi_init(DCR_EM2_EMI_BASE);

    init_test_data();

    static const tlb_entry sram0_tlb_entry_cacheable[] = {
/*       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    write_tlb_entries(sram0_tlb_entry_cacheable, ARRAY_SIZE(sram0_tlb_entry_cacheable));

    check_dcu_lru_array();

    test_event(EVENT_FINISHED);

    return 0;
}
