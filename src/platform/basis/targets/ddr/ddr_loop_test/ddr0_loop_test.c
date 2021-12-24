//-----------------------------------------------------------------------------
//  This program is for checking DDR0 SDRAM correct work
//  
//  Test includes:
//    - clear debug log (not used now)
//    - turn DDR0 On
//    - check SDRAM stability (define TEST_NO_STABILITY to exclude this feature)
//    - check memory with post test (March algorytms)
//        select the deepth of check with defines TEST_FAST, TEST_NORMAL or TEST_MAXIMUM
//        select parameters of checked memory POST_TEST_BASE and POST_TEST_SIZE
//    - start MDMA transfers simultaneously with post test to make a large physical payload on DDR
//        (define TEST_NO_DMA to exclude this feature)
//    
//    
//    
//    Test duration (RTL): < NA
//    Test duration (real): <
//-----------------------------------------------------------------------------

#include <stdint.h>

#include <regs/regs_ddr.h>
#include <regs/regs_crg.h>
#include <regs/regs_gpio.h>
#include <regs/regs_mdma.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/ddr_test_lib.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <platform/devices.h>
#include <platform/interrupts.h>

#include <devices/sp804.h>

//-----------------------------------------------------------------------------
//  Test features control
//-----------------------------
// #define TEST_NO_DMA

//-------------
// #define TEST_NO_STABILITY
#define STABILITY_DELAY  200000000
#define STABILITY_REP_NUMBER  5

//-------------
// #define TEST_FAST
#define TEST_NORMAL
// #define TEST_MAXIMUM

#define POST_TEST_BASE  EMI0_BASE
//  On RCM MT143.05 PCB only 1/2 of memory space is available
//  End part of memory is used by MDMA
#define POST_TEST_SIZE  0x1FEC0000

#define REP_NUMBER      10

//  This parameter only used for preparation SDRAM in case of ECC
#define REQUIRED_MEMORY_SIZE  0x20000000
//-------------------------------------------------------------


extern uint32_t memory_post_test(unsigned long vstart, unsigned long memsize,
        const unsigned long long * pattern, const unsigned long long * otherpattern, uint32_t flags);
extern uint32_t memory_regions_post_test(unsigned long vstart, unsigned long memsize,
        const unsigned long long * pattern, const unsigned long long * otherpattern, uint32_t flags);

#if 1
#include "ddr_test_array.h"
#else
static unsigned long long src_test_array[16384] __attribute__ ((section (".src_array"), aligned (8))) = {};
static unsigned long long dst_test_array[16384] __attribute__ ((section (".dst_array"), aligned (8))) = {};
#endif

static const unsigned long long pattern[13] = {
    0xaaaaaaaaaaaaaaaaULL,
    0xccccccccccccccccULL,
    0xf0f0f0f0f0f0f0f0ULL,
    0xff00ff00ff00ff00ULL,
    0xffff0000ffff0000ULL,
    0xffffffff00000000ULL,
    0x00000000ffffffffULL,
    0x0000ffff0000ffffULL,
    0x00ff00ff00ff00ffULL,
    0x0f0f0f0f0f0f0f0fULL,
    0x3333333333333333ULL,
    0x5555555555555555ULL,
    0x0000000000000000ULL // termitator !!!
};

static const unsigned long long otherpattern = 0x0123456789abcdefULL;

static volatile uint32_t ret, rep_cnt, err_cnt, done_cnt;

//-----------------------------------------------------------------------------
//  MDMA related elements
//-----------------------------
#define MDMA0_SDRAM_SRC  (EMI0_BASE + 0x1FFC0000)
#define MDMA0_SDRAM_DST  (EMI0_BASE + 0x1FFE0000)

#define STOP_FLAG  0x10000000
#define MDMA_LEN   ((16384 << 2) | STOP_FLAG)

#define INT_STAT    0x4
#define BAD_STAT    0x8
#define STOP_STAT   0x10

uint32_t mdma_transfer_direction = 0;

volatile struct __attribute__ ((aligned (8))) mdma_rd_descriptor {
    uint32_t data_src;
    uint32_t data_len;
    uint32_t reserved_0;
    uint32_t stop_word;
} mdma0_rd_descriptor;

volatile struct __attribute__ ((aligned (8))) mdma_wr_descriptor {
    uint32_t data_dst;
    uint32_t data_len;
    uint32_t reserved_0;
    uint32_t stop_word;
} mdma0_wr_descriptor;

static inline void mdma0_start ()
{
    iowrite32(1, MDMA0_BASE + MDMA_ENABLE_W);
    iowrite32(1, MDMA0_BASE + MDMA_ENABLE_R);    
}

