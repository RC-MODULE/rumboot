/*
 * test_oi10_cpu_021_tag_lru_fill_l1i.c
 *
 *  Created on: JAN 10, 2019
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>

typedef void func();

                           //MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,    W,      I,      M,      G,        E,                   UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,      TID,               WAY,                BID,                V   )
#define TLB_ENTRY_INV        MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,       0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY_SRAM0_1    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_16KB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM0_2    MMU_TLB_ENTRY(  0x000,  0x00004,    0x00004,    MMU_TLBE_DSIZ_16KB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define START_ADDR      SRAM0_BASE
#define DATA_SIZE       0x8000 // 32KB
#define WORD_NUM        0x400  // 1024 (DATA_SIZE/32)
#define CACHE_LINE_SIZE 32     // L1
#define CODE_BLR_INSTR  0x4E800020

void init_test_data (uint32_t data_addr)
{
    memset((void*)START_ADDR, 0x00, DATA_SIZE);
    for (uint32_t ind = 0, addr = START_ADDR; ind < WORD_NUM - 1; ind++ , addr += CACHE_LINE_SIZE)
    {
        //4 words code
        iowrite64(ioread64(data_addr), addr);
        iowrite64(ioread64(data_addr + 0x8), addr + 0x8);
    }
    iowrite32(CODE_BLR_INSTR, START_ADDR + DATA_SIZE - CACHE_LINE_SIZE);
    msync();
}

void check_with_icread(uint32_t expected_val)
{
    uint32_t reg_ICDBDR0, reg_ICDBTRL, reg_ICDBTRH;
    rumboot_printf("Number of elements = 0x%x\n", WORD_NUM);
    for (uint32_t ind = 0, addr = START_ADDR; ind < WORD_NUM; ind++ , addr += CACHE_LINE_SIZE)
    {
        icread((void*)addr);
        isync();
        reg_ICDBDR0 = spr_read(SPR_ICDBDR0),
        reg_ICDBTRH = spr_read(SPR_ICDBTRH);

        rumboot_printf("ind = %x, addr = %x\n", ind, addr);
        TEST_ASSERT((reg_ICDBTRH & 0x1000) == 0x1000, "TEST ERROR: Valid bit in ICDBTRH[12] not expected");  //tag valid bit
        TEST_ASSERT((reg_ICDBTRH>>13) == (addr >> 13), "Invalid tag in ICDBTRH"); //check tag
        if (ind != WORD_NUM - 1)
        {
            TEST_ASSERT(expected_val == reg_ICDBDR0, "TEST ERROR: Invalid instruction code in ICDBDR0"); //check instruction code
        }
        else //check last elem
        {
            reg_ICDBTRL = spr_read(SPR_ICDBTRL);
            TEST_ASSERT(CODE_BLR_INSTR == reg_ICDBDR0, "TEST ERROR: Invalid instruction code in ICDBDR0");     //check instruction code
            TEST_ASSERT((reg_ICDBTRL & 0xF0000000) == 0xF0000000, "TEST ERROR: Invalid LRUV in ICDBTRL"); //check LRUV bits
        }
    }
}

int main (void)
{
    rumboot_printf("Emi init\n");
    emi_init(DCR_EM2_EMI_BASE);

//*******************************************
    rumboot_printf("Init data\n");

    register uint32_t data_p = 0;
    asm volatile
    (
        "b 2f\n\t"
        "1: mfctr 7\n\t"                //section 1: -> this 4 words will be copied in EM2
        "addi  7, 7, 32\n\t"
        "mtctr 7\n\t"
        "bctr\n\t"
        "2: lis %0, 1b@h\n\t"
        "ori    %0, %0, 1b@l\n\t"
        :"=r"(data_p)
        :
        :"r7"
    );

    //copy 4 (*WORD_NUM) words code from section "1" in EM2, align 32
    init_test_data(data_p);
    rumboot_printf("Data initialized\n");
//*******************************************

    rumboot_printf("Set tlb (l1, l2 cache on)\n");
    static const tlb_entry sram0_tlb_entry[] = { {TLB_ENTRY_INV}, {TLB_ENTRY_SRAM0_1}, {TLB_ENTRY_SRAM0_2} };
    write_tlb_entries(sram0_tlb_entry, ARRAY_SIZE(sram0_tlb_entry));

//*******************************************
    rumboot_printf("Call test func from SRAM0_BASE\n");

    //write start addr in CTR
    uint32_t base_addr = START_ADDR;
    asm volatile
    (
        "lis   7, %0@h\n\t"
        "ori   7, 7, %0@l\n\t"
        "mtctr 7\n\t"
        ::"r"(base_addr):"r7"
    );

    func* f = (func*)(START_ADDR);
    f();

//*******************************************
    rumboot_printf("Check with icread\n");
    check_with_icread (ioread32(data_p));

    rumboot_printf("TEST OK\n");
    return 0;
}
