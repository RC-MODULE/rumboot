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
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>


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

#define CURRENT_MEMORY_RX_0 ((uint32_t)data_out_0_1)
#define CURRENT_MEMORY_TX_0 ((uint32_t)data_in_0_1)
#define CURRENT_MEMORY_RX_1 ((uint32_t)data_out_0_3)
#define CURRENT_MEMORY_TX_1 ((uint32_t)data_in_0_3)

#define IM0_TX_0        IM0_BASE
#define IM0_TX_1        IM0_TX_0 + DATA_SIZE_0
#define IM0_RX_0        IM0_TX_1 + DATA_SIZE_1
#define IM0_RX_1        IM0_RX_0 + DATA_SIZE_0


#define IM1_TX_0        IM1_BASE
#define IM1_TX_1        IM1_TX_0 + DATA_SIZE_0
#define IM1_RX_0        IM1_TX_1 + DATA_SIZE_1
#define IM1_RX_1        IM1_RX_0 + DATA_SIZE_0


#define SSRAM_TX_0      SSRAM_BASE
#define SSRAM_TX_1      SSRAM_TX_0 + DATA_SIZE_0
#define SSRAM_RX_0      SSRAM_TX_1 + DATA_SIZE_1
#define SSRAM_RX_1      SSRAM_RX_0 + DATA_SIZE_0


#define SDRAM_TX_0      SDRAM_BASE
#define SDRAM_TX_1      SDRAM_TX_0 + DATA_SIZE_0
#define SDRAM_RX_0      SDRAM_TX_1 + DATA_SIZE_1
#define SDRAM_RX_1      SDRAM_RX_0 + DATA_SIZE_0

#ifndef DATA_SIZE_0
#define DATA_SIZE_0 0x10
#endif
#ifndef DATA_SIZE_1
#define DATA_SIZE_1 0x20
#endif
//Here are defined addresses, for using different memories use addresses from defines above
//Note that for current memory (say, IM0 for IRAM configuration) you should use CURRENT_MEMORY_* addresses
//in order not to erase code
#ifndef TX_DATA_ADDR_0
#define TX_DATA_ADDR_0   CURRENT_MEMORY_TX_0 //Here we have Mem0 address from chain Mem0->SW0->SW1->Mem1
#endif
#ifndef TX_DATA_ADDR_1
#define TX_DATA_ADDR_1   CURRENT_MEMORY_TX_1 //Here we have Mem1 address from chain Mem1->SW1->SW0->Mem0
#endif
#ifndef RX_DATA_ADDR_0
#define RX_DATA_ADDR_0   CURRENT_MEMORY_RX_0 //Here we have Mem1 address from chain Mem0->SW0->SW1->Mem1
#endif
#ifndef RX_DATA_ADDR_1
#define RX_DATA_ADDR_1   CURRENT_MEMORY_RX_1 //Here we have Mem0 address from chain Mem1->SW1->SW0->Mem0
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

static volatile uint8_t __attribute__((section(".data"),aligned(8))) tx_desc_0_im[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"),aligned(8))) tx_desc_1_im[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"),aligned(8))) rx_desc_0_im[0x100] = {0} ;
static volatile uint8_t __attribute__((section(".data"),aligned(8))) rx_desc_1_im[0x100] = {0} ;

static volatile uint8_t __attribute__((section(".data"))) data_in_0_1[DATA_SIZE_0]  = {0x0};
static volatile uint8_t __attribute__((section(".data"))) data_out_0_1[DATA_SIZE_0] = {0x0};
static volatile uint8_t __attribute__((section(".data"))) data_in_0_3[DATA_SIZE_1]  = {0x0};
static volatile uint8_t __attribute__((section(".data"))) data_out_0_3[DATA_SIZE_1] = {0x0};

static volatile uint32_t hscb0_status;
static volatile uint32_t hscb1_status;
static volatile uint32_t hscb0_dma_status;
static volatile uint32_t hscb1_dma_status;
static volatile bool hscb0_link_established;
static volatile bool hscb1_link_established;

hscb_instance_t  hscb_cfg[2];

