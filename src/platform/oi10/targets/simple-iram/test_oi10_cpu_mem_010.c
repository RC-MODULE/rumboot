#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <platform/common_macros/common_macros.h>

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>
#include <platform/devices/nor_1636RR4.h>

//                                  MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_CACHE_VALID_0     MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_VALID_1     MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_VALID_2     MMU_TLB_ENTRY(  0x001,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_VALID_3     MMU_TLB_ENTRY(  0x001,  0xC0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define TLB_ENTRY_VALID_0           MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_VALID_1           MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_VALID_2           MMU_TLB_ENTRY(  0x001,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_VALID_3           MMU_TLB_ENTRY(  0x001,  0xC0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define DATA_CACHE_VAL 0x1234ABCD

//tag array
uint32_t test_tag_array[] =
{
    0b1111111111111111 << 2,
    0b0000000000000000 << 2,
    0b0011111111111111 << 2,
    0b1100000000000000 << 2,
    0b0011111111000000 << 2,
    0b1100000000111111 << 2,
    0b0011110000111100 << 2,
    0b1100001111000011 << 2,
    0b0011001100110011 << 2,
    0b1100110011001100 << 2,
    0b1010101010101010 << 2,
    0b0101010101010101 << 2
};

#define REVERSE8(X) \
    (((X & 0x01) << 7) | \
    ((X & 0x02) << 5) | \
    ((X & 0x04) << 3) | \
    ((X & 0x08) << 1) | \
    ((X & 0x10) >> 1) | \
    ((X & 0x20) >> 3) | \
    ((X & 0x40) >> 5) | \
    ((X & 0x80) >> 7) )

#define MOVE1TO6(X) (((X&(1<<1))<<5) |  ((X&0x7C)>>1) | (X & 1))
#define TAG_A_TO_INDEX(A)  (REVERSE8(MOVE1TO6(A) << 1))

/*
 * erpn - 0 or 1 for em0
 * tag - 0..0x7FFFF
 * erpn_tag:
 * [28:19] = ERPN, (may be 0 or 1 only for EM2, see tlb entries)
 * [18:0] = TAG
 * [31:29] = 0
 */
static uint32_t calc_addr (uint32_t tag, uint32_t index)
{
    return (0x00 | ((tag)<<13) | ((index)<<5) | (0<<2));
}

static void init_block(uint8_t way, uint32_t base_ind)
{
    for (uint32_t i = 0, ind = base_ind; i < ARRAY_SIZE(test_tag_array); i++, ind++ )
    {
        uint32_t tag = test_tag_array[i] | way;
        uint32_t index = TAG_A_TO_INDEX(ind);

        uint32_t addr = calc_addr(tag, index);

        if (addr >= NOR_BASE)
        {
            nor_write32(DATA_CACHE_VAL, addr);
            for (uint32_t j = 1; j < 32; j++)
                nor_write32(0x00, addr + 4*j);
        }
        else
        {
            iowrite32(DATA_CACHE_VAL, addr);
            for (uint32_t j = 1; j < 32; j++)
                iowrite32(0x00, addr + 4*j);

        }
        msync();
    }
}

static void mem_init(void) {
    init_block(0b00, 0x00);
    init_block(0b01, 0x80);
    set_mem_window(MEM_WINDOW_3);
    init_block(0b10, 0x00);
    init_block(0b11, 0x80);
    set_mem_window(MEM_WINDOW_0);
}


void cache_dcu_tag(uint8_t way, uint32_t base_ind)
{
    for (uint32_t i = 0, ind = base_ind; i < ARRAY_SIZE(test_tag_array); i++, ind++)
    {
        uint32_t tag = test_tag_array[i] | way;
        uint32_t index = TAG_A_TO_INDEX(ind);

        uint32_t addr = calc_addr(tag, index);
        uint64_t phys_addr = rumboot_virt_to_phys((void*)addr);

        rumboot_printf("TAG = %x\n", ((uint32_t)phys_addr) >> 13);
        rumboot_printf("address = %x_%x\n", (uint32_t)(phys_addr >> 32), (uint32_t)phys_addr);

        /*1st read initiates a caching*/
        rumboot_printf("read...\n");
        uint32_t read_data = ioread32(addr);
        msync();

        TEST_ASSERT(read_data == DATA_CACHE_VAL, "Compare failed");

        rumboot_printf("dcread...\n");
        uint32_t dcread_data = dcread ((uint32_t) phys_addr);
        isync();
        TEST_ASSERT(dcread_data == DATA_CACHE_VAL, "Compare failed");
        uint32_t reg = spr_read(SPR_DCDBTRH);
        rumboot_printf("SPR_DCDBTRH = %x, TAG = %x\n\n", reg,  reg >> 13);
        TEST_ASSERT((reg >> 13) == (((uint32_t)phys_addr) >> 13), "TEST_ERROR: wrong tag in SPR_DCDBTRH");
        TEST_ASSERT( ((uint32_t) (phys_addr >> 32)) == (reg & 1), "ERROR: Ext addr value in SPR_DCDBTRH" );
    }
}

static bool test_dcu_tag_array()
{
    rumboot_printf("Start test dcu tag array\n");
    cache_dcu_tag(0b00, 0x00);
    cache_dcu_tag(0b01, 0x80);
    set_mem_window(MEM_WINDOW_3);
    cache_dcu_tag(0b10, 0x00);
    cache_dcu_tag(0b11, 0x80);
    set_mem_window(MEM_WINDOW_0);

    return true;
}

int main()
{
    static const tlb_entry tlb_entry_valid[] = {
                    {TLB_ENTRY_VALID_0}, {TLB_ENTRY_VALID_1}, {TLB_ENTRY_VALID_2}, {TLB_ENTRY_VALID_3}
        };
    write_tlb_entries(tlb_entry_valid, ARRAY_SIZE(tlb_entry_valid));

    emi_init(DCR_EM2_EMI_BASE);

    rumboot_memfill8_modelling ((void*)0x00, 0x10000, 0x00, 0x00); //anti x

    rumboot_printf("Init mem\n");
    mem_init();
    rumboot_printf("Init mem done\n");

    rumboot_printf("Set tlb\n");
    static const tlb_entry tlb_entry_cacheable_valid[] = {
            {TLB_ENTRY_CACHE_VALID_0}, {TLB_ENTRY_CACHE_VALID_1}, {TLB_ENTRY_CACHE_VALID_2}, {TLB_ENTRY_CACHE_VALID_3}
    };
    write_tlb_entries(tlb_entry_cacheable_valid, ARRAY_SIZE(tlb_entry_cacheable_valid));

    if (!test_dcu_tag_array())
     {
         rumboot_printf("TEST_ERROR\n");
         return 1;
     }

    rumboot_printf("TEST_OK\n");
    return 0;

}

