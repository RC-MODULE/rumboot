#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <platform/easynmc/easynmc.h>
#include <stdint.h>
#include <devices/greth_edcl.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/rumboot.h>

#include <rumboot/memtest.h>


#include <rumboot/regpoker.h>
#include <platform/devices.h>

#include <regs/regs_gpio_pl061.h>
#include <platform/regs/regs_com.h>
#include <platform/regs/regs_greth.h>

#include <regs/regs_sdio.h>
#include <regs/regs_spi.h>
#include <regs/regs_mkio.h>
#include <regs/regs_uart_pl011.h>
#include <platform/regs/regs_hscb.h>
#include <platform/regs/regs_ndma.h>
#include <platform/regs/regs_nic400.h>
#include <platform/regs/regs_scrb.h>

//#undef GSPI_STATUS_DEFAULT
//#define GSPI_STATUS_DEFAULT 0x82


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
    {"GPIO_DIR",        REGPOKER_WRITE32, GPIO_DIR,                   GPIO_DIR_DEFAULT,    0x00            },
    {"GPIO_IS",         REGPOKER_WRITE32, GPIO_IS,                    GPIO_IS_DEFAULT,     GPIO_REG_MASK   },
    {"GPIO_IBE",        REGPOKER_WRITE32, GPIO_IBE,                   GPIO_IBE_DEFAULT,    GPIO_REG_MASK   },
    {"GPIO_IEV",        REGPOKER_WRITE32, GPIO_IEV,                   GPIO_IEV_DEFAULT,    GPIO_REG_MASK   },
    {"GPIO_IE",         REGPOKER_WRITE32, GPIO_IE,                    GPIO_IE_DEFAULT,     GPIO_REG_MASK   },
    {"GPIO_AFSEL",      REGPOKER_WRITE32, GPIO_AFSEL,                 GPIO_AFSEL_DEFAULT,  GPIO_REG_MASK   },
    {/* Sentinel */}};



struct regpoker_checker greth0_read_checker[] = {
    {"CTRL              ", REGPOKER_READ32  , CTRL          ,     GRETH_CTRL_DEFAULT           ,  GRETH_CTRL_MASK            }, 
    {"MDIO_CTRL         ", REGPOKER_READ32  , MDIO_CTRL     ,     GRETH_MDIO_CTRL_DEFAULT      ,  GRETH_MDIO_CTRL_MASK       }, 
    {"EDCL_IP           ", REGPOKER_READ32  , EDCL_IP       ,     GRETH0_EDCL_IP_DEFAULT        ,  GRETH_EDCL_IP_MASK         },
    {"EDCL_MAC_MSB      ", REGPOKER_READ32  , EDCL_MAC_MSB  ,     GRETH0_EDCL_MAC_MSB_DEFAULT   ,  GRETH_EDCL_MAC_MSB_MASK    },
    {"EDCL_MAC_LSB      ", REGPOKER_READ32  , EDCL_MAC_LSB  ,     GRETH0_EDCL_MAC_LSB_DEFAULT   ,  GRETH_EDCL_MAC_LSB_MASK    },
    { } // Missing      
    };

struct regpoker_checker greth0_rw_checker[] = {
    {"EDCL_IP           ", REGPOKER_WRITE32 , EDCL_IP       ,     GRETH0_EDCL_IP_DEFAULT        ,  GRETH_EDCL_IP_MASK         },
    { } // Missing      
    };

struct regpoker_checker greth1_read_checker[] = {
    {"CTRL              ", REGPOKER_READ32  , CTRL          ,     GRETH_CTRL_DEFAULT           ,  GRETH_CTRL_MASK            }, 
    {"MDIO_CTRL         ", REGPOKER_READ32  , MDIO_CTRL     ,     GRETH_MDIO_CTRL_DEFAULT      ,  GRETH_MDIO_CTRL_MASK       }, 
    {"EDCL_IP           ", REGPOKER_READ32  , EDCL_IP       ,     GRETH1_EDCL_IP_DEFAULT        ,  GRETH_EDCL_IP_MASK         },
    {"EDCL_MAC_MSB      ", REGPOKER_READ32  , EDCL_MAC_MSB  ,     GRETH1_EDCL_MAC_MSB_DEFAULT   ,  GRETH_EDCL_MAC_MSB_MASK    },
    {"EDCL_MAC_LSB      ", REGPOKER_READ32  , EDCL_MAC_LSB  ,     GRETH1_EDCL_MAC_LSB_DEFAULT   ,  GRETH_EDCL_MAC_LSB_MASK    },
    { } // Missing      
    };