static void handler( int irq, void *arg ) {
    //get interrupt source
    rumboot_printf( "IRQ arrived  \n" );
    rumboot_putstring("NON_CRITICAL int handler message\n");
//    rumboot_puthex (irq);
    if (irq == HSCB_UNDER_TEST_INT) {
        rumboot_putstring("HSCB_0_IRQ\n");
        hscb0_status = hscb_get_status(HSCB_UNDER_TEST_BASE);
        if(!hscb0_link_established)
            hscb0_link_established = hscb0_status & (1 << HSCB_STATUS_ACTIVE_LINK_i);
        rumboot_puthex (hscb0_status );
    }
    if (irq == HSCB_UNDER_TEST_DMA_INT) {
        rumboot_putstring("HSCB_0_DMA_IRQ\n");
        hscb0_dma_status = hscb_get_adma_ch_status(HSCB_UNDER_TEST_BASE );
        rumboot_puthex(hscb0_dma_status);
        if (hscb0_dma_status & HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask)
            hscb0_dma_status = hscb_get_rdma_status(HSCB_UNDER_TEST_BASE);
        else if (hscb0_dma_status & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
            hscb0_dma_status = hscb_get_wdma_status(HSCB_UNDER_TEST_BASE);
        rumboot_puthex(hscb0_dma_status);
    }
    if (irq == HSCB_SUPPLEMENTARY_INT) {
        rumboot_putstring("HSCB_1_IRQ\n");
        hscb1_status = hscb_get_status(HSCB_SUPPLEMENTARY_BASE);
        if(!hscb1_link_established)
            hscb1_link_established = hscb1_status & (1 << HSCB_STATUS_ACTIVE_LINK_i);
        rumboot_puthex (hscb1_status);
    }
    if (irq == HSCB_SUPPLEMENTARY_DMA_INT) {
        rumboot_putstring("HSCB_1_DMA_IRQ\n");
        hscb1_dma_status = hscb_get_adma_ch_status(HSCB_SUPPLEMENTARY_BASE );
        isync();
        msync();
        rumboot_puthex(hscb1_dma_status);
        if (hscb1_dma_status & HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask)
            hscb1_dma_status = hscb_get_rdma_status(HSCB_SUPPLEMENTARY_BASE);
        else if (hscb1_dma_status & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
            hscb1_dma_status = hscb_get_wdma_status(HSCB_SUPPLEMENTARY_BASE);
    }
}

static void set_test_data(uint32_t base_addr, uint32_t length, int increment){
    uint8_t data = DATA_INITIAL_VALUE;
    if ((base_addr >= NOR_BASE) && (base_addr <= IM0_BASE))
        rumboot_putstring("Oops! NOR initialization is still a To-Do in this test!");
    else
    for(uint32_t i = 0; i < length; ++i){
        iowrite8(data,(base_addr+i));
        data += increment;
    }
}
inline static void print_hscb_descriptor(uint32_t addr){
    rumboot_printf("descriptor address: \n%x\nfirst word == \n%0x\nsecond word == \n%0x\n",
            addr,hscb_change_endian(*((uint32_t*)addr)),hscb_change_endian(*(((uint32_t*)addr)+1)));
}

void init_hscb_cfg_short(hscb_instance_t* hscb_inst)
{
    (hscb_inst + 0)->src_hscb_base_addr   = HSCB_UNDER_TEST_BASE;
    (hscb_inst + 0)->dst_hscb_base_addr   = HSCB_SUPPLEMENTARY_BASE;
    (hscb_inst + 0)->src_addr             = (uint32_t*)TX_DATA_ADDR_0;
    (hscb_inst + 0)->dst_addr             = (uint32_t*)RX_DATA_ADDR_1;
    (hscb_inst + 0)->src_size             = DATA_SIZE_0;
    (hscb_inst + 0)->dst_size             = DATA_SIZE_1;
    (hscb_inst + 0)->tx_descr_addr        = (uint32_t)tx_desc_0_im;
    (hscb_inst + 0)->rx_descr_addr        = (uint32_t)rx_desc_0_im;

    (hscb_inst + 1)->src_hscb_base_addr   = HSCB_SUPPLEMENTARY_BASE;
    (hscb_inst + 1)->dst_hscb_base_addr   = HSCB_UNDER_TEST_BASE;
    (hscb_inst + 1)->src_addr             = (uint32_t*)TX_DATA_ADDR_1;
    (hscb_inst + 1)->dst_addr             = (uint32_t*)RX_DATA_ADDR_0;
    (hscb_inst + 1)->src_size             = DATA_SIZE_1;
    (hscb_inst + 1)->dst_size             = DATA_SIZE_0;
    (hscb_inst + 1)->tx_descr_addr        = (uint32_t)tx_desc_1_im;
    (hscb_inst + 1)->rx_descr_addr        = (uint32_t)rx_desc_1_im;

    //Some dirty hack to pass around [-Werror=unused-variable] for transfers not in current memory
    data_in_0_1[0] = 0xff;
    data_out_0_1[0] = data_in_0_1[0] - 1;
    data_in_0_3[0] = data_out_0_3[0] - 1;
    data_out_0_3[0] = data_in_0_3[0] - 1;
}

static uint32_t check_hscb_func(uint32_t base_addr, uint32_t supplementary_base_addr){
    rumboot_printf("Check functionality...\n");

// setup of interrupts
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

    uint8_t tmp_data = 0;

    init_hscb_cfg_short(hscb_cfg);

    TEST_ASSERT(((uint32_t)(hscb_cfg->dst_addr) < NOR_BASE) || ((uint32_t)(hscb_cfg->dst_addr) >= IM0_BASE),
        "RX_DATA_ADDR_0 - destination address cannot be NOR!");
    TEST_ASSERT(((uint32_t)((hscb_cfg + 1)->dst_addr) < NOR_BASE) || ((uint32_t)((hscb_cfg + 1)->dst_addr) >= IM0_BASE),
        "RX_DATA_ADDR_1 - destination address cannot be NOR!");


    // Set data for HSCB0
    rumboot_putstring("(uint32_t)hscb_cfg->src_addr == ");
    rumboot_puthex((uint32_t)hscb_cfg->src_addr);
    rumboot_putstring("(uint32_t)hscb_cfg->dst_addr == ");
    rumboot_puthex((uint32_t)hscb_cfg->dst_addr);
    set_test_data(TX_DATA_ADDR_0,DATA_SIZE_0,INCREMENT_0);
    set_test_data(RX_DATA_ADDR_0,DATA_SIZE_0,0);
    // Set data for HSCB1
    rumboot_putstring("(uint32_t)((hscb_cfg + 1)->src_addr) == ");
    rumboot_puthex((uint32_t)((hscb_cfg + 1)->src_addr));
    rumboot_putstring("(uint32_t)((hscb_cfg + 1)->dst_addr) == ");
    rumboot_puthex((uint32_t)((hscb_cfg + 1)->dst_addr));
    set_test_data(TX_DATA_ADDR_1,DATA_SIZE_1,INCREMENT_1);
    set_test_data(RX_DATA_ADDR_1,DATA_SIZE_1,0);
    msync();
    hscb_config_for_receive_and_transmit(hscb_cfg);
    hscb_config_for_receive_and_transmit((hscb_cfg + 1));
    print_hscb_descriptor(hscb_cfg->tx_descr_addr);
    print_hscb_descriptor(hscb_cfg->rx_descr_addr);
    print_hscb_descriptor((hscb_cfg + 1)->tx_descr_addr);
    print_hscb_descriptor((hscb_cfg + 1)->rx_descr_addr);

    // Enable HSCB0 and HSCB1
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),        hscb_cfg->src_hscb_base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),        (hscb_cfg + 1)->src_hscb_base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        hscb_cfg->src_hscb_base_addr + HSCB_SETTINGS);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        (hscb_cfg + 1)->src_hscb_base_addr + HSCB_SETTINGS);
    // Wait connecting
    rumboot_putstring( "Wait HSCB0 and HSCB1 enable\n" );
    while (!(hscb0_link_established & hscb1_link_established)){
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
    // Enable DMA for HSCB0 and HSCB1
    rumboot_putstring( "Start work!\n" );
    hscb_run_rdma(hscb_cfg->src_hscb_base_addr);
    hscb_run_wdma(hscb_cfg->src_hscb_base_addr);
    hscb_run_rdma((hscb_cfg + 1)->src_hscb_base_addr);
    hscb_run_wdma((hscb_cfg + 1)->src_hscb_base_addr);
    msync();
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
    hscb_get_desc((hscb_cfg->rx_descr_addr), (uint8_t*)RX_DATA_ADDR_1, &len, &act0, &interrupt, &end, &valid, 1);
    rumboot_putstring( "Length:" );
    rumboot_puthex (len);
    for (i=0; i<=len-1; i++) {
        tmp_data = ioread8(TX_DATA_ADDR_1+i);
        rumboot_puthex(RX_DATA_ADDR_1+i);
        rumboot_putstring("Expected data: ");
        rumboot_puthex(tmp_data);
        rumboot_putstring("Obtained data: ");
        rumboot_puthex (ioread8(RX_DATA_ADDR_1+i));
        TEST_ASSERT ((ioread8(RX_DATA_ADDR_1+i) == tmp_data), "Data compare ERROR!!!\n" );
    }

    rumboot_putstring( "HSCB0 to HSCB1 descriptor #1\n" );
    hscb_get_desc(((hscb_cfg + 1)->rx_descr_addr), (uint8_t*)RX_DATA_ADDR_0, &len, &act0, &interrupt, &end, &valid, 1);
    rumboot_putstring( "Length:" );
    rumboot_puthex (len);
    for (i=0; i<=len-1; i++) {
        tmp_data = ioread8(TX_DATA_ADDR_0+i);
        rumboot_puthex(RX_DATA_ADDR_0+i);
        rumboot_putstring("Expected data: ");
        rumboot_puthex(tmp_data);
        rumboot_putstring("Obtained data: ");
        rumboot_puthex (ioread8(RX_DATA_ADDR_0+i));
        TEST_ASSERT ((ioread8(RX_DATA_ADDR_0+i) == tmp_data), "Data compare ERROR!!!\n" );
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
    emi_init(DCR_EM2_EMI_BASE);
    result += check_hscb_func( HSCB_UNDER_TEST_BASE, HSCB_SUPPLEMENTARY_BASE );
    //result += check_gpio_func( HSCB_UNDER_TEST_BASE, 0x2A );

    return result;
}
