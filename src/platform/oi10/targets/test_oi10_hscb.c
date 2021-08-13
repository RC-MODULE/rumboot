/*
 * test_oi10_hscb.c
 *
 *  Created on: Jul 23, 2018
 *      Author: r.galiulin
 */


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_hscb.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <rumboot/memfill.h>
#include <devices/ugly/hscb.h>
#include <devices/ugly/emi.h>
#include <platform/devices/nor_1636RR4.h>
#include <arch/ppc_476fp_mmu.h>


#define MAX_ATTEMPTS 100000


#ifdef CHECK_REGS
static uint32_t check_hscb_default_val( uint32_t base_addr ) {
    rumboot_printf("Check the default values of the registers:\n");

    struct regpoker_checker check_default_array[] = {
        {   "HSCB_ID",              REGPOKER_READ32,    HSCB_ID,                HSCB_ID_DFLT,               HSCB_ID_MASK                },
        {   "HSCB_VERSION",         REGPOKER_READ32,    HSCB_VERSION,           HSCB_VERSION_DFLT,          HSCB_VERSION_MASK           },
        {   "HSCB_SW_RESET",        REGPOKER_READ32,    HSCB_SW_RESET,          HSCB_SW_RESET_DFLT,         HSCB_SW_RESET_MASK          },
        {   "HSCB_SETTINGS",        REGPOKER_READ32,    HSCB_SETTINGS,          HSCB_SETTINGS_DFLT,         HSCB_SETTINGS_MASK          },
//      {   "HSCB_STATUS",          REGPOKER_READ32,    HSCB_STATUS,            HSCB_STATUS_DFLT,           HSCB_STATUS_MASK            },  // TODO: ustroystvo zapushcheno
        {   "HSCB_IRQ_MASK",        REGPOKER_READ32,    HSCB_IRQ_MASK,          HSCB_IRQ_MASK_DFLT,         HSCB_IRQ_MASK_MASK          },
        {   "HSCB_TRANS_CLK",       REGPOKER_READ32,    HSCB_TRANS_CLK,         HSCB_TRANS_CLK_DFLT,        HSCB_TRANS_CLK_MASK         },
        {   "HSCB_TIMINGS",         REGPOKER_READ32,    HSCB_TIMINGS,           HSCB_TIMINGS_DFLT,          HSCB_TIMINGS_MASK           },
        {   "HSCB_INTERVAL",        REGPOKER_READ32,    HSCB_INTERVAL,          HSCB_INTERVAL_DFLT,         HSCB_INTERVAL_MASK          },
        {   "HSCB_TIME_CODE_INNER", REGPOKER_READ32,    HSCB_TIME_CODE_INNER,   HSCB_TIME_CODE_INNER_DFLT,  HSCB_TIME_CODE_INNER_MASK   },
        {   "HSCB_TIME_CODE_OUTER", REGPOKER_READ32,    HSCB_TIME_CODE_OUTER,   HSCB_TIME_CODE_OUTER_DFLT,  HSCB_TIME_CODE_OUTER_MASK   },
        {   "HSCB_RMAP_PARAMS",     REGPOKER_READ32,    HSCB_RMAP_PARAMS,       HSCB_RMAP_PARAMS_DFLT,      HSCB_RMAP_PARAMS_MASK       },
        {   "HSCB_RMAP_STATUS",     REGPOKER_READ32,    HSCB_RMAP_STATUS,       HSCB_RMAP_STATUS_DFLT,      HSCB_RMAP_STATUS_MASK       },
        {   "HSCB_RMAP_C_STATUS",   REGPOKER_READ32,    HSCB_RMAP_C_STATUS,     HSCB_RMAP_C_STATUS_DFLT,    HSCB_RMAP_C_STATUS_MASK     },
        {   "HSCB_ADMA_ID",         REGPOKER_READ32,    HSCB_ADMA_ID,           HSCB_ADMA_ID_DFLT,          HSCB_ADMA_ID_MASK           },
        {   "HSCB_ADMA_REV",        REGPOKER_READ32,    HSCB_ADMA_REV,          HSCB_ADMA_REV_DFLT,         HSCB_ADMA_REV_MASK          },
        {   "HSCB_ADMA_SW_RESET",   REGPOKER_READ32,    HSCB_ADMA_SW_RESET,     HSCB_ADMA_SW_RESET_DFLT,    HSCB_ADMA_SW_RESET_MASK     },
        {   "HSCB_ADMA_CH_STATUS",  REGPOKER_READ32,    HSCB_ADMA_CH_STATUS,    HSCB_ADMA_CH_STATUS_DFLT,   HSCB_ADMA_CH_STATUS_MASK    },
        {   "HSCB_ARLEN",           REGPOKER_READ32,    HSCB_ARLEN,             HSCB_ARLEN_DFLT,            HSCB_ARLEN_MASK             },
        {   "HSCB_AWLEN",           REGPOKER_READ32,    HSCB_AWLEN,             HSCB_AWLEN_DFLT,            HSCB_AWLEN_MASK             },
        {   "HSCB_BRESP",           REGPOKER_READ32,    HSCB_BRESP,             HSCB_BRESP_DFLT,            HSCB_BRESP_MASK             },
        {   "HSCB_RAXI_ERR_ADDR",   REGPOKER_READ32,    HSCB_RAXI_ERR_ADDR,     HSCB_RAXI_ERR_ADDR_DFLT,    HSCB_RAXI_ERR_ADDR_MASK     },
        {   "HSCB_ARCACHE",         REGPOKER_READ32,    HSCB_ARCACHE,           HSCB_ARCACHE_DFLT,          HSCB_ARCACHE_MASK           },
        {   "HSCB_AWCACHE",         REGPOKER_READ32,    HSCB_AWCACHE,           HSCB_AWCACHE_DFLT,          HSCB_AWCACHE_MASK           },
        {   "HSCB_ARPROT",          REGPOKER_READ32,    HSCB_ARPROT,            HSCB_ARPROT_DFLT,           HSCB_ARPROT_MASK            },
        {   "HSCB_AWPROT",          REGPOKER_READ32,    HSCB_AWPROT,            HSCB_AWPROT_DFLT,           HSCB_AWPROT_MASK            },
        {   "HSCB_ARLOCK",          REGPOKER_READ32,    HSCB_ARLOCK,            HSCB_ARLOCK_DFLT,           HSCB_ARLOCK_MASK            },
        {   "HSCB_AWLOCK",          REGPOKER_READ32,    HSCB_AWLOCK,            HSCB_AWLOCK_DFLT,           HSCB_AWLOCK_MASK            },
        {   "HSCB_ARBURST",         REGPOKER_READ32,    HSCB_ARBURST,           HSCB_ARBURST_DFLT,          HSCB_ARBURST_MASK           },
        {   "HSCB_AWBURST",         REGPOKER_READ32,    HSCB_AWBURST,           HSCB_AWBURST_DFLT,          HSCB_AWBURST_MASK           },
        {   "HSCB_RDMA_SETTINGS",   REGPOKER_READ32,    HSCB_RDMA_SETTINGS,     HSCB_RDMA_SETTINGS_DFLT,    HSCB_XDMA_SETTINGS_MASK     },
        {   "HSCB_RDMA_STATUS",     REGPOKER_READ32,    HSCB_RDMA_STATUS,       HSCB_RDMA_STATUS_DFLT,      HSCB_RDMA_STATUS_MASK       },
        {   "HSCB_RDMA_SYS_ADDR",   REGPOKER_READ32,    HSCB_RDMA_SYS_ADDR,     HSCB_RDMA_SYS_ADDR_DFLT,    HSCB_XDMA_SYS_ADDR_MASK     },
        {   "HSCB_RDMA_TBL_SIZE",   REGPOKER_READ32,    HSCB_RDMA_TBL_SIZE,     HSCB_RDMA_TBL_SIZE_DFLT,    HSCB_XDMA_TBL_SIZE_MASK     },
        {   "HSCB_RDMA_ADDR",       REGPOKER_READ32,    HSCB_RDMA_ADDR,         HSCB_RDMA_ADDR_DFLT,        HSCB_XDMA_ADDR_MASK         },
        {   "HSCB_RDMA_LEN",        REGPOKER_READ32,    HSCB_RDMA_LEN,          HSCB_RDMA_LEN_DFLT,         HSCB_XDMA_LEN_MASK          },
        {   "HSCB_RDMA_DESC_ADDR",  REGPOKER_READ32,    HSCB_RDMA_DESC_ADDR,    HSCB_RDMA_DESC_ADDR_DFLT,   HSCB_XDMA_DESC_ADDR_MASK    },
        {   "HSCB_WDMA_SETTINGS",   REGPOKER_READ32,    HSCB_WDMA_SETTINGS,     HSCB_WDMA_SETTINGS_DFLT,    HSCB_XDMA_SETTINGS_MASK     },
        {   "HSCB_WDMA_STATUS",     REGPOKER_READ32,    HSCB_WDMA_STATUS,       HSCB_WDMA_STATUS_DFLT,      HSCB_WDMA_STATUS_MASK       },
        {   "HSCB_WDMA_SYS_ADDR",   REGPOKER_READ32,    HSCB_WDMA_SYS_ADDR,     HSCB_WDMA_SYS_ADDR_DFLT,    HSCB_XDMA_SYS_ADDR_MASK     },
        {   "HSCB_WDMA_TBL_SIZE",   REGPOKER_READ32,    HSCB_WDMA_TBL_SIZE,     HSCB_WDMA_TBL_SIZE_DFLT,    HSCB_XDMA_TBL_SIZE_MASK     },
        {   "HSCB_WDMA_ADDR",       REGPOKER_READ32,    HSCB_WDMA_ADDR,         HSCB_WDMA_ADDR_DFLT,        HSCB_XDMA_ADDR_MASK         },
        {   "HSCB_WDMA_LEN",        REGPOKER_READ32,    HSCB_WDMA_LEN,          HSCB_WDMA_LEN_DFLT,         HSCB_XDMA_LEN_MASK          },
        {   "HSCB_WDMA_DESC_ADDR",  REGPOKER_READ32,    HSCB_WDMA_DESC_ADDR,    HSCB_WDMA_DESC_ADDR_DFLT,   HSCB_XDMA_DESC_ADDR_MASK    },
        { /* Sentinel */ }
    };

    if( rumboot_regpoker_check_array( check_default_array, base_addr ) == 0 ) {
        rumboot_printf( " OK\n" );
        return 0;
    }

    rumboot_printf( " ERROR\n" );
    return 1;
}

