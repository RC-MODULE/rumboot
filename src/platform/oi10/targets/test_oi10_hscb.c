/*
 * test_oi10_hscb.c
 *
 *  Created on: Jul 23, 2018
 *      Author: r.galiulin
 */


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/interrupts.h>
#include <regs/regs_hscb.h>
#include <rumboot/irq.h>
#include <devices/hscb.h>


#define GPIO_REG_MASK   0xFFFFFFFF
#define MAX_ATTEMPTS 100000


#ifdef CHECK_REGS
static uint32_t check_hscb_default_val( uint32_t base_addr ) {
    rumboot_printf("Check the default values of the registers:\n");

    struct regpoker_checker check_default_array[] = {
        {   "HSCB_ID",              REGPOKER_READ32,    HSCB_ID,                HSCB_ID_DFLT,               HSCB_ID_MASK                },
        {   "HSCB_VERSION",         REGPOKER_READ32,    HSCB_VERSION,           HSCB_VERSION_DFLT,          HSCB_VERSION_MASK           },
        {   "HSCB_SW_RESET",        REGPOKER_READ32,    HSCB_SW_RESET,          HSCB_SW_RESET_DFLT,         HSCB_SW_RESET_MASK          },
        {   "HSCB_SETTINGS",        REGPOKER_READ32,    HSCB_SETTINGS,          (0b1 << HSCB_SETTINGS_EN_HSCB_i)                            // TODO: V spetsifikatsii 0x00000000
                                                                              | (0b1 << HSCB_SETTINGS_EN_RMAP_i),
                                                                                                            HSCB_SETTINGS_MASK          },
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
    rumboot_printf("Check WRITE/READ registers:");

    struct regpoker_checker check_rw_array[] = {
        {   "HSCB_SW_RESET",        REGPOKER_WRITE32,   HSCB_SW_RESET,          (0b1 << HSCB_SW_RESET_RST_i),
                                                                                                            0                           },
        {   "HSCB_SW_RESET",        REGPOKER_READ32,    HSCB_SW_RESET,          HSCB_SW_RESET_DFLT,         HSCB_SW_RESET_MASK          },
        {   "HSCB_SETTINGS",        REGPOKER_WRITE32,   HSCB_SETTINGS,          (0b0 << HSCB_SETTINGS_EN_HSCB_i),
                                                                                                            0                           },
        {   "HSCB_SETTINGS",        REGPOKER_WRITE32,   HSCB_SETTINGS,          HSCB_SETTINGS_DFLT,         HSCB_SETTINGS_MASK
                                                                                                         & ~HSCB_SETTINGS_EN_HSCB_mask  },
        {   "HSCB_IRQ_MASK",        REGPOKER_WRITE32,   HSCB_IRQ_MASK,          HSCB_IRQ_MASK_DFLT,         HSCB_IRQ_MASK_MASK          },
        {   "HSCB_TRANS_CLK",       REGPOKER_WRITE32,   HSCB_TRANS_CLK,         HSCB_TRANS_CLK_DFLT,        HSCB_TRANS_CLK_MASK         },
        {   "HSCB_TIMINGS",         REGPOKER_WRITE32,   HSCB_TIMINGS,           HSCB_TIMINGS_DFLT,          HSCB_TIMINGS_MASK           },
        {   "HSCB_INTERVAL",        REGPOKER_WRITE32,   HSCB_INTERVAL,          HSCB_INTERVAL_DFLT,         HSCB_INTERVAL_MASK          },
        {   "HSCB_TIME_CODE_INNER", REGPOKER_WRITE32,   HSCB_TIME_CODE_INNER,   HSCB_TIME_CODE_INNER_DFLT,  HSCB_TIME_CODE_INNER_MASK   },
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
        {   "HSCB_RDMA_DESC_ADDR",  REGPOKER_WRITE32,   HSCB_RDMA_DESC_ADDR,    HSCB_RDMA_DESC_ADDR_DFLT,   HSCB_XDMA_DESC_ADDR_MASK    },
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

static volatile uint32_t IRQ;

static volatile uint32_t hscb0_status;
static volatile uint32_t hscb1_status;
static volatile uint32_t hscb0_dma_status;
static volatile uint32_t hscb1_dma_status;

static volatile uint8_t __attribute__((section(".data"))) sys_0_rx[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"))) sys_0_tx[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"))) sys_1_rx[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"))) sys_1_tx[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"))) tx_data_0_im[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"))) tx_data_1_im[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"))) rx_data_0_im[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"))) rx_data_1_im[0x100] = {0} ;


static void handler( int irq, void *arg ) {
    //get interrupt source
    rumboot_printf( "IRQ arrived  \n" );
    rumboot_putstring("NON_CRITICAL int handler message\n");
//    rumboot_puthex (irq);
    if (irq == HSCB_UNDER_TEST_INT) {
        rumboot_putstring("HSCB_0_IRQ\n");
        hscb0_status = ioread32(HSCB_UNDER_TEST_BASE + HSCB_STATUS);
    }
    if (irq == HSCB_UNDER_TEST_DMA_INT) {
        rumboot_putstring("HSCB_0_DMA_IRQ\n");
        hscb0_dma_status = ioread32(HSCB_UNDER_TEST_BASE + HSCB_ADMA_CH_STATUS);
        rumboot_puthex(hscb0_dma_status);
        if (hscb0_dma_status & 0x00000001)
            hscb0_dma_status = ioread32(HSCB_UNDER_TEST_BASE + HSCB_RDMA_STATUS);
        else if (hscb0_dma_status & 0x00010000)
            hscb0_dma_status = ioread32(HSCB_UNDER_TEST_BASE + HSCB_WDMA_STATUS);
        rumboot_puthex(hscb0_dma_status);
    }
    if (irq == HSCB_SUPPLEMENTARY_INT) {
        rumboot_putstring("HSCB_1_IRQ\n");
        hscb1_status = ioread32(HSCB_SUPPLEMENTARY_BASE + HSCB_STATUS);
        rumboot_puthex (hscb1_status);
    }
    if (irq == HSCB_SUPPLEMENTARY_DMA_INT) {
        rumboot_putstring("HSCB_1_DMA_IRQ\n");
        hscb1_dma_status = ioread32(HSCB_SUPPLEMENTARY_BASE + HSCB_ADMA_CH_STATUS);
        isync();
        msync();
        rumboot_puthex(hscb1_dma_status);
        if (hscb1_dma_status & 0x00000001)
            hscb1_dma_status = ioread32(HSCB_SUPPLEMENTARY_BASE + HSCB_RDMA_STATUS);
        else if (hscb1_dma_status & 0x00010000)
            hscb1_dma_status = ioread32(HSCB_SUPPLEMENTARY_BASE + HSCB_WDMA_STATUS);
    }
    rumboot_printf( "Clear interrupts\n" );
    IRQ = 1;
}

static uint32_t check_hscb_func(uint32_t base_addr, uint32_t supplementary_base_addr){
    rumboot_printf("Check functionality...\n");

// setup of interrupts
    IRQ = 0;
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

        rumboot_putstring( "------- HSCB Short ACCESS test -------\n" );
    // Local variables
    int i = 0;
    int cnt = 0;

    bool act0 = 0;
    bool interrupt = 0;
    bool end = 0;
    bool valid = 0;
    uint32_t len = 0;

    hscb0_status = 0;
    hscb1_status = 0;
    hscb0_dma_status = 0;
    hscb1_dma_status = 0;

    uint32_t sys_addr_0_rx = (uint32_t)sys_0_rx; //
    uint32_t sys_addr_0_tx = (uint32_t)sys_0_tx; //
    uint32_t sys_addr_1_rx = (uint32_t)sys_1_rx; //
    uint32_t sys_addr_1_tx = (uint32_t)sys_1_tx; //
    uint32_t cur_tbl_addr = 0;
    uint32_t tx_data_addr_0_im  = (uint32_t)tx_data_0_im; //
    uint32_t tx_data_addr_1_im  = (uint32_t)tx_data_1_im; //
    uint32_t rx_data_addr_0_im  = (uint32_t)rx_data_0_im; //
    uint32_t rx_data_addr_1_im  = (uint32_t)rx_data_1_im; //

    uint8_t tmp_data = 0;


    // Set data for HSCB0
    uint8_t data_in_0_1[11] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a};
    uint8_t data_out_0_1[11] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    // Set data for HSCB1
    uint8_t data_in_0_3[13] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c};
    uint8_t data_out_0_3[13] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    // Set descriptors table HSCB0
    // -- Set Tx data
    cur_tbl_addr = set_desc(sys_addr_0_tx, sizeof(data_in_0_1), tx_data_addr_0_im, data_in_0_1, 1, 0, 1, 1);
    cur_tbl_addr = set_desc(cur_tbl_addr, 0, 0, 0, 0, 0, 0, 0); // Bad descriptor
    // -- Set RDMA
    iowrite32( sys_addr_0_tx,                   base_addr + HSCB_RDMA_SYS_ADDR);
    iowrite32( cur_tbl_addr - sys_addr_0_tx,    base_addr + HSCB_RDMA_TBL_SIZE);
    // -- Set Rx data
    cur_tbl_addr = set_desc(sys_addr_0_rx, sizeof(data_out_0_3), rx_data_addr_0_im, data_out_0_3, 1, 0, 1, 1);
    cur_tbl_addr = set_desc(cur_tbl_addr, 0, 0, 0, 0, 0, 0, 0); // Bad descriptor
    // -- Set RDMA
    iowrite32( sys_addr_0_rx,                   base_addr + HSCB_WDMA_SYS_ADDR);
    iowrite32( cur_tbl_addr - sys_addr_0_rx,    base_addr + HSCB_WDMA_TBL_SIZE);

    // Set descriptors table HSCB1
    // -- Set Tx data
    cur_tbl_addr = set_desc(sys_addr_1_tx, sizeof(data_in_0_3), tx_data_addr_1_im, data_in_0_3, 1, 0, 1, 1);
    cur_tbl_addr = set_desc(cur_tbl_addr, 0, 0, 0, 0, 0, 0, 1); // Bad descriptor
  // -- Set RDMA
    iowrite32( sys_addr_1_tx,                   supplementary_base_addr + HSCB_RDMA_SYS_ADDR);
    iowrite32( cur_tbl_addr - sys_addr_1_tx,    supplementary_base_addr + HSCB_RDMA_TBL_SIZE);
    // -- Set Rx data
    cur_tbl_addr = set_desc(sys_addr_1_rx, sizeof(data_out_0_1), rx_data_addr_1_im, data_out_0_1, 1, 0, 1, 1);
    cur_tbl_addr = set_desc(cur_tbl_addr, 0, 0, 0, 0, 0, 0, 1); // Bad descriptor
    // -- Set RDMA
    iowrite32( sys_addr_1_rx,                   supplementary_base_addr + HSCB_WDMA_SYS_ADDR);
    iowrite32( cur_tbl_addr - sys_addr_1_rx,    supplementary_base_addr + HSCB_RDMA_TBL_SIZE);
    // Enable HSCB0 and HSCB1

    iowrite32(0x200,        base_addr + HSCB_IRQ_MASK);
    iowrite32(0x200,        supplementary_base_addr + HSCB_IRQ_MASK);
    iowrite32(0x01000001,   base_addr + HSCB_SETTINGS);
    iowrite32(0x01000001,   supplementary_base_addr + HSCB_SETTINGS);
    // Wait connecting
    rumboot_putstring( "Wait HSCB0 and HSCB1 enable\n" );
    while (!(hscb0_status & hscb1_status)){
        if (cnt == MAX_ATTEMPTS) {
            rumboot_putstring( "Wait interrupt Time-out\n" );
            return 1;
        }
        else
            cnt += 1;
    }
    cnt = 0;
    hscb0_status = 0;
    hscb1_status = 0;

    rumboot_putstring( "HSCB link has enabled\n" );
    // Setting DMA for HSCB0 and HSCB1
    iowrite32( 0xf,     base_addr + HSCB_ARLEN);
    iowrite32( 0xf,     base_addr + HSCB_AWLEN);
    iowrite32( 0xf,     supplementary_base_addr + HSCB_ARLEN);
    iowrite32( 0xf,     supplementary_base_addr + HSCB_AWLEN);
    // Enable DMA for HSCB0 and HSCB1
    rumboot_putstring( "Start work!\n" );
    iowrite32( 0x70000000,  base_addr + HSCB_RDMA_SETTINGS);
    iowrite32( 0x70000004,  base_addr + HSCB_WDMA_SETTINGS);
    iowrite32( 0x70000000,  supplementary_base_addr + HSCB_RDMA_SETTINGS);
    iowrite32( 0x70000004,  supplementary_base_addr + HSCB_WDMA_SETTINGS);
    rumboot_putstring( "Wait HSCB0 and HSCB1 finish work\n" );
    while (!(hscb0_dma_status & hscb1_dma_status)){
        if (cnt == MAX_ATTEMPTS) {
            rumboot_putstring( "Wait interrupt Time-out\n" );
            return 1;
        }
        else
            cnt += 1;
    }
    cnt = 0;
    hscb0_dma_status = 0;
    hscb1_dma_status = 0;

    rumboot_putstring( "Finish work!\n" );
    rumboot_putstring( "HSCB1 to HSCB0 descriptor #1\n" );
    cur_tbl_addr = get_desc(sys_addr_0_rx, data_out_0_3, &len, &act0, &interrupt, &end, &valid, 1);
    rumboot_putstring( "Length:" );
    rumboot_puthex (len);
    for (i=0; i<=len-1; i++) {
        tmp_data = ioread8(TX_DATA_ADDR_1+i);
        rumboot_putstring("Expected data: ");
        rumboot_puthex(tmp_data);
        rumboot_putstring("Obtained data: ");
        rumboot_puthex (data_out_0_3[i]);
        TEST_ASSERT ((data_out_0_3[i] == tmp_data), "Data compare ERROR!!!\n" );
    }

    rumboot_putstring( "HSCB0 to HSCB1 descriptor #1\n" );
    cur_tbl_addr = get_desc(sys_addr_1_rx, data_out_0_1, &len, &act0, &interrupt, &end, &valid, 1);
    rumboot_putstring( "Length:" );
    rumboot_puthex (len);
    for (i=0; i<=len-1; i++) {
        tmp_data = ioread8(TX_DATA_ADDR_0+i);
        rumboot_putstring("Expected data: ");
        rumboot_puthex(tmp_data);
        rumboot_putstring("Obtained data: ");
        rumboot_puthex (data_out_0_1[i]);
        TEST_ASSERT ((data_out_0_1[i] == tmp_data), "Data compare ERROR!!!\n" );
    }

    return 0;
}


int main() {
    uint32_t result = 0x0;

    rumboot_printf( "Check HSCB (0x%x) \n", HSCB_UNDER_TEST_BASE );

#ifdef CHECK_REGS
    result += check_hscb_default_val( HSCB_UNDER_TEST_BASE );
    result += check_hscb_regs( HSCB_UNDER_TEST_BASE );
#endif
    result += check_hscb_func( HSCB_UNDER_TEST_BASE, HSCB_SUPPLEMENTARY_BASE );
    //result += check_gpio_func( HSCB_UNDER_TEST_BASE, 0x2A );

    return result;
}
