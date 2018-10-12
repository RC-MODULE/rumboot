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

#define EVENT_CHK_LOAD_IM0_BE                       TEST_EVENT_CODE_MIN + 0
#define EVENT_CHK_LOAD_IM0_LE                       TEST_EVENT_CODE_MIN + 1

#define EVENT_CHK_LOAD_IM1_BE                       TEST_EVENT_CODE_MIN + 2
#define EVENT_CHK_LOAD_IM1_LE                       TEST_EVENT_CODE_MIN + 3

#define EVENT_CHK_LOAD_SRAM0_BE                     TEST_EVENT_CODE_MIN + 4
#define EVENT_CHK_LOAD_SRAM0_LE                     TEST_EVENT_CODE_MIN + 5

#define EVENT_CHK_LOAD_SRAM1_BE                     TEST_EVENT_CODE_MIN + 6
#define EVENT_CHK_LOAD_SRAM1_LE                     TEST_EVENT_CODE_MIN + 7

#define EVENT_CHK_STORE_NOR_BE                      TEST_EVENT_CODE_MIN + 8
#define EVENT_CHK_STORE_NOR_LE                      TEST_EVENT_CODE_MIN + 9

#define EVENT_CHK_LOAD_SDRAM_BE                     TEST_EVENT_CODE_MIN + 10
#define EVENT_CHK_LOAD_SDRAM_LE                     TEST_EVENT_CODE_MIN + 11

#define EVENT_CHK_LOAD_SSRAM_BE                     TEST_EVENT_CODE_MIN + 12
#define EVENT_CHK_STORE_SSRAM_BE                    TEST_EVENT_CODE_MIN + 13

#define EVENT_CHK_LOAD_SSRAM_LE                     TEST_EVENT_CODE_MIN + 14
#define EVENT_CHK_STORE_SSRAM_LE                    TEST_EVENT_CODE_MIN + 15

#define EVENT_CHK_LOAD_PRAM_BE                      TEST_EVENT_CODE_MIN + 16
#define EVENT_CHK_LOAD_PRAM_LE                      TEST_EVENT_CODE_MIN + 17

#define EVENT_CHK_LOAD_NOR_BE                       TEST_EVENT_CODE_MIN + 18
#define EVENT_CHK_LOAD_NOR_LE                       TEST_EVENT_CODE_MIN + 19

#define EVENT_INIT_DATA								TEST_EVENT_CODE_MIN + 20

#define TLB_ENTRY_IM0_LITTLE_MIRROR_0   MMU_TLB_ENTRY(  0x010,  0x80000,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,    MMU_TLBWE_BE_UND,     0b1 )
#define TLB_ENTRY_IM0_LITTLE_MIRROR_1   MMU_TLB_ENTRY(  0x010,  0x80010,    0x80050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,    MMU_TLBWE_BE_UND,     0b1 )
#define TLB_ENTRY_IM0_BIG_MIRROR_0      MMU_TLB_ENTRY(  0x010,  0x80000,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,    MMU_TLBWE_BE_UND,     0b1 )
#define TLB_ENTRY_IM0_BIG_MIRROR_1      MMU_TLB_ENTRY(  0x010,  0x80010,    0x80050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,    MMU_TLBWE_BE_UND,     0b1 )
#define TLB_ENTRY_IM1_LITTLE            MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_3,   0b1 )
#define TLB_ENTRY_IM1_BIG               MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_3,   0b1 )
#define TLB_ENTRY_SRAM0_LITTLE          MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM0_BIG             MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM1_NOR_LITTLE      MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM1_NOR_BIG         MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SDRAM_LITTLE          MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_PRAM_LITTLE           MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SSRAM_BIG             MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SDRAM_BIG             MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_PRAM_BIG              MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SSRAM_LITTLE          MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define IM0_BASE_MIRROR 0x80040000

#define IM0_ARR_SIZE   8
uint8_t __attribute__((section(".data"))) im0_array [IM0_ARR_SIZE] = {0};


