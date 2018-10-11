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
#include <regs/regs_hscb.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
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

#define CURRENT_MEMORY_RX_0 IM0_BASE
#define CURRENT_MEMORY_TX_0 IM0_BASE
#define CURRENT_MEMORY_RX_1 IM0_BASE
#define CURRENT_MEMORY_TX_1 IM0_BASE

#define IM0_TX_0        IM0_BASE
#define IM0_TX_1        IM0_BASE
#define IM0_RX_0        IM0_BASE
#define IM0_RX_1        IM0_BASE


#define IM1_TX_0        IM1_BASE
#define IM1_TX_1        IM1_BASE
#define IM1_RX_0        IM1_BASE
#define IM1_RX_1        IM1_BASE


#define SSRAM_TX_0      SSRAM_BASE
#define SSRAM_TX_1      SSRAM_BASE
#define SSRAM_RX_0      SSRAM_BASE
#define SSRAM_RX_1      SSRAM_BASE


#define SDRAM_TX_0      SDRAM_BASE
#define SDRAM_TX_1      SDRAM_BASE
#define SDRAM_RX_0      SDRAM_BASE
#define SDRAM_RX_1      SDRAM_BASE

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

#ifdef HSCB_SHORT_TEST

    #ifndef TX_0_HEAP_NAME
    #define TX_0_HEAP_NAME "IM0"
    #endif

    #ifndef RX_0_HEAP_NAME
    #define RX_0_HEAP_NAME "IM0"
    #endif

    #ifndef TX_1_HEAP_NAME
    #define TX_1_HEAP_NAME "IM0"
    #endif

    #ifndef RX_1_HEAP_NAME
    #define RX_1_HEAP_NAME "IM0"
    #endif

#else

    #ifndef TX_0_HEAP_NAME
    #define TX_0_HEAP_NAME "IM0"
    #endif

    #ifndef RX_0_HEAP_NAME
    #define RX_0_HEAP_NAME "SSRAM"
    #endif

    #ifndef TX_1_HEAP_NAME
    #define TX_1_HEAP_NAME "IM0"
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
    #define RX_4_HEAP_NAME "IM0"
    #endif

    #ifndef TX_5_HEAP_NAME
    #define TX_5_HEAP_NAME "SSRAM"
    #endif

    #ifndef RX_5_HEAP_NAME
    #define RX_5_HEAP_NAME "IM0"
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
#define DEFAULT_HEAP_ID 0

//static volatile uint8_t __attribute__((section(".data"),aligned(8))) tx_desc_0_im[0x100] = {0} ;
//static volatile uint8_t __attribute__((section(".data"),aligned(8))) tx_desc_1_im[0x100] = {0} ;
//static volatile uint8_t __attribute__((section(".data"),aligned(8))) rx_desc_0_im[0x100] = {0} ;
//static volatile uint8_t __attribute__((section(".data"),aligned(8))) rx_desc_1_im[0x100] = {0} ;

//static volatile hscb_packed_descr_struct_t __attribute__((section(".data"),aligned(8))) static_descriptors[DESCRIPTOR_TABLES_COUNT][2];

//static uint8_t*  data_areas[COUNT_AREAS];/*from even to odd*/
//static uint32_t  data_area_sizes[COUNT_AREAS/2] = {0x10, 0x10};

static volatile uint32_t hscb0_status;
static volatile uint32_t hscb1_status;
static volatile uint32_t hscb0_dma_status;
static volatile uint32_t hscb1_dma_status;
static volatile bool hscb0_link_established;
static volatile bool hscb1_link_established;


