//-----------------------------------------------------------------------------
//  This program is for checking DDR0 SDRAM correct work
//  
//  Test includes:
//    - 
//    - 
//    - 
//      - 
//      - 
//      - 
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

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/ddr_test_lib.h>

#include <platform/devices.h>

//-------------------------------------------------------------
//  Test features control
//-----------------------------
// #define TEST_NO_DMA

// #define TEST_NO_STABILITY
#define STABILITY_DELAY  200000000
#define STABILITY_REP_NUMBER  5

// #define TEST_FAST
// #define TEST_NORMAL
#define REP_NUMBER      20


// extern uint32_t _debug_log;
// extern uint32_t _mem_log;
extern uint32_t memory_post_test(unsigned long vstart, unsigned long memsize,
        const unsigned long long * pattern, const unsigned long long * otherpattern,
        uint32_t * log, uint32_t flags);
extern uint32_t memory_regions_post_test(unsigned long vstart, unsigned long memsize,
        const unsigned long long * pattern, const unsigned long long * otherpattern,
        uint32_t * log, uint32_t flags);

// #include "irq_macros.h"

//  Why 0 was here?
// #if 0
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

//-----------------------------------------------------------------------------------------------------------------------------
//  Common functions
//-----------------------------------------------------------------------------------------------------------------------------
void delay_cycle (volatile uint32_t delay)
{
    while (delay != 0)
        delay--;
}

#if !defined(TEST_NO_DMA)
//-----------------------------------------------------------------------------
//  DMA interruption handler.
//-----------------------------
/*
static volatile uint32_t irq, err;

void ISR_MDMAC_INT(void)
{
    unsigned uint32_t val_pre, val_post;

    val_pre = rgMDMAC_Control;
    rgMDMAC_Control = 0;
    val_post = rgMDMAC_Control;

    irq += 1;

    if (val_pre & 0x4) {
        err = 1;
        return;
    }

    if (!(val_pre & 0x2) || (val_post & 0x8)) {
        err = 1;
        return;
    }

    rgMDMAC_SrcAddress = (unsigned long)0x3FFC0000;
    rgMDMAC_DstAddress = (unsigned long)0x3FFE0000;
    rgMDMAC_MainCounter = 0x4000;
    rgMDMAC_Control = 0x01;

    return;
}
*/
#endif

void clear_log (uint32_t * debug_log)
{
    for (uint32_t i = 0; i < 512; i+=4)
        iowrite32 (0x00000000, (uint32_t) debug_log + i);
}

