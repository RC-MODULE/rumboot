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

#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <rumboot/memfill.h>


#define PTRN_INC_1 0x1
#define PTRN_INC_2 0x2
#define PTRN_INC_3 0x3

#define START_ADDR          (SRAM0_BASE + 256*1024) // skip code

#define GET_EXP_DATA(addr, inc)  ( (((addr - START_ADDR) / sizeof(uint32_t)) + 1) * inc )

#define EVENT_START_CHECK_O_M_ADDR   TEST_EVENT_CODE_MIN + 0
#define EVENT_END_CHECK_O_M_ADDR     TEST_EVENT_CODE_MIN + 1


static bool read_and_check(
        uint32_t const base_addr,
        uint32_t const size_b,
        uint32_t const data_inc
) {
    for( uint32_t addr = base_addr; addr < base_addr+size_b; addr += sizeof(uint64_t) ) {
        msync();

        uint64_t rdata = ioread64(addr);
        if( rdata != (((uint64_t)GET_EXP_DATA(addr, data_inc) << 32) | (uint64_t)GET_EXP_DATA((addr+sizeof(uint32_t)), data_inc)) ) {
            rumboot_printf( "ERROR: invalid read data\n" );
            rumboot_printf( "expected: data_h=%x, data_l=%x\n", (uint32_t)GET_EXP_DATA(addr, data_inc), (uint32_t)GET_EXP_DATA((addr+sizeof(uint32_t)), data_inc) );
            rumboot_printf( "actual: data_h=%x, data_l=%x\n\n", (uint32_t)(rdata >> 32), (uint32_t)(rdata & 0xFFFFFFFF) );

            return false;
        }
        msync();

        //l2c get way
        int cache_way;
        uint64_t full_phys_addr = rumboot_virt_to_phys((void *) addr);
        uint32_t ext_phys_addr  = (uint32_t)(full_phys_addr >> 32);
        uint32_t phys_addr      = (uint32_t)(full_phys_addr & 0xFFFFFFFF);
        if( !l2c_arracc_get_way_by_address( DCR_L2C_BASE, ext_phys_addr, phys_addr, &cache_way ) ) {
            rumboot_printf( "==> Error reading (way) via L2ARRACC* <==\n" );
            return false;
        }
        //l2c get data
        uint64_t l2c_data;
        if( !l2c_arracc_data_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &l2c_data ) ) {
            rumboot_printf( "==> Error reading (data) via L2ARRACC* <==\n" );
            return false;
        }

        if( l2c_data != rdata ) {
            rumboot_printf( "ERROR: invalid cache data\n" );
            rumboot_printf( "expected: data_h=%x, data_l=%x\n", (uint32_t)(rdata >> 32), (uint32_t)(rdata & 0xFFFFFFFF) );
            rumboot_printf( "actual: l2c_data_h=%x, l2c_data_l=%x\n\n", (uint32_t)(l2c_data >> 32), (uint32_t)(l2c_data & 0xFFFFFFFF) );

            return false;
        }
    }
    return true;
}