static uint32_t check_hscb_regs( uint32_t base_addr ) {
    rumboot_printf("Check WRITE/READ registers:\n");

    struct regpoker_checker check_rw_array[] = {
        {   "HSCB_SW_RESET",        REGPOKER_WRITE32,   HSCB_SW_RESET,          (0b1 << HSCB_SW_RESET_RST_i),
                                                                                                            0                           },
        {   "HSCB_SW_RESET",        REGPOKER_READ32,    HSCB_SW_RESET,          HSCB_SW_RESET_DFLT,         HSCB_SW_RESET_MASK          },
        {   "HSCB_SETTINGS",        REGPOKER_WRITE32,   HSCB_SETTINGS,          (0b0 << HSCB_SETTINGS_EN_HSCB_i),
                                                                                                            0                           },
        {   "HSCB_SETTINGS",        REGPOKER_WRITE32,   HSCB_SETTINGS,          HSCB_SETTINGS_DFLT
                                                                                & (~HSCB_SETTINGS_EN_HSCB_mask),
                                                                                                            HSCB_SETTINGS_MASK
                                                                                                         & ~HSCB_SETTINGS_EN_HSCB_mask  },
        {   "HSCB_IRQ_MASK",        REGPOKER_WRITE32,   HSCB_IRQ_MASK,          HSCB_IRQ_MASK_DFLT,         HSCB_IRQ_MASK_MASK          },
        {   "HSCB_TRANS_CLK",       REGPOKER_WRITE32,   HSCB_TRANS_CLK,         HSCB_TRANS_CLK_DFLT,        HSCB_TRANS_CLK_MASK         },
        {   "HSCB_TIMINGS",         REGPOKER_WRITE32,   HSCB_TIMINGS,           HSCB_TIMINGS_DFLT,          HSCB_TIMINGS_MASK           },
        {   "HSCB_INTERVAL",        REGPOKER_WRITE32,   HSCB_INTERVAL,          HSCB_INTERVAL_DFLT,         HSCB_INTERVAL_MASK          },
        {   "HSCB_TIME_CODE_INNER", REGPOKER_WRITE32,   HSCB_TIME_CODE_INNER,   HSCB_TIME_CODE_INNER_DFLT,  HSCB_TIME_CODE_INNER_INIT_CODE_mask   },
        {   "HSCB_RMAP_PARAMS",     REGPOKER_WRITE32,   HSCB_RMAP_PARAMS,       HSCB_RMAP_PARAMS_DFLT,      HSCB_RMAP_PARAMS_MASK       },
        {   "HSCB_ADMA_SW_RESET",   REGPOKER_WRITE32,   HSCB_ADMA_SW_RESET,     (0b1 << HSCB_ADMA_SW_RESET_i),
                                                                                                            0                           },
        {   "HSCB_ADMA_SW_RESET",   REGPOKER_READ32,    HSCB_ADMA_SW_RESET,     HSCB_ADMA_SW_RESET_DFLT,    HSCB_ADMA_SW_RESET_MASK     },
        {   "HSCB_ARLEN",           REGPOKER_WRITE32,   HSCB_ARLEN,             HSCB_ARLEN_DFLT,            HSCB_ARLEN_MASK             },
        {   "HSCB_AWLEN",           REGPOKER_WRITE32,   HSCB_AWLEN,             HSCB_AWLEN_DFLT,            HSCB_AWLEN_MASK             },
        {   "HSCB_ARCACHE",         REGPOKER_WRITE32,   HSCB_ARCACHE,           HSCB_ARCACHE_DFLT,          HSCB_ARCACHE_MASK           },
        {   "HSCB_AWCACHE",         REGPOKER_WRITE32,   HSCB_AWCACHE,           HSCB_AWCACHE_DFLT,          HSCB_AWCACHE_MASK           },
        {   "HSCB_ARPROT",          REGPOKER_WRITE32,   HSCB_ARPROT,            HSCB_ARPROT_DFLT,           HSCB_ARPROT_MASK            },
        {   "HSCB_AWPROT",          REGPOKER_WRITE32,   HSCB_AWPROT,            HSCB_AWPROT_DFLT,           HSCB_AWPROT_MASK            },
        {   "HSCB_ARLOCK",          REGPOKER_WRITE32,   HSCB_ARLOCK,            HSCB_ARLOCK_DFLT,           HSCB_ARLOCK_MASK            },
        {   "HSCB_AWLOCK",          REGPOKER_WRITE32,   HSCB_AWLOCK,            HSCB_AWLOCK_DFLT,           HSCB_AWLOCK_MASK            },
        {   "HSCB_ARBURST",         REGPOKER_WRITE32,   HSCB_ARBURST,           HSCB_ARBURST_DFLT,          HSCB_ARBURST_MASK           },
        {   "HSCB_AWBURST",         REGPOKER_WRITE32,   HSCB_AWBURST,           HSCB_AWBURST_DFLT,          HSCB_AWBURST_MASK           },
        {   "HSCB_RDMA_SETTINGS",   REGPOKER_WRITE32,   HSCB_RDMA_SETTINGS,     HSCB_RDMA_SETTINGS_DFLT,    HSCB_XDMA_SETTINGS_MASK
                                                                                                         & ~HSCB_XDMA_SETTINGS_EN_XDMA_mask
                                                                                                         & ~HSCB_XDMA_SETTINGS_CANCEL_XDMA_mask },
        {   "HSCB_RDMA_SYS_ADDR",   REGPOKER_WRITE32,   HSCB_RDMA_SYS_ADDR,     HSCB_RDMA_SYS_ADDR_DFLT,    HSCB_XDMA_SYS_ADDR_MASK     },
        {   "HSCB_RDMA_TBL_SIZE",   REGPOKER_WRITE32,   HSCB_RDMA_TBL_SIZE,     HSCB_RDMA_TBL_SIZE_DFLT,    HSCB_XDMA_TBL_SIZE_MASK     },
/*        {   "HSCB_RDMA_DESC_ADDR",  REGPOKER_WRITE32,   HSCB_RDMA_DESC_ADDR,    HSCB_RDMA_DESC_ADDR_DFLT,   HSCB_XDMA_DESC_ADDR_MASK    },*///according to discussion in OI10-84
        {   "HSCB_WDMA_SETTINGS",   REGPOKER_WRITE32,   HSCB_WDMA_SETTINGS,     HSCB_WDMA_SETTINGS_DFLT,    HSCB_XDMA_SETTINGS_MASK
                                                                                                         & ~HSCB_XDMA_SETTINGS_EN_XDMA_mask
                                                                                                         & ~HSCB_XDMA_SETTINGS_CANCEL_XDMA_mask },
        {   "HSCB_WDMA_SYS_ADDR",   REGPOKER_WRITE32,   HSCB_WDMA_SYS_ADDR,     HSCB_WDMA_SYS_ADDR_DFLT,    HSCB_XDMA_SYS_ADDR_MASK     },
        {   "HSCB_WDMA_TBL_SIZE",   REGPOKER_WRITE32,   HSCB_WDMA_TBL_SIZE,     HSCB_WDMA_TBL_SIZE_DFLT,    HSCB_XDMA_TBL_SIZE_MASK     },
        {   "HSCB_WDMA_DESC_ADDR",  REGPOKER_WRITE32,   HSCB_WDMA_DESC_ADDR,    HSCB_WDMA_DESC_ADDR_DFLT,   HSCB_XDMA_DESC_ADDR_MASK    },
        { /* Sentinel */ }
    };

    if( rumboot_regpoker_check_array( check_rw_array, base_addr ) == 0 ) {
        rumboot_printf( " OK\n" );
        return 0;
    }

    rumboot_printf( " ERROR\n" );
    return 1;
}
#endif