int main (void)
{
    rumboot_printf ("\nddr0_loop_test test start\n");
    // uint32_t *debug_log  = &_debug_log;

    //  Set debug log pointer to IM1 heap start - it must not be used.
    uint32_t *debug_log  = (uint32_t*) 0x0007B000;
    uint32_t log_cnt;
    
    clear_log (debug_log);

    // LED control
    // *((unsigned long *)0x000CC010) |= 0x9;
    // *((unsigned long *)0x000CC000) |= 0x9;
    
    //init GPIO,write
    // iowrite32(0xff, base_gpio_addr + GPIO_SOURCE);//1-pad exchange with gpio
    // iowrite32(direction, base_gpio_addr + GPIO_DIRECTION);//1-output,0-input
    // iowrite32(send_data, base_gpio_addr + GPIO_WRITE_APBtoPAD);


    ret = -1;
    rep_cnt = 0;
    err_cnt = 0;
    done_cnt = 0;
    log_cnt = 0;

// #if 1
    // for (uint32_t n = 0; n < 16384; n++)
        // src_test_array[n] = 0x7777777777777777ULL;
// #endif

    // TEST_ATOMIC_BLOCK() {
        // *(debug_log + 0) = 0xEF5555EF;
        // *(debug_log + 1) = 0x11235813;
        // *(debug_log + 2) = log_cnt;
        // *(debug_log + 3) = 8192 - 10;

        // *((uint32_t *)(*(debug_log + 1)) + 0) = 0;
        // *((uint32_t *)(*(debug_log + 1)) + 1) = 0;
        // *((uint32_t *)(*(debug_log + 1)) + 2) = 24576 - 10;
    // }

#if !defined(TEST_NO_DMA)
//-----------------------------------------------------------------------------
//  TODO. Configure GIC for DMA interruptions handling here.
//  Mask interrupts for a while...
//-----------------------------
/*
    err = 0;
    irq = 0;


    rgMDMAC_InterruptMask = 0x3;

    rgGICD_ICFGR3 = 0xAAAAAAAA;
    rgGICD_IGROUPR0 = 0;
    rgGICD_IGROUPR1 = 0;
    rgGICD_IGROUPR2 = 0;
    rgGICD_ISENABLER1 = 0x01000000;
    rgGICC_PMR = 0xFF;
    rgGICC_CTLR = 0x1;
    rgGICD_CTLR = 0x1;
*/
#endif

    if (ddr_init (DDR0_BASE) != 0)
        return -1;
    rumboot_printf ("  ddr0 initialised\n");

//-----------------------------------------------------------------------------
//  TODO. Add ECC usage. Initialise SDRAM before reading.
//-----------------------------
//-----------------------------------------------------------------------------
//  TODO. Add version for RTL (#ifdef based).
//-----------------------------
//-----------------------------------------------------------------------------
//  TODO. Add regeneration checking.
//-----------------------------

//-----------------------------------------------------------------------------
//  Check SDRAM stability (regeneration)
//-----------------------------
#if !defined(TEST_NO_STABILITY)
    rumboot_printf ("  start stability test\n");
    for (uint32_t i = 0; i < (sizeof (src_test_array) >> 3); i++)
    {
        ((uint32_t *) EMI0_BASE) [i] = ((uint32_t *) src_test_array) [i];
    }
    // rumboot_printf ("    stability check delay start ... ");
    delay_cycle (STABILITY_DELAY);
    // rumboot_printf ("ok\n");
    for (uint32_t i = 0; i < (sizeof (src_test_array) >> 3); i++)
    {
        if (((uint32_t *) EMI0_BASE) [i] != ((uint32_t *) src_test_array) [i])
        {
            ret = -1;
            rumboot_printf ("  ERROR: stability check FAILED.\n");
            rumboot_printf ("    etalon 0x%08x    read 0x%08x\n", ((uint32_t *) src_test_array) [i], ((uint32_t *) EMI0_BASE) [i]);
        }
    }
#endif
    
#if !defined(TEST_NO_DMA)
//-----------------------------------------------------------------------------
//  TODO. Copy data from IM to the end of DDR SDRAM
//-----------------------------
/*
    rgMDMAC_SrcAddress = (unsigned long)src_test_array;
    rgMDMAC_DstAddress = (unsigned long)0x3FFC0000;
    rgMDMAC_MainCounter = 0x4000;
    rgMDMAC_Control = 0x01;

    while ((rgMDMAC_Control & 0x2) == 0) {};

    if (rgMDMAC_Control & 0x4)
        err = 1;

    rgMDMAC_Control = 0x00;

    ---------------------------------------------------------
     Log error in DMA transaction
    if (err) {
        TEST_ATOMIC_BLOCK() {
            log_cnt = *(debug_log + 2);
            *(debug_log + log_cnt + 4) = 0xDDDDDD01;
            *(debug_log + 2) = log_cnt + 1;
        }

        goto test_exit;
    }
*/
//-----------------------------------------------------------------------------
//  TODO. Start transactions DDR <-> DDR.
//    Repeat them infinitely in IRQ handler
//-----------------------------
/*
    rgMDMAC_InterruptMask = 0x0;

    rgMDMAC_SrcAddress = (unsigned long)0x3FFC0000;
    rgMDMAC_DstAddress = (unsigned long)0x3FFE0000;
    rgMDMAC_MainCounter = 0x4000;
    rgMDMAC_Control = 0x01;
*/
#endif

    ret = 0;

//-----------------------------------------------------------------------------
//  Cycle of testing DDR with CPU
//-----------------------------
    while (rep_cnt < REP_NUMBER) {
        uint32_t *mem_log, mem_cnt, mem_size;
        uint32_t tmp, log_size;

        // TEST_ATOMIC_BLOCK() {
            // log_cnt = *(debug_log + 2);
            // *(debug_log + log_cnt + 4) = rep_cnt;
            // *(debug_log + 2) = log_cnt + 1;
        // }
    
    ddr_check_status (DDR0_BASE);
    
#ifdef TEST_FAST
        rumboot_printf ("  start fast post test  %d\n", rep_cnt);
        tmp = memory_post_test(EMI0_BASE, 0x1FEC0000, pattern, &otherpattern, debug_log, 0);
#elif TEST_NORMAL
        rumboot_printf ("  start normal post test  %d\n", rep_cnt);
        tmp = memory_regions_post_test(EMI0_BASE, 0x1FEC0000, pattern, &otherpattern, debug_log, 1);
#else
        rumboot_printf ("  start maximum post test  %d\n", rep_cnt);
        tmp = memory_post_test(EMI0_BASE, 0x1FEC0000, pattern, &otherpattern, debug_log, 1);
#endif
        if (tmp) {
            rumboot_printf ("  ERROR !!!\n");
            ret = -1;
            err_cnt++;
        }
        else
            done_cnt++;

        rep_cnt++;

        mem_log = (uint32_t *)(*(debug_log + 1));

        // TEST_ATOMIC_BLOCK() {
            // log_cnt = *(debug_log + 2);
            // log_size = *(debug_log + 3);

            // mem_cnt = *(mem_log + 1);
            // mem_size = *(mem_log + 2);
        // }

        if ((log_cnt > (log_size - 4)) || (mem_cnt > (mem_size - 8)))
            break;
    }

#if !defined(TEST_NO_DMA)
//-----------------------------------------------------------------------------
//  Make current DMA transaction the last one
//-----------------------------
/*
    rgMDMAC_InterruptMask = 0x3;

    while (!err && !(rgMDMAC_Control & 0x2)) {};

    if (rgMDMAC_Control & 0x4)
        err = 1;

    rgMDMAC_Control = 0x00;

    if (err) {
            TEST_ATOMIC_BLOCK() {
                    log_cnt = *(debug_log + 2);
                    *(debug_log + log_cnt + 4) = 0xDDDDDD02;
                    *(debug_log + 2) = log_cnt + 1;
            }

        goto test_exit;
    }
*/
#endif

#if !defined(TEST_NO_DMA)
//-----------------------------------------------------------------------------
//  Copy resulting array with DMA from SDRAM -> IM
//-----------------------------
/*
    rgMDMAC_SrcAddress = (unsigned long)0x3FFE0000;
    rgMDMAC_DstAddress = (unsigned long)dst_test_array;
    rgMDMAC_MainCounter = 0x4000;
    rgMDMAC_Control = 0x01;

    while ((rgMDMAC_Control & 0x2) == 0) {};

    if (rgMDMAC_Control & 0x4)
        err = 1;

    rgMDMAC_Control = 0x00;

    if (err) {
                TEST_ATOMIC_BLOCK() {
                        log_cnt = *(debug_log + 2);
                        *(debug_log + log_cnt + 4) = 0xDDDDDD03;
                        *(debug_log + 2) = log_cnt + 1;
                }

        goto test_exit;
    }
*/
//-------------------------------------------------------------
//  Compare result of DMA transactions with etalon.
//    Try to classify error type.
/*
    for (uint32_t i = 0; i < 16384; i++) {
        uint32_t src_hi, src_lo, dst_hi, dst_lo;
        uint32_t * mem_log = (uint32_t *)(*(debug_log + 1));
        uint32_t mem_cnt;

        if (src_test_array[i] != dst_test_array[i]) {
            src_hi = (src_test_array[i] >> 32) & 0xFFFFFFFF;
            src_lo = src_test_array[i] & 0xFFFFFFFF;

            dst_hi = (dst_test_array[i] >> 32) & 0xFFFFFFFF;
            dst_lo = dst_test_array[i] & 0xFFFFFFFF;

                    TEST_ATOMIC_BLOCK() {
                mem_cnt = *(mem_log + 1);
                *(mem_log + mem_cnt + 3) = 0xDDDDDD04;
                *(mem_log + mem_cnt + 4) = i;
                *(mem_log + mem_cnt + 5) = src_lo;
                *(mem_log + mem_cnt + 6) = src_hi;
                *(mem_log + mem_cnt + 7) = dst_lo;
                *(mem_log + mem_cnt + 8) = dst_hi;
                *(mem_log + 1) = mem_cnt + 6;

                            log_cnt = *(debug_log + 2);
                            *(debug_log + log_cnt + 4) = 0xDDDDDD04;
                            *(debug_log + 2) = log_cnt + 1;
                    }

            ret = -1;
            break;
        }
    }
*/
#endif
    
test_exit:

#if !defined(TEST_NO_DMA)
//-----------------------------------------------------------------------------
//  Turn DMA off
//-----------------------------
/*
    rgGICD_CTLR = 0x0;
    rgGICC_CTLR = 0x0;

    if (err)
        ret = -1;
*/
#endif

    if (!ret)
        rumboot_printf ("\nddr0_loop_test  PASS\n");
        // *((unsigned long *)0x000CC000) &= ~0x8;
    else
        rumboot_printf ("\nddr0_loop_test  FAILED\n");
        // *((unsigned long *)0x000CC000) &= ~0x1;

    // TEST_ATOMIC_BLOCK() {
        // log_cnt = *(debug_log + 2);
        // *(debug_log + log_cnt + 4) = err_cnt;
        // *(debug_log + log_cnt + 5) = done_cnt;
        // *(debug_log + log_cnt + 6) = 0xFEAAAAFE;
        // *(debug_log + 2) = log_cnt + 2;
    // }

    return ret;
}