static void handler( int irq, void *arg ) {
    //get interrupt source
    rumboot_printf( "IRQ arrived  \n" );
    rumboot_putstring("NON_CRITICAL int handler message\n");
//    rumboot_puthex (irq);
    if (irq == HSCB_UNDER_TEST_INT) {
        rumboot_putstring("HSCB_0_IRQ\n");
        rumboot_puthex (hscb_get_status(HSCB_UNDER_TEST_BASE) );
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
        rumboot_puthex (hscb_get_status(HSCB_SUPPLEMENTARY_BASE));
    }
    if (irq == HSCB_SUPPLEMENTARY_DMA_INT) {
        rumboot_putstring("HSCB_1_DMA_IRQ\n");
        hscb1_dma_status = hscb_get_adma_ch_status(HSCB_SUPPLEMENTARY_BASE );
        rumboot_puthex(hscb1_dma_status);
        if (hscb1_dma_status & HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask)
            hscb1_dma_status = hscb_get_rdma_status(HSCB_SUPPLEMENTARY_BASE);
        else if (hscb1_dma_status & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
            hscb1_dma_status = hscb_get_wdma_status(HSCB_SUPPLEMENTARY_BASE);
        rumboot_puthex(hscb1_dma_status);
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
                                        RX_3_HEAP_NAME,
                                        TX_3_HEAP_NAME,
                                        RX_4_HEAP_NAME,
                                        TX_4_HEAP_NAME,
                                        RX_5_HEAP_NAME,
                                        TX_5_HEAP_NAME,};
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
    TEST_ASSERT(((*data_areas) != NULL), "Cannot allocate from default heap!");
    for (i = 0; i < count_of_memory_areas; ++i){
        (*data_areas)[i] = rumboot_malloc_from_named_heap_aligned(memory_heap_names[i], memory_area_sizes[i >> 1], 0x8);
        if((*data_areas)[i] == NULL){
            rumboot_putstring("Cannot allocate from heap with name ");
            rumboot_putstring((const char * const)&(memory_heap_names[i]));
            TEST_ASSERT(((*data_areas)[i] != NULL), "Cannot work without this memory ;( Exiting...");
        }
        rumboot_puthex(i);
        rumboot_puthex((uint32_t)(*data_areas)[i]);

    }
    rumboot_putstring("end prepare_memory_areas");
    for(i = 0; i < (count_of_memory_areas >> 1); ++i){
        (*data_area_sizes)[i] = memory_area_sizes[i];
//        rumboot_putstring("data_areas[i] == ");
//        rumboot_puthex(data_areas[i]);
    }
    return count_of_memory_areas;
}

static void set_test_data(uint32_t base_addr, uint32_t length, int increment,bool nor_change_endian){
    uint8_t data = DATA_INITIAL_VALUE;
    if ((base_addr >= NOR_BASE) && (base_addr <= NOR_BASE + NOR_SIZE)){
        /*TODO: check me*/
        uint32_t data32 = 0;
        for(uint32_t i = 0; i < ((length >> 2) << 2); i += 4){
            data32 |= data << 24;
            data += increment;
            data32 |= data << 16;
            data += increment;
            data32 |= data << 8;
            data += increment;
            data32 |= data;
            if(nor_change_endian)
                data32 = HSCB_CHANGE_ENDIAN_WORD(data32);
            nor_write32(data32, base_addr + i);
            data32 = 0;
        }
        for(uint32_t i = 1; i <= (length % 4); ++i){
            data32 |= data << ((4 - i) << 3);
            data += increment;
        }
        if(nor_change_endian)
            data32 = HSCB_CHANGE_ENDIAN_WORD(data32);
        nor_write32(data32, base_addr + ((length >> 2) << 2));
        data32 = 0;
    }
    else
    for(uint32_t i = 0; i < length; ++i){
        iowrite8(data,(base_addr+i));
        data += increment;
    }
}

void set_test_data_multiple(    uint8_t** data_areas,
                                uint32_t* data_area_sizes,
                                uint32_t  count_of_memory_areas,
                                bool nor_change_endian){
    int i;
#ifdef HSCB_SHORT_TEST
    uint32_t increments[] = {   INCREMENT_0,
                                0,
                                INCREMENT_1,
                                0};
#else
    uint32_t increments[] = {   INCREMENT_0,
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
        rumboot_puthex(i);
        rumboot_puthex((uint32_t)(*(data_areas + i)));
        set_test_data((uint32_t)(*(data_areas + i)),data_area_sizes[i >> 1],increments[i],nor_change_endian);
    }
    rumboot_putstring("end set_test_data_multiple");
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
    rumboot_puthex((uint32_t)*array_of_descriptors);
    TEST_ASSERT(((*array_of_descriptors) != NULL), "Cannot allocate from default heap!");
    for(i = 0; i < counts_of_descriptor_tables; ++i){
        (*array_of_descriptors)[i] = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
                ( hscb_get_tbl_len_by_count(counts_of_descriptors[i])),
                0x8);
        TEST_ASSERT(((*array_of_descriptors)[i] != NULL), "Cannot allocate from default heap!");
        rumboot_puthex(i);
        rumboot_puthex((uint32_t)(*array_of_descriptors)[i]);
        rumboot_puthex(counts_of_descriptors[i]);
    }
    rumboot_putstring("end prepare_descriptor_areas");
}

