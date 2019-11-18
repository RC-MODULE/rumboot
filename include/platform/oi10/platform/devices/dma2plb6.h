#ifndef DMA2PLB6_H_
#define DMA2PLB6_H_

#include <stdint.h>
#include <stdbool.h>


typedef enum {
    tr_type_single,
    tr_type_scatter_gather
}d2p6_transfer_type;

typedef enum{
    channel0 = 0,
    channel1 = 1,
    channel2 = 2,
    channel3 = 3
}DmaChannel;

struct channel_status {
    uint32_t terminalcnt_reached;
    uint32_t error_detected;
    uint32_t spec_error_status;
    uint32_t busy;
    uint32_t sc_gth_status;
};

//error_status in SR
enum {
    error_write_request = 0b000,
    error_read_request_regular = 0b010,
    error_read_data_regular = 0b011,
    error_read_request_scatter_or_resume = 0b100,
    error_read_data_scatter_or_resume = 0b101,
    error_alignnment = 0b110,
    error_scatter_alignment = 0b111
};

typedef enum {
    int_mode_level_wait_clr = 0b00,
    //value 01 is reserved
    int_mode_pulse_wait_clr = 0b10,
    int_mode_pulse_no_wait_clr = 0b11
}interrupt_mode;

typedef enum {
    snp_mode_off = 0b00,
    snp_mode_dst = 0b01,
    snp_mode_src = 0b10,
    snp_mode_src_dst = 0b11
}snoopable_mode;

typedef enum {
    tr_width_byte = 0b000,//byte
    tr_width_halfword = 0b001,//halfword
    tr_width_word = 0b010,//word
    tr_width_doubleword = 0b011,//double
    tr_width_quadword = 0b100//quad
}transfer_width;

typedef enum {
    rw_tr_size_1q = 0b000,
    rw_tr_size_2q = 0b001,
    rw_tr_size_4q = 0b010,
    rw_tr_size_8q = 0b011
}rw_transfer_size;

typedef enum {
    striding_src = 0b10,
    striding_dst = 0b01,
    striding_src_dst = 0b11,
    striding_none = 0b00
}tr_striding;

typedef enum {
    priority_low = 0b00,
    priority_medium_low = 0b01,
    priority_medium_high = 0b10,
    priority_high = 0b11
}channel_priority;

struct striding_params {
    tr_striding striding;
    uint32_t tbs;//transfer block_size, in doublewords
    uint32_t sbs;//skip block size, in doublewords
};

struct dma2plb6_setup_info {
    uint32_t base_addr;
    DmaChannel channel;
    //d2p6_transfer_type tr_type; //for future use
    transfer_width transfer_width;
    rw_transfer_size rw_transfer_size;
    uint64_t source_adr;//real physical address, not a virtual!
    uint64_t dest_adr;//real physical address, not a virtual!
    uint32_t count;//number of transfers
    struct striding_params striding_info;
    channel_priority priority;
    uint32_t tc_int_enable;
    uint32_t err_int_enable;
    interrupt_mode int_mode;
    snoopable_mode snp_mode;
    //uint32_t * descriptor;//pointer for scatter/gather transfers
    //uint32_t options;//OPTIONS register as is
};

//

struct channel_status dma2plb6_ch_get_status(uint32_t base_address, DmaChannel channel);

void dma2plb6_trace_error_status(struct channel_status const * status);
/**
 * Blocking version of dma2plb6_mcpy
 * @param setup_info
 * @param status
 * @return
 */
bool dma2plb6_single_copy(struct dma2plb6_setup_info * setup_info, struct channel_status * status);
/**
 * Blocking version of dma2plb6_mcpy_coherency_required
 * @param setup_info
 * @param status
 * @return
 */
bool dma2plb6_single_copy_coherency_required(struct dma2plb6_setup_info const * setup_info, struct channel_status * status);
void dma2plb6_clear_interrupt(uint32_t base_addr, DmaChannel channel);
/**
 * Async copy. This function is not applicable for requests to coherent slaves
 * @param setup_info
 */
void dma2plb6_mcpy(struct dma2plb6_setup_info * setup_info);
/**
 * Async copy
 * @param setup_info
 */

void dma2plb6_mcpy_coherency_required(struct dma2plb6_setup_info const * setup_info);
/**
 * Waits dma2plb6_mcpy or dma2plb6_mcpy_coherency_required
 * @param setup_info
 * @param status
 * @return
 */
bool wait_dma2plb6_mcpy(struct dma2plb6_setup_info const * setup_info, struct channel_status * status);
void dma2plb6_enable_o_slv_err_interrupt(uint32_t base_addr);
void dma2plb6_disable_o_slv_err_interrupt(uint32_t base_addr);

void dma2plb6_disable_channel(uint32_t base_addr, DmaChannel channel);

void dma2plb6_disable_all_channel(uint32_t const base_addr);

void dma2plb6_mcpy_init(struct dma2plb6_setup_info const * const setup_info);

void dma2plb6_enable_channel(uint32_t const base_addr, DmaChannel const channel);

void dma2plb6_enable_all_channels(uint32_t const base_addr);

void dma2plb6_clear_status_reg(uint32_t const base_addr);

#endif // DMA2PLB6_H_