struct regpoker_checker greth1_rw_checker[] = {
    {"EDCL_IP           ", REGPOKER_WRITE32 , EDCL_IP       ,     GRETH1_EDCL_IP_DEFAULT        ,  GRETH_EDCL_IP_MASK         },
    { } // Missing      
    };



struct regpoker_checker sdio_read_checker[] = {
    {"SDIO_SDR_CARD_BLOCK_SET_REG    ", REGPOKER_READ32, SDIO_SDR_CARD_BLOCK_SET_REG, SDIO_SDR_CARD_BLOCK_SET_REG_DFLT, 0x03ff03ff},
    {"SDIO_SDR_CTRL_REG              ", REGPOKER_READ32, SDIO_SDR_CTRL_REG,           SDIO_SDR_CTRL_REG_DFLT,           0x003f7fd3},
    {"SDIO_SDR_CMD_ARGUMENT_REG      ", REGPOKER_READ32, SDIO_SDR_CMD_ARGUMENT_REG,   SDIO_SDR_CMD_ARGUMENT_REG_DFLT,   0xffffffff},
    {"SDIO_SDR_ADDRESS_REG           ", REGPOKER_READ32, SDIO_SDR_ADDRESS_REG,        SDIO_SDR_ADDRESS_REG_DFLT,        0x00000004},
    {"SDIO_SDR_STATUS_REG            ", REGPOKER_READ32, SDIO_SDR_STATUS_REG,         0x800,                            0x006f9c00},
    { } // Missing
    };

static struct regpoker_checker const spi_checker[] = {
    {"SSPCR0",       REGPOKER_READ32,  GSPI_SSPCR0,         GSPI_SSPCR0_DEFAULT,       GSPI_SSPCR0_MASK            },
    {"SSPCR1",       REGPOKER_READ32,  GSPI_SSPCR1,         GSPI_SSPCR1_DEFAULT,       GSPI_SSPCR1_MASK            },
    {"SSPSR",        REGPOKER_READ32,  GSPI_SSPSR,          GSPI_SSPSR_DEFAULT,        GSPI_SSPSR_MASK             },
    { } // Missing
    };



struct regpoker_checker mkio_checker[] ={
        {"IRQ    ", REGPOKER_READ32, IRQ,     0x00000000, 0xFFFFFFFF},
        {"IRQE   ", REGPOKER_READ32, IRQE,    0x00000000, 0xFFFFFFFF},
        {"HWCFG  ", REGPOKER_READ32, HWCFG,   0x00000000, 0xFFFFFFFF},
        {"BMLBE  ", REGPOKER_READ32, BMLBE,   0x00000007, 0xFFFFFFFF},

        {"BCRD   ", REGPOKER_WRITE32, BCRD,   0x00000000, 0xFFFFFFFC},
        {"BCBS   ", REGPOKER_WRITE32, BCBS,   0x00000000, 0xFFFFFFFF},
        {"RTSTBA ", REGPOKER_WRITE32, RTSTBA, 0x00000000, 0xFFFFFE00},
        { } // Missing
    };



struct regpoker_checker uart_checker[] = {
        {"UARTRSR",       REGPOKER_READ32,  UARTRSR,         UARTRSR_default,       UARTRSR_mask         },
        {"UARTFR",        REGPOKER_READ32,  UARTFR,          UARTFR_default,        UARTFR_mask          },
        {"UARTILPR",      REGPOKER_READ32,  UARTILPR,        UARTILPR_default,      UARTILPR_mask        },
        {"UARTIBRD",      REGPOKER_READ32,  UARTIBRD,        UARTIBRD_default,      UARTIBRD_mask        },
        {"UARTFBRD",      REGPOKER_READ32,  UARTFBRD,        UARTFBRD_default,      UARTFBRD_mask        },
        {"UARTLCR_H",     REGPOKER_READ32,  UARTLCR_H,       UARTLCR_H_default,     UARTLCR_H_mask       },

        {"UARTILPR",      REGPOKER_WRITE32, UARTILPR,        0x00,                  UARTILPR_mask        },
        {"UARTIBRD",      REGPOKER_WRITE32, UARTIBRD,        0x00,                  UARTFBRD_mask        },
        {"UARTFBRD",      REGPOKER_WRITE32, UARTFBRD,        0x00,                  UARTFBRD_mask        },
        {"UARTLCR_H",     REGPOKER_WRITE32, UARTLCR_H,       0x00,                  UARTLCR_H_mask       },
        { } // Missing
    };