inline static void print_hscb_descriptor(hscb_packed_descr_struct_t descriptor){
    rumboot_printf("descriptor address: \n%x\nfirst word == \n%x\nsecond word == \n%x\n",
            &descriptor,hscb_change_endian(descriptor.start_address),hscb_change_endian(descriptor.length_attr));
}
void init_hscb_cfg( uint8_t**                       data_areas,
                    uint32_t*                       data_area_sizes,
                    hscb_packed_descr_struct_t**    hscb_descr,
                    uint32_t*                       count_of_descriptors,
                    uint32_t                        count_descr_tables,
                    bool                            change_endian){
    int i,j;
    uint32_t count_of_processed_descriptors = 0;
    rumboot_putstring("init_hscb_cfg");
    for (i = 0; i < count_descr_tables; ++i){
        for (j = 0; j < count_of_descriptors[i]; ++j){
            rumboot_puthex((i << 16) | j );
            rumboot_puthex((uint32_t)((&(*(hscb_descr + i))[j])));
            (*(hscb_descr + i))[j].start_address =
                    (change_endian)  ? HSCB_CHANGE_ENDIAN_WORD((uint32_t)(*(data_areas + i + j)))
                                     : (uint32_t)(*(data_areas + i + j));
            (*(hscb_descr + i))[j].length_attr =
                    HSCB_CREATE_DESCRIPTOR_LEN_ATTR_ENDIAN_EXE(
                                             data_area_sizes[(i + (j * count_descr_tables)) >> 1],
                                             HSCB_ACT_TRAN,
                                             HSCB_ACT0_LAST,
                                             HSCB_DESCR_ITRPT_ON,
                                             HSCB_DESCR_VALID,
                                             change_endian) ;
        }
        rumboot_puthex(count_of_descriptors[i]);
        rumboot_puthex((uint32_t)(&(*(hscb_descr + i))[count_of_descriptors[i]]));
        (*(hscb_descr + i))[count_of_descriptors[i]].start_address = 0;
        (*(hscb_descr + i))[count_of_descriptors[i]].length_attr = 0;
        count_of_processed_descriptors += count_of_descriptors[i] ;
    }
    rumboot_putstring("end init_hscb_cfg");
}

