/*
 * test_oi10_hscb.c
 *
 *  Created on: Jul 23, 2018
 *      Author: r.galiulin
 */


#include <stdint.h>
#include <stdbool.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <platform/devices.h>
#include <regs/regs_hscb.h>


#define GPIO_REG_MASK   0xFFFFFFFF
#define CHECK_REGS
#define HSCB_X_BASE     SW0_BASE

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


int main() {
    uint32_t result = 0x0;

    rumboot_printf( "Check HSCB (0x%x) \n", HSCB_X_BASE );

#ifdef CHECK_REGS
    result += check_hscb_default_val( HSCB_X_BASE );
    result += check_hscb_regs( HSCB_X_BASE );
#endif
    //result += check_gpio_func( HSCB_X_BASE, 0x2A );

    return result;
}