#define HSCB_ID_DFLT              0x42435348
#define HSCB_VERSION_DFLT         0x00000102
#define HSCB_SW_RESET_DFLT        0x00000000
#define HSCB_TRANS_CLK_DFLT       0x00c81414

struct regpoker_checker hscb_read_checker[] = {
        {"ID               ", REGPOKER_READ32, HSCB_ID,              HSCB_ID_DFLT,              0xffffffff}, // new
        {"VERSION          ", REGPOKER_READ32, HSCB_VERSION,         HSCB_VERSION_DFLT,         0xffffffff}, // new
        {"SW_RESET         ", REGPOKER_READ32, HSCB_SW_RESET,        HSCB_SW_RESET_DFLT,        0xffffffff}, // new
        {"TRANS_CLK        ", REGPOKER_READ32, HSCB_TRANS_CLK,       HSCB_TRANS_CLK_DFLT,       0xffffffff}, // new
        { } // Missing
    };
    

struct regpoker_checker com_port_checker[] = {
    {"MainCounter_tr  ",        REGPOKER_READ32, MainCounter_tr  ,      COM_MAINCOUNTER_TR_DEFAULT  ,   COM_MAINCOUNTER_TR_MASK     },
    {"Address_tr      ",        REGPOKER_READ32, Address_tr      ,      COM_ADDRESS_TR_DEFAULT      ,   COM_ADDRESS_TR_MASK         },
    {"Bias_tr         ",        REGPOKER_READ32, Bias_tr         ,      COM_BIAS_TR_DEFAULT         ,   COM_BIAS_TR_MASK            },
    {"RowCounter_tr   ",        REGPOKER_READ32, RowCounter_tr   ,      COM_ROWCOUNTER_TR_DEFAULT   ,   COM_ROWCOUNTER_TR_MASK      },
    {"AddressMode_tr  ",        REGPOKER_READ32, AddressMode_tr  ,      COM_ADDRESSMODE_TR_DEFAULT  ,   COM_ADDRESSMODE_TR_MASK     },
    {"CSR_tr          ",        REGPOKER_READ32, CSR_tr          ,      COM_CSR_TR_DEFAULT          ,   COM_CSR_TR_MASK             },
    {"InterruptMask_tr",        REGPOKER_READ32, InterruptMask_tr,      COM_INTERRUPTMASK_TR_DEFAULT,   COM_INTERRUPTMASK_TR_MASK   },
    {"InternalState_tr",        REGPOKER_READ32, InternalState_tr,      COM_INTERNALSTATE_TR_DEFAULT,   COM_INTERNALSTATE_TR_MASK   },

    {/* Sentinel */}
};

struct regpoker_checker com_port_rw_checker[] = {
    //{"MainCounter_tr  ",        REGPOKER_WRITE32, MainCounter_tr  ,      COM_MAINCOUNTER_TR_DEFAULT  ,   COM_MAINCOUNTER_TR_MASK     },
    {"Address_tr      ",          REGPOKER_WRITE32, Address_tr      ,      COM_ADDRESS_TR_DEFAULT      ,   COM_ADDRESS_TR_MASK         },
    {"Bias_tr         ",          REGPOKER_WRITE32, Bias_tr         ,      COM_BIAS_TR_DEFAULT         ,   COM_BIAS_TR_MASK            },

    {/* Sentinel */}
};



#define NDMA_MAINCOUNTER_DEFAULT 0x00000100
#define NDMARD_ADDRESS_DEFAULT   0x00020000
#define NDMAWR_ADDRESS_DEFAULT   0x00000000

struct regpoker_checker axi_dma_read_checker[] = {
    {"NDMA_MainCounter", REGPOKER_READ32, NDMA_MainCounter , NDMA_MAINCOUNTER_DEFAULT, 0x0000FFFF         },
    {"NDMARD_Address  ", REGPOKER_READ32, NDMARD_Address   , NDMARD_ADDRESS_DEFAULT  , 0xFFFFFFFF         },
    {"NDMAWR_Address  ", REGPOKER_READ32, NDMAWR_Address   , NDMAWR_ADDRESS_DEFAULT  , 0xFFFFFFFF         },
    {/* Sentinel */}
};