void set_descriptor_tables( uint32_t base_addr,
                            uint32_t supplementary_base_addr,
                            hscb_packed_descr_struct_t** hscb_descr,
                            uint32_t* descr_table_length,
                            uint32_t count_descr_tables){

    rumboot_putstring("set_descriptor_tables");

    rumboot_putstring("descr_table_length[0]");
    rumboot_puthex(descr_table_length[0]);
    rumboot_putstring("hscb_get_tbl_len_by_count(descr_table_length[0])");
    rumboot_puthex(hscb_get_tbl_len_by_count(descr_table_length[0]));

    hscb_set_rdma_tbl_size(base_addr, hscb_get_tbl_len_by_count(descr_table_length[0]));
    hscb_set_wdma_tbl_size(base_addr, hscb_get_tbl_len_by_count(descr_table_length[3]));

    rumboot_putstring("(*(hscb_descr + 0))[0]");
    print_hscb_descriptor((*(hscb_descr + 0))[0]);
    rumboot_putstring("rumboot_virt_to_dma((*(hscb_descr + 0)))");
    rumboot_puthex(rumboot_virt_to_dma((*(hscb_descr + 0))));

    hscb_set_rdma_sys_addr(base_addr, rumboot_virt_to_dma((*(hscb_descr + 0))));
    hscb_set_wdma_sys_addr(base_addr, rumboot_virt_to_dma((*(hscb_descr + 3))));

    hscb_set_wdma_tbl_size(supplementary_base_addr, hscb_get_tbl_len_by_count(descr_table_length[1]));
    hscb_set_rdma_tbl_size(supplementary_base_addr, hscb_get_tbl_len_by_count(descr_table_length[2]));

    rumboot_putstring("(*(hscb_descr + 1))[0]");
    print_hscb_descriptor((*(hscb_descr + 1))[0]);
    rumboot_putstring("rumboot_virt_to_dma((*(hscb_descr + 1)))");
    rumboot_puthex(rumboot_virt_to_dma((*(hscb_descr + 1))));

    hscb_set_wdma_sys_addr(supplementary_base_addr, rumboot_virt_to_dma((*(hscb_descr + 1))));
    hscb_set_rdma_sys_addr(supplementary_base_addr, rumboot_virt_to_dma((*(hscb_descr + 2))));

    rumboot_putstring("end set_descriptor_tables");
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
    int i = 0;
    int j = 0;
    int cnt = 0;
    hscb_descr_struct_t hscb_descr;

    hscb0_status = 0;
    hscb1_status = 0;
    hscb0_dma_status = 0;
    hscb1_dma_status = 0;

    uint8_t expected_data = 0;
    uint8_t obtained_data = 0;

    rumboot_printf("Check functionality...\n");


    rumboot_putstring("address of descriptor_counts");
    rumboot_puthex((uint32_t)descriptor_counts);
    prepare_descriptor_areas(&descriptors,descriptor_counts,DESCRIPTOR_TABLES_COUNT);
    rumboot_putstring("address of descriptors");
    rumboot_puthex((uint32_t)descriptors);
    init_hscb_cfg( data_areas, data_area_sizes, descriptors, descriptor_counts,DESCRIPTOR_TABLES_COUNT, HSCB_ROTATE_BYTES_ENABLE);
    rumboot_putstring("printing descriptors in cycle");

    for (i = 0; i < DESCRIPTOR_TABLES_COUNT; ++i){
        rumboot_puthex(i);
        rumboot_puthex(descriptor_counts[i]);
        rumboot_puthex((uint32_t)(descriptors[i]));
        for(j = 0; j < descriptor_counts[i]; ++j){
            rumboot_puthex((i << 16) | j);
            rumboot_puthex((uint32_t)((*(descriptors + i))+j));
            rumboot_puthex((*(descriptors + i))[j].start_address);
            rumboot_puthex((*(descriptors + i))[j].length_attr);
        }
    }
    rumboot_putstring("end printing descriptors in cycle");
    print_hscb_descriptor((*(descriptors + 0))[0]);//TX0
    print_hscb_descriptor((*(descriptors + 1))[0]);//RX0
    print_hscb_descriptor((*(descriptors + 2))[0]);//TX1
    print_hscb_descriptor((*(descriptors + 3))[0]);//RX1

    set_descriptor_tables(base_addr, supplementary_base_addr, descriptors,descriptor_counts,DESCRIPTOR_TABLES_COUNT);
    hscb_set_max_speed(base_addr);
    hscb_set_max_speed(supplementary_base_addr);

    // Enable HSCB0 and HSCB1
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    supplementary_base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        base_addr + HSCB_SETTINGS);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        supplementary_base_addr + HSCB_SETTINGS);
    // Wait connecting
    rumboot_putstring( "Waiting for HSCB0 and HSCB1 link establishing\n" );