#ifdef HSCB_FUNC_TEST
#ifndef DATA_SIZE_0
#define DATA_SIZE_0 0x10
#endif
#ifndef DATA_SIZE_1
#define DATA_SIZE_1 0x20
#endif
#ifndef DATA_SIZE_2
#define DATA_SIZE_2 0x18
#endif
#ifndef DATA_SIZE_3
#define DATA_SIZE_3 0x28
#endif
#ifndef DATA_SIZE_4
#define DATA_SIZE_4 0x38
#endif
#ifndef DATA_SIZE_5
#define DATA_SIZE_5 0x30
#endif

#ifdef HSCB_SHORT_TEST

    #ifndef TX_0_HEAP_NAME
    #define TX_0_HEAP_NAME "IM1"
    #endif

    #ifndef RX_0_HEAP_NAME
    #define RX_0_HEAP_NAME "IM1"
    #endif

    #ifndef TX_1_HEAP_NAME
    #define TX_1_HEAP_NAME "IM1"
    #endif

    #ifndef RX_1_HEAP_NAME
    #define RX_1_HEAP_NAME "IM1"
    #endif

#else

    #ifndef TX_0_HEAP_NAME
    #define TX_0_HEAP_NAME "IM2"
    #endif

    #ifndef RX_0_HEAP_NAME
    #define RX_0_HEAP_NAME "SSRAM"
    #endif

    #ifndef TX_1_HEAP_NAME
    #define TX_1_HEAP_NAME "IM2"
    #endif

    #ifndef RX_1_HEAP_NAME
    #define RX_1_HEAP_NAME "IM1"
    #endif

    #ifndef TX_2_HEAP_NAME
    #define TX_2_HEAP_NAME "SSRAM"
    #endif

    #ifndef RX_2_HEAP_NAME
    #define RX_2_HEAP_NAME "IM1"
    #endif

    #ifndef TX_3_HEAP_NAME
    #define TX_3_HEAP_NAME "IM1"
    #endif

    #ifndef RX_3_HEAP_NAME
    #define RX_3_HEAP_NAME "SSRAM"
    #endif

    #ifndef TX_4_HEAP_NAME
    #define TX_4_HEAP_NAME "IM1"
    #endif

    #ifndef RX_4_HEAP_NAME
    #define RX_4_HEAP_NAME "IM2"
    #endif

    #ifndef TX_5_HEAP_NAME
    #define TX_5_HEAP_NAME "SSRAM"
    #endif

    #ifndef RX_5_HEAP_NAME
    #define RX_5_HEAP_NAME "IM2"
    #endif

