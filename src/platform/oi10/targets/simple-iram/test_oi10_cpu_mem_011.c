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

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <devices/ugly/plb6mcif2.h>
#include <devices/ugly/emi.h>
#include <regs/regs_emi.h>
#include <regs/fields/emi.h>


/* Addresses (config) */
#define MEM_BASE                    SSRAM_BASE
#define PHY_BASE                    MEM_BASE
#define DCU_BASE                    0xFFF80000
#define TRANSFER_DST                PHY_BASE
#define TRANSFER_SRC                cache_fill_data

/* Constants */
#define NOTHING                     0x00000000
#define TEST_OK                     NOTHING
#define TEST_ERROR                  !TEST_OK
#define DISABLED                    NOTHING
#define ENABLED                     !DISABLED
#define L1_LINES                    256
#define L1_LINE_SIZE                32
#define L1_LINE_WORDS               (L1_LINE_SIZE / sizeof(uint32_t))
#define USED_CACHE_LINES            12
#define L1_WAYS                     4
#define L1_WAY_DATA_SIZE            (L1_LINE_SIZE * L1_LINES)
#define L1_ALIGN_LINES              (L1_LINES - USED_CACHE_LINES)
#define L1_ALIGN_BYTES              (L1_ALIGN_LINES * L1_LINE_SIZE)
#define L1_ALIGN_WORDS              (L1_ALIGN_BYTES / sizeof(uint32_t))
#define USED_WAY_SPACE              (USED_CACHE_LINES * L1_LINE_SIZE)

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
#define D_W0L4      0xEEEEEEEE,0xEEEEEEEE,0xEEEEEEEE,0xEEEEEEEE,    \
                    0xEEEEEEEE,0xEEFFFFFF,0xFFFFFFFF,0xFFFFFFFF
#define D_W0L5      0xEEEEEEEE,0xEEEEEEEE,0xEEEEEEEE,0xEEEEEEEE,    \
                    0x77777777,0x77777777,0x77777777,0x77777777
#define D_W0L6      0x77777777,0x77777777,0x77777777,0x77777777,    \
                    0x77777777,0x77777777,0x77777777,0x77777777
#define D_W0L7      0x77777777,0x77777777,0x77777777,0x77777777,    \
                    0xEEEEEEEE,0xEEEEEEEE,0xEEEEEEEE,0xEEEEEEEE
#define D_W0L8      0x08040201,0x08040201,0x08040201,0x08040201,    \
                    0x08040201,0x08040201,0x08040201,0x08040201					
/* Way 1 */
#define D_W1L0      0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,    \
                    0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000
#define D_W1L1      0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,    \
                    0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000
#define D_W1L2      0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,    \
                    0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF
#define D_W1L3      0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,    \
                    0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF
#define D_W1L4      0xEEEEEEEE,0x77777777,0xEEEEEEEE,0x77777777,    \
                    0xEEEE7777,0xEEEE7777,0xEEEE7777,0xEEEE7777
#define D_W1L5      0xEEEEEEEE,0x77777777,0xEEEEEEEE,0x77777777,    \
                    0xEEEE7777,0xEEEE7777,0xEEEE7777,0xEEEE7777
#define D_W1L6      0x77777777,0xEEEEEEEE,0x77777777,0xEEEEEEEE,    \
                    0x7777EEEE,0x7777EEEE,0x7777EEEE,0x7777EEEE
#define D_W1L7      0x77777777,0xEEEEEEEE,0x77777777,0xEEEEEEEE,    \
                    0x7777EEEE,0x7777EEEE,0x7777EEEE,0x7777EEEE
/* Way 2 */
#define D_W2L0      0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,    \
                    0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0
#define D_W2L1      0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,    \
                    0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0
#define D_W2L2      0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,    \
                    0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F
#define D_W2L3      0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,    \
                    0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F
#define D_W2L4      0xEE77EE77,0xEE77EE77,0xEE77EE77,0xEE77EE77,    \
                    0xE7E7E7E7,0xE7E7E7E7,0xE7E7E7E7,0xE7E7E7E7
#define D_W2L5      0xEE77EE77,0xEE77EE77,0xEE77EE77,0xEE77EE77,    \
                    0xE7E7E7E7,0xE7E7E7E7,0xE7E7E7E7,0xE7E7E7E7
#define D_W2L6      0x11881188,0x11881188,0x11881188,0x11881188,    \
                    0x18181818,0x18181818,0x18181818,0x18181818