//    while (!(hscb0_link_established & hscb1_link_established)){
//        if (cnt == MAX_ATTEMPTS) {
//            rumboot_putstring( "Wait interrupt Time-out\n" );
//            return 1;
//        }
//        else
//            cnt += 1;
//    }
//    cnt = 0;
//    hscb0_status = 0;
//    hscb1_status = 0;
    if(!(   hscb_wait_status(base_addr,                 (HSCB_STATE_RUN << HSCB_STATUS_STATE_i)) &
            hscb_wait_status(supplementary_base_addr,   (HSCB_STATE_RUN << HSCB_STATUS_STATE_i)))){
        rumboot_putstring( "Wait HSCB RUN state Time-out\n" );
        rumboot_printf("HSCB(0x%x) status: 0x%x\n", base_addr, hscb_get_status(base_addr));
        rumboot_printf("HSCB(0x%x) status: 0x%x\n", supplementary_base_addr, hscb_get_status(base_addr));
        return 1;
    }

    rumboot_putstring( "HSCB link has been enabled\n" );
    // Enable DMA for HSCB0 and HSCB1
    rumboot_putstring( "Start work!\n" );
    hscb_run_wdma(base_addr);
    hscb_run_wdma(supplementary_base_addr);
    hscb_run_rdma(base_addr);
    hscb_run_rdma(supplementary_base_addr);
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
    rumboot_puthex((uint32_t)(&(*(descriptors + 3))[0]));
    hscb_descr = hscb_get_descr_from_mem((uint32_t)(&(*(descriptors + 3))[0]), HSCB_ROTATE_BYTES_ENABLE);
    rumboot_putstring( "Start addr:" );
    rumboot_puthex (hscb_descr.start_address);
    rumboot_putstring( "Length:" );
    rumboot_puthex (hscb_descr.length);
    for (i = 0; i < hscb_descr.length; ++i) {
        expected_data = ioread8(hscb_descr.start_address + i);
        obtained_data = ioread8(hscb_descr.start_address + i);
        if((obtained_data != expected_data)){
            rumboot_putstring("Data compare ERROR!!!\n");
            rumboot_puthex(hscb_descr.start_address + i);
            rumboot_putstring("Expected data: ");
            rumboot_puthex(expected_data);
            rumboot_putstring("Obtained data: ");
            rumboot_puthex (obtained_data);
            return 1;
        }
//        TEST_ASSERT ((obtained_data == expected_data), "Data compare ERROR!!!\n" );
    }
    rumboot_putstring( "HSCB1 to HSCB0 descriptor #1: successfully checked.\n" );

    rumboot_putstring( "HSCB0 to HSCB1 descriptor #1\n" );
    rumboot_puthex((uint32_t)(*(descriptors + 1)));
    hscb_descr = hscb_get_descr_from_mem((uint32_t)(*(descriptors + 1)), HSCB_ROTATE_BYTES_ENABLE);
    rumboot_putstring( "Length:" );
    rumboot_puthex (hscb_descr.length);
    for (i = 0; i < hscb_descr.length; ++i) {
        expected_data = ioread8(hscb_descr.start_address + i);
        obtained_data = ioread8(hscb_descr.start_address + i);
        if((obtained_data != expected_data)){
            rumboot_putstring("Data compare ERROR!!!\n");
            rumboot_puthex(hscb_descr.start_address  + i);
            rumboot_putstring("Expected data: ");
            rumboot_puthex(expected_data);
            rumboot_putstring("Obtained data: ");
            rumboot_puthex (obtained_data);
            return 1;
        }
//        TEST_ASSERT ((obtained_data == expected_data), "Data compare ERROR!!!\n" );
    }
    rumboot_putstring( "HSCB0 to HSCB1 descriptor #1: successfully checked.\n" );

    return 0;
}
#else