static inline void mdma0_transceive (uint32_t data_src, uint32_t data_dst)
{
    // rumboot_printf ("  DMA start\n");
    // rumboot_printf ("    data_src  0x%08x\n", data_src);
    // rumboot_printf ("    data_dst  0x%08x\n", data_dst);
    // rumboot_printf ("    data_len  0x%08x\n", MDMA_LEN);
    
    // rumboot_printf ("    MDMA0_BASE  0x%08x\n", MDMA0_BASE);
    
    mdma0_rd_descriptor.data_src   = data_src    ;
    mdma0_rd_descriptor.data_len   = MDMA_LEN    ;
    mdma0_rd_descriptor.reserved_0 = 0x00000000  ;
    mdma0_rd_descriptor.stop_word  = 0xFFFFFFFF  ;

    mdma0_wr_descriptor.data_dst   = data_dst    ;
    mdma0_wr_descriptor.data_len   = MDMA_LEN    ;
    mdma0_wr_descriptor.reserved_0 = 0x00000000  ;
    mdma0_wr_descriptor.stop_word  = 0xFFFFFFFF  ;

    iowrite32((uint32_t)(&mdma0_rd_descriptor), MDMA0_BASE + MDMA_DESC_ADDR_R);
    iowrite32((uint32_t)(&mdma0_wr_descriptor), MDMA0_BASE + MDMA_DESC_ADDR_W);
    
    mdma0_start ();
}

//-----------------------------------------------------------------------------
//  Common functions
//-----------------------------
void delay_cycle (volatile uint32_t delay)
{
    while (delay != 0)
        delay--;
}

//-----------------------------------------------------------------------------
//  DMA interruption handler.
//-----------------------------
#if !defined(TEST_NO_DMA)
static void mdma0_irq_handler (int irq, void *arg)
{
	// rumboot_printf ("\n -> MDMA0_IRQ irq happened \n");
    
    //  Clear irq status regs in MDMA
    ioread32(MDMA0_BASE + MDMA_STATUS_W);
    ioread32(MDMA0_BASE + MDMA_GP_STATUS);
    //  Create new MDMA packet transaction
    //    Transfers are made in both sides to accumulate errors
    if (mdma_transfer_direction)
        mdma0_transceive ((uint32_t) MDMA0_SDRAM_SRC, (uint32_t) MDMA0_SDRAM_DST);
    else
        mdma0_transceive ((uint32_t) MDMA0_SDRAM_DST, (uint32_t) MDMA0_SDRAM_SRC);

    mdma_transfer_direction = !mdma_transfer_direction;
}
#endif

int main (void)
{
    uint32_t time_start;
    uint32_t time_finish;
    
    rumboot_printf ("\nddr0_loop_test test start\n");
    time_start = rumboot_platform_get_uptime();
    rumboot_printf ("  TIME: start:  %d us\n", time_start);

    ret = -1;
    rep_cnt = 0;
    err_cnt = 0;
    done_cnt = 0;

    if (ddr_init (DDR0_BASE) != 0)
        return -1;
    rumboot_printf ("  ddr0 initialised\n");
    ddr_ecc_init (DDR0_BASE, EMI0_BASE, REQUIRED_MEMORY_SIZE);

//-----------------------------------------------------------------------------
//  TODO. Add ECC usage. Initialise SDRAM before reading.
//-----------------------------
//-----------------------------------------------------------------------------
//  TODO. Add version for RTL (#ifdef based).
//-----------------------------

//-----------------------------------------------------------------------------
//  Check SDRAM stability (regeneration)
//-----------------------------
#if !defined(TEST_NO_STABILITY)
    uint32_t rdata;
    uint32_t err_cntr = 0;
    
    rumboot_printf ("  start stability test...\n");
    for (uint32_t i = 0; i < (sizeof (src_test_array) >> 3); i++)
    {
        ((uint32_t *) EMI0_BASE) [i] = ((uint32_t *) src_test_array) [i];
    }
    // rumboot_printf ("    stability check delay start ... ");
    delay_cycle (STABILITY_DELAY);
    // rumboot_printf ("ok\n");
    for (uint32_t i = 0; i < (sizeof (src_test_array) >> 3); i++)
    {
        rdata = ((uint32_t *) EMI0_BASE) [i];
        if (rdata != ((uint32_t *) src_test_array) [i])
        {
            err_cntr++;
            ret = -1;
            rumboot_printf ("  ERROR_STABILITY");
            rumboot_printf ("    #%d  etalon=0x%08x    read=0x%08x    read_2=0x%08x    XOR=0x%08x\n", i, ((uint32_t *) src_test_array) [i], rdata, ((uint32_t *) EMI0_BASE) [i], ((uint32_t *) src_test_array) [i]^rdata);
            if (err_cntr == 50)
            {
                rumboot_printf ("  Error counter overflow, test continued\n");
                break;
            }
        }
    }
    if (err_cntr == 0)
        rumboot_printf ("    PASSED\n");
#endif
    
#if !defined(TEST_NO_DMA)
//-----------------------------------------------------------------------------
//  Copy data from IM to the end of DDR SDRAM
//-----------------------------

    rumboot_printf ("  copy data from IM to the end of DDR SDRAM for DMA");
    mdma0_transceive ((uint32_t) src_test_array, (uint32_t) MDMA0_SDRAM_SRC);
    
    while ((ioread32(MDMA0_BASE + MDMA_STATUS_W) & 0x10) == 0)
        ;
    
//-----------------------------------------------------------------------------
//  Start transactions DDR <-> DDR.
//    Repeat them infinitely in IRQ handler
//-----------------------------

    //  GIC configuration
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, MDMA0_IRQ, 0, mdma0_irq_handler, NULL);
    /* Activate the table */
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(MDMA0_IRQ);
    rumboot_irq_sei();
    
    //  Enable IRQ in MDMA
    iowrite32 (INT_STAT | STOP_STAT, MDMA0_BASE + MDMA_IRQ_MASK_W);
    
    //  Transceive data    SDRAM -> SDRAM
    rumboot_printf ("  start MDMA infinite transactions in background\n");
    mdma0_transceive ((uint32_t) MDMA0_SDRAM_SRC, (uint32_t) MDMA0_SDRAM_DST);