#define D_W2L7      0x11881188,0x11881188,0x11881188,0x11881188,    \
                    0x18181818,0x18181818,0x18181818,0x18181818
/* Way 3 */
#define D_W3L0      0x33333333,0x33333333,0x33333333,0x33333333,    \
                    0x55555555,0x55555555,0x55555555,0x55555555
#define D_W3L1      0x33333333,0x33333333,0x33333333,0x33333333,    \
                    0x55555555,0x55555555,0x55555555,0x55555555
#define D_W3L2      0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,    \
                    0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA
#define D_W3L3      0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,    \
                    0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA
#define D_W3L4      0x11111111,0x11111111,0x11111111,0x11111111,    \
                    0xEEEEEEEE,0xEEEEEEEE,0xEEEEEEEE,0xEEEEEEEE
#define D_W3L5      0x77777777,0x77777777,0x77777777,0x77777777,    \
                    0x88888888,0x88888888,0x88888888,0x88888888
#define D_W3L6      0x11111111,0x11111111,0x11111111,0x11111111,    \
                    0x88888888,0x88888888,0x88888888,0x88888888
#define D_W3L7      0xEEEEEEEE,0xEEEEEEEE,0xEEEEEEEE,0xEEEEEEEE,    \
                    0x77777777,0x77777777,0x77777777,0x77777777

/* Compound cache ways data */
#define DATA_WAY_0  {D_W0L0},{D_W0L1},{D_W0L2},{D_W0L3},    \
                    {D_W0L4},{D_W0L5},{D_W0L6},{D_W0L7},    \
                    {D_W0L8},{D_W0L8},{D_W0L8},{D_W0L8}
					
#define DATA_WAY_1  {D_W1L0},{D_W1L1},{D_W1L2},{D_W1L3},    \
                    {D_W1L4},{D_W1L5},{D_W1L6},{D_W1L7},    \
					{D_W0L8},{D_W0L8},{D_W0L8},{D_W0L8}
#define DATA_WAY_2  {D_W2L0},{D_W2L1},{D_W2L2},{D_W2L3},    \
                    {D_W2L4},{D_W2L5},{D_W2L6},{D_W2L7},    \
					{D_W0L8},{D_W0L8},{D_W0L8},{D_W0L8}
#define DATA_WAY_3  {D_W3L0},{D_W3L1},{D_W3L2},{D_W3L3},    \
                    {D_W3L4},{D_W3L5},{D_W3L6},{D_W3L7},    \
                    {D_W0L8},{D_W0L8},{D_W0L8},{D_W0L8}

/* Functions */
#define WORDS2BYTES(WORDS)              ((WORDS) << 2)
#define PHY2RPN20(ADDR)                 (((ADDR) & 0xFFFFF000) >> 12)
#define ADDR2EPN                        PHY2RPN20
#define WAY_OFFSET(WAY)                 (L1_WAY_DATA_SIZE * (WAY))
#define CALC_REL_ADDR(A1,A2)            ((A2) - (A1))
                                        /* WA(Y), INDE(X), WOR(D)*/
#define CALC_ADDR(Y,X,D)                ((((Y) & 0x03) << 0x0D) |           \
                                         (((X) & 0xFF) << 0x05) |           \
                                         (((D) & 0x07) << 0x02))
/* Make transfer info entry
 * (struct transfer_info_t) */
#define MK_TRF_ENTRY(DWAY,SWAY)         {(void*)(TRANSFER_DST               \
                                            + WAY_OFFSET(DWAY)),            \
                                         (void*)(TRANSFER_SRC[SWAY]),       \
                                         L1_WAY_DATA_SIZE, USED_WAY_SPACE}

/* Types and structures definitions */

struct transfer_info_t
{
    void        *dst;   /* Destination address  */
    void        *src;   /* Source      address  */
    uint32_t     asz;   /* Area size            */
    uint32_t     dsz;   /* Data size            */
};

typedef uint32_t cache_fill_t[L1_WAYS][USED_CACHE_LINES][L1_LINE_WORDS];
typedef uint32_t cache_data_t[L1_WAYS][L1_LINES        ][L1_LINE_WORDS];

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
/* uint32_t    cache_fill_data[L1_WAYS][USED_CACHE_LINES][L1_LINE_WORDS] = */
cache_fill_t    cache_fill_data =
{
    {DATA_WAY_0},
    {DATA_WAY_1},
    {DATA_WAY_2},
    {DATA_WAY_3}
};

