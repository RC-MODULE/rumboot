/*
 * gbit_greth.h
 *
 *  Created on: Jul 11, 2018
 *      Author: v.gulyaev
 */

#ifndef PLATFORM_DEVICES_GRETH_H
#define PLATFORM_DEVICES_GRETH_H


#include <stdint.h>
#include <stdbool.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/platform.h>

#include <platform/devices.h>
#include <regs/regs_greth.h>
#include <arch/common_macros.h>
#include <arch/test_macro.h>
#include <platform/devices.h>

//#define ETH_DEBUG

/**
 *
 * \defgroup greth Gaisler GRETH
 * \ingroup devices
 *
 * \code{.c}
 * #include <devices/greth.h>
 * \endcode
 *
 * \addtogroup devices_spi
 *
 *  @{
 */

/**
 * greth_rx_tx_descr: Structure contains receive/transmit descriptor parameters
 * options: receive/transmit descriptor control information
 * start_addr: Pointer to the buffer area from where the packet data will be loaded
 */
typedef struct greth_rx_tx_descr
{
    uint32_t options;
    uint32_t start_addr;
}greth_descr_t;

/**
 * Transmit descriptor control fields offsets
 *
 * Attempt Limit Error (AL) - The packet was not transmitted because the maximum number of attempts was reached.
 *
 * Underrun Error (UE) - The packet was incorrectly transmitted due to a FIFO underrun error.
 *
 * Interrupt Enable (IE) - Enable Interrupts. An interrupt will be generated when the packet from this
 * descriptor has been sent provided that the transmitter interrupt enable bit in the control register is set.
 * The interrupt is generated regardless if the packet was transmitted successfully or if it terminated
 * with an error.
 *
 * Wrap (WR) - Set to one to make the descriptor pointer wrap to zero after this descriptor has been
 * used. If this bit is not set the pointer will increment by 8. The pointer automatically wraps to zero
 * when the 1 kB boundary of the descriptor table is reached.
 *
 * Enable (EN) - Set to one to enable the descriptor. Should always be set last of all the descriptor
 * fields.
 *
 * LENGTH - The number of bytes to be transmitted.
 */
#define GRETH_TX_DESCR_AL                            15
#define GRETH_TX_DESCR_UE                            14
#define GRETH_TX_DESCR_IE                            13
#define GRETH_TX_DESCR_WR                            12
#define GRETH_TX_DESCR_EN                            11
//bits 10:0 LENGTH

/**
 * Receive descriptor control fields offsets
 *
 * Multicast address (MC) - The destination address of the packet was a multicast address (not broad-
 * cast).
 *
 * Length error (LE) - The length/type field of the packet did not match the actual number of received
 * bytes.
 *
 * Overrun error (OE) - The frame was incorrectly received due to a FIFO overrun.
 *
 * CRC error (CE) - A CRC error was detected in this frame.
 *
 * Frame too long (FT) - A frame larger than the maximum size was received. The excessive part
 * was truncated.
 *
 * Alignment error (AE) - An odd number of nibbles were received.
 *
 * Interrupt Enable (IE) - Enable Interrupts. An interrupt will be generated when a packet has been
 * received to this descriptor provided that the receiver interrupt enable bit in the control register is set.
 * The interrupt is generated regardless if the packet was received successfully or if it terminated with
 * an error.
 *
 * Wrap (WR) - Set to one to make the descriptor pointer wrap to zero after this descriptor has been
 * used. If this bit is not set the pointer will increment by 8. The pointer automatically wraps to zero
 * when the 1 kB boundary of the descriptor table is reached.
 *
 * Enable (EN) - Set to one to enable the descriptor. Should always be set last of all the descriptor
 * fields.
 *
 * LENGTH - The number of bytes received to this descriptor.
 */
#define GRETH_RX_DESCR_MC                            26
#define GRETH_RX_DESCR_LE                            18
#define GRETH_RX_DESCR_OE                            17
#define GRETH_RX_DESCR_CE                            16
#define GRETH_RX_DESCR_FT                            15
#define GRETH_RX_DESCR_AE                            14
#define GRETH_RX_DESCR_IE                            13
#define GRETH_RX_DESCR_WR                            12
#define GRETH_RX_DESCR_EN                            11
//bits 10:0 LENGTH

/**
 * ETH PHY registers offsets for MDIO check
 */
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

/**
 * ETH PHY registers default values for MDIO check
 */
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

/**
 * ETH PHY address for MDIO check
 */
#define ETH_PHY_ADDR    0

/**
 * Macro for using in EDCL check
 */