static uint32_t check_hscb_func(
        uint32_t base_addr,
        uint32_t supplementary_base_addr,
        uint8_t **  data_areas,
        uint32_t*   data_area_sizes,
        uint32_t    data_areas_count){
    hscb_packed_descr_struct_t* descriptors[DESCRIPTOR_TABLES_COUNT];
    uint32_t descriptor_counts[DESCRIPTOR_TABLES_COUNT] = {3, 3, 3, 3};
    int cnt = 0;
    int i;
    hscb_descr_struct_t hscb_descr;

    hscb0_status = 0;
    hscb1_status = 0;
    hscb0_dma_status = 0;
    hscb1_dma_status = 0;

    uint8_t expected_data = 0;
    uint8_t obtained_data = 0;
    rumboot_putstring( "------- HSCB ACCESS test -------\n" );

    prepare_descriptor_areas(&descriptors,descriptor_counts,DESCRIPTOR_TABLES_COUNT);
    init_hscb_cfg( data_areas, data_area_sizes, descriptors, descriptor_counts,DESCRIPTOR_TABLES_COUNT, HSCB_ROTATE_BYTES_ENABLE);

    set_descriptor_tables(base_addr, supplementary_base_addr, descriptors,descriptor_counts,DESCRIPTOR_TABLES_COUNT);
    hscb_set_max_speed(base_addr);
    hscb_set_max_speed(supplementary_base_addr);
        // Enable HSCB0 and HSCB1
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    supplementary_base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        base_addr + HSCB_SETTINGS);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        supplementary_base_addr + HSCB_SETTINGS);
    msync();
    rumboot_putstring( "Wait HSCB0 and HSCB1 finish work\n" );
    while (!(hscb0_link_established & hscb1_link_established)){
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

    rumboot_putstring( "HSCB link has enabled\n" );
    // Enable DMA for HSCB0 and HSCB1
    rumboot_putstring( "Start work!\n" );
    hscb_run_wdma(base_addr);
    hscb_run_wdma(supplementary_base_addr);
    hscb_run_rdma(base_addr);
    hscb_run_rdma(supplementary_base_addr);
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

   return 0;
}
#endif

int main() {
    uint32_t result = 0x0;
    struct rumboot_irq_entry *tbl;
    uint32_t count_of_memory_areas;
    uint8_t** data_areas = NULL;
    uint32_t* data_area_sizes = NULL;

    rumboot_printf( "Check HSCB (0x%x) \n", HSCB_UNDER_TEST_BASE );

#ifdef CHECK_REGS
    result += check_hscb_default_val( HSCB_UNDER_TEST_BASE );
    result += check_hscb_regs( HSCB_UNDER_TEST_BASE );
    hscb_sw_rst(HSCB_UNDER_TEST_BASE);
    hscb_adma_sw_rst(HSCB_UNDER_TEST_BASE);
    hscb_sw_rst(HSCB_SUPPLEMENTARY_BASE);
    hscb_adma_sw_rst(HSCB_SUPPLEMENTARY_BASE);
#endif
    emi_init(DCR_EM2_EMI_BASE);
    tbl = create_irq_handlers();

    count_of_memory_areas = prepare_memory_areas(&data_areas, &data_area_sizes);
    set_test_data_multiple(data_areas, data_area_sizes, count_of_memory_areas, HSCB_ROTATE_BYTES_DISABLE);
#ifdef HSCB_SHORT_TEST
    result += check_hscb_short_func(HSCB_UNDER_TEST_BASE, HSCB_SUPPLEMENTARY_BASE, data_areas, data_area_sizes, count_of_memory_areas);
#else
    result += check_hscb_func(HSCB_UNDER_TEST_BASE, HSCB_SUPPLEMENTARY_BASE);
#endif
    delete_irq_handlers(tbl);

    //result += check_gpio_func( HSCB_UNDER_TEST_BASE, 0x2A );

    return result;
}
