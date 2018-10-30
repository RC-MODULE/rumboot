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


#define EVENT_CHK_MEM_BE                            TEST_EVENT_CODE_MIN + 0
#define EVENT_CHK_MEM_LE                            TEST_EVENT_CODE_MIN + 1

#define EVENT_CHK_MEM_WR_BE                         TEST_EVENT_CODE_MIN + 2
#define EVENT_CHK_MEM_WR_LE                         TEST_EVENT_CODE_MIN + 3

#define TLB_ENTRY_IM0_LITTLE_MIRROR_0   MMU_TLB_ENTRY(  0x010,  0x80000,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_1,     0b1 )
#define TLB_ENTRY_IM0_LITTLE_MIRROR_1   MMU_TLB_ENTRY(  0x010,  0x80010,    0x80050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_2,     0b1 )
#define TLB_ENTRY_IM0_BIG_MIRROR_0      MMU_TLB_ENTRY(  0x010,  0x80000,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_1,     0b1 )
#define TLB_ENTRY_IM0_BIG_MIRROR_1      MMU_TLB_ENTRY(  0x010,  0x80010,    0x80050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_2,     0b1 )
#define TLB_ENTRY_IM1_LITTLE            MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_3,     0b1 )
#define TLB_ENTRY_IM1_BIG               MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_3,     0b1 )
#define TLB_ENTRY_SRAM0_LITTLE          MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM0_BIG             MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM1_NOR_LITTLE      MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM1_NOR_BIG         MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SDRAM_LITTLE          MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_PRAM_LITTLE           MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SSRAM_BIG             MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SDRAM_BIG             MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_PRAM_BIG              MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SSRAM_LITTLE          MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define IM0_BASE_MIRROR 0x80040000

#define IM0_ARR_SIZE   8
uint8_t __attribute__((section(".data"))) im0_array [IM0_ARR_SIZE] = {0};

void TLB_BE ()
{
    //IM0
    static const tlb_entry im0_tlb_entry_big[] = {
            {TLB_ENTRY_IM0_BIG_MIRROR_0},
            {TLB_ENTRY_IM0_BIG_MIRROR_1}
    };
        write_tlb_entries(im0_tlb_entry_big,2);

   //IM1
    static const tlb_entry im1_tlb_entry_big = {TLB_ENTRY_IM1_BIG};
        write_tlb_entries(&im1_tlb_entry_big ,1);
   //SRAM0
    static tlb_entry sram0_tlb_entry_big = {TLB_ENTRY_SRAM0_BIG};
        write_tlb_entries(&sram0_tlb_entry_big ,1);
    //NOR
    static tlb_entry nor_tlb_entry_big = {TLB_ENTRY_SRAM1_NOR_BIG};
        write_tlb_entries(&nor_tlb_entry_big ,1);
    //SRAM1
    static tlb_entry sram1_tlb_entry_big = {TLB_ENTRY_SRAM1_NOR_BIG};
        write_tlb_entries(&sram1_tlb_entry_big ,1);
    //SDRAM
    static tlb_entry sdram_tlb_entry_big = {TLB_ENTRY_SDRAM_BIG};
        write_tlb_entries(&sdram_tlb_entry_big ,1);
    //SSRAM
    static tlb_entry ssram_tlb_entry_big = {TLB_ENTRY_SSRAM_BIG};
        write_tlb_entries(&ssram_tlb_entry_big,1);
    //PRAM
    static tlb_entry pram_tlb_entry_big = {TLB_ENTRY_PRAM_BIG};
        write_tlb_entries(&pram_tlb_entry_big ,1);
}