#endif

#ifndef DATA_INITIAL_VALUE
#define DATA_INITIAL_VALUE 0
#endif

#ifndef INCREMENT_0
#define INCREMENT_0 1
#endif

#ifndef INCREMENT_1
#define INCREMENT_1 -1
#endif

#ifndef INCREMENT_2
#define INCREMENT_2 2
#endif

#ifndef INCREMENT_3
#define INCREMENT_3 -2
#endif

#ifndef INCREMENT_4
#define INCREMENT_4 3
#endif

#ifndef INCREMENT_5
#define INCREMENT_5 -3
#endif

#ifdef HSCB_SHORT_TEST
#define COUNT_AREAS 4
#else
#define COUNT_AREAS 12
#endif

#define DESCRIPTOR_TABLES_COUNT 4
#define DEFAULT_HEAP_ID 1

static volatile uint32_t hscb0_status;
static volatile uint32_t hscb1_status;
static volatile uint32_t hscb0_dma_status;
static volatile uint32_t hscb1_dma_status;
static volatile bool hscb0_link_established;
static volatile bool hscb1_link_established;

static void handler( int irq, void *arg ) {
    //get interrupt source
    rumboot_printf( "IRQ arrived  \n" );
//    rumboot_puthex (irq);
    if (irq == HSCB_UNDER_TEST_INT) {
        rumboot_printf("HSCB_IRQ (0x%x), status : 0x%x\n",
                HSCB_UNDER_TEST_BASE,
                hscb_get_status(HSCB_UNDER_TEST_BASE));
    }
    if (irq == HSCB_UNDER_TEST_DMA_INT) {
        hscb0_dma_status = hscb_get_adma_ch_status(HSCB_UNDER_TEST_BASE );
        rumboot_printf("HSCB_DMA_IRQ (0x%x), AXI DMA channel status : 0x%x\nDetailed HSCB DMA status : ",
                HSCB_UNDER_TEST_BASE,
                hscb0_dma_status);
        if (hscb0_dma_status & HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask)
            hscb0_dma_status = hscb_get_rdma_status(HSCB_UNDER_TEST_BASE);
        else if (hscb0_dma_status & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
            hscb0_dma_status = hscb_get_wdma_status(HSCB_UNDER_TEST_BASE);
        rumboot_printf("0x%x\n", hscb0_dma_status);
    }
    if (irq == HSCB_SUPPLEMENTARY_INT) {
        rumboot_printf("HSCB_IRQ (0x%x), status : 0x%x\n",
                HSCB_SUPPLEMENTARY_BASE,
                hscb_get_status(HSCB_SUPPLEMENTARY_BASE));
    }
    if (irq == HSCB_SUPPLEMENTARY_DMA_INT) {
        hscb1_dma_status = hscb_get_adma_ch_status(HSCB_SUPPLEMENTARY_BASE );
        rumboot_printf("HSCB_DMA_IRQ (0x%x), AXI DMA channel status : 0x%x\nDetailed HSCB DMA status : ",
                HSCB_SUPPLEMENTARY_BASE,
                hscb1_dma_status);
        if (hscb1_dma_status & HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask)
            hscb1_dma_status = hscb_get_rdma_status(HSCB_SUPPLEMENTARY_BASE);
        else if (hscb1_dma_status & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
            hscb1_dma_status = hscb_get_wdma_status(HSCB_SUPPLEMENTARY_BASE);
        rumboot_printf("0x%x\n", hscb1_dma_status);
    }
}

struct rumboot_irq_entry * create_irq_handlers()
{
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, HSCB_UNDER_TEST_INT,          RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, HSCB_UNDER_TEST_DMA_INT,      RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, HSCB_SUPPLEMENTARY_INT,       RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, HSCB_SUPPLEMENTARY_DMA_INT,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler, ( void* )0 );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( HSCB_UNDER_TEST_INT );
    rumboot_irq_enable( HSCB_UNDER_TEST_DMA_INT );
    rumboot_irq_enable( HSCB_SUPPLEMENTARY_INT );
    rumboot_irq_enable( HSCB_SUPPLEMENTARY_DMA_INT );
    rumboot_irq_sei();

     return tbl;
}

void delete_irq_handlers(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}


uint32_t prepare_memory_areas(uint8_t*** data_areas, uint32_t** data_area_sizes){
    int i;
#ifdef HSCB_SHORT_TEST
    char* memory_heap_names[] = {TX_0_HEAP_NAME,RX_0_HEAP_NAME,TX_1_HEAP_NAME,RX_1_HEAP_NAME};
    uint32_t memory_area_sizes[] = {DATA_SIZE_0, DATA_SIZE_1};
#else
    char* memory_heap_names[] =  {      TX_0_HEAP_NAME,
                                        RX_0_HEAP_NAME,
                                        TX_1_HEAP_NAME,
                                        RX_1_HEAP_NAME,
                                        TX_2_HEAP_NAME,
                                        RX_2_HEAP_NAME,
                                        TX_3_HEAP_NAME,
                                        RX_3_HEAP_NAME,
                                        TX_4_HEAP_NAME,
                                        RX_4_HEAP_NAME,
                                        TX_5_HEAP_NAME,
                                        RX_5_HEAP_NAME,};
    uint32_t memory_area_sizes[] = {    DATA_SIZE_0,
                                        DATA_SIZE_1,
                                        DATA_SIZE_2,
                                        DATA_SIZE_3,
                                        DATA_SIZE_4,
                                        DATA_SIZE_5};
#endif
    uint32_t count_of_memory_areas = sizeof(memory_heap_names)/sizeof(memory_heap_names[0]);

    TEST_ASSERT((count_of_memory_areas >> 1) == sizeof(memory_area_sizes)/sizeof(memory_area_sizes[0]),
            "A pair of memory areas must correspond to a single memory_area_size");

    rumboot_putstring("prepare_memory_areas");
    *data_areas = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
                count_of_memory_areas * sizeof(uint8_t*),
                0x8);
    *data_area_sizes = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
                count_of_memory_areas * sizeof(uint8_t*),
                0x8);
    TEST_ASSERT(((*data_areas) != NULL), "Cannot allocate from default heap!");
    for (i = 0; i < count_of_memory_areas; ++i){
        (*data_areas)[i] = rumboot_malloc_from_named_heap_aligned(memory_heap_names[i], memory_area_sizes[i >> 1], 0x8);
        if(i%2)
            TEST_ASSERT((((uint32_t)((*data_areas)[i]) < NOR_BASE) || ((uint32_t)((*data_areas)[i]) >= (NOR_BASE + NOR_SIZE))),
                    "Do not select NOR as a receiving area!");
        if((*data_areas)[i] == NULL){
            rumboot_putstring("Cannot allocate from heap with name ");
            rumboot_putstring((const char * const)&(memory_heap_names[i]));
            TEST_ASSERT(((*data_areas)[i] != NULL), "Cannot work without this memory ;( Exiting...");
        }
#ifdef TEST_OI10_HSCB_FULL_TRACING
        rumboot_puthex(i);
        rumboot_puthex((uint32_t)(*data_areas)[i]);
#endif
    }
