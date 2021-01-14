/*
 * test_oi10_cpu_mem_016.c
 *
 *  Created on: Nov 21, 2018
 *      Author: s.chernousov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>

double test_15_data_array64[4] = {0};

#define MEM_CACHED_PAGE            SRAM0_BASE

typedef enum{
    EVENT_CHECK_MASK_MEM_A = TEST_EVENT_CODE_MIN + 0,
    EVENT_CHECK_MASK_MEM_B = TEST_EVENT_CODE_MIN + 1,
    EVENT_CHECK_MASK_MEM_C = TEST_EVENT_CODE_MIN + 2,
    EVENT_CHECK_MASK_MEM_D = TEST_EVENT_CODE_MIN + 3,
    EVENT_CHECK_MASK_MEM_E = TEST_EVENT_CODE_MIN + 4,
    EVENT_CHECK_MASK_MEM_F = TEST_EVENT_CODE_MIN + 5,
    EVENT_CHECK_MASK_MEM_G = TEST_EVENT_CODE_MIN + 6,
    EVENT_CHECK_MASK_MEM_H = TEST_EVENT_CODE_MIN + 7,
    TEST_EVENT_VERIFY      = TEST_EVENT_CODE_MIN + 8
}test_event_code;

typedef struct {
    uint32_t way;
    uint32_t index;
    uint32_t word;
}way_index_word;

typedef enum {
    SRAM_A,
    SRAM_B,
    SRAM_C,
    SRAM_D,
    SRAM_E,
    SRAM_F,
    SRAM_G,
    SRAM_H
}array_sram;

#define MODIFY_BYTE_BY_WAY_INDEX(WAY,INDEX, BYTE, VALUE) \
    iowrite8((uint8_t)(VALUE), MEM_CACHED_PAGE + ((WAY)<<13) + ((INDEX)<<5) + ((BYTE)<<0)); \
    msync();

#define REVERSE8(X) \
    ( \
    ((X & 0x01) << 7) | \
    ((X & 0x02) << 5) | \
    ((X & 0x04) << 3) | \
    ((X & 0x08) << 1) | \
    ((X & 0x10) >> 1) | \
    ((X & 0x20) >> 3) | \
    ((X & 0x40) >> 5) | \
    ((X & 0x80) >> 7) \
    )

#define A_TO_INDEX(A) \
    REVERSE8( (((A & 0xFC) >> 1) | (A & 0x1) ) << 1)

void fpu_enable (void)
{
    msr_write(msr_read() | (0b1 << ITRPT_XSR_FP_i));
}

void CACHE_DATA_LINE_BY_WAY_INDEX(uint32_t WAY, uint32_t INDEX, uint32_t WORD)
{
    uint32_t addr = MEM_CACHED_PAGE + (WAY<<13) + (INDEX<<5) + (WORD<<2);
    rumboot_printf("Caching data: way (0x%x), index (0x%x), word (0x%x), addr = 0x%x\n", WAY, INDEX, WORD, addr);
    /*Minimal caching size is 1 line = 32 bytes = 8 (WORD)s = 4 double(WORD)s*/
    iowrite64d(test_15_data_array64[0], addr);
    iowrite64d(test_15_data_array64[1], addr + 0x8);
    iowrite64d(test_15_data_array64[2], addr + 0x10);
    iowrite64d(test_15_data_array64[3], addr + 0x18);
    msync();
    /*1st read initiates a caching*/
    TEST_ASSERT (ioread64d(addr) == test_15_data_array64[0],"TEST ERROR: Compare failed at read of first (WORD)");
}

static inline void write_sram_with_mask(uint32_t wayF, uint32_t wayS,uint32_t index,uint32_t word,uint32_t value)
{
    MODIFY_BYTE_BY_WAY_INDEX(wayF,index,(word << 2)+0x0, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayF,index,(word << 2)+0x4, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayS,index,(word << 2)+0x0, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayS,index,(word << 2)+0x4, value);

    MODIFY_BYTE_BY_WAY_INDEX(wayF,index,(word << 2)+0x1, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayF,index,(word << 2)+0x5, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayS,index,(word << 2)+0x1, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayS,index,(word << 2)+0x5, value);

    MODIFY_BYTE_BY_WAY_INDEX(wayF,index,(word << 2)+0x2, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayF,index,(word << 2)+0x6, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayS,index,(word << 2)+0x2, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayS,index,(word << 2)+0x6, value);

    MODIFY_BYTE_BY_WAY_INDEX(wayF,index,(word << 2)+0x3, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayF,index,(word << 2)+0x7, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayS,index,(word << 2)+0x3, value);
    MODIFY_BYTE_BY_WAY_INDEX(wayS,index,(word << 2)+0x7, value);
}