void TLB_LE ()
{
    //IM0
    static const tlb_entry im0_tlb_entry_little[] = {
            {TLB_ENTRY_IM0_LITTLE_MIRROR_0},
            {TLB_ENTRY_IM0_LITTLE_MIRROR_1}
    };
        write_tlb_entries(im0_tlb_entry_little,2);

    //IM1
    static const tlb_entry im1_tlb_entry_little = {TLB_ENTRY_IM1_LITTLE};
        write_tlb_entries(&im1_tlb_entry_little,1);
    //SRAM0
    static tlb_entry sram0_tlb_entry_little = {TLB_ENTRY_SRAM0_LITTLE};
        write_tlb_entries(&sram0_tlb_entry_little ,1);
    //NOR
    static tlb_entry nor_tlb_entry_little = {TLB_ENTRY_SRAM1_NOR_LITTLE};
        write_tlb_entries(&nor_tlb_entry_little ,1);
    //SRAM1
    static tlb_entry sram1_tlb_entry_little = {TLB_ENTRY_SRAM1_NOR_LITTLE};
        write_tlb_entries(&sram1_tlb_entry_little ,1);
    //SDRAM
    static tlb_entry sdram_tlb_entry_little = {TLB_ENTRY_SDRAM_LITTLE};
        write_tlb_entries(&sdram_tlb_entry_little ,1);
    //SSRAM
    static tlb_entry ssram_tlb_entry_little = {TLB_ENTRY_SSRAM_LITTLE};
        write_tlb_entries(&ssram_tlb_entry_little ,1);
    //PRAM
    static tlb_entry pram_tlb_entry_little = {TLB_ENTRY_PRAM_LITTLE};
        write_tlb_entries(&pram_tlb_entry_little ,1);
}

void print_name_mem (uint32_t addr)
{
    if ( (addr >= IM0_BASE)&& (addr < IM0_BASE + 0x00040000)) rumboot_printf ("IM0\n");
    if ( (addr >= IM1_BASE)&& (addr < IM1_BASE + 0x00040001)) rumboot_printf ("IM1\n");
    if ( (addr >= SRAM0_BASE) && (addr < SRAM0_BASE + SRAM0_SIZE) ) rumboot_printf ("SRAM0\n");
    if ( (addr >= NOR_BASE) && (addr < NOR_BASE + NOR_SIZE) ) rumboot_printf ("NOR\n");
    if ( (addr >= SRAM1_BASE) && (addr < SRAM1_BASE + SRAM1_SIZE) ) rumboot_printf ("SRAM1\n");
    if ( (addr >= SDRAM_BASE) && (addr < SDRAM_BASE + SDRAM_SIZE) ) rumboot_printf ("SDRAM\n");
    if ( (addr >= SSRAM_BASE) && (addr < SSRAM_BASE + SSRAM_SIZE) ) rumboot_printf ("SSRAM\n");
    if ( (addr >= PIPELINED_BASE) && (addr < PIPELINED_BASE + PIPELINED_SIZE) ) rumboot_printf ("PIPELINED\n");
}

void check_wr_mem_prog_or_hard (uint32_t base_addr, uint32_t code_event, uint8_t const *const data )
{
    rumboot_printf ( "Check MEM:\n");
    print_name_mem(base_addr);
    iowrite32 ((*(uint32_t*)data) , base_addr + 0x00);
#ifdef ENDIAN_HARD_CHECK
    uint32_t event [] = {
            code_event,
            base_addr,
            data[0],data[1],data[2],data[3] };

    rumboot_platform_event_raise(EVENT_TESTEVENT, event, ARRAY_SIZE(event) );
#endif
    /*
#ifdef ENDIAN_HARD_CHECK
    test_event (code_event);
#endif
*/
    uint32_t EVENT = code_event;
    if (EVENT == EVENT_CHK_MEM_BE)
    {
     TEST_ASSERT ( ioread8 ( base_addr + 0x00) == data[0], "ERROR ENDIAN" );
     TEST_ASSERT ( ioread8 ( base_addr + 0x01) == data[1], "ERROR ENDIAN" );
     TEST_ASSERT ( ioread8 ( base_addr + 0x02) == data[2], "ERROR ENDIAN" );
     TEST_ASSERT ( ioread8 ( base_addr + 0x03) == data[3], "ERROR ENDIAN" );
    }
    else
    {
    TEST_ASSERT ( ioread8 ( base_addr + 0x00) == data[3], "ERROR ENDIAN" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x01) == data[2], "ERROR ENDIAN" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x02) == data[1], "ERROR ENDIAN" );
    TEST_ASSERT ( ioread8 ( base_addr + 0x03) == data[0], "ERROR ENDIAN" );
    }
}