struct regpoker_checker axi_dma_rw_checker[] = {
    {"NDMA_MainCounter", REGPOKER_READ32, NDMA_MainCounter , NDMA_MAINCOUNTER_DEFAULT , 0x0000FFFF         },
    {"NDMARD_Address  ", REGPOKER_READ32, NDMARD_Address   , NDMARD_ADDRESS_DEFAULT   , 0xFFFFFFFF         },
    {"NDMAWR_Address  ", REGPOKER_READ32, NDMAWR_Address   , NDMAWR_ADDRESS_DEFAULT   , 0xFFFFFFFF         },
    {/* Sentinel */}
};




struct regpoker_checker interconnect_axi64_rw_checker[] = {
    { "NIC400_SW_0_S_RQOS", REGPOKER_READ32, NIC400_SW_0_S_RQOS, 0x0, 0b1111 },
    { "NIC400_SW_0_S_RQOS", REGPOKER_WRITE32, NIC400_SW_0_S_RQOS, 0x0, 0b1111 },
    
    { "NIC400_SW_0_S_WQOS", REGPOKER_READ32, NIC400_SW_0_S_WQOS, 0x0, 0b1111 },
    { "NIC400_SW_0_S_WQOS", REGPOKER_WRITE32, NIC400_SW_0_S_WQOS, 0x0, 0b1111 },
    
    { "NIC400_SW_0_S_FN_MOD", REGPOKER_READ32, NIC400_SW_0_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_SW_0_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_0_S_FN_MOD, 0x0, 0b11 },
    
    { "NIC400_SW_1_S_RQOS", REGPOKER_READ32, NIC400_SW_1_S_RQOS, 0x0, 0b1111 },
    { "NIC400_SW_1_S_RQOS", REGPOKER_WRITE32, NIC400_SW_1_S_RQOS, 0x0, 0b1111 },
    
    { "NIC400_SW_1_S_WQOS", REGPOKER_READ32, NIC400_SW_1_S_WQOS, 0x0, 0b1111 },
    { "NIC400_SW_1_S_WQOS", REGPOKER_WRITE32, NIC400_SW_1_S_WQOS, 0x0, 0b1111 },
    
    { "NIC400_SW_1_S_FN_MOD", REGPOKER_READ32, NIC400_SW_1_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_SW_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_SW_1_S_FN_MOD, 0x0, 0b11 },
    {" NIC400_PERIPHERAL_ID4", REGPOKER_READ32, NIC400_PERIPHERAL_ID4, 0x04, 0xff },
    { "NIC400_PERIPHERAL_ID5", REGPOKER_READ32, NIC400_PERIPHERAL_ID5, 0x00, 0xff },
    { "NIC400_PERIPHERAL_ID6", REGPOKER_READ32, NIC400_PERIPHERAL_ID6, 0x00, 0xff },
    { "NIC400_PERIPHERAL_ID7", REGPOKER_READ32, NIC400_PERIPHERAL_ID7, 0x00, 0xff },
    { "NIC400_PERIPHERAL_ID0", REGPOKER_READ32, NIC400_PERIPHERAL_ID0, 0x00, 0xff },
    { "NIC400_PERIPHERAL_ID1", REGPOKER_READ32, NIC400_PERIPHERAL_ID1, 0xB4, 0xff },
    { "NIC400_PERIPHERAL_ID2", REGPOKER_READ32, NIC400_PERIPHERAL_ID2, 0x5B, 0xff },
    { "NIC400_PERIPHERAL_ID3", REGPOKER_READ32, NIC400_PERIPHERAL_ID3, 0x00, 0xff },
    {}
};

struct regpoker_checker interconnect_axi32_right_rw_checker[] = {
    { "NIC400_GSPI_1_S_RQOS", REGPOKER_READ32, NIC400_GSPI_1_S_RQOS, 0x0, 0b1111 },
    { "NIC400_GSPI_1_S_RQOS", REGPOKER_WRITE32, NIC400_GSPI_1_S_RQOS, 0x0, 0b1111 },

    { "NIC400_GSPI_1_S_WQOS", REGPOKER_READ32, NIC400_GSPI_1_S_WQOS, 0x0, 0b1111 },
    { "NIC400_GSPI_1_S_WQOS", REGPOKER_WRITE32, NIC400_GSPI_1_S_WQOS, 0x0, 0b1111 },

    { "NIC400_GSPI_1_S_FN_MOD", REGPOKER_READ32, NIC400_GSPI_1_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_GSPI_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_GSPI_1_S_FN_MOD, 0x0, 0b11 },


