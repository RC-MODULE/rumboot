/*
 * test_oi10_endianness.c
 *
 *  Created on: Jul 30, 2018
 *      Author: a.tarasov
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
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>

#define TLB_ENTRY_IM0_LITTLE_MIRROR_0   MMU_TLB_ENTRY(  0x010,  0x80000,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,    MMU_TLBWE_BE_UND,     0b1 )
#define TLB_ENTRY_IM0_LITTLE_MIRROR_1   MMU_TLB_ENTRY(  0x010,  0x80010,    0x80050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,    MMU_TLBWE_BE_UND,     0b1 )
#define TLB_ENTRY_IM1_LITTLE            MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM0_LITTLE          MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM1_NOR_LITTLE      MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM1_NOR_BIG         MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SDRAM_LITTLE          MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_PRAM_LITTLE           MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SSRAM_BIG             MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define IM0_BASE_MIRROR 0x80040000

#define IM0_ARR_SIZE   8
uint8_t __attribute__((section(".data"))) im0_array [IM0_ARR_SIZE] = {0};


void check_mem_big_e (uint32_t base_addr)
{
    rumboot_printf("load\n");
    iowrite8 (0x01 ,  base_addr + 0x00);
    iowrite8 (0x23 ,  base_addr + 0x01);
    iowrite8 (0x45 ,  base_addr + 0x02);
    iowrite8 (0x67 ,  base_addr + 0x03);
    TEST_ASSERT ( ioread32 ( base_addr) == 0x01234567, "ERROR: this not BigEnd" );

    rumboot_printf("store\n");
    iowrite32 (0x89ABCDEF , base_addr + 0x04);
    TEST_ASSERT ( ioread8 ( base_addr+ 0x04) == 0x89, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x05) == 0xAB, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x06) == 0xCD, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x07) == 0xEF, "ERROR: this not BigEnd" );
}

void check_mem_little_e (uint32_t base_addr)
{
    rumboot_printf("load\n");
    iowrite8 (0x01 , base_addr + 0x00);
    iowrite8 (0x23 , base_addr + 0x01);
    iowrite8 (0x45 , base_addr + 0x02);
    iowrite8 (0x67 , base_addr + 0x03);
    TEST_ASSERT ( ioread32 (base_addr) == 0x67452301, "ERROR: this not LittleEnd" );

    rumboot_printf("store\n");
    iowrite32 (0x89ABCDEF , base_addr + 0x04);
    TEST_ASSERT ( ioread8 (base_addr + 0x04) == 0xEF, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x05) == 0xCD, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x06) == 0xAB, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x07) == 0x89, "ERROR: this not LittleEnd" );
}

void check_mem_nor_big_e (uint32_t base_addr)
{
    rumboot_printf("store\n");
    nor_write32 (0x01234567 , base_addr + 0x00 );
    TEST_ASSERT ( ioread8 (base_addr + 0x00) == 0x01, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x01) == 0x23, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x02) == 0x45, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x03) == 0x67, "ERROR: this not BigEnd" );
}

void check_mem_nor_little_e (uint32_t base_addr)
{
    rumboot_printf("store\n");
    nor_write32 (0x89ABCDEF , base_addr + 0x04 );
    TEST_ASSERT ( ioread8 (base_addr + 0x04) == 0xEF, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x05) == 0xCD, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x06) == 0xAB, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x07) == 0x89, "ERROR: this not LittleEnd" );
}


int main ()
{
    emi_init(DCR_EM2_EMI_BASE);

    //IM0 BigeEndian (default)
    rumboot_printf("Check IM0 BigEndian\n");
    check_mem_big_e ((uint32_t) im0_array);

    //IM0 LittleEndian
    rumboot_printf("Check IM0 LittleEndian\n");

    static const tlb_entry im0_tlb_entry_little_0 = {TLB_ENTRY_IM0_LITTLE_MIRROR_0};
       write_tlb_entries(&im0_tlb_entry_little_0,1);

    static const tlb_entry im0_tlb_entry_little_1 = {TLB_ENTRY_IM0_LITTLE_MIRROR_1};
       write_tlb_entries(&im0_tlb_entry_little_1,1);

    uint32_t im0_array_mirror = (uint32_t) im0_array - IM0_BASE + IM0_BASE_MIRROR;
    check_mem_little_e (im0_array_mirror);

    //IM1 BigeEndian (default)
    rumboot_printf("Check IM1 BigEndian\n");
    check_mem_big_e (IM1_BASE);

    //IM1 Little Endian
    static const tlb_entry im1_tlb_entry_little = {TLB_ENTRY_IM1_LITTLE};
    write_tlb_entries(&im1_tlb_entry_little,1);

    rumboot_printf("Check IM1 LittleEndian\n");
    check_mem_little_e (IM1_BASE);


    //SRAM0 BigEndian (default)
    rumboot_printf("Check SRAM0 BigEndian\n");
    check_mem_big_e (SRAM0_BASE);


   //SRAM0 LittleEndian
    rumboot_printf("Check SRAM0 LittleEndian\n");

    static const tlb_entry sram0_tlb_entry_little = {TLB_ENTRY_SRAM0_LITTLE};
    write_tlb_entries(&sram0_tlb_entry_little,1);

    check_mem_little_e (SRAM0_BASE);


    //NOR BigEndian (default)
    rumboot_printf("Check NOR BigEndian\n");
    check_mem_nor_big_e (NOR_BASE);


    //NOR LittleEndian
    rumboot_printf("Check NOR LittleEndian\n");

    static const tlb_entry nor_tlb_entry_little = {TLB_ENTRY_SRAM1_NOR_LITTLE};
    write_tlb_entries(&nor_tlb_entry_little,1);

    check_mem_nor_little_e (NOR_BASE);


    //SRAM1 BigEndian
    rumboot_printf("Check SRAM1 BigEndian\n");

    static const tlb_entry sram1_tlb_entry_big = {TLB_ENTRY_SRAM1_NOR_BIG};
    write_tlb_entries(&sram1_tlb_entry_big,1);

    check_mem_big_e (SRAM1_BASE);


   //SRAM0 LittleEndian
    rumboot_printf("Check SRAM1 LittleEndian\n");

    static const tlb_entry sram1_tlb_entry_little = {TLB_ENTRY_SRAM1_NOR_LITTLE};
    write_tlb_entries(&sram1_tlb_entry_little,1);

    check_mem_little_e (SRAM1_BASE);

    //SDRAM LittleEndian
     rumboot_printf("Check SDRAM LittleEndian\n");

     static const tlb_entry sdram_tlb_entry_little = {TLB_ENTRY_SDRAM_LITTLE};
     write_tlb_entries(&sdram_tlb_entry_little,1);

     check_mem_little_e (SDRAM_BASE);

     //SSRAM BigEndian
      rumboot_printf("Check SSRAM LittleEndian\n");

      static const tlb_entry ssram_tlb_entry_big = {TLB_ENTRY_SSRAM_BIG};
      write_tlb_entries(&ssram_tlb_entry_big,1);

      check_mem_big_e (SSRAM_BASE);

      //PRAM LittleEndian
       rumboot_printf("Check PRAM LittleEndian\n");

       static const tlb_entry pram_tlb_entry_little = {TLB_ENTRY_PRAM_LITTLE};
       write_tlb_entries(&pram_tlb_entry_little,1);

       check_mem_little_e (PIPELINED_BASE);

    rumboot_printf("TEST OK\n");
    return 0;
}
