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
#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>

#define EVENT_CHK_NOR_STORE_BE                      TEST_EVENT_CODE_MIN + 0
#define EVENT_CHK_NOR_STORE_LE                      TEST_EVENT_CODE_MIN + 1

#define EVENT_CHK_NOR_LOAD_BE                       TEST_EVENT_CODE_MIN + 2
#define EVENT_CHK_NOR_LOAD_LE                       TEST_EVENT_CODE_MIN + 3

#define EVENT_CHK_MEM_STORE_BE                      TEST_EVENT_CODE_MIN + 4
#define EVENT_CHK_MEM_STORE_LE                      TEST_EVENT_CODE_MIN + 5

#define EVENT_CHK_MEM_LOAD_BE                       TEST_EVENT_CODE_MIN + 6
#define EVENT_CHK_MEM_LOAD_LE                       TEST_EVENT_CODE_MIN + 7

/*                                      MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
#define TLB_ENTRY_IM0_LITTLE_MIRROR_0   MMU_TLB_ENTRY(  0x010,  0x80000,    0x80060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_IM0_LITTLE_MIRROR_1   MMU_TLB_ENTRY(  0x010,  0x80010,    0x80070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_IM0_BIG_MIRROR_0      MMU_TLB_ENTRY(  0x010,  0x80000,    0x80060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_IM0_BIG_MIRROR_1      MMU_TLB_ENTRY(  0x010,  0x80010,    0x80070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_IM1_LITTLE            MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_3,     0b1 )
//#define TLB_ENTRY_IM1_BIG             MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_3,     0b1 )
//#define TLB_ENTRY_IM2_BIG             MMU_TLB_ENTRY(  0x020,  0xC0040,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_5,     0b1 )
#define TLB_ENTRY_IM2_LITTLE            MMU_TLB_ENTRY(  0x020,  0xC0040,    0x80040,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_5,     0b1 )
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

#define IM0_BASE_MIRROR 0x80060000

#define IM0_ARR_SIZE   8
static uint8_t __attribute__((section(".data"))) im0_array [IM0_ARR_SIZE] = {0};
static uint32_t const im0_array_mirror = (uint32_t) im0_array - IM0_BASE + IM0_BASE_MIRROR;


void TLB_BE ()
{
    //IM0
    static const tlb_entry im0_tlb_entry_big[] = {
            {TLB_ENTRY_IM0_BIG_MIRROR_0},
            {TLB_ENTRY_IM0_BIG_MIRROR_1}
    };
        write_tlb_entries(im0_tlb_entry_big,2);
   /*
   //IM1
    static const tlb_entry im1_tlb_entry_big = {TLB_ENTRY_IM1_BIG};
        write_tlb_entries(&im1_tlb_entry_big ,1);
    //IM2
     static const tlb_entry im2_tlb_entry_big = {TLB_ENTRY_IM2_BIG};
         write_tlb_entries(&im2_tlb_entry_big ,1);
   */
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
    //IM2
    static const tlb_entry im2_tlb_entry_little = {TLB_ENTRY_IM2_LITTLE};
        write_tlb_entries(&im2_tlb_entry_little,1);
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
    extern char IM0_MEMORY_SIZE[];
    extern char IM1_MEMORY_SIZE[];
    extern char IM2_MEMORY_SIZE[];

    if ( (addr >= IM0_BASE_MIRROR)&& (addr < IM0_BASE_MIRROR + (uint32_t)IM0_MEMORY_SIZE)) rumboot_printf ("IM0\n");
    if ( (addr >= IM1_BASE)       && (addr < IM1_BASE + (uint32_t)IM1_MEMORY_SIZE))        rumboot_printf ("IM1\n");
    if ( (addr >= IM2_BASE)       && (addr < IM2_BASE + (uint32_t)IM2_MEMORY_SIZE))        rumboot_printf ("IM2\n");
    if ( (addr >= SRAM0_BASE)     && (addr < SRAM0_BASE + SRAM0_SIZE) )                    rumboot_printf ("SRAM0\n");
    if ( (addr >= NOR_BASE)       && (addr < NOR_BASE + NOR_SIZE) )                        rumboot_printf ("NOR\n");
    if ( (addr >= SRAM1_BASE)     && (addr < SRAM1_BASE + SRAM1_SIZE) )                    rumboot_printf ("SRAM1\n");
    if ( (addr >= SDRAM_BASE)     && (addr < SDRAM_BASE + SDRAM_SIZE) )                    rumboot_printf ("SDRAM\n");
    if ( (addr >= SSRAM_BASE)     && (addr < SSRAM_BASE + SSRAM_SIZE) )                    rumboot_printf ("SSRAM\n");
    if ( (addr >= PIPELINED_BASE) && (addr < PIPELINED_BASE + PIPELINED_SIZE) )            rumboot_printf ("PIPELINED\n");
}