#endif

    ret = 0;

//-----------------------------------------------------------------------------
//  Cycle of testing DDR with CPU
//-----------------------------
    while (rep_cnt < REP_NUMBER) {
        uint32_t tmp;

    ddr_check_status (DDR0_BASE);
    
#ifdef TEST_FAST
        rumboot_printf ("  start fast post test  %d\n", rep_cnt);
        tmp = memory_post_test(POST_TEST_BASE, POST_TEST_SIZE, pattern, &otherpattern, 0);
#endif
#ifdef TEST_NORMAL
        rumboot_printf ("  start normal post test  %d\n", rep_cnt);
        tmp = memory_regions_post_test(POST_TEST_BASE, POST_TEST_SIZE, pattern, &otherpattern, 1);
#endif
#ifdef TEST_MAXIMUM
        rumboot_printf ("  start maximum post test  %d\n", rep_cnt);
        tmp = memory_post_test(POST_TEST_BASE, POST_TEST_SIZE, pattern, &otherpattern, 1);
#endif
        if (tmp) {
            ret = -1;
            err_cnt++;
        }
        else
            done_cnt++;

        rep_cnt++;
    }

#if !defined(TEST_NO_DMA)
//-----------------------------------------------------------------------------
//  Make current DMA transaction the last one
//-----------------------------
    rumboot_printf ("  cancel MDMA work\n");
    iowrite32(0, MDMA0_BASE + MDMA_IRQ_MASK_W);
    
    while ((ioread32(MDMA0_BASE + MDMA_STATUS_W) & 0x10) == 0)
        ;

//-----------------------------------------------------------------------------
//  Copy resulting array with DMA from SDRAM -> IM
//-----------------------------
    mdma0_transceive ((uint32_t) MDMA0_SDRAM_DST, (uint32_t) dst_test_array);
    while ((ioread32(MDMA0_BASE + MDMA_STATUS_W) & 0x10) == 0)
        ;
    
    err_cntr = 0;
    
    rumboot_printf ("  compare MDMA result with etalon\n");
    for (uint32_t i = 0; i < 16384; i++)
        if (src_test_array[i] != dst_test_array[i])
        {
            err_cntr++;
            ret = -1;
            rumboot_printf ("      ERROR_MDMA");
            rumboot_printf ("        #%d  dst_test_array=0x%08x    src_test_array=0x%08x    XOR=0x%08x\n", i, dst_test_array[i], src_test_array[i], src_test_array[i]^dst_test_array[i]);
            if (err_cntr == 50)
            {
                rumboot_printf ("  Error counter overflow, test continued\n");
                break;
            }
        }
#endif

    if (!ret)
        rumboot_printf ("\nddr0_loop_test  PASS\n");
    else
        rumboot_printf ("\nddr0_loop_test  FAILED\n");

    time_finish = rumboot_platform_get_uptime();
    rumboot_printf ("  TIME: end:  %d us\n", time_finish);
    rumboot_printf ("  TIME: delta:  %d us\n", time_finish-time_start);
    
    return ret;
}