void check_rd_mem_prog_not_nor (uint32_t base_addr, uint32_t code_event, uint8_t const *const data)
{
    rumboot_printf ( "Check MEM:\n" );
    print_name_mem(base_addr);
    uint32_t EVENT = code_event;
    if (EVENT == EVENT_CHK_MEM_BE)
    {
     iowrite8 (data[0] ,        base_addr);
     iowrite8 (data[1] ,        base_addr + 0x01);
     iowrite8 (data[2] ,        base_addr + 0x02);
     iowrite8 (data[3] ,        base_addr + 0x03);
    }
    else
    {
     iowrite8 (data[3] ,        base_addr);
     iowrite8 (data[2] ,        base_addr + 0x01);
     iowrite8 (data[1] ,        base_addr + 0x02);
     iowrite8 (data[0] ,        base_addr + 0x03);
    }
    TEST_ASSERT ( ioread32 ( base_addr + 0x00) == *((uint32_t*)data), "ERROR ENDIAN" );

}

void check_NOR_prog (uint32_t base_addr, uint32_t code_event, uint8_t const *const data, uint8_t const *const data0, uint8_t const *const data1)
{
    rumboot_printf ( "Check MEM:\n");
    print_name_mem(base_addr);
    rumboot_printf("Check WRITE\n");
    uint32_t EVENT = code_event;
    if (EVENT == EVENT_CHK_MEM_BE)
    {
        nor_write32 ((*(uint32_t*)data) , base_addr + 0x10);
        TEST_ASSERT ( ioread8 ( base_addr + 0x10) == data[0], "ERROR ENDIAN" );
        TEST_ASSERT ( ioread8 ( base_addr + 0x11) == data[1], "ERROR ENDIAN" );
        TEST_ASSERT ( ioread8 ( base_addr + 0x12)  == data[2], "ERROR ENDIAN" );
        TEST_ASSERT ( ioread8 ( base_addr + 0x13)  == data[3], "ERROR ENDIAN" );
    }
    else
    {
        nor_write32 ((*(uint32_t*)data) , base_addr + 0x14);
        TEST_ASSERT ( ioread8 ( base_addr + 0x14)  == data[3], "ERROR ENDIAN" );
        TEST_ASSERT ( ioread8 ( base_addr + 0x15)  == data[2], "ERROR ENDIAN" );
        TEST_ASSERT ( ioread8 ( base_addr + 0x16)  == data[1], "ERROR ENDIAN" );
        TEST_ASSERT ( ioread8 ( base_addr + 0x17)  == data[0], "ERROR ENDIAN" );
    }

#ifdef ENDIAN_HARD_CHECK
    uint32_t event [] = {
            code_event,
            base_addr,
            data[0],data[1],data[2],data[3] };

    rumboot_platform_event_raise(EVENT_TESTEVENT, event, ARRAY_SIZE(event) );
#endif

    rumboot_printf("Check READ\n");
    if (EVENT == EVENT_CHK_MEM_BE)
    {
        nor_write32  ((*(uint32_t*)data0) , base_addr + 0x00);
        nor_write32  ((*(uint32_t*)data1), base_addr + 0x04);
        TEST_ASSERT ( ioread32 ( base_addr + 0x01) == 0x11223344, "ERROR ENDIAN" );
    }
    else
    {
        nor_write32  ((*(uint32_t*)data0) , base_addr + 0x08);
        nor_write32  ((*(uint32_t*)data1),   base_addr + 0xC);
        TEST_ASSERT ( ioread32 ( base_addr + 0x09) == 0x77001122, "ERROR ENDIAN" );
    }
}