void check_load (uint32_t base_addr, uint32_t code_event, uint8_t const *const data )
{
    rumboot_printf ( "Check MEM:");
    print_name_mem(base_addr);

    iowrite8 (data[0] ,        base_addr + 0x00);
    iowrite8 (data[1] ,        base_addr + 0x01);
    iowrite8 (data[2] ,        base_addr + 0x02);
    iowrite8 (data[3] ,        base_addr + 0x03);
    msync();

#ifdef ENDIAN_HARD_CHECK

       uint32_t event [] = {
               code_event,
               base_addr};

       rumboot_platform_event_raise(EVENT_TESTEVENT, event, ARRAY_SIZE(event) );

#endif

       rumboot_printf ( "Check SOFT\n");

       register uint32_t gpr;
       asm volatile
       (
          "lis 7, 0\n\t"
          "lwz 7, 0(%1)\n\t"
          "mr %0, 7\n\t"
          : "=r"(gpr)
          : "r"(base_addr)
          : "r7"
       );

       rumboot_printf ( "gpr %x\n",gpr );

       if (code_event == EVENT_CHK_MEM_LOAD_BE)
       {  TEST_ASSERT ( gpr == (   (data[0] << 24)
                                 | (data[1] << 16)
                                 | (data[2] << 8)
                                 | (data[3] << 0) ), "ERROR ENDIAN" );
       }
       else
       {
           TEST_ASSERT ( gpr == (  (data[3] << 24)
                                 | (data[2] << 16)
                                 | (data[1] << 8)
                                 | (data[0] << 0) ), "ERROR ENDIAN" );
       }
}

void check_store (uint32_t base_addr, uint32_t code_event, uint8_t const *const data)
{
    rumboot_printf ( "Check MEM:");
    print_name_mem(base_addr);
    uint32_t const data_value = (*(uint32_t*)data);

#ifdef ENDIAN_HARD_CHECK

       uint32_t event [] = {
               code_event,
               base_addr};

       rumboot_platform_event_raise(EVENT_TESTEVENT, event, ARRAY_SIZE(event) );
#endif
       rumboot_printf ( "Check SOFT\n");

       asm volatile
       (
           "lis 7, 0\n\t"
           "mr 7, %1\n\t"
           "stw 7, 0(%0)\n\t"
           "msync \n\t"
           "lis 7, 0\n\t"
           ::"r"(base_addr),"r"(data_value)
           : "r7"
       );

       if (code_event == EVENT_CHK_MEM_STORE_BE)
       {
           TEST_ASSERT ( ioread8 ( base_addr + 0x00)  == data[0], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x01)  == data[1], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x02)  == data[2], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x03)  == data[3], "ERROR ENDIAN" );
       }
       else
       {
           TEST_ASSERT ( ioread8 ( base_addr + 0x00)  == data[3], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x01)  == data[2], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x02)  == data[1], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x03)  == data[0], "ERROR ENDIAN" );
       }
}

