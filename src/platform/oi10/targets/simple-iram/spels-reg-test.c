/* -*- rumboot-test-labels: hwonly -*- */
#include <rumboot/platform.h>
#include <rumboot/regpoker.h>
#include <rumboot/printf.h>
#include <platform/devices.h>


// Processing src/platform/oi10/targets/test_oi10_greth.c / test_oi10_greth
#include <devices/ugly/greth.h>
#include <regs/regs_greth.h>

#define GRETH_CTRL_MASK      0xfe007fff
#define GRETH_CTRL_DFLT      0x08000090
#define GRETH_STAT_MASK      0x000000C0
#define GRETH_STAT_DFLT      0x00000000
#define GRETH_MDIO_CTRL_MASK 0xFFFFFFDF
#define GRETH_MDIO_CTRL_DFLT 0x01e10140

// Processing src/platform/oi10/targets/test_oi10_plb6_axi.c / test_oi10_plb6_axi
struct regpoker_checker greth0_read_checker[] = {
    {"CTRL              ", REGPOKER_READ32, CTRL,               0x98000000,      GRETH_CTRL_MASK        }, // pcb version
    //{"CTRL              ", REGPOKER_READ32, CTRL,               GRETH_CTRL_DFLT,      GRETH_CTRL_MASK        }, // rtl version
    //{"STATUS            ", REGPOKER_READ32, STATUS,             GRETH_STAT_DFLT,      GRETH_STAT_MASK        }, // new
    // not stable on pcb{"MDIO_CTRL         ", REGPOKER_READ32, MDIO_CTRL,          0xffff000e, GRETH_MDIO_CTRL_MASK   }, // pcb version
    //{"MDIO_CTRL         ", REGPOKER_READ32, MDIO_CTRL,          GRETH_MDIO_CTRL_DFLT, GRETH_MDIO_CTRL_MASK   }, // rtl version
    {"TRANSMIT_DESCR_PTR", REGPOKER_READ8,  TRANSMIT_DESCR_PTR, 0x0,                  0x3F8                  },
    {"RECEIVER_DESCR_PTR", REGPOKER_READ8,  RECEIVER_DESCR_PTR, 0x0,                  0x3F8                  },
    {"EDCL_IP           ", REGPOKER_READ32, EDCL_IP,            EDCLIP0,              ~0                     },
    {"EDCL_MAC_MSB      ", REGPOKER_READ32, EDCL_MAC_MSB,       EDCLMAC_MSB,          0xffff                 },
    {"EDCL_MAC_LSB      ", REGPOKER_READ32, EDCL_MAC_LSB,       EDCLMAC_LSB,          ~0                     },
    { } // Missing
    };

struct regpoker_checker greth1_read_checker[] = {
    {"CTRL              ", REGPOKER_READ32, CTRL,               0x98000000,      GRETH_CTRL_MASK        }, // pcb version
    //{"CTRL              ", REGPOKER_READ32, CTRL,               GRETH_CTRL_DFLT,      GRETH_CTRL_MASK        }, // rtl version
    //{"STATUS            ", REGPOKER_READ32, STATUS,             GRETH_STAT_DFLT,      GRETH_STAT_MASK     }, // new
    // not stable on pcb{"MDIO_CTRL         ", REGPOKER_READ32, MDIO_CTRL,          0xffff000e, GRETH_MDIO_CTRL_MASK   }, // pcb version
    //{"MDIO_CTRL         ", REGPOKER_READ32, MDIO_CTRL,          GRETH_MDIO_CTRL_DFLT, GRETH_MDIO_CTRL_MASK   }, // rtl version
    {"TRANSMIT_DESCR_PTR", REGPOKER_READ8,  TRANSMIT_DESCR_PTR, 0x0,                  0x3F8               },
    {"RECEIVER_DESCR_PTR", REGPOKER_READ8,  RECEIVER_DESCR_PTR, 0x0,                  0x3F8               },
    {"EDCL_IP           ", REGPOKER_READ32, EDCL_IP, EDCLIP1,                         ~0                  },
    {"EDCL_MAC_MSB      ", REGPOKER_READ32, EDCL_MAC_MSB,       EDCLMAC_MSB,          0xffff              },
    {"EDCL_MAC_LSB      ", REGPOKER_READ32, EDCL_MAC_LSB,       0x660e1001,           ~0                  },
    { } // Missing
    };

struct regpoker_checker greth_write_checker[] = {
    {"EDCL_IP           ", REGPOKER_WRITE32, EDCL_IP,      0, ~0       },
    {"EDCL_MAC_MSB      ", REGPOKER_WRITE32, EDCL_MAC_MSB, 0, 0xffff   },
    {"EDCL_MAC_LSB      ", REGPOKER_WRITE32, EDCL_MAC_LSB, 0, ~0       },
    {}
};

#include <platform/regs/regs_hscb.h>

#define HSCB_ID_DFLT              0x42435348
#define HSCB_VERSION_DFLT         0x00000102
#define HSCB_SW_RESET_DFLT        0x00000000
#define HSCB_SW_STATUS_DFLT       0x00000000
#define HSCB_IRQ_MASK_DFLT        0x00000000
#define HSCB_TRANS_CLK_DFLT       0x00c81414
#define HSCB_TIMINGS_DFLT         0xc8a00500
#define HSCB_INTERVAL_DFLT        0x000007d0
#define HSCB_TIME_CODE_INNER_DFLT 0x00000101
#define HSCB_TIME_CODE_OUTER_DFLT 0x00000000
#define HSCB_RMAP_PARAMS_DFLT     0x00017799
#define HSCB_RMAP_STATUS_DFLT     0x00000000
#define HSCB_RMAP_C_STATUS_DFLT   0x00000000
#define HSCB_ADMA_ID_DFLT         0x414d4441
#define HSCB_ADMA_REV_DFLT        0x00000011
#define HSCB_ADMA_SW_RESET_DFLT   0x00000000
#define HSCB_ADMA_CH_STATUS_DFLT  0x00000000
#define HSCB_ARLEN_DFLT           0xf
#define HSCB_AWLEN_DFLT           0xf
#define HSCB_BRESP_DFLT           0x0
#define HSCB_RAXI_ERR_ADDR_DFLT   0x0
#define HSCB_ARCACHE_DFLT         0x3
#define HSCB_AWCACHE_DFLT         0x3
#define HSCB_ARPROT_DFLT          0x2
#define HSCB_AWPROT_DFLT          0x2
#define HSCB_ARLOCK_DFLT          0x0
#define HSCB_AWLOCK_DFLT          0x0
#define HSCB_ARBURST_DFLT         0x1
#define HSCB_AWBURST_DFLT         0x1
#define HSCB_RDMA_SETTINGS_DFLT   0x0
#define HSCB_RDMA_STATUS_DFLT     0x80
#define HSCB_RDMA_SYS_ADDR_DFLT   0x0
#define HSCB_RDMA_TBL_SIZE_DFLT   0x800
#define HSCB_RDMA_ADDR_DFLT       0x0
#define HSCB_RDMA_LEN_DFLT        0x0
#define HSCB_RDMA_DESC_ADDR_DFLT  0x0
#define HSCB_WDMA_SETTINGS_DFLT   0x0
#define HSCB_WDMA_STATUS_DFLT     0x80
#define HSCB_WDMA_SYS_ADDR_DFLT   0x0
#define HSCB_WDMA_TBL_SIZE_DFLT   0x800
#define HSCB_WDMA_ADDR_DFLT       0x0
#define HSCB_WDMA_LEN_DFLT        0x0
#define HSCB_WDMA_DESC_ADDR_DFLT  0x0


struct regpoker_checker hscb_read_checker[] = {
    {"ID               ", REGPOKER_READ32, HSCB_ID,              HSCB_ID_DFLT,              0xffffffff}, // new
    {"VERSION          ", REGPOKER_READ32, HSCB_VERSION,         HSCB_VERSION_DFLT,         0xffffffff}, // new
    {"SW_RESET         ", REGPOKER_READ32, HSCB_SW_RESET,        HSCB_SW_RESET_DFLT,        0xffffffff}, // new
    {"SETTINGS         ", REGPOKER_READ32, HSCB_SETTINGS,        HSCB_SETTINGS_DFLT,        0xffffffff}, // new
    //{"STATUS           ", REGPOKER_READ32, HSCB_STATUS,          HSCB_STATUS_DFLT,          0xffffffff}, // new
    {"IRQ_MASK         ", REGPOKER_READ32, HSCB_IRQ_MASK,        HSCB_IRQ_MASK_DFLT,        0xffffffff}, // new
    {"TRANS_CLK        ", REGPOKER_READ32, HSCB_TRANS_CLK,       HSCB_TRANS_CLK_DFLT,       0xffffffff}, // new
    {"TIMINGS          ", REGPOKER_READ32, HSCB_TIMINGS,         HSCB_TIMINGS_DFLT,         0xffffffff}, // new
    {"INTERVAL         ", REGPOKER_READ32, HSCB_INTERVAL,        HSCB_INTERVAL_DFLT,        0xffffffff}, // new
    {"TIME_CODE_INNER  ", REGPOKER_READ32, HSCB_TIME_CODE_INNER, HSCB_TIME_CODE_INNER_DFLT, 0xffffffff}, // new
    {"TIME_CODE_OUTER  ", REGPOKER_READ32, HSCB_TIME_CODE_OUTER, HSCB_TIME_CODE_OUTER_DFLT, 0xffffffff}, // new
    {"RMAP_PARAMS      ", REGPOKER_READ32, HSCB_RMAP_PARAMS,     HSCB_RMAP_PARAMS_DFLT,     0xffffffff}, // new
    {"RMAP_C_STATUS    ", REGPOKER_READ32, HSCB_RMAP_C_STATUS,   HSCB_RMAP_C_STATUS_DFLT,   0xffffffff}, // new
    {"ADMA_ID          ", REGPOKER_READ32, HSCB_ADMA_ID,         HSCB_ADMA_ID_DFLT,         0xffffffff}, // new
    {"ADMA_REV         ", REGPOKER_READ32, HSCB_ADMA_REV,        HSCB_ADMA_REV_DFLT,        0xffffffff}, // new
    {"ADMA_SW_RESET    ", REGPOKER_READ32, HSCB_ADMA_SW_RESET,   HSCB_ADMA_SW_RESET_DFLT,   0xffffffff}, // new
    {"ADMA_CH_STATUS   ", REGPOKER_READ32, HSCB_ADMA_CH_STATUS,  HSCB_ADMA_CH_STATUS_DFLT,  0xffffffff}, // new
    {"ARLEN            ", REGPOKER_READ32, HSCB_ARLEN,           HSCB_ARLEN_DFLT,           0x0000000f}, // new
    {"ARWEN            ", REGPOKER_READ32, HSCB_AWLEN,           HSCB_AWLEN_DFLT,           0x0000000f}, // new
    {"BRESP            ", REGPOKER_READ32, HSCB_BRESP,           HSCB_BRESP_DFLT,           0x00000003}, // new
    {"RAXI_ERR_ADDR    ", REGPOKER_READ32, HSCB_RAXI_ERR_ADDR,   HSCB_RAXI_ERR_ADDR_DFLT,   0xffffffff}, // new
    {"ARCACHE          ", REGPOKER_READ32, HSCB_ARCACHE,         HSCB_ARCACHE_DFLT,         0x0000000f}, // new
    {"AWCACHE          ", REGPOKER_READ32, HSCB_AWCACHE,         HSCB_AWCACHE_DFLT,         0x0000000f}, // new
    {"ARPROT           ", REGPOKER_READ32, HSCB_ARPROT,          HSCB_ARPROT_DFLT,          0x00000007}, // new
    {"AWPROT           ", REGPOKER_READ32, HSCB_AWPROT,          HSCB_AWPROT_DFLT,          0x00000007}, // new
    {"ARBURST          ", REGPOKER_READ32, HSCB_ARBURST,         HSCB_ARBURST_DFLT,         0x0000000f}, // new
    {"AWBURST          ", REGPOKER_READ32, HSCB_AWBURST,         HSCB_AWBURST_DFLT,         0x00000003}, // new
    {"RDMA_SETTINGS    ", REGPOKER_READ32, HSCB_RDMA_SETTINGS,   HSCB_RDMA_SETTINGS_DFLT,   0xffffffff}, // new
    {"RDMA_STATUS      ", REGPOKER_READ32, HSCB_RDMA_STATUS,     HSCB_RDMA_STATUS_DFLT,     0xffffffff}, // new
    {"RDMA_SYS_ADDR    ", REGPOKER_READ32, HSCB_RDMA_SYS_ADDR,   HSCB_RDMA_SYS_ADDR_DFLT,   0xffffffff}, // new
    {"RDMA_TBL_SIZE    ", REGPOKER_READ32, HSCB_RDMA_TBL_SIZE,   HSCB_RDMA_TBL_SIZE_DFLT,   0xffffffff}, // new
    {"RDMA_ADDR        ", REGPOKER_READ32, HSCB_RDMA_ADDR,       HSCB_RDMA_ADDR_DFLT,       0xffffffff}, // new
    {"RDMA_LEN         ", REGPOKER_READ32, HSCB_RDMA_LEN,        HSCB_RDMA_LEN_DFLT,        0xffffffff}, // new
    {"RDMA_DESC_ADDR   ", REGPOKER_READ32, HSCB_RDMA_DESC_ADDR,  HSCB_RDMA_DESC_ADDR_DFLT,  0xffffffff}, // new
    {"WDMA_SETTINGS    ", REGPOKER_READ32, HSCB_WDMA_SETTINGS,   HSCB_WDMA_SETTINGS_DFLT,   0xffffffff}, // new
    {"WDMA_STATUS      ", REGPOKER_READ32, HSCB_WDMA_STATUS,     HSCB_WDMA_STATUS_DFLT,     0xffffffff}, // new
    {"WDMA_SYS_ADDR    ", REGPOKER_READ32, HSCB_WDMA_SYS_ADDR,   HSCB_WDMA_SYS_ADDR_DFLT,   0xffffffff}, // new
    {"WDMA_TBL_SIZE    ", REGPOKER_READ32, HSCB_WDMA_TBL_SIZE,   HSCB_WDMA_TBL_SIZE_DFLT,   0xffffffff}, // new
    {"WDMA_ADDR        ", REGPOKER_READ32, HSCB_WDMA_ADDR,       HSCB_WDMA_ADDR_DFLT,       0xffffffff}, // new
    {"WDMA_LEN         ", REGPOKER_READ32, HSCB_WDMA_LEN,        HSCB_WDMA_LEN_DFLT,        0xffffffff}, // new
    {"WDMA_DESC_ADDR   ", REGPOKER_READ32, HSCB_WDMA_DESC_ADDR,  HSCB_WDMA_DESC_ADDR_DFLT,  0xffffffff}, // new
    
    
    { } // Missing
    };
    

#include <regs/regs_sdio.h>

#define SDIO_DCCR_0_DFLT                   0x0
#define SDIO_DCSSAR_0_DFLT                 0x0
#define SDIO_DCDSAR_0_DFLT                 0x0
#define SDIO_DCDTR_0_DFLT                  0x0
#define SDIO_DCCR_1_DFLT                   0x0
#define SDIO_DCSSAR_1_DFLT                 0x0
#define SDIO_DCDSAR_1_DFLT                 0x0
#define SDIO_DCDTR_1_DFLT                  0x0
#define SDIO_SDR_BUF_TRAN_RESP_REG_DFLT    0x0
#define SDIO_BUF_TRAN_CTRL_REG_DFLT        0x0
#define SDIO_MULTI_BLK_CTRL_REG_DFLT       0x0
#define SDIO_DMA_CHANEL_FINISH_REG_DFLT    0x0
#define SPISDIO_ENABLE_DFLT                0x0
#define SPISDIO_SDIO_CLK_DIVIDE_DFLT       0x1
#define SDIO_INT_STATUS_DFLT               0x0
#define SPISDIO_SDIO_INT_MASKS_DFLT        0x0
#define SPISDIO_SDIO_CLK_POLARITY_DFLT     0x0
                                           
#define SPISDIO_SDIO_CLK_POLARITY          0x310


struct regpoker_checker sdio_read_checker[] = {
    {"SDIO_SDR_CARD_BLOCK_SET_REG    ", REGPOKER_READ32, SDIO_SDR_CARD_BLOCK_SET_REG, SDIO_SDR_CARD_BLOCK_SET_REG_DFLT, 0x03ff03ff}, // new
    {"SDIO_SDR_CTRL_REG              ", REGPOKER_READ32, SDIO_SDR_CTRL_REG,           SDIO_SDR_CTRL_REG_DFLT,           0x003f7fd3}, // new
    {"SDIO_SDR_CMD_ARGUMENT_REG      ", REGPOKER_READ32, SDIO_SDR_CMD_ARGUMENT_REG,   SDIO_SDR_CMD_ARGUMENT_REG_DFLT,   0xffffffff}, // new
    {"SDIO_SDR_ADDRESS_REG           ", REGPOKER_READ32, SDIO_SDR_ADDRESS_REG,        SDIO_SDR_ADDRESS_REG_DFLT,        0x00000004}, // new
    {"SDIO_SDR_STATUS_REG            ", REGPOKER_READ32, SDIO_SDR_STATUS_REG,         0xc00,                            0x006f9c00}, // pcb version
    //{"SDIO_SDR_STATUS_REG            ", REGPOKER_READ32, SDIO_SDR_STATUS_REG,         0x800,                            0x006f9c00}, // rtl version
    {"SDIO_SDR_ERROR_ENABLE_REG      ", REGPOKER_READ32, SDIO_SDR_ERROR_ENABLE_REG,   SDIO_SDR_ERROR_ENABLE_REG_DFLT,   0x000003ff}, // new
    {"SDIO_SDR_RESPONSE1_REG         ", REGPOKER_READ32, SDIO_SDR_RESPONSE1_REG,      SDIO_SDR_RESPONSE1_REG_DFLT,      0xffffffff}, // new
    {"SDIO_SDR_RESPONSE2_REG         ", REGPOKER_READ32, SDIO_SDR_RESPONSE2_REG,      SDIO_SDR_RESPONSE2_REG_DFLT,      0xffffffff}, // new
    {"SDIO_SDR_RESPONSE3_REG         ", REGPOKER_READ32, SDIO_SDR_RESPONSE3_REG,      SDIO_SDR_RESPONSE3_REG_DFLT,      0xffffffff}, // new
    {"SDIO_SDR_RESPONSE4_REG         ", REGPOKER_READ32, SDIO_SDR_RESPONSE4_REG,      SDIO_SDR_RESPONSE4_REG_DFLT,      0xffffffff}, // new
    {"SDIO_DCCR_0                    ", REGPOKER_READ32, SDIO_DCCR_0,                 SDIO_DCCR_0_DFLT,                 0x000870f7}, // new
    {"SDIO_DCSSAR_0                  ", REGPOKER_READ32, SDIO_DCSSAR_0,               SDIO_DCSSAR_0_DFLT,               0xffffffff}, // new
    {"SDIO_DCDSAR_0                  ", REGPOKER_READ32, SDIO_DCDSAR_0,               SDIO_DCDSAR_0_DFLT,               0xffffffff}, // new
    {"SDIO_DCDTR_0                   ", REGPOKER_READ32, SDIO_DCDTR_0,                SDIO_DCDTR_0_DFLT,                0xffffffff}, // new
    {"SDIO_DCCR_1                    ", REGPOKER_READ32, SDIO_DCCR_1,                 SDIO_DCCR_1_DFLT,                 0x000870f7}, // new
    {"SDIO_DCSSAR_1                  ", REGPOKER_READ32, SDIO_DCSSAR_1,               SDIO_DCSSAR_1_DFLT,               0xffffffff}, // new
    {"SDIO_DCDSAR_1                  ", REGPOKER_READ32, SDIO_DCDSAR_1,               SDIO_DCDSAR_1_DFLT,               0xffffffff}, // new
    {"SDIO_DCDTR_1                   ", REGPOKER_READ32, SDIO_DCDTR_1,                SDIO_DCDTR_1_DFLT,                0xffffffff}, // new
    {"SDIO_SDR_BUF_TRAN_RESP_REG     ", REGPOKER_READ32, SDIO_SDR_BUF_TRAN_RESP_REG,  SDIO_SDR_BUF_TRAN_RESP_REG_DFLT,  0x0000003f}, // new
    {"SDIO_BUF_TRAN_CTRL_REG         ", REGPOKER_READ32, SDIO_BUF_TRAN_CTRL_REG,      SDIO_BUF_TRAN_CTRL_REG_DFLT,      0x00000007}, // new
    {"SDIO_MULTI_BLK_CTRL_REG        ", REGPOKER_READ32, SDIO_MULTI_BLK_CTRL_REG,     SDIO_MULTI_BLK_CTRL_REG_DFLT,     0x00007000}, // new
    {"SDIO_DMA_CHANEL_FINISH_REG     ", REGPOKER_READ32, SDIO_DMA_CHANEL_FINISH_REG,  SDIO_DMA_CHANEL_FINISH_REG_DFLT,  0x00000003}, // new
    {"SPISDIO_ENABLE                 ", REGPOKER_READ32, SPISDIO_ENABLE,              SPISDIO_ENABLE_DFLT,              0x00000001}, // new
    {"SPISDIO_SDIO_CLK_DIVIDE        ", REGPOKER_READ32, SPISDIO_SDIO_CLK_DIVIDE,     SPISDIO_SDIO_CLK_DIVIDE_DFLT,     0x000000ff}, // new
    {"SDIO_INT_STATUS                ", REGPOKER_READ32, SDIO_INT_STATUS,             SDIO_INT_STATUS_DFLT,             0x000000ff}, // new
    {"SPISDIO_SDIO_INT_MASKS         ", REGPOKER_READ32, SPISDIO_SDIO_INT_MASKS,      SPISDIO_SDIO_INT_MASKS_DFLT,      0x000000ff}, // new
    {"SPISDIO_SDIO_CLK_POLARITY      ", REGPOKER_READ32, SPISDIO_SDIO_CLK_POLARITY,   SPISDIO_SDIO_CLK_POLARITY_DFLT,   0x00000001}, // new
    
    { } // Missing
    };

// Processing src/platform/oi10/targets/test_oi10_lscb.c / test_oi10_lscb
#include <regs/regs_mkio.h>
#include <devices/mkio.h>

struct regpoker_checker mkio_checker[] =
    {
        {"IRQ    ", REGPOKER_READ32, IRQ,     0x00000000, 0xFFFFFFFF},
        {"IRQE   ", REGPOKER_READ32, IRQE,    0x00000000, 0xFFFFFFFF},
        {"HWCFG  ", REGPOKER_READ32, HWCFG,   0x00000000, 0xFFFFFFFF},
        {"BCSL   ", REGPOKER_READ32, BCSL,    0xF0000000, 0xFFFFFFFF},
        {"BCA    ", REGPOKER_READ32, BCA,     0x00000000, 0xFFFFFFFF},
        {"BCTNP  ", REGPOKER_READ32, BCTNP,   0x00000000, 0xFFFFFFFF},
        {"BCANP  ", REGPOKER_READ32, BCANP,   0x00000000, 0xFFFFFFFF},
        {"BCT    ", REGPOKER_READ32, BCT,     0x00000000, 0xFFFFFFFF},
        {"BCTW   ", REGPOKER_READ32, BCTW,    0x00000000, 0xFFFFFFFF},
        {"BCRD   ", REGPOKER_READ32, BCRD,    0x00000000, 0xFFFFFFFF},
        {"BCBS   ", REGPOKER_READ32, BCBS,    0x00000000, 0xFFFFFFFF},
        {"BCTCP  ", REGPOKER_READ32, BCTCP,   0x00000000, 0xFFFFFFFF},
        {"BCACP  ", REGPOKER_READ32, BCACP,   0x00000000, 0xFFFFFFFF},
        {"RTS    ", REGPOKER_READ32, RTS,     0x80000000, 0xFFFFFFFF},
        {"RTC    ", REGPOKER_READ32, RTC,     0x0000E03E, 0xFFFFFFFF},
        {"RTBS   ", REGPOKER_READ32, RTBS,    0x00000000, 0xFFFFFFFF},
        {"RTSW   ", REGPOKER_READ32, RTSW,    0x00000000, 0xFFFFFFFF},
        {"RTSY   ", REGPOKER_READ32, RTSY,    0x00000000, 0xFFFFFFFF},
        {"RTSTBA ", REGPOKER_READ32, RTSTBA,  0x00000000, 0xFFFFFFFF},
        {"RTMCC  ", REGPOKER_READ32, RTMCC,   0x00000555, 0xFFFFFFFF},
        {"RTTTC  ", REGPOKER_READ32, RTTTC,   0x00000000, 0xFFFFFFFF},
        {"RTELM  ", REGPOKER_READ32, RTELM,   0xFFFFFFFC, 0xFFFFFFFF},
        {"RTELP  ", REGPOKER_READ32, RTELP,   0x00000000, 0xFFFFFFFF},
        {"RTELIP ", REGPOKER_READ32, RTELIP,  0x00000000, 0xFFFFFFFF},
        {"BMS    ", REGPOKER_READ32, BMS,     0x80000000, 0xFFFFFFFF},
        {"BMC    ", REGPOKER_READ32, BMC,     0x00000000, 0xFFFFFFFF},
        {"BMRTAF ", REGPOKER_READ32, BMRTAF,  0xFFFFFFFF, 0xFFFFFFFF},
        {"BMRTSF ", REGPOKER_READ32, BMRTSF,  0xFFFFFFFF, 0xFFFFFFFF},
        {"BMRTMC ", REGPOKER_READ32, BMRTMC,  0xFFFFFFFF, 0xFFFFFFFF},
        {"BMLBS  ", REGPOKER_READ32, BMLBS,   0x00000000, 0xFFFFFFFF},
        {"BMLBE  ", REGPOKER_READ32, BMLBE,   0x00000007, 0xFFFFFFFF},
        {"BMLBP  ", REGPOKER_READ32, BMLBP,   0x00000000, 0xFFFFFFFF},
        {"BMTTC  ", REGPOKER_READ32, BMTTC,   0x00000000, 0xFFFFFFFF},
        {"BCRD   ", REGPOKER_WRITE32, BCRD,   0x00000000, 0xFFFFFFFC},
        {"BCBS   ", REGPOKER_WRITE32, BCBS,   0x00000000, 0xFFFFFFFF},
        {"RTSTBA ", REGPOKER_WRITE32, RTSTBA, 0x00000000, 0xFFFFFE00},
        {"RTTTC  ", REGPOKER_WRITE32, RTTTC,  0x00000000, 0xFFFFFFFF},
        {"RTELP  ", REGPOKER_WRITE32, RTELP,  0x00000000, 0xFFFFFFFC},
        {"BMRTAF ", REGPOKER_WRITE32, BMRTAF, 0xFFFFFFFF, 0xFFFFFFFF},
        {"BMRTSF ", REGPOKER_WRITE32, BMRTSF, 0xFFFFFFFF, 0xFFFFFFFF},
        {"BMLBS  ", REGPOKER_WRITE32, BMLBS,  0x00000000, 0xFFFFFFF8},
        {"BMLBE  ", REGPOKER_WRITE32, BMLBE,  0x00000007, 0x003FFFF8},
        {"BMLBP  ", REGPOKER_WRITE32, BMLBP,  0x00000000, 0x003FFFF8},
        {"BMTTC  ", REGPOKER_WRITE32, BMTTC,  0x00000000, 0xFFFFFFFF},
        { } // Missing
};
// Processing src/platform/oi10/targets/test_oi10_uart_000.c / test_oi10_uart_000
#include <regs/regs_uart_pl011.h>
#include <devices/uart_pl011.h>

