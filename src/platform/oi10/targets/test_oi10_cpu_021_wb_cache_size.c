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

#define START_ADDR          (SRAM0_BASE + 0x10000)
#define DATA_SIZE           0x40000  //256KB
#define NUM_WORDS           (DATA_SIZE/sizeof(uint32_t))
#define GET_EXP_DATA(addr, inc)  ( ((addr - START_ADDR) >> 2)  * inc )

#define EVENT_START_CHECK_O_M_ADDR   TEST_EVENT_CODE_MIN + 0
#define EVENT_END_CHECK_O_M_ADDR     TEST_EVENT_CODE_MIN + 1

                                    //MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,  IL1D,   W,     I,     M,     G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,               V   )
#define TLB_ENTRY_CACHE_WB_VALID      MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,   0b1,    0b0,   0b0,   0b1,   0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,  0b1 )

static bool read_and_check (uint32_t const base_addr, uint32_t const size_b, uint32_t const data_inc) {
    for( uint32_t addr = base_addr; addr < base_addr+size_b; addr += sizeof(uint64_t) ) {
        msync();

        uint64_t rdata = ioread64(addr);
        if( rdata != (((uint64_t)GET_EXP_DATA(addr, data_inc) << 32) | (uint64_t)GET_EXP_DATA((addr+sizeof(uint32_t)), data_inc)) ) {
            rumboot_printf("ERROR: invalid read data\n");
            rumboot_printf("expected:\n");
            rumboot_printf("data_h = %x\n", (uint32_t)GET_EXP_DATA(addr, data_inc));
            rumboot_printf("data_l = %x\n", (uint32_t)GET_EXP_DATA((addr+sizeof(uint32_t)), data_inc));
            rumboot_printf("actual:\n");
            rumboot_printf("data_h = %x\n", (uint32_t)(rdata >> 32));
            rumboot_printf("data_l = %x\n\n", (uint32_t)(rdata & 0xFFFFFFFF));

            return false;
        }
        msync();

        //l2c get way
        int cache_way;
        uint64_t full_phys_addr = rumboot_virt_to_phys((void *) addr);
        uint32_t ext_phys_addr  = (uint32_t)(full_phys_addr >> 32);
        uint32_t phys_addr      = (uint32_t)(full_phys_addr & 0xFFFFFFFF);
        if( l2c_arracc_get_way_by_address( DCR_L2C_BASE, ext_phys_addr, phys_addr, &cache_way ) == false ) {
            rumboot_printf("==> Error reading (way) via L2ARRACC* <==\n");
            return false;
        }
        //l2c get data
        uint64_t l2c_data;
        if( l2c_arracc_data_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &l2c_data ) == false ) {
            rumboot_printf("==> Error reading (data) via L2ARRACC* <==\n");
            return false;
        }

        if( l2c_data != rdata ) {
            rumboot_printf("ERROR: invalid cache data\n");
            rumboot_printf("expected:\n");
            rumboot_printf("data_h = %x\n", (uint32_t)(rdata >> 32));
            rumboot_printf("data_l = %x\n", (uint32_t)(rdata & 0xFFFFFFFF) );
            rumboot_printf("actual:\n");
            rumboot_printf("l2c_data_h = %x\n", (uint32_t)(l2c_data >> 32));
            rumboot_printf("l2c_data_l = %x\n\n", (uint32_t)(l2c_data & 0xFFFFFFFF) );

            return false;
        }
    }
    return true;
}