void check_nor_load (uint32_t base_addr, uint32_t code_event,  uint8_t const *const data0, uint8_t const *const data1)
{
       rumboot_printf("Check LOAD\n");

       nor_write32  ((*(uint32_t*)data0), base_addr );
       nor_write32  ((*(uint32_t*)data1), base_addr + 0x04);
       msync();

#ifdef ENDIAN_HARD_CHECK

       uint32_t event [] = {
               code_event,
               base_addr + 1};

       rumboot_platform_event_raise(EVENT_TESTEVENT, event, ARRAY_SIZE(event) );

#endif

       rumboot_printf ( "Check SOFT\n");

       register uint32_t gpr;

       asm volatile
       (
          "lis 7, 0\n\t"
          "lwz 7, 0(%1)\n\t"
          "mr %0, 7\n\t"
          : "=r"(gpr)
          : "r"(base_addr + 1)
          : "r7"
       );

       if (code_event == EVENT_CHK_MEM_LOAD_BE)
       {
           TEST_ASSERT ( gpr == ( (data0[1] << 24)
                                                         | (data0[2] << 16)
                                                         | (data0[3] << 8)
                                                         | (data1[0] << 0) ), "ERROR ENDIAN" );
       }
       else
       {
           TEST_ASSERT ( gpr == ( (data1[3] << 24)
                                                         | (data0[0] << 16)
                                                         | (data0[1] << 8)
                                                         | (data0[2] << 0) ), "ERROR ENDIAN" );
       }
}

void check_nor_store (uint32_t base_addr, uint32_t code_event, uint8_t const *const data)
{
       rumboot_printf("Check STORE\n");
       uint32_t const data_value = (*(uint32_t*)data);
       nor_write32   (data_value, base_addr);
       rumboot_printf ( "Check SOFT\n");

       if (code_event == EVENT_CHK_NOR_STORE_BE)
       {
           TEST_ASSERT ( ioread8 ( base_addr + 0x00)  == data[0], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x01)  == data[1], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x02)  == data[2], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x03)  == data[3], "ERROR ENDIAN" );
       }
       else
       {
           TEST_ASSERT ( ioread8 ( base_addr + 0x00)  == data[3], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x01)  == data[2], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x02)  == data[1], "ERROR ENDIAN" );
           TEST_ASSERT ( ioread8 ( base_addr + 0x03)  == data[0], "ERROR ENDIAN" );
       }
#ifdef ENDIAN_HARD_CHECK

       uint32_t event [] = {
               code_event,
               base_addr,
               data_value
       };

       rumboot_platform_event_raise(EVENT_TESTEVENT, event, ARRAY_SIZE(event) );

#endif

}