#define EDCL_WR     true
#define EDCL_RD     false
#ifdef RUMBOOT_SOC_O32T
    #define EDCLADDRL0  1
    #define EDCLADDRL1  2
    
    /**
    * Default EDCL MAC addresses and IP at OI-10 platform
    */
    #define EDCLMAC_MSB     0xEC17 
    #define EDCLMAC_LSB     0x66EDC100 
    #define EDCLIP      0xC0A80320
    #define EDCLIP0     (EDCLIP | EDCLADDRL0)
    #define EDCLIP1     (EDCLIP | EDCLADDRL1)
#else
    #define EDCLADDRL0  0
    #define EDCLADDRL1  1
    
    /**
    * Default EDCL MAC addresses and IP at OI-10 platform
    */
    #define EDCLMAC_MSB     0xEC17
    #define EDCLMAC_LSB     0x660E1000
    #define EDCLIP      0xC0A80130
    #define EDCLIP0     (EDCLIP | EDCLADDRL0)
    #define EDCLIP1     (EDCLIP | EDCLADDRL1)
#endif

/**
 * Defining GRETH index on OI10 platform by base address
 */
#define GET_GRETH_IDX(base_addr)  (base_addr==GRETH_0_BASE ? 0 : 1)

/**
 * greth_speed_t: Enum for GRETH speed setting
 */
typedef enum greth_speed
{
    GRETH_SPEED_100MB,
    GRETH_SPEED_10MB
}greth_speed_t;

/**
 * greth_mac_str: Structure contains GRETH MAC address
 * mac_msb: The two most significant bytes of the MAC Address
 * mac_lsb: The four least significant bytes of the MAC Address
 */
typedef struct greth_mac_str
{
    uint16_t mac_msb;
    uint32_t mac_lsb;
}greth_mac_t;

/**
 * greth_ctrl_struct: Structure contains GRETH configuration parameters
 *
 * mdio_int_enable: Set to one when the core supports mdio interrupts. Read only.
 *
 * edcl_disable: Set to one to disable the EDCL and zero to enable it. Reset value taken from
 * the ethi.edcldisable signal. Only available if the EDCL hardware is present in the core.
 *
 * ram_debug_enable: Set to one to enable the RAM debug mode. Reset value: ‘0’. Only available
 * if the VHDL generic ramdebug is nonzero.
 *
 * disable_duplex_detection: Disable the EDCL speed/duplex detection FSM. If the FSM cannot
 * complete the detection the MDIO interface will be locked in busy mode. If software needs to access
 * the MDIO the FSM can be disabled here and as soon as the MDIO busy bit is 0 the interface is available
 * Note that the FSM cannot be reenabled again.
 *
 * multicast_enable: Enable reception of multicast addresses. Reset value: ‘0’.
 *
 * phy_int_enable: Enables interrupts for detected PHY status changes.
 *
 * speed: Sets the current speed mode. 0 = 10 Mbit, 1 = 100 Mbit. Only used in RMII mode (rmii = 1).
 * A default value is automatically read from the PHY after reset. Reset value: ‘1’.
 *
 * promiscuous_mode: If set, the GRETH operates in promiscuous mode which means it will
 * receive all packets regardless of the destination address. Reset value: ‘0’.
 *
 * fullduplex_enable: If set, the GRETH operates in full-duplex mode otherwise it operates in half-
 * duplex. Reset value: ‘0’.
 *
 * receiver_int_enable: Enable Receiver Interrupts. An interrupt will be generated each time a
 * packet is received when this bit is set. The interrupt is generated regardless if the packet was
 * received successfully or if it terminated with an error. Reset value: ‘0’.
 *
 * transmitter_int_enable: Enable Transmitter Interrupts. An interrupt will be generated each time a
 * packet is transmitted when this bit is set. The interrupt is generated regardless if the packet was
 * transmitted successfully or if it terminated with an error. Reset value: ‘0’.
 *
 * receiver_enable: Receive enable (RE) - Should be written with a one each time new descriptors are enabled. As long
 * as this bit is one the GRETH will read new descriptors and as soon as it encounters a disabled
 * descriptor it will stop until RE is set again. This bit should be written with a one after the new
 * descriptors have been enabled. Reset value: ‘0’.
 *
 * transmitter_enable: Transmit enable (TE) - Should be written with a one each time new descriptors are enabled. As long
 * as this bit is one the GRETH will read new descriptors and as soon as it encounters a disabled
 * descriptor it will stop until TE is set again. This bit should be written with a one after the new
 * descriptors have been enabled. Reset value: ‘0’.
 */
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

/**
 * edcl_test_data_struct: Structure for prepare data for EDCL access
 */
typedef struct edcl_test_data_struct
{
    greth_mac_t  src_mac;
    uint32_t     src_ip;
    greth_mac_t  dst_mac;
    uint32_t     dst_ip;
    uint32_t     edcl_seq_number;
    uint32_t     addr;
    uint8_t*     data;
    uint32_t     len;
    bool         wrrd;
}edcl_test_data_struct_t;