#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_putstring("end prepare_memory_areas");
#endif
    for(i = 0; i < (count_of_memory_areas >> 1); ++i){
        (*data_area_sizes)[i] = memory_area_sizes[i];
    }
    return count_of_memory_areas;
}


void set_test_data_multiple(    uint8_t** data_areas,
                                uint32_t* data_area_sizes,
                                uint32_t  count_of_memory_areas,
                                bool nor_change_endian){
    int i;
    uint32_t data_initial_value = 0;
    int increment = 0;
#ifdef HSCB_SHORT_TEST
    int increments[] = {        INCREMENT_0,
                                0,
                                INCREMENT_1,
                                0};
#else
    int increments[] = {        INCREMENT_0,
                                0,
                                INCREMENT_1,
                                0,
                                INCREMENT_2,
                                0,
                                INCREMENT_3,
                                0,
                                INCREMENT_4,
                                0,
                                INCREMENT_5,
                                0};
#endif
    TEST_ASSERT((count_of_memory_areas == (sizeof(increments) / sizeof(increments[0]))),
            "Size of increments array does not correspond the count of data areas");
    rumboot_putstring("set_test_data_multiple");
    for(i = 0; i < count_of_memory_areas; ++i){
#ifdef TEST_OI10_HSCB_FULL_TRACING
        rumboot_puthex(i);
        rumboot_puthex((uint32_t)(*(data_areas + i)));
#endif
        data_initial_value = (((DATA_INITIAL_VALUE) & 0xff) << 24) |
                             (((DATA_INITIAL_VALUE +      increments[i] ) & 0xff) << 16)  |
                             (((DATA_INITIAL_VALUE + (2 * increments[i])) & 0xff) <<  8) |
                             (((DATA_INITIAL_VALUE + (3 * increments[i])) & 0xff) <<  0);
        if(nor_change_endian)
            data_initial_value = HSCB_CHANGE_ENDIAN_WORD(data_initial_value);
        if(increments[i] >= 0)
            increment = (((increments[i] ) & 0xff) << 2)  |
                        (((increments[i] ) & 0xff) << 10) |
                        (((increments[i] ) & 0xff) << 18) |
                        (((increments[i] ) & 0xff) << 26);
        else
            increment = -(  (((-increments[i] ) & 0xff) << 2)  +
                            (((-increments[i] ) & 0xff) << 10) +
                            (((-increments[i] ) & 0xff) << 18) +
                            (((-increments[i] ) & 0xff) << 26) );
        rumboot_memfill32((*(data_areas + i)), data_area_sizes[i >> 1], data_initial_value, increment );
    }
#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_putstring("end set_test_data_multiple");
#endif
}

void prepare_descriptor_areas(
        hscb_packed_descr_struct_t ***  array_of_descriptors,
        uint32_t*                       counts_of_descriptors,
        uint32_t                        counts_of_descriptor_tables){

    uint32_t i;
    rumboot_putstring("prepare_descriptor_areas");
    *array_of_descriptors = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
                counts_of_descriptor_tables * sizeof(hscb_packed_descr_struct_t*),
                0x8);
#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_puthex((uint32_t)*array_of_descriptors);
#endif
    TEST_ASSERT(((*array_of_descriptors) != NULL), "Cannot allocate from default heap!");
    for(i = 0; i < counts_of_descriptor_tables; ++i){
        (*array_of_descriptors)[i] = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
                ( hscb_get_tbl_len_by_count(counts_of_descriptors[i])),
                0x8);
        TEST_ASSERT(((*array_of_descriptors)[i] != NULL), "Cannot allocate from default heap!");
#ifdef TEST_OI10_HSCB_FULL_TRACING
        rumboot_puthex(i);
        rumboot_puthex((uint32_t)(*array_of_descriptors)[i]);
        rumboot_puthex(counts_of_descriptors[i]);
#endif
    }
#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_putstring("end prepare_descriptor_areas");
#endif
}

#ifdef TEST_OI10_HSCB_FULL_TRACING
inline static void print_hscb_descriptor(hscb_packed_descr_struct_t descriptor){
    rumboot_printf("descriptor address: \n%x\nstart_address == \n%x\nlength_attr == \n%x\n",
            &descriptor,hscb_change_endian(descriptor.start_address, true),hscb_change_endian(descriptor.length_attr, true));
}

void print_hscb_descriptors_in_cycle(   hscb_packed_descr_struct_t**    descriptors,
                                        uint32_t*                       descriptor_counts,
                                        uint32_t                        descriptor_tables_count){
    int i,j;
    rumboot_putstring("printing descriptors in cycle");

    for (i = 0; i < descriptor_tables_count; ++i){
        rumboot_puthex(i);
        rumboot_puthex(descriptor_counts[i]);
        rumboot_puthex((uint32_t)(descriptors[i]));
        for(j = 0; j < descriptor_counts[i]; ++j){
            rumboot_puthex((i << 16) | j);
            rumboot_puthex((uint32_t)((*(descriptors + i))+j));
            rumboot_puthex(*((uint32_t*)((*(descriptors + i)) + j)));
            rumboot_puthex((((uint32_t)((*(descriptors + i)) + j)) + 4));
            rumboot_puthex( *((uint32_t*)(((uint32_t)((*(descriptors + i)) + j)) + 4)) );
        }
    }
    rumboot_putstring("end printing descriptors in cycle");

}
#endif