    { "NIC400_ETH_1_S_RQOS", REGPOKER_READ32, NIC400_ETH_1_S_RQOS, 0x0, 0b1111 },
    { "NIC400_ETH_1_S_RQOS", REGPOKER_WRITE32, NIC400_ETH_1_S_RQOS, 0x0, 0b1111 },

    { "NIC400_ETH_1_S_WQOS", REGPOKER_READ32, NIC400_ETH_1_S_WQOS, 0x0, 0b1111 },
    { "NIC400_ETH_1_S_WQOS", REGPOKER_WRITE32, NIC400_ETH_1_S_WQOS, 0x0, 0b1111 },

    { "NIC400_ETH_1_S_FN_MOD", REGPOKER_READ32, NIC400_ETH_1_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_ETH_1_S_FN_MOD", REGPOKER_WRITE32, NIC400_ETH_1_S_FN_MOD, 0x0, 0b11 },
    {}
};

struct regpoker_checker interconnect_axi32_left_rw_checker[] = {
    { "NIC400_GSPI_0_S_RQOS", REGPOKER_READ32, NIC400_GSPI_0_S_RQOS, 0x0, 0b1111 },
    { "NIC400_GSPI_0_S_RQOS", REGPOKER_WRITE32, NIC400_GSPI_0_S_RQOS, 0x0, 0b1111 },

    { "NIC400_GSPI_0_S_WQOS", REGPOKER_READ32, NIC400_GSPI_0_S_WQOS, 0x0, 0b1111 },
    { "NIC400_GSPI_0_S_WQOS", REGPOKER_WRITE32, NIC400_GSPI_0_S_WQOS, 0x0, 0b1111 },

    { "NIC400_GSPI_0_S_FN_MOD", REGPOKER_READ32, NIC400_GSPI_0_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_GSPI_0_S_FN_MOD", REGPOKER_WRITE32, NIC400_GSPI_0_S_FN_MOD, 0x0, 0b11 },


    { "NIC400_ETH_0_S_RQOS", REGPOKER_READ32, NIC400_ETH_0_S_RQOS, 0x0, 0b1111 },
    { "NIC400_ETH_0_S_RQOS", REGPOKER_WRITE32, NIC400_ETH_0_S_RQOS, 0x0, 0b1111 },

    { "NIC400_ETH_0_S_WQOS", REGPOKER_READ32, NIC400_ETH_0_S_WQOS, 0x0, 0b1111 },
    { "NIC400_ETH_0_S_WQOS", REGPOKER_WRITE32, NIC400_ETH_0_S_WQOS, 0x0, 0b1111 },

    { "NIC400_ETH_0_S_FN_MOD", REGPOKER_READ32, NIC400_ETH_0_S_FN_MOD, 0x0, 0b11 },
    { "NIC400_ETH_0_S_FN_MOD", REGPOKER_WRITE32, NIC400_ETH_0_S_FN_MOD, 0x0, 0b11 },
    {}
};




struct regpoker_checker scrb_rw_checker[] = {
    {"SCRB_REG_CR  ", REGPOKER_WRITE32  , SCRB_REG_CR         ,     0x0           ,  0x00000038 }, 
    {"SCRB_REG_IMR  ", REGPOKER_WRITE32  , SCRB_REG_IMR       ,     0x0           ,  0x00000003 }, 
    { } // Missing      
    };



struct rumboot_pokerlist { 
    const  struct regpoker_checker *checker; 
    uintptr_t base;
};

static struct rumboot_pokerlist the_big_list[] = {

    {greth0_read_checker,       GRETH_0_BASE},
    {greth0_rw_checker,         GRETH_0_BASE},

    {greth1_read_checker,       GRETH_1_BASE},
    {greth1_rw_checker,         GRETH_1_BASE},

    {gpio_checker,              GPIO_0_BASE },
    {gpio_checker,              GPIO_1_BASE },
    {gpio_rw_checker,           GPIO_0_BASE },
    {gpio_rw_checker,           GPIO_1_BASE },

    {sdio_read_checker,         SDIO_0_BASE},
    {sdio_read_checker,         SDIO_1_BASE},
    {spi_checker,               GSPI_0_BASE}, 
    {spi_checker,               GSPI_1_BASE},

    {mkio_checker,               MKIO0_BASE},
    {mkio_checker,               MKIO1_BASE},
    {mkio_checker,               MKIO2_BASE},
    {mkio_checker,               MKIO3_BASE},

    {uart_checker,               UART0_BASE},
    {uart_checker,               UART1_BASE},

