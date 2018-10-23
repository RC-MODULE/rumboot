/*
 * test_oi10_cpu_mem_011.c
 *
 *  Created on: Oct 18, 2018
 *      Author: a.gurov
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RUMBOOT_ASSERT_WARN_ONLY

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

#include <platform/common_macros/common_macros.h>

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>


/* Addresses (config) */
#define MEM_BASE                    SSRAM_BASE
#define PHY_BASE                    MEM_BASE
#define DCU_BASE                    0xFFF80000

/* Constants */
#define NOTHING                     0x00000000
#define TEST_OK                     NOTHING
#define TEST_ERROR                  !TEST_OK
#define DISABLED                    NOTHING
#define ENABLED                     !DISABLED
#define L1_LINES                    256
#define L1_LINE_SIZE                32
#define USED_CACHE_LINES            8
#define L1_WAYS                     4
#define L1_WAY_DATA_SIZE            (L1_LINE_SIZE * L1_LINES)
#define L1_ALIGN_LINES              (L1_LINES - USED_CACHE_LINES)
#define L1_ALIGN_BYTES              (L1_ALIGN_LINES * L1_LINE_SIZE)
#define L1_ALIGN_WORDS              (L1_ALIGN_BYTES / sizeof(uint32_t))
#define USED_WAY_SPACE              (USED_CACHE_LINES * L1_LINE_SIZE)

#define EM0_CACHED_PAGE             0x80000000
#define EM0_NOT_CACHED_PAGE         0x40000000

#define MAY_BE_NOT_USED             __attribute__((unused))

/* Cache template fill data */
/* Way 0 */
#define D_W0L0      0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,    \
                    0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
#define D_W0L1      0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,    \
                    0x00000000,0x00000000,0x00000000,0x00000000
#define D_W0L2      0x00000000,0x00000000,0x00000000,0x00000000,    \
                    0x00000000,0x00000000,0x00000000,0x00000000
#define D_W0L3      0x00000000,0x00000000,0x00000000,0x00000000,    \
                    0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
#define D_W0L4      0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,    \
                    0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
#define D_W0L5      0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,    \
                    0x00000000,0x00000000,0x00000000,0x00000000
#define D_W0L6      0x00000000,0x00000000,0x00000000,0x00000000,    \
                    0x00000000,0x00000000,0x00000000,0x00000000
#define D_W0L7      0x00000000,0x00000000,0x00000000,0x00000000,    \
                    0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
/* Way 1 */
#define D_W1L0      0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,    \
                    0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000
#define D_W1L1      0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,    \
                    0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000
#define D_W1L2      0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,    \
                    0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF
#define D_W1L3      0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,    \
                    0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF
#define D_W1L4      0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,    \
                    0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000
#define D_W1L5      0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,    \
                    0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000
#define D_W1L6      0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,    \
                    0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF
#define D_W1L7      0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,    \
                    0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF
/* Way 2 */
#define D_W2L0      0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,    \
                    0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0
#define D_W2L1      0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,    \
                    0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0
#define D_W2L2      0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,    \
                    0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F
#define D_W2L3      0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,    \
                    0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F
#define D_W2L4      0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,    \
                    0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0
#define D_W2L5      0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,    \
                    0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0
#define D_W2L6      0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,    \
                    0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F
#define D_W2L7      0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,    \
                    0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F
/* Way 3 */
#define D_W3L0      0x33333333,0x33333333,0x33333333,0x33333333,    \
                    0x55555555,0x55555555,0x55555555,0x55555555
#define D_W3L1      0x33333333,0x33333333,0x33333333,0x33333333,    \
                    0x55555555,0x55555555,0x55555555,0x55555555
#define D_W3L2      0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,    \
                    0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA
#define D_W3L3      0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,    \
                    0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA
#define D_W3L4      0x33333333,0x33333333,0x33333333,0x33333333,    \
                    0x55555555,0x55555555,0x55555555,0x55555555
#define D_W3L5      0x33333333,0x33333333,0x33333333,0x33333333,    \
                    0x55555555,0x55555555,0x55555555,0x55555555
#define D_W3L6      0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,    \
                    0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA
#define D_W3L7      0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,    \
                    0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA

/* Compound cache ways data */
#define DATA_WAY_0  {{D_W0L0}},{{D_W0L1}},{{D_W0L2}},{{D_W0L3}},    \
                    {{D_W0L4}},{{D_W0L5}},{{D_W0L6}},{{D_W0L7}}
#define DATA_WAY_1  {{D_W1L0}},{{D_W1L1}},{{D_W1L2}},{{D_W1L3}},    \
                    {{D_W1L4}},{{D_W1L5}},{{D_W1L6}},{{D_W1L7}}
#define DATA_WAY_2  {{D_W2L0}},{{D_W2L1}},{{D_W2L2}},{{D_W2L3}},    \
                    {{D_W2L4}},{{D_W2L5}},{{D_W2L6}},{{D_W2L7}}
#define DATA_WAY_3  {{D_W3L0}},{{D_W3L1}},{{D_W3L2}},{{D_W3L3}},    \
                    {{D_W3L4}},{{D_W3L5}},{{D_W3L6}},{{D_W3L7}}


/* Functions */
#define WORDS2BYTES(WORDS)              ((WORDS) << 2)
#define CAST_ADDR(VAL)                  ((void*)    (VAL))
#define CAST_UINT(VAL)                  ((uint32_t) (VAL))
#define CAST_FUNC(VAL)                  ((void(*)())(VAL));
#define PHY2RPN20(ADDR)                 (((ADDR) & 0xFFFFF000) >> 12)
#define ADDR2EPN                        PHY2RPN20
#define WAY_OFFSET(WAY)                 (L1_WAY_DATA_SIZE * (WAY))
#define CALC_REL_ADDR(A1,A2)            ((A2) - (A1))
#define CALC_ADDR(WAY,INDEX,OFFT)       (((WAY)   << 13) | \
                                         ((INDEX) << 5 ) | \
                                         ((OFFT)  << 2 )   \
                                        )
#define MK_TRF_ENTRY(WAY)               {CAST_ADDR(DCU_BASE                 \
                                            + WAY_OFFSET(WAY)),             \
                                         CAST_ADDR(cache_fill_data[WAY]),   \
                                         USED_WAY_SPACE, L1_ALIGN_BYTES}


#define CACHE_DATA_LINE_BY_WAY_INDEX(WAY,INDEX,WORD,DATA_OFFSET)    \
    trace_msg("Caching data: WAY | INDEX \n"); \
    trace_hex(WAY);\
    trace_hex(INDEX);\
    /*Minimal caching size is 1 line = 32 bytes = 8 words = 4 doublewords*/ \
    MEM64(EM0_CACHED_PAGE + (WAY<<23) + (INDEX<<5) + (WORD<<2)) = test_11_data_array64[DATA_OFFSET]; \
    MEM64(EM0_CACHED_PAGE + (WAY<<23) + (INDEX<<5) + ((WORD+2)<<2)) = test_11_data_array64[DATA_OFFSET+1]; \
    MEM64(EM0_CACHED_PAGE + (WAY<<23) + (INDEX<<5) + ((WORD+4)<<2)) = test_11_data_array64[DATA_OFFSET+2]; \
    MEM64(EM0_CACHED_PAGE + (WAY<<23) + (INDEX<<5) + ((WORD+6)<<2)) = test_11_data_array64[DATA_OFFSET+3]; \
    msync(); \
    isync(); \
    /*1st read initiates a caching*/ \
    COMPARE_VALUES(MEM32(EM0_CACHED_PAGE+ (WAY<<23) + (INDEX<<5) + ((WORD+0)<<2)),test_11_data_array32[DATA_OFFSET*2],"Compare failed at read of first word");

#define REV8(V)         ((((V) & 0x01) << 7)    |   \
                         (((V) & 0x02) << 5)    |   \
                         (((V) & 0x04) << 3)    |   \
                         (((V) & 0x08) << 1)    |   \
                         (((V) & 0x10) >> 1)    |   \
                         (((V) & 0x20) >> 3)    |   \
                         (((V) & 0x40) >> 5)    |   \
                         (((V) & 0x80) >> 7))
#define A_TO_INDEX_EVEN(A) \
    REVERSE8( (((A & 0xFC) >> 1) | (A & 0x1) ) << 1)
#define A_TO_INDEX_ODD(A) \
    (0x80+REVERSE8( (((A & 0xFC) >> 1) | (A & 0x1) ) << 1))