void check_mem_be_prog (uint32_t base_addr, uint32_t value)
{
    rumboot_printf("load\n");
    iowrite8 ((value & 0xFF000000) >> 24 ,  base_addr);
    iowrite8 ((value & 0x00FF0000) >> 16,  base_addr + 0x01);
    iowrite8 ((value & 0x0000FF00) >> 8,  base_addr + 0x02);
    iowrite8 ((value & 0x000000FF) ,  base_addr + 0x03);
    msync();
    TEST_ASSERT ( ioread32 ( base_addr) == value, "ERROR: this not BigEnd" );

    rumboot_printf("store\n");
    iowrite32 (value , base_addr + 0x04);
    msync();
    TEST_ASSERT ( ioread8 ( base_addr+ 0x04) == (value & 0xFF000000) >> 24, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x05) == (value & 0x00FF0000) >> 16, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x06) == (value & 0x0000FF00) >> 8, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x07) == (value & 0x000000FF), "ERROR: this not BigEnd" );

    //clear values
    iowrite32 (0x00 , base_addr + 0);
    iowrite32 (0x00 , base_addr + 0x04);
    msync();
}

void check_mem_le_prog (uint32_t base_addr, uint32_t value)
{
    rumboot_printf("load\n");
    iowrite8 ((value & 0x000000FF) , base_addr);
    iowrite8 ((value & 0x0000FF00) >> 8 , base_addr + 0x01);
    iowrite8 ((value & 0x00FF0000) >> 16, base_addr + 0x02);
    iowrite8 ((value & 0xFF000000) >> 24, base_addr + 0x03);
    msync();
    TEST_ASSERT ( ioread32 (base_addr) == value, "ERROR: this not LittleEnd" );

    rumboot_printf("store\n");
    iowrite32 (value , base_addr + 0x04);
    msync();
    TEST_ASSERT ( ioread8 (base_addr + 0x04) == (value & 0x000000FF), "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x05) == (value & 0x0000FF00) >> 8, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x06) == (value & 0x00FF0000) >> 16, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x07) == (value & 0xFF000000) >> 24, "ERROR: this not LittleEnd" );

    //clear values
    iowrite32 (0x00 , base_addr + 0);
    iowrite32 (0x00 , base_addr + 0x04);
    msync();
}

void check_mem_nor_be_prog (uint32_t base_addr, uint32_t value)
{
    rumboot_printf("store\n");
    nor_write32 (value , base_addr);
    msync();
    TEST_ASSERT ( ioread8 ( base_addr+ 0) == (value & 0xFF000000) >> 24, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x01) == (value & 0x00FF0000) >> 16, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x02) == (value & 0x0000FF00) >> 8, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x03) == (value & 0x000000FF), "ERROR: this not BigEnd" );
}

