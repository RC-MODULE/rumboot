/*
 * gbit_greth.h
 *
 *  Created on: Jul 11, 2018
 *      Author: v.gulyaev
 */

#ifndef PLATFORM_DEVICES_GBIT_GRETH_H
#define PLATFORM_DEVICES_GBIT_GRETH_H


#include <stdint.h>
#include <stdbool.h>

#include <platform/devices.h>
#include <platform/regs/regs_greth.h>
#include <platform/regs/regs_plb4ahb.h>
#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/test_assert.h>

#define ETH_DEBUG

//Transmit descriptor fields
#define GRETH_TX_DESCR_UC                            20
#define GRETH_TX_DESCR_TC                            19
#define GRETH_TX_DESCR_IC                            18
#define GRETH_TX_DESCR_MO                            17
#define GRETH_TX_DESCR_LC                            16
#define GRETH_TX_DESCR_AL                            15
#define GRETH_TX_DESCR_UE                            14
#define GRETH_TX_DESCR_IE                            13
#define GRETH_TX_DESCR_WR                            12
#define GRETH_TX_DESCR_EN                            11
//bits 10:0 LENGTH

//Receive descriptor fields
#define GRETH_RX_DESCR_MC                            26
#define GRETH_RX_DESCR_IF                            25
#define GRETH_RX_DESCR_TR                            24
#define GRETH_RX_DESCR_TD                            23
#define GRETH_RX_DESCR_UR                            22
#define GRETH_RX_DESCR_UD                            21
#define GRETH_RX_DESCR_IR                            20
#define GRETH_RX_DESCR_ID                            19
#define GRETH_RX_DESCR_LE                            18
#define GRETH_RX_DESCR_OE                            17
#define GRETH_RX_DESCR_CE                            16
#define GRETH_RX_DESCR_FT                            15
#define GRETH_RX_DESCR_AE                            14
#define GRETH_RX_DESCR_IE                            13
#define GRETH_RX_DESCR_WR                            12
#define GRETH_RX_DESCR_EN                            11
//bits 10:0 LENGTH

//ETH PHY regs from environment
#define ETH_PHY_CTRL       0
#define ETH_PHY_STATUS     1
#define ETH_PHY_ID0        2
#define ETH_PHY_ID1        3
#define ETH_PHY_ANEGADV    4
#define ETH_PHY_ANEGLP     5
#define ETH_PHY_ANEGXP     6
#define ETH_PHY_ANEGNPTX   7
#define ETH_PHY_ANEGNPLP   8
#define ETH_PHY_MSTSLVCTRL 9
#define ETH_PHY_MSTSLVSTAT 10
#define ETH_PHY_EXTSTATUS  15

#define ETH_PHY_CTRL_DEFAULT       0x7100
#define ETH_PHY_STATUS_DEFAULT     0x7E29
#define ETH_PHY_ID0_DEFAULT        0xBBCD
#define ETH_PHY_ID1_DEFAULT        0x9C83
#define ETH_PHY_ANEGADV_DEFAULT    0x01E1
#define ETH_PHY_ANEGLP_DEFAULT     0x01E1
#define ETH_PHY_ANEGXP_DEFAULT     0x00
#define ETH_PHY_ANEGNPTX_DEFAULT   0x2001
#define ETH_PHY_ANEGNPLP_DEFAULT   0x2001
#define ETH_PHY_MSTSLVCTRL_DEFAULT 0x00
#define ETH_PHY_MSTSLVSTAT_DEFAULT 0x7000
#define ETH_PHY_EXTSTATUS_DEFAULT  0x0


typedef enum greth_speed
{
    GRETH_SPEED_1GB,
    GRETH_SPEED_100MB,
    GRETH_SPEED_10MB
}greth_speed_t;

typedef struct greth_mac_str
{
    uint16_t mac_msb;
    uint32_t mac_lsb;
}greth_mac_t;

typedef struct greth_rx_tx_descr
{
    uint32_t options;
    uint32_t start_addr;
}greth_descr_t;

typedef struct greth_ctrl_struct
{
    bool mdio_int_enable;
    bool edcl_disable;
    bool ram_debug_enable;
    bool disable_duplex_detection;
    bool multicast_enable;
    bool phy_int_enable;
    bool burstmode_enable;
    greth_speed_t speed;
    bool promiscuous_mode;
    bool fullduplex_enable;
    bool receiver_int_enable;
    bool transmitter_int_enable;
    bool receiver_enable;
    bool transmitter_enable;
}greth_ctrl_struct_t;

//length in a single descriptor
//set not 1514(by spec) because this value isn't multiply of 32
#define GRETH_MAX_DESC_DATA_LENGTH    1504
//maximum count of descriptors (1K border = 512)
#define GRETH_MAX_DESCRIPTORS_COUNT   512

#define GRETH_TIMEOUT                 10000


bool greth_mem_copy(uint32_t base_addr, void volatile * src, void volatile * dst, uint32_t length);
bool mdio_write( uint32_t base_addr, uint8_t phy_addr, uint8_t reg_addr, uint16_t data);
uint16_t mdio_read( uint32_t base_addr, uint8_t phy_addr, uint8_t reg_addr);

#endif /* PLATFORM_DEVICES_GBIT_GRETH_H */
