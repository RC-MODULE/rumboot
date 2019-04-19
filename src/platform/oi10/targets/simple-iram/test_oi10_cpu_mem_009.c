/*
 * test_oi10_cpu_mem_009.c
 *
 *  Created on: Oct 12, 2018
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
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>


/* Addresses (config) */
#define MEM_BASE                        SSRAM_BASE
#define PHY_BASE                        MEM_BASE
#define ICU_BASE                        0xFFF80000

/* Constants */
#define NOTHING                         0x00000000
#define TEST_OK                         NOTHING
#define TEST_ERROR                      !TEST_OK

#define EM0_CACHED_PAGE                 0x40000000
#define EM0_NOT_CACHED_PAGE             0x40000000
#define ICU_MEM_SIZE                    32768

#define B0_RECORDS                      16
#define B3_RECORDS                      16
#define USED_INDEXES                    (B0_RECORDS + B3_RECORDS)

/* Functions */
#define EVENT_CACHING_DONE              (TEST_EVENT_CODE_MIN + 0)
#define WORDS2BYTES(WORDS)              ((WORDS) << 2)
#define PHY2RPN20(ADDR)                 (((ADDR) & 0xFFFFF000) >> 12)
#define ADDR2EPN                        PHY2RPN20


#define WBR0(OP,W0,W1,W2,W3,W4,W5,W6)   {OP,W0,W1,W2,W3,W4,W5,W6}
#define WBR3(OP,W0,W1,W2,W3,W4,W5,W6)   {W0,W1,W2,OP,W3,W4,W5,W6}

#define CALC_REL_ADDR(A1,A2)            ((A2) - (A1))
#define CALC_ADDR(WAY,INDEX,OFFT)       (((WAY)   << 13) | \
                                         ((INDEX) << 5 ) | \
                                         ((OFFT)  << 2 )   \
                                        )
#define CALC_FILL(W0,I0,O0, W1,I1,O1)   (                               \
                                            (CALC_ADDR(W1,I1,O1) -      \
                                             CALC_ADDR(W0,I0,O0)) >> 2  \
                                        )
#define NEXT_WAY_OFFSET                 (CALC_FILL(0x00,0x1F,0x03, 0x01,0x00,0x00))
#define FILL_OFFSET                     (CALC_FILL(0x00,0x20,0x00, 0x01,0x00,0x00))


/* Macros for branch instruction synthesis */
#define ADDR26MASK                      0x03FFFFFC
/* + Flags */
#define LK                              0x00000001  /* LinKed branch              */
#define AA                              0x00000002  /* Absolute Address           */
#define RA                              0x00000000  /* Relative Address           */
#define RALK                            (RA | LK)   /* Absolute Address with LinK */
#define AALK                            (AA | LK)   /* Relative Address with LinK */
#define B_OP                            0x48000000
#define BLR_OP                          0x4E800020
#define PPC_BRANCH(ADDR,FLAGS)          (B_OP | ((ADDR) & ADDR26MASK) | ((FLAGS) & 3))
/* + Branch to next */
#define B2NEXTI                         PPC_BRANCH(WORDS2BYTES(8              ),RA)
#define B2NEXT3                         PPC_BRANCH(WORDS2BYTES(8 + 3          ),RA)
#define B2NEXTW                         PPC_BRANCH(WORDS2BYTES(NEXT_WAY_OFFSET),RA)

#define PRINT_SIZEOF(T)                 rumboot_printf("sizeof(%s)=%d\n", #T, sizeof(T))

#define WBR0_DATA                                                                               \
    WBR0(B2NEXTI,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF), \
    WBR0(B2NEXTI,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000), \
    WBR0(B2NEXTI,0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000), \
    WBR0(B2NEXTI,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF), \
    WBR0(B2NEXTI,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000), \
    WBR0(B2NEXTI,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF), \
    WBR0(B2NEXTI,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000), \
    WBR0(B2NEXTI,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF), \
    WBR0(B2NEXTI,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00), \
    WBR0(B2NEXTI,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF), \
    WBR0(B2NEXTI,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0), \
    WBR0(B2NEXTI,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F), \
    WBR0(B2NEXTI,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC), \
    WBR0(B2NEXTI,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333), \
    WBR0(B2NEXTI,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA), \
    WBR0(B2NEXT3,0x05050505,0x05050505,0x05050505,0x05050505,0x05050505,0x05050505,0x05050505)
#define WBR3_DATA                                                                               \
    WBR3(B2NEXTI,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF), \
    WBR3(B2NEXTI,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000), \
    WBR3(B2NEXTI,0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000), \
    WBR3(B2NEXTI,0x00000000,0xFFFFFFFF,0xFFFFFFFF,0x00000000,0x00000000,0xFFFFFFFF,0xFFFFFFFF), \
    WBR3(B2NEXTI,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000), \
    WBR3(B2NEXTI,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF,0x00000000,0xFFFFFFFF), \
    WBR3(B2NEXTI,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000,0xFFFF0000), \
    WBR3(B2NEXTI,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF,0x0000FFFF), \
    WBR3(B2NEXTI,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00,0xFF00FF00), \
    WBR3(B2NEXTI,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF,0x00FF00FF), \
    WBR3(B2NEXTI,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0), \
    WBR3(B2NEXTI,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F,0x0F0F0F0F), \
    WBR3(B2NEXTI,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC,0xCCCCCCCC), \
    WBR3(B2NEXTI,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333,0x33333333), \
    WBR3(B2NEXTI,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA), \
    WBR3(B2NEXTW,0x05050505,0x05050505,0x05050505,0x05050505,0x05050505,0x05050505,0x05050505)