void init_hscb_cfg( uint8_t**                       data_areas,
                    uint32_t*                       data_area_sizes,
                    hscb_packed_descr_struct_t**    hscb_descr,
                    uint32_t*                       count_of_descriptors,
                    uint32_t                        count_descr_tables,
                    bool                            change_endian){
    int i,j;
    rumboot_putstring("init_hscb_cfg");
    for (i = 0; i < count_descr_tables; ++i){
        for (j = 0; j < count_of_descriptors[i]; ++j){
#ifdef TEST_OI10_HSCB_FULL_TRACING
            rumboot_puthex((i << 16) | j );
            rumboot_puthex((uint32_t)(&((*(hscb_descr + i))[j])));
#endif
            (*(hscb_descr + i))[j].start_address =
                    (change_endian)  ? HSCB_CHANGE_ENDIAN_WORD(rumboot_virt_to_dma(data_areas[i + (j * count_descr_tables)]))
                                     : rumboot_virt_to_dma(data_areas[i + (j * count_descr_tables)]);
            (*(hscb_descr + i))[j].length_attr =
                    HSCB_CREATE_DESCRIPTOR_LEN_ATTR_ENDIAN_EXE(
                                             data_area_sizes[(i + (j * count_descr_tables)) >> 1],
                                             HSCB_ACT_TRAN,
                                             HSCB_ACT0_LAST,
                                             HSCB_DESCR_ITRPT_ON,
                                             HSCB_DESCR_VALID,
                                             change_endian) ;
        }
#ifdef TEST_OI10_HSCB_FULL_TRACING
        rumboot_puthex(count_of_descriptors[i]);
        rumboot_puthex((uint32_t)(&(*(hscb_descr + i))[count_of_descriptors[i]]));
#endif
        (*(hscb_descr + i))[count_of_descriptors[i]].start_address = 0;
        (*(hscb_descr + i))[count_of_descriptors[i]].length_attr = 0;
    }
#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_putstring("end init_hscb_cfg");
#endif
}

void set_descriptor_tables( uint32_t base_addr,
                            uint32_t supplementary_base_addr,
                            hscb_packed_descr_struct_t** hscb_descr,
                            uint32_t* descr_table_length,
                            uint32_t count_descr_tables){

    rumboot_putstring("set_descriptor_tables");
#ifdef TEST_OI10_HSCB_FULL_TRACING

    rumboot_putstring("descr_table_length[0]");
    rumboot_puthex(descr_table_length[0]);
    rumboot_putstring("hscb_get_tbl_len_by_count(descr_table_length[0])");
    rumboot_puthex(hscb_get_tbl_len_by_count(descr_table_length[0]));
#endif
    hscb_set_rdma_tbl_size(base_addr, hscb_get_tbl_len_by_count(descr_table_length[0]));
    hscb_set_wdma_tbl_size(base_addr, hscb_get_tbl_len_by_count(descr_table_length[3]));

#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_putstring("(*(hscb_descr + 0))[0]");
    print_hscb_descriptor((*(hscb_descr + 0))[0]);
    rumboot_putstring("rumboot_virt_to_dma((*(hscb_descr + 0)))");
    rumboot_puthex(rumboot_virt_to_dma((*(hscb_descr + 0))));
#endif
    hscb_set_rdma_sys_addr(base_addr, rumboot_virt_to_dma((*(hscb_descr + 0))));
    hscb_set_wdma_sys_addr(base_addr, rumboot_virt_to_dma((*(hscb_descr + 3))));

    hscb_set_wdma_tbl_size(supplementary_base_addr, hscb_get_tbl_len_by_count(descr_table_length[1]));
    hscb_set_rdma_tbl_size(supplementary_base_addr, hscb_get_tbl_len_by_count(descr_table_length[2]));

#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_putstring("(*(hscb_descr + 1))[0]");
    print_hscb_descriptor((*(hscb_descr + 1))[0]);
    rumboot_putstring("rumboot_virt_to_dma((*(hscb_descr + 1)))");
    rumboot_puthex(rumboot_virt_to_dma((*(hscb_descr + 1))));
#endif
    hscb_set_wdma_sys_addr(supplementary_base_addr, rumboot_virt_to_dma((*(hscb_descr + 1))));
    hscb_set_rdma_sys_addr(supplementary_base_addr, rumboot_virt_to_dma((*(hscb_descr + 2))));

#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_putstring("end set_descriptor_tables");
#endif
}

uint32_t hscb_check_data(
        hscb_packed_descr_struct_t  rx_table[],
        uint32_t                    rx_descr_count,
        hscb_packed_descr_struct_t  tx_table[],
        uint32_t                    tx_descr_count
        ){
    int j = 0;
    hscb_descr_struct_t hscb_descr_tx;
    hscb_descr_struct_t hscb_descr_rx;
    uint32_t result = 0;

    for (j = 0; j < rx_descr_count; ++j){
        rumboot_printf( "Descriptor #%d\n", j );
        rumboot_puthex((uint32_t)(&(rx_table[j])));
        hscb_descr_rx = hscb_get_descr_from_mem((uint32_t)(&(rx_table[j])), HSCB_ROTATE_BYTES_ENABLE);
        rumboot_putstring( "Start addr:" );
        rumboot_puthex (hscb_descr_rx.start_address);
        rumboot_putstring( "Length:" );
        rumboot_puthex (hscb_descr_rx.length);
        hscb_descr_tx = hscb_get_descr_from_mem((uint32_t)(&(tx_table[j])), HSCB_ROTATE_BYTES_ENABLE);
        result = memcmp((void *)hscb_descr_tx.start_address, (void *) hscb_descr_rx.start_address,hscb_descr_rx.length);
        rumboot_printf( "Descriptor #%d checked.\n", j );
    }
    /*Avoiding 4-4 = 0 in multiple checks*/
    if(result)
        return 1;
    else
        return 0;
}

void free_2D_arrays(void **  data_areas, uint32_t    data_areas_count)
{
    if(data_areas_count <= 0)
        return;
    for(int i = 0; i < data_areas_count; ++i)
        rumboot_free(*(data_areas + data_areas_count));
    rumboot_free(data_areas);
}