#ifdef ENDIAN_HARD_CHECK
void check_mem_HARD (uint32_t base_addr, uint32_t code_event, uint8_t const *const data)
{
   // uint32_t EVENT = code_event;
    uint32_t event [] = {
            code_event,
            base_addr,
            data[0],data[1],data[2],data[3] };

    rumboot_platform_event_raise(EVENT_TESTEVENT, event, ARRAY_SIZE(event) );

    rumboot_printf ( "Check MEM:\n");
    print_name_mem(base_addr);
    rumboot_printf ( "Check data: %x\n", *((uint32_t*)data) );
    rumboot_printf ( "Check mem: %x\n", ioread32 ( base_addr ) );

        TEST_ASSERT ( ioread32 ( base_addr ) == *((uint32_t*)data), "ERROR ENDIAN" );

}
#endif


int main ()
{
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("=========> Check BigEndian\n");
    rumboot_printf("=========> Check WRITE\n");
    TLB_BE ();

    uint8_t const data_0[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t const data_1[4] = {0x23, 0x45, 0x67, 0x89};
    uint8_t const data_2[4] = {0x34, 0x56, 0x78, 0x91};
    uint8_t const data_3[4] = {0x45, 0x67, 0x89, 0x12};
    uint8_t const data_4[4] = {0x56, 0x78, 0x91, 0x23};
    uint8_t const data_5[4] = {0x57, 0x75, 0x93, 0x20};
    uint8_t const data_6[4] = {0x58, 0x78, 0x11, 0x00};
    uint8_t const data_7[4] = {0x44, 0x18, 0x10, 0x21};
    uint8_t const data_8[4] = {0x00, 0x11, 0x22, 0x33};
    uint8_t const data_9[4] = {0x44, 0x55, 0x66, 0x77};
    uint8_t const data_10[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t const data_11[4] = {0x23, 0x45, 0x67, 0x89};
    uint8_t const data_12[4] = {0x34, 0x56, 0x78, 0x91};
    uint8_t const data_14[4] = {0x56, 0x78, 0x91, 0x23};
    uint8_t const data_15[4] = {0x56, 0x78, 0x91, 0x20};
    uint8_t const data_16[4] = {0x58, 0x78, 0x11, 0x00};
    uint8_t const data_17[4] = {0x44, 0x18, 0x10, 0x21};
    uint32_t im0_array_mirror = (uint32_t) im0_array - IM0_BASE + IM0_BASE_MIRROR;

    check_wr_mem_prog_or_hard ((uint32_t)im0_array_mirror, EVENT_CHK_MEM_BE, data_0);
    check_wr_mem_prog_or_hard (IM1_BASE,       EVENT_CHK_MEM_BE, data_1);
    check_wr_mem_prog_or_hard (SRAM0_BASE,     EVENT_CHK_MEM_BE, data_2);
    check_wr_mem_prog_or_hard (SRAM1_BASE,     EVENT_CHK_MEM_BE, data_4);
    check_wr_mem_prog_or_hard (SDRAM_BASE,     EVENT_CHK_MEM_BE, data_5);
    check_wr_mem_prog_or_hard (SSRAM_BASE,     EVENT_CHK_MEM_BE, data_6);
    check_wr_mem_prog_or_hard (PIPELINED_BASE, EVENT_CHK_MEM_BE, data_7);
    rumboot_printf("=========> Check READ\n");
    check_rd_mem_prog_not_nor ((uint32_t)im0_array_mirror, EVENT_CHK_MEM_BE, data_10);
    check_rd_mem_prog_not_nor (IM1_BASE, EVENT_CHK_MEM_BE,   data_11);
    check_rd_mem_prog_not_nor (SRAM0_BASE, EVENT_CHK_MEM_BE, data_12);
    check_rd_mem_prog_not_nor (SRAM1_BASE, EVENT_CHK_MEM_BE, data_14);
    check_rd_mem_prog_not_nor (SDRAM_BASE, EVENT_CHK_MEM_BE, data_15);
    check_rd_mem_prog_not_nor (SSRAM_BASE, EVENT_CHK_MEM_BE, data_16);
    check_rd_mem_prog_not_nor (PIPELINED_BASE, EVENT_CHK_MEM_BE, data_17);
    rumboot_printf("=========> Check NOR\n");
    check_NOR_prog (NOR_BASE, EVENT_CHK_MEM_BE, data_3, data_8, data_9);
    rumboot_printf("=========> Check WRITE HARD\n");
    check_mem_HARD ((uint32_t)im0_array_mirror, EVENT_CHK_MEM_BE, data_0);
    check_mem_HARD (IM1_BASE,       EVENT_CHK_MEM_WR_BE, data_1);
    check_mem_HARD (SRAM0_BASE,     EVENT_CHK_MEM_WR_BE, data_2);
    check_mem_HARD (NOR_BASE + 0x14,EVENT_CHK_MEM_WR_BE, data_3);
    check_mem_HARD (SRAM1_BASE,     EVENT_CHK_MEM_WR_BE, data_4);
    //check_mem_HARD (SDRAM_BASE,     EVENT_CHK_MEM_WR_BE, data_5);
    check_mem_HARD (SSRAM_BASE,     EVENT_CHK_MEM_WR_BE, data_6);
    check_mem_HARD (PIPELINED_BASE, EVENT_CHK_MEM_WR_BE, data_7);

    rumboot_printf("=========> Check LittleEndian\n");
    rumboot_printf("=========> Check WRITE\n");
    TLB_LE ();
    check_wr_mem_prog_or_hard ((uint32_t)im0_array_mirror, EVENT_CHK_MEM_LE, data_0);
    check_wr_mem_prog_or_hard (IM1_BASE,       EVENT_CHK_MEM_LE, data_1);
    check_wr_mem_prog_or_hard (SRAM0_BASE,     EVENT_CHK_MEM_LE, data_2);
    check_wr_mem_prog_or_hard (SRAM1_BASE,     EVENT_CHK_MEM_LE, data_4);
    check_wr_mem_prog_or_hard (SDRAM_BASE,     EVENT_CHK_MEM_LE, data_5);
    check_wr_mem_prog_or_hard (SSRAM_BASE,     EVENT_CHK_MEM_LE, data_6);
    check_wr_mem_prog_or_hard (PIPELINED_BASE, EVENT_CHK_MEM_LE, data_7);
    rumboot_printf("=========> Check READ\n");
    check_rd_mem_prog_not_nor ((uint32_t)im0_array_mirror, EVENT_CHK_MEM_LE, data_10);
    check_rd_mem_prog_not_nor (IM1_BASE,       EVENT_CHK_MEM_LE, data_11);
    check_rd_mem_prog_not_nor (SRAM0_BASE,     EVENT_CHK_MEM_LE, data_12);
    check_rd_mem_prog_not_nor (SRAM1_BASE,     EVENT_CHK_MEM_LE, data_14);
    check_rd_mem_prog_not_nor (SDRAM_BASE,     EVENT_CHK_MEM_LE, data_15);
    check_rd_mem_prog_not_nor (SSRAM_BASE,     EVENT_CHK_MEM_LE, data_16);
    check_rd_mem_prog_not_nor (PIPELINED_BASE, EVENT_CHK_MEM_LE, data_17);
    rumboot_printf("=========> Check NOR\n");
    check_NOR_prog (NOR_BASE + 0x10, EVENT_CHK_MEM_LE, data_3, data_8, data_9);
    rumboot_printf("=========> Check WRITE HARD\n");
    check_mem_HARD ((uint32_t)im0_array_mirror, EVENT_CHK_MEM_BE, data_0);
    check_mem_HARD (IM1_BASE,       EVENT_CHK_MEM_WR_LE, data_1);
    check_mem_HARD (SRAM0_BASE,     EVENT_CHK_MEM_WR_LE, data_2);
    check_mem_HARD (NOR_BASE + 0x18,EVENT_CHK_MEM_WR_LE, data_3);
    check_mem_HARD (SRAM1_BASE,     EVENT_CHK_MEM_WR_LE, data_4);
   // check_mem_HARD (SDRAM_BASE,     EVENT_CHK_MEM_WR_LE, data_5);
    check_mem_HARD (SSRAM_BASE,     EVENT_CHK_MEM_WR_LE, data_6);
    check_mem_HARD (PIPELINED_BASE, EVENT_CHK_MEM_WR_LE, data_7);

    rumboot_printf("TEST OK\n");
    return 0;
}
