/*
 * test_oi10_cpu_mem_008_base.c
 *
 *  Created on: Oct 9, 2018
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <arch/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <arch/dma2plb6.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>
#include <platform/regs/regs_l2c_l2.h>
#include <arch/dma2plb6.h>
#include <platform/devices/nor_1636RR4.h>

//                                  MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_CACHE_VALID_0     MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_VALID_1     MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_VALID_2     MMU_TLB_ENTRY(  0x001,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_CACHE_VALID_3     MMU_TLB_ENTRY(  0x001,  0xC0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define TLB_ENTRY_VALID_0           MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_VALID_1           MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_VALID_2           MMU_TLB_ENTRY(  0x001,  0x80000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_VALID_3           MMU_TLB_ENTRY(  0x001,  0xC0000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_3,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

typedef void func();

#define CODE_BLR_INSTR 0x4E800020

#define GET_ADDR_BY_TAG_INDEX_WORD(TAG,INDEX,WORD) \
        ( (0x00) | ((TAG) << 13) | ((INDEX) << 5) | ((WORD) << 2) )

uint32_t test_addr_array [] =
{
    0b0111111111111111 << 2,
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

static void init_block(uint8_t way, uint32_t ind)
{
    for (uint i = 0, index = ind; i < ARRAY_SIZE(test_addr_array); i++, index += 4 )
    {
        uint32_t address = GET_ADDR_BY_TAG_INDEX_WORD(test_addr_array[i] | way, index, 0);
        //rumboot_printf("address = %x\n", address);
        if (address >= NOR_BASE)
        {
            nor_write32(CODE_BLR_INSTR, address);
            for (uint32_t j = 1; j < 32; j++)
                nor_write32(0x60000000, address + 4*j);
        }
        else
        {
            iowrite32(CODE_BLR_INSTR, address);
            for (uint32_t j = 1; j < 32; j++)
                iowrite32(0x60000000, address + 4*j);

        }
    }
}

static void mem_init(void) {
    set_mem_window (MEM_WINDOW_0);
    init_block(0b00, 0x00);
    init_block(0b00, 0x80);
    init_block(0b01, 0x00);
    init_block(0b01, 0x80);
    set_mem_window (MEM_WINDOW_3);
    init_block(0b10, 0x00);
    init_block(0b10, 0x80);
    init_block(0b11, 0x00);
    init_block(0b11, 0x80);
}

static void cache_icu_tag(uint8_t way, uint32_t ind)
{
    for (uint i = 0, index = ind; i < ARRAY_SIZE(test_addr_array); i++, index += 4 )
    {
        uint32_t tag = test_addr_array[i] | way;
        uint32_t address = GET_ADDR_BY_TAG_INDEX_WORD(tag, index, 0);
        uint64_t phys_addr = rumboot_virt_to_phys((void*)address);
        rumboot_printf("TAG = %x\n", ((uint32_t)phys_addr) >> 13);
        rumboot_printf("address = %x_%x\n", (uint32_t)(phys_addr >> 32), (uint32_t)phys_addr);

        func* f = (func*)(address);
        f();
        icread( (void *) ((uint32_t) phys_addr) );
        isync();
        uint32_t reg_spr_icdbtrh = spr_read(SPR_ICDBTRH);
        rumboot_printf("SPR_ICDBTRH = %x, tag = %x\n", reg_spr_icdbtrh, reg_spr_icdbtrh >> 13);
        if( (((uint32_t)phys_addr) >> 13) != (reg_spr_icdbtrh >> 13)) {
            TEST_ASSERT(0,"ERROR: Wrong tag in SPR_ICDBTRH");
        }
        TEST_ASSERT( ((uint32_t) (phys_addr >> 32)) == (reg_spr_icdbtrh & 1), "ERROR: Ext addr value in SPR_ICDBTRH" );
    }
}

static bool test_icu_tag_array()
{
    rumboot_printf("Start test icu tag array\n");
    set_mem_window (MEM_WINDOW_0);
    cache_icu_tag(0b00, 0x00);
    cache_icu_tag(0b00, 0x80);
    cache_icu_tag(0b01, 0x00);
    cache_icu_tag(0b01, 0x80);
    set_mem_window (MEM_WINDOW_3);
    cache_icu_tag(0b10, 0x00);
    cache_icu_tag(0b10, 0x80);
    cache_icu_tag(0b11, 0x00);
    cache_icu_tag(0b11, 0x80);

    return true;
}

int main(void)
{
    static const tlb_entry tlb_entry_valid[] = {
                {TLB_ENTRY_VALID_0}, {TLB_ENTRY_VALID_1}, {TLB_ENTRY_VALID_2}, {TLB_ENTRY_VALID_3}
    };
    write_tlb_entries(tlb_entry_valid, ARRAY_SIZE(tlb_entry_valid));

    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Init mem\n");
    mem_init();
    rumboot_printf("Init mem done\n");

    rumboot_printf("Set tlb\n");
    static const tlb_entry tlb_entry_cacheable_valid[] = {
            {TLB_ENTRY_CACHE_VALID_0}, {TLB_ENTRY_CACHE_VALID_1}, {TLB_ENTRY_CACHE_VALID_2}, {TLB_ENTRY_CACHE_VALID_3}
    };
    write_tlb_entries(tlb_entry_cacheable_valid, ARRAY_SIZE(tlb_entry_cacheable_valid));

    if (!test_icu_tag_array()) return 1;

    return 0;
}
