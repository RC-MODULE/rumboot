#ifndef __REGS_MGETH__H__
#define __REGS_MGETH__H__

// Common registers
#define MGETH_ID              0x0000
#define MGETH_VERSION         0x0004
#define MGETH_SW_RST          0x0008
#define MGETH_GLOBAL_STATUS   0x000C

// MGETH registers
#define MGETH_STATUS          0x0010
#define MGETH_IRQ_MASK        0x0018
#define MGETH_CONTROL         0x001C
#define MGETH_LEN_MASK_CH0    0x0020
#define MGETH_LEN_MASK_CHd       0x4
#define MGETH_TX0_DELAY_TIMER 0x0030
#define MGETH_TXd_DELAY_TIMER    0x4
#define HD_SGMII_MODE         0x0040

// Receive channel registers (relative begin of channel)
#define RXx_ETH_MASK_VALUE_0 -0x0100
#define RXx_ETH_MASK_VALUE_d     0x4
#define RXx_ETH_MASK_VALUE_c      32
#define RXx_ETH_MASK_ACTIV_0 -0x0080
#define RXx_ETH_MASK_ACTIV_d     0x4
#define RXx_ETH_MASK_ACTIV_c      32

// Receive channels
#define MGETH_RECV_CH_0       0x0300
#define MGETH_RECV_CH_d        0x200

// Transmit channels
#define MGETH_SEND_CH_0       0x0A00
#define MGETH_SEND_CH_d        0x100

// Setup registers of DMA channel (relative begin of channel)
#define ENABLE_x                0x00
#define SETTINGS_x              0x10
#define IRQ_MASK_x              0x14
#define STATUS_x                0x18
#define DESC_ADDR_x             0x20

#endif // __REGS_MGETH__H__