/**
 * Length in a single descriptor
 * set not 1514(by spec) because this value isn't multiply of 32
 */
#define GRETH_MAX_DESC_DATA_LENGTH    1504

/**
 * maximum count of descriptors (1K border = 512)
 */
#define GRETH_MAX_DESCRIPTORS_COUNT   512

/**
 * Timeout for waiting GRETH interrupt or changing STATUS register
 */
#define GRETH_TIMEOUT                 1000

/**
 * \brief Resets the GRETH core
 * \param base_addr: base address of GRETH
 */
bool greth_reset(uint32_t base_addr);

/**
 * \brief Copy data from source memory region to destination memory region by external loopback
 * \param base_addr: base address of GRETH
 * \param src: pointer to source data region
 * \param dst: pointer to destination memory region
 */
bool greth_mem_copy(uint32_t base_addr, void volatile * src, void volatile * dst, uint32_t length);

/**
 * \brief Write data to external PHY IC register
 * \param base_addr: base address of GRETH
 * \param phy_addr: address of PHY IC
 * \param reg_addr: target register address
 * \param data: data for writing
 */
bool greth_mdio_write( uint32_t base_addr, uint8_t phy_addr, uint8_t reg_addr, uint16_t data);

/**
 * \brief Read data from external PHY IC register
 * \param base_addr: base address of GRETH
 * \param phy_addr: address of PHY IC
 * \param reg_addr: target register address
 * \return data from target register
 */
uint16_t greth_mdio_read( uint32_t base_addr, uint8_t phy_addr, uint8_t reg_addr);

/**
 * \brief Set GRETH configuration for receive
 * \param base_addr: base address of GRETH
 * \param dst: pointer for storing received data
 * \param length: size of received data
 * \param rx_descriptor_data_: pointer to receive descriptor
 * \param gr_mac: MAC address for setting
 * \return If receiving data is OK - true, else - false
 */
bool greth_configure_for_receive( uint32_t base_addr, void volatile * const dst, uint32_t length, greth_descr_t* rx_descriptor_data_, greth_mac_t* gr_mac);

/**
 * \brief Enables GRETH to receive data
 * \param base_addr: base address of GRETH
 * \param rcv_int_ena: enable inerrupt at receiving data
 */
void greth_start_receive(uint32_t base_addr, bool rcv_int_ena);

/**
 * \brief Enables GRETH to receive and transmit data
 * \param base_addr: base address of GRETH
 * \param rcv_int_ena: enable inerrupt at receiving data
 */
void greth_start_receive_transmit(uint32_t base_addr, bool rcv_int_ena);

/**
 * \brief Waiting receiving data by polling STATUS register
 * \param base_addr: base address of GRETH
 * \return If data were received - true, else - false
 */
bool greth_wait_receive(uint32_t base_addr);

/**
 * \brief Waiting receiving data by polling interrupt handler flag
 * \param base_addr: base address of GRETH
 * \param eth_irq_handled_flag: interrupt handler flag
 * \return If data were received - true, else - false
 */
bool greth_wait_receive_irq(uint32_t base_addr, uint32_t volatile* eth_irq_handled_flag);


/**
 * \brief Waiting RE (receive error) interrupt
 * \param base_addr: base address of GRETH
 * \param eth_irq_handled_flag: interrupt handler flag
 * \return If happened receive error - true, else - false
 */
bool greth_wait_receive_err_irq(uint32_t base_addr, uint32_t volatile* eth_irq_handled_flag);

/**
 * \brief Set GRETH configuration for transmit
 * \param base_addr: base address of GRETH
 * \param src: pointer for transmitting data
 * \param length: size of received data
 * \param tx_descriptor_data_: pointer to transmit descriptor
 * \param gr_mac: MAC address for setting
 * \return If receiving data is OK - true, else - false
 */
bool greth_configure_for_transmit( uint32_t base_addr, void volatile * const src, uint32_t length, greth_descr_t* tx_descriptor_data_, greth_mac_t* gr_mac);

/**
 * \brief Enables GRETH to transmit data
 * \param base_addr: base address of GRETH
 */
void greth_start_transmit(uint32_t base_addr);

/**
 * \brief Clearing STATUS register
 * \param base_addr: base address of GRETH
 * \param mask: mask for clearing bits
 */
void greth_clear_status_bits(uint32_t base_addr, uint32_t mask);

/**
 * \brief Getting data from STATUS register
 * \param base_addr: base address of GRETH
 * \return data from STATUS register
 */
uint32_t greth_get_status(uint32_t base_addr);

/**
 * \brief Runs transaction for reading data by EDCL interface
 * \param base_addr: base address of GRETH
 */
void greth_start_edcl_transfer(uint32_t base_addr);

/**
 * @}
 */
#endif /* PLATFORM_DEVICES_GRETH_H */