#ifdef HSCB_SHORT_TEST

static uint32_t check_hscb_short_func(
        uint32_t    base_addr,
        uint32_t    supplementary_base_addr,
        uint8_t **  data_areas,
        uint32_t*   data_area_sizes,
        uint32_t    data_areas_count){

    hscb_packed_descr_struct_t** descriptors = NULL;
    uint32_t descriptor_counts[] = {1, 1, 1, 1};
    int cnt = 0;
    int result = 0;

    hscb0_status = 0;
    hscb1_status = 0;
    hscb0_dma_status = 0;
    hscb1_dma_status = 0;

    rumboot_printf("Check functionality...\n");


#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_putstring("address of descriptor_counts");
    rumboot_puthex((uint32_t)descriptor_counts);
#endif

    prepare_descriptor_areas(&descriptors,descriptor_counts,DESCRIPTOR_TABLES_COUNT);

#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_putstring("address of descriptors");
    rumboot_puthex((uint32_t)descriptors);
#endif

    init_hscb_cfg( data_areas, data_area_sizes, descriptors, descriptor_counts,DESCRIPTOR_TABLES_COUNT, HSCB_ROTATE_BYTES_ENABLE);

#ifdef TEST_OI10_HSCB_FULL_TRACING
    print_hscb_descriptors_in_cycle(descriptors, descriptor_counts, DESCRIPTOR_TABLES_COUNT);
    print_hscb_descriptor((*(descriptors + 0))[0]);//TX0
    print_hscb_descriptor((*(descriptors + 1))[0]);//RX0
    print_hscb_descriptor((*(descriptors + 2))[0]);//TX1
    print_hscb_descriptor((*(descriptors + 3))[0]);//RX1
#endif

    set_descriptor_tables(base_addr, supplementary_base_addr, descriptors,descriptor_counts,DESCRIPTOR_TABLES_COUNT);
    hscb_set_max_speed(base_addr);
    hscb_set_max_speed(supplementary_base_addr);

    // Enable HSCB0 and HSCB1
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    supplementary_base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        base_addr + HSCB_SETTINGS);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        supplementary_base_addr + HSCB_SETTINGS);
    // Wait connecting
    rumboot_printf( "Waiting for HSCB (0x%x) and HSCB (0x%x) link establishing\n", base_addr, supplementary_base_addr);
    if(!(   hscb_wait_status(base_addr,                 (HSCB_STATE_RUN << HSCB_STATUS_STATE_i)) &
            hscb_wait_status(supplementary_base_addr,   (HSCB_STATE_RUN << HSCB_STATUS_STATE_i)))){
        rumboot_putstring( "Wait HSCB RUN state Time-out\n" );
        rumboot_printf("HSCB (0x%x) status: 0x%x\n", base_addr, hscb_get_status(base_addr));
        rumboot_printf("HSCB (0x%x) status: 0x%x\n", supplementary_base_addr, hscb_get_status(supplementary_base_addr));
        free_2D_arrays((void **)descriptors, DESCRIPTOR_TABLES_COUNT);
        return 1;
    }

    rumboot_putstring( "HSCB link has been enabled\n" );
    // Enable DMA for HSCB0 and HSCB1
    rumboot_putstring( "Start work!\n" );
    hscb_run_rdma(base_addr);
    hscb_run_rdma(supplementary_base_addr);
    hscb_run_wdma(base_addr);
    hscb_run_wdma(supplementary_base_addr);
    msync();
    if(rumboot_virt_to_dma((*(descriptors + 0)) + descriptor_counts[0]) != ioread32(base_addr + HSCB_RDMA_DESC_ADDR)){
        rumboot_printf("Expected value of descriptor address == 0x%x, obtained value == 0x%x.",
                            rumboot_virt_to_dma((*(descriptors + 0)) + descriptor_counts[0]),
                            ioread32(base_addr + HSCB_RDMA_DESC_ADDR));
        result += 1;
    }
    rumboot_printf( "Wait HSCB (0x%x) and HSCB (0x%x) finish work\n", base_addr, supplementary_base_addr );
    while (!(hscb0_dma_status & hscb1_dma_status)){
        if (cnt == MAX_ATTEMPTS) {
            rumboot_putstring( "Wait interrupt Time-out\n" );
            free_2D_arrays((void **)descriptors, DESCRIPTOR_TABLES_COUNT);
            return 1;
        }
        else
            cnt += 1;
    }
    cnt = 0;
    hscb0_dma_status = 0;
    hscb1_dma_status = 0;

    rumboot_putstring( "Finish work!\n" );
    rumboot_printf( "HSCB (0x%x) to HSCB (0x%x) descriptors checking\n", supplementary_base_addr, base_addr );
    rumboot_puthex((uint32_t)(*(descriptors + 3)));
    result += hscb_check_data(*(descriptors + 3), descriptor_counts[3], *(descriptors + 2), descriptor_counts[2]);
    rumboot_printf( "HSCB (0x%x) to HSCB (0x%x) descriptors: checked.\n", supplementary_base_addr, base_addr  );

    rumboot_printf( "HSCB (0x%x) to HSCB (0x%x) descriptors checking\n", base_addr, supplementary_base_addr );
    rumboot_puthex((uint32_t)(*(descriptors + 1)));
    result += hscb_check_data(*(descriptors + 1), descriptor_counts[1], *(descriptors + 0), descriptor_counts[0]);
    rumboot_printf( "HSCB (0x%x) to HSCB (0x%x) descriptors: checked.\n", base_addr, supplementary_base_addr );
    free_2D_arrays((void **)descriptors, DESCRIPTOR_TABLES_COUNT);
    return result;
}
#else