//default values
#define     UARTRSR_DEFAULT         0x0
#define     UARTECR_DEFAULT         0x0
#define     UARTFR_DEFAULT          0x090
#define     UARTILPR_DEFAULT        0x0
#define     UARTIBRD_DEFAULT        0x0
#define     UARTFBRD_DEFAULT        0x0
#define     UARTLCR_H_DEFAULT       0x0
#define     UARTCR_DEFAULT          0x0300
#define     UARTIFLS_DEFAULT        0x12
#define     UARTIMSC_DEFAULT        0x0
#define     UARTRIS_DEFAULT         0x0
#define     UARTMIS_DEFAULT         0x0
#define     UARTDMACR_DEFAULT       0x0
#define     UARTPeriphID0_DEFAULT   0x11
#define     UARTPeriphID1_DEFAULT   0x10
#define     UARTPeriphID2_DEFAULT   0x4
#define     UARTPeriphID3_DEFAULT   0x0
#define     UARTPCellID0_DEFAULT    0x0D
#define     UARTPCellID1_DEFAULT    0xF0
#define     UARTPCellID2_DEFAULT    0x05
#define     UARTPCellID3_DEFAULT    0xB1

//Mask
#define     UARTDR_MSK          0x000
#define     UARTRSR_MSK         0xF
#define     UARTECR_MSK         0xF
#define     UARTFR_MSK          0b011111000
#define     UARTILPR_MSK        0xFF
#define     UARTIBRD_MSK        0xFFFF
#define     UARTFBRD_MSK        0x3F
#define     UARTLCR_H_MSK       0xFF
#define     UARTCR_MSK          0xFF87
#define     UARTIFLS_MSK        0x3F
#define     UARTIMSC_MSK        0x7FF
#define     UARTRIS_MSK         0x7F0
#define     UARTMIS_MSK         0x7F0
#define     UARTICR_MSK         0x7FF
#define     UARTDMACR_MSK       0x7
#define     UARTPeriphID0_MSK   0xFF
#define     UARTPeriphID1_MSK   0xFF
#define     UARTPeriphID2_MSK   0x0F
#define     UARTPeriphID3_MSK   0xFF
#define     UARTPCellID0_MSK    0xFF
#define     UARTPCellID1_MSK    0xFF
#define     UARTPCellID2_MSK    0xFF
#define     UARTPCellID3_MSK    0xFF

struct regpoker_checker uart_checker[] = {
    {"UARTRSR",       REGPOKER_READ32,  UARTRSR,         UARTRSR_DEFAULT,       UARTRSR_MSK         },
    {"UARTFR",        REGPOKER_READ32,  UARTFR,          UARTFR_DEFAULT,        UARTFR_MSK          },
    {"UARTILPR",      REGPOKER_READ32,  UARTILPR,        UARTILPR_DEFAULT,      UARTILPR_MSK        },
    {"UARTIBRD",      REGPOKER_READ32,  UARTIBRD,        UARTIBRD_DEFAULT,      UARTIBRD_MSK        },
    {"UARTFBRD",      REGPOKER_READ32,  UARTFBRD,        UARTFBRD_DEFAULT,      UARTFBRD_MSK        },
    {"UARTLCR_H",     REGPOKER_READ32,  UARTLCR_H,       UARTLCR_H_DEFAULT,     UARTLCR_H_MSK       },
    {"UARTCR",        REGPOKER_READ32,  UARTCR,          UARTCR_DEFAULT,        UARTCR_MSK          },
    {"UARTIFLS",      REGPOKER_READ32,  UARTIFLS,        UARTIFLS_DEFAULT,      UARTIFLS_MSK        },
    {"UARTIMSC",      REGPOKER_READ32,  UARTIMSC,        UARTIMSC_DEFAULT,      UARTIMSC_MSK        },
    {"UARTRIS",       REGPOKER_READ32,  UARTRIS,         UARTRIS_DEFAULT,       UARTRIS_MSK         },
    {"UARTMIS",       REGPOKER_READ32,  UARTMIS,         UARTMIS_DEFAULT,       UARTMIS_MSK         },
    {"UARTDMACR",     REGPOKER_READ32,  UARTDMACR,       UARTDMACR_DEFAULT,     UARTDMACR_MSK       },
    {"UARTPeriphID0", REGPOKER_READ32,  UARTPeriphID0,   UARTPeriphID0_DEFAULT, UARTPeriphID0_MSK   },
    {"UARTPeriphID1", REGPOKER_READ32,  UARTPeriphID1,   UARTPeriphID1_DEFAULT, UARTPeriphID1_MSK   },
    {"UARTPeriphID2", REGPOKER_READ32,  UARTPeriphID2,   UARTPeriphID2_DEFAULT, UARTPeriphID2_MSK   },
    {"UARTPeriphID3", REGPOKER_READ32,  UARTPeriphID3,   UARTPeriphID3_DEFAULT, UARTPeriphID3_MSK   },
    {"UARTPCellID0",  REGPOKER_READ32,  UARTPCellID0,    UARTPCellID0_DEFAULT,  UARTPCellID0_MSK    },
    {"UARTPCellID1",  REGPOKER_READ32,  UARTPCellID1,    UARTPCellID1_DEFAULT,  UARTPCellID1_MSK    },
    {"UARTPCellID2",  REGPOKER_READ32,  UARTPCellID2,    UARTPCellID2_DEFAULT,  UARTPCellID2_MSK    },
    {"UARTPCellID3",  REGPOKER_READ32,  UARTPCellID3,    UARTPCellID3_DEFAULT,  UARTPCellID3_MSK    },
    {"UARTILPR",      REGPOKER_WRITE32, UARTILPR,        0x00,                  UARTILPR_MSK        },
    {"UARTIBRD",      REGPOKER_WRITE32, UARTIBRD,        0x00,                  UARTFBRD_MSK        },
    {"UARTFBRD",      REGPOKER_WRITE32, UARTFBRD,        0x00,                  UARTFBRD_MSK        },
    {"UARTLCR_H",     REGPOKER_WRITE32, UARTLCR_H,       0x00,                  UARTLCR_H_MSK       },
    {"UARTCR",        REGPOKER_WRITE32, UARTCR,          0x00,                  0xFF86              },
    {"UARTIFLS",      REGPOKER_WRITE32, UARTIFLS,        0x00,                  UARTIFLS_MSK        },
    {"UARTDMACR",     REGPOKER_WRITE32, UARTDMACR,       0x00,                  UARTDMACR_MSK       },
    {/* Sentinel */}};


#include <regs/regs_spi.h>
#include <regs/regs_gpio_pl061.h>

#undef GSPI_STATUS_DEFAULT
#define GSPI_STATUS_DEFAULT 0x82

// Processing src/platform/oi10/targets/test_oi10_spi.c / test_oi10_spi
static struct regpoker_checker const spi_checker[] = {
    //{"STATUS",       REGPOKER_READ32, GSPI_STATUS,          GSPI_STATUS_DEFAULT,       GSPI_STATUS_MASK}, /*default SPIx_IRQ=z; need $plusargs("spi_irq_test") */
    {"SSPCR0",       REGPOKER_READ32,  GSPI_SSPCR0,         GSPI_SSPCR0_DEFAULT,       GSPI_SSPCR0_MASK            },
    {"SSPCR1",       REGPOKER_READ32,  GSPI_SSPCR1,         GSPI_SSPCR1_DEFAULT,       GSPI_SSPCR1_MASK            },
    {"SSPSR",        REGPOKER_READ32,  GSPI_SSPSR,          GSPI_SSPSR_DEFAULT,        GSPI_SSPSR_MASK             },
    {"SSPCPSR",      REGPOKER_READ32,  GSPI_SSPCPSR,        GSPI_SSPCPSR_DEFAULT,      GSPI_SSPCPSR_MASK           },
    {"SSPIMSC",      REGPOKER_READ32,  GSPI_SSPIMSC,        GSPI_SSPIMSC_DEFAULT,      GSPI_SSPIMSC_MASK           },
    {"SSPRIS",       REGPOKER_READ32,  GSPI_SSPRIS,         GSPI_SSPRIS_DEFAULT,       GSPI_SSPRIS_MASK            },
    {"SSPDMACR",     REGPOKER_READ32,  GSPI_SSPDMACR,       GSPI_SSPDMACR_DEFAULT,     GSPI_SSPDMACR_MASK          },
    {"SSPPeriphID0", REGPOKER_READ32,  GSPI_SSPPeriphID0,   GSPI_SSPPeriphID0_DEFAULT, GSPI_SSPPeriphID0_MASK      },
    {"SSPPeriphID1", REGPOKER_READ32,  GSPI_SSPPeriphID1,   GSPI_SSPPeriphID1_DEFAULT, GSPI_SSPPeriphID1_MASK      },
    {"SSPPeriphID2", REGPOKER_READ32,  GSPI_SSPPeriphID2,   GSPI_SSPPeriphID2_DEFAULT, GSPI_SSPPeriphID2_MASK      },
    {"SSPPeriphID3", REGPOKER_READ32,  GSPI_SSPPeriphID3,   GSPI_SSPPeriphID3_DEFAULT, GSPI_SSPPeriphID3_MASK      },
    {"SSPPCellID0",  REGPOKER_READ32,  GSPI_SSPPCellID0,    GSPI_SSPPCellID0_DEFAULT,  GSPI_SSPPCellID0_MASK       },
    {"SSPPCellID1",  REGPOKER_READ32,  GSPI_SSPPCellID1,    GSPI_SSPPCellID1_DEFAULT,  GSPI_SSPPCellID1_MASK       },
    {"SSPPCellID2",  REGPOKER_READ32,  GSPI_SSPPCellID2,    GSPI_SSPPCellID2_DEFAULT,  GSPI_SSPPCellID2_MASK       },
    {"SSPPCellID3",  REGPOKER_READ32,  GSPI_SSPPCellID3,    GSPI_SSPPCellID3_DEFAULT,  GSPI_SSPPCellID3_MASK       },
    {"SSPCR0",       REGPOKER_WRITE32, GSPI_SSPCR0,         0x00,                      GSPI_SSPCR0_MASK            },
    {"SSPCR1",       REGPOKER_WRITE32, GSPI_SSPCR1,         0x00,                      GSPI_SSPCR1_MASK            },
    {"SSPCPSR",      REGPOKER_WRITE32, GSPI_SSPCPSR,        0x00,                      GSPI_SSPCPSR_MASK & ~0x01   },
    {"SSPIMSC",      REGPOKER_WRITE32, GSPI_SSPIMSC,        0x00,                      GSPI_SSPIMSC_MASK           },
    {"SSPDMACR",     REGPOKER_WRITE32, GSPI_SSPDMACR,       0x00,                      GSPI_SSPDMACR_MASK          },
    {"DMAWSTART",    REGPOKER_READ32,  GSPI_DMAWSTART,      GSPI_DMAWSTART_DEFAULT,    GSPI_DMAWSTART_MASK         },
    {"DMAWEND",      REGPOKER_READ32,  GSPI_DMAWEND,        GSPI_DMAWEND_DEFAULT,      GSPI_DMAWEND_MASK           },
    {"DMAWCNTRL",    REGPOKER_READ32,  GSPI_DMAWCNTRL,      GSPI_DMAWCNTRL_DEFAULT,    GSPI_DMAWCNTRL_MASK         },
    {"DMAWCUR",      REGPOKER_READ32,  GSPI_DMAWCUR,        GSPI_DMAWCUR_DEFAULT,      GSPI_DMAWCUR_MASK           },
    {"AXIW_BRESP",   REGPOKER_READ32,  GSPI_AXIW_BRESP,     GSPI_AXIW_BRESP_DEFAULT,   GSPI_AXIW_BRESP_MASK        },
    {"DMARSTART",    REGPOKER_READ32,  GSPI_DMARSTART,      GSPI_DMARSTART_DEFAULT,    GSPI_DMARSTART_MASK         },
    {"DMAREND",      REGPOKER_READ32,  GSPI_DMAREND,        GSPI_DMAREND_DEFAULT,      GSPI_DMAREND_MASK           },
    {"DMARCNTRL",    REGPOKER_READ32,  GSPI_DMARCNTRL,      GSPI_DMARCNTRL_DEFAULT,    GSPI_DMARCNTRL_MASK         },
    {"DMARCUR",      REGPOKER_READ32,  GSPI_DMARCUR,        GSPI_DMARCUR_DEFAULT,      GSPI_DMARCUR_MASK           },
    {"AXIR_BUFTYPE", REGPOKER_READ32,  GSPI_AXIR_BUFTYPE,   GSPI_AXIR_BUFTYPE_DEFAULT, GSPI_AXIR_BUFTYPE_MASK      },
    {"AXIR_BUFENA",  REGPOKER_READ32,  GSPI_AXIR_BUFENA,    GSPI_AXIR_BUFENA_DEFAULT,  GSPI_AXIR_BUFENA_MASK       },
    {"AXIR_ERRADDR", REGPOKER_READ32,  GSPI_AXIR_ERRADDR,   GSPI_AXIR_ERRADDR_DEFAULT, GSPI_AXIR_ERRADDR_MASK      },
    {"AXI_PARAMS",   REGPOKER_READ32,  GSPI_AXI_PARAMS,     GSPI_AXI_PARAMS_DEFAULT,   GSPI_AXI_PARAMS_MASK        },
    {"WORDOP",       REGPOKER_READ32,  GSPI_WORDOP,         GSPI_WORDOP_DEFAULT,       GSPI_WORDOP_MASK            },
    {"SOFTSS",       REGPOKER_READ32,  GSPI_SOFTSS,         GSPI_SOFTSS_DEFAULT,       GSPI_SOFTSS_MASK            },
    {"SOFTRST",      REGPOKER_READ32,  GSPI_SOFTRST,        GSPI_SOFTRST_DEFAULT,      GSPI_SOFTRST_MASK           },
    {"IRQMASKS",     REGPOKER_READ32,  GSPI_IRQMASKS,       GSPI_IRQMASKS_DEFAULT,     GSPI_IRQMASKS_MASK          },
    {"DMAWSTART",    REGPOKER_WRITE32, GSPI_DMAWSTART,      0x00,                      GSPI_DMAWSTART_MASK         },
    {"DMAWEND",      REGPOKER_WRITE32, GSPI_DMAWEND,        0x00,                      GSPI_DMAWEND_MASK           },
    {"DMARSTART",    REGPOKER_WRITE32, GSPI_DMARSTART,      0x00,                      GSPI_DMARSTART_MASK         },
    {"DMAREND",      REGPOKER_WRITE32, GSPI_DMAREND,        0x00,                      GSPI_DMAREND_MASK           },
    {"AXIR_BUFTYPE", REGPOKER_WRITE32, GSPI_AXIR_BUFTYPE,   0x00,                      GSPI_AXIR_BUFTYPE_MASK      },
    {"AXI_PARAMS",   REGPOKER_WRITE32, GSPI_AXI_PARAMS,     0x00,                      GSPI_AXI_PARAMS_MASK        },
    {"WORDOP",       REGPOKER_WRITE32, GSPI_WORDOP,         0x00,                      GSPI_WORDOP_MASK            },
    {"SOFTSS",       REGPOKER_WRITE32, GSPI_SOFTSS,         0x00,                      GSPI_SOFTSS_MASK            },
    {"IRQMASKS",     REGPOKER_WRITE32, GSPI_IRQMASKS,       0x00,                      GSPI_IRQMASKS_MASK          },
    {/* Sentinel */}};

// Processing src/platform/oi10/targets/test_oi10_gpio.c / test_oi10_gpio
struct regpoker_checker gpio_checker[] = {
    {"GPIO_DATA",      REGPOKER_READ32, GPIO_DATA,      GPIO_DATA_DEFAULT,      GPIO_REG_MASK   }, /* check only first of 256 */
    {"GPIO_DIR",       REGPOKER_READ32, GPIO_DIR,       GPIO_DIR_DEFAULT,       GPIO_REG_MASK   },
    {"GPIO_IS",        REGPOKER_READ32, GPIO_IS,        GPIO_IS_DEFAULT,        GPIO_REG_MASK   },
    {"GPIO_IBE",       REGPOKER_READ32, GPIO_IBE,       GPIO_IBE_DEFAULT,       GPIO_REG_MASK   },
    {"GPIO_IEV",       REGPOKER_READ32, GPIO_IEV,       GPIO_IEV_DEFAULT,       GPIO_REG_MASK   },
    {"GPIO_IE",        REGPOKER_READ32, GPIO_IE,        GPIO_IE_DEFAULT,        GPIO_REG_MASK   },
    {"GPIO_RIS",       REGPOKER_READ32, GPIO_RIS,       GPIO_RIS_DEFAULT,       GPIO_REG_MASK   },
    {"GPIO_MIS",       REGPOKER_READ32, GPIO_MIS,       GPIO_MIS_DEFAULT,       GPIO_REG_MASK   },
    {"GPIO_AFSEL",     REGPOKER_READ32, GPIO_AFSEL,     GPIO_AFSEL_DEFAULT,     GPIO_REG_MASK   },
    {"GPIO_PeriphID0", REGPOKER_READ32, GPIO_PeriphID0, GPIO_PeriphID0_DEFAULT, GPIO_REG_MASK   },
    {"GPIO_PeriphID1", REGPOKER_READ32, GPIO_PeriphID1, GPIO_PeriphID1_DEFAULT, GPIO_REG_MASK   },
    {"GPIO_PeriphID2", REGPOKER_READ32, GPIO_PeriphID2, GPIO_PeriphID2_DEFAULT, GPIO_REG_MASK   },
    {"GPIO_PeriphID3", REGPOKER_READ32, GPIO_PeriphID3, GPIO_PeriphID3_DEFAULT, GPIO_REG_MASK   },
    {"GPIO_CellID0",   REGPOKER_READ32, GPIO_CellID0,   GPIO_CellID0_DEFAULT,   GPIO_REG_MASK   },
    {"GPIO_CellID1",   REGPOKER_READ32, GPIO_CellID1,   GPIO_CellID1_DEFAULT,   GPIO_REG_MASK   },
    {"GPIO_CellID2",   REGPOKER_READ32, GPIO_CellID2,   GPIO_CellID2_DEFAULT,   GPIO_REG_MASK   },
    {"GPIO_CellID3",   REGPOKER_READ32, GPIO_CellID3,   GPIO_CellID3_DEFAULT,   GPIO_REG_MASK   },
    {/* Sentinel */}};
struct regpoker_checker gpio_rw_checker[] = {
    {"GPIO_DIR",        REGPOKER_WRITE32, GPIO_DIR,                  ~GPIO_DIR_DEFAULT,    GPIO_REG_MASK   },
    {"GPIO_DATAFF",     REGPOKER_WRITE32, GPIO_DATA + GPIO_ADDR_MASK, GPIO_DATA_DEFAULT,   GPIO_REG_MASK   },
    //{"GPIO_DATA00",     REGPOKER_WRITE32, GPIO_DATA,                  GPIO_DATA_DEFAULT,   0x00            },
    {"GPIO_DIR",        REGPOKER_WRITE32, GPIO_DIR,                   GPIO_DIR_DEFAULT,    0x00            },
    {"GPIO_IS",         REGPOKER_WRITE32, GPIO_IS,                    GPIO_IS_DEFAULT,     GPIO_REG_MASK   },
    {"GPIO_IBE",        REGPOKER_WRITE32, GPIO_IBE,                   GPIO_IBE_DEFAULT,    GPIO_REG_MASK   },
    {"GPIO_IEV",        REGPOKER_WRITE32, GPIO_IEV,                   GPIO_IEV_DEFAULT,    GPIO_REG_MASK   },
    {"GPIO_IE",         REGPOKER_WRITE32, GPIO_IE,                    GPIO_IE_DEFAULT,     GPIO_REG_MASK   },
    {"GPIO_AFSEL",      REGPOKER_WRITE32, GPIO_AFSEL,                 GPIO_AFSEL_DEFAULT,  GPIO_REG_MASK   },
    {/* Sentinel */}};

// Processing src/platform/oi10/targets/test_oi10_ctrl_000.c / test_oi10_ctrl_000
#include <platform/regs/sctl.h>
struct regpoker_checker sctl_checker[] =
    {
        {"KMBIST_CHAIN_SF_0",    REGPOKER_READ_DCR, KMBIST_CHAIN_SF_0,    0,                         SCTL_KMBIST_CHAIN_SF_mask   },
        {"KMBIST_CHAIN_SF_1",    REGPOKER_READ_DCR, KMBIST_CHAIN_SF_1,    0,                         SCTL_KMBIST_CHAIN_SF_mask   },
        {"KMBIST_CHAIN_SF_2",    REGPOKER_READ_DCR, KMBIST_CHAIN_SF_2,    0,                         SCTL_KMBIST_CHAIN_SF_mask   },
        {"KMBIST_CHAIN_SF_3",    REGPOKER_READ_DCR, KMBIST_CHAIN_SF_3,    0,                         SCTL_KMBIST_CHAIN_SF_mask   },
        {"KMBIST_CHAIN_SF_4",    REGPOKER_READ_DCR, KMBIST_CHAIN_SF_4,    0,                         SCTL_KMBIST_CHAIN_SF_mask   },
        {"KMBIST_CHAIN_SF_5",    REGPOKER_READ_DCR, KMBIST_CHAIN_SF_5,    0,                         SCTL_KMBIST_CHAIN_SF_mask   },
        {"KMBIST_CHAIN_SF_6",    REGPOKER_READ_DCR, KMBIST_CHAIN_SF_6,    0,                         SCTL_KMBIST_CHAIN_SF_mask   },
        {"KMBIST_CHAIN_SF_7",    REGPOKER_READ_DCR, KMBIST_CHAIN_SF_7,    0,                         SCTL_KMBIST_CHAIN_SF_mask   },
        {"KMBIST_CHAIN_SF_8",    REGPOKER_READ_DCR, KMBIST_CHAIN_SF_8,    0,                         SCTL_KMBIST_CHAIN_SF_mask   },
        {"PPC_SYS_CONF",         REGPOKER_READ_DCR, SCTL_PPC_SYS_CONF,    SCTL_PPC_SYS_CONF_dflt,    SCTL_PPC_SYS_CONF_mask      },
        {"SCTL_PPC_SLP_CTRL",    REGPOKER_READ_DCR, SCTL_PPC_SLP_CTRL,    0,                         SCTL_PPC_SLP_CTRL_mask      },
        {"SCTL_PPC_PMU_CTRL",    REGPOKER_READ_DCR, SCTL_PPC_PMU_CTRL,    0,                         SCTL_PPC_PMU_CTRL_mask      },
        {"SCTL_IFSYS_ETH_CTRL",  REGPOKER_READ_DCR, SCTL_IFSYS_ETH_CTRL,  SCTL_IFSYS_ETH_CTRL_dflt,  SCTL_IFSYS_ETH_CTRL_mask    },
        {"SCTL_IFSYS_ETH_HPROT", REGPOKER_READ_DCR, SCTL_IFSYS_ETH_HPROT, SCTL_IFSYS_ETH_HPROT_dflt, SCTL_IFSYS_ETH_HPROT_mask   },
        {}};


struct regpoker_checker sctl_rw_checker[] = {
    {"KMBIST_CHAIN_SF_0", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_0,    0,                         SCTL_KMBIST_CHAIN_SF_TM_mask   },
    {"KMBIST_CHAIN_SF_1", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_1,    0,                         SCTL_KMBIST_CHAIN_SF_TM_mask   },
    {"KMBIST_CHAIN_SF_2", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_2,    0,                         SCTL_KMBIST_CHAIN_SF_TM_mask   },
    {"KMBIST_CHAIN_SF_3", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_3,    0,                         SCTL_KMBIST_CHAIN_SF_TM_mask   },
    {"KMBIST_CHAIN_SF_4", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_4,    0,                         SCTL_KMBIST_CHAIN_SF_TM_mask   },
    {"KMBIST_CHAIN_SF_5", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_5,    0,                         SCTL_KMBIST_CHAIN_SF_TM_mask   },
    {"KMBIST_CHAIN_SF_6", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_6,    0,                         SCTL_KMBIST_CHAIN_SF_TM_mask   },
    {"KMBIST_CHAIN_SF_7", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_7,    0,                         SCTL_KMBIST_CHAIN_SF_TM_mask   },
    {"KMBIST_CHAIN_SF_8", REGPOKER_WRITE_DCR, KMBIST_CHAIN_SF_8,    0,                         SCTL_KMBIST_CHAIN_SF_TM_mask   },
    {"PPC_SLP_CTRL",      REGPOKER_WRITE_DCR, SCTL_PPC_SLP_CTRL,    0,                         SCTL_SLP_CTRL_TYPE_mask        },
    {"PPC_PMU_CTRL",      REGPOKER_WRITE_DCR, SCTL_PPC_PMU_CTRL,    0,                         SCTL_PPC_PMU_CTRL_mask         },
    {"IFSYS_ETH_CTRL",    REGPOKER_WRITE_DCR, SCTL_IFSYS_ETH_CTRL,  SCTL_IFSYS_ETH_CTRL_dflt,  SCTL_IFSYS_ETH_CTRL_mask       },
    {"IFSYS_ETH_HPROT",   REGPOKER_WRITE_DCR, SCTL_IFSYS_ETH_HPROT, SCTL_IFSYS_ETH_HPROT_dflt, SCTL_IFSYS_ETH_HPROT_mask      },
    {"IFSYS_ETH_HPROT",   REGPOKER_WRITE_DCR, SCTL_IFSYS_ETH_HPROT, 0xFFFFFFFF,                0                              },
    {"IFSYS_ETH_HPROT",   REGPOKER_READ_DCR,  SCTL_IFSYS_ETH_HPROT, 0xFFFFFFFF,                SCTL_IFSYS_ETH_HPROT_mask      },
    {"IFSYS_ETH_HPROT",   REGPOKER_WRITE_DCR, SCTL_IFSYS_ETH_HPROT, 0x33333333,                0                              },
    {"IFSYS_ETH_HPROT",   REGPOKER_READ_DCR,  SCTL_IFSYS_ETH_HPROT, 0x33333333,                SCTL_IFSYS_ETH_HPROT_mask      },
    {}};

// Processing src/platform/oi10/targets/test_oi10_ctrl_005.c / test_oi10_ctrl_005
#include <regs/regs_sp805.h>

