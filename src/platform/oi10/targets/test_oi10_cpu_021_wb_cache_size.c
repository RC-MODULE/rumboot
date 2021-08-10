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
#include <arch/mem_window.h>
#include <devices/ugly/emi.h>
#include <platform/devices/l2c.h>
#include <arch/dma2plb6.h>
#include <rumboot/memfill.h>


#define PTRN_INC_1  0x1
#define PTRN_INC_2  0x2
#define PTRN_INC_3  0x3

#define PAGE_SIZE           (256*1024)
#define CODE_ADDR           (SRAM0_BASE)
#define CACHEABLE_ADDR1     (CODE_ADDR + PAGE_SIZE)
#define UNCACHEABLE_ADDR    (CACHEABLE_ADDR1 + PAGE_SIZE)
#define CACHEABLE_ADDR2     (UNCACHEABLE_ADDR + PAGE_SIZE)

#define GET_EXP_DATA(base_addr, addr, inc)  ( (((addr - base_addr) / sizeof(uint32_t)) + 1) * inc )


static bool read_and_check(
        uint32_t const base_addr,
        uint32_t const size_b,
        uint32_t const data_inc,
        bool const check_l2c
) {
    for( uint32_t addr = base_addr; addr < base_addr+size_b; addr += sizeof(uint64_t) ) {
        msync();

        uint64_t rdata = ioread64(addr);
        uint32_t exp_data_h = GET_EXP_DATA(base_addr, addr, data_inc);
        uint32_t exp_data_l = GET_EXP_DATA(base_addr, (addr+sizeof(uint32_t)), data_inc);
        if( rdata != (((uint64_t)exp_data_h << 32) | (uint64_t)exp_data_l) ) {
            rumboot_printf( "ERROR: invalid read data\n" );
            rumboot_printf( "expected: data_h=%x, data_l=%x\n", (uint32_t)exp_data_h, exp_data_l );
            rumboot_printf( "actual: data_h=%x, data_l=%x\n\n", (uint32_t)(rdata >> 32), (uint32_t)(rdata & 0xFFFFFFFF) );

            return false;
        }
        msync();

        if( check_l2c ) {
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
    }
    return true;
}


static bool dma2plb6_copy(
        uint32_t const src,
        uint32_t const dst,
        uint32_t const count
) {
    struct dma2plb6_setup_info dma_info = {
        .base_addr              = DCR_DMAPLB6_BASE,
        .source_adr             = rumboot_virt_to_phys( (void*)src ),
        .dest_adr               = rumboot_virt_to_phys( (void*)dst ),
        .priority               = priority_medium_low,
        .striding_info.striding = striding_none,
        .tc_int_enable          = false,
        .err_int_enable         = false,
        .int_mode               = int_mode_level_wait_clr,
        .channel                = channel0,
        .transfer_width         = tr_width_quadword,
        .rw_transfer_size       = rw_tr_size_8q,
        .count                  = count/16
    };

    struct channel_status status = {};
    if( !dma2plb6_single_copy( &dma_info, &status ) ) {
        dma2plb6_trace_error_status(&status);
        return false;
    }
    return true;
}

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
    rumboot_memfill32( (void*)CACHEABLE_ADDR1, check_words_num, PTRN_INC_1, PTRN_INC_1 );
    msync();

    rumboot_printf( "Set tlb (WB) (4a)\n" );
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
        {MMU_TLB_ENTRY(  0x000,  0x000B0,    0x000B0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        /* test data l2 cacheable page */
        {MMU_TLB_ENTRY(  0x000,  0x000C0,    0x000C0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x000D0,    0x000D0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x000E0,    0x000E0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x000F0,    0x000F0,    MMU_TLBE_DSIZ_64KB,     0b1,   0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    write_tlb_entries( wb_tlb_entries, ARRAY_SIZE(wb_tlb_entries) );

    rumboot_printf( "Invalidate L2, L1D\n" );
    dci(2); dci(0);
    msync();

    rumboot_printf( "2. Read and check cacheable memory 1 L2C Line Size (pattern 1)\n" );
    if( !read_and_check( CACHEABLE_ADDR1, L2C_LINE_SIZE, PTRN_INC_1, true ) ) return false;

    rumboot_printf( "3. Write direct to cacheable memory 1 L2C Line Size (pattern 1)\n" );
    rumboot_memfill32( (void*)UNCACHEABLE_ADDR, line_words_num, PTRN_INC_3, PTRN_INC_3 );
    if( !dma2plb6_copy( UNCACHEABLE_ADDR, CACHEABLE_ADDR1, L2C_LINE_SIZE ) ) return false;
    rumboot_printf( "3a. Read and check cacheable memory 1 L2C Line Size (pattern 1)\n" );
    if( !read_and_check( CACHEABLE_ADDR1, L2C_LINE_SIZE, PTRN_INC_1, true ) ) return false;
    rumboot_printf( "3b. Invalidate L2C. Reread and check cacheable memory 1 L2C Line Size (pattern 3)\n" );
    dci( 2 );
    if( !read_and_check( CACHEABLE_ADDR1, L2C_LINE_SIZE, PTRN_INC_3, true ) ) return false;

    rumboot_printf( "4. Rewrite cacheable memory 1 L2C Line Size (pattern 2)\n" );
    for( uint32_t i = 0, d = PTRN_INC_2; i < line_words_num; i++, d += PTRN_INC_2 ) {
        iowrite32(d, CACHEABLE_ADDR1 + sizeof(uint32_t)*i);
    }
    msync();
    rumboot_printf( "4b. Copy direct cacheable memory 1 to uncacheable memory L2C Line Size (pattern 3)\n" );
    if( !dma2plb6_copy( CACHEABLE_ADDR1, UNCACHEABLE_ADDR, L2C_LINE_SIZE ) ) return false;

    rumboot_printf( "5. Read and check cacheable memory 1 L2C Line Size (pattern 2). Read and check uncacheable memory L2C Line Size (pattern 3)\n" );
    if( !read_and_check( CACHEABLE_ADDR1, L2C_LINE_SIZE, PTRN_INC_2, true ) ) return false;
    if( !read_and_check( UNCACHEABLE_ADDR, L2C_LINE_SIZE, PTRN_INC_3, false ) ) return false;

    rumboot_printf( "6. Rewrite cacheable memory 1 L2C Line Size (pattern 1)\n");
    for( uint32_t i = 0, d = PTRN_INC_1; i < line_words_num; i++, d += PTRN_INC_1 ) {
        iowrite32(d, CACHEABLE_ADDR1 + sizeof(uint32_t)*i);
    }
    msync();

    rumboot_printf( "7. Read and check cacheable memory 1, size = 0x%x (pattern 1)\n", l2c_check_size );
    if( !read_and_check( CACHEABLE_ADDR1, l2c_check_size, PTRN_INC_1, true ) ) return false;

    rumboot_printf( "8. Rewrite direct cacheable memory 1, size = 0x%x (pattern 3)\n", l2c_check_size );
    rumboot_memfill32( (void*)UNCACHEABLE_ADDR, check_words_num, PTRN_INC_3, PTRN_INC_3 );
    if( !dma2plb6_copy( UNCACHEABLE_ADDR, CACHEABLE_ADDR1, l2c_check_size ) ) return false;
    rumboot_printf( "8a. Read and check cacheable memory 1, size = 0x%x (pattern 1)\n", l2c_check_size );
    if( !read_and_check( CACHEABLE_ADDR1, l2c_check_size, PTRN_INC_1, true ) ) return false;
    rumboot_printf( "8b. Invalidate L2C. Reread and check cacheable memory 1, size = 0x%x (pattern 3)\n", l2c_check_size );
    dci( 2 );
    if( !read_and_check( CACHEABLE_ADDR1, l2c_check_size, PTRN_INC_3, true ) ) return false;

    rumboot_printf( "9. Write, read and check cacheable memory 2 L2C Line Size (pattern 1)\n");
    rumboot_memfill8_modelling( (void*)CACHEABLE_ADDR2, l2c_check_size, 0, 0 );
    for( uint32_t i = 0, d = PTRN_INC_1; i < line_words_num; i++, d += PTRN_INC_1 ) {
        iowrite32(d, CACHEABLE_ADDR2 + sizeof(uint32_t)*i);
    }
    msync();
    if( !read_and_check( CACHEABLE_ADDR2, L2C_LINE_SIZE, PTRN_INC_1, true ) ) return false;

    rumboot_printf( "10. Write direct to cacheable memory 2 L2C Line Size (pattern 3)\n" );
    rumboot_memfill32( (void*)UNCACHEABLE_ADDR, line_words_num, PTRN_INC_3, PTRN_INC_3 );
    if( !dma2plb6_copy( UNCACHEABLE_ADDR, CACHEABLE_ADDR2, L2C_LINE_SIZE ) ) return false;
    rumboot_printf( "Flash cacheable memory 2 L2C Line Size (pattern 1). Reread and check cacheable memory 2 L2C Line Size (pattern 1)\n" );
    dcbf( (void*)CACHEABLE_ADDR2 );
    if( !read_and_check( CACHEABLE_ADDR2, L2C_LINE_SIZE, PTRN_INC_1, true ) ) return false;
    if( !dma2plb6_copy( CACHEABLE_ADDR2, UNCACHEABLE_ADDR, L2C_LINE_SIZE ) ) return false;
    if( !read_and_check( UNCACHEABLE_ADDR, L2C_LINE_SIZE, PTRN_INC_1, false ) ) return false;

    rumboot_printf( "Testing function completed\n" );
    return true;
}
