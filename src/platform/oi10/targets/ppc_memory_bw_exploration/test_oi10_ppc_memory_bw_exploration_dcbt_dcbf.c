/*
 * test_oi10_cpu_021_wt_100.c
 *
 *  Created on: Sep 3, 2018
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <platform/trace.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <devices/ugly/emi.h>
#include <platform/devices/l2c.h>
#include <arch/dma2plb6.h>
#include <rumboot/memfill.h>


#define PTRN_INC_1  0x1
#define PTRN_INC_2  0x2
#define PTRN_INC_3  0x3

#define PAGE_SIZE           (256*1024)
#define CODE_ADDR           (SRAM0_BASE)
#define CACHEABLE_SRAM0     (CODE_ADDR + PAGE_SIZE)
#define UNCACHEABLE_SRAM0    (CACHEABLE_SRAM0 + PAGE_SIZE)
#define CACHEABLE_ADDR2     (UNCACHEABLE_SRAM0 + PAGE_SIZE)
#define UNCACHEABLE_SSRAM   (SSRAM_BASE)
#define CACHEABLE_SSRAM   (SSRAM_BASE + PAGE_SIZE)

#define GET_EXP_DATA(base_addr, addr, inc)  ( (((addr - base_addr) / sizeof(uint32_t)) + 1) * inc )


#define NUM_OF_WORDS 1024
#define NUM_OF_ARRAYS 8
#define PPC476FP_L2_CACHELINE_SIZE 128u 

bool __attribute__((section(".text.test"))) cache_testing_function( void ) {
#ifdef L2C_CHECK_SIZE
    uint32_t const l2c_check_size = L2C_CHECK_SIZE;
#else
    struct l2c_mem_layout l2c_layout;
    l2c_get_mem_layout( DCR_L2C_BASE, &l2c_layout );
    uint32_t const l2c_size_in_bytes[4] = {
        [L2C_L2Size_128KB] = 128*1024,
        [L2C_L2Size_256KB] = 256*1024,
        [L2C_L2Size_512KB] = 512*1024,
        [L2C_L2Size_1MB] = 1*1024*1024
    };
    uint32_t const l2c_check_size = l2c_size_in_bytes[l2c_layout.l2size];
#endif
    uint32_t const check_words_num  = l2c_check_size / sizeof(uint32_t);
    uint32_t const line_words_num = L2C_LINE_SIZE / sizeof(uint32_t);

    rumboot_printf( "Start testing function\n" );

    rumboot_printf( "1. Init direct cacheable memory 1 (pattern 1)\n" );
    rumboot_memfill32( (void*)CACHEABLE_SRAM0, check_words_num, 0xACACDCDC, 0);
    rumboot_memfill32( (void*)CACHEABLE_SSRAM, check_words_num, 0xACACDCDC, 0);
    msync();

    rumboot_printf( "Set tlb (WB) (4a)\n" );
    static tlb_entry const wb_tlb_entries[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,  IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        ///* code noncacheable page */
        //{MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        //{MMU_TLB_ENTRY(  0x000,  0x00010,    0x00010,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        //{MMU_TLB_ENTRY(  0x000,  0x00020,    0x00020,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        //{MMU_TLB_ENTRY(  0x000,  0x00030,    0x00030,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        /* code cacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_64KB,     0b0,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00010,    0x00010,    MMU_TLBE_DSIZ_64KB,     0b0,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00020,    0x00020,    MMU_TLBE_DSIZ_64KB,     0b0,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00030,    0x00030,    MMU_TLBE_DSIZ_64KB,     0b0,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        /* test data l2 cacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x00040,    0x00040,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00050,    0x00050,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00060,    0x00060,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00070,    0x00070,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        /* test data noncacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x00080,    0x00080,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00090,    0x00090,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x000A0,    0x000A0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x000B0,    0x000B0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        /* test data l2 cacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x000C0,    0x000C0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x000D0,    0x000D0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x000E0,    0x000E0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x000F0,    0x000F0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
