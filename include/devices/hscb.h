/*
 * hscb.h
 *
 *  Created on: Jul 19, 2018
 *      Author: r.galiulin
 */

#ifndef DEVICES_HSCB_H
#define DEVICES_HSCB_H

#include <regs/regs_hscb.h>

/**
 * \defgroup devices_hscb HSCB
 * \ingroup devices
 *
 * Change endianness in 4-byte word function
 * - data_in:   input 4-byte data word
 * Function returns data word with reversed byte order

 *
 *
 * \addtogroup devices_hscb
 *
 * @{
 */

/**
 * hscb_descr_struct: Structure contains receive descriptor parameters which use to transmit
 * start_address: 32-bit start address
 * length: Data block length (max 64M-1)
 * act: descr type
 * last: is last packet descriptor
 * ie: interrupt at finish execution
 * err: link fail
 * valid: data is correct
 */
typedef struct hscb_descr_struct
{
    uint32_t start_address;
    uint32_t length;
    uint8_t  act;
    bool     act0;
    bool     ie;
    bool     err;
    bool     valid;
}hscb_descr_struct_t;

/**
 * hscb_cfg: Structure contains HSCB settings
 * en_hscb:     SW interface ON
 * tx_endian:   TX channel bytes order (1-Big Endian; 0-Little Endian)
 * rx_endian:   RX channel bytes order (1-Big Endian; 0-Little Endian)
 * time_mode:   Time-Code mode 0-receiver; 1-transmitter
 * loop_back:   1-loopback is ON
 * en_rmap:     RMAP handling
 * en_trim_clk: Frequency auto-tune
 * rx_fix_en:   Recover receiver clock by incoming data
 */
typedef struct hscb_cfg
{
    bool en_hscb;
    bool tx_endian;
    bool rx_endian;
    bool time_mode;
    bool loop_back;
    bool en_rmap;
    bool en_trim_clk;
    bool rx_fix_en;
}hscb_cfg_t;

/**
 * hscb_trans_clock_cfg: Structure contains HSCB trans_clock settings
 * freq:                 Transmitter divider factor
 * init_freq:            Transmitter divider factor at rate 10Mbit/s
 * limit:                Minimal transmit bit rate
 * time_64:              6.4 us counter
 * time_128              12.8 us counter
 * silence_time:         Max time between symbols without link fail
 */
typedef struct hscb_timings_cfg
{
    uint8_t  freq;
    uint8_t  init_freq;
    uint8_t  limit;
    uint16_t time_64;
    uint16_t time_128;
    uint8_t  silence_time;
}hscb_timings_cfg_t;

/**
 * \brief Change endianness in 4-byte word function
 * \param[in]   data_in input 4-byte data word
 * \return              Function returns data word with reversed byte order
 */

uint32_t hscb_change_endian (uint32_t data_in);

/**
 * \brief Convert two 4-byte data words to 8 1-byte words function
 * \param[in]   data_in     input 4-byte data array pointer
 * \param[out]  data_out    result 1-byte data array pointer
 * \param[in]   len         length of data to be processed in bytes
 */
void hscb_convert_to_bytes (uint32_t* data_in, uint8_t* data_out, uint32_t len);

/**
 * \brief Set descriptor data function
 * \param[in]   sys_addr    address of descriptor
 * \param[in]   len         length of data array
 * \param[in]   desc_addr   address of data array (address in descriptor)
 * \param[in]   data_in     byte-data array for write to memory pointer
 * \param[in]   act0        descriptors flag pointer
 * \param[in]   interrupt   descriptors flag pointer
 * \param[in]   end         descriptors flag pointer
 * \param[in]   valid       descriptors flag pointer
 * \return                  Function returns address of next descriptor
 */
uint32_t hscb_set_desc (uint32_t sys_addr, uint32_t len, uint32_t desc_addr, uint8_t* data_in, bool act0, bool interrupt, bool valid, bool endian);

/**
 * \brief Get descriptor data function
 * \param[in]   sys_addr    address of descriptor
 * \param[in]   data_out    accepted byte-data array pointer
 * \param[in]   len         length of data array pointer
 * \param[in]   act0        descriptors flag pointer
 * \param[in]   interrupt   descriptors flag pointer
 * \param[in]   end         descriptors flag pointer
 * \param[in]   valid       descriptors flag pointer
 * \return                  In case COMPL descriptor
 *
 *      function returns address of next descriptor
 *
 *      function returns 0xffffffff as impossible address
 */
uint32_t hscb_get_desc (uint32_t sys_addr, uint8_t* data_out, uint32_t* len,  bool* act0, bool* interrupt, bool* end, bool* valid, bool endian);


/**
 * \brief Set descriptor in memory
 * \param[in] sys_addr address in memory for placing descriptor
 * \param[in] pointer on read data descriptor
 */
void hscb_set_descr_in_mem(uint32_t sys_addr, hscb_descr_struct_t* descr);

bool hscb_sw_rst(uint32_t base_addr);
void hscb_set_config(uint32_t base_addr, hscb_cfg_t* cfg);
void hscb_get_config(uint32_t base_addr, hscb_cfg_t* cfg);
uint32_t hscb_get_status(uint32_t base_addr);
void hscb_set_irq_mask(uint32_t base_addr, uint32_t mask);
void hscb_set_timings(uint32_t base_addr, hscb_timings_cfg_t* cfg);
void hscb_get_timings(uint32_t base_addr, hscb_timings_cfg_t* cfg);
bool hscb_adma_sw_rst(uint32_t base_addr);
uint32_t hscb_get_adma_ch_status(uint32_t base_addr);
void hscb_set_rdma_irq_mask(uint32_t base_addr, uint32_t mask);
void hscb_set_wdma_irq_mask(uint32_t base_addr, uint32_t mask);
uint32_t hscb_get_rdma_status(uint32_t base_addr);
uint32_t hscb_get_wdma_status(uint32_t base_addr);
void hscb_set_rdma_tbl_size(uint32_t base_addr, uint32_t size);
uint32_t hscb_get_rdma_tbl_size(uint32_t base_addr);
void hscb_set_wdma_tbl_size(uint32_t base_addr, uint32_t size);
uint32_t hscb_get_wdma_tbl_size(uint32_t base_addr);
void hscb_set_rdma_sys_addr(uint32_t base_addr, uint32_t descr_addr);
uint32_t hscb_get_rdma_sys_addr(uint32_t base_addr);
void hscb_set_wdma_sys_addr(uint32_t base_addr, uint32_t descr_addr);
uint32_t hscb_get_wdma_sys_addr(uint32_t base_addr);


#define HSCB_SW_RESET_TIMEOUT   100
/**
 * @}
 */

#endif /* DEVICES_HSCB_H */