void check_mem_nor_le_prog (uint32_t base_addr, uint32_t value)
{
    rumboot_printf("store\n");
    nor_write32 (value , base_addr);
    msync();
    TEST_ASSERT ( ioread8 (base_addr) == (value & 0x000000FF), "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x01) == (value & 0x0000FF00) >> 8, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x02) == (value & 0x00FF0000) >> 16, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (base_addr + 0x03) == (value & 0xFF000000) >> 24, "ERROR: this not LittleEnd" );
}

void check_IM0_PROG ()
{
    //IM0 BigeEndian (default)
    rumboot_printf("Check IM0 BigEndian\n");
    check_mem_be_prog ((uint32_t) im0_array, 0x01020304);

    //IM0 LittleEndian
    rumboot_printf("Check IM0 LittleEndian\n");

    static const tlb_entry im0_tlb_entry_little_0 = {TLB_ENTRY_IM0_LITTLE_MIRROR_0};
       write_tlb_entries(&im0_tlb_entry_little_0,1);

    static const tlb_entry im0_tlb_entry_little_1 = {TLB_ENTRY_IM0_LITTLE_MIRROR_1};
       write_tlb_entries(&im0_tlb_entry_little_1,1);

    uint32_t im0_array_mirror = (uint32_t) im0_array - IM0_BASE + IM0_BASE_MIRROR;
    check_mem_le_prog (im0_array_mirror, 0x01020304);
}

void check_IM1_PROG ()
{
    //IM1 BigeEndian (default)
    rumboot_printf("Check IM1 BigEndian\n");
    check_mem_be_prog (IM1_BASE, 0x22334455);

    //IM1 Little Endian
    static const tlb_entry im1_tlb_entry_little = {TLB_ENTRY_IM1_LITTLE};
    write_tlb_entries(&im1_tlb_entry_little,1);

    rumboot_printf("Check IM1 LittleEndian\n");
    check_mem_le_prog (IM1_BASE + 0x8, 0x22334455);

    static const tlb_entry im1_tlb_entry_big = {TLB_ENTRY_IM1_BIG};
    write_tlb_entries(&im1_tlb_entry_big,1);
	}

void check_SRAM0_PROG ()
{
    //SRAM0 BigEndian (default)
    rumboot_printf("Check SRAM0 BigEndian\n");
    check_mem_be_prog (SRAM0_BASE, 0x33445566);

   //SRAM0 LittleEndian
    rumboot_printf("Check SRAM0 LittleEndian\n");

    static const tlb_entry sram0_tlb_entry_little = {TLB_ENTRY_SRAM0_LITTLE};
    write_tlb_entries(&sram0_tlb_entry_little,1);

    check_mem_le_prog (SRAM0_BASE + 0x8, 0x33445566);
	}

void check_NOR_PROG ()
{
    //NOR BigEndian (default)
    rumboot_printf("Check NOR BigEndian\n");
    check_mem_nor_be_prog (NOR_BASE, 0x44556677);

    //NOR LittleEndian
    rumboot_printf("Check NOR LittleEndian\n");

    static const tlb_entry nor_tlb_entry_little = {TLB_ENTRY_SRAM1_NOR_LITTLE};
    write_tlb_entries(&nor_tlb_entry_little,1);

    check_mem_nor_le_prog (NOR_BASE+0x4, 0x44556677);
	}

void check_SRAM1_PROG ()
{
    //SRAM1 BigEndian
    rumboot_printf("Check SRAM1 BigEndian\n");

    static const tlb_entry sram1_tlb_entry_big = {TLB_ENTRY_SRAM1_NOR_BIG};
    write_tlb_entries(&sram1_tlb_entry_big,1);

    check_mem_be_prog (SRAM1_BASE, 0x55667788);

   //SRAM1 LittleEndian
    rumboot_printf("Check SRAM1 LittleEndian\n");

    static const tlb_entry sram1_tlb_entry_little = {TLB_ENTRY_SRAM1_NOR_LITTLE};
    write_tlb_entries(&sram1_tlb_entry_little,1);

    check_mem_le_prog (SRAM1_BASE+0x8, 0x66778899);
	}

void check_SDRAM_PROG ()
{
    //SDRAM BigEndian
     rumboot_printf("Check SDRAM BigEndian\n");

     static const tlb_entry sdram_tlb_entry_big = {TLB_ENTRY_SDRAM_BIG};
     write_tlb_entries(&sdram_tlb_entry_big,1);

     check_mem_be_prog (SDRAM_BASE, 0x778899AA);

    //SDRAM LittleEndian
     rumboot_printf("Check SDRAM LittleEndian\n");

     static const tlb_entry sdram_tlb_entry_little = {TLB_ENTRY_SDRAM_LITTLE};
     write_tlb_entries(&sdram_tlb_entry_little,1);

     check_mem_le_prog (SDRAM_BASE+0x8, 0x778899AA);
    }

void check_SSRAM_PROG ()
{
    //SSRAM BigEndian
     rumboot_printf("Check SSRAM BigEndian\n");

     static const tlb_entry ssram_tlb_entry_big = {TLB_ENTRY_SSRAM_BIG};
     write_tlb_entries(&ssram_tlb_entry_big,1);

     check_mem_be_prog (SSRAM_BASE, 0x8899AABB);

     //SSRAM LittleEndian
      rumboot_printf("Check SSRAM LittleEndian\n");

      static const tlb_entry ssram_tlb_entry_little = {TLB_ENTRY_SSRAM_LITTLE};
      write_tlb_entries(&ssram_tlb_entry_little,1);

      check_mem_le_prog (SSRAM_BASE+0x8, 0x8899AABB);
	}

void check_PRAM_PROG ()
{
    //PRAM BigEndian
     rumboot_printf("Check PRAM BigEndian\n");

     static const tlb_entry pram_tlb_entry_big = {TLB_ENTRY_PRAM_BIG};
     write_tlb_entries(&pram_tlb_entry_big,1);

     check_mem_be_prog (PIPELINED_BASE, 0x99AABBCC);

     //PRAM LittleEndian
      rumboot_printf("Check PRAM LittleEndian\n");

      static const tlb_entry pram_tlb_entry_little = {TLB_ENTRY_PRAM_LITTLE};
      write_tlb_entries(&pram_tlb_entry_little,1);

      check_mem_le_prog (PIPELINED_BASE + 0x8, 0x99AABBCC);
	}

#ifdef ENDIAN_HARD_CHECK

void check_NOR_store_hard (uint32_t addr_mem, uint32_t code_event, uint32_t value)
{
    nor_write32 (value , addr_mem + 0x00 );
    test_event (code_event);
}

void check_mem_store_hard (uint32_t addr_mem, uint32_t code_event, uint32_t value)
{
    iowrite32 (value , addr_mem + 0x04);
    test_event (code_event);
}

void check_mem_load_hard (uint32_t addr_mem, uint32_t code_event, uint32_t value)
{
    iowrite8 ((value & 0xFF000000) >> 24 ,  addr_mem);
    iowrite8 ((value & 0x00FF0000) >> 16,  addr_mem + 0x01);
    iowrite8 ((value & 0x0000FF00) >> 8,  addr_mem + 0x02);
    iowrite8 ((value & 0x000000FF) ,  addr_mem + 0x03);
    msync();
    rumboot_printf ( "DATA = %x\n", ioread32(addr_mem) );
    asm volatile
        (
            "lwz 7, 0(%0)\n\t"
            ::"r"(addr_mem)
        );
    test_event (code_event);

    //clear values
    iowrite32 (0x00 , addr_mem + 0);
    iowrite32 (0x00 , addr_mem + 0x04);
    msync();
}

void check_IM0_HARD ()
{
    //IM0 BigeEndian (default)
    rumboot_printf("Check IM0\n");
    rumboot_printf("Check BigEndian\n");
    check_mem_load_hard ((uint32_t)im0_array, EVENT_CHK_LOAD_IM0_BE, 0x01234567);

    //IM0 LittleEndian
    rumboot_printf("Check LittleEndian\n");

    static const tlb_entry im0_tlb_entry_little_0 = {TLB_ENTRY_IM0_LITTLE_MIRROR_0};
       write_tlb_entries(&im0_tlb_entry_little_0,1);

    static const tlb_entry im0_tlb_entry_little_1 = {TLB_ENTRY_IM0_LITTLE_MIRROR_1};
       write_tlb_entries(&im0_tlb_entry_little_1,1);

    uint32_t im0_array_mirror = (uint32_t) im0_array - IM0_BASE + IM0_BASE_MIRROR;
    check_mem_load_hard ((uint32_t)im0_array_mirror, EVENT_CHK_LOAD_IM0_LE, 0x01234567);

    //return big
    static const tlb_entry im0_tlb_entry_big_0 = {TLB_ENTRY_IM0_BIG_MIRROR_0};
       write_tlb_entries(&im0_tlb_entry_big_0,1);

    static const tlb_entry im0_tlb_entry_big_1 = {TLB_ENTRY_IM0_BIG_MIRROR_1};
       write_tlb_entries(&im0_tlb_entry_big_1,1);
}

void check_IM1_HARD ()
{
    rumboot_printf("Check IM1\n");
    rumboot_printf("BigEndian\n");
    static const tlb_entry im1_tlb_entry_big = {TLB_ENTRY_IM1_BIG};
    write_tlb_entries(&im1_tlb_entry_big ,1);
    check_mem_load_hard (IM1_BASE, EVENT_CHK_LOAD_IM1_BE, 0x11223344);

    rumboot_printf("LittleEndian\n");
    static const tlb_entry im1_tlb_entry_little = {TLB_ENTRY_IM1_LITTLE};
    write_tlb_entries(&im1_tlb_entry_little,1);
    check_mem_load_hard (IM1_BASE, EVENT_CHK_LOAD_IM1_LE, 0x11223344);

    //return big
    write_tlb_entries(&im1_tlb_entry_big,1);
}

void check_SRAM0_HARD ()
{
    rumboot_printf("Check SRAM0\n");
    rumboot_printf("Check BigEndian\n");
    static tlb_entry sram0_tlb_entry_big = {TLB_ENTRY_SRAM0_BIG};
    write_tlb_entries(&sram0_tlb_entry_big ,1);
    check_mem_load_hard (SRAM0_BASE, EVENT_CHK_LOAD_SRAM0_BE, 0x13457891);

    rumboot_printf("Check LittleEndian\n");
    static tlb_entry sram0_tlb_entry_little = {TLB_ENTRY_SRAM0_LITTLE};
    write_tlb_entries(&sram0_tlb_entry_little ,1);
    check_mem_load_hard (SRAM0_BASE, EVENT_CHK_LOAD_SRAM0_LE, 0x13457891);

    //return big
    static tlb_entry sram0_tlb_entry_big_h = {TLB_ENTRY_SRAM0_BIG};
    write_tlb_entries(&sram0_tlb_entry_big_h,1);
}

void check_NOR_HARD ()
{
    rumboot_printf("Check Store NOR\n");
    rumboot_printf("Check BigEndian\n");
    static tlb_entry nor_tlb_entry_big = {TLB_ENTRY_SRAM1_NOR_BIG};
    write_tlb_entries(&nor_tlb_entry_big ,1);
    //init
    rumboot_printf("Check Load\n");
    check_mem_load_hard (NOR_BASE + 0x8, EVENT_CHK_LOAD_NOR_BE,0xFF789531);
    rumboot_printf("Check Store \n");
    check_NOR_store_hard (NOR_BASE + 0xC, EVENT_CHK_STORE_NOR_BE, 0x89ABCDEF);

    rumboot_printf("Check LittleEndian\n");
    static tlb_entry nor_tlb_entry_little = {TLB_ENTRY_SRAM1_NOR_LITTLE};
    write_tlb_entries(&nor_tlb_entry_little ,1);
    //init
    rumboot_printf("Check Load\n");
    check_mem_load_hard (NOR_BASE + 0x10, EVENT_CHK_LOAD_NOR_LE, 0xFF789531 );
    rumboot_printf("Check Store \n");
    check_NOR_store_hard (NOR_BASE + 0x14, EVENT_CHK_STORE_NOR_LE, 0x89ABCDEF);

    //return big
    static tlb_entry nor_tlb_entry_big_h = {TLB_ENTRY_SRAM1_NOR_BIG};
    write_tlb_entries(&nor_tlb_entry_big_h,1);
}

void check_SRAM1_HARD ()
{
    rumboot_printf("Check SRAM1\n");
    rumboot_printf("Check BigEndian\n");
    static tlb_entry sram1_tlb_entry_big = {TLB_ENTRY_SRAM1_NOR_BIG};
    write_tlb_entries(&sram1_tlb_entry_big ,1);
    check_mem_load_hard (SRAM1_BASE, EVENT_CHK_LOAD_SRAM1_BE, 0x21345683);

    rumboot_printf("Check LittleEndian\n");
    static tlb_entry sram1_tlb_entry_little = {TLB_ENTRY_SRAM1_NOR_LITTLE};
    write_tlb_entries(&sram1_tlb_entry_little ,1);
    check_mem_load_hard (SRAM1_BASE, EVENT_CHK_LOAD_SRAM1_LE, 0x21345683);

    //return big
    static tlb_entry sram1_tlb_entry_big_h = {TLB_ENTRY_SRAM1_NOR_BIG};
    write_tlb_entries(&sram1_tlb_entry_big_h,1);
}

void check_SDRAM_HARD ()
{
    rumboot_printf("Check SDRAM \n");
    rumboot_printf("Check BigEndian\n");
    static tlb_entry sdram_tlb_entry_big = {TLB_ENTRY_SDRAM_BIG};
    write_tlb_entries(&sdram_tlb_entry_big ,1);
    check_mem_load_hard (SDRAM_BASE, EVENT_CHK_LOAD_SDRAM_BE, 0x3541761A);

    rumboot_printf("Check LittleEndian\n");
    static tlb_entry sdram_tlb_entry_little = {TLB_ENTRY_SDRAM_LITTLE};
    write_tlb_entries(&sdram_tlb_entry_little ,1);
    check_mem_load_hard (SDRAM_BASE, EVENT_CHK_LOAD_SDRAM_LE, 0x3541761A);

    //return big
    static tlb_entry sdram_tlb_entry_big_h = {TLB_ENTRY_SDRAM_BIG};
    write_tlb_entries(&sdram_tlb_entry_big_h,1);
}

void check_SSRAM_HARD ()
{
    rumboot_printf("Check SSRAM \n");
    rumboot_printf("Check BigEndian\n");
    static tlb_entry ssram_tlb_entry_big = {TLB_ENTRY_SSRAM_BIG};
    write_tlb_entries(&ssram_tlb_entry_big,1);
    rumboot_printf("Check Load SSRAM \n");
    check_mem_load_hard (SSRAM_BASE, EVENT_CHK_LOAD_SSRAM_BE, 0x89ABCDEF);
    rumboot_printf("Check Store SSRAM \n");
    check_mem_store_hard (SSRAM_BASE, EVENT_CHK_STORE_SSRAM_BE,0x89ABCDEF);

    rumboot_printf("Check LittleEndian\n");
    static tlb_entry ssram_tlb_entry_little = {TLB_ENTRY_SSRAM_LITTLE};
    write_tlb_entries(&ssram_tlb_entry_little ,1);
    rumboot_printf("Check Load SSRAM \n");
    check_mem_load_hard (SSRAM_BASE, EVENT_CHK_LOAD_SSRAM_LE, 0x89ABCDEF);
    rumboot_printf("Check Store SSRAM \n");
    check_mem_store_hard (SSRAM_BASE, EVENT_CHK_STORE_SSRAM_LE,0x89ABCDEF);

    //return big
    static tlb_entry ssram_tlb_entry_big_h = {TLB_ENTRY_SSRAM_BIG};
    write_tlb_entries(&ssram_tlb_entry_big_h,1);
}

void check_PRAM_HARD ()
{
    rumboot_printf("Check PRAM \n");
    rumboot_printf("Check BigEndian\n");
    static tlb_entry pram_tlb_entry_big = {TLB_ENTRY_PRAM_BIG};
    write_tlb_entries(&pram_tlb_entry_big ,1);
    check_mem_load_hard (PIPELINED_BASE, EVENT_CHK_LOAD_PRAM_BE, 0x9214F098);

    rumboot_printf("Check LittleEndian\n");
    static tlb_entry pram_tlb_entry_little = {TLB_ENTRY_PRAM_LITTLE};
    write_tlb_entries(&pram_tlb_entry_little ,1);
    check_mem_load_hard (PIPELINED_BASE, EVENT_CHK_LOAD_PRAM_LE, 0x9214F098);

    //return big
    static tlb_entry pram_tlb_entry_big_h = {TLB_ENTRY_PRAM_BIG};
    write_tlb_entries(&pram_tlb_entry_big_h,1);
}

#endif

int main ()
{
    emi_init(DCR_EM2_EMI_BASE);

    check_IM0_PROG ();
    check_IM1_PROG ();
    check_SRAM0_PROG ();
    check_NOR_PROG ();
    check_SRAM1_PROG ();
    check_SDRAM_PROG ();
    check_SSRAM_PROG ();
    check_PRAM_PROG ();

#ifdef ENDIAN_HARD_CHECK

   rumboot_printf("HARD CHECK \n");
   test_event_send_test_id("test_oi10_endianness");
   test_event (EVENT_INIT_DATA);
/*
 * load:
 *  apparatno init SSRAM, NOR
 *  programno init IM0, IM1, PL, SD, SRAM, SRAM1
 * store:
 *  tolko SSRAM, NOR
 */
   check_IM0_HARD ();
   check_IM1_HARD ();
   check_SRAM0_HARD ();
   check_NOR_HARD ();
   check_SRAM1_HARD ();
   check_SDRAM_HARD ();
   check_SSRAM_HARD ();
   check_PRAM_HARD ();

#endif
    rumboot_printf("TEST OK\n");
    return 0;
}