    {hscb_read_checker,          HSCB0_BASE},
    {hscb_read_checker,          HSCB1_BASE},
    {hscb_read_checker,          HSCB2_BASE},
    {hscb_read_checker,          HSCB3_BASE},

    {com_port_checker,          COM0_BASE   },
    {com_port_checker,          COM1_BASE   },
    {com_port_rw_checker,       COM0_BASE   },
    {com_port_rw_checker,       COM1_BASE   },

    {axi_dma_read_checker,      RCM_NDMA_BASE},
    {axi_dma_rw_checker,        RCM_NDMA_BASE},

    {interconnect_axi64_rw_checker, AXI64_CTRL_BASE},
    {interconnect_axi32_right_rw_checker, AXI32_RIGHT_CTRL_BASE},
    {interconnect_axi32_left_rw_checker, AXI32_LEFT_CTRL_BASE},
    {scrb_rw_checker, SCRB_BASE},

    {}
};


static int is_readonly(const  struct regpoker_checker *chk){
    while (chk->tp != REGPOKER_EOL) {
        if ((chk->tp == REGPOKER_WRITE8) ||
        (chk->tp == REGPOKER_WRITE16) ||
        (chk->tp == REGPOKER_WRITE32) ) {
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
        rumboot_printf("Performing %s on %x ... \n", check? "read check" : "write check", pos->base);
        int ret = rumboot_regpoker_check_array(pos->checker, pos->base);
        rumboot_printf("%s (%d)\n", ret ? "FAILED" : "OK", ret);
        pos++;
        total += ret;
    }
    return total;
}



int regpoker_mem_check(){

    rumboot_printf("Getting ADDRESSES\n");

    void *im1_addr_ptr = rumboot_malloc_from_named_heap("IM1", 4);
    void *im2_addr_ptr = rumboot_malloc_from_named_heap("IM2", 4);
    void *im3_addr_ptr = rumboot_malloc_from_named_heap("IM3", 4);
    void *sram_addr_ptr = rumboot_malloc_from_named_heap("SRAM", 4);

    uintptr_t im1_addr_changed_ptr = rumboot_virt_to_dma(im1_addr_ptr);
    uintptr_t im2_addr_changed_ptr = rumboot_virt_to_dma(im2_addr_ptr);
    uintptr_t im3_addr_changed_ptr = rumboot_virt_to_dma(im3_addr_ptr);

    uint32_t errors = 0 ;

    uint32_t pp1 = (uintptr_t) im1_addr_ptr;
        rumboot_printf("addr IM1 0x%x traslates to 0x%x\n", 
                pp1, rumboot_virt_to_dma(pp1)
        );

    uint32_t pp2 = (uintptr_t) im2_addr_ptr;
        rumboot_printf("addr IM2 0x%x traslates to 0x%x\n", 
                pp2, rumboot_virt_to_dma(pp2)
        );

    uint32_t pp3 = (uintptr_t) im3_addr_ptr;
        rumboot_printf("addr IM3 0x%x traslates to 0x%x\n", 
                pp3, rumboot_virt_to_dma(pp3)
        );

    uint32_t pp4 = (uintptr_t) sram_addr_ptr;
       rumboot_printf("addr SRAM 0x%x traslates to 0x%x\n", 
               pp4, rumboot_virt_to_dma(pp4)
       );

    struct regpoker_checker im_direct_rw_checker[] = {
        {"mem_cell      ", REGPOKER_WRITE32, 0      ,      0x0      ,   0xFFFFFFFF         },
        {/* Sentinel */}
    };

    rumboot_printf("Checking EMI\n");

	errors += memtest(im1_addr_ptr, 8);
	errors += memtest(im2_addr_ptr, 8);
	errors += memtest(im3_addr_ptr, 8);
	errors += memtest(im1_addr_changed_ptr>>2, 8);
	errors += memtest(im2_addr_changed_ptr>>2, 8);
	errors += memtest(im3_addr_changed_ptr>>2, 8);
	errors += memtest(sram_addr_ptr, 4);

    return errors;
}



int main(int argc, char** argv)
{
    iowrite32(0x0, 0xc0028400); /* Revert gpio dir value to stock */
    int errors = 0;
    errors += regpoker_exec_list(the_big_list, 1); /* read-only check */
    errors += regpoker_exec_list(the_big_list, 0); /* read and write checks */
    errors += regpoker_mem_check(); /* read and write checks */
    rumboot_printf("Done testing\n");
    return errors;

}//