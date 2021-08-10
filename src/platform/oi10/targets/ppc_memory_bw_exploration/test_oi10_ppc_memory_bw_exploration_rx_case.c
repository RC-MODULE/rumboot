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
#include <arch/ppc_476fp_itrpt_fields.h>
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
#define CACHEABLE_SSRAM     (SSRAM_BASE + PAGE_SIZE)
#define CACHEABLE_SRAM1     (SRAM1_BASE)
#define CACHEABLE_IM0       (IM0_BASE + 64*1024)
#define CACHEABLE_IM1       (IM1_BASE + 64*1024)
#define PPC476FP_L2_CACHELINE_SIZE 128u 
//0x60000000
#define GET_EXP_DATA(base_addr, addr, inc)  ( (((addr - base_addr) / sizeof(uint32_t)) + 1) * inc )

#ifndef READ_SIZE
    #define READ_SIZE 4
#endif
#ifndef NUM_OF_BUFS
    #define NUM_OF_BUFS 10
#endif
#ifndef SIZE_OF_BUFS
    #define SIZE_OF_BUFS 4096
#endif
#ifndef SIMPLE_CRC
    #define SIMPLE_CRC 1
#endif
#ifndef NUM_OF_ITERATION
    #define NUM_OF_ITERATION 1
#endif