#define WD_REG_LOAD_mask        0xffffffff
#define WD_REG_VALUE_mask       0x00000003
#define WD_REG_CONTROL_mask     0x3
#define WD_REG_RIS_mask         0x1
#define WD_REG_MIS_mask         0x1
#define WD_REG_LOCK_mask        0xffffffff
#define WD_REG_ITCR_mask        0x1
#define WD_REG_PERIPHID0_mask   0xff
#define WD_REG_PERIPHID1_mask   0xff
#define WD_REG_PERIPHID2_mask   0xff
#define WD_REG_PERIPHID3_mask   0xff
#define WD_REG_PCELLID0_mask    0xff
#define WD_REG_PCELLID1_mask    0xff
#define WD_REG_PCELLID2_mask    0xff
#define WD_REG_PCELLID3_mask    0xff

#define WD_REG_LOAD_DFLT        0xffffffff
#define WD_REG_VALUE_DFLT       0x00000003
#define WD_REG_CONTROL_DFLT     0
#define WD_REG_RIS_DFLT         0
#define WD_REG_MIS_DFLT         0
#define WD_REG_LOCK_DFLT        0x1ACCE551
#define WD_REG_ITCR_DFLT        0
#define WD_REG_PERIPHID0_DFLT   0x5
#define WD_REG_PERIPHID1_DFLT   0x18
#define WD_REG_PERIPHID2_DFLT   0x14
#define WD_REG_PERIPHID3_DFLT   0
#define WD_REG_PCELLID0_DFLT    0xD
#define WD_REG_PCELLID1_DFLT    0xF0
#define WD_REG_PCELLID2_DFLT    0x5
#define WD_REG_PCELLID3_DFLT    0xB1

struct regpoker_checker wdt_checker[] = {
    {"WD_REG_PERIPHID0", REGPOKER_READ_DCR, WD_REG_PERIPHID0, WD_REG_PERIPHID0_DFLT, WD_REG_PERIPHID0_mask   },
    {"WD_REG_PERIPHID1", REGPOKER_READ_DCR, WD_REG_PERIPHID1, WD_REG_PERIPHID1_DFLT, WD_REG_PERIPHID1_mask   },
    {"WD_REG_PERIPHID2", REGPOKER_READ_DCR, WD_REG_PERIPHID2, WD_REG_PERIPHID2_DFLT, WD_REG_PERIPHID2_mask   },
    {"WD_REG_PERIPHID3", REGPOKER_READ_DCR, WD_REG_PERIPHID3, WD_REG_PERIPHID3_DFLT, WD_REG_PERIPHID3_mask   },
    {"WD_REG_PCELLID0",  REGPOKER_READ_DCR, WD_REG_PCELLID0,  WD_REG_PCELLID0_DFLT,  WD_REG_PCELLID0_mask    },
    {"WD_REG_PCELLID1",  REGPOKER_READ_DCR, WD_REG_PCELLID1,  WD_REG_PCELLID1_DFLT,  WD_REG_PCELLID1_mask    },
    {"WD_REG_PCELLID2",  REGPOKER_READ_DCR, WD_REG_PCELLID2,  WD_REG_PCELLID2_DFLT,  WD_REG_PCELLID2_mask    },
    {"WD_REG_PCELLID3",  REGPOKER_READ_DCR, WD_REG_PCELLID3,  WD_REG_PCELLID3_DFLT,  WD_REG_PCELLID3_mask    },
    {"WdogValue",        REGPOKER_READ_DCR, WD_REG_VALUE,     WD_REG_VALUE_DFLT,     WD_REG_VALUE_mask       },
    {}};

struct regpoker_checker wdt_rw_checker[] = {
    {"WdogLoad",    REGPOKER_READ_DCR,  WD_REG_LOAD,    WD_REG_LOAD_DFLT,    WD_REG_LOAD_mask      },
    {"WdogLoad",    REGPOKER_WRITE_DCR, WD_REG_LOAD,    WD_REG_LOAD_DFLT,    WD_REG_LOAD_mask      },
    {"WdogControl", REGPOKER_READ_DCR,  WD_REG_CONTROL, WD_REG_CONTROL_DFLT, WD_REG_CONTROL_mask   },
    {"WdogControl", REGPOKER_WRITE_DCR, WD_REG_CONTROL, WD_REG_CONTROL_DFLT, WD_REG_CONTROL_mask   },
    {"WdogRIS",     REGPOKER_READ_DCR,  WD_REG_RIS,     WD_REG_RIS_DFLT,     WD_REG_RIS_mask       },
    {"WdogMIS",     REGPOKER_READ_DCR,  WD_REG_MIS,     WD_REG_MIS_DFLT,     WD_REG_MIS_mask       },
    {"WdogITCR",    REGPOKER_READ_DCR,  WD_REG_ITCR,    WD_REG_ITCR_DFLT,    WD_REG_ITCR_mask      },
    {"WdogITCR",    REGPOKER_WRITE_DCR, WD_REG_ITCR,    WD_REG_ITCR_DFLT,    WD_REG_ITCR_mask      },
    {"WdogLock",    REGPOKER_READ_DCR,  WD_REG_LOCK,    0,                   0                     },
    {"WdogLock",    REGPOKER_WRITE_DCR, WD_REG_LOCK,    0,                   1                     },
    {"WdogLock",    REGPOKER_READ_DCR,  WD_REG_LOCK,    1,                   1                     },

    {}};

// Processing src/platform/oi10/targets/simple-iram/test_oi10_em2_104.c / test_oi10_em2_104
struct regpoker_checker em2_checker_a[] = {
    {"Data0_8",  REGPOKER_READ8, 0x0,  0x10, 0xff   },
    {"Data1_8",  REGPOKER_READ8, 0x1,  0x10, 0xff   },
    {"Data2_8",  REGPOKER_READ8, 0x2,  0x10, 0xff   },
    {"Data3_8",  REGPOKER_READ8, 0x3,  0x10, 0xff   },
    {"Data4_8",  REGPOKER_READ8, 0x4,  0x01, 0xff   },
    {"Data5_8",  REGPOKER_READ8, 0x5,  0x23, 0xff   },
    {"Data6_8",  REGPOKER_READ8, 0x6,  0x45, 0xff   },
    {"Data7_8",  REGPOKER_READ8, 0x7,  0x67, 0xff   },
    {"Data8_8",  REGPOKER_READ8, 0x8,  0x89, 0xff   },
    {"Data9_8",  REGPOKER_READ8, 0x9,  0xAB, 0xff   },
    {"Data10_8", REGPOKER_READ8, 0xA,  0xCD, 0xff   },
    {"Data11_8", REGPOKER_READ8, 0xB,  0xEF, 0xff   },
    {"Data12_8", REGPOKER_READ8, 0xC,  0x00, 0xff   },
    {"Data13_8", REGPOKER_READ8, 0xD,  0x00, 0xff   },
    {"Data14_8", REGPOKER_READ8, 0xE,  0x00, 0xff   },
    {"Data15_8", REGPOKER_READ8, 0xF,  0x00, 0xff   },
    {"Data16_8", REGPOKER_READ8, 0x10, 0xff, 0xff   },
    {"Data17_8", REGPOKER_READ8, 0x11, 0xff, 0xff   },
    {"Data18_8", REGPOKER_READ8, 0x12, 0xff, 0xff   },
    {"Data19_8", REGPOKER_READ8, 0x13, 0xff, 0xff   },
    {"Data20_8", REGPOKER_READ8, 0x14, 0x55, 0xff   },
    {"Data21_8", REGPOKER_READ8, 0x15, 0x55, 0xff   },
    {"Data22_8", REGPOKER_READ8, 0x16, 0x55, 0xff   },
    {"Data23_8", REGPOKER_READ8, 0x17, 0x55, 0xff   },
    {"Data24_8", REGPOKER_READ8, 0x18, 0xCC, 0xff   },
    {"Data25_8", REGPOKER_READ8, 0x19, 0xCC, 0xff   },
    {"Data26_8", REGPOKER_READ8, 0x1A, 0xCC, 0xff   },
    {"Data27_8", REGPOKER_READ8, 0x1B, 0xCC, 0xff   },
    {"Data28_8", REGPOKER_READ8, 0x1C, 0x77, 0xff   },
    {"Data29_8", REGPOKER_READ8, 0x1D, 0x77, 0xff   },
    {"Data30_8", REGPOKER_READ8, 0x1E, 0x77, 0xff   },
    {"Data31_8", REGPOKER_READ8, 0x1F, 0x77, 0xff   },
    {/* Sentinel */}};
struct regpoker_checker em2_checker_b[] = {
    {"Data0_16",  REGPOKER_READ16, 0x0,  0x1010, 0xffff   },
    {"Data1_16",  REGPOKER_READ16, 0x2,  0x1010, 0xffff   },
    {"Data2_16",  REGPOKER_READ16, 0x4,  0x0123, 0xffff   },
    {"Data3_16",  REGPOKER_READ16, 0x6,  0x4567, 0xffff   },
    {"Data4_16",  REGPOKER_READ16, 0x8,  0x89AB, 0xffff   },
    {"Data5_16",  REGPOKER_READ16, 0xA,  0xCDEF, 0xffff   },
    {"Data6_16",  REGPOKER_READ16, 0xC,  0x0000, 0xffff   },
    {"Data7_16",  REGPOKER_READ16, 0xE,  0x0000, 0xffff   },
    {"Data8_16",  REGPOKER_READ16, 0x10, 0xffff, 0xffff   },
    {"Data9_16",  REGPOKER_READ16, 0x12, 0xffff, 0xffff   },
    {"Data10_16", REGPOKER_READ16, 0x14, 0x5555, 0xffff   },
    {"Data11_16", REGPOKER_READ16, 0x16, 0x5555, 0xffff   },
    {"Data12_16", REGPOKER_READ16, 0x18, 0xCCCC, 0xffff   },
    {"Data13_16", REGPOKER_READ16, 0x1A, 0xCCCC, 0xffff   },
    {"Data14_16", REGPOKER_READ16, 0x1C, 0x7777, 0xffff   },
    {"Data15_16", REGPOKER_READ16, 0x1E, 0x7777, 0xffff   },
    {/* Sentinel */}};
struct regpoker_checker em2_checker_c[] = {
    {"Data0_32", REGPOKER_READ32, 0x0,  0x10101010, 0xffffffff   },
    {"Data1_32", REGPOKER_READ32, 0x4,  0x01234567, 0xffffffff   },
    {"Data2_32", REGPOKER_READ32, 0x8,  0x89ABCDEF, 0xffffffff   },
    {"Data3_32", REGPOKER_READ32, 0xC,  0x00000000, 0xffffffff   },
    {"Data4_32", REGPOKER_READ32, 0x10, 0xffffffff, 0xffffffff   },
    {"Data5_32", REGPOKER_READ32, 0x14, 0x55555555, 0xffffffff   },
    {"Data6_32", REGPOKER_READ32, 0x18, 0xCCCCCCCC, 0xffffffff   },
    {"Data7_32", REGPOKER_READ32, 0x1C, 0x77777777, 0xffffffff   },

    {/* Sentinel */}};
struct regpoker_checker em2_checker_d[] = {
    {"Data0_64", REGPOKER_READ64, 0x0,  0x1010101001234567, 0xffffffffffffffff   },
    {"Data1_64", REGPOKER_READ64, 0x8,  0x89ABCDEF00000000, 0xffffffffffffffff   },
    {"Data2_64", REGPOKER_READ64, 0x10, 0xffffffff55555555, 0xffffffffffffffff   },
    {"Data3_64", REGPOKER_READ64, 0x18, 0xCCCCCCCC77777777, 0xffffffffffffffff   },
    {/* Sentinel */}};

// Processing src/platform/oi10/targets/simple-iram/test_oi10_ctrl_002_1.c / test_oi10_ctrl_002_1
#include <platform/regs/regs_crg_sys.h>
struct regpoker_checker crg_checker[] = {
    {"CRG_SYS_PLL_STAT",   REGPOKER_READ_DCR, CRG_SYS_PLL_STAT,   0x00000011, 0x00000011   },
    {"CRG_SYS_PLL_CTRL",   REGPOKER_READ_DCR, CRG_SYS_PLL_CTRL,   0x00000000, 0x00000003   },
    {"CRG_SYS_PLL_LDUR",   REGPOKER_READ_DCR, CRG_SYS_PLL_LDUR,   0x0000044C, 0x000007ff   },
    {"CRG_SYS_PLL_PRDIV",  REGPOKER_READ_DCR, CRG_SYS_PLL_PRDIV,  0x00000001, 0x0000001f   },
    {"CRG_SYS_PLL_FBDIV",  REGPOKER_READ_DCR, CRG_SYS_PLL_FBDIV,  0x00000020, 0x000007ff   },
    {"CRG_SYS_PLL_PSDIV",  REGPOKER_READ_DCR, CRG_SYS_PLL_PSDIV,  0x00000002, 0x00000007   },
    {"CRG_SYS_WR_LOCK",    REGPOKER_READ_DCR, CRG_SYS_WR_LOCK,    0x00000001, 0x00000001   },
    {"CRG_SYS_RST_MON",    REGPOKER_READ_DCR, CRG_SYS_RST_MON,    0x00000800, 0x00000f07   },
    {"CRG_SYS_RST_CFG0",   REGPOKER_READ_DCR, CRG_SYS_RST_CFG0,   0x00000096, 0x00000fff   },
    {"CRG_SYS_RST_CFG1",   REGPOKER_READ_DCR, CRG_SYS_RST_CFG1,   0x00960064, 0x0fff0fff   },
    {"CRG_SYS_RST_CFG2",   REGPOKER_READ_DCR, CRG_SYS_RST_CFG2,   0x0107000E, 0x0107002f   },
    {"CRG_SYS_CKUPDATE",   REGPOKER_READ_DCR, CRG_SYS_CKUPDATE,   0x00000000, 0x00000001   },
    {"CRG_SYS_INTMASK",    REGPOKER_READ_DCR, CRG_SYS_INTMASK,    0x00000000, 0x00000001   },
    {"CRG_SYS_INTCLR",     REGPOKER_READ_DCR, CRG_SYS_INTCLR,     0x00000000, 0x00000001   },
    {"CRG_SYS_CKDIVMODE0", REGPOKER_READ_DCR, CRG_SYS_CKDIVMODE0, 0x00000000, 0x0000001f   },
    {"CRG_SYS_CKDIVMODE2", REGPOKER_READ_DCR, CRG_SYS_CKDIVMODE2, 0x00000001, 0x0000001f   },
    {"CRG_SYS_CKDIVMODE3", REGPOKER_READ_DCR, CRG_SYS_CKDIVMODE3, 0x00000000, 0x0000001f   }, // pcb version
    //{"CRG_SYS_CKDIVMODE3", REGPOKER_READ_DCR, CRG_SYS_CKDIVMODE3, 0x00000013, 0x0000001f   }, // rtl version
    {"CRG_SYS_CKDIVMODE4", REGPOKER_READ_DCR, CRG_SYS_CKDIVMODE4, 0x00000009, 0x0000001f   },
    {}};

struct regpoker_checker crg_checker_rw[] = {
    //{"CRG_SYS_PLL_STAT",    REGPOKER_WRITE_DCR, CRG_SYS_PLL_STAT,   0x00000000, 0x00000000   },
    {"CRG_SYS_PLL_CTRL",    REGPOKER_WRITE_DCR, CRG_SYS_PLL_CTRL,   0x00000000, 0x00000003   },
    {"CRG_SYS_PLL_LDUR",    REGPOKER_WRITE_DCR, CRG_SYS_PLL_LDUR,   0x0000044C, 0x000007ff   },
    {"CRG_SYS_PLL_PRDIV",   REGPOKER_WRITE_DCR, CRG_SYS_PLL_PRDIV,  0x00000001, 0x0000001f   },
    {"CRG_SYS_PLL_FBDIV",   REGPOKER_WRITE_DCR, CRG_SYS_PLL_FBDIV,  0x00000020, 0x000007ff   },
    {"CRG_SYS_PLL_PSDIV",   REGPOKER_WRITE_DCR, CRG_SYS_PLL_PSDIV,  0x00000002, 0x00000007   },
    //{"CRG_SYS_WR_LOCK",     REGPOKER_WRITE_DCR, CRG_SYS_WR_LOCK,    0x00000001, 0xffffffff   },
    //{"CRG_SYS_RST_MON",     REGPOKER_WRITE_DCR, CRG_SYS_RST_MON,    0x00000800, 0x00000f07   },
    {"CRG_SYS_RST_CFG0",    REGPOKER_WRITE_DCR, CRG_SYS_RST_CFG0,   0x00000096, 0x00000fff   },
    {"CRG_SYS_RST_CFG1",    REGPOKER_WRITE_DCR, CRG_SYS_RST_CFG1,   0x00960064, 0x0fff0fff   },
    {"CRG_SYS_RST_CFG2",    REGPOKER_WRITE_DCR, CRG_SYS_RST_CFG2,   0x0107000E, 0x0107002f   },
    //{"CRG_SYS_CKUPDATE",    REGPOKER_WRITE_DCR, CRG_SYS_CKUPDATE,   0x00000000, 0x00000001   },
    {"CRG_SYS_INTMASK",     REGPOKER_WRITE_DCR, CRG_SYS_INTMASK,    0x00000000, 0x00000001   },
    //{"CRG_SYS_INTCLR",      REGPOKER_WRITE_DCR, CRG_SYS_INTCLR,     0x00000000, 0x00000001   },
    {"CRG_SYS_CKDIVMODE0",  REGPOKER_WRITE_DCR, CRG_SYS_CKDIVMODE0, 0x00000000, 0x0000001f   },
    {"CRG_SYS_CKDIVMODE2",  REGPOKER_WRITE_DCR, CRG_SYS_CKDIVMODE2, 0x00000001, 0x0000001f   },
    {"CRG_SYS_CKDIVMODE3",  REGPOKER_WRITE_DCR, CRG_SYS_CKDIVMODE3, 0x00000013, 0x0000001f   },
    {"CRG_SYS_CKDIVMODE4",  REGPOKER_WRITE_DCR, CRG_SYS_CKDIVMODE4, 0x00000009, 0x0000001f   },
    {}};
// Processing src/platform/oi10/targets/simple-iram/test_oi10_sys_012.c / test_oi10_sys_012
#include <platform/regs/regs_nic400.h>
struct regpoker_checker nic_checker[] = {
    {"NIC400_PERIPHERAL_ID4", REGPOKER_READ32, NIC400_PERIPHERAL_ID4, 0x04, 0xff   },
    {"NIC400_PERIPHERAL_ID5", REGPOKER_READ32, NIC400_PERIPHERAL_ID5, 0x00, 0xff   },
    {"NIC400_PERIPHERAL_ID6", REGPOKER_READ32, NIC400_PERIPHERAL_ID6, 0x00, 0xff   },
    {"NIC400_PERIPHERAL_ID7", REGPOKER_READ32, NIC400_PERIPHERAL_ID7, 0x00, 0xff   },
    {"NIC400_PERIPHERAL_ID0", REGPOKER_READ32, NIC400_PERIPHERAL_ID0, 0x00, 0xff   },
    {"NIC400_PERIPHERAL_ID1", REGPOKER_READ32, NIC400_PERIPHERAL_ID1, 0xB4, 0xff   },
    {"NIC400_PERIPHERAL_ID2", REGPOKER_READ32, NIC400_PERIPHERAL_ID2, 0x5B, 0xff   },
    {"NIC400_PERIPHERAL_ID3", REGPOKER_READ32, NIC400_PERIPHERAL_ID3, 0x00, 0xff   },
    {}};