#define WAY_FILL_DATA                   {.b0 = {WBR0_DATA},.b3 = {WBR3_DATA}}


typedef struct
{
    uint32_t    op;
    uint32_t    w0;
    uint32_t    w1;
    uint32_t    w2;
    uint32_t    w3;
    uint32_t    w4;
    uint32_t    w5;
    uint32_t    w6;
} icu_words_b0; /* Internal structure */

typedef struct
{
    uint32_t    w0;
    uint32_t    w1;
    uint32_t    w2;
    uint32_t    op;
    uint32_t    w3;
    uint32_t    w4;
    uint32_t    w5;
    uint32_t    w6;
} icu_words_b3; /* Internal structure */

typedef struct
{
    icu_words_b0    b0[B0_RECORDS];
    icu_words_b3    b3[B3_RECORDS];
    uint32_t        fill[FILL_OFFSET];
} icu_way_data_t;

typedef void(*icu_chain_func)();

/* Variables declarations */
static const
tlb_entry cached_mirror_entries[] =
{       /*             ERPN,  RPN,                 EPN  */
        {MMU_TLB_ENTRY(0x000, PHY2RPN20(PHY_BASE), ADDR2EPN(ICU_BASE),
                MMU_TLBE_DSIZ_64KB, /* DSIZ             */
        /*      IL1I,IL1D,   W,I,M,G, E                 */
                0,   0,      1,0,1,0, MMU_TLBE_E_BIG_END,
        /*      UX,UW,UR, SX,SW,SR, DULXE,IULXE,TS      */
                0, 0, 0,  1, 1, 1,  0,    0,    0,
        /*      TID,         WAY,            BID,               V */
                MEM_WINDOW_0,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1)},
};

icu_way_data_t __attribute__((aligned(256))) icu_data[4] =
{
    WAY_FILL_DATA,
    WAY_FILL_DATA,
    WAY_FILL_DATA,
    WAY_FILL_DATA
};
/* ---------------------- */

uint32_t icu_check(icu_way_data_t *ways_data)
{
    uint32_t         result = TEST_OK,
                     needed = 0,
                     readed = 0;
    uint32_t        *data   = (uint32_t*)ways_data;
    int              y,x,d; /* wa(y), inde(x), wor(d) */
    for(y = 0; y < 4; y++)
    {
        rumboot_printf("\tCheck way %d...\n", y);
        for(x = 0; x < USED_INDEXES; x++)
        {
            rumboot_printf("\t\tCheck index %d...\n", x);
            for(d = 0; d < 8; d++)
            {
                icread((void*)CALC_ADDR(y,x,d));
                isync(); /* Mandatory after icread! */
                needed = data[CALC_ADDR(y,x,d) >> 2];
                readed = spr_read(SPR_ICDBDR0);
                if(!!(result = (readed != needed)))
                    rumboot_printf(
                            "\t\t\t%d -> READED: 0x%X, EXPECTED: 0x%X\n",
                            d, readed, needed);
                TEST_ASSERT(readed == needed, "Unexpected ICU data\n");
            }
        }
    }

    return result;
}


inline void icu_patch(icu_way_data_t *data, uint32_t opcode)
{
    data[3].b3[B0_RECORDS-1].op = opcode;
}

uint32_t main(void)
{
    uint32_t         status = TEST_OK,
                     result = 0;

    icu_way_data_t   *icu_cached_data    = (icu_way_data_t*)ICU_BASE;
    void            (*icu_cache_start)() = NULL;

    rumboot_putstring("Test started...\n");

    rumboot_printf("Init EMI...\n");
    emi_init(DCR_EM2_EMI_BASE);
    rumboot_printf("Init done.\n");

    write_tlb_entries(cached_mirror_entries, 1);

    TEST_ASSERT(result = (sizeof(icu_data) == ICU_MEM_SIZE),
            "Something wrong in icu data structere!\n");
    status |= !result;

    rumboot_printf("Copy of %d bytes cache data to cached RAM at 0x%X...\n",
            ICU_MEM_SIZE , SSRAM_BASE);
    icu_patch(icu_data, BLR_OP);
    memcpy((void*)ICU_BASE, (void*)icu_data, ICU_MEM_SIZE);
    msync();
    icu_cache_start = (icu_chain_func)icu_cached_data;

    rumboot_printf("Copy done.\n");

    /* Control structure sizes */
    PRINT_SIZEOF(icu_way_data_t);
    PRINT_SIZEOF(icu_data);

    rumboot_printf("Execute ICU-Cache...\n");
    ici(0);
    isync();
    icu_cache_start();
    rumboot_printf("Check ICU data...\n");
    status |= icu_check(icu_cached_data);

    rumboot_printf("TEST %s!\n", !status ? "OK" : "ERROR");

    return status;
}