//*******SSRAM************************************************************************************************************************************************************************************************
        /* test data noncacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40010,    0x40010,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40020,    0x40020,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40030,    0x40030,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        /* test data l2 cacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x40040,    0x40040,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40050,    0x40050,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40060,    0x40060,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40070,    0x40070,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    write_tlb_entries( wb_tlb_entries, ARRAY_SIZE(wb_tlb_entries) );
//***********************************************************************************************************************************************************************************************************
    rumboot_printf( "Invalidate L2, L1D\n" );
    dci(2); dci(0);
    msync();
//***********************************************************************************************************************************************************************************************************
    uint32_t* src;
    uint32_t  it;
    uint32_t* volatile __attribute__((used)) start_signal;
    uint32_t mem_base[] = {CACHEABLE_SRAM0, CACHEABLE_SSRAM, UNCACHEABLE_SRAM0, UNCACHEABLE_SSRAM};
    char mem_name[][20] = {"CACHEABLE_SRAM0", "CACHEABLE_SSRAM", "UNCACHEABLE_SRAM0", "UNCACHEABLE_SSRAM"};
    unsigned long range_start;
    unsigned long range_end;
    unsigned long addr;
    unsigned long mask_line_base;
    unsigned long start_addr;
    unsigned long end_addr;
    uint32_t src_prnt_l, src_prnt_b;
    uint64_t src_p;
//***********************************************************************************************************************************************************************************************************
// ----- EMI CACHED MEM TESTS    
//***********************************************************************************************************************************************************************************************************
    for (int k=0; k<2; k++){
        rumboot_printf("*************************************************************************\n");
        //--------
        src = mem_base[k];
        src_p = rumboot_virt_to_phys(src);
        src_prnt_l = src_p;
        src_prnt_b = src_p >> 32;
        //-------------
        start_addr = (unsigned long)src;
        end_addr = start_addr + NUM_OF_ARRAYS*NUM_OF_WORDS * sizeof(uint32_t);
        // Устанавливаем адрес на первое слово в строке (это необязательно делать)
        mask_line_base = (~(PPC476FP_L2_CACHELINE_SIZE - 1));
        range_start = start_addr & mask_line_base;
        range_end = (end_addr & mask_line_base) + PPC476FP_L2_CACHELINE_SIZE;
        // Инвалидируем данные из диапазона из кэша данных, чтобы при следующем
        // использовании данные были загружены из памяти
        // dcbf сбрасывает 4 строки DL1 (и соответственно одну строку L2)
        //-------------
        rumboot_printf("%s: read and follow flush data cache blocks (%d) from 0x%x 0x%x\n", mem_name[k], PPC476FP_L2_CACHELINE_SIZE, range_start, range_end);
        *start_signal = 0xBABA0001; 
        msync();
        for (addr = range_start, it =0; addr < range_end; addr += PPC476FP_L2_CACHELINE_SIZE, it++)
        {
            dcbt(0, (void *)addr);
            if(it % 4 ==0) msync(); // without msync test doesn't work: most dcbt are dropped due to DCU pipeline overflow !!!
        }
        msync();
        //-------------        
        rumboot_printf("Fill prefetched cache lines");
        *start_signal = 0xBABA0002;
        msync();
        for (int i=0; i < NUM_OF_WORDS; i++)  {
            for(int j=0;j < NUM_OF_ARRAYS; j++) {
                src[i+j*NUM_OF_WORDS] = i+j*NUM_OF_WORDS;
            }
        }        
        //-------------        
        *start_signal = 0xBABA0003;
        msync();
        for (addr = range_start; addr < range_end; addr += PPC476FP_L2_CACHELINE_SIZE)
        {
            dcbf((void *)addr);
        }
        //-------------        
        msync();
    }   
    rumboot_printf("End \n");
//***********************************************************************************************************************************************************************************************************
// ----- 
//***********************************************************************************************************************************************************************************************************    
    return true;
}