static uint32_t check_hscb_func(
        uint32_t base_addr,
        uint32_t supplementary_base_addr,
        uint8_t **  data_areas,
        uint32_t*   data_area_sizes,
        uint32_t    data_areas_count){
    hscb_packed_descr_struct_t** descriptors = NULL;
    uint32_t descriptor_counts[DESCRIPTOR_TABLES_COUNT] = {3, 3, 3, 3};
    int cnt = 0;
    int result = 0;

    hscb0_status = 0;
    hscb1_status = 0;
    hscb0_dma_status = 0;
    hscb1_dma_status = 0;

    rumboot_putstring( "------- HSCB ACCESS test -------\n" );

    prepare_descriptor_areas(&descriptors,descriptor_counts,DESCRIPTOR_TABLES_COUNT);
    init_hscb_cfg( data_areas, data_area_sizes, descriptors, descriptor_counts,DESCRIPTOR_TABLES_COUNT, HSCB_ROTATE_BYTES_ENABLE);

#ifdef TEST_OI10_HSCB_FULL_TRACING
    print_hscb_descriptors_in_cycle(descriptors, descriptor_counts, DESCRIPTOR_TABLES_COUNT);
#endif

    set_descriptor_tables(base_addr, supplementary_base_addr, descriptors,descriptor_counts,DESCRIPTOR_TABLES_COUNT);
    hscb_set_max_speed(base_addr);
    hscb_set_max_speed(supplementary_base_addr);
        // Enable HSCB0 and HSCB1
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    supplementary_base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        base_addr + HSCB_SETTINGS);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        supplementary_base_addr + HSCB_SETTINGS);
    rumboot_printf( "Waiting for HSCB (0x%x) and HSCB (0x%x) link establishing\n", base_addr, supplementary_base_addr);
    if(!(   hscb_wait_status(base_addr,                 (HSCB_STATE_RUN << HSCB_STATUS_STATE_i)) &
            hscb_wait_status(supplementary_base_addr,   (HSCB_STATE_RUN << HSCB_STATUS_STATE_i)))){
        rumboot_putstring( "Wait HSCB RUN state Time-out\n" );
        rumboot_printf("HSCB (0x%x) status: 0x%x\n", base_addr, hscb_get_status(base_addr));
        rumboot_printf("HSCB (0x%x) status: 0x%x\n", supplementary_base_addr, hscb_get_status(supplementary_base_addr));
        free_2D_arrays((void **)descriptors, DESCRIPTOR_TABLES_COUNT);
        return 1;
    }

    rumboot_putstring( "HSCB link has enabled\n" );
    // Enable DMA for HSCB0 and HSCB1
    rumboot_putstring( "Start work!\n" );
    hscb_run_rdma(base_addr);
    hscb_run_rdma(supplementary_base_addr);
    hscb_run_wdma(base_addr);
    hscb_run_wdma(supplementary_base_addr);
    if(rumboot_virt_to_dma((*(descriptors + 0)) + descriptor_counts[0]) != ioread32(base_addr + HSCB_RDMA_DESC_ADDR)){
        rumboot_printf("Expected value of descriptor address == 0x%x, obtained value == 0x%x.",
                            rumboot_virt_to_dma((*(descriptors + 0)) + descriptor_counts[0]),
                            ioread32(base_addr + HSCB_RDMA_DESC_ADDR));
        result += 1;
    }
    rumboot_printf( "Wait HSCB (0x%x) and HSCB (0x%x) finish work\n", base_addr, supplementary_base_addr );
    while (!(hscb0_dma_status & hscb1_dma_status)){
        if (cnt == MAX_ATTEMPTS) {
            rumboot_putstring( "Wait interrupt Time-out\n" );
            free_2D_arrays((void **)descriptors, DESCRIPTOR_TABLES_COUNT);
            return 1;
        }
        else
            cnt += 1;
    }
    cnt = 0;
    hscb0_dma_status = 0;
    hscb1_dma_status = 0;
    rumboot_printf( "Finish work!\n" );


    rumboot_printf( "HSCB (0x%x) to HSCB (0x%x) descriptors checking\n", supplementary_base_addr, base_addr );
    rumboot_puthex((uint32_t)(*(descriptors + 3)));
    result += hscb_check_data(*(descriptors + 3), descriptor_counts[3], *(descriptors + 2), descriptor_counts[2]);
    rumboot_printf( "HSCB (0x%x) to HSCB (0x%x) descriptors: checked.\n", supplementary_base_addr, base_addr  );

    rumboot_printf( "HSCB (0x%x) to HSCB (0x%x) descriptors checking\n", base_addr, supplementary_base_addr );
    rumboot_puthex((uint32_t)(*(descriptors + 1)));
    result += hscb_check_data(*(descriptors + 1), descriptor_counts[1], *(descriptors + 0), descriptor_counts[0]);
    rumboot_printf( "HSCB (0x%x) to HSCB (0x%x) descriptors: checked.\n", base_addr, supplementary_base_addr );
    free_2D_arrays((void **)descriptors, DESCRIPTOR_TABLES_COUNT);
    return result;
}
#endif
#endif

int main() {
    uint32_t result = 0x0;


#ifdef CHECK_REGS
    rumboot_printf( "Checking regs of HSCB (0x%x) \n", HSCB_UNDER_TEST_BASE );
    result += check_hscb_default_val( HSCB_UNDER_TEST_BASE );
    result += check_hscb_regs( HSCB_UNDER_TEST_BASE );
    hscb_sw_rst(HSCB_UNDER_TEST_BASE);
    hscb_adma_sw_rst(HSCB_UNDER_TEST_BASE);
#endif
#ifdef HSCB_FUNC_TEST
    rumboot_printf( "Checking base functions of HSCB (0x%x) and HSCB (0x%x)\n",
            HSCB_UNDER_TEST_BASE, HSCB_SUPPLEMENTARY_BASE);
    hscb_sw_rst(HSCB_UNDER_TEST_BASE);
    hscb_adma_sw_rst(HSCB_UNDER_TEST_BASE);
    hscb_sw_rst(HSCB_SUPPLEMENTARY_BASE);
    hscb_adma_sw_rst(HSCB_SUPPLEMENTARY_BASE);
    
    struct rumboot_irq_entry *tbl;
    uint32_t count_of_memory_areas;
    uint8_t** data_areas = NULL;
    uint32_t* data_area_sizes = NULL;

    static struct tlb_entry const em2_nospeculative_tlb_entries[] =
    {
    /*   MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };

    write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));


    emi_init(DCR_EM2_EMI_BASE);
    tbl = create_irq_handlers();

    count_of_memory_areas = prepare_memory_areas(&data_areas, &data_area_sizes);
    set_test_data_multiple(data_areas, data_area_sizes, count_of_memory_areas, HSCB_ROTATE_BYTES_DISABLE);
#ifdef HSCB_SHORT_TEST
    result += check_hscb_short_func(HSCB_UNDER_TEST_BASE, HSCB_SUPPLEMENTARY_BASE, data_areas, data_area_sizes, count_of_memory_areas);
#else
    result += check_hscb_func(HSCB_UNDER_TEST_BASE, HSCB_SUPPLEMENTARY_BASE, data_areas, data_area_sizes, count_of_memory_areas);
#endif
    delete_irq_handlers(tbl);
    free_2D_arrays((void **)data_areas, count_of_memory_areas);
    rumboot_free(data_areas);
    rumboot_free(data_area_sizes);
#endif
    return result;
}
