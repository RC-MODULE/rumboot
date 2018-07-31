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

#define TLB_ENTRY_IM0_LITTLE_MIRROR_0   MMU_TLB_ENTRY(  0x010,  0x80000,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_1,     0b1 )
#define TLB_ENTRY_IM0_LITTLE_MIRROR_1   MMU_TLB_ENTRY(  0x010,  0x80010,    0x80050,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )
#define TLB_ENTRY_IM1_LITTLE            MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SRAM0_LITTLE          MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_NOR_LITTLE            MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define IM0_BASE_MIRROR 0x80040000

#define IM0_ARR_SIZE   8
uint8_t __attribute__((section(".data"))) im0_array [IM0_ARR_SIZE] = {0};

int main ()
{
    emi_init();

    //IM0 BigeEndian (default)
    rumboot_printf("Check IM0 BigEndian\n");

    rumboot_printf("load\n");
    iowrite8 (0x12 , (uint32_t) im0_array + 0x00);
    iowrite8 (0x34 , (uint32_t) im0_array + 0x01);
    iowrite8 (0x56 , (uint32_t) im0_array + 0x02);
    iowrite8 (0x78 , (uint32_t) im0_array + 0x03);
    TEST_ASSERT ( ioread32 ((uint32_t) im0_array) == 0x12345678, "ERROR: this not BigEnd" );

    rumboot_printf("store\n");
    iowrite32 (0xAB02CD56 ,(uint32_t) im0_array + 0x04);
    TEST_ASSERT ( ioread8 ((uint32_t) im0_array + 0x04) == 0xAB, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ((uint32_t) im0_array + 0x05) == 0x02, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ((uint32_t) im0_array + 0x06) == 0xCD, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 ((uint32_t) im0_array + 0x07) == 0x56, "ERROR: this not BigEnd" );

    //IM0 LittleEndian
    rumboot_printf("Check IM0 LittleEndian\n");

    static const tlb_entry im0_tlb_entry_little_0 = {TLB_ENTRY_IM0_LITTLE_MIRROR_0};
       write_tlb_entries(&im0_tlb_entry_little_0,1);
       msync();
       isync();

    static const tlb_entry im0_tlb_entry_little_1 = {TLB_ENTRY_IM0_LITTLE_MIRROR_1};
       write_tlb_entries(&im0_tlb_entry_little_1,1);
       msync();
       isync();

    uint32_t im0_array_mirror = (uint32_t) im0_array - IM0_BASE + IM0_BASE_MIRROR;

    rumboot_printf("load\n");
    iowrite8 (0x12 , im0_array_mirror + 0x00);
    iowrite8 (0x34 , im0_array_mirror + 0x01);
    iowrite8 (0x56 , im0_array_mirror + 0x02);
    iowrite8 (0x78 , im0_array_mirror + 0x03);
    TEST_ASSERT ( ioread32 (im0_array_mirror) == 0x78563412, "ERROR: this not LittleEnd" );

    rumboot_printf("store\n");
    iowrite32 (0xAB02CD56 , im0_array_mirror + 0x04);
    TEST_ASSERT ( ioread8 (im0_array_mirror + 0x04) == 0x56, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (im0_array_mirror + 0x05) == 0xCD, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (im0_array_mirror + 0x06) == 0x02, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (im0_array_mirror + 0x07) == 0xAB, "ERROR: this not LittleEnd" );

    //IM1 BigeEndian (default)
    rumboot_printf("Check IM1 BigEndian\n");

    rumboot_printf("load\n");
    iowrite8 (0x12 , IM1_BASE + 0x00);
    iowrite8 (0x34 , IM1_BASE + 0x01);
    iowrite8 (0x56 , IM1_BASE + 0x02);
    iowrite8 (0x78 , IM1_BASE + 0x03);
    TEST_ASSERT ( ioread32 (IM1_BASE) == 0x12345678, "ERROR: this not BigEnd" );

    rumboot_printf("store\n");
    iowrite32 (0xAB02CD56 , IM1_BASE + 0x04);
    TEST_ASSERT ( ioread8 (IM1_BASE + 0x04) == 0xAB, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (IM1_BASE + 0x05) == 0x02, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (IM1_BASE + 0x06) == 0xCD, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (IM1_BASE + 0x07) == 0x56, "ERROR: this not BigEnd" );

    //IM1 Little Endian
    static const tlb_entry im1_tlb_entry_little = {TLB_ENTRY_IM1_LITTLE};
    write_tlb_entries(&im1_tlb_entry_little,1);
    msync();
    isync();

    rumboot_printf("Check IM1 LittleEndian\n");

    rumboot_printf("load\n");
    iowrite8 (0x12 , IM1_BASE + 0x00);
    iowrite8 (0x34 , IM1_BASE + 0x01);
    iowrite8 (0x56 , IM1_BASE + 0x02);
    iowrite8 (0x78 , IM1_BASE + 0x03);
    TEST_ASSERT ( ioread32 (IM1_BASE) == 0x78563412, "ERROR: this not LittleEnd" );

    rumboot_printf("store\n");
    iowrite32 (0xAB02CD56 , IM1_BASE + 0x04);
    TEST_ASSERT ( ioread8 (IM1_BASE + 0x04) == 0x56, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (IM1_BASE + 0x05) == 0xCD, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (IM1_BASE + 0x06) == 0x02, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (IM1_BASE + 0x07) == 0xAB, "ERROR: this not LittleEnd" );

//SRAM0 BigEndian (default)
    rumboot_printf("Check SRAM0 BigEndian\n");

    rumboot_printf("load\n");
    iowrite8 (0x12 , SRAM0_BASE + 0x00);
    iowrite8 (0x34 , SRAM0_BASE + 0x01);
    iowrite8 (0x56 , SRAM0_BASE + 0x02);
    iowrite8 (0x78 , SRAM0_BASE + 0x03);
    TEST_ASSERT ( ioread32 (SRAM0_BASE) == 0x12345678, "ERROR: this not BigEnd" );

    rumboot_printf("store\n");
    iowrite32 (0xAB02CD56 , SRAM0_BASE + 0x04);
    TEST_ASSERT ( ioread8 (SRAM0_BASE + 0x04) == 0xAB, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (SRAM0_BASE + 0x05) == 0x02, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (SRAM0_BASE + 0x06) == 0xCD, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (SRAM0_BASE + 0x07) == 0x56, "ERROR: this not BigEnd" );

   //SRAM0 LittleEndian
    rumboot_printf("Check SRAM0 LittleEndian\n");

    static const tlb_entry sram0_tlb_entry_little = {TLB_ENTRY_SRAM0_LITTLE};
    write_tlb_entries(&sram0_tlb_entry_little,1);
    msync();
    isync();

    rumboot_printf("load\n");
    iowrite8 (0x12 , SRAM0_BASE + 0x00);
    iowrite8 (0x34 , SRAM0_BASE + 0x01);
    iowrite8 (0x56 , SRAM0_BASE + 0x02);
    iowrite8 (0x78 , SRAM0_BASE + 0x03);
    TEST_ASSERT ( ioread32 (SRAM0_BASE) == 0x78563412, "ERROR: this not LittleEnd" );

    rumboot_printf("store\n");
    iowrite32 (0xAB02CD56 , SRAM0_BASE + 0x04);
    TEST_ASSERT ( ioread8 (SRAM0_BASE + 0x04) == 0x56, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (SRAM0_BASE + 0x05) == 0xCD, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (SRAM0_BASE + 0x06) == 0x02, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (SRAM0_BASE + 0x07) == 0xAB, "ERROR: this not LittleEnd" );

    //NOR BigEndian (default)
    rumboot_printf("Check NOR BigEndian\n");

    rumboot_printf("store\n");
    nor_write32 (0xAB02CD56 , NOR_BASE + 0x00 );
    TEST_ASSERT ( ioread8 (NOR_BASE + 0x00) == 0xAB, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (NOR_BASE + 0x01) == 0x02, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (NOR_BASE + 0x02) == 0xCD, "ERROR: this not BigEnd" );
    TEST_ASSERT ( ioread8 (NOR_BASE + 0x03) == 0x56, "ERROR: this not BigEnd" );


    //NOR LittleEndian

    rumboot_printf("Check NOR LittleEndian\n");

    static const tlb_entry nor_tlb_entry_little = {TLB_ENTRY_NOR_LITTLE};
    write_tlb_entries(&nor_tlb_entry_little,1);
    msync();
    isync();

    rumboot_printf("store\n");
    nor_write32 (0xA312DC57 , NOR_BASE + 0x04 );
    TEST_ASSERT ( ioread8 (NOR_BASE + 0x04) == 0x57, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (NOR_BASE + 0x05) == 0xDC, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (NOR_BASE + 0x06) == 0x12, "ERROR: this not LittleEnd" );
    TEST_ASSERT ( ioread8 (NOR_BASE + 0x07) == 0xA3, "ERROR: this not LittleEnd" );

    rumboot_printf("TEST OK\n");
    return 0;
}