bool __attribute__((section(".text.test"))) cache_testing_function()
{
    rumboot_printf ("Start testing function\n");

    rumboot_printf ("1. Write mem (pattern 1)\n");
    rumboot_memfill32( (void*)START_ADDR, NUM_WORDS + (L2C_LINE_SIZE/sizeof(uint32_t)), 0x00, PTRN_INC_1);
    msync();

    rumboot_printf ("Set tlb (WB)\n");
    static const tlb_entry sram0_tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_WB_VALID};
    write_tlb_entries(&sram0_tlb_entry_cacheable_valid, 1);
    dci(2); dci(0);

    rumboot_printf ("2. Read and check L2C Line Size\n");
    if ( !read_and_check (START_ADDR, L2C_LINE_SIZE, PTRN_INC_1)) return false;

    rumboot_printf ("3. Read and check L2C Line Size (check L2C.O_M_ADDR)\n");
    test_event(EVENT_START_CHECK_O_M_ADDR);
    if ( !read_and_check (START_ADDR, L2C_LINE_SIZE, PTRN_INC_1)) return false;
    test_event(EVENT_END_CHECK_O_M_ADDR);

    rumboot_printf ("4. Rewrite L2C Line Size (pattern 2)\n");
    for (uint32_t i = 0, d = 0x00; i < (L2C_LINE_SIZE/sizeof(uint32_t)); i++, d += PTRN_INC_2)
        iowrite32(d, START_ADDR + sizeof(uint32_t)*i);
    msync();

    rumboot_printf ("5. Read and check L2C Line Size (check L2C.O_M_ADDR)\n");
    test_event(EVENT_START_CHECK_O_M_ADDR);
    if ( !read_and_check (START_ADDR, L2C_LINE_SIZE, PTRN_INC_2)) return false;
    test_event(EVENT_END_CHECK_O_M_ADDR);

    rumboot_printf ("6. Rewrite L2C Line Size (pattern 1)\n");
    for (uint32_t i = 0, d = 0x00; i < (L2C_LINE_SIZE/sizeof(uint32_t)); i++, d += PTRN_INC_1)
        iowrite32(d, START_ADDR + sizeof(uint32_t)*i);
    msync();

    rumboot_printf ("7. Read and check mem, size = 0x%x\n", DATA_SIZE);
    if ( !read_and_check (START_ADDR, DATA_SIZE, PTRN_INC_1)) return false;

    rumboot_printf ("8. Read and check mem, size = 0x%x (check L2C.O_M_ADDR)\n", DATA_SIZE);
    test_event(EVENT_START_CHECK_O_M_ADDR);
    if ( !read_and_check (START_ADDR, DATA_SIZE, PTRN_INC_1)) return false;
    test_event(EVENT_END_CHECK_O_M_ADDR);

    rumboot_printf ("9. Rewrite mem (pattern 2)\n");
    for (uint32_t i = 0, d = 0x00; i < NUM_WORDS; i++, d += PTRN_INC_2)
        iowrite32(d, START_ADDR + sizeof(uint32_t)*i);
    msync();

    rumboot_printf ("10. Read and check mem, size = 0x%x (check L2C.O_M_ADDR)\n", DATA_SIZE);
    test_event(EVENT_START_CHECK_O_M_ADDR);
    if ( !read_and_check (START_ADDR, DATA_SIZE, PTRN_INC_2)) return false;
    test_event(EVENT_END_CHECK_O_M_ADDR);

    rumboot_printf ("11. Read and check new L2C Line Size\n");
    if ( !read_and_check ((START_ADDR + DATA_SIZE), L2C_LINE_SIZE, PTRN_INC_1)) return false;

    rumboot_printf ("12. Rewrite new L2C Line Size\n");
    for (uint32_t i = 0, d = GET_EXP_DATA((START_ADDR + DATA_SIZE), PTRN_INC_2); i < (L2C_LINE_SIZE/sizeof(uint32_t)); i++, d += PTRN_INC_2)
        iowrite32(d, (START_ADDR + DATA_SIZE) + sizeof(uint32_t)*i);
    msync();

    rumboot_printf ("13. Read and check new L2C Line Size\n");
    test_event(EVENT_START_CHECK_O_M_ADDR);
    if ( !read_and_check ((START_ADDR + DATA_SIZE), L2C_LINE_SIZE, PTRN_INC_2)) return false;
    test_event(EVENT_END_CHECK_O_M_ADDR);


    rumboot_printf ("Testing function completed\n");
    return true;
}