bool __attribute__((section(".text.test"))) cache_testing_function( void ) {
//#ifdef L2C_CHECK_SIZE
//    uint32_t const l2c_check_size = L2C_CHECK_SIZE;
//#else
//    struct l2c_mem_layout l2c_layout;
//    l2c_get_mem_layout( DCR_L2C_BASE, &l2c_layout );
//    uint32_t const l2c_size_in_bytes[4] = {
//        [L2C_L2Size_128KB] = 128*1024,
//        [L2C_L2Size_256KB] = 256*1024,
//        [L2C_L2Size_512KB] = 512*1024,
//        [L2C_L2Size_1MB] = 1*1024*1024
//    };
//    uint32_t const l2c_check_size = l2c_size_in_bytes[l2c_layout.l2size];
//#endif
    
    uint32_t const l2c_check_size = NUM_OF_BUFS*SIZE_OF_BUFS;
    uint32_t const check_words_num  = l2c_check_size / READ_SIZE;
    uint32_t const line_words_num = L2C_LINE_SIZE / sizeof(uint32_t);
    uint32_t k = -1;
    uint32_t stack_h;

    rumboot_printf( "Start testing function\n" );

    stack_h = __builtin_frame_address(0);
    rumboot_printf( "Stack = %x\n", stack_h );
    
#ifdef SRAM0_CACHE
    rumboot_printf( "Init CACHEABLE_SRAM0: addr = %x, size = %d\n", CACHEABLE_SRAM0, check_words_num);
    rumboot_memfill32( (void*)CACHEABLE_SRAM0, check_words_num, 0x01010101, 0);
    k = 0;
#else    
    #ifdef SSRAM_CACHE
        rumboot_printf( "Init CACHEABLE_SSRAM: addr = %x, size = %d\n", CACHEABLE_SSRAM, check_words_num);
        rumboot_memfill32( (void*)CACHEABLE_SSRAM, check_words_num, 0x01010101, 0);
        k = 1;
    #else
        #ifdef SRAM1_CACHE
            rumboot_printf( "Init CACHEABLE_SRAM1: addr = %x, size = %d\n", CACHEABLE_SRAM1, check_words_num);
            rumboot_memfill32( (void*)CACHEABLE_SRAM1, check_words_num, 0x01010101, 0);
            k = 2;
            uint32_t tmp = dcr_read(DCR_EM2_EMI_BASE+0x20);
            rumboot_printf( "Read SSi: tmp = %x\n", tmp);
            tmp = tmp & (0xFFFFE1FF);
            rumboot_printf( "Reset SSi: tmp = %x\n", tmp);
            tmp = tmp | (0xB<<9);
            rumboot_printf( "Modified SSi: tmp = %x\n", tmp);
            dcr_write(DCR_EM2_EMI_BASE+0x20, tmp);
        #else
            #ifdef IM0_CACHE
                k = 3;
                rumboot_printf( "Init CACHEABLE_IM0: k =%d,  addr = %x, size = %d\n", k, CACHEABLE_IM0, check_words_num);
                if(check_words_num > (48*1024/4)) {
                    rumboot_printf( "ERROR: size of initialized IM0 exceeds safe value\n" );
                    return false;
                }
                rumboot_memfill32( (void*)CACHEABLE_IM0, check_words_num, 0x01010101, 0);
            #else    
                #ifdef IM1_CACHE
                    rumboot_printf( "Init CACHEABLE_IM1: addr = %x, size = %d\n", CACHEABLE_IM1, check_words_num);
                    if(check_words_num > (48*1024/4)) {
                        rumboot_printf( "ERROR: size of initialized IM1 exceeds safe value\n" );
                        return false;
                    }
                    rumboot_memfill32( (void*)CACHEABLE_IM1, check_words_num, 0x01010101, 0);
                    k = 4;
                #else
                    rumboot_printf( "Unknown type of memory\n" );
                    return false;
                #endif
            #endif
        #endif
    #endif
#endif



  
    
    msync();

    rumboot_printf( "Set tlb (WB) (4a)\n" );
    static tlb_entry const wb_tlb_entries[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,  IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
    // ---------- re-define for IM0
        {MMU_TLB_ENTRY(  0x010,  0x80010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b0 )},
        {MMU_TLB_ENTRY(  0x010,  0x8001C,    0x8001C,    MMU_TLBE_DSIZ_16KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
        
        {MMU_TLB_ENTRY(  0x010,  0x80010,    0x80010,    MMU_TLBE_DSIZ_16KB,     0b1,   0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
        {MMU_TLB_ENTRY(  0x010,  0x80014,    0x80014,    MMU_TLBE_DSIZ_16KB,     0b1,   0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
        {MMU_TLB_ENTRY(  0x010,  0x80018,    0x80018,    MMU_TLBE_DSIZ_16KB,     0b1,   0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
    // ------ re-define for IM1
        {MMU_TLB_ENTRY(  0x020,  0xC0010,    0x80030,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_4,     0b0 )}, // remove old 
        {MMU_TLB_ENTRY(  0x020,  0xC001C,    0x8003C,    MMU_TLBE_DSIZ_16KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
        
        {MMU_TLB_ENTRY(  0x020,  0xC0010,    0x80030,    MMU_TLBE_DSIZ_16KB,     0b1,   0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
        {MMU_TLB_ENTRY(  0x020,  0xC0014,    0x80034,    MMU_TLBE_DSIZ_16KB,     0b1,   0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
        {MMU_TLB_ENTRY(  0x020,  0xC0018,    0x80038,    MMU_TLBE_DSIZ_16KB,     0b1,   0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
        
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
        {MMU_TLB_ENTRY(  0x000,  0x40070,    0x40070,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
//*******IM0************************************************************************************************************************************************************************************************
        /* test data l2 cacheable page */
      //{MMU_TLB_ENTRY(  0x010,  0x80010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},                
      //{MMU_TLB_ENTRY(  0x010,  0x80018,    0x80018,    MMU_TLBE_DSIZ_32KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},        
//*******SRAM1************************************************************************************************************************************************************************************************
        /* test data l2 cacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x60000,    0x60000,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x60010,    0x60010,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x60020,    0x60020,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x60030,    0x60030,    MMU_TLBE_DSIZ_64KB,     0b1,   0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}               
    };

    write_tlb_entries( wb_tlb_entries, ARRAY_SIZE(wb_tlb_entries) );
//***********************************************************************************************************************************************************************************************************
    rumboot_printf( "Invalidate L2, L1D\n" );
    dci(2); dci(0);
    msync();
//***********************************************************************************************************************************************************************************************************
    #if READ_SIZE==1
        uint8_t* src;
    #elif READ_SIZE==4
        uint32_t* src;
    #elif READ_SIZE==8
        uint64_t* src;
    #else
        rumboot_printf("Undefined READ_SIZE\n");
        return false;
    #endif
    uint32_t  it;
    uint32_t* volatile __attribute__((used)) start_signal;
    uint32_t mem_base[] = {CACHEABLE_SRAM0, CACHEABLE_SSRAM, CACHEABLE_SRAM1, CACHEABLE_IM0, CACHEABLE_IM1};
    char mem_name[][20] = {"CACHEABLE_SRAM0", "CACHEABLE_SSRAM", "CACHEABLE_SRAM1", "CACHEABLE_IM0", "CACHEABLE_IM1"};
    unsigned long range_start;
    unsigned long range_end;
    unsigned long addr;
    unsigned long mask_line_base;
    unsigned long start_addr;
    unsigned long end_addr;
    uint32_t src_prnt_l, src_prnt_b, crc;
    uint64_t src_p, stime[2], ftime[2], time_diff;
//***********************************************************************************************************************************************************************************************************
// ----- EMI CACHED MEM TESTS    
//***********************************************************************************************************************************************************************************************************
    /// -------- !!!!!!!!!!!
    #ifdef MC_DISABLE
        uint32_t const msr_old_value = msr_read();
        rumboot_printf("msr_old_value = %x\n", msr_old_value);
        rumboot_printf("msr_new_value = %x\n", msr_old_value & ~(0b1 << ITRPT_XSR_ME_i));
        msr_write( msr_old_value & ~(0b1 << ITRPT_XSR_ME_i));   // disable machine check
    #endif
    
        
    //time = (uint64_t)spr_read(SPR_TBL_R) + (uint64_t)spr_read(SPR_TBU_R);
        for (it=0; it < NUM_OF_ITERATION; it++)  {
            crc = 0;
            rumboot_printf("****** Start iteration N = %d************\n", it);
            //--------
            src = mem_base[k];
            src_p = rumboot_virt_to_phys(src);
            src_prnt_l = src_p;
            src_prnt_b = src_p >> 32;
            //-------------
            //*start_signal = 0xBABA0000;
            msync();
            //stime[k] = (uint64_t)spr_read(SPR_TBL_R) + (uint64_t)spr_read(SPR_TBU_R);
            rumboot_printf("%s: read data from RX-buffers (size = %d) by word_size = %d located at address 0x%x (0x%x%x) and pseudo-calculate CRC  \n", mem_name[k], SIZE_OF_BUFS, READ_SIZE, src, src_prnt_b, src_prnt_l);
            for (int i=0; i < NUM_OF_BUFS; i++)  {
                //rumboot_printf("%s: read data from %d-s RX-buffer (size = %d) by word_size = %d located at address 0x%x (0x%x%x) and pseudo-calculate CRC  \n", mem_name[k], i, SIZE_OF_BUFS, READ_SIZE, src, src_prnt_b, src_prnt_l);
            // *start_signal = *start_signal + i;
            // --- invalidate L2-cache data blocks cpntains RX-buffer
                msync(); 
                start_addr = (unsigned long)src;
                end_addr = start_addr + (SIZE_OF_BUFS-1);
                // Устанавливаем адрес на первое слово в строке (это необязательно делать)
                mask_line_base = (~(PPC476FP_L2_CACHELINE_SIZE - 1));
                range_start = start_addr & mask_line_base;
                range_end = (end_addr & mask_line_base) + PPC476FP_L2_CACHELINE_SIZE;
                for (addr = range_start; addr < range_end; addr += PPC476FP_L2_CACHELINE_SIZE)
                {
                    dcbi((void *)addr);
                }
            // --- read RX-buffer and calculate CRC
                msync();
                for(int j=0;j < SIZE_OF_BUFS/READ_SIZE; j++) {
                    for(int l=0; l < READ_SIZE; l++) {
                        #if SIMPLE_CRC==0
                            crc = crc + (src[j]>>(8*l) & 0x0000000F);// pseudo-calculation 
                        #else
                            if(src[j] != 0x01010101) {
                                rumboot_printf("DATA_MISSMATCH: src[j] =0x%x , ref_value = 0x01010101\n", src[j]);
                                return false;
                            }
                            crc = crc + src[j];
                        #endif
                    }
                }
                src+=SIZE_OF_BUFS/READ_SIZE;
            }
            //ftime[k] = (uint64_t)spr_read(SPR_TBL_R) + (uint64_t)spr_read(SPR_TBU_R);
            rumboot_printf("CRC of all buffer: %x\n", crc);
        }
   //for (int k=0; k<2; k++){
   //     time_diff = ftime[0] - stime[0];
   //     src_prnt_l = time_diff;
   //     src_prnt_b = time_diff >> 32;
   //     rumboot_printf("%s: spent time is %d%d\n", mem_name[k], src_prnt_b, src_prnt_l);
   //}
   
   return true;
}