struct regpoker_checker nic_checker_rw32[] = {
    {"NIC400_CPU_S_RQOS",      REGPOKER_READ32,  NIC400_CPU_S_RQOS,      0x0, 0b1111   },
    {"NIC400_CPU_S_RQOS",      REGPOKER_WRITE32, NIC400_CPU_S_RQOS,      0x0, 0b1111   },
    {"NIC400_CPU_S_WQOS",      REGPOKER_READ32,  NIC400_CPU_S_WQOS,      0x0, 0b1111   },
    {"NIC400_CPU_S_WQOS",      REGPOKER_WRITE32, NIC400_CPU_S_RQOS,      0x0, 0b1111   },
    {"NIC400_CPU_S_FN_MOD",    REGPOKER_READ32,  NIC400_CPU_S_FN_MOD,    0x0, 0b11     },
    {"NIC400_CPU_S_FN_MOD",    REGPOKER_WRITE32, NIC400_CPU_S_FN_MOD,    0x0, 0b11     },
    {"NIC400_GSPI_1_S_RQOS",   REGPOKER_READ32,  NIC400_GSPI_1_S_RQOS,   0x0, 0b1111   },
    {"NIC400_GSPI_1_S_RQOS",   REGPOKER_WRITE32, NIC400_GSPI_1_S_RQOS,   0x0, 0b1111   },
    {"NIC400_GSPI_1_S_WQOS",   REGPOKER_READ32,  NIC400_GSPI_1_S_WQOS,   0x0, 0b1111   },
    {"NIC400_GSPI_1_S_WQOS",   REGPOKER_WRITE32, NIC400_GSPI_1_S_WQOS,   0x0, 0b1111   },
    {"NIC400_GSPI_1_S_FN_MOD", REGPOKER_READ32,  NIC400_GSPI_1_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_GSPI_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_GSPI_1_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_ETH_1_S_RQOS",    REGPOKER_READ32,  NIC400_ETH_1_S_RQOS,    0x0, 0b1111   },
    {"NIC400_ETH_1_S_RQOS",    REGPOKER_WRITE32, NIC400_ETH_1_S_RQOS,    0x0, 0b1111   },
    {"NIC400_ETH_1_S_WQOS",    REGPOKER_READ32,  NIC400_ETH_1_S_WQOS,    0x0, 0b1111   },
    {"NIC400_ETH_1_S_WQOS",    REGPOKER_WRITE32, NIC400_ETH_1_S_WQOS,    0x0, 0b1111   },
    {"NIC400_ETH_1_S_FN_MOD",  REGPOKER_READ32,  NIC400_ETH_1_S_FN_MOD,  0x0, 0b11     },
    {"NIC400_ETH_1_S_FN_MOD",  REGPOKER_WRITE32, NIC400_ETH_1_S_FN_MOD,  0x0, 0b11     },
    {"NIC400_SW_0_3_S_RQOS",   REGPOKER_READ32,  NIC400_SW_0_3_S_RQOS,   0x4, 0b1111   },
    {"NIC400_SW_0_3_S_RQOS",   REGPOKER_WRITE32, NIC400_SW_0_3_S_RQOS,   0x4, 0b1111   },
    {"NIC400_SW_0_3_S_WQOS",   REGPOKER_READ32,  NIC400_SW_0_3_S_WQOS,   0x4, 0b1111   },
    {"NIC400_SW_0_3_S_WQOS",   REGPOKER_WRITE32, NIC400_SW_0_3_S_WQOS,   0x4, 0b1111   },
    {"NIC400_SW_0_3_S_FN_MOD", REGPOKER_READ32,  NIC400_SW_0_3_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_SW_0_3_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_0_3_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_GSPI_0_S_RQOS",   REGPOKER_READ32,  NIC400_GSPI_0_S_RQOS,   0x0, 0b1111   },
    {"NIC400_GSPI_0_S_RQOS",   REGPOKER_WRITE32, NIC400_GSPI_0_S_RQOS,   0x0, 0b1111   },
    {"NIC400_GSPI_0_S_WQOS",   REGPOKER_READ32,  NIC400_GSPI_0_S_WQOS,   0x0, 0b1111   },
    {"NIC400_GSPI_0_S_WQOS",   REGPOKER_WRITE32, NIC400_GSPI_0_S_WQOS,   0x0, 0b1111   },
    {"NIC400_GSPI_0_S_FN_MOD", REGPOKER_READ32,  NIC400_GSPI_0_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_GSPI_0_S_FN_MOD", REGPOKER_WRITE32, NIC400_GSPI_0_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_ETH_0_S_RQOS",    REGPOKER_READ32,  NIC400_ETH_0_S_RQOS,    0x0, 0b1111   },
    {"NIC400_ETH_0_S_RQOS",    REGPOKER_WRITE32, NIC400_ETH_0_S_RQOS,    0x0, 0b1111   },
    {"NIC400_ETH_0_S_WQOS",    REGPOKER_READ32,  NIC400_ETH_0_S_WQOS,    0x0, 0b1111   },
    {"NIC400_ETH_0_S_WQOS",    REGPOKER_WRITE32, NIC400_ETH_0_S_WQOS,    0x0, 0b1111   },
    {"NIC400_ETH_0_S_FN_MOD",  REGPOKER_READ32,  NIC400_ETH_0_S_FN_MOD,  0x0, 0b11     },
    {"NIC400_ETH_0_S_FN_MOD",  REGPOKER_WRITE32, NIC400_ETH_0_S_FN_MOD,  0x0, 0b11     },
    {"NIC400_MKIO_0_S_RQOS",   REGPOKER_READ32,  NIC400_MKIO_0_S_RQOS,   0x0, 0b1111   },
    {"NIC400_MKIO_0_S_RQOS",   REGPOKER_WRITE32, NIC400_MKIO_0_S_RQOS,   0x0, 0b1111   },
    {"NIC400_MKIO_0_S_WQOS",   REGPOKER_READ32,  NIC400_MKIO_0_S_WQOS,   0x0, 0b1111   },
    {"NIC400_MKIO_0_S_WQOS",   REGPOKER_WRITE32, NIC400_MKIO_0_S_WQOS,   0x0, 0b1111   },
    {"NIC400_MKIO_0_S_FN_MOD", REGPOKER_READ32,  NIC400_MKIO_0_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_MKIO_0_S_FN_MOD", REGPOKER_WRITE32, NIC400_MKIO_0_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_MKIO_1_S_RQOS",   REGPOKER_READ32,  NIC400_MKIO_1_S_RQOS,   0x0, 0b1111   },
    {"NIC400_MKIO_1_S_RQOS",   REGPOKER_WRITE32, NIC400_MKIO_1_S_RQOS,   0x0, 0b1111   },
    {"NIC400_MKIO_1_S_WQOS",   REGPOKER_READ32,  NIC400_MKIO_1_S_WQOS,   0x0, 0b1111   },
    {"NIC400_MKIO_1_S_WQOS",   REGPOKER_WRITE32, NIC400_MKIO_1_S_WQOS,   0x0, 0b1111   },
    {"NIC400_MKIO_1_S_FN_MOD", REGPOKER_READ32,  NIC400_MKIO_1_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_MKIO_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_MKIO_1_S_FN_MOD, 0x0, 0b11     },

    {}};

struct regpoker_checker nic_checker_rw64[] = {
    {"NIC400_SW_0_S_RQOS",     REGPOKER_READ32,  NIC400_SW_0_S_RQOS,     0x0, 0b1111   },
    {"NIC400_SW_0_S_RQOS",     REGPOKER_WRITE32, NIC400_SW_0_S_RQOS,     0x0, 0b1111   },
    {"NIC400_SW_0_S_WQOS",     REGPOKER_READ32,  NIC400_SW_0_S_WQOS,     0x0, 0b1111   },
    {"NIC400_SW_0_S_WQOS",     REGPOKER_WRITE32, NIC400_SW_0_S_WQOS,     0x0, 0b1111   },
    {"NIC400_SW_0_S_FN_MOD",   REGPOKER_READ32,  NIC400_SW_0_S_FN_MOD,   0x0, 0b11     },
    {"NIC400_SW_0_S_FN_MOD",   REGPOKER_WRITE32, NIC400_SW_0_S_FN_MOD,   0x0, 0b11     },
    {"NIC400_SW_1_S_RQOS",     REGPOKER_READ32,  NIC400_SW_1_S_RQOS,     0x0, 0b1111   },
    {"NIC400_SW_1_S_RQOS",     REGPOKER_WRITE32, NIC400_SW_1_S_RQOS,     0x0, 0b1111   },
    {"NIC400_SW_1_S_WQOS",     REGPOKER_READ32,  NIC400_SW_1_S_WQOS,     0x0, 0b1111   },
    {"NIC400_SW_1_S_WQOS",     REGPOKER_WRITE32, NIC400_SW_1_S_WQOS,     0x0, 0b1111   },
    {"NIC400_SW_1_S_FN_MOD",   REGPOKER_READ32,  NIC400_SW_1_S_FN_MOD,   0x0, 0b11     },
    {"NIC400_SW_1_S_FN_MOD",   REGPOKER_WRITE32, NIC400_SW_1_S_FN_MOD,   0x0, 0b11     },
    {"NIC400_SW_2_1_S_RQOS",   REGPOKER_READ32,  NIC400_SW_2_1_S_RQOS,   0x0, 0b1111   },
    {"NIC400_SW_2_1_S_RQOS",   REGPOKER_WRITE32, NIC400_SW_2_1_S_RQOS,   0x0, 0b1111   },
    {"NIC400_SW_2_1_S_WQOS",   REGPOKER_READ32,  NIC400_SW_2_1_S_WQOS,   0x0, 0b1111   },
    {"NIC400_SW_2_1_S_WQOS",   REGPOKER_WRITE32, NIC400_SW_2_1_S_WQOS,   0x0, 0b1111   },
    {"NIC400_SW_2_1_S_FN_MOD", REGPOKER_READ32,  NIC400_SW_2_1_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_SW_2_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_2_1_S_FN_MOD, 0x0, 0b11     },
    {"NIC400_SW_3_S_RQOS",     REGPOKER_READ32,  NIC400_SW_3_S_RQOS,     0x0, 0b1111   },
    {"NIC400_SW_3_S_RQOS",     REGPOKER_WRITE32, NIC400_SW_3_S_RQOS,     0x0, 0b1111   },
    {"NIC400_SW_3_S_WQOS",     REGPOKER_READ32,  NIC400_SW_3_S_WQOS,     0x0, 0b1111   },
    {"NIC400_SW_3_S_WQOS",     REGPOKER_WRITE32, NIC400_SW_3_S_WQOS,     0x0, 0b1111   },
    {"NIC400_SW_3_S_FN_MOD",   REGPOKER_READ32,  NIC400_SW_3_S_FN_MOD,   0x0, 0b11     },
    {"NIC400_SW_3_S_FN_MOD",   REGPOKER_WRITE32, NIC400_SW_3_S_FN_MOD,   0x0, 0b11     },
    {}};


#include <platform/regs/regs_mcif2arb4.h>

static const struct regpoker_checker mcif2arb4_regs_default[] = {
    //      name                check type          offset         exp val                  mask
    {"MCIF2ARB4_MACR",      REGPOKER_READ_DCR, MCIF2ARB4_MACR,      MCIF2ARB4_MACR_DFLT,       MCIF2ARB4_MACR_MSK},
    
    
    {/* Sentinel */}};   
    
#include <platform/regs/regs_plb6plb4.h>

#define P64CR_MSK     0x0fff0703
#define ESR_MSK       0xfffff001
#define P64REV_MSK    0x000fffff
#define EARH_MSK      0xffffffff
#define EARL_MSK      0xffffffff
#define ADDRL1_MSK    0xffffffff
#define ADDRH1_MSK    0xffffffff
#define TESR_MSK      0xfffff001

static const struct regpoker_checker plb6plb4_0_regs_default[] = {
    //   name                check type         offset       exp val                mask
    {"PLB6PLB4_P64CR ",      REGPOKER_READ_DCR, P64CR ,      0x00000303,            P64CR_MSK    },
    {"PLB6PLB4_ESR   ",      REGPOKER_READ_DCR, ESR   ,      ESR_RESET_VAL,         ESR_MSK      },
    {"PLB6PLB4_P64REV",      REGPOKER_READ_DCR, P64REV,      P64REV_RESET_VAL,      P64REV_MSK   },
    {"PLB6PLB4_EARH  ",      REGPOKER_READ_DCR, EARH  ,      EARH_RESET_VAL,        EARH_MSK     },
    {"PLB6PLB4_EARL  ",      REGPOKER_READ_DCR, EARL  ,      EARL_RESET_VAL,        EARL_MSK     },
    //{"PLB6PLB4_ADDRL0",      REGPOKER_READ_DCR, ADDRL0,      ADDRL0_RESET_VAL,      ADDRL0_MSK   }, belong to input strap
    //{"PLB6PLB4_ADDRH0",      REGPOKER_READ_DCR, ADDRH0,      ADDRH0_RESET_VAL,      ADDRH0_MSK   },
    {"PLB6PLB4_ADDRL1",      REGPOKER_READ_DCR, ADDRL1,      0x10,                  ADDRL1_MSK   },
    {"PLB6PLB4_ADDRH1",      REGPOKER_READ_DCR, ADDRH1,      0x1f,                  ADDRH1_MSK   },
    {"PLB6PLB4_TESR  ",      REGPOKER_READ_DCR, TESR  ,      TESR_RESET_VAL,        TESR_MSK     },
    
    
    {/* Sentinel */}}; 
    
static const struct regpoker_checker plb6plb4_1_regs_default[] = {
    //   name                check type         offset       exp val                mask
    {"PLB6PLB4_P64CR ",      REGPOKER_READ_DCR, P64CR ,      P64CR_0_RESET_VAL,     P64CR_MSK   },
    {"PLB6PLB4_ESR   ",      REGPOKER_READ_DCR, ESR   ,      ESR_RESET_VAL,         ESR_MSK     },
    {"PLB6PLB4_P64REV",      REGPOKER_READ_DCR, P64REV,      P64REV_RESET_VAL,      P64REV_MSK  },
    {"PLB6PLB4_EARH  ",      REGPOKER_READ_DCR, EARH  ,      EARH_RESET_VAL,        EARH_MSK    },
    {"PLB6PLB4_EARL  ",      REGPOKER_READ_DCR, EARL  ,      EARL_RESET_VAL,        EARL_MSK    },
    //{"PLB6PLB4_ADDRL0",      REGPOKER_READ_DCR, ADDRL0,      ADDRL0_RESET_VAL,      ADDRL0_MSK  }, belong to input strap
    //{"PLB6PLB4_ADDRH0",      REGPOKER_READ_DCR, ADDRH0,      ADDRH0_RESET_VAL,      ADDRH0_MSK  },
    {"PLB6PLB4_ADDRL1",      REGPOKER_READ_DCR, ADDRL1,      ADDRL1_RESET_VAL,      ADDRL1_MSK  },
    {"PLB6PLB4_ADDRH1",      REGPOKER_READ_DCR, ADDRH1,      ADDRH1_RESET_VAL,      ADDRH1_MSK  },
    {"PLB6PLB4_TESR  ",      REGPOKER_READ_DCR, TESR  ,      TESR_RESET_VAL,        TESR_MSK    },
    
    
    {/* Sentinel */}};
    
//#include <platform/regs/regs_cldcr.h> // doesn't compile with this include, test_oi10_sys 010 already has this include...

#define CLDCR_TCRREV       0x0
#define CLDCR_ITRCTRL      0x1
#define CLDCR_LTRCTRL      0x2

#define CLDCR_TCRREV_DFLT  0x00000101
#define CLDCR_ITRCTRL_DFLT 0x0
#define CLDCR_LTRCTRL_DFLT 0x0

#define CLDCR_TCRREV_MSK   0x000fffff
#define CLDCR_ITRCTRL_MSK  0x000000ff
#define CLDCR_LTRCTRL_MSK  0x000000ff

static const struct regpoker_checker cldcr_regs_default[] = {
    //      name                check type          offset         exp val                  mask
    {"CLDCR_TCRREV",       REGPOKER_READ_DCR, CLDCR_TCRREV,      CLDCR_TCRREV_DFLT,       CLDCR_TCRREV_MSK    },
    {"CLDCR_ITRCTRL",      REGPOKER_READ_DCR, CLDCR_ITRCTRL,     CLDCR_ITRCTRL_DFLT,      CLDCR_ITRCTRL_MSK   },
    {"CLDCR_LTRCTRL",      REGPOKER_READ_DCR, CLDCR_LTRCTRL,     CLDCR_LTRCTRL_DFLT,      CLDCR_LTRCTRL_MSK   },
    
    
    
    {/* Sentinel */}};   

#include <platform/regs/regs_plb4arb8m.h>

#define PREV_MSK 0x000fffff
#define PACR_MSK 0xff000000
#define PESR_MSK 0xffffffff
#define PEAR_MSK 0xffffffff
 
static const struct regpoker_checker plb4arb8m_0_regs_default[] = {
    //name         check type         offset     exp val                  mask
    {"PREV",       REGPOKER_READ_DCR, PREV,      PREV_0_RESET_VAL,       PREV_MSK   },
    {"PACR",       REGPOKER_READ_DCR, PACR,      PACR_0_RESET_VAL,       PACR_MSK   },
    {"PESR_RC",    REGPOKER_READ_DCR, PESR_RC,   PESR_0_RESET_VAL,       PESR_MSK   },
    {"PESR_W",     REGPOKER_READ_DCR, PESR_W,    PESR_0_RESET_VAL,       PESR_MSK   },
    //{"PEARL",      REGPOKER_READ_DCR, PEARL,     PEARL_RESET_VAL,      PEAR_MSK     },
    //{"PEARH",      REGPOKER_READ_DCR, PEARH,     PEARH_RESET_VAL,      PEAR_MSK     },    
    
    {/* Sentinel */}};     
    
static const struct regpoker_checker plb4arb8m_2_regs_default[] = {
    //name         check type         offset     exp val                  mask
    {"PREV",       REGPOKER_READ_DCR, PREV,      0x00000000,             PREV_MSK   },
    {"PACR",       REGPOKER_READ_DCR, PACR,      0x00000000,             PACR_MSK   },
    {"PESR_RC",    REGPOKER_READ_DCR, PESR_RC,   PESR_0_RESET_VAL,       PESR_MSK   },
    {"PESR_W",     REGPOKER_READ_DCR, PESR_W,    PESR_0_RESET_VAL,       PESR_MSK   },
    //{"PEARL",      REGPOKER_READ_DCR, PEARL,     PEARL_RESET_VAL,      PEAR_MSK     },
    //{"PEARH",      REGPOKER_READ_DCR, PEARH,     PEARH_RESET_VAL,      PEAR_MSK     },    
    
    {/* Sentinel */}}; 
    
#include <platform/regs/regs_plb4ahb.h>

#define REVID_MSK          0xffffffff
#define SEAR_ADDRUPPER_MSK 0x0000000f
#define SEAR_ADDRLOWER_MSK 0xffffffff
#define SESR_MSK           0xffffc00f
#define TOP_ADDR_MSK       0x800000ff
#define BOT_ADDR_MSK       0x800000ff
#define ATTRS_MSK          0x00000001

static const struct regpoker_checker plb4ahb_regs_default[] = {
    
    //name                   check type         offset               exp val                     mask  
    //uni-directional
    {"REVID",                REGPOKER_READ_DCR, REVID,               REVID_VALUE,                REVID_MSK            },
    {"SEAR_ADDRUPPER",       REGPOKER_READ_DCR, SEAR_ADDRUPPER,      SEAR_ADDRUPPER_VALUE,       SEAR_ADDRUPPER_MSK   },
    {"SEAR_ADDRLOWER",       REGPOKER_READ_DCR, SEAR_ADDRLOWER,      SEAR_ADDRLOWER_VALUE,       SEAR_ADDRLOWER_MSK   },
    {"SESR",                 REGPOKER_READ_DCR, SESR,                SESR_VALUE,                 SESR_MSK             },
    {"TOP_ADDR",             REGPOKER_READ_DCR, TOP_ADDR,            TOP_ADDR_VALUE,             TOP_ADDR_MSK         },
    {"BOT_ADDR",             REGPOKER_READ_DCR, BOT_ADDR,            BOT_ADDR_VALUE,             BOT_ADDR_MSK         },
    {"ATTRS",                REGPOKER_READ_DCR, ATTRS,               ATTRS_VALUE,                ATTRS_MSK            },
   
    
    {/* Sentinel */}};  
    
#include <regs/regs_mpic128.h>

#define MPIC128_VPx_MSK       0xc0cf00ff
#define MPIC128_DSTx_MSK      0x0000000f
#define MPIC128_CTPx_MSK      0x0000000f
#define MPIC128_WAIx_MSK      0x00000003
#define MPIC128_NCIARx_MSK    0x000000ff
#define MPIC128_NCEOIx_MSK    0xffffffff
#define MPIC128_CIARx_MSK     0x000000ff
#define MPIC128_CEOIx_MSK     0xffffffff
#define MPIC128_IARx_MSK      0x000000ff
#define MPIC128_EOIx_MSK      0xffffffff
#define MPIC128_FRG0_MSK      0x07ff1fff
#define MPIC128_GCF0_MSK      0xa000000f
#define MPIC128_VITC_MSK      0x00001f1f
#define MPIC128_VRID_MSK      0xfffff007
#define MPIC128_REVID_MSK     0x000fffff
#define MPIC128_VIR0_MSK      0x00ffffff
#define MPIC128_PINI_MSK      0x0000000f
#define MPIC128_IVPx_MSK      0xc00f00ff
#define MPIC128_SPV_MSK       0x000000ff
#define MPIC128_TFR_MSK       0xffffffff
#define MPIC128_TCCx_MSK      0xffffffff
#define MPIC128_TBCx_MSK      0xffffffff
#define MPIC128_TVPx_MSK      0xffffffff
#define MPIC128_TDSTx_MSK     0xffffffff


static const struct regpoker_checker mpic128_regs_default[] = {
    
    //name                         check type         offset                     exp val                           mask  
    {"MPIC128_VP0",                REGPOKER_READ_DCR, MPIC128_VP0,               MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP1",                REGPOKER_READ_DCR, MPIC128_VP1,               MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP2",                REGPOKER_READ_DCR, MPIC128_VP2,               MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP3",                REGPOKER_READ_DCR, MPIC128_VP3,               MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP4",                REGPOKER_READ_DCR, MPIC128_VP4,               MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP5",                REGPOKER_READ_DCR, MPIC128_VP5,               MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP6",                REGPOKER_READ_DCR, MPIC128_VP6,               MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP7",                REGPOKER_READ_DCR, MPIC128_VP7,               MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP8",                REGPOKER_READ_DCR, MPIC128_VP8,               MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP9",                REGPOKER_READ_DCR, MPIC128_VP9,               MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP10",               REGPOKER_READ_DCR, MPIC128_VP10,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP11",               REGPOKER_READ_DCR, MPIC128_VP11,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP12",               REGPOKER_READ_DCR, MPIC128_VP12,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP13",               REGPOKER_READ_DCR, MPIC128_VP13,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP14",               REGPOKER_READ_DCR, MPIC128_VP14,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP15",               REGPOKER_READ_DCR, MPIC128_VP15,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP16",               REGPOKER_READ_DCR, MPIC128_VP16,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP17",               REGPOKER_READ_DCR, MPIC128_VP17,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP18",               REGPOKER_READ_DCR, MPIC128_VP18,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP19",               REGPOKER_READ_DCR, MPIC128_VP19,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP20",               REGPOKER_READ_DCR, MPIC128_VP20,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP21",               REGPOKER_READ_DCR, MPIC128_VP21,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP22",               REGPOKER_READ_DCR, MPIC128_VP22,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP23",               REGPOKER_READ_DCR, MPIC128_VP23,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP24",               REGPOKER_READ_DCR, MPIC128_VP24,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP25",               REGPOKER_READ_DCR, MPIC128_VP25,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP26",               REGPOKER_READ_DCR, MPIC128_VP26,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP27",               REGPOKER_READ_DCR, MPIC128_VP27,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP28",               REGPOKER_READ_DCR, MPIC128_VP28,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP29",               REGPOKER_READ_DCR, MPIC128_VP29,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP30",               REGPOKER_READ_DCR, MPIC128_VP30,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP31",               REGPOKER_READ_DCR, MPIC128_VP31,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP32",               REGPOKER_READ_DCR, MPIC128_VP32,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP33",               REGPOKER_READ_DCR, MPIC128_VP33,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP34",               REGPOKER_READ_DCR, MPIC128_VP34,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP35",               REGPOKER_READ_DCR, MPIC128_VP35,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP36",               REGPOKER_READ_DCR, MPIC128_VP36,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP37",               REGPOKER_READ_DCR, MPIC128_VP37,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP38",               REGPOKER_READ_DCR, MPIC128_VP38,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP39",               REGPOKER_READ_DCR, MPIC128_VP39,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP40",               REGPOKER_READ_DCR, MPIC128_VP40,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP41",               REGPOKER_READ_DCR, MPIC128_VP41,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP42",               REGPOKER_READ_DCR, MPIC128_VP42,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP43",               REGPOKER_READ_DCR, MPIC128_VP43,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP44",               REGPOKER_READ_DCR, MPIC128_VP44,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP45",               REGPOKER_READ_DCR, MPIC128_VP45,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP46",               REGPOKER_READ_DCR, MPIC128_VP46,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP47",               REGPOKER_READ_DCR, MPIC128_VP47,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP48",               REGPOKER_READ_DCR, MPIC128_VP48,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP49",               REGPOKER_READ_DCR, MPIC128_VP49,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP50",               REGPOKER_READ_DCR, MPIC128_VP50,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP51",               REGPOKER_READ_DCR, MPIC128_VP51,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP52",               REGPOKER_READ_DCR, MPIC128_VP52,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP53",               REGPOKER_READ_DCR, MPIC128_VP53,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP54",               REGPOKER_READ_DCR, MPIC128_VP54,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP55",               REGPOKER_READ_DCR, MPIC128_VP55,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP56",               REGPOKER_READ_DCR, MPIC128_VP56,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP57",               REGPOKER_READ_DCR, MPIC128_VP57,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP58",               REGPOKER_READ_DCR, MPIC128_VP58,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP59",               REGPOKER_READ_DCR, MPIC128_VP59,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP60",               REGPOKER_READ_DCR, MPIC128_VP60,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP61",               REGPOKER_READ_DCR, MPIC128_VP61,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP62",               REGPOKER_READ_DCR, MPIC128_VP62,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP63",               REGPOKER_READ_DCR, MPIC128_VP63,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP64",               REGPOKER_READ_DCR, MPIC128_VP64,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP65",               REGPOKER_READ_DCR, MPIC128_VP65,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP66",               REGPOKER_READ_DCR, MPIC128_VP66,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP67",               REGPOKER_READ_DCR, MPIC128_VP67,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP68",               REGPOKER_READ_DCR, MPIC128_VP68,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP69",               REGPOKER_READ_DCR, MPIC128_VP69,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP70",               REGPOKER_READ_DCR, MPIC128_VP70,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP71",               REGPOKER_READ_DCR, MPIC128_VP71,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP72",               REGPOKER_READ_DCR, MPIC128_VP72,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP73",               REGPOKER_READ_DCR, MPIC128_VP73,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP74",               REGPOKER_READ_DCR, MPIC128_VP74,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP75",               REGPOKER_READ_DCR, MPIC128_VP75,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP76",               REGPOKER_READ_DCR, MPIC128_VP76,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP77",               REGPOKER_READ_DCR, MPIC128_VP77,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP78",               REGPOKER_READ_DCR, MPIC128_VP78,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP79",               REGPOKER_READ_DCR, MPIC128_VP79,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP80",               REGPOKER_READ_DCR, MPIC128_VP80,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP81",               REGPOKER_READ_DCR, MPIC128_VP81,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP82",               REGPOKER_READ_DCR, MPIC128_VP82,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP83",               REGPOKER_READ_DCR, MPIC128_VP83,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP84",               REGPOKER_READ_DCR, MPIC128_VP84,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP85",               REGPOKER_READ_DCR, MPIC128_VP85,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP86",               REGPOKER_READ_DCR, MPIC128_VP86,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP87",               REGPOKER_READ_DCR, MPIC128_VP87,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP88",               REGPOKER_READ_DCR, MPIC128_VP88,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP89",               REGPOKER_READ_DCR, MPIC128_VP89,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP90",               REGPOKER_READ_DCR, MPIC128_VP90,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP91",               REGPOKER_READ_DCR, MPIC128_VP91,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP92",               REGPOKER_READ_DCR, MPIC128_VP92,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP93",               REGPOKER_READ_DCR, MPIC128_VP93,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP94",               REGPOKER_READ_DCR, MPIC128_VP94,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP95",               REGPOKER_READ_DCR, MPIC128_VP95,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP96",               REGPOKER_READ_DCR, MPIC128_VP96,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP97",               REGPOKER_READ_DCR, MPIC128_VP97,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP98",               REGPOKER_READ_DCR, MPIC128_VP98,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP99",               REGPOKER_READ_DCR, MPIC128_VP99,              MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP100",              REGPOKER_READ_DCR, MPIC128_VP100,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP101",              REGPOKER_READ_DCR, MPIC128_VP101,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP102",              REGPOKER_READ_DCR, MPIC128_VP102,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP103",              REGPOKER_READ_DCR, MPIC128_VP103,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP104",              REGPOKER_READ_DCR, MPIC128_VP104,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP105",              REGPOKER_READ_DCR, MPIC128_VP105,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP106",              REGPOKER_READ_DCR, MPIC128_VP106,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP107",              REGPOKER_READ_DCR, MPIC128_VP107,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP108",              REGPOKER_READ_DCR, MPIC128_VP108,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP109",              REGPOKER_READ_DCR, MPIC128_VP109,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP110",              REGPOKER_READ_DCR, MPIC128_VP110,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP111",              REGPOKER_READ_DCR, MPIC128_VP111,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP112",              REGPOKER_READ_DCR, MPIC128_VP112,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP113",              REGPOKER_READ_DCR, MPIC128_VP113,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP114",              REGPOKER_READ_DCR, MPIC128_VP114,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP115",              REGPOKER_READ_DCR, MPIC128_VP115,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP116",              REGPOKER_READ_DCR, MPIC128_VP116,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP117",              REGPOKER_READ_DCR, MPIC128_VP117,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP118",              REGPOKER_READ_DCR, MPIC128_VP118,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP119",              REGPOKER_READ_DCR, MPIC128_VP119,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP120",              REGPOKER_READ_DCR, MPIC128_VP120,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP121",              REGPOKER_READ_DCR, MPIC128_VP121,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP122",              REGPOKER_READ_DCR, MPIC128_VP122,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP123",              REGPOKER_READ_DCR, MPIC128_VP123,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP124",              REGPOKER_READ_DCR, MPIC128_VP124,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP125",              REGPOKER_READ_DCR, MPIC128_VP125,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP126",              REGPOKER_READ_DCR, MPIC128_VP126,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    {"MPIC128_VP127",              REGPOKER_READ_DCR, MPIC128_VP127,             MPIC128_VP0_VALUE,                MPIC128_VPx_MSK},
    
    {"MPIC128_DST0",                REGPOKER_READ_DCR, MPIC128_DST0,               MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST1",                REGPOKER_READ_DCR, MPIC128_DST1,               MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST2",                REGPOKER_READ_DCR, MPIC128_DST2,               MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST3",                REGPOKER_READ_DCR, MPIC128_DST3,               MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST4",                REGPOKER_READ_DCR, MPIC128_DST4,               MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST5",                REGPOKER_READ_DCR, MPIC128_DST5,               MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST6",                REGPOKER_READ_DCR, MPIC128_DST6,               MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST7",                REGPOKER_READ_DCR, MPIC128_DST7,               MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST8",                REGPOKER_READ_DCR, MPIC128_DST8,               MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST9",                REGPOKER_READ_DCR, MPIC128_DST9,               MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST10",               REGPOKER_READ_DCR, MPIC128_DST10,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST11",               REGPOKER_READ_DCR, MPIC128_DST11,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST12",               REGPOKER_READ_DCR, MPIC128_DST12,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST13",               REGPOKER_READ_DCR, MPIC128_DST13,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST14",               REGPOKER_READ_DCR, MPIC128_DST14,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST15",               REGPOKER_READ_DCR, MPIC128_DST15,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST16",               REGPOKER_READ_DCR, MPIC128_DST16,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST17",               REGPOKER_READ_DCR, MPIC128_DST17,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST18",               REGPOKER_READ_DCR, MPIC128_DST18,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST19",               REGPOKER_READ_DCR, MPIC128_DST19,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST20",               REGPOKER_READ_DCR, MPIC128_DST20,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST21",               REGPOKER_READ_DCR, MPIC128_DST21,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST22",               REGPOKER_READ_DCR, MPIC128_DST22,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST23",               REGPOKER_READ_DCR, MPIC128_DST23,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST24",               REGPOKER_READ_DCR, MPIC128_DST24,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST25",               REGPOKER_READ_DCR, MPIC128_DST25,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST26",               REGPOKER_READ_DCR, MPIC128_DST26,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST27",               REGPOKER_READ_DCR, MPIC128_DST27,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST28",               REGPOKER_READ_DCR, MPIC128_DST28,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST29",               REGPOKER_READ_DCR, MPIC128_DST29,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST30",               REGPOKER_READ_DCR, MPIC128_DST30,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST31",               REGPOKER_READ_DCR, MPIC128_DST31,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST32",               REGPOKER_READ_DCR, MPIC128_DST32,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST33",               REGPOKER_READ_DCR, MPIC128_DST33,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST34",               REGPOKER_READ_DCR, MPIC128_DST34,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST35",               REGPOKER_READ_DCR, MPIC128_DST35,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST36",               REGPOKER_READ_DCR, MPIC128_DST36,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST37",               REGPOKER_READ_DCR, MPIC128_DST37,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST38",               REGPOKER_READ_DCR, MPIC128_DST38,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST39",               REGPOKER_READ_DCR, MPIC128_DST39,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST40",               REGPOKER_READ_DCR, MPIC128_DST40,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST41",               REGPOKER_READ_DCR, MPIC128_DST41,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST42",               REGPOKER_READ_DCR, MPIC128_DST42,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST43",               REGPOKER_READ_DCR, MPIC128_DST43,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST44",               REGPOKER_READ_DCR, MPIC128_DST44,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST45",               REGPOKER_READ_DCR, MPIC128_DST45,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST46",               REGPOKER_READ_DCR, MPIC128_DST46,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST47",               REGPOKER_READ_DCR, MPIC128_DST47,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST48",               REGPOKER_READ_DCR, MPIC128_DST48,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST49",               REGPOKER_READ_DCR, MPIC128_DST49,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST50",               REGPOKER_READ_DCR, MPIC128_DST50,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST51",               REGPOKER_READ_DCR, MPIC128_DST51,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST52",               REGPOKER_READ_DCR, MPIC128_DST52,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST53",               REGPOKER_READ_DCR, MPIC128_DST53,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST54",               REGPOKER_READ_DCR, MPIC128_DST54,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST55",               REGPOKER_READ_DCR, MPIC128_DST55,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST56",               REGPOKER_READ_DCR, MPIC128_DST56,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST57",               REGPOKER_READ_DCR, MPIC128_DST57,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST58",               REGPOKER_READ_DCR, MPIC128_DST58,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST59",               REGPOKER_READ_DCR, MPIC128_DST59,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST60",               REGPOKER_READ_DCR, MPIC128_DST60,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST61",               REGPOKER_READ_DCR, MPIC128_DST61,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST62",               REGPOKER_READ_DCR, MPIC128_DST62,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST63",               REGPOKER_READ_DCR, MPIC128_DST63,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST64",               REGPOKER_READ_DCR, MPIC128_DST64,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST65",               REGPOKER_READ_DCR, MPIC128_DST65,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST66",               REGPOKER_READ_DCR, MPIC128_DST66,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST67",               REGPOKER_READ_DCR, MPIC128_DST67,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST68",               REGPOKER_READ_DCR, MPIC128_DST68,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST69",               REGPOKER_READ_DCR, MPIC128_DST69,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST70",               REGPOKER_READ_DCR, MPIC128_DST70,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST71",               REGPOKER_READ_DCR, MPIC128_DST71,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST72",               REGPOKER_READ_DCR, MPIC128_DST72,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST73",               REGPOKER_READ_DCR, MPIC128_DST73,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST74",               REGPOKER_READ_DCR, MPIC128_DST74,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST75",               REGPOKER_READ_DCR, MPIC128_DST75,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST76",               REGPOKER_READ_DCR, MPIC128_DST76,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST77",               REGPOKER_READ_DCR, MPIC128_DST77,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST78",               REGPOKER_READ_DCR, MPIC128_DST78,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST79",               REGPOKER_READ_DCR, MPIC128_DST79,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST80",               REGPOKER_READ_DCR, MPIC128_DST80,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST81",               REGPOKER_READ_DCR, MPIC128_DST81,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST82",               REGPOKER_READ_DCR, MPIC128_DST82,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST83",               REGPOKER_READ_DCR, MPIC128_DST83,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST84",               REGPOKER_READ_DCR, MPIC128_DST84,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST85",               REGPOKER_READ_DCR, MPIC128_DST85,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST86",               REGPOKER_READ_DCR, MPIC128_DST86,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST87",               REGPOKER_READ_DCR, MPIC128_DST87,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST88",               REGPOKER_READ_DCR, MPIC128_DST88,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST89",               REGPOKER_READ_DCR, MPIC128_DST89,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST90",               REGPOKER_READ_DCR, MPIC128_DST90,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST91",               REGPOKER_READ_DCR, MPIC128_DST91,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST92",               REGPOKER_READ_DCR, MPIC128_DST92,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST93",               REGPOKER_READ_DCR, MPIC128_DST93,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST94",               REGPOKER_READ_DCR, MPIC128_DST94,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST95",               REGPOKER_READ_DCR, MPIC128_DST95,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST96",               REGPOKER_READ_DCR, MPIC128_DST96,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST97",               REGPOKER_READ_DCR, MPIC128_DST97,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST98",               REGPOKER_READ_DCR, MPIC128_DST98,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST99",               REGPOKER_READ_DCR, MPIC128_DST99,              MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST100",              REGPOKER_READ_DCR, MPIC128_DST100,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST101",              REGPOKER_READ_DCR, MPIC128_DST101,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST102",              REGPOKER_READ_DCR, MPIC128_DST102,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST103",              REGPOKER_READ_DCR, MPIC128_DST103,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST104",              REGPOKER_READ_DCR, MPIC128_DST104,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST105",              REGPOKER_READ_DCR, MPIC128_DST105,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST106",              REGPOKER_READ_DCR, MPIC128_DST106,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST107",              REGPOKER_READ_DCR, MPIC128_DST107,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST108",              REGPOKER_READ_DCR, MPIC128_DST108,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST109",              REGPOKER_READ_DCR, MPIC128_DST109,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST110",              REGPOKER_READ_DCR, MPIC128_DST110,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST111",              REGPOKER_READ_DCR, MPIC128_DST111,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST112",              REGPOKER_READ_DCR, MPIC128_DST112,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST113",              REGPOKER_READ_DCR, MPIC128_DST113,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST114",              REGPOKER_READ_DCR, MPIC128_DST114,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST115",              REGPOKER_READ_DCR, MPIC128_DST115,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST116",              REGPOKER_READ_DCR, MPIC128_DST116,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST117",              REGPOKER_READ_DCR, MPIC128_DST117,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST118",              REGPOKER_READ_DCR, MPIC128_DST118,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST119",              REGPOKER_READ_DCR, MPIC128_DST119,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST120",              REGPOKER_READ_DCR, MPIC128_DST120,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST121",              REGPOKER_READ_DCR, MPIC128_DST121,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST122",              REGPOKER_READ_DCR, MPIC128_DST122,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST123",              REGPOKER_READ_DCR, MPIC128_DST123,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST124",              REGPOKER_READ_DCR, MPIC128_DST124,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST125",              REGPOKER_READ_DCR, MPIC128_DST125,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST126",              REGPOKER_READ_DCR, MPIC128_DST126,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_DST127",              REGPOKER_READ_DCR, MPIC128_DST127,             MPIC128_DST0_VALUE,                MPIC128_DSTx_MSK},
    {"MPIC128_CTP0",                REGPOKER_READ_DCR, MPIC128_CTP0,               0x00000000,                        MPIC128_CTPx_MSK},
    {"MPIC128_CTP1",                REGPOKER_READ_DCR, MPIC128_CTP1,               MPIC128_CTP1_VALUE,                MPIC128_CTPx_MSK},
    {"MPIC128_CTP2",                REGPOKER_READ_DCR, MPIC128_CTP2,               MPIC128_CTP2_VALUE,                MPIC128_CTPx_MSK},
    {"MPIC128_CTP3",                REGPOKER_READ_DCR, MPIC128_CTP3,               MPIC128_CTP3_VALUE,                MPIC128_CTPx_MSK},
    {"MPIC128_WAI0",                REGPOKER_READ_DCR, MPIC128_WAI0,               MPIC128_WAI0_VALUE,                MPIC128_WAIx_MSK},
    {"MPIC128_WAI1",                REGPOKER_READ_DCR, MPIC128_WAI1,               MPIC128_WAI1_VALUE,                MPIC128_WAIx_MSK},
    {"MPIC128_WAI2",                REGPOKER_READ_DCR, MPIC128_WAI2,               MPIC128_WAI2_VALUE,                MPIC128_WAIx_MSK},
    {"MPIC128_WAI3",                REGPOKER_READ_DCR, MPIC128_WAI3,               MPIC128_WAI3_VALUE,                MPIC128_WAIx_MSK},
    {"MPIC128_NCIAR0",              REGPOKER_READ_DCR, MPIC128_NCIAR0,             MPIC128_NCIAR0_VALUE,              MPIC128_NCIARx_MSK},
    {"MPIC128_NCIAR1",              REGPOKER_READ_DCR, MPIC128_NCIAR1,             MPIC128_NCIAR1_VALUE,              MPIC128_NCIARx_MSK},
    {"MPIC128_NCIAR2",              REGPOKER_READ_DCR, MPIC128_NCIAR2,             MPIC128_NCIAR2_VALUE,              MPIC128_NCIARx_MSK},
    {"MPIC128_NCIAR3",              REGPOKER_READ_DCR, MPIC128_NCIAR3,             MPIC128_NCIAR3_VALUE,              MPIC128_NCIARx_MSK},
    {"MPIC128_NCEOI0",              REGPOKER_READ_DCR, MPIC128_NCEOI0,             MPIC128_NCEOI0_VALUE,              MPIC128_NCEOIx_MSK},
    {"MPIC128_NCEOI1",              REGPOKER_READ_DCR, MPIC128_NCEOI1,             MPIC128_NCEOI1_VALUE,              MPIC128_NCEOIx_MSK},
    {"MPIC128_NCEOI2",              REGPOKER_READ_DCR, MPIC128_NCEOI2,             MPIC128_NCEOI2_VALUE,              MPIC128_NCEOIx_MSK},
    {"MPIC128_NCEOI3",              REGPOKER_READ_DCR, MPIC128_NCEOI3,             MPIC128_NCEOI3_VALUE,              MPIC128_NCEOIx_MSK},
    {"MPIC128_CIAR0",               REGPOKER_READ_DCR, MPIC128_CIAR0,              MPIC128_CIAR0_VALUE,               MPIC128_CIARx_MSK},
    {"MPIC128_CIAR1",               REGPOKER_READ_DCR, MPIC128_CIAR1,              MPIC128_CIAR1_VALUE,               MPIC128_CIARx_MSK},
    {"MPIC128_CIAR2",               REGPOKER_READ_DCR, MPIC128_CIAR2,              MPIC128_CIAR2_VALUE,               MPIC128_CIARx_MSK},
    {"MPIC128_CIAR3",               REGPOKER_READ_DCR, MPIC128_CIAR3,              MPIC128_CIAR3_VALUE,               MPIC128_CIARx_MSK},
    {"MPIC128_CEOI0",               REGPOKER_READ_DCR, MPIC128_CEOI0,              MPIC128_CEOI0_VALUE,               MPIC128_CEOIx_MSK},
    {"MPIC128_CEOI1",               REGPOKER_READ_DCR, MPIC128_CEOI1,              MPIC128_CEOI1_VALUE,               MPIC128_CEOIx_MSK},
    {"MPIC128_CEOI2",               REGPOKER_READ_DCR, MPIC128_CEOI2,              MPIC128_CEOI2_VALUE,               MPIC128_CEOIx_MSK},
    {"MPIC128_CEOI3",               REGPOKER_READ_DCR, MPIC128_CEOI3,              MPIC128_CEOI3_VALUE,               MPIC128_CEOIx_MSK},
    {"MPIC128_IAR0",                REGPOKER_READ_DCR, MPIC128_IAR0,               MPIC128_IAR0_VALUE,                MPIC128_IARx_MSK},
    {"MPIC128_IAR1",                REGPOKER_READ_DCR, MPIC128_IAR1,               MPIC128_IAR1_VALUE,                MPIC128_IARx_MSK},
    {"MPIC128_IAR2",                REGPOKER_READ_DCR, MPIC128_IAR2,               MPIC128_IAR2_VALUE,                MPIC128_IARx_MSK},
    {"MPIC128_IAR3",                REGPOKER_READ_DCR, MPIC128_IAR3,               MPIC128_IAR3_VALUE,                MPIC128_IARx_MSK},
    {"MPIC128_EOI0",                REGPOKER_READ_DCR, MPIC128_EOI0,               MPIC128_EOI0_VALUE,                MPIC128_EOIx_MSK},
    {"MPIC128_EOI1",                REGPOKER_READ_DCR, MPIC128_EOI1,               MPIC128_EOI1_VALUE,                MPIC128_EOIx_MSK},
    {"MPIC128_EOI2",                REGPOKER_READ_DCR, MPIC128_EOI2,               MPIC128_EOI2_VALUE,                MPIC128_EOIx_MSK},
    {"MPIC128_EOI3",                REGPOKER_READ_DCR, MPIC128_EOI3,               MPIC128_EOI3_VALUE,                MPIC128_EOIx_MSK},
    {"MPIC128_FRG0",                REGPOKER_READ_DCR, MPIC128_FRG0,               MPIC128_FRG0_VALUE,                MPIC128_FRG0_MSK},
    {"MPIC128_GCF0",                REGPOKER_READ_DCR, MPIC128_GCF0,               0x2000000f,                        MPIC128_GCF0_MSK},
    {"MPIC128_VITC",                REGPOKER_READ_DCR, MPIC128_VITC,               MPIC128_VITC_VALUE,                MPIC128_VITC_MSK},
    {"MPIC128_VRID",                REGPOKER_READ_DCR, MPIC128_VRID,               MPIC128_VRID_VALUE,                MPIC128_VRID_MSK},
    {"MPIC128_REVID",               REGPOKER_READ_DCR, MPIC128_REVID,              MPIC128_REVID_VALUE,               MPIC128_REVID_MSK},
    {"MPIC128_VIR",                 REGPOKER_READ_DCR, MPIC128_VIR0,               MPIC128_VIR0_VALUE,                MPIC128_VIR0_MSK},
    {"MPIC128_PINI",                REGPOKER_READ_DCR, MPIC128_PINI,               MPIC128_PINI_VALUE,                MPIC128_PINI_MSK},
    {"MPIC128_IVP0",                REGPOKER_READ_DCR, MPIC128_IVP0,               MPIC128_IVP0_VALUE,                MPIC128_IVPx_MSK},
    {"MPIC128_IVP1",                REGPOKER_READ_DCR, MPIC128_IVP1,               MPIC128_IVP1_VALUE,                MPIC128_IVPx_MSK},
    {"MPIC128_IVP2",                REGPOKER_READ_DCR, MPIC128_IVP2,               MPIC128_IVP2_VALUE,                MPIC128_IVPx_MSK},
    {"MPIC128_IVP3",                REGPOKER_READ_DCR, MPIC128_IVP3,               MPIC128_IVP3_VALUE,                MPIC128_IVPx_MSK},
    {"MPIC128_SPV",                 REGPOKER_READ_DCR, MPIC128_SPV,                MPIC128_SPV_VALUE,                 MPIC128_SPV_MSK},
    {"MPIC128_TFR",                 REGPOKER_READ_DCR, MPIC128_TFR,                MPIC128_TFR_VALUE,                 MPIC128_TFR_MSK},
    {"MPIC128_TCC0",                REGPOKER_READ_DCR, MPIC128_TCC0,               MPIC128_TCC0_VALUE,                MPIC128_TCCx_MSK},
    {"MPIC128_TCC1",                REGPOKER_READ_DCR, MPIC128_TCC1,               MPIC128_TCC1_VALUE,                MPIC128_TCCx_MSK},
    {"MPIC128_TCC2",                REGPOKER_READ_DCR, MPIC128_TCC2,               MPIC128_TCC2_VALUE,                MPIC128_TCCx_MSK},
    {"MPIC128_TCC3",                REGPOKER_READ_DCR, MPIC128_TCC3,               MPIC128_TCC3_VALUE,                MPIC128_TCCx_MSK},
    {"MPIC128_TBC0",                REGPOKER_READ_DCR, MPIC128_TBC0,               MPIC128_TBC0_VALUE,                MPIC128_TBCx_MSK},
    {"MPIC128_TBC1",                REGPOKER_READ_DCR, MPIC128_TBC1,               MPIC128_TBC1_VALUE,                MPIC128_TBCx_MSK},
    {"MPIC128_TBC2",                REGPOKER_READ_DCR, MPIC128_TBC2,               MPIC128_TBC2_VALUE,                MPIC128_TBCx_MSK},
    {"MPIC128_TBC3",                REGPOKER_READ_DCR, MPIC128_TBC3,               MPIC128_TBC3_VALUE,                MPIC128_TBCx_MSK},
    {"MPIC128_TVP0",                REGPOKER_READ_DCR, MPIC128_TVP0,               MPIC128_TVP0_VALUE,                MPIC128_TVPx_MSK},
    {"MPIC128_TVP1",                REGPOKER_READ_DCR, MPIC128_TVP1,               MPIC128_TVP1_VALUE,                MPIC128_TVPx_MSK},
    {"MPIC128_TVP2",                REGPOKER_READ_DCR, MPIC128_TVP2,               MPIC128_TVP2_VALUE,                MPIC128_TVPx_MSK},
    {"MPIC128_TVP3",                REGPOKER_READ_DCR, MPIC128_TVP3,               MPIC128_TVP3_VALUE,                MPIC128_TVPx_MSK},
    {"MPIC128_TDST0",               REGPOKER_READ_DCR, MPIC128_TDST0,              MPIC128_TDST0_VALUE,               MPIC128_TDSTx_MSK},
    {"MPIC128_TDST1",               REGPOKER_READ_DCR, MPIC128_TDST1,              MPIC128_TDST1_VALUE,               MPIC128_TDSTx_MSK},
    {"MPIC128_TDST2",               REGPOKER_READ_DCR, MPIC128_TDST2,              MPIC128_TDST2_VALUE,               MPIC128_TDSTx_MSK},
    {"MPIC128_TDST3",               REGPOKER_READ_DCR, MPIC128_TDST3,              MPIC128_TDST3_VALUE,               MPIC128_TDSTx_MSK},    
   
    
    {/* Sentinel */}}; 
    
#include <platform/regs/regs_dcrarb.h>

#define DCRARB_TADDR_MSK  0xffffffff
#define DCRARB_MADDR_MSK  0xffffffff
#define DCRARB_MASK_MSK   0xffffffff
#define DCRARB_DACR_MSK   0xc30003f0
#define DCRARB_DAESR_MSK  0xefc0000f
#define DCRARB_DAEAR_MSK  0xffffffff
#define DCRARB_DASMR_MSK  0x8000000f
#define DCRARB_REVID_MSK  0x000fffff


static const struct regpoker_checker dcrarb_regs_default[] = {
    
    //name                       check type         offset               exp val                     mask  
    //uni-directional
    {"DCRARB_TADDR         ",    REGPOKER_READ_DCR, DCRARB_TADDR     ,   DCRARB_TADDR_VALUE     ,       DCRARB_TADDR_MSK     },
    {"DCRARB_MADDR         ",    REGPOKER_READ_DCR, DCRARB_MADDR     ,   DCRARB_MADDR_VALUE     ,       DCRARB_MADDR_MSK     },
    {"DCRARB_MASK          ",    REGPOKER_READ_DCR, DCRARB_MASK      ,   DCRARB_MASK_VALUE      ,       DCRARB_MASK_MSK      },
    {"DCRARB_DACR          ",    REGPOKER_READ_DCR, DCRARB_DACR      ,   DCRARB_DACR_VALUE      ,       DCRARB_DACR_MSK      },
    {"DCRARB_DAESR         ",    REGPOKER_READ_DCR, DCRARB_DAESR     ,   DCRARB_DAESR_VALUE     ,       DCRARB_DAESR_MSK     },
    {"DCRARB_DAEAR         ",    REGPOKER_READ_DCR, DCRARB_DAEAR     ,   DCRARB_DAEAR_VALUE     ,       DCRARB_DAEAR_MSK     },
    //{"DCRARB_DASMR         ",    REGPOKER_READ_DCR, DCRARB_DASMR     ,   DCRARB_DASMR_VALUE     ,       DCRARB_DASMR_MSK     },
    {"DCRARB_REVID         ",    REGPOKER_READ_DCR, DCRARB_REVID     ,   DCRARB_REVID_VALUE     ,       DCRARB_REVID_MSK     },
    
    {/* Sentinel */}};  
    
#include <platform/regs/regs_itrace.h>

#define ITC0_TLA1_MSK 0x00000000
#define ITC0_TLA2_MSK 0x00000000
#define ITC0_WCNT_MSK 0x00000000
#define ITC0_CTO_MSK  0x00000000
#define ITC0_C1ML_MSK 0x00000000
#define ITC0_C2ML_MSK 0x00000000




static const struct regpoker_checker itrace_regs_default[] = {
    
    //name                       check type         offset               exp val                     mask  
    {"ITC0_TC           ",    REGPOKER_READ_DCR, ITC0_TC       ,   ITC0_TC_DEFAULT       ,       ITC0_TC_MSK       },
    {"ITC0_TS           ",    REGPOKER_READ_DCR, ITC0_TS       ,   ITC0_TS_DEFAULT       ,       ITC0_TS_MSK       },
    //{"ITC0_TLA1         ",    REGPOKER_READ_DCR, ITC0_TLA1     ,   ITC0_TLA1_DEFAULT     ,       ITC0_TLA1_MSK     },
    //{"ITC0_TLA2         ",    REGPOKER_READ_DCR, ITC0_TLA2     ,   ITC0_TLA2_DEFAULT     ,       ITC0_TLA2_MSK     },
    {"ITC0_WCNT         ",    REGPOKER_READ_DCR, ITC0_WCNT     ,   ITC0_WCNT_DEFAULT     ,       ITC0_WCNT_MSK     },
    {"ITC0_CTO          ",    REGPOKER_READ_DCR, ITC0_CTO      ,   ITC0_CTO_DEFAULT      ,       ITC0_CTO_MSK      },
    {"ITC0_C1MH         ",    REGPOKER_READ_DCR, ITC0_C1MH     ,   ITC0_C1MH_DEFAULT     ,       ITC0_C1MH_MSK     },
    //{"ITC0_C1ML         ",    REGPOKER_READ_DCR, ITC0_C1ML     ,   ITC0_C1ML_DEFAULT     ,       ITC0_C1ML_MSK     },
    {"ITC0_C2MH         ",    REGPOKER_READ_DCR, ITC0_C2MH     ,   ITC0_C2MH_DEFAULT     ,       ITC0_C2MH_MSK     },
    //{"ITC0_C2ML         ",    REGPOKER_READ_DCR, ITC0_C2ML     ,   ITC0_C2ML_DEFAULT     ,       ITC0_C2ML_MSK     },
    //{"ITC0_ITAI         ",    REGPOKER_READ_DCR, ITC0_ITAI     ,   ITC0_ITAI_DEFAULT     ,       ITC0_ITAI_MSK     },
    //{"ITC0_ITAD         ",    REGPOKER_READ_DCR, ITC0_ITAD     ,   ITC0_ITAD_DEFAULT     ,       ITC0_ITAD_MSK     },
    
    
    {/* Sentinel */}};

#include <platform/regs/regs_ltrace.h>


#define LTR_WC_MSK     0xffffffff
#define LTR_CT_MSK     0xffffffff
#define LTR_CC1PH_MSK  0xffffffff
#define LTR_CC1PL_MSK  0xffffffff
#define LTR_CC1MH_MSK  0xffffffff
#define LTR_CC1ML_MSK  0xffffffff
#define LTR_CC2PH_MSK  0xffffffff
#define LTR_CC2PL_MSK  0xffffffff
#define LTR_CC2MH_MSK  0xffffffff
#define LTR_CC2ML_MSK  0xffffffff
#define LTR_CC3PH_MSK  0xffffffff
#define LTR_CC3PL_MSK  0xffffffff
#define LTR_CC3MH_MSK  0xffffffff
#define LTR_CC3ML_MSK  0xffffffff
#define LTR_CMH_MSK    0xffffffff
#define LTR_CML_MSK    0xffffffff


static const struct regpoker_checker ltrace_regs_default[] = {
    
    //name                    check type         offset            exp val                     mask  
    {"LTR_TC            ",    REGPOKER_READ_DCR, LTR_TC        ,   LTR_TC_DEFAULT      ,       LTR_TC_MSK      },
    {"LTR_TS            ",    REGPOKER_READ_DCR, LTR_TS        ,   LTR_TS_DEFAULT      ,       LTR_TS_MSK      },
    //{"LTR_LA1           ",    REGPOKER_READ_DCR, LTR_LA1       ,   LTR_LA1_DEFAULT     ,       LTR_LA1_MSK     },
    //{"LTR_LA2           ",    REGPOKER_READ_DCR, LTR_LA2       ,   LTR_LA2_DEFAULT     ,       LTR_LA2_MSK     },
    {"LTR_WC            ",    REGPOKER_READ_DCR, LTR_WC        ,   LTR_WC_DEFAULT      ,       LTR_WC_MSK      },  
    {"LTR_CT            ",    REGPOKER_READ_DCR, LTR_CT        ,   LTR_CT_DEFAULT      ,       LTR_CT_MSK      },  
    {"LTR_CC1PH         ",    REGPOKER_READ_DCR, LTR_CC1PH     ,   LTR_CC1PH_DEFAULT   ,       LTR_CC1PH_MSK   },
    {"LTR_CC1PL         ",    REGPOKER_READ_DCR, LTR_CC1PL     ,   LTR_CC1PL_DEFAULT   ,       LTR_CC1PL_MSK   },
    {"LTR_CC1MH         ",    REGPOKER_READ_DCR, LTR_CC1MH     ,   LTR_CC1MH_DEFAULT   ,       LTR_CC1MH_MSK   },
    {"LTR_CC1ML         ",    REGPOKER_READ_DCR, LTR_CC1ML     ,   LTR_CC1ML_DEFAULT   ,       LTR_CC1ML_MSK   },
    {"LTR_CC2PH         ",    REGPOKER_READ_DCR, LTR_CC2PH     ,   LTR_CC2PH_DEFAULT   ,       LTR_CC2PH_MSK   },
    {"LTR_CC2PL         ",    REGPOKER_READ_DCR, LTR_CC2PL     ,   LTR_CC2PL_DEFAULT   ,       LTR_CC2PL_MSK   },
    {"LTR_CC2MH         ",    REGPOKER_READ_DCR, LTR_CC2MH     ,   LTR_CC2MH_DEFAULT   ,       LTR_CC2MH_MSK   },
    {"LTR_CC2ML         ",    REGPOKER_READ_DCR, LTR_CC2ML     ,   LTR_CC2ML_DEFAULT   ,       LTR_CC2ML_MSK   },
    {"LTR_CC3PH         ",    REGPOKER_READ_DCR, LTR_CC3PH     ,   LTR_CC3PH_DEFAULT   ,       LTR_CC3PH_MSK   },
    {"LTR_CC3PL         ",    REGPOKER_READ_DCR, LTR_CC3PL     ,   LTR_CC3PL_DEFAULT   ,       LTR_CC3PL_MSK   },
    {"LTR_CC3MH         ",    REGPOKER_READ_DCR, LTR_CC3MH     ,   LTR_CC3MH_DEFAULT   ,       LTR_CC3MH_MSK   },
    {"LTR_CC3ML         ",    REGPOKER_READ_DCR, LTR_CC3ML     ,   LTR_CC3ML_DEFAULT   ,       LTR_CC3ML_MSK   },
    {"LTR_CMH           ",    REGPOKER_READ_DCR, LTR_CMH       ,   LTR_CMH_DEFAULT     ,       LTR_CMH_MSK     },
    {"LTR_CML           ",    REGPOKER_READ_DCR, LTR_CML       ,   LTR_CML_DEFAULT     ,       LTR_CML_MSK     },
    //{"LTR_AI            ",    REGPOKER_READ_DCR, LTR_AI        ,   LTR_AI_DEFAULT      ,       LTR_AI_MSK      },
    //{"LTR_AD            ",    REGPOKER_READ_DCR, LTR_AD        ,   LTR_AD_DEFAULT      ,       LTR_AD_MSK      },
    
    
    {/* Sentinel */}};
    
#include <arch/regs/plb6bc.h>


static const struct regpoker_checker plb6bc_regs_default[] = {
    
    //name                   check type         offset               exp val                     mask  
    //{"PLB6BC_CR0           ",           REGPOKER_READ_DCR, PLB6BC_CR0           ,          PLB6BC_CR0_DFLT,            PLB6BC_CR0_MSK            },
    {"PLB6BC_PRI           ",           REGPOKER_READ_DCR, PLB6BC_PRI           ,          PLB6BC_PRI_DFLT,            PLB6BC_PRI_MSK            },
    //{"PLB6BC_TSNOOP        ",           REGPOKER_READ_DCR, PLB6BC_TSNOOP        ,          PLB6BC_TSNOOP_DFLT,         PLB6BC_TSNOOP_MSK         }, Not available
    {"PLB6BC_PAAM_WIN_EXT  ",           REGPOKER_READ_DCR, PLB6BC_PAAM_WIN_EXT  ,          PLB6BC_PAAM_WIN_EXT_DFLT,   PLB6BC_PAAM_WIN_EXT_MSK   },
    {"PLB6BC_SGD1          ",           REGPOKER_READ_DCR, PLB6BC_SGD1          ,          0x00000010,                 PLB6BC_SGDx_MSK           },
    {"PLB6BC_SGD2          ",           REGPOKER_READ_DCR, PLB6BC_SGD2          ,          0x00000010,                 PLB6BC_SGDx_MSK           },
    {"PLB6BC_SGD3          ",           REGPOKER_READ_DCR, PLB6BC_SGD3          ,          0x00000020,                 PLB6BC_SGDx_MSK           },
    {"PLB6BC_SGD4          ",           REGPOKER_READ_DCR, PLB6BC_SGD4          ,          PLB6BC_SGD4_DFLT,           PLB6BC_SGDx_MSK           },
    {"PLB6BC_SGD5          ",           REGPOKER_READ_DCR, PLB6BC_SGD5          ,          PLB6BC_SGD5_DFLT,           PLB6BC_SGDx_MSK           },
    {"PLB6BC_SGD6          ",           REGPOKER_READ_DCR, PLB6BC_SGD6          ,          PLB6BC_SGD6_DFLT,           PLB6BC_SGDx_MSK           },
    {"PLB6BC_SGD7          ",           REGPOKER_READ_DCR, PLB6BC_SGD7          ,          PLB6BC_SGD7_DFLT,           PLB6BC_SGDx_MSK           },
    {"PLB6BC_ERR           ",           REGPOKER_READ_DCR, PLB6BC_ERR           ,          0x800d8000,            PLB6BC_ERR_MSK            }, // pcb version
    //{"PLB6BC_ERR           ",           REGPOKER_READ_DCR, PLB6BC_ERR           ,          PLB6BC_ERR_DFLT,            PLB6BC_ERR_MSK            }, //rtl version
    {"PLB6BC_MSRSP         ",           REGPOKER_READ_DCR, PLB6BC_MSRSP         ,          PLB6BC_MSRSP_DFLT,          PLB6BC_MSRSP_MSK          },
    {"PLB6BC_HCPP          ",           REGPOKER_READ_DCR, PLB6BC_HCPP          ,          0x00001000,                 PLB6BC_HCPP_MSK           },
    {"PLB6BC_HD            ",           REGPOKER_READ_DCR, PLB6BC_HD            ,          PLB6BC_HD_DFLT,             PLB6BC_HD_MSK             },
    {"PLB6BC_QD            ",           REGPOKER_READ_DCR, PLB6BC_QD            ,          PLB6BC_QD_DFLT,             PLB6BC_QD_MSK             },
    {"PLB6BC_SHD           ",           REGPOKER_READ_DCR, PLB6BC_SHD           ,          PLB6BC_SHD_DFLT,            PLB6BC_SHD_MSK            },
    {"PLB6BC_REVID         ",           REGPOKER_READ_DCR, PLB6BC_REVID         ,          0x00000102,                 PLB6BC_REVID_MSK          },
   
    
    {/* Sentinel */}};  
    
#include <platform/regs/regs_dma2plb6.h>

#define PLB6_DMA_DSTRx_MSK  0xffffffff
#define PLB6_DMA_SAHx_MSK   0xffffffff
#define PLB6_DMA_SALx_MSK   0xffffffff
#define PLB6_DMA_DAHx_MSK   0xffffffff
#define PLB6_DMA_DALx_MSK   0xffffffff
#define PLB6_DMA_SGHx_MSK   0xffffffff
#define PLB6_DMA_SGLx_MSK   0xffffffff
#define PLB6_DMA_PMEAx_MSK  0xffffffff
#define PLB6_DMA_PSEAx_MSK  0xffffffff

static const struct regpoker_checker dma2plb6_regs_default[] = {
    
    //name                     check type         offset                   exp val                     mask  
    {"CR0    ",                REGPOKER_READ_DCR, PLB6_DMA_CR0    ,        PLB6_DMA_CR0_DEFAULT,       PLB6_DMA_CR0_MSK},
    {"CTC0   ",                REGPOKER_READ_DCR, PLB6_DMA_CTC0   ,        PLB6_DMA_CTC0_DEFAULT,      PLB6_DMA_CTC0_MSK},
    {"DSTR0  ",                REGPOKER_READ_DCR, PLB6_DMA_DSTR0  ,        PLB6_DMA_DSTR0_DEFAULT,     PLB6_DMA_DSTRx_MSK},
    {"SAH0   ",                REGPOKER_READ_DCR, PLB6_DMA_SAH0   ,        PLB6_DMA_SAH0_DEFAULT,      PLB6_DMA_SAHx_MSK},
    {"SAL0   ",                REGPOKER_READ_DCR, PLB6_DMA_SAL0   ,        PLB6_DMA_SAL0_DEFAULT,      PLB6_DMA_SALx_MSK},
    {"DAH0   ",                REGPOKER_READ_DCR, PLB6_DMA_DAH0   ,        PLB6_DMA_DAH0_DEFAULT,      PLB6_DMA_DAHx_MSK},
    {"DAL0   ",                REGPOKER_READ_DCR, PLB6_DMA_DAL0   ,        PLB6_DMA_DAL0_DEFAULT,      PLB6_DMA_DALx_MSK},
    {"SGH0   ",                REGPOKER_READ_DCR, PLB6_DMA_SGH0   ,        PLB6_DMA_SGH0_DEFAULT,      PLB6_DMA_SGHx_MSK},
    {"SGL0   ",                REGPOKER_READ_DCR, PLB6_DMA_SGL0   ,        PLB6_DMA_SGL0_DEFAULT,      PLB6_DMA_SGLx_MSK},
    {"SGC0   ",                REGPOKER_READ_DCR, PLB6_DMA_SGC0   ,        PLB6_DMA_SGC0_DEFAULT,      PLB6_DMA_SGC0_MSK},
    {"CHRES0 ",                REGPOKER_READ_DCR, PLB6_DMA_CHRES0 ,        PLB6_DMA_CHRES0_DEFAULT,    PLB6_DMA_CHRES0_MSK},
    {"CHLOCK0",                REGPOKER_READ_DCR, PLB6_DMA_CHLOCK0,        PLB6_DMA_CHLOCK0_DEFAULT,   PLB6_DMA_CHLOCK0_MSK},
    {"SGCA0  ",                REGPOKER_READ_DCR, PLB6_DMA_SGCA0  ,        PLB6_DMA_SGCA0_DEFAULT,     PLB6_DMA_SGCA0_MSK},
    {"CR1    ",                REGPOKER_READ_DCR, PLB6_DMA_CR1    ,        PLB6_DMA_CR1_DEFAULT,       PLB6_DMA_CR1_MSK},
    {"CTC1   ",                REGPOKER_READ_DCR, PLB6_DMA_CTC1   ,        PLB6_DMA_CTC1_DEFAULT,      PLB6_DMA_CTC1_MSK},
    {"DSTR1  ",                REGPOKER_READ_DCR, PLB6_DMA_DSTR1  ,        PLB6_DMA_DSTR1_DEFAULT,     PLB6_DMA_DSTRx_MSK},
    {"SAH1   ",                REGPOKER_READ_DCR, PLB6_DMA_SAH1   ,        PLB6_DMA_SAH1_DEFAULT,      PLB6_DMA_SAHx_MSK},
    {"SAL1   ",                REGPOKER_READ_DCR, PLB6_DMA_SAL1   ,        PLB6_DMA_SAL1_DEFAULT,      PLB6_DMA_SALx_MSK},
    {"DAH1   ",                REGPOKER_READ_DCR, PLB6_DMA_DAH1   ,        PLB6_DMA_DAH1_DEFAULT,      PLB6_DMA_DAHx_MSK},
    {"DAL1   ",                REGPOKER_READ_DCR, PLB6_DMA_DAL1   ,        PLB6_DMA_DAL1_DEFAULT,      PLB6_DMA_DALx_MSK},
    {"SGH1   ",                REGPOKER_READ_DCR, PLB6_DMA_SGH1   ,        PLB6_DMA_SGH1_DEFAULT,      PLB6_DMA_SGHx_MSK},
    {"SGL1   ",                REGPOKER_READ_DCR, PLB6_DMA_SGL1   ,        PLB6_DMA_SGL1_DEFAULT,      PLB6_DMA_SGLx_MSK},
    {"SGC1   ",                REGPOKER_READ_DCR, PLB6_DMA_SGC1   ,        PLB6_DMA_SGC1_DEFAULT,      PLB6_DMA_SGC1_MSK},
    {"CHRES1 ",                REGPOKER_READ_DCR, PLB6_DMA_CHRES1 ,        PLB6_DMA_CHRES1_DEFAULT,    PLB6_DMA_CHRES1_MSK},
    {"CHLOCK1",                REGPOKER_READ_DCR, PLB6_DMA_CHLOCK1,        PLB6_DMA_CHLOCK1_DEFAULT,   PLB6_DMA_CHLOCK1_MSK},
    {"SGCA1  ",                REGPOKER_READ_DCR, PLB6_DMA_SGCA1  ,        PLB6_DMA_SGCA1_DEFAULT,     PLB6_DMA_SGCA1_MSK},
    {"CR2    ",                REGPOKER_READ_DCR, PLB6_DMA_CR2    ,        PLB6_DMA_CR2_DEFAULT,       PLB6_DMA_CR2_MSK},
    {"CTC2   ",                REGPOKER_READ_DCR, PLB6_DMA_CTC2   ,        PLB6_DMA_CTC2_DEFAULT,      PLB6_DMA_CTC2_MSK},
    {"DSTR2  ",                REGPOKER_READ_DCR, PLB6_DMA_DSTR2  ,        PLB6_DMA_DSTR2_DEFAULT,     PLB6_DMA_DSTRx_MSK},
    {"SAH2   ",                REGPOKER_READ_DCR, PLB6_DMA_SAH2   ,        PLB6_DMA_SAH2_DEFAULT,      PLB6_DMA_SAHx_MSK},
    {"SAL2   ",                REGPOKER_READ_DCR, PLB6_DMA_SAL2   ,        PLB6_DMA_SAL2_DEFAULT,      PLB6_DMA_SALx_MSK},
    {"DAH2   ",                REGPOKER_READ_DCR, PLB6_DMA_DAH2   ,        PLB6_DMA_DAH2_DEFAULT,      PLB6_DMA_DAHx_MSK},
    {"DAL2   ",                REGPOKER_READ_DCR, PLB6_DMA_DAL2   ,        PLB6_DMA_DAL2_DEFAULT,      PLB6_DMA_DALx_MSK},
    {"SGH2   ",                REGPOKER_READ_DCR, PLB6_DMA_SGH2   ,        PLB6_DMA_SGH2_DEFAULT,      PLB6_DMA_SGHx_MSK},
    {"SGL2   ",                REGPOKER_READ_DCR, PLB6_DMA_SGL2   ,        PLB6_DMA_SGL2_DEFAULT,      PLB6_DMA_SGLx_MSK},
    {"SGC2   ",                REGPOKER_READ_DCR, PLB6_DMA_SGC2   ,        PLB6_DMA_SGC2_DEFAULT,      PLB6_DMA_SGC2_MSK},
    {"CHRES2 ",                REGPOKER_READ_DCR, PLB6_DMA_CHRES2 ,        PLB6_DMA_CHRES2_DEFAULT,    PLB6_DMA_CHRES2_MSK},
    {"CHLOCK2",                REGPOKER_READ_DCR, PLB6_DMA_CHLOCK2,        PLB6_DMA_CHLOCK2_DEFAULT,   PLB6_DMA_CHLOCK2_MSK},
    {"SGCA2  ",                REGPOKER_READ_DCR, PLB6_DMA_SGCA2  ,        PLB6_DMA_SGCA2_DEFAULT,     PLB6_DMA_SGCA2_MSK},
    {"CR3    ",                REGPOKER_READ_DCR, PLB6_DMA_CR3    ,        PLB6_DMA_CR3_DEFAULT,       PLB6_DMA_CR3_MSK},
    {"CTC3   ",                REGPOKER_READ_DCR, PLB6_DMA_CTC3   ,        PLB6_DMA_CTC3_DEFAULT,      PLB6_DMA_CTC3_MSK},
    {"DSTR3  ",                REGPOKER_READ_DCR, PLB6_DMA_DSTR3  ,        PLB6_DMA_DSTR3_DEFAULT,     PLB6_DMA_DSTRx_MSK},
    {"SAH3   ",                REGPOKER_READ_DCR, PLB6_DMA_SAH3   ,        PLB6_DMA_SAH3_DEFAULT,      PLB6_DMA_SAHx_MSK},
    {"SAL3   ",                REGPOKER_READ_DCR, PLB6_DMA_SAL3   ,        PLB6_DMA_SAL3_DEFAULT,      PLB6_DMA_SALx_MSK},
    {"DAH3   ",                REGPOKER_READ_DCR, PLB6_DMA_DAH3   ,        PLB6_DMA_DAH3_DEFAULT,      PLB6_DMA_DAHx_MSK},
    {"DAL3   ",                REGPOKER_READ_DCR, PLB6_DMA_DAL3   ,        PLB6_DMA_DAL3_DEFAULT,      PLB6_DMA_DALx_MSK},
    {"SGH3   ",                REGPOKER_READ_DCR, PLB6_DMA_SGH3   ,        PLB6_DMA_SGH3_DEFAULT,      PLB6_DMA_SGHx_MSK},
    {"SGL3   ",                REGPOKER_READ_DCR, PLB6_DMA_SGL3   ,        PLB6_DMA_SGL3_DEFAULT,      PLB6_DMA_SGLx_MSK},
    {"SGC3   ",                REGPOKER_READ_DCR, PLB6_DMA_SGC3   ,        PLB6_DMA_SGC3_DEFAULT,      PLB6_DMA_SGC3_MSK},
    {"CHRES3 ",                REGPOKER_READ_DCR, PLB6_DMA_CHRES3 ,        PLB6_DMA_CHRES3_DEFAULT,    PLB6_DMA_CHRES3_MSK},
    {"CHLOCK3",                REGPOKER_READ_DCR, PLB6_DMA_CHLOCK3,        PLB6_DMA_CHLOCK3_DEFAULT,   PLB6_DMA_CHLOCK3_MSK},
    {"SGCA3  ",                REGPOKER_READ_DCR, PLB6_DMA_SGCA3  ,        PLB6_DMA_SGCA3_DEFAULT,     PLB6_DMA_SGCA3_MSK},
    {"SR     ",                REGPOKER_READ_DCR, PLB6_DMA_SR     ,        PLB6_DMA_SR_DEFAULT,        PLB6_DMA_SR_MSK},
    {"PMEAH  ",                REGPOKER_READ_DCR, PLB6_DMA_PMEAH  ,        PLB6_DMA_PMEAH_DEFAULT,     PLB6_DMA_PMEAx_MSK},
    {"PMEAL  ",                REGPOKER_READ_DCR, PLB6_DMA_PMEAL  ,        PLB6_DMA_PMEAL_DEFAULT,     PLB6_DMA_PMEAx_MSK},
    {"PSE    ",                REGPOKER_READ_DCR, PLB6_DMA_PSE    ,        PLB6_DMA_PSE_DEFAULT,       PLB6_DMA_PSE_MSK},
    {"PSEAH  ",                REGPOKER_READ_DCR, PLB6_DMA_PSEAH  ,        PLB6_DMA_PSEAH_DEFAULT,     PLB6_DMA_PSEAx_MSK},
    {"PSEAL  ",                REGPOKER_READ_DCR, PLB6_DMA_PSEAL  ,        PLB6_DMA_PSEAL_DEFAULT,     PLB6_DMA_PSEAx_MSK},
    {"CRS    ",                REGPOKER_READ_DCR, PLB6_DMA_CRS    ,        PLB6_DMA_CRS_DEFAULT,       PLB6_DMA_CRS_MSK},
    {"REVID  ",                REGPOKER_READ_DCR, PLB6_DMA_REVID  ,        PLB6_DMA_REVID_DEFAULT,     PLB6_DMA_REVID_MSK},
    {"ARB    ",                REGPOKER_READ_DCR, PLB6_DMA_ARB    ,        PLB6_DMA_ARB_DEFAULT,       PLB6_DMA_ARB_MSK},
    {"OPTIONS",                REGPOKER_READ_DCR, PLB6_DMA_OPTIONS,        PLB6_DMA_OPTIONS_DEFAULT,   PLB6_DMA_OPTIONS_MSK},
    {"PERM   ",                REGPOKER_READ_DCR, PLB6_DMA_PERM   ,        PLB6_DMA_PERM_DEFAULT,      PLB6_DMA_PERM_MSK},
    //{"PNC   ",                 REGPOKER_READ_DCR, PLB6_DMA_PNC   ,         PLB6_DMA_PNC_DEFAULT,       PLB6_DMA_PNC_MSK},
   
    
    {/* Sentinel */}};
    
#include <platform/regs/regs_srammc2plb4.h>

#define SRAMMC2PLB4_0_SB0CR_DFLT 0xffff048f
#define SRAMMC2PLB4_0_SB1CR_DFLT 0xffff448f
#define SRAMMC2PLB4_0_SB2CR_DFLT 0xffff848f
#define SRAMMC2PLB4_0_SB3CR_DFLT 0xffffc48f

#define SRAMMC2PLB4_1_SB0CR_DFLT 0x80000780
#define SRAMMC2PLB4_1_SB1CR_DFLT 0x80008780
#define SRAMMC2PLB4_1_SB2CR_DFLT 0x80010780
#define SRAMMC2PLB4_1_SB3CR_DFLT 0x80018780


 
static const struct regpoker_checker srammc2plb4_0_regs_default[] = {
    //name          check type         offset                  exp val                       mask
    {"SB0CR",       REGPOKER_READ_DCR, SRAMMC2PLB4_SB0CR,      SRAMMC2PLB4_0_SB0CR_DFLT,     SRAMMC2PLB4_SBxCR_MASK   },
    {"SB1CR",       REGPOKER_READ_DCR, SRAMMC2PLB4_SB1CR,      SRAMMC2PLB4_0_SB1CR_DFLT,     SRAMMC2PLB4_SBxCR_MASK   },
    {"SB2CR",       REGPOKER_READ_DCR, SRAMMC2PLB4_SB2CR,      SRAMMC2PLB4_0_SB2CR_DFLT,     SRAMMC2PLB4_SBxCR_MASK   },
    {"SB3CR",       REGPOKER_READ_DCR, SRAMMC2PLB4_SB3CR,      SRAMMC2PLB4_0_SB3CR_DFLT,     SRAMMC2PLB4_SBxCR_MASK   },
    {"BEAR",        REGPOKER_READ_DCR, SRAMMC2PLB4_BEAR,       SRAMMC2PLB4_BEAR_DFLT,        SRAMMC2PLB4_BEAR_MASK    },
    {"BESR0",       REGPOKER_READ_DCR, SRAMMC2PLB4_BESR0,      SRAMMC2PLB4_BESRx_DFLT,       SRAMMC2PLB4_BESR0_MASK   },
    {"BESR1",       REGPOKER_READ_DCR, SRAMMC2PLB4_BESR1,      SRAMMC2PLB4_BESRx_DFLT,       SRAMMC2PLB4_BESR1_MASK   },
    {"PMEG",        REGPOKER_READ_DCR, SRAMMC2PLB4_PMEG,       SRAMMC2PLB4_PMEG_DFLT,        SRAMMC2PLB4_PMEG_MASK    },
    {"CID",         REGPOKER_READ_DCR, SRAMMC2PLB4_CID,        SRAMMC2PLB4_CID_DFLT,         SRAMMC2PLB4_CID_MASK     },
    {"REVID",       REGPOKER_READ_DCR, SRAMMC2PLB4_REVID,      SRAMMC2PLB4_REVID_DFLT,       SRAMMC2PLB4_REVID_MASK   },
    {"DPC",         REGPOKER_READ_DCR, SRAMMC2PLB4_DPC,        SRAMMC2PLB4_DPC_DFLT,         SRAMMC2PLB4_DPC_MASK     },
        
    {/* Sentinel */}}; 
    
static const struct regpoker_checker srammc2plb4_1_regs_default[] = {
    //name         check type         offset     exp val                  mask
    {"SB0CR",       REGPOKER_READ_DCR, SRAMMC2PLB4_SB0CR,      SRAMMC2PLB4_1_SB0CR_DFLT,     SRAMMC2PLB4_SBxCR_MASK   },
    {"SB1CR",       REGPOKER_READ_DCR, SRAMMC2PLB4_SB1CR,      SRAMMC2PLB4_1_SB1CR_DFLT,     SRAMMC2PLB4_SBxCR_MASK   },
    {"SB2CR",       REGPOKER_READ_DCR, SRAMMC2PLB4_SB2CR,      SRAMMC2PLB4_1_SB2CR_DFLT,     SRAMMC2PLB4_SBxCR_MASK   },
    {"SB3CR",       REGPOKER_READ_DCR, SRAMMC2PLB4_SB3CR,      SRAMMC2PLB4_1_SB3CR_DFLT,     SRAMMC2PLB4_SBxCR_MASK   },
    {"BEAR",        REGPOKER_READ_DCR, SRAMMC2PLB4_BEAR,       SRAMMC2PLB4_BEAR_DFLT,        SRAMMC2PLB4_BEAR_MASK    },
    {"BESR0",       REGPOKER_READ_DCR, SRAMMC2PLB4_BESR0,      SRAMMC2PLB4_BESRx_DFLT,       SRAMMC2PLB4_BESR0_MASK   },
    {"BESR1",       REGPOKER_READ_DCR, SRAMMC2PLB4_BESR1,      SRAMMC2PLB4_BESRx_DFLT,       SRAMMC2PLB4_BESR1_MASK   },
    {"PMEG",        REGPOKER_READ_DCR, SRAMMC2PLB4_PMEG,       SRAMMC2PLB4_PMEG_DFLT,        SRAMMC2PLB4_PMEG_MASK    },
    {"CID",         REGPOKER_READ_DCR, SRAMMC2PLB4_CID,        SRAMMC2PLB4_CID_DFLT,         SRAMMC2PLB4_CID_MASK     },
    {"REVID",       REGPOKER_READ_DCR, SRAMMC2PLB4_REVID,      SRAMMC2PLB4_REVID_DFLT,       SRAMMC2PLB4_REVID_MASK   },
    {"DPC",         REGPOKER_READ_DCR, SRAMMC2PLB4_DPC,        SRAMMC2PLB4_DPC_DFLT,         SRAMMC2PLB4_DPC_MASK     },
        
    {/* Sentinel */}}; 

#include <platform/regs/regs_aximcif2.h>

static const struct regpoker_checker aximcif2_regs_default[] = {
    //      name                check type          offset         exp val                  mask
    {"AXIMCIF2_BESR_read",      REGPOKER_READ_DCR, AXIMCIF2_BESR_read,      AXIMCIF2_BESR_DFLT,       AXIMCIF2_BESR_MSK         },
    {"AXIMCIF2_AEARL",          REGPOKER_READ_DCR, AXIMCIF2_AEARL,          AXIMCIF2_AEARL_DFLT,      AXIMCIF2_AEARL_MSK        },
    {"AXIMCIF2_AEARU",          REGPOKER_READ_DCR, AXIMCIF2_AEARU,          AXIMCIF2_AEARU_DFLT,      AXIMCIF2_AEARU_MSK        },
    {"AXIMCIF2_ERRID",          REGPOKER_READ_DCR, AXIMCIF2_ERRID,          AXIMCIF2_ERRID_DFLT,      AXIMCIF2_ERRID_MSK        },
    {"AXIMCIF2_AXIERRMASK",     REGPOKER_READ_DCR, AXIMCIF2_AXIERRMASK,     AXIMCIF2_AXIERRMASK_DFLT, AXIMCIF2_AXIERRMASK_MSK   },
    {"AXIMCIF2_AXIASYNC",       REGPOKER_READ_DCR, AXIMCIF2_AXIASYNC,       AXIMCIF2_AXIASYNC_DFLT,   AXIMCIF2_AXIASYNC_MSK     },
    {"AXIMCIF2_AXICFG",         REGPOKER_READ_DCR, AXIMCIF2_AXICFG,         AXIMCIF2_AXICFG_DFLT,     AXIMCIF2_AXICFG_MSK       },
    {"AXIMCIF2_AXISTS",         REGPOKER_READ_DCR, AXIMCIF2_AXISTS,         AXIMCIF2_AXISTS_DFLT,     AXIMCIF2_AXISTS_MSK       },
    {"AXIMCIF2_AXIMR0CF",       REGPOKER_READ_DCR, AXIMCIF2_MR0CF,          AXIMCIF2_MR0CF_DFLT,      AXIMCIF2_MRxCF_MSK        },
    {"AXIMCIF2_AXIMR1CF",       REGPOKER_READ_DCR, AXIMCIF2_MR1CF,          AXIMCIF2_MR1CF_DFLT,      AXIMCIF2_MRxCF_MSK        },
    {"AXIMCIF2_AXIMR2CF",       REGPOKER_READ_DCR, AXIMCIF2_MR2CF,          AXIMCIF2_MR2CF_DFLT,      AXIMCIF2_MRxCF_MSK        },
    {"AXIMCIF2_AXIMR3CF",       REGPOKER_READ_DCR, AXIMCIF2_MR3CF,          AXIMCIF2_MR3CF_DFLT,      AXIMCIF2_MRxCF_MSK        },
    {"AXIMCIF2_AXIRID",         REGPOKER_READ_DCR, AXIMCIF2_RID,            AXIMCIF2_RID_DFLT,        AXIMCIF2_RID_MSK          },
    
    
    {/* Sentinel */}};    
    
// Processing src/platform/oi10/targets/simple-iram/test_oi10_em2_105.c / test_oi10_em2_105
#include <devices/ugly/emi.h>
#define ROM_IN_NOR_SIZE (0)

struct regpoker_checker em2x_checker[] = {
    {"Data0",  REGPOKER_READ32, 0x0,                               0x00000000, 0xffffffff   },
    {"Data1",  REGPOKER_READ32, 0x4,                               0x10101010, 0xffffffff   },
    {"Data2",  REGPOKER_READ32, 0x8,                               0x11111111, 0xffffffff   },
    {"Data3",  REGPOKER_READ32, 0xC,                               0xBABADEDA, 0xffffffff   },
    {"Data4",  REGPOKER_READ32, 0x10,                              0xDEDABABA, 0xffffffff   },
    {"Data5",  REGPOKER_READ32, 0x14,                              0xAAAAAAAA, 0xffffffff   },
    {"Data6",  REGPOKER_READ32, 0x18,                              0x55555555, 0xffffffff   },
    {"Data7",  REGPOKER_READ32, 0x1C,                              0xCCCCCCCC, 0xffffffff   },
    {"Data8",  REGPOKER_READ32, NOR_SIZE - ROM_IN_NOR_SIZE - 0x20, 0x00000000, 0xffffffff   },
    {"Data9",  REGPOKER_READ32, NOR_SIZE - ROM_IN_NOR_SIZE - 0x1C, 0x10101010, 0xffffffff   },
    {"Data10", REGPOKER_READ32, NOR_SIZE - ROM_IN_NOR_SIZE - 0x18, 0x11111111, 0xffffffff   },
    {"Data11", REGPOKER_READ32, NOR_SIZE - ROM_IN_NOR_SIZE - 0x14, 0xBABADEDA, 0xffffffff   },
    {"Data12", REGPOKER_READ32, NOR_SIZE - ROM_IN_NOR_SIZE - 0x10, 0xDEDABABA, 0xffffffff   },
    {"Data13", REGPOKER_READ32, NOR_SIZE - ROM_IN_NOR_SIZE - 0x0C, 0xAAAAAAAA, 0xffffffff   },
    {"Data14", REGPOKER_READ32, NOR_SIZE - ROM_IN_NOR_SIZE - 0x08, 0x55555555, 0xffffffff   },
    {"Data15", REGPOKER_READ32, NOR_SIZE - ROM_IN_NOR_SIZE - 0x04, 0xCCCCCCCC, 0xffffffff   },
    {/* Sentinel */}};
// Processing src/platform/oi10/targets/simple-iram/test_oi10_em2_101.c / test_oi10_em2_101
#include <arch/regs/plb6mcif2.h>

static const struct regpoker_checker plb6mcif2_regs_default[] = {
    //      name                check type          offset         exp val                  mask
    {"PLB6MCIF2_BEARL",     REGPOKER_READ_DCR, PLB6MCIF2_BEARL,     0x00,                0xffffffff   },
    {"PLB6MCIF2_BEARU",     REGPOKER_READ_DCR, PLB6MCIF2_BEARU,     0x00,                0xffffffff   },
    {"PLB6MCIF2_BESR_read", REGPOKER_READ_DCR, PLB6MCIF2_BESR_read, 0x00,                0xffffffff   },
    {"PLB6MCIF2_INTR_EN",   REGPOKER_READ_DCR, PLB6MCIF2_INTR_EN,   0xFFE001C0,          0xffffffff   },
    {"PLB6MCIF2_MAP0CF",    REGPOKER_READ_DCR, PLB6MCIF2_MAP0CF,    0x00,                0xffffffff   },
    {"PLB6MCIF2_MAP1CF",    REGPOKER_READ_DCR, PLB6MCIF2_MAP1CF,    0x00,                0xffffffff   },
    {"PLB6MCIF2_MAP2CF",    REGPOKER_READ_DCR, PLB6MCIF2_MAP2CF,    0x00,                0xffffffff   },
    {"PLB6MCIF2_MAP3CF",    REGPOKER_READ_DCR, PLB6MCIF2_MAP3CF,    0x00,                0xffffffff   },
    {"PLB6MCIF2_MAXMEM",    REGPOKER_READ_DCR, PLB6MCIF2_MAXMEM,    0xC0000000,          0xffffffff   }, //default: [0:1]= 0b11
    {"PLB6MCIF2_MR0CF",     REGPOKER_READ_DCR, PLB6MCIF2_MR0CF,     0x00009001,          0xffffffff   },
    {"PLB6MCIF2_MR1CF",     REGPOKER_READ_DCR, PLB6MCIF2_MR1CF,     0xf0009001,          0xffffffff   },
    {"PLB6MCIF2_MR2CF",     REGPOKER_READ_DCR, PLB6MCIF2_MR2CF,     0x00,                0xffffffff   },
    {"PLB6MCIF2_MR3CF",     REGPOKER_READ_DCR, PLB6MCIF2_MR3CF,     0x00,                0xffffffff   },
    {"PLB6MCIF2_P6BMTAG1",  REGPOKER_READ_DCR, PLB6MCIF2_P6BMTAG1,  0x00,                0xffffffff   },
    {"PLB6MCIF2_P6BMTAG2",  REGPOKER_READ_DCR, PLB6MCIF2_P6BMTAG2,  0x00,                0xffffffff   },
    {"PLB6MCIF2_P6BMTAG3",  REGPOKER_READ_DCR, PLB6MCIF2_P6BMTAG3,  0x00,                0xffffffff   },
    {"PLB6MCIF2_PLBASYNC",  REGPOKER_READ_DCR, PLB6MCIF2_PLBASYNC,  0x00,                0xffffffff   },
    {"PLB6MCIF2_PLBCFG",    REGPOKER_READ_DCR, PLB6MCIF2_PLBCFG,    0x200000F1,          0xffffffff   },
    {"PLB6MCIF2_PLBORD",    REGPOKER_READ_DCR, PLB6MCIF2_PLBORD,    0x40000000,          0xffffffff   },
    {"PLB6MCIF2_PUABA",     REGPOKER_READ_DCR, PLB6MCIF2_PUABA,     0x00,                0xffffffff   },
    {"PLB6MCIF2_RID",       REGPOKER_READ_DCR, PLB6MCIF2_RID,       PLB6MCIF2_WATERMARK, 0xffffffff   },
    {"PLB6MCIF2_SR0CF",     REGPOKER_READ_DCR, PLB6MCIF2_SR0CF,     0x00,                0xffffffff   },
    {"PLB6MCIF2_SR1CF",     REGPOKER_READ_DCR, PLB6MCIF2_SR1CF,     0x00,                0xffffffff   },
    {"PLB6MCIF2_SR2CF",     REGPOKER_READ_DCR, PLB6MCIF2_SR2CF,     0x00,                0xffffffff   },
    {"PLB6MCIF2_SR3CF",     REGPOKER_READ_DCR, PLB6MCIF2_SR3CF,     0x00,                0xffffffff   },
    {"PLB6MCIF2_STATUS",    REGPOKER_READ_DCR, PLB6MCIF2_STATUS,    0x80000000,          0xffffffff   },
    {/* Sentinel */}};
static const struct regpoker_checker plb6mcif2_regs_write[] = {
    //      name                check type          offset           val            mask
    {"PLB6MCIF2_INTR_EN",  REGPOKER_WRITE_DCR, PLB6MCIF2_INTR_EN,  0xFFE001C0, 0xffffffff               },
    {"PLB6MCIF2_MAP0CF",   REGPOKER_WRITE_DCR, PLB6MCIF2_MAP0CF,   0x00,       0xffffffff               },
    {"PLB6MCIF2_MAP1CF",   REGPOKER_WRITE_DCR, PLB6MCIF2_MAP1CF,   0x00,       0xffffffff               },
    {"PLB6MCIF2_MAP2CF",   REGPOKER_WRITE_DCR, PLB6MCIF2_MAP2CF,   0x00,       0xffffffff               },
    {"PLB6MCIF2_MAP3CF",   REGPOKER_WRITE_DCR, PLB6MCIF2_MAP3CF,   0x00,       0xffffffff               },
    {"PLB6MCIF2_MAXMEM",   REGPOKER_WRITE_DCR, PLB6MCIF2_MAXMEM,   0xC0000000, PLB6MCIF2_MAXMEM_MSK     },
    {"PLB6MCIF2_MR0CF",    REGPOKER_WRITE_DCR, PLB6MCIF2_MR0CF,    0x01,       PLB6MCIF2_MRxCF_MSK      },
    {"PLB6MCIF2_MR1CF",    REGPOKER_WRITE_DCR, PLB6MCIF2_MR1CF,    0x00,       PLB6MCIF2_MRxCF_MSK      },
    {"PLB6MCIF2_MR2CF",    REGPOKER_WRITE_DCR, PLB6MCIF2_MR2CF,    0x00,       PLB6MCIF2_MRxCF_MSK      },
    {"PLB6MCIF2_MR3CF",    REGPOKER_WRITE_DCR, PLB6MCIF2_MR3CF,    0xfff80001, PLB6MCIF2_MRxCF_MSK      },
    {"PLB6MCIF2_P6BMTAG1", REGPOKER_WRITE_DCR, PLB6MCIF2_P6BMTAG1, 0x00,       PLB6MCIF2_P6BMTAG1_MSK   },
    {"PLB6MCIF2_P6BMTAG2", REGPOKER_WRITE_DCR, PLB6MCIF2_P6BMTAG2, 0x00,       PLB6MCIF2_P6BMTAG2_MSK   },
    {"PLB6MCIF2_P6BMTAG3", REGPOKER_WRITE_DCR, PLB6MCIF2_P6BMTAG3, 0x00,       PLB6MCIF2_P6BMTAG3_MSK   },
    {"PLB6MCIF2_PLBASYNC", REGPOKER_WRITE_DCR, PLB6MCIF2_PLBASYNC, 0x00,       PLB6MCIF2_PLBASYNC_MSK   },
    {"PLB6MCIF2_PLBCFG",   REGPOKER_WRITE_DCR, PLB6MCIF2_PLBCFG,   0x200000F1, PLB6MCIF2_PLBCFG_MSK     },
    {"PLB6MCIF2_PLBORD",   REGPOKER_WRITE_DCR, PLB6MCIF2_PLBORD,   0x40000000, PLB6MCIF2_PLBORD_MSK     },
    {"PLB6MCIF2_PUABA",    REGPOKER_WRITE_DCR, PLB6MCIF2_PUABA,    0x00,       0xfffffffe},
    {/* Sentinel */}};

#include <platform/regs/regs_mclfir.h>
static const struct regpoker_checker mclfir_regs_default[] = {
    //      name                       check type          offset                     exp val     mask
    {"MCLFIR_MC_ERR0",               REGPOKER_READ_DCR, MCLFIR_MC_ERR0,               0x00,       0xffffffff   },
    {"MCLFIR_MC_ERR1",               REGPOKER_READ_DCR, MCLFIR_MC_ERR1,               0x00,       0xffffffff   },
    {"MCLFIR_MC_ERR_MSK0",           REGPOKER_READ_DCR, MCLFIR_MC_ERR_MSK0,           0xFFFFFFFF, 0xffffffff   },
    {"MCLFIR_MC_ERR_MSK1",           REGPOKER_READ_DCR, MCLFIR_MC_ERR_MSK1,           0xFFFFFFC0, 0xffffffff   },
    {"MCLFIR_MC_ERR_ACTION0",        REGPOKER_READ_DCR, MCLFIR_MC_ERR_ACTION0,        0x00,       0xffffffff   },
    {"MCLFIR_MC_ERR_ACTION1",        REGPOKER_READ_DCR, MCLFIR_MC_ERR_ACTION1,        0x00,       0xffffffff   },
    {"MCLFIR_MC_ERR_WOF0",           REGPOKER_READ_DCR, MCLFIR_MC_ERR_WOF0,           0x00,       0xffffffff   },
    {"MCLFIR_MC_ERR_WOF1",           REGPOKER_READ_DCR, MCLFIR_MC_ERR_WOF1,           0x00,       0xffffffff   },
    {"MCLFIR_MC_LFIR",               REGPOKER_READ_DCR, MCLFIR_MC_LFIR,               0x00,       0xffffffff   },
    {"MCLFIR_MC_LFIR_MSK",           REGPOKER_READ_DCR, MCLFIR_MC_LFIR_MSK,           0xC0000000, 0xffffffff   },
    {"MCLFIR_MC_CONSOL_STAT",        REGPOKER_READ_DCR, MCLFIR_MC_CONSOL_STAT,        0x00f00000, 0xffffffff   },
    {"MCLFIR_MC_CONSOL_STAT_TRIG",   REGPOKER_READ_DCR, MCLFIR_MC_CONSOL_STAT_TRIG,   0x00,       0xffffffff   },
    {"MCLFIR_MC_CONSOL_STAT_MSK",    REGPOKER_READ_DCR, MCLFIR_MC_CONSOL_STAT_MSK,    0xFFF08000, 0xffffffff   },
    {"MCLFIR_MC_CONSOL_STATSUM",     REGPOKER_READ_DCR, MCLFIR_MC_CONSOL_STATSUM,     0x00,       0xffffffff   },
    {"MCLFIR_MC_CONSOL_STATSUM_MSK", REGPOKER_READ_DCR, MCLFIR_MC_CONSOL_STATSUM_MSK, 0x80000000, 0xffffffff   },
    {}};
static const struct regpoker_checker mclfir_regs_write[] = {
    //      name                check type        offset                 val       mask
    {"MCLFIR_MC_ERR_ACTION0", REGPOKER_WRITE_DCR, MCLFIR_MC_ERR_ACTION0, 0x00, 0xffffffff   },
    {"MCLFIR_MC_ERR_ACTION1", REGPOKER_WRITE_DCR, MCLFIR_MC_ERR_ACTION1, 0x00, 0xffffffc0   },
    {"MCLFIR_MC_ERR_WOF0",    REGPOKER_WRITE_DCR, MCLFIR_MC_ERR_WOF0,    0x00, 0xffffffff   },
    {"MCLFIR_MC_ERR_WOF1",    REGPOKER_WRITE_DCR, MCLFIR_MC_ERR_WOF1,    0x00, 0xffffffc0   },
    {}};

#include <regs/regs_emi.h>
#define EMI_SSx_DFLT 0x00800000
#define EMI_SDx_DFLT 0x00000000
static const struct regpoker_checker emi_regs_default[] = {
    // name             check type     offset        exp val          mask
    /* not stable on pcb
    {"EMI_SS0",     REGPOKER_READ_DCR,     EMI_SS0,     0x800000, 0xffffffff   }, // pcb version
    {"EMI_SS1",     REGPOKER_READ_DCR,     EMI_SS1,     0x800000, 0xffffffff   }, // pcb version
    {"EMI_SS2",     REGPOKER_READ_DCR,     EMI_SS2,     0x800000, 0xffffffff   }, // pcb version
    {"EMI_SS3",     REGPOKER_READ_DCR,     EMI_SS3,     0x800000, 0xffffffff   }, // pcb version
    {"EMI_SS4",     REGPOKER_READ_DCR,     EMI_SS4,     0x800000, 0xffffffff   }, // pcb version
    {"EMI_SS5",     REGPOKER_READ_DCR,     EMI_SS5,     0x800000, 0xffffffff   }, // pcb version
    */
    //{"EMI_SS0",     REGPOKER_READ_DCR,     EMI_SS0,     EMI_SSx_DFLT, 0xffffffff   }, // rtl version
    //{"EMI_SS1",     REGPOKER_READ_DCR,     EMI_SS1,     EMI_SSx_DFLT, 0xffffffff   }, // rtl version
    //{"EMI_SS2",     REGPOKER_READ_DCR,     EMI_SS2,     EMI_SSx_DFLT, 0xffffffff   }, // rtl version
    //{"EMI_SS3",     REGPOKER_READ_DCR,     EMI_SS3,     EMI_SSx_DFLT, 0xffffffff   }, // rtl version
    //{"EMI_SS4",     REGPOKER_READ_DCR,     EMI_SS4,     EMI_SSx_DFLT, 0xffffffff   }, // rtl version
    //{"EMI_SS5",     REGPOKER_READ_DCR,     EMI_SS5,     EMI_SSx_DFLT, 0xffffffff   }, // rtl version
    {"EMI_SD0",     REGPOKER_READ_DCR,     EMI_SD0,     EMI_SDx_DFLT, 0xffffffff   },
    {"EMI_SD1",     REGPOKER_READ_DCR,     EMI_SD1,     EMI_SDx_DFLT, 0xffffffff   },
    {"EMI_SD2",     REGPOKER_READ_DCR,     EMI_SD2,     EMI_SDx_DFLT, 0xffffffff   },
    {"EMI_SD3",     REGPOKER_READ_DCR,     EMI_SD3,     EMI_SDx_DFLT, 0xffffffff   },
    {"EMI_SD4",     REGPOKER_READ_DCR,     EMI_SD4,     EMI_SDx_DFLT, 0xffffffff   },
    {"EMI_SD5",     REGPOKER_READ_DCR,     EMI_SD5,     EMI_SDx_DFLT, 0xffffffff   },
    {"EMI_RFC",     REGPOKER_READ_DCR,     EMI_RFC,     0x00,         0xffffffff   },
    {"EMI_HSTSR",   REGPOKER_READ_DCR,     EMI_HSTSR,   0x00,         0xffffffff   },
    {"EMI_ECNT20",  REGPOKER_READ_DCR,     EMI_ECNT20,  0x00,         0xffffffff   },
    {"EMI_ECNT53",  REGPOKER_READ_DCR,     EMI_ECNT53,  0x00,         0xffffffff   },
    {"EMI_BUSEN",   REGPOKER_READ_DCR,     EMI_BUSEN,   0x00,         0xffffffff   },
    {"EMI_WECR",    REGPOKER_READ_DCR,     EMI_WECR,    0x00,         0xffffffff   },
    {"EMI_FLCNTRL", REGPOKER_READ_DCR,     EMI_FLCNTRL, 0x1C,         0xffffffff   },
    {"EMI_IMR",     REGPOKER_READ_DCR,     EMI_IMR,     0x00,         0xffffffff   },
    {"EMI_IMR_SET", REGPOKER_READ_DCR,     EMI_IMR_SET, 0x00,         0xffffffff   },
    {"EMI_IMR_RST", REGPOKER_READ_DCR,     EMI_IMR_RST, 0x00,         0xffffffff   },
    {"EMI_IRR",     REGPOKER_READ_DCR,     EMI_IRR,     0x00,         0xffffffff   },
    {"EMI_IRR_RST", REGPOKER_READ_DCR,     EMI_IRR_RST, 0x00,         0xffffffff   },
    {/* Sentinel */}};
static const struct regpoker_checker emi_regs_write[] = {
    // name             check type       offset         val           mask
    {"EMI_SS0",     REGPOKER_WRITE_DCR, EMI_SS0,     EMI_SSx_DFLT, 0x001fffff   },
    {"EMI_SD0",     REGPOKER_WRITE_DCR, EMI_SD0,     EMI_SDx_DFLT, 0x0000fffe   },
    {"EMI_SS1",     REGPOKER_WRITE_DCR, EMI_SS1,     EMI_SSx_DFLT, 0x001fffff   },
    {"EMI_SD1",     REGPOKER_WRITE_DCR, EMI_SD1,     EMI_SDx_DFLT, 0x0000fffe   },
    {"EMI_SS2",     REGPOKER_WRITE_DCR, EMI_SS2,     EMI_SSx_DFLT, 0x001fffff   },
    {"EMI_SD2",     REGPOKER_WRITE_DCR, EMI_SD2,     EMI_SDx_DFLT, 0x0000fffe   },
    {"EMI_SS3",     REGPOKER_WRITE_DCR, EMI_SS3,     EMI_SSx_DFLT, 0x001fffff   },
    {"EMI_SD3",     REGPOKER_WRITE_DCR, EMI_SD3,     EMI_SDx_DFLT, 0x0000fffe   },
    {"EMI_SS4",     REGPOKER_WRITE_DCR, EMI_SS4,     EMI_SSx_DFLT, 0x001fffff   },
    {"EMI_SD4",     REGPOKER_WRITE_DCR, EMI_SD4,     EMI_SDx_DFLT, 0x0000fffe   },
    {"EMI_SS5",     REGPOKER_WRITE_DCR, EMI_SS5,     EMI_SSx_DFLT, 0x001fffff   },
    {"EMI_SD5",     REGPOKER_WRITE_DCR, EMI_SD5,     EMI_SDx_DFLT, 0x0000fffe   },
    {"EMI_RFC",     REGPOKER_WRITE_DCR, EMI_RFC,     0x00,         0x0003fffe   },
    {"EMI_HSTSR",   REGPOKER_WRITE_DCR, EMI_HSTSR,   0x00,         0x0000003f   },
    {"EMI_ECNT20",  REGPOKER_WRITE_DCR, EMI_ECNT20,  0x00,         0x00ffffff   },
    {"EMI_ECNT53",  REGPOKER_WRITE_DCR, EMI_ECNT53,  0x00,         0x00ffffff   },
    {"EMI_BUSEN",   REGPOKER_WRITE_DCR, EMI_BUSEN,   0x00,         0x00000001   },
    {"EMI_WECR",    REGPOKER_WRITE_DCR, EMI_WECR,    0x00,         0x0000007f   },
    {"EMI_FLCNTRL", REGPOKER_WRITE_DCR, EMI_FLCNTRL, 0x1C,         0x0000001f   },
    {"EMI_IMR",     REGPOKER_WRITE_DCR, EMI_IMR,     0x00,         0x0001ffff   },
    {"EMI_IRR",     REGPOKER_WRITE_DCR, EMI_IRR,     0x00,         0x0001ffff   },
    {"EMI_ECCWRR",  REGPOKER_WRITE_DCR, EMI_ECCWRR,  0x00,         0x000000ff   },
    {"EMI_ECCRDR",  REGPOKER_WRITE_DCR, EMI_ECCRDR,  0x00,         0x000000ff   },
    {"EMI_H1CMR",   REGPOKER_WRITE_DCR, EMI_H1CMR,   0x00,         0x0000007f   },
    {"EMI_H2CMR",   REGPOKER_WRITE_DCR, EMI_H2CMR,   0x00,         0x0000007f   },
    {"EMI_RREQCMR", REGPOKER_WRITE_DCR, EMI_RREQCMR, 0x00,         0x0000007f   },
    {"EMI_WREQCMR", REGPOKER_WRITE_DCR, EMI_WREQCMR, 0x00,         0x0000007f   },
    {"EMI_WDCMR",   REGPOKER_WRITE_DCR, EMI_WDCMR,   0x00,         0x0000007f   },
    {"EMI_H1ADR",   REGPOKER_WRITE_DCR, EMI_H1ADR,   0x00,         0xfffffff0   },
    {"EMI_H2ADR",   REGPOKER_WRITE_DCR, EMI_H2ADR,   0x00,         0xfffffff0   },
    {"EMI_RREQADR", REGPOKER_WRITE_DCR, EMI_RREQADR, 0x00,         0xfffffff0   },
    {"EMI_WREQADR", REGPOKER_WRITE_DCR, EMI_WREQADR, 0x00,         0xfffffff0   },
    {"EMI_WDADR",   REGPOKER_WRITE_DCR, EMI_WDADR,   0x00,         0xfffffff0   },
    {/* Sentinel */}};

#include <regs/regs_sp804.h>
#define DIT0_REG_LOAD0_DFLT     0
#define DIT0_REG_VALUE0_DFLT    0xFFFFFFFF
#define DIT0_REG_CONTROL0_DFLT  0x20
#define DIT0_REG_RIS0_DFLT      0
#define DIT0_REG_MIS0_DFLT      0
#define DIT0_REG_BGLOAD0_DFLT   0

#define DIT0_REG_LOAD1_DFLT     0
#define DIT0_REG_VALUE1_DFLT    0xFFFFFFFF
#define DIT0_REG_CONTROL1_DFLT  0x20
#define DIT0_REG_RIS1_DFLT      0
#define DIT0_REG_MIS1_DFLT      0
#define DIT0_REG_BGLOAD1_DFLT   0

#define DIT_REG_ITCR_DFLT       0
#define DIT_REG_ITOP_DFLT       0
#define DIT_REG_PERIPHID0_DFLT  0x4
#define DIT_REG_PERIPHID1_DFLT  0x18
#define DIT_REG_PERIPHID2_DFLT  0x14
#define DIT_REG_PERIPHID3_DFLT  0
#define DIT_REG_PCELLID0_DFLT   0xD
#define DIT_REG_PCELLID1_DFLT   0xF0
#define DIT_REG_PCELLID2_DFLT   0x5
#define DIT_REG_PCELLID3_DFLT   0xB1

#define DIT0_REG_LOAD0_mask     0xFFFFFFFF
#define DIT0_REG_VALUE0_mask    0xFFFFFFFF
#define DIT0_REG_CONTROL0_mask  0xFF
#define DIT0_REG_RIS0_mask      1
#define DIT0_REG_MIS0_mask      1
#define DIT0_REG_BGLOAD0_mask   0xFFFFFFFF

#define DIT0_REG_LOAD1_mask     0xFFFFFFFF
#define DIT0_REG_VALUE1_mask    0xFFFFFFFF
#define DIT0_REG_CONTROL1_mask  0xFF
#define DIT0_REG_RIS1_mask      1
#define DIT0_REG_MIS1_mask      1
#define DIT0_REG_BGLOAD1_mask   0xFFFFFFFF

#define DIT_REG_ITCR_mask       1
#define DIT_REG_ITOP_mask       3
#define DIT_REG_PERIPHID0_mask  0xFF
#define DIT_REG_PERIPHID1_mask  0xFF
#define DIT_REG_PERIPHID2_mask  0xFF
#define DIT_REG_PERIPHID3_mask  0xFF
#define DIT_REG_PCELLID0_mask   0xFF
#define DIT_REG_PCELLID1_mask   0xFF
#define DIT_REG_PCELLID2_mask   0xFF
#define DIT_REG_PCELLID3_mask   0xFF

struct regpoker_checker sp804_checker[] = {
    {"DIT0_REG_LOAD0",    REGPOKER_READ_DCR, DIT0_REG_LOAD0,    DIT0_REG_LOAD0_DFLT,    DIT0_REG_LOAD0_mask      },
    {"DIT0_REG_VALUE0",   REGPOKER_READ_DCR, DIT0_REG_VALUE0,   DIT0_REG_VALUE0_DFLT,   DIT0_REG_VALUE0_mask     },
    {"DIT0_REG_CONTROL0", REGPOKER_READ_DCR, DIT0_REG_CONTROL0, DIT0_REG_CONTROL0_DFLT, DIT0_REG_CONTROL0_mask   },
    {"DIT0_REG_RIS0",     REGPOKER_READ_DCR, DIT0_REG_RIS0,     DIT0_REG_RIS0_DFLT,     DIT0_REG_RIS0_mask       },
    {"DIT0_REG_MIS0",     REGPOKER_READ_DCR, DIT0_REG_MIS0,     DIT0_REG_MIS0_DFLT,     DIT0_REG_MIS0_mask       },
    {"DIT0_REG_BGLOAD0",  REGPOKER_READ_DCR, DIT0_REG_BGLOAD0,  DIT0_REG_BGLOAD0_DFLT,  DIT0_REG_BGLOAD0_mask    },

    {"DIT0_REG_LOAD1",    REGPOKER_READ_DCR, DIT0_REG_LOAD1,    DIT0_REG_LOAD1_DFLT,    DIT0_REG_LOAD1_mask      },
    {"DIT0_REG_VALUE1",   REGPOKER_READ_DCR, DIT0_REG_VALUE1,   DIT0_REG_VALUE1_DFLT,   DIT0_REG_VALUE1_mask     },
    {"DIT0_REG_CONTROL1", REGPOKER_READ_DCR, DIT0_REG_CONTROL1, DIT0_REG_CONTROL1_DFLT, DIT0_REG_CONTROL1_mask   },
    {"DIT0_REG_RIS1",     REGPOKER_READ_DCR, DIT0_REG_RIS1,     DIT0_REG_RIS1_DFLT,     DIT0_REG_RIS1_mask       },
    {"DIT0_REG_MIS1",     REGPOKER_READ_DCR, DIT0_REG_MIS1,     DIT0_REG_MIS1_DFLT,     DIT0_REG_MIS1_mask       },
    {"DIT0_REG_BGLOAD1",  REGPOKER_READ_DCR, DIT0_REG_BGLOAD1,  DIT0_REG_BGLOAD1_DFLT,  DIT0_REG_BGLOAD1_mask    },

    {"DIT_REG_ITCR",      REGPOKER_READ_DCR, DIT_REG_ITCR,      DIT_REG_ITCR_DFLT,      DIT_REG_ITCR_mask        },
    {"DIT_REG_ITOP",      REGPOKER_READ_DCR, DIT_REG_ITOP,      DIT_REG_ITOP_DFLT,      DIT_REG_ITOP_mask        },
    {"DIT_REG_PERIPHID0", REGPOKER_READ_DCR, DIT_REG_PERIPHID0, DIT_REG_PERIPHID0_DFLT, DIT_REG_PERIPHID0_mask   },
    {"DIT_REG_PERIPHID1", REGPOKER_READ_DCR, DIT_REG_PERIPHID1, DIT_REG_PERIPHID1_DFLT, DIT_REG_PERIPHID1_mask   },
    {"DIT_REG_PERIPHID2", REGPOKER_READ_DCR, DIT_REG_PERIPHID2, DIT_REG_PERIPHID2_DFLT, DIT_REG_PERIPHID2_mask   },
    {"DIT_REG_PERIPHID3", REGPOKER_READ_DCR, DIT_REG_PERIPHID3, DIT_REG_PERIPHID3_DFLT, DIT_REG_PERIPHID3_mask   },
    {"DIT_REG_PCELLID0",  REGPOKER_READ_DCR, DIT_REG_PCELLID0,  DIT_REG_PCELLID0_DFLT,  DIT_REG_PCELLID0_mask    },
    {"DIT_REG_PCELLID1",  REGPOKER_READ_DCR, DIT_REG_PCELLID1,  DIT_REG_PCELLID1_DFLT,  DIT_REG_PCELLID1_mask    },
    {"DIT_REG_PCELLID2",  REGPOKER_READ_DCR, DIT_REG_PCELLID2,  DIT_REG_PCELLID2_DFLT,  DIT_REG_PCELLID2_mask    },
    {"DIT_REG_PCELLID3",  REGPOKER_READ_DCR, DIT_REG_PCELLID3,  DIT_REG_PCELLID3_DFLT,  DIT_REG_PCELLID3_mask    },
    {}};

struct regpoker_checker sp804_checker_rw[] = {
    {"Timer1Load",   REGPOKER_READ_DCR,  DIT0_REG_LOAD0,   0x00000000, 0xffffffff   },
    {"Timer1Load",   REGPOKER_WRITE_DCR, DIT0_REG_LOAD0,   0x00000000, 0xffffffff   },
    {"Timer1Value",  REGPOKER_READ_DCR,  DIT0_REG_VALUE0,  0x00000000, 0xffffffff   },
    {"Timer1RIS",    REGPOKER_READ_DCR,  DIT0_REG_RIS0,    0b0,        0b1          },
    {"Timer1MIS",    REGPOKER_READ_DCR,  DIT0_REG_MIS0,    0b0,        0b1          },
    {"Timer1BGLoad", REGPOKER_READ_DCR,  DIT0_REG_BGLOAD0, 0x00000000, 0xffffffff   },
    {"Timer1BGLoad", REGPOKER_WRITE_DCR, DIT0_REG_BGLOAD0, 0x00000000, 0xffffffff   },
    {"Timer2Load",   REGPOKER_WRITE_DCR, DIT0_REG_LOAD1,   0x00000000, 0xffffffff   },
    {"Timer2Value",  REGPOKER_READ_DCR,  DIT0_REG_VALUE1,  0x00000000, 0x00000000   },
    {"Timer2RIS",    REGPOKER_READ_DCR,  DIT0_REG_RIS1,    0b0,        0b1          },
    {"Timer2MIS",    REGPOKER_READ_DCR,  DIT0_REG_MIS1,    0b0,        0b1          },
    {"Timer2BGLoad", REGPOKER_READ_DCR,  DIT0_REG_BGLOAD1, 0x0,        0xff         },
    {"Timer2BGLoad", REGPOKER_WRITE_DCR, DIT0_REG_BGLOAD1, 0x0,        0xff         },
    {"TimerITCR",    REGPOKER_READ_DCR,  DIT_REG_ITCR,     0b0,        0b1          },
    {"TimerITCR",    REGPOKER_WRITE_DCR, DIT_REG_ITCR,     0b0,        0b1          },
    {"TimerITOP",    REGPOKER_READ_DCR,  DIT_REG_ITOP,     0b00,       0b11         },
    {}};

    
 #include <platform/regs/regs_l2c_l2.h> 
 
#define L2C_L2CDCRAI_MSK 0x00000fff
#define L2C_L2CDCRDI_MSK 0xffffffff
#define L2C_PMUDCRAI_MSK 0x00000fff
#define L2C_PMUDCRDI_MSK 0xffffffff
 


static const struct regpoker_checker l2c_regs_default[] = {
    //      name                check type          offset             exp val     mask
    {"L2C_L2CDCRAI    ",       REGPOKER_READ_DCR, L2C_L2CDCRAI    ,      0x00000000, L2C_L2CDCRAI_MSK},
    {"L2C_L2CDCRDI    ",       REGPOKER_READ_DCR, L2C_L2CDCRDI    ,      0x00000001, L2C_L2CDCRDI_MSK},
    {"L2C_PMUDCRAI    ",       REGPOKER_READ_DCR, L2C_PMUDCRAI    ,      0x00000000, L2C_PMUDCRAI_MSK},
    {"L2C_PMUDCRDI    ",       REGPOKER_READ_DCR, L2C_PMUDCRDI    ,      0xffff0008, L2C_PMUDCRDI_MSK},
    /*
    {"L2C_L2ISTAT    ",       REGPOKER_READ_DCR, L2C_L2ISTAT    ,      0x00000000, 0xffffffff},
    {"L2C_L2PNCR     ",       REGPOKER_READ_DCR, L2C_L2PNCR     ,      0x00000001, 0xffffffff},
    {"L2C_L2REVID    ",       REGPOKER_READ_DCR, L2C_L2REVID    ,      0x00000000, 0xffffffff},
    {"L2C_L2CNFG0    ",       REGPOKER_READ_DCR, L2C_L2CNFG0    ,      0x00000000, 0xffffffff},
    {"L2C_L2CNFG1    ",       REGPOKER_READ_DCR, L2C_L2CNFG1    ,      0x00000000, 0xffffffff},
    {"L2C_L2DBGSEL   ",       REGPOKER_READ_DCR, L2C_L2DBGSEL   ,      0x00000000, 0xffffffff},
    {"L2C_L2DBGDATA0 ",       REGPOKER_READ_DCR, L2C_L2DBGDATA0 ,      0x00000000, 0xffffffff},
    {"L2C_L2DBGDATA1 ",       REGPOKER_READ_DCR, L2C_L2DBGDATA1 ,      0x00000000, 0xffffffff},
    {"L2C_L2SLEEPSTAT",       REGPOKER_READ_DCR, L2C_L2SLEEPSTAT,      0x00000000, 0xffffffff},
    {"L2C_L2SLEEPREQ ",       REGPOKER_READ_DCR, L2C_L2SLEEPREQ ,      0x00000000, 0x00000001},
    {"L2C_L2MCK      ",       REGPOKER_READ_DCR, L2C_L2MCK      ,      0x00000008, 0x00000ffe},
    {"L2C_L2MCKEN    ",       REGPOKER_READ_DCR, L2C_L2MCKEN    ,      0x00000000, 0xffffffff},
    {"L2C_L2FERR     ",       REGPOKER_READ_DCR, L2C_L2FERR     ,      0x00000000, 0xffffffff},
    {"L2C_L2INT      ",       REGPOKER_READ_DCR, L2C_L2INT      ,      0x00000000, 0xffffffff},
    {"L2C_L2INTEN    ",       REGPOKER_READ_DCR, L2C_L2INTEN    ,      0x00000000, 0xffffffff},
    {"L2C_L2LOG0     ",       REGPOKER_READ_DCR, L2C_L2LOG0     ,      0x00000000, 0xffffffff},
    {"L2C_L2LOG1     ",       REGPOKER_READ_DCR, L2C_L2LOG1     ,      0x00000000, 0xffffffff},
    {"L2C_L2LOG2     ",       REGPOKER_READ_DCR, L2C_L2LOG2     ,      0x00000000, 0xffffffff},
    {"L2C_L2LOG3     ",       REGPOKER_READ_DCR, L2C_L2LOG3     ,      0x00000000, 0xffffffff},
    {"L2C_L2LOG4     ",       REGPOKER_READ_DCR, L2C_L2LOG4     ,      0x00000000, 0xffffffff},
    {"L2C_L2LOG5     ",       REGPOKER_READ_DCR, L2C_L2LOG5     ,      0x00000000, 0xffffffff},
    {"L2C_L2PLBCFG   ",       REGPOKER_READ_DCR, L2C_L2PLBCFG   ,      0x00000000, 0xffffffff},
    {"L2C_L2PLBDBG   ",       REGPOKER_READ_DCR, L2C_L2PLBDBG   ,      0x00000000, 0xffffffff},
    {"L2C_L2PLBERAP  ",       REGPOKER_READ_DCR, L2C_L2PLBERAP  ,      0x00000000, 0xffffffff},
    {"L2C_L2PLBSTAT0 ",       REGPOKER_READ_DCR, L2C_L2PLBSTAT0 ,      0x00000000, 0xffffffff},
    {"L2C_L2PLBSTAT1 ",       REGPOKER_READ_DCR, L2C_L2PLBSTAT1 ,      0x00000000, 0xffffffff},
    {"L2C_L2PLBFRC0  ",       REGPOKER_READ_DCR, L2C_L2PLBFRC0  ,      0x00000000, 0xffffffff},
    {"L2C_L2PLBFRC1  ",       REGPOKER_READ_DCR, L2C_L2PLBFRC1  ,      0x00000000, 0xffffffff},
    {"L2C_L2PLBMCKEN0",       REGPOKER_READ_DCR, L2C_L2PLBMCKEN0,      0x00000000, 0xffffffff},
    {"L2C_L2PLBMCKEN1",       REGPOKER_READ_DCR, L2C_L2PLBMCKEN1,      0x00000000, 0xffffffff},
    {"L2C_L2PLBFERR0 ",       REGPOKER_READ_DCR, L2C_L2PLBFERR0 ,      0x00000000, 0xffffffff},
    {"L2C_L2PLBFERR1 ",       REGPOKER_READ_DCR, L2C_L2PLBFERR1 ,      0x00000000, 0xffffffff},
    {"L2C_L2PLBINTEN0",       REGPOKER_READ_DCR, L2C_L2PLBINTEN0,      0x00000000, 0xffffffff},
    {"L2C_L2PLBINTEN1",       REGPOKER_READ_DCR, L2C_L2PLBINTEN1,      0x00000000, 0xffffffff},
    {"L2C_L2ARRCFG   ",       REGPOKER_READ_DCR, L2C_L2ARRCFG   ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRDBG0  ",       REGPOKER_READ_DCR, L2C_L2ARRDBG0  ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRDBG1  ",       REGPOKER_READ_DCR, L2C_L2ARRDBG1  ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRDBG2  ",       REGPOKER_READ_DCR, L2C_L2ARRDBG2  ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRDBG3  ",       REGPOKER_READ_DCR, L2C_L2ARRDBG3  ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRACCCTL",       REGPOKER_READ_DCR, L2C_L2ARRACCCTL,      0x00000000, 0xffffffff},
    {"L2C_L2ARRACCADR",       REGPOKER_READ_DCR, L2C_L2ARRACCADR,      0x00000000, 0xffffffff},
    {"L2C_L2ARRACCDI0",       REGPOKER_READ_DCR, L2C_L2ARRACCDI0,      0x00000000, 0xffffffff},
    {"L2C_L2ARRACCDI1",       REGPOKER_READ_DCR, L2C_L2ARRACCDI1,      0x00000000, 0xffffffff},
    {"L2C_L2ARRACCDI2",       REGPOKER_READ_DCR, L2C_L2ARRACCDI2,      0x00000000, 0xffffffff},
    {"L2C_L2ARRACCDO0",       REGPOKER_READ_DCR, L2C_L2ARRACCDO0,      0x00000000, 0xffffffff},
    {"L2C_L2ARRACCDO1",       REGPOKER_READ_DCR, L2C_L2ARRACCDO1,      0x00000000, 0xffffffff},
    {"L2C_L2ARRACCDO2",       REGPOKER_READ_DCR, L2C_L2ARRACCDO2,      0x00000000, 0xffffffff},
    {"L2C_L2ARRSTAT0 ",       REGPOKER_READ_DCR, L2C_L2ARRSTAT0 ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRSTAT1 ",       REGPOKER_READ_DCR, L2C_L2ARRSTAT1 ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRSTAT2 ",       REGPOKER_READ_DCR, L2C_L2ARRSTAT2 ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRFRC0  ",       REGPOKER_READ_DCR, L2C_L2ARRFRC0  ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRFRC1  ",       REGPOKER_READ_DCR, L2C_L2ARRFRC1  ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRFRC2  ",       REGPOKER_READ_DCR, L2C_L2ARRFRC2  ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRMCKEN0",       REGPOKER_READ_DCR, L2C_L2ARRMCKEN0,      0x00000000, 0xffffffff},
    {"L2C_L2ARRMCKEN1",       REGPOKER_READ_DCR, L2C_L2ARRMCKEN1,      0x00000000, 0xffffffff},
    {"L2C_L2ARRMCKEN2",       REGPOKER_READ_DCR, L2C_L2ARRMCKEN2,      0x00000000, 0xffffffff},
    {"L2C_L2ARRFERR0 ",       REGPOKER_READ_DCR, L2C_L2ARRFERR0 ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRFERR1 ",       REGPOKER_READ_DCR, L2C_L2ARRFERR1 ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRFERR2 ",       REGPOKER_READ_DCR, L2C_L2ARRFERR2 ,      0x00000000, 0xffffffff},
    {"L2C_L2ARRINTEN0",       REGPOKER_READ_DCR, L2C_L2ARRINTEN0,      0x00000000, 0xffffffff},
    {"L2C_L2ARRINTEN1",       REGPOKER_READ_DCR, L2C_L2ARRINTEN1,      0x00000000, 0xffffffff},
    {"L2C_L2ARRINTEN2",       REGPOKER_READ_DCR, L2C_L2ARRINTEN2,      0x00000000, 0xffffffff},
    {"L2C_L2CPUCFG   ",       REGPOKER_READ_DCR, L2C_L2CPUCFG   ,      0x00000000, 0xffffffff},
    {"L2C_L2CPUDBG   ",       REGPOKER_READ_DCR, L2C_L2CPUDBG   ,      0x00000000, 0xffffffff},
    {"L2C_L2CPUSTAT  ",       REGPOKER_READ_DCR, L2C_L2CPUSTAT  ,      0x00000000, 0xffffffff},
    {"L2C_L2CPUFRC   ",       REGPOKER_READ_DCR, L2C_L2CPUFRC   ,      0x00000000, 0xffffffff},
    {"L2C_L2CPUMCKEN ",       REGPOKER_READ_DCR, L2C_L2CPUMCKEN ,      0x00000000, 0xffffffff},
    {"L2C_L2CPUFERR  ",       REGPOKER_READ_DCR, L2C_L2CPUFERR  ,      0x00000000, 0xffffffff},
    {"L2C_L2CPUINTEN ",       REGPOKER_READ_DCR, L2C_L2CPUINTEN ,      0x00000000, 0xffffffff},
    {"L2C_L2RACCFG   ",       REGPOKER_READ_DCR, L2C_L2RACCFG   ,      0x00000000, 0xffffffff},
    {"L2C_L2RACDBG0  ",       REGPOKER_READ_DCR, L2C_L2RACDBG0  ,      0x00000000, 0xffffffff},
    {"L2C_L2RACDBG1  ",       REGPOKER_READ_DCR, L2C_L2RACDBG1  ,      0x00000000, 0xffffffff},
    {"L2C_L2RACSTAT0 ",       REGPOKER_READ_DCR, L2C_L2RACSTAT0 ,      0x00000000, 0xffffffff},
    {"L2C_L2RACFRC0  ",       REGPOKER_READ_DCR, L2C_L2RACFRC0  ,      0x00000000, 0xffffffff},
    {"L2C_L2RACMCKEN0",       REGPOKER_READ_DCR, L2C_L2RACMCKEN0,      0x00000000, 0xffffffff},
    {"L2C_L2RACFERR0 ",       REGPOKER_READ_DCR, L2C_L2RACFERR0 ,      0x00000000, 0xffffffff},
    {"L2C_L2RACINTEN0",       REGPOKER_READ_DCR, L2C_L2RACINTEN0,      0x00000000, 0xffffffff},
    {"L2C_L2WACCFG   ",       REGPOKER_READ_DCR, L2C_L2WACCFG   ,      0x00000000, 0xffffffff},
    {"L2C_L2WACDBG0  ",       REGPOKER_READ_DCR, L2C_L2WACDBG0  ,      0x00000000, 0xffffffff},
    {"L2C_L2WACDBG1  ",       REGPOKER_READ_DCR, L2C_L2WACDBG1  ,      0x00000000, 0xffffffff},
    {"L2C_L2WACDBG2  ",       REGPOKER_READ_DCR, L2C_L2WACDBG2  ,      0x00000000, 0xffffffff},
    {"L2C_L2WACSTAT0 ",       REGPOKER_READ_DCR, L2C_L2WACSTAT0 ,      0x00000000, 0xffffffff},
    {"L2C_L2WACSTAT1 ",       REGPOKER_READ_DCR, L2C_L2WACSTAT1 ,      0x00000000, 0xffffffff},
    {"L2C_L2WACSTAT2 ",       REGPOKER_READ_DCR, L2C_L2WACSTAT2 ,      0x00000000, 0xffffffff},
    {"L2C_L2WACFRC0  ",       REGPOKER_READ_DCR, L2C_L2WACFRC0  ,      0x00000000, 0xffffffff},
    {"L2C_L2WACFRC1  ",       REGPOKER_READ_DCR, L2C_L2WACFRC1  ,      0x00000000, 0xffffffff},
    {"L2C_L2WACFRC2  ",       REGPOKER_READ_DCR, L2C_L2WACFRC2  ,      0x00000000, 0xffffffff},
    {"L2C_L2WACMCKEN0",       REGPOKER_READ_DCR, L2C_L2WACMCKEN0,      0x00000000, 0xffffffff},
    {"L2C_L2WACMCKEN1",       REGPOKER_READ_DCR, L2C_L2WACMCKEN1,      0x00000000, 0xffffffff},
    {"L2C_L2WACMCKEN2",       REGPOKER_READ_DCR, L2C_L2WACMCKEN2,      0x00000000, 0xffffffff},
    {"L2C_L2WACFERR0 ",       REGPOKER_READ_DCR, L2C_L2WACFERR0 ,      0x00000000, 0xffffffff},
    {"L2C_L2WACFERR1 ",       REGPOKER_READ_DCR, L2C_L2WACFERR1 ,      0x00000000, 0xffffffff},
    {"L2C_L2WACFERR2 ",       REGPOKER_READ_DCR, L2C_L2WACFERR2 ,      0x00000000, 0xffffffff},
    {"L2C_L2WACINTEN0",       REGPOKER_READ_DCR, L2C_L2WACINTEN0,      0x00000000, 0xffffffff},
    {"L2C_L2WACINTEN1",       REGPOKER_READ_DCR, L2C_L2WACINTEN1,      0x00000000, 0xffffffff},
    {"L2C_L2WACINTEN2",       REGPOKER_READ_DCR, L2C_L2WACINTEN2,      0x00000000, 0xffffffff},
    */
    
    
    
    {/* Sentinel */}};
/* ************************************ */


struct rumboot_pokerlist { 
    const  struct regpoker_checker *checker; 
    uintptr_t base;
};

static struct rumboot_pokerlist the_big_list[] = {
    {plb4arb8m_0_regs_default,   DCR_PLB4ARB8M_0_BASE},    
    {srammc2plb4_0_regs_default, DCR_SRAMMC2PLB4_0_BASE},
    {srammc2plb4_1_regs_default, DCR_SRAMMC2PLB4_1_BASE},
    {plb4ahb_regs_default,       DCR_PLB4AHB_0_BASE},
    {plb4arb8m_2_regs_default,   DCR_PLB4ARB8M_2_BASE},
    
    {dma2plb6_regs_default,      DCR_DMAPLB6_BASE},
    {plb6bc_regs_default,        DCR_PLB6_BC_BASE},
    
    {plb6plb4_0_regs_default,    DCR_PLB6PLB4_0_BASE},
    {plb6plb4_1_regs_default,    DCR_PLB6PLB4_1_BASE},    
    
    {l2c_regs_default,           DCR_L2C_BASE},
    {dcrarb_regs_default,        DCR_ARB_BASE},
    {itrace_regs_default,        DCR_ITRACE_BASE},
    {ltrace_regs_default,        DCR_LTRACE_BASE},
    {cldcr_regs_default,         DCR_CLDCR_BASE}, 
    
    
    {plb6mcif2_regs_default,     DCR_EM2_PLB6MCIF2_BASE},
    {plb6mcif2_regs_write,       DCR_EM2_PLB6MCIF2_BASE },    
    {emi_regs_default,           DCR_EM2_EMI_BASE },
    {emi_regs_write,             DCR_EM2_EMI_BASE },    
    {mclfir_regs_default,        DCR_EM2_MCLFIR_BASE},
    {mclfir_regs_write,          DCR_EM2_MCLFIR_BASE}, 
    
    {crg_checker,                DCR_CRG_BASE},
    //{crg_checker_rw,             DCR_CRG_BASE},    
    {sctl_checker,               DCR_SCTL_BASE},
    {sctl_rw_checker,            DCR_SCTL_BASE},    
    {sp804_checker,              DCR_TIMERS_BASE },
    // Will screw up timing and delays
    //{sp804_checker_rw,           DCR_TIMERS_BASE },    
    {wdt_checker,                DCR_WATCHDOG_BASE},
    {wdt_rw_checker,             DCR_WATCHDOG_BASE},    
    {mcif2arb4_regs_default,     DCR_MCIF2ARB4_BASE},    
    {aximcif2_regs_default,      DCR_AXIMCIF2_BASE},
    
    {mpic128_regs_default, DCR_MPIC128_BASE},
    
    {mkio_checker,               MKIO0_BASE},
    {mkio_checker,               MKIO1_BASE},
    {gpio_checker,               GPIO_0_BASE },
    {gpio_checker,               GPIO_1_BASE },
    {gpio_rw_checker,            GPIO_0_BASE },
    {gpio_rw_checker,            GPIO_1_BASE },
    //{uart_checker,               UART0_BASE}, /* Screws up serial communications */
    {uart_checker,               UART1_BASE},
    
    {greth0_read_checker, GRETH_0_BASE},
    {greth1_read_checker, GRETH_1_BASE},
    {greth_write_checker, GRETH_0_BASE},
    {greth_write_checker, GRETH_1_BASE},
                                 
    {spi_checker,                GSPI_0_BASE}, /* N.B. Might screws up SPI communications */
    {spi_checker,                GSPI_1_BASE},
                                 
    {sdio_read_checker,          SDIO_0_BASE},
    {sdio_read_checker,          SDIO_1_BASE},
                                 
    {nic_checker,                AXI32HSIFS_CTRL_BASE},
    {nic_checker,                AXI64HSIFS_CTRL_BASE},
                                 
    {nic_checker_rw32,           AXI32HSIFS_CTRL_BASE },
    {nic_checker_rw64,           AXI64HSIFS_CTRL_BASE },
                                 
    {hscb_read_checker,          HSCB0_BASE},
    {hscb_read_checker,          HSCB1_BASE},
    {hscb_read_checker,          HSCB2_BASE},
    {hscb_read_checker,          HSCB3_BASE},

    /* These may need adjustments to target hw */
    //{em2_checker_a,       NOR_BASE },
    //{em2_checker_b,       NOR_BASE },
    //{em2_checker_c,       NOR_BASE },
    //{em2_checker_d,       NOR_BASE },

    /* These may need adjustments as well */
//    {crg_checker,         DCR_CRG_BASE},
//    {crg_checker_rw,      DCR_CRG_BASE},

#if 0
    {&em2x_checker, NULL, 1 },

#endif
    {}
};


static int is_readonly(const  struct regpoker_checker *chk){
    while (chk->tp != REGPOKER_EOL) {
        if ((chk->tp == REGPOKER_WRITE8) ||
        (chk->tp == REGPOKER_WRITE16) ||
        (chk->tp == REGPOKER_WRITE32) ||
        (chk->tp == REGPOKER_WRITE_DCR)) {
            return 0;
        }
        chk++;
    }
    return 1;
}

int regpoker_exec_list(const  struct rumboot_pokerlist *pos, int readonly)
{
    int total = 0;
    while (pos->checker) {
        int check = is_readonly(pos->checker);
        if (readonly && (check != readonly)) {
            pos++;
            continue; /* Skip write tests */
        }
        rumboot_printf("Performing %s on %x ... ", check? "read check" : "write check", pos->base);
        int ret = rumboot_regpoker_check_array(pos->checker, pos->base);
        rumboot_printf("%s (%d)\n", ret ? "FAILED" : "OK", ret);
        pos++;
        total += ret;
    }
    return total;
}

int main()
{
    iowrite32(0x0, 0xc0028400); /* Revert gpio dir value to stock */
    int ret = regpoker_exec_list(the_big_list, 1); /* read-only check */
    ret = regpoker_exec_list(the_big_list, 0); /* read and write checks */
    rumboot_printf("Done testing\n");
    return ret;
}