bool __attribute__((section(".text.test"))) cache_testing_function() {
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

    rumboot_printf( "1. Write mem (pattern 1)\n" );
    rumboot_memfill32( (void*)START_ADDR, check_words_num+line_words_num, GET_EXP_DATA(START_ADDR, PTRN_INC_1), PTRN_INC_1 );
    msync();

    rumboot_printf( "Set tlb (WB)\n" );
    static tlb_entry const wb_tlb_entries[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,  IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )},
        /* code noncacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00010,    0x00010,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00020,    0x00020,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00030,    0x00030,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        /* test data l2 cacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x00040,    0x00040,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00050,    0x00050,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00060,    0x00060,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00070,    0x00070,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        /* test data noncacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x00080,    0x00080,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x00090,    0x00090,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x000A0,    0x000A0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x000B0,    0x000B0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    write_tlb_entries( wb_tlb_entries, ARRAY_SIZE(wb_tlb_entries) );

    rumboot_printf( "Invalidate L2, L1D\n" );
    dci(2); dci(0);
    msync();

    rumboot_printf( "2. Read and check L2C Line Size\n" );
    if( !read_and_check (START_ADDR, L2C_LINE_SIZE, PTRN_INC_1) ) return false;

    rumboot_printf( "3. Read and check L2C Line Size (check L2C.O_M_ADDR)\n" );
    test_event(EVENT_START_CHECK_O_M_ADDR);
    if( !read_and_check (START_ADDR, L2C_LINE_SIZE, PTRN_INC_1) ) return false;
    test_event(EVENT_END_CHECK_O_M_ADDR);

    rumboot_printf( "4. Rewrite L2C Line Size (pattern 2)\n" );
    for( uint32_t i = 0, d = GET_EXP_DATA(START_ADDR, PTRN_INC_2); i < line_words_num; i++, d += PTRN_INC_2 ) {
        iowrite32(d, START_ADDR + sizeof(uint32_t)*i);
    }
    msync();

    rumboot_printf(  "5. Read and check L2C Line Size (check L2C.O_M_ADDR)\n" );
    test_event(EVENT_START_CHECK_O_M_ADDR);
    if( !read_and_check (START_ADDR, L2C_LINE_SIZE, PTRN_INC_2) ) return false;
    test_event(EVENT_END_CHECK_O_M_ADDR);

    rumboot_printf( "6. Rewrite L2C Line Size (pattern 1)\n");
    for( uint32_t i = 0, d = GET_EXP_DATA(START_ADDR, PTRN_INC_1); i < line_words_num; i++, d += PTRN_INC_1 ) {
        iowrite32(d, START_ADDR + sizeof(uint32_t)*i);
    }
    msync();

    rumboot_printf( "7. Read and check mem, size = 0x%x\n", l2c_check_size );
    if( !read_and_check (START_ADDR, l2c_check_size, PTRN_INC_1) ) return false;

    rumboot_printf( "8. Read and check mem, size = 0x%x (check L2C.O_M_ADDR)\n", l2c_check_size );
    test_event(EVENT_START_CHECK_O_M_ADDR);
    if( !read_and_check (START_ADDR, l2c_check_size, PTRN_INC_1) ) return false;
    test_event(EVENT_END_CHECK_O_M_ADDR);

    rumboot_printf( "9. Rewrite mem (pattern 2)\n" );
    for( uint32_t i = 0, d = GET_EXP_DATA(START_ADDR, PTRN_INC_2); i < check_words_num; i++, d += PTRN_INC_2 ) {
        iowrite32(d, START_ADDR + sizeof(uint32_t)*i);
    }
    msync();

    rumboot_printf( "10. Read and check mem, size = 0x%x (check L2C.O_M_ADDR)\n", l2c_check_size );
    test_event(EVENT_START_CHECK_O_M_ADDR);
    if( !read_and_check (START_ADDR, l2c_check_size, PTRN_INC_2) ) return false;
    test_event(EVENT_END_CHECK_O_M_ADDR);

    rumboot_printf( "11. Read and check new L2C Line Size\n" );
    if( !read_and_check ((START_ADDR + l2c_check_size), L2C_LINE_SIZE, PTRN_INC_1) ) return false;

    rumboot_printf( "12. Rewrite new L2C Line Size\n");
    for( uint32_t i = 0, d = GET_EXP_DATA((START_ADDR + l2c_check_size), PTRN_INC_2); i < line_words_num; i++, d += PTRN_INC_2 ) {
        iowrite32(d, (START_ADDR + l2c_check_size) + sizeof(uint32_t)*i);
    }
    msync();

    rumboot_printf( "13. Read and check new L2C Line Size\n" );
    test_event(EVENT_START_CHECK_O_M_ADDR);
    if( !read_and_check ((START_ADDR + l2c_check_size), L2C_LINE_SIZE, PTRN_INC_2) ) return false;
    test_event(EVENT_END_CHECK_O_M_ADDR);


    rumboot_printf( "Testing function completed\n" );
    return true;
}
