/*
 * hscb.h
 *
 *  Created on: Jul 19, 2018
 *      Author: r.galiulin
 */

#ifndef DEVICES_HSCB_H
#define DEVICES_HSCB_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/arch/ppc/test_macro.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/regs/regs_hscb.h>

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
    uint32_t    start_address;
    uint32_t    length;
    hscb_act_t  act;
    hscb_act0_t act0;
    bool        ie;
    bool        err;
    bool        valid;
}hscb_descr_struct_t;

/**
 * hscb_packed_descr_struct: Structure contains descriptor parameters which are used to transmit
 * start_address: 32-bit start address
 * length: Data block length (max 64M-1)
 * act: descr type
 * last: is last packet descriptor
 * ie: interrupt at finish execution
 * err: link fail
 * valid: data is correct
 */
typedef struct hscb_packed_descr_struct
{
    uint32_t    length_attr;
    uint32_t    start_address;
}hscb_packed_descr_struct_t;

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
 * hscb_rwdma_settings
 * rw_desc_int:       Interrupt mask "Descriptor execution is finished"
 * rw_desc_end:       Interrupt mask "Finished stop R(W)DMA execution"
 * rw_bad_desc:       Interrupt mask "Descriptor of rd chan tagged as completed"
 * rw_axi_error:      Interrupt mask "Error on system bus"
 * rw_buf_full:       Interrupt mask "RD/WR buffer is full"
 * rw_buf_empty:      Interrupt mask "RD/WR buffer is empty"
 * en_rwdma:          RD/WR channel is ON
 * en_rwdma_desc_tbl: Accept external desc table (must be true)
 * rwdma_long_len:    Extend Length field through Custom field
 * cansel_rwdma:      Stop operation of RD/WR DMA
 */
typedef struct hscb_rwdma_settings
{
    bool rw_desc_int;
    bool rw_desc_end;
    bool rw_bad_desc;
    bool rw_axi_error;
    bool rw_buf_full;
    bool rw_buf_empty;
    bool en_rwdma;
    bool en_rwdma_desc_tbl;
    bool rwdma_long_len;
    bool cancel_rwdma;
}hscb_rwdma_settings_t;

/**
 * hscb_axi_params_cfg: Structure contains HSCB AXI parameters
 * arlen:         ARLEN[3:0] value for AXI transactions
 * awlen:         AWLEN[3:0] value for AXI transactions
 * bresp:         BRESP[1:0] value for AXI transactions
 * raxi_err_addr: Address which produced error on AXI
 * arcache:       ARCACHE[3:0] value for AXI transactions
 * awcache:       AWCACHE[3:0] value for AXI transactions
 * arprot:        ARPROT[2:0] value for AXI transactions
 * awprot:        AWPROT[2:0] value for AXI transactions
 * arlock:        ARLOCK[1:0] value for AXI transactions
 * awlock:        AWLOCK[1:0] value for AXI transactions
 * arburst:       ARBURST[1:0] value for AXI transactions
 * awburst:       AWBURST[1:0] value for AXI transactions
 */
typedef struct hscb_axi_params_cfg
{
    hscb_axi_arwlen_t   arlen;
    hscb_axi_arwlen_t   awlen;
    hscb_axi_bresp_t    bresp;
    uint32_t            raxi_err_addr;
    hscb_axi_arwcache_t arcache;
    hscb_axi_arwcache_t awcache;
    hscb_axi_arwprot_t  arprot;
    hscb_axi_arwprot_t  awprot;
    hscb_axi_arwlock_t  arlock;
    hscb_axi_arwlock_t  awlock;
    hscb_axi_arwburst_t arburst;
    hscb_axi_arwburst_t awburst;
}hscb_axi_params_cfg_t;



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
 * \brief Set descriptor in memory
 * \param[in] sys_addr address in memory for placing descriptor
 * \param[in] pointer on read data descriptor
 */
void hscb_set_descr_in_mem(uint32_t sys_addr, uint32_t src_data_addr, uint32_t len);

/**
 * \brief Get descriptor from memory
 * \param[in] sys_addr descriptor address in memory
 * \param[in] endian should we rotate bytes?
 * \return    descriptor structure
 */
hscb_descr_struct_t hscb_get_descr_from_mem(uint32_t sys_addr, bool endian);

typedef struct hscb_instance
{
    uint32_t        src_hscb_base_addr;
    uint32_t        dst_hscb_base_addr;
    uint32_t*       src_addr;
    uint32_t        src_size;
    uint32_t*       dst_addr;
    uint32_t        dst_size;
    uint32_t        tx_descr_addr;
    uint32_t        rx_descr_addr;
}hscb_instance_t;

bool hscb_sw_rst(uint32_t base_addr);
void hscb_set_config(uint32_t base_addr, hscb_cfg_t* cfg);
void hscb_get_config(uint32_t base_addr, hscb_cfg_t* cfg);
uint32_t hscb_get_status(uint32_t base_addr);
uint32_t hscb_get_tbl_len_by_count(uint32_t count);
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
void hscb_set_axi_params(uint32_t base_addr, hscb_axi_params_cfg_t* cfg);
void hscb_get_axi_params(uint32_t base_addr, hscb_axi_params_cfg_t* cfg);
void hscb_set_rdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings);
void hscb_get_rdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings);
void hscb_set_wdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings);
void hscb_get_wdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings);
bool hscb_wait_status(uint32_t base_addr, uint32_t status);
bool hscb_transmit(uint32_t base_addr, uint32_t src_data_addr, uint32_t len, uint32_t desc_addr);
bool hscb_receive(uint32_t base_addr, uint32_t dst_data_addr, uint32_t len, uint32_t desc_addr);
void hscb_configure_for_transmit(uint32_t base_addr, uint32_t src_data_addr, uint32_t len, uint32_t desc_addr);
void hscb_configure_for_receive(uint32_t base_addr, uint32_t dst_data_addr, uint32_t len, uint32_t desc_addr);
void hscb_run_rdma(uint32_t base_addr);
void hscb_run_wdma(uint32_t base_addr);
void hscb_set_max_speed(uint32_t base_addr);
void hscb_enable(uint32_t base_addr);
void hscb_config_for_receive_and_transmit(hscb_instance_t* hscb_inst);


#define HSCB_SW_RESET_TIMEOUT   100
#define HSCB_TIMEOUT            100

#define GET_HSCB_IDX_BY_ADDR(addr)\
    ((addr==HSCB0_BASE) ? 0 :\
    (addr==HSCB1_BASE) ? 1 :\
    (addr==HSCB2_BASE) ? 2 :\
    (addr==HSCB3_BASE) ? 3 :15)



/**
 * @}
 */

#endif /* DEVICES_HSCB_H */