int main ()
{
    test_event_send_test_id( "test_oi10_endianness");
    emi_init(DCR_EM2_EMI_BASE);

    static uint8_t const data_0[4] = {0x12, 0x34, 0x56, 0x78};
    static uint8_t const data_1[4] = {0x23, 0x45, 0x67, 0x89};
    static uint8_t const data_2[4] = {0x34, 0x56, 0x78, 0x91};
    static uint8_t const data_3[4] = {0x45, 0x67, 0x89, 0x12};
    static uint8_t const data_4[4] = {0x56, 0x78, 0x91, 0x23};
    static uint8_t const data_5[4] = {0x57, 0x75, 0x93, 0x20};
    static uint8_t const data_6[4] = {0x58, 0x78, 0x11, 0x00};
    static uint8_t const data_7[4] = {0x44, 0x18, 0x10, 0x21};
    static uint8_t const data_8[4] = {0x00, 0x11, 0x22, 0x33};
    static uint8_t const data_9[4] = {0x44, 0x55, 0x66, 0x77};
    static uint8_t const data_10[4] = {0x10, 0x14, 0x59, 0x55};
    static uint8_t const data_11[4] = {0x13, 0x42, 0x07, 0x19};
    static uint8_t const data_12[4] = {0x44, 0x50, 0x38, 0x21};
    static uint8_t const data_13[4] = {0x22, 0x10, 0x31, 0x66};
    static uint8_t const data_14[4] = {0x86, 0x18, 0x51, 0x29};
    static uint8_t const data_15[4] = {0x16, 0x70, 0x01, 0x80};
    static uint8_t const data_16[4] = {0x08, 0x38, 0x14, 0x30};
    static uint8_t const data_17[4] = {0x34, 0x08, 0x15, 0x31};

    rumboot_printf("\nCheck BigEndian\n");
    TLB_BE ();

    rumboot_printf("\nCheck LOAD\n");
        check_load (im0_array_mirror,    EVENT_CHK_MEM_LOAD_BE, data_1);
        check_load (SRAM0_BASE,          EVENT_CHK_MEM_LOAD_BE, data_2);
        check_load (SRAM1_BASE,          EVENT_CHK_MEM_LOAD_BE, data_3);
        check_load (SDRAM_BASE,          EVENT_CHK_MEM_LOAD_BE, data_4);
        check_load (SSRAM_BASE,          EVENT_CHK_MEM_LOAD_BE, data_5);
        check_load (PIPELINED_BASE,      EVENT_CHK_MEM_LOAD_BE, data_6);

    rumboot_printf("\nCheck STORE\n");
        check_store (im0_array_mirror,    EVENT_CHK_MEM_STORE_BE, data_7);
        check_store (SRAM0_BASE,          EVENT_CHK_MEM_STORE_BE, data_8);
        check_store (SRAM1_BASE,          EVENT_CHK_MEM_STORE_BE, data_9);
        check_store (SDRAM_BASE,          EVENT_CHK_MEM_STORE_BE, data_10);
        check_store (SSRAM_BASE,          EVENT_CHK_MEM_STORE_BE, data_11);
        check_store (PIPELINED_BASE,      EVENT_CHK_MEM_STORE_BE, data_12);

    rumboot_printf("\nCheck NOR\n");
    check_nor_load (NOR_BASE, EVENT_CHK_MEM_LOAD_BE, data_8, data_9);
    check_nor_store (NOR_BASE + 0x08, EVENT_CHK_NOR_STORE_BE, data_10);

    rumboot_printf("\nCheck LittleEndian\n");
    TLB_LE ();

    rumboot_printf("\nCheck LOAD\n");
    check_load (im0_array_mirror,    EVENT_CHK_MEM_LOAD_LE, data_10);
    check_load (IM1_BASE,            EVENT_CHK_MEM_LOAD_LE, data_11);
    check_load (IM2_BASE,            EVENT_CHK_MEM_LOAD_LE, data_12);
    check_load (SRAM0_BASE,          EVENT_CHK_MEM_LOAD_LE, data_13);
    check_load (SRAM1_BASE,          EVENT_CHK_MEM_LOAD_LE, data_14);
    check_load (SDRAM_BASE,          EVENT_CHK_MEM_LOAD_LE, data_15);
    check_load (SSRAM_BASE,          EVENT_CHK_MEM_LOAD_LE, data_16);
    check_load (PIPELINED_BASE,      EVENT_CHK_MEM_LOAD_LE, data_17);

    rumboot_printf("\nCheck STORE\n");
    check_store (im0_array_mirror,    EVENT_CHK_MEM_STORE_LE, data_1);
    check_store (IM1_BASE,            EVENT_CHK_MEM_STORE_LE, data_1);
    check_store (IM2_BASE,            EVENT_CHK_MEM_STORE_LE, data_2);
    check_store (SRAM0_BASE,          EVENT_CHK_MEM_STORE_LE, data_3);
    check_store (SRAM1_BASE,          EVENT_CHK_MEM_STORE_LE, data_4);
    check_store (SDRAM_BASE,          EVENT_CHK_MEM_STORE_LE, data_5);
    check_store (SSRAM_BASE,          EVENT_CHK_MEM_STORE_LE, data_6);
    check_store (PIPELINED_BASE,      EVENT_CHK_MEM_STORE_LE, data_7);

    rumboot_printf("\nCheck NOR\n");
    check_nor_load (NOR_BASE + 0x10, EVENT_CHK_MEM_LOAD_LE, data_10, data_11);
    check_nor_store (NOR_BASE + 0x18, EVENT_CHK_NOR_STORE_LE, data_0);
    rumboot_printf("TEST OK\n");
    return 0;
}