way_index_word get_wiw(uint32_t A, array_sram sram)
{
    /*
     * ABCD: EVEN < 128, ODD >= 128
     * EFGH: ODD < 128, EVEN >= 128
     *
     * AB/EF: RA[27] = 0
     * CD/GH: RA[27] = 1
     *
     * A/C/E/G: RA[28] = 0
     * B/D/F/H: RA[28] = 1
     */
    bool DataEvenIndexHnL = 0;
    uint32_t way;
    uint32_t index;
    uint32_t word;  //RA[27:29]
    way_index_word wiw;

    DataEvenIndexHnL = ((A >> 1) & 0x1);

    if( (sram == SRAM_A) ||
        (sram == SRAM_B) ||
        (sram == SRAM_E) ||
        (sram == SRAM_F) )
        way = DataEvenIndexHnL ? 2 : 0;
    else
        way = DataEvenIndexHnL ? 0 : 2;


    index = A_TO_INDEX(A);
    if( (sram == SRAM_A) ||
        (sram == SRAM_B) ||
        (sram == SRAM_C) ||
        (sram == SRAM_D) )
    {
        if(index & 0x1)//if ODD
        {
            if(index < 128)
                index += 128;//ODD >=128
        }
        else
        {
            if(index >= 128)
                index -= 128;
        }
    }
    else
    {
        if(index & 0x1)//if ODD
        {
            if(index >= 128)
                index -= 128;//ODD >=128
        }
        else
        {
            if(index < 128)
                index += 128;
        }
    }

    /* AB/EF: RA[27] = 0
    * CD/GH: RA[27] = 1
    *
    * A/C/E/G: RA[28] = 0
    * B/D/F/H: RA[28] = 1
    * */
    //Index done. Get word
    switch(sram)
    {
        //                  27                          28        29
        case SRAM_A:
        case SRAM_E: word = (DataEvenIndexHnL << 2) | (0 << 1) | (0 << 0);
                     break;
        case SRAM_B:
        case SRAM_F: word = (DataEvenIndexHnL << 2) | (1 << 1) | (0 << 0);
                     break;
        case SRAM_C:
        case SRAM_G: word = (DataEvenIndexHnL << 2) | (0 << 1) | (0 << 0);
                     break;
        case SRAM_D:
        case SRAM_H: word = (DataEvenIndexHnL << 2) | (1 << 1) | (0 << 0);
                     break;

        default: word = 0; //never used
    }

    wiw.way = way;
    wiw.index = index;
    wiw.word = word;
    return wiw;
}

void run_bw(array_sram sram, uint32_t event)
{
    uint32_t addr[] = {
            0x00, //0
            0xFF, //2^n-1
            0x7F, //2^(n-1)-1
            0x80  //2^(n-1)
    };
    uint32_t i = 0;
    uint32_t wayF = 0; //first used way
    uint32_t wayS = 0; //second used way
    uint32_t index = 0;
    uint32_t word = 0;
    way_index_word wiw;
    for(i = 0; i < 4; i++)
    {
        wiw = get_wiw(addr[i], sram);
        wayF = wiw.way;
        wayS = wayF + 1;
        index = wiw.index;
        word = wiw.word;
        /*
         * 1. place in cache expected data sequence (zeros)
         */
        CACHE_DATA_LINE_BY_WAY_INDEX (0,index, 0);
        CACHE_DATA_LINE_BY_WAY_INDEX (1,index, 0);
        CACHE_DATA_LINE_BY_WAY_INDEX (2,index, 0);
        CACHE_DATA_LINE_BY_WAY_INDEX (3,index, 0);
        /*
         * 2. For each MASK
         */
        //write '111..1' & MASK
        test_event(event);
        write_sram_with_mask(wayF,wayS,index, word, 0xFF);
        //check '111..1' by hardware..
        /*
         * 3. For each MASK:
         */
        //write '000..0' & MASK
        test_event(event);
        write_sram_with_mask(wayF,wayS,index, word, 0x00);
        //check '000..0' by hardware..
    }
}

//                                      MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                  IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_CACHE_VALID           MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,     0b1,    0b0,    0b1,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

int main ( void )
{
    test_event_send_test_id("test_oi10_cpu_mem_015");

     rumboot_printf("Emi init\n");
     emi_init(DCR_EM2_EMI_BASE);
     fpu_enable();

     rumboot_printf("Init sram0\n");

     rumboot_printf("Set tlb\n");
     static const tlb_entry tlb_entry_cacheable_valid = {TLB_ENTRY_CACHE_VALID};
     write_tlb_entries(&tlb_entry_cacheable_valid, 1);

     rumboot_printf("Send event verify\n");
     test_event(TEST_EVENT_VERIFY);

     //START TEST
     rumboot_printf("Start test\n");
     run_bw(SRAM_A, EVENT_CHECK_MASK_MEM_A);
     run_bw(SRAM_B, EVENT_CHECK_MASK_MEM_B);
     run_bw(SRAM_C, EVENT_CHECK_MASK_MEM_C);
     run_bw(SRAM_D, EVENT_CHECK_MASK_MEM_D);
     run_bw(SRAM_E, EVENT_CHECK_MASK_MEM_E);
     run_bw(SRAM_F, EVENT_CHECK_MASK_MEM_F);
     run_bw(SRAM_G, EVENT_CHECK_MASK_MEM_G);
     run_bw(SRAM_H, EVENT_CHECK_MASK_MEM_H);

     return 0;
}