#define CACHE_LINE_BY_WAY_ADDR_EVEN(WAY,A,DATA_OFFSET) \
    trace_msg("Sram.A=");\
    trace_hex(A);\
    CACHE_DATA_LINE_BY_WAY_INDEX((WAY),(A_TO_INDEX_EVEN((A))),0,(DATA_OFFSET))
#define CACHE_LINE_BY_WAY_ADDR_ODD(WAY,A,DATA_OFFSET) \
    trace_msg("Sram.A=");\
    trace_hex(A);\
    CACHE_DATA_LINE_BY_WAY_INDEX((WAY),(A_TO_INDEX_ODD((A))),0,(DATA_OFFSET))


/* Types and structures definitions */
typedef struct
{
    uint32_t    word[L1_LINE_SIZE / sizeof(uint32_t)];
} cache_line_t;

typedef struct
{
    void        *dst;   /* Destination address  */
    void        *src;   /* Destination address  */
    uint32_t     asz;   /* Area size            */
    uint32_t     dsz;   /* Data size            */
} transfer_info_t;

/* Variables declarations */

static const
tlb_entry cached_mirror_entries[] =
{       /*             ERPN,  RPN,                 EPN  */
        {MMU_TLB_ENTRY(0x000, PHY2RPN20(PHY_BASE), ADDR2EPN(DCU_BASE),
                MMU_TLBE_DSIZ_64KB, /* DSIZ             */
        /*      IL1I,IL1D,   W,I,M,G, E                 */
                0,   0,      1,0,1,0, MMU_TLBE_E_BIG_END,
        /*      UX,UW,UR, SX,SW,SR, DULXE,IULXE,TS      */
                0, 0, 0,  1, 1, 1,  0,    0,    0,
        /*      TID,         WAY,            BID,               V */
                MEM_WINDOW_0,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1)},
};

static const
MAY_BE_NOT_USED
cache_line_t    cache_fill_data[L1_WAYS][USED_CACHE_LINES] =
{
    {DATA_WAY_0},
    {DATA_WAY_1},
    {DATA_WAY_2},
    {DATA_WAY_3}
};

static const
MAY_BE_NOT_USED
transfer_info_t cache_transfer_info[L1_WAYS] =
{
        MK_TRF_ENTRY(0),
        MK_TRF_ENTRY(1),
        MK_TRF_ENTRY(2),
        MK_TRF_ENTRY(3)
};

MAY_BE_NOT_USED
void dcu_transfer_data(transfer_info_t *trfinfo)
{
    /* align address */
    uint32_t    aa = CAST_UINT(trfinfo->dst) + trfinfo->dsz;
    rumboot_printf(
            " + Copy of %d bytes cache data from 0x%X"
            " to cached RAM at 0x%X...\n",
            trfinfo->dsz,
            CAST_UINT(trfinfo->src),
            CAST_UINT(trfinfo->dsz));
    memcpy(trfinfo->dst, trfinfo->src, trfinfo->dsz);
    rumboot_printf(
            " - Fill by zero %d bytes in cached RAM at 0x%X...\n",
            trfinfo->asz - trfinfo->dsz, aa);
    memset(CAST_ADDR(aa), NOTHING, trfinfo->asz - trfinfo->dsz);

}