static const
struct transfer_info_t cache_transfer_info[L1_WAYS][L1_WAYS] =
{
    {
        MK_TRF_ENTRY(0,0),
        MK_TRF_ENTRY(1,1),
        MK_TRF_ENTRY(2,2),
        MK_TRF_ENTRY(3,3)
    },
    {
        MK_TRF_ENTRY(0,1),
        MK_TRF_ENTRY(1,2),
        MK_TRF_ENTRY(2,3),
        MK_TRF_ENTRY(3,0)
    },
    {
        MK_TRF_ENTRY(0,2),
        MK_TRF_ENTRY(1,3),
        MK_TRF_ENTRY(2,0),
        MK_TRF_ENTRY(3,1)
    },
    {
        MK_TRF_ENTRY(0,3),
        MK_TRF_ENTRY(1,0),
        MK_TRF_ENTRY(2,1),
        MK_TRF_ENTRY(3,2)
    },
};

void dcu_transfer_data(const struct transfer_info_t *trfinfo)
{
    /* a4a - address for align, s4a - size for align */
    uint32_t    a4a = (uint32_t)(trfinfo->dst) + trfinfo->dsz,
                s4a = trfinfo->asz - trfinfo->dsz;
    static char memcpy_msg[] =
                    " + Copy of %d bytes cache data from 0x%X"
                    " to cached RAM at 0x%X...\n",
                memset_msg[] =
                    " - Fill by zero %d bytes in cached RAM at 0x%X...\n";
    rumboot_printf(memcpy_msg, trfinfo->dsz,
            (uint32_t)(trfinfo->src), (uint32_t)(trfinfo->dst));
    memcpy(trfinfo->dst, trfinfo->src, trfinfo->dsz);
    rumboot_printf(memset_msg, s4a, a4a);
    memset((void*)(a4a), NOTHING, s4a);

}

uint32_t dcu_check_data(cache_fill_t *s, cache_data_t *t)
{
    uint32_t     result = TEST_OK,
                 status = 0,
                 way    = 0,
                 line   = 0,
                 word   = 0,
                 readed = 0,
                 needed = 0;
    volatile
    uint32_t     cached = 0;

    dci(0);
    for(way = 0; way < L1_WAYS; way++)
    {
        rumboot_printf("*** Way %d/%d ***\n", way, L1_WAYS - 1);
        for(line = 0; line < USED_CACHE_LINES; line++)
        {
            rumboot_printf(" + Line %d/%d...\n",
                    line, USED_CACHE_LINES - 1);
            for(word = 0; word < L1_LINE_WORDS; word++)
            {
                needed = (*s)[way][line][word];
                cached = (*t)[way][line][word];
                readed = dcread(CALC_ADDR(way,line,word));
                isync();
                result = (needed == cached);
                status |= !result;
                if(!result)
                    rumboot_printf("At way:%d,line:%d,word:%d -->\n",
                            way,line,word);
                TEST_ASSERT(result,"Invalid data in cached memory!\n");
                result = (needed == readed);
                status |= !result;
                if(!result)
                    rumboot_printf("At way:%d,line:%d,word:%d -->\n",
                            way,line,word);
                TEST_ASSERT(result,"Invalid data in DCU memory!\n");
            }
        }
    }
    rumboot_printf("DCU check data %s!\n",
            (!status)?"success":"failed");
    return status;
}

uint32_t main(void)
{
    uint32_t         status = TEST_OK,
                     result = 0,
                     seq    = 0,
                     way    = 0;

    rumboot_putstring("Test started...\n");

    result ^=0;
    rumboot_printf("Init EMI...\n");
    emi_init(DCR_EM2_EMI_BASE);
    rumboot_printf("Init done.\n");

    write_tlb_entries(cached_mirror_entries, 1);

    rumboot_printf(
        "Transfer template data to cached memory at 0x%X...\n",
        TRANSFER_DST);
    for(seq = 0; seq < L1_WAYS; seq++)
    {
        rumboot_printf(" +++ Sequence #%d:\n", seq);
        for(way = 0; way < L1_WAYS; way++)
            dcu_transfer_data(&cache_transfer_info[seq][way]);
        rumboot_printf("Transfer done.\n");

        status |= dcu_check_data((void*)(TRANSFER_SRC),(void*)(DCU_BASE));
    }

    rumboot_printf("TEST %s!\n", !status ? "OK" : "ERROR");

    return status;
}