uint32_t main(void)
{
    uint32_t         status = TEST_OK,
                     result = 0,
                     way    = 0;

    /* Cached Way Data Pointers*/
    uintptr_t        cwdp[L1_WAYS];

    rumboot_putstring("Test started...\n");

    result ^=0;
    rumboot_printf("Init EMI...\n");
    emi_init(DCR_EM2_EMI_BASE);
    rumboot_printf("Init done.\n");

    write_tlb_entries(cached_mirror_entries, 1);


    for(way = 0; way < 4; way++)
    {
        cwdp[way] = DCU_BASE + WAY_OFFSET(way);
        rumboot_printf(
                " + Copy of %d bytes cache data from 0x%X"
                " to cached RAM at 0x%X...\n",
                USED_WAY_SPACE,
                CAST_UINT(cache_fill_data[way]),
                cwdp[way]);
        memcpy( CAST_ADDR(cwdp[way]),
                CAST_ADDR(cache_fill_data[way]),
                USED_WAY_SPACE);
        rumboot_printf(
                " - Fill by zero %d bytes in cached RAM at 0x%X...\n",
                L1_ALIGN_BYTES,
                cwdp[way] + USED_WAY_SPACE);
        memset( CAST_ADDR(cwdp[way] + USED_WAY_SPACE),
                NOTHING, L1_ALIGN_BYTES);
    }


    /*
     * Write 8 cache lines
     * index = INDEX field = Set in cache
     * pointer = test data offset
     */
    MAY_BE_NOT_USED
    void cache_0_abcd(uint32_t addr, uint32_t test_data_pointer)
    {
    #if DISABLED
        rumboot_printf("Caching PPC0.SRAM A,B,C,D (Even index < 128)\n");
        CACHE_LINE_BY_WAY_ADDR_EVEN(0,addr+0,test_data_pointer + 0x00);  /*AB A=00, CD A=02, BW MSB*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(1,addr+0,test_data_pointer + 0x04);  /*AB A=00, CD A=02, BW LSB*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(0,addr+1,test_data_pointer + 0x08);  /*AB A=01, CD A=03, BW MSB*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(1,addr+1,test_data_pointer + 0x0C);  /*AB A=01, CD A=03, BW LSB*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(2,addr+2,test_data_pointer + 0x10);  /*AB A=02, CD A=00, BW MSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(3,addr+2,test_data_pointer + 0x14);  /*AB A=02, CD A=00, BW LSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(2,addr+3,test_data_pointer + 0x18);  /*AB A=03, CD A=01, BW MSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(3,addr+3,test_data_pointer + 0x1C);  /*AB A=03, CD A=01, BW LSB => EvenIndexHighNotLow=1*/
    #endif
    }

    MAY_BE_NOT_USED
    void cache_1_abcd(uint32_t addr, uint32_t test_data_pointer)
    {
    #if DISABLED
        rumboot_printf("Caching PPC1.SRAM A,B,C,D (Even index < 128)\n");
        CACHE_LINE_BY_WAY_ADDR_EVEN(0,addr+0,test_data_pointer + 0x00);  /*AB A=00, CD A=02, BW MSB*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(1,addr+0,test_data_pointer + 0x04);  /*AB A=00, CD A=02, BW LSB*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(0,addr+1,test_data_pointer + 0x08);  /*AB A=01, CD A=03, BW MSB*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(1,addr+1,test_data_pointer + 0x0C);  /*AB A=01, CD A=03, BW LSB*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(2,addr+2,test_data_pointer + 0x10);  /*AB A=02, CD A=00, BW MSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(3,addr+2,test_data_pointer + 0x14);  /*AB A=02, CD A=00, BW LSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(2,addr+3,test_data_pointer + 0x18);  /*AB A=03, CD A=01, BW MSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_EVEN(3,addr+3,test_data_pointer + 0x1C);  /*AB A=03, CD A=01, BW LSB => EvenIndexHighNotLow=1*/
    #endif
    }

    MAY_BE_NOT_USED
    void cache_0_efgh(uint32_t addr, uint32_t test_data_pointer)
    {
    #if DISABLED
        rumboot_printf("Caching PPC0.SRAM E,F,G,H (Even index >= 128)\n");
        CACHE_LINE_BY_WAY_ADDR_ODD(0,addr+0,test_data_pointer + 0x00);  /*AB A=00, CD A=02, BW MSB*/
        CACHE_LINE_BY_WAY_ADDR_ODD(1,addr+0,test_data_pointer + 0x04);  /*AB A=00, CD A=02, BW LSB*/
        CACHE_LINE_BY_WAY_ADDR_ODD(0,addr+1,test_data_pointer + 0x08);  /*AB A=01, CD A=03, BW MSB*/
        CACHE_LINE_BY_WAY_ADDR_ODD(1,addr+1,test_data_pointer + 0x0C);  /*AB A=01, CD A=03, BW LSB*/
        CACHE_LINE_BY_WAY_ADDR_ODD(2,addr+2,test_data_pointer + 0x10);  /*AB A=02, CD A=00, BW MSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_ODD(3,addr+2,test_data_pointer + 0x14);  /*AB A=02, CD A=00, BW LSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_ODD(2,addr+3,test_data_pointer + 0x18);  /*AB A=03, CD A=01, BW MSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_ODD(3,addr+3,test_data_pointer + 0x1C);  /*AB A=03, CD A=01, BW LSB => EvenIndexHighNotLow=1*/
    #endif
    }

    MAY_BE_NOT_USED
    void cache_1_efgh(uint32_t addr, uint32_t test_data_pointer)
    {
    #if DISABLED
        rumboot_printf("Caching PPC1.SRAM E,F,G,H (Even index >= 128)\n");
        CACHE_LINE_BY_WAY_ADDR_ODD(0,addr+0,test_data_pointer + 0x00);  /*AB A=00, CD A=02, BW MSB*/
        CACHE_LINE_BY_WAY_ADDR_ODD(1,addr+0,test_data_pointer + 0x04);  /*AB A=00, CD A=02, BW LSB*/
        CACHE_LINE_BY_WAY_ADDR_ODD(0,addr+1,test_data_pointer + 0x08);  /*AB A=01, CD A=03, BW MSB*/
        CACHE_LINE_BY_WAY_ADDR_ODD(1,addr+1,test_data_pointer + 0x0C);  /*AB A=01, CD A=03, BW LSB*/
        CACHE_LINE_BY_WAY_ADDR_ODD(2,addr+2,test_data_pointer + 0x10);  /*AB A=02, CD A=00, BW MSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_ODD(3,addr+2,test_data_pointer + 0x14);  /*AB A=02, CD A=00, BW LSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_ODD(2,addr+3,test_data_pointer + 0x18);  /*AB A=03, CD A=01, BW MSB => EvenIndexHighNotLow=1*/
        CACHE_LINE_BY_WAY_ADDR_ODD(3,addr+3,test_data_pointer + 0x1C);  /*AB A=03, CD A=01, BW LSB => EvenIndexHighNotLow=1*/
    #endif
    }

    // write valid non-cacheable
    /*
    write_tlb_entries(&em0_tlb_entry_non_cacheable_valid,1);
    if((SPR_PIR_read() == 0) ||                                     //it's PPC0
    ( (SPR_PIR_read() == 1) && (get_slp_status() & 0x1) ))      //it's PPC1 and PPC0 is sleep
    {
     trace_msg("Start ddr init\n");
     ddr_init_impl( DdrHlbId_Em0,
             DdrInitMode_ViolateSpecWaitRequirements,
             DdrEccMode_Off,
             DdrPmMode_DisableAllPowerSavingFeatures,
             DdrBurstLength_4,
             DdrPartialWriteMode_ReadModifyWrite);
     DDR_INIT_DONE = true;
     trace_msg("ddr initialized\n");
    }
    else
     wait_ddr_init_done();
    //write invalid non-cacheable
    write_tlb_entries(&em0_tlb_entry_non_cacheable_invalid,1);
    //write valid cacheable
    write_tlb_entries(&em0_tlb_entry_cacheable_valid,1);
    msync();
    isync();
    */

    //START TEST
    /*
    if(SPR_PIR_read() == 0)
    {
     cache_0_abcd(0x00,test_data_pointer+0x00);
     cache_0_abcd(0x04,test_data_pointer+0x20);
     cache_0_abcd(0x08,test_data_pointer+0x40);
     cache_0_abcd(0x0C,test_data_pointer+0x60);

     cache_0_efgh(0x00,test_data_pointer+0x00);
     cache_0_efgh(0x04,test_data_pointer+0x20);
     cache_0_efgh(0x08,test_data_pointer+0x40);
     cache_0_efgh(0x0C,test_data_pointer+0x60);
    }
    else
    {
     cache_1_efgh(0x00,test_data_pointer+0x00);
     cache_1_efgh(0x04,test_data_pointer+0x20);
     cache_1_efgh(0x08,test_data_pointer+0x40);
     cache_1_efgh(0x0C,test_data_pointer+0x60);

     cache_1_abcd(0x00,test_data_pointer+0x00);
     cache_1_abcd(0x04,test_data_pointer+0x20);
     cache_1_abcd(0x08,test_data_pointer+0x40);
     cache_1_abcd(0x0C,test_data_pointer+0x60);
    }
    */
    // test_event(EVENT_CACHING_DONE);

    rumboot_printf("TEST %s!\n", !status ? "OK" : "ERROR");

    return status;
}



