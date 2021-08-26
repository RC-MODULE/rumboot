/*
 * hscb.c
 *
 *  Created on: Jul 19, 2018
 *      Author: r.galiulin
 */
#include <algo/crc8.h>
#include <devices/ugly/hscb.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>

uint32_t hscb_change_endian (uint32_t data_in, bool change_endian){
    uint32_t data_out = 0;

    if(change_endian)
    {
        data_out = data_in << 24 & 0xff000000;
        data_out = data_out | (data_in << 8  & 0x00ff0000);
        data_out = data_out | (data_in >> 8  & 0x0000ff00);
        data_out = data_out | (data_in >> 24 & 0x000000ff);
    }
    else
        data_out = data_in;
    return data_out;
}

void hscb_convert_to_bytes (uint32_t* data_in, uint8_t* data_out, uint32_t len) {
    int i = 0;

    for (i=0;i<=len;i=i+8){
        data_out[i  ] = data_in[(i+4)/4];
        data_out[i+1] = data_in[(i+4)/4] >> 8;
        data_out[i+2] = data_in[(i+4)/4] >> 16;
        data_out[i+3] = data_in[(i+4)/4] >> 24;
        data_out[i+4] = data_in[i/4];
        data_out[i+5] = data_in[i/4] >> 8;
        data_out[i+6] = data_in[i/4] >> 16;
        data_out[i+7] = data_in[i/4] >> 24;
        }
}

bool hscb_sw_rst(uint32_t base_addr)
{
    uint32_t time = 0;
// Add by M.Chelyshev as glitch-protect
    uint32_t tmp = 0;
    tmp = ioread32(base_addr + HSCB_SETTINGS);
    tmp = tmp & 0xfffffffe;
    iowrite32(tmp, base_addr + HSCB_SETTINGS);
// ------------------------------------    
    iowrite32(1, base_addr + HSCB_SW_RESET);
    while ((ioread32(base_addr + HSCB_SW_RESET)!=0) && (time++<HSCB_SW_RESET_TIMEOUT));
    if (time<HSCB_SW_RESET_TIMEOUT)
        return true;
    else
        return false;
}

void hscb_set_config(uint32_t base_addr, hscb_cfg_t* cfg)
{
    uint32_t tmp = 0;
    tmp |= (cfg->en_hscb << HSCB_SETTINGS_EN_HSCB_i);
//    tmp |= (cfg->tx_endian << HSCB_SETTINGS_TX_ENDIAN_i);
//    tmp |= (cfg->rx_endian << HSCB_SETTINGS_RX_ENDIAN_i);
    tmp |= (cfg->time_mode << HSCB_SETTINGS_TIME_MODE_i);
    tmp |= (cfg->loop_back << HSCB_SETTINGS_LOOP_BACK_i);
    tmp |= (cfg->en_rmap << HSCB_SETTINGS_EN_RMAP_i);
    tmp |= (cfg->en_trim_clk << HSCB_SETTINGS_EN_TRIM_CLK_i);
    tmp |= (cfg->rx_fix_en << HSCB_SETTINGS_RX_FIX_EN_i);
    iowrite32(tmp | (1 << 24), base_addr + HSCB_SETTINGS);
}

void hscb_get_config(uint32_t base_addr, hscb_cfg_t* cfg)
{
    uint32_t settings;
    settings = ioread32(base_addr + HSCB_SETTINGS);
    cfg->en_hscb     = (settings & HSCB_SETTINGS_EN_HSCB_mask)      >> HSCB_SETTINGS_EN_HSCB_i;
//    cfg->tx_endian   = (settings & HSCB_SETTINGS_TX_ENDIAN_mask)    >> HSCB_SETTINGS_TX_ENDIAN_i;
//    cfg->rx_endian   = (settings & HSCB_SETTINGS_RX_ENDIAN_mask)    >> HSCB_SETTINGS_RX_ENDIAN_i;
    cfg->time_mode   = (settings & HSCB_SETTINGS_TIME_MODE_mask)    >> HSCB_SETTINGS_TIME_MODE_i;
    cfg->loop_back   = (settings & HSCB_SETTINGS_LOOP_BACK_mask)    >> HSCB_SETTINGS_LOOP_BACK_i;
    cfg->en_rmap     = (settings & HSCB_SETTINGS_EN_RMAP_i)         >> HSCB_SETTINGS_EN_RMAP_i;
    cfg->en_trim_clk = (settings & HSCB_SETTINGS_EN_TRIM_CLK_mask)  >> HSCB_SETTINGS_EN_TRIM_CLK_i;
    cfg->rx_fix_en   = (settings & HSCB_SETTINGS_RX_FIX_EN_mask)    >> HSCB_SETTINGS_RX_FIX_EN_i;
}

void hscb_set_descr_in_mem(hscb_descr_struct_t descr, uint32_t sys_addr)
{
    iowrite32( hscb_change_endian(((descr.length << HSCB_RD_DESCR_LENGTH_i) & HSCB_RD_DESCR_LENGTH_mask)   |
                                  ((descr.act    << HSCB_RD_DESCR_ACT_i)    & HSCB_RD_DESCR_ACT_mask)      |
                                  ((descr.act0   << HSCB_RD_DESCR_ACT0_i)   & HSCB_RD_DESCR_ACT0_mask)     |
                                  ((descr.ie     << HSCB_RD_DESCR_IE_i)     & HSCB_RD_DESCR_IE_mask)       |
                                  ((descr.valid  << HSCB_RD_DESCR_VALID_i)  & HSCB_RD_DESCR_VALID_mask),
                                    descr.change_endian),
                                   sys_addr);
    iowrite32( hscb_change_endian(descr.start_address, descr.change_endian), sys_addr + 0x04);
//    rumboot_printf("Preparing descriptor: MEM[0x%x] = 0x%x\n", sys_addr,     ioread32(sys_addr));
//    rumboot_printf("Preparing descriptor: MEM[0x%x] = 0x%x\n", sys_addr + 4, ioread32(sys_addr + 4));
}

void hscb_set_empty_descr_in_mem(uint32_t sys_addr, bool change_endian)
{
    iowrite32( hscb_change_endian( HSCB_DESCR_VALID << HSCB_RD_DESCR_VALID_i, change_endian),  sys_addr);
    iowrite32( 0x0,         sys_addr + 0x04);
//    rumboot_printf("Preparing descriptor: MEM[0x%x] = 0x%x\n", sys_addr,     ioread32(sys_addr));
//    rumboot_printf("Preparing descriptor: MEM[0x%x] = 0x%x\n", sys_addr + 4, ioread32(sys_addr + 4));
}

void hscb_set_single_descr_in_mem(uint32_t sys_addr, uint32_t src_data_addr, uint32_t len)
{
    hscb_descr_struct_t descr;
    descr.start_address = src_data_addr;
    descr.length = len;
    descr.act  = HSCB_ACT_TRAN;
    descr.act0 = HSCB_ACT0_LAST;
    descr.ie = false;
    descr.valid = true;
    descr.change_endian = true;
    hscb_set_descr_in_mem( descr, sys_addr);
    hscb_set_empty_descr_in_mem( sys_addr + 0x8, descr.change_endian );
//    rumboot_printf("Preparing descriptor: MEM[0x%x] = 0x%x\n", sys_addr,     ioread32(sys_addr));
//    rumboot_printf("Preparing descriptor: MEM[0x%x] = 0x%x\n", sys_addr + 4, ioread32(sys_addr + 4));
}

hscb_descr_struct_t hscb_get_descr_from_mem(uint32_t sys_addr, bool endian)
{
    uint32_t desc_data;
    hscb_descr_struct_t result;
    desc_data = hscb_change_endian(ioread32(sys_addr), endian);
    result.start_address = hscb_change_endian(ioread32(sys_addr + 4), endian);

    result.length = (desc_data & HSCB_RD_DESCR_LENGTH_mask) >> HSCB_RD_DESCR_LENGTH_i;
    result.act  = (desc_data & HSCB_RD_DESCR_ACT_mask) >> HSCB_RD_DESCR_ACT_i;
    result.act0 = (desc_data & HSCB_RD_DESCR_ACT0_mask) >> HSCB_RD_DESCR_ACT0_i;
    result.ie = ((desc_data & HSCB_RD_DESCR_IE_mask) >> HSCB_RD_DESCR_IE_i) ? true : false;
    result.err = ((desc_data & HSCB_RD_DESCR_ERR_mask) >> HSCB_RD_DESCR_ERR_i) ? true : false;
    result.valid = ((desc_data & HSCB_RD_DESCR_VALID_mask) >> HSCB_RD_DESCR_VALID_i) ? true : false;
    result.change_endian = endian;
    return result;
}

uint32_t hscb_get_status(uint32_t base_addr)
{
    return ioread32(base_addr + HSCB_STATUS);
}

inline uint32_t hscb_get_tbl_len_by_count(uint32_t count){
    return sizeof(hscb_packed_descr_struct_t) * (count + 1) ;
}

void hscb_set_irq_mask(uint32_t base_addr, uint32_t mask)
{
    iowrite32(mask, base_addr + HSCB_IRQ_MASK);
}

void hscb_set_timings(uint32_t base_addr, hscb_timings_cfg_t* cfg)
{
    iowrite32(((cfg->freq      << HSCB_TRANS_CLK_FREQ_i)      & HSCB_TRANS_CLK_FREQ_mask)      |
              ((cfg->init_freq << HSCB_TRANS_CLK_INIT_FREQ_i) & HSCB_TRANS_CLK_INIT_FREQ_mask) |
              ((cfg->limit     << HSCB_TRANS_CLK_LIMIT_i)     & HSCB_TRANS_CLK_LIMIT_mask), base_addr + HSCB_TRANS_CLK);

    iowrite32(((cfg->time_64      << HSCB_TIMINGS_TIME_64_i)      & HSCB_TIMINGS_TIME_64_mask)       |
              ((cfg->time_128     << HSCB_TIMINGS_TIME_128_i)     & HSCB_TIMINGS_TIME_128_mask)      |
              ((cfg->silence_time << HSCB_TIMINGS_SILENCE_TIME_i) & HSCB_TIMINGS_SILENCE_TIME_mask), base_addr + HSCB_TIMINGS);
}

void hscb_get_timings(uint32_t base_addr, hscb_timings_cfg_t* cfg)
{
    uint32_t buf;
    buf = ioread32(base_addr + HSCB_TRANS_CLK);
    cfg->freq      = (buf & HSCB_TRANS_CLK_FREQ_mask)      >> HSCB_TRANS_CLK_FREQ_i;
    cfg->init_freq = (buf & HSCB_TRANS_CLK_INIT_FREQ_mask) >> HSCB_TRANS_CLK_INIT_FREQ_i;
    cfg->limit     = (buf & HSCB_TRANS_CLK_LIMIT_mask)     >> HSCB_TRANS_CLK_LIMIT_i;

    buf = ioread32(base_addr + HSCB_TIMINGS);
    cfg->time_64      = (buf & HSCB_TIMINGS_TIME_64_mask)      >> HSCB_TIMINGS_TIME_64_i;
    cfg->time_128     = (buf & HSCB_TIMINGS_TIME_128_mask)     >> HSCB_TIMINGS_TIME_128_i;
    cfg->silence_time = (buf & HSCB_TIMINGS_SILENCE_TIME_mask) >> HSCB_TIMINGS_SILENCE_TIME_i;
}

bool hscb_adma_sw_rst(uint32_t base_addr)
{
    uint32_t time = 0;
    iowrite32(1, base_addr + HSCB_ADMA_SW_RESET);
    while ((ioread32(base_addr + HSCB_ADMA_SW_RESET)!=0) && (time++<HSCB_SW_RESET_TIMEOUT));
    if (time<HSCB_SW_RESET_TIMEOUT)
        return true;
    else
        return false;
}

uint32_t hscb_get_adma_ch_status(uint32_t base_addr)
{
    return ioread32(base_addr + HSCB_ADMA_CH_STATUS);
}

void hscb_set_rdma_irq_mask(uint32_t base_addr, uint32_t mask)
{
    iowrite32(mask, base_addr + HSCB_RDMA_SETTINGS);
}

void hscb_set_wdma_irq_mask(uint32_t base_addr, uint32_t mask)
{
    iowrite32(mask, base_addr + HSCB_WDMA_SETTINGS);
}


uint32_t hscb_get_rdma_status(uint32_t base_addr)
{
    return ioread32(base_addr + HSCB_RDMA_STATUS);
}

uint32_t hscb_get_wdma_status(uint32_t base_addr)
{
    return ioread32(base_addr + HSCB_WDMA_STATUS);
}

void hscb_set_rdma_tbl_size(uint32_t base_addr, uint32_t size)
{
    iowrite32(size, base_addr + HSCB_RDMA_TBL_SIZE);
}

uint32_t hscb_get_rdma_tbl_size(uint32_t base_addr)
{
    return ioread32(base_addr + HSCB_RDMA_TBL_SIZE);
}

void hscb_set_wdma_tbl_size(uint32_t base_addr, uint32_t size)
{
    iowrite32(size, base_addr + HSCB_WDMA_TBL_SIZE);
}

uint32_t hscb_get_wdma_tbl_size(uint32_t base_addr)
{
    return ioread32(base_addr + HSCB_WDMA_TBL_SIZE);
}

void hscb_set_rdma_sys_addr(uint32_t base_addr, uint32_t descr_addr)
{
    iowrite32(descr_addr, base_addr + HSCB_RDMA_SYS_ADDR);
}

uint32_t hscb_get_rdma_sys_addr(uint32_t base_addr)
{
    return ioread32(base_addr + HSCB_RDMA_SYS_ADDR);
}

void hscb_set_wdma_sys_addr(uint32_t base_addr, uint32_t descr_addr)
{
    iowrite32(descr_addr, base_addr + HSCB_WDMA_SYS_ADDR);
}

uint32_t hscb_get_wdma_sys_addr(uint32_t base_addr)
{
    return ioread32(base_addr + HSCB_WDMA_SYS_ADDR);
}

void hscb_set_axi_params(uint32_t base_addr, hscb_axi_params_cfg_t* cfg)
{
    iowrite32( cfg->arlen,   base_addr + HSCB_ARLEN         );
    iowrite32( cfg->awlen,   base_addr + HSCB_AWLEN         );
    iowrite32( cfg->arcache, base_addr + HSCB_ARCACHE       );
    iowrite32( cfg->awcache, base_addr + HSCB_AWCACHE       );
    iowrite32( cfg->arprot,  base_addr + HSCB_ARPROT        );
    iowrite32( cfg->awprot,  base_addr + HSCB_AWPROT        );
    iowrite32( cfg->arlock,  base_addr + HSCB_ARLOCK        );
    iowrite32( cfg->awlock,  base_addr + HSCB_AWLOCK        );
    iowrite32( cfg->arburst, base_addr + HSCB_ARBURST       );
    iowrite32( cfg->awburst, base_addr + HSCB_AWBURST       );
}

void hscb_get_axi_params(uint32_t base_addr, hscb_axi_params_cfg_t* cfg)
{
    cfg->arlen         = ioread32( base_addr + HSCB_ARLEN         );
    cfg->awlen         = ioread32( base_addr + HSCB_AWLEN         );
    cfg->bresp         = ioread32( base_addr + HSCB_BRESP         );
    cfg->raxi_err_addr = ioread32( base_addr + HSCB_RAXI_ERR_ADDR );
    cfg->arcache       = ioread32( base_addr + HSCB_ARCACHE       );
    cfg->awcache       = ioread32( base_addr + HSCB_AWCACHE       );
    cfg->arprot        = ioread32( base_addr + HSCB_ARPROT        );
    cfg->awprot        = ioread32( base_addr + HSCB_AWPROT        );
    cfg->arlock        = ioread32( base_addr + HSCB_ARLOCK        );
    cfg->awlock        = ioread32( base_addr + HSCB_AWLOCK        );
    cfg->arburst       = ioread32( base_addr + HSCB_ARBURST       );
    cfg->awburst       = ioread32( base_addr + HSCB_AWBURST       );
}

void hscb_set_rdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings)
{
    iowrite32( ( settings->rw_desc_int       << HSCB_XDMA_SETTINGS_X_DESC_INT_i )          |
               ( settings->rw_desc_end       << HSCB_XDMA_SETTINGS_X_DESC_END_i )          |
               ( settings->rw_bad_desc       << HSCB_XDMA_SETTINGS_X_BAD_DESC_i )          |
               ( settings->rw_axi_error      << HSCB_XDMA_SETTINGS_X_AXI_ERROR_i )      |
               ( settings->rw_buf_full       << HSCB_XDMA_SETTINGS_X_BUF_FULL_i )       |
               ( settings->rw_buf_empty      << HSCB_XDMA_SETTINGS_X_BUF_EMPTY_i )      |
               ( settings->en_rwdma          << HSCB_XDMA_SETTINGS_EN_XDMA_i )          |
               ( settings->en_rwdma_desc_tbl << HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_i ) |
               ( settings->rwdma_long_len    << HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_i )    |
               ( settings->cancel_rwdma      << HSCB_XDMA_SETTINGS_CANCEL_XDMA_i ),
                base_addr + HSCB_RDMA_SETTINGS         );
}

void hscb_get_rdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings)
{
    uint32_t val;
    val = ioread32(base_addr+ HSCB_RDMA_SETTINGS);
    settings->rw_desc_int       = ( val & HSCB_XDMA_SETTINGS_X_DESC_INT_mask )          >> HSCB_XDMA_SETTINGS_X_DESC_INT_i;
    settings->rw_desc_end       = ( val & HSCB_XDMA_SETTINGS_X_DESC_END_mask )          >> HSCB_XDMA_SETTINGS_X_DESC_END_i;
    settings->rw_bad_desc       = ( val & HSCB_XDMA_SETTINGS_X_BAD_DESC_mask )          >> HSCB_XDMA_SETTINGS_X_BAD_DESC_i;
    settings->rw_axi_error      = ( val & HSCB_XDMA_SETTINGS_X_AXI_ERROR_mask )      >> HSCB_XDMA_SETTINGS_X_AXI_ERROR_i;
    settings->rw_buf_full       = ( val & HSCB_XDMA_SETTINGS_X_BUF_FULL_mask )       >> HSCB_XDMA_SETTINGS_X_BUF_FULL_i;
    settings->rw_buf_empty      = ( val & HSCB_XDMA_SETTINGS_X_BUF_EMPTY_mask )      >> HSCB_XDMA_SETTINGS_X_BUF_EMPTY_i;
    settings->en_rwdma          = ( val & HSCB_XDMA_SETTINGS_EN_XDMA_mask )          >> HSCB_XDMA_SETTINGS_EN_XDMA_i;
    settings->en_rwdma_desc_tbl = ( val & HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_mask ) >> HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_i;
    settings->rwdma_long_len    = ( val & HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_mask )    >> HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_i;
    settings->cancel_rwdma      = ( val & HSCB_XDMA_SETTINGS_CANCEL_XDMA_mask )      >> HSCB_XDMA_SETTINGS_CANCEL_XDMA_i;
}

void hscb_set_wdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings)
{
    iowrite32( ( settings->rw_desc_int       << HSCB_XDMA_SETTINGS_X_DESC_INT_i )          |
               ( settings->rw_desc_end       << HSCB_XDMA_SETTINGS_X_DESC_END_i )          |
               ( settings->rw_bad_desc       << HSCB_XDMA_SETTINGS_X_BAD_DESC_i )          |
               ( settings->rw_axi_error      << HSCB_XDMA_SETTINGS_X_AXI_ERROR_i )      |
               ( settings->rw_buf_full       << HSCB_XDMA_SETTINGS_X_BUF_FULL_i )       |
               ( settings->rw_buf_empty      << HSCB_XDMA_SETTINGS_X_BUF_EMPTY_i )      |
               ( settings->en_rwdma          << HSCB_XDMA_SETTINGS_EN_XDMA_i )          |
               ( settings->en_rwdma_desc_tbl << HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_i ) |
               ( settings->rwdma_long_len    << HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_i )    |
               ( settings->cancel_rwdma      << HSCB_XDMA_SETTINGS_CANCEL_XDMA_i ),
                base_addr + HSCB_WDMA_SETTINGS         );
}

void hscb_get_wdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings)
{
    uint32_t val;
    val = ioread32(base_addr+ HSCB_WDMA_SETTINGS);
    settings->rw_desc_int       = ( val & HSCB_XDMA_SETTINGS_X_DESC_INT_mask )          >> HSCB_XDMA_SETTINGS_X_DESC_INT_i;
    settings->rw_desc_end       = ( val & HSCB_XDMA_SETTINGS_X_DESC_END_mask )          >> HSCB_XDMA_SETTINGS_X_DESC_END_i;
    settings->rw_bad_desc       = ( val & HSCB_XDMA_SETTINGS_X_BAD_DESC_mask )          >> HSCB_XDMA_SETTINGS_X_BAD_DESC_i;
    settings->rw_axi_error      = ( val & HSCB_XDMA_SETTINGS_X_AXI_ERROR_mask )      >> HSCB_XDMA_SETTINGS_X_AXI_ERROR_i;
    settings->rw_buf_full       = ( val & HSCB_XDMA_SETTINGS_X_BUF_FULL_mask )       >> HSCB_XDMA_SETTINGS_X_BUF_FULL_i;
    settings->rw_buf_empty      = ( val & HSCB_XDMA_SETTINGS_X_BUF_EMPTY_mask )      >> HSCB_XDMA_SETTINGS_X_BUF_EMPTY_i;
    settings->en_rwdma          = ( val & HSCB_XDMA_SETTINGS_EN_XDMA_mask )          >> HSCB_XDMA_SETTINGS_EN_XDMA_i;
    settings->en_rwdma_desc_tbl = ( val & HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_mask ) >> HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_i;
    settings->rwdma_long_len    = ( val & HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_mask )    >> HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_i;
    settings->cancel_rwdma      = ( val & HSCB_XDMA_SETTINGS_CANCEL_XDMA_mask )      >> HSCB_XDMA_SETTINGS_CANCEL_XDMA_i;
}

bool hscb_wait_status(uint32_t base_addr, uint32_t status)
{
    uint32_t time = 0;
    while (!(hscb_get_status(base_addr) & status) && (time++<HSCB_TIMEOUT));
    if (time < HSCB_TIMEOUT)
    {
        return true;
    }
    else
    {
        rumboot_printf("Unexpected HSCB(0x%x) status: 0x%x\n", base_addr, hscb_get_status(base_addr));
        return false;
    }
}

bool hscb_wait_rdma_status(uint32_t base_addr, uint32_t status)
{
    uint32_t time = 0;
    while (!(hscb_get_rdma_status(base_addr) & status) && (time++<HSCB_TIMEOUT));
    if (time < HSCB_TIMEOUT)
    {
        return true;
    }
    else
    {
        rumboot_printf("Unexpected HSCB(0x%x) status: 0x%x\n", base_addr, hscb_get_status(base_addr));
        return false;
    }
}

void hscb_configure_for_transmit(uint32_t base_addr, uint32_t src_data_addr, uint32_t len, uint32_t desc_addr)
{
    rumboot_printf("Setting transmit descriptor to addr 0x%X\n", desc_addr);
    hscb_set_single_descr_in_mem(desc_addr, src_data_addr, len);

    rumboot_putstring("Setting RDMA_TBL_SIZE and RDMA_SYS_ADDR\n");
    hscb_set_rdma_tbl_size(base_addr, hscb_get_tbl_len_by_count(1));
    hscb_set_rdma_sys_addr(base_addr, rumboot_virt_to_dma((uint32_t *) desc_addr));
/*
    rumboot_printf("Setting WDMA_TBL_SIZE and WDMA_SYS_ADDR\n");
    hscb_set_wdma_tbl_size(base_addr, 0x14);
    hscb_set_wdma_sys_addr(base_addr, rumboot_virt_to_dma((uint32_t *) desc_addr + 0x20));
*/
}

void hscb_configure_for_receive(uint32_t base_addr, uint32_t dst_data_addr, uint32_t len, uint32_t desc_addr)
{
    rumboot_printf("Setting receive descriptor to addr 0x%X\n", desc_addr);
    hscb_set_single_descr_in_mem(desc_addr, dst_data_addr, len);
/*
    rumboot_printf("Setting RDMA_TBL_SIZE and RDMA_SYS_ADDR\n");
    hscb_set_rdma_tbl_size(base_addr, 0x14);
    hscb_set_rdma_sys_addr(base_addr, rumboot_virt_to_dma((uint32_t *) (desc_addr + 0x20)));
*/
    rumboot_printf("Setting WDMA_TBL_SIZE and WDMA_SYS_ADDR (0x%X)\n", base_addr);
    hscb_set_wdma_tbl_size(base_addr, hscb_get_tbl_len_by_count(1));
    hscb_set_wdma_sys_addr(base_addr, rumboot_virt_to_dma((uint32_t *) desc_addr));
}

void hscb_enable(uint32_t base_addr)
{
    hscb_cfg_t hscb_cfg;
//    rumboot_printf("Enable HSCB (0x%X)\n", base_addr);
    hscb_get_config(base_addr, &hscb_cfg);
    hscb_cfg.en_hscb = true;
    hscb_cfg.rx_fix_en = false;
    hscb_set_config(base_addr, &hscb_cfg);
}

void hscb_run_rdma(uint32_t base_addr)
{
    hscb_rwdma_settings_t rdma_settings;
    rumboot_printf("Run RDMA (0x%X)\n", base_addr);
    hscb_get_rdma_settings(base_addr, &rdma_settings);
    rdma_settings.en_rwdma = true;
    rdma_settings.en_rwdma_desc_tbl = true;
    rdma_settings.rwdma_long_len = true;
    hscb_set_rdma_settings(base_addr, &rdma_settings);
}

void hscb_run_wdma(uint32_t base_addr)
{
    hscb_rwdma_settings_t wdma_settings;
    rumboot_printf("Run WDMA (0x%X)\n", base_addr);
    hscb_get_wdma_settings(base_addr, &wdma_settings);
    wdma_settings.rw_bad_desc = true;
    wdma_settings.en_rwdma = true;
    wdma_settings.en_rwdma_desc_tbl = true;
    wdma_settings.rwdma_long_len = true;
    hscb_set_wdma_settings(base_addr, &wdma_settings);
}

void hscb_set_max_speed(uint32_t base_addr)
{
    hscb_timings_cfg_t timings;
    hscb_get_timings(base_addr, &timings);
    /*
    rumboot_printf("FREQ = %d\n", timings.freq);
    rumboot_printf("INIT_FREQ = %d\n", timings.init_freq);
    rumboot_printf("LIMIT = %d\n", timings.limit);
    rumboot_printf("SILENCE_TIME = %d\n", timings.silence_time);
    rumboot_printf("TIME_128 = %d\n", timings.time_128);
    rumboot_printf("TIME_64 = %d\n", timings.time_64);
    */
    timings.freq = 0;
    hscb_set_timings(base_addr, &timings);
}

bool hscb_transmit(uint32_t base_addr, uint32_t src_data_addr, uint32_t len, uint32_t desc_addr)
{
    uint8_t hscb_idx = GET_HSCB_IDX_BY_ADDR(base_addr);

    rumboot_printf("HSCB%d(0x%x) will transmit %d bytes from address 0x%X\n", hscb_idx, base_addr, len, src_data_addr);
    hscb_configure_for_transmit(base_addr, src_data_addr, len, desc_addr);
    //hscb_set_max_speed(base_addr);
    hscb_enable(base_addr);

    rumboot_putstring("Waiting ACTIVE_LINK status\n");
    if (!hscb_wait_status(base_addr, HSCB_STATUS_ACTIVE_LINK_mask)) return false;


    hscb_run_rdma(base_addr);

    return true;
}

bool hscb_receive(uint32_t base_addr, uint32_t dst_data_addr, uint32_t len, uint32_t desc_addr)
{
    uint8_t hscb_idx = GET_HSCB_IDX_BY_ADDR(base_addr);

    rumboot_printf("HSCB%d(0x%x) will receive %d bytes to address 0x%X\n", hscb_idx, base_addr, len, dst_data_addr);

    hscb_configure_for_receive(base_addr, dst_data_addr, len, desc_addr);
    //hscb_set_max_speed(base_addr);
    hscb_enable(base_addr);

    rumboot_putstring("Waiting ACTIVE_LINK status\n");
    if (!hscb_wait_status(base_addr, HSCB_STATUS_ACTIVE_LINK_mask)) return false;


    hscb_run_rdma(base_addr);

    return true;
}

void hscb_config_for_receive_and_transmit_same_multiple(hscb_instance_t* hscb_inst, uint32_t count)
{
    hscb_descr_struct_t descr;
    uint32_t sys_addr;

//    rumboot_printf("\nHSCB%d(0x%x) will transmit %d bytes from address 0x%X %d times.\n",
//            GET_HSCB_IDX_BY_ADDR(hscb_inst->src_hscb_base_addr),
//            hscb_inst->src_hscb_base_addr,
//            hscb_inst->src_size,
//            rumboot_virt_to_dma(hscb_inst->src_addr),
//            count);
    //rumboot_printf("Setting transmit descriptor to addr 0x%X\n", hscb_inst->tx_descr_addr);
    descr.start_address = rumboot_virt_to_dma(hscb_inst->src_addr);
    descr.length = hscb_inst->src_size;
    descr.act  = HSCB_ACT_TRAN;
    descr.act0 = HSCB_ACT0_LAST;
    descr.ie = false;
    descr.valid = true;
    descr.change_endian = true;
    for(sys_addr = hscb_inst->tx_descr_addr; sys_addr < HSCB_DESCR_SIZE * count + hscb_inst->tx_descr_addr; sys_addr += HSCB_DESCR_SIZE)
        hscb_set_descr_in_mem( descr, sys_addr);
    hscb_set_empty_descr_in_mem( hscb_inst->tx_descr_addr + HSCB_DESCR_SIZE * count, descr.change_endian );

//    rumboot_printf("HSCB%d(0x%x) will receive %d bytes to address 0x%X  %d times.\n",
//            GET_HSCB_IDX_BY_ADDR(hscb_inst->src_hscb_base_addr),
//            hscb_inst->src_hscb_base_addr,
//            hscb_inst->dst_size,
//            rumboot_virt_to_dma(hscb_inst->dst_addr),
//            count);
    //rumboot_printf("Setting receive descriptor to addr 0x%X\n", hscb_inst->rx_descr_addr);
    descr.start_address = rumboot_virt_to_dma(hscb_inst->dst_addr);
    descr.length = hscb_inst->dst_size;
    for(sys_addr = hscb_inst->rx_descr_addr; sys_addr < HSCB_DESCR_SIZE * count + hscb_inst->rx_descr_addr; sys_addr += HSCB_DESCR_SIZE)
        hscb_set_descr_in_mem( descr, sys_addr);
    hscb_set_empty_descr_in_mem( hscb_inst->rx_descr_addr + HSCB_DESCR_SIZE * count, descr.change_endian );

    //rumboot_printf("Setting RDMA_TBL_SIZE and RDMA_SYS_ADDR\n");
    hscb_set_rdma_tbl_size(hscb_inst->src_hscb_base_addr, hscb_get_tbl_len_by_count(count));
    hscb_set_rdma_sys_addr(hscb_inst->src_hscb_base_addr, rumboot_virt_to_dma((uint32_t *) hscb_inst->tx_descr_addr));

    //rumboot_printf("Setting WDMA_TBL_SIZE and WDMA_SYS_ADDR\n");
    hscb_set_wdma_tbl_size(hscb_inst->src_hscb_base_addr, hscb_get_tbl_len_by_count(count));
    hscb_set_wdma_sys_addr(hscb_inst->src_hscb_base_addr, rumboot_virt_to_dma((uint32_t *) hscb_inst->rx_descr_addr));


    hscb_set_max_speed(hscb_inst->src_hscb_base_addr);
}

void hscb_config_for_receive_and_transmit(hscb_instance_t* hscb_inst)
{
    hscb_config_for_receive_and_transmit_same_multiple(hscb_inst, 1);
}

uint8_t hscb_rmap_make_reply_instruction(uint8_t instruction)
{
    return (instruction & ((uint8_t)((~HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_mask)
            | (HSCB_RMAP_PACKET_TYPE_REPLY << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i))));
}
uint32_t hscb_rmap_get_reply_addr_actual_length(hscb_uint8_array_with_length_t reply_addr)
{
    uint32_t length = reply_addr.length;
    if(length == 0)
        return length;
    for( int i = 0; (reply_addr.array[i] == 0) && (i < (reply_addr.length - 1)); ++i)
        --length;
    return length;
}

uint8_t hscb_rmap_get_reply_byte(   hscb_uint8_array_with_length_t  rmap_reply,
                                    uint32_t                        start_index,
                                    hscb_rmap_reply_packet_fields_t required_field)
{
    return rmap_reply.array[start_index + required_field];
}


uint32_t hscb_rmap_reply_get_data_len(   hscb_uint8_array_with_length_t  rmap_reply,
                                                uint32_t                        start_index)
{
    return ((hscb_rmap_get_reply_byte(rmap_reply, start_index, HSCB_RMAP_REPLY_DATA_LEN_2_i)) << 16)
          | ((hscb_rmap_get_reply_byte(rmap_reply, start_index, HSCB_RMAP_REPLY_DATA_LEN_1_i)) << 8)
          | ((hscb_rmap_get_reply_byte(rmap_reply, start_index, HSCB_RMAP_REPLY_DATA_LEN_0_i)));
}

uint32_t hscb_rmap_reply_calculate_length(  hscb_uint8_array_with_length_t  rmap_reply,
                                            uint32_t                        reply_addr_chain_length)
{
    uint32_t length_of_reply_packet = reply_addr_chain_length;
    if (hscb_rmap_get_reply_byte(rmap_reply, reply_addr_chain_length, HSCB_RMAP_REPLY_INSTRUCTION_i)
                                & HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_mask)
        length_of_reply_packet += 8;
    else
    {
        uint32_t reply_data_length = hscb_rmap_reply_get_data_len(rmap_reply, reply_addr_chain_length);
        length_of_reply_packet += 12 + ((reply_data_length) ? 1 : 0) + reply_data_length;
    }
    return length_of_reply_packet;
}

uint32_t hscb_prepare_rmap_packet(hscb_rmap_packet_raw_configuration_t rmap_packet_raw,
        hscb_rmap_packet_ready_for_transmit_t* rmap_packet_ready)
{
    hscb_descr_struct_t descr = {};
    uint32_t initial_data_area_index = rmap_packet_ready->count_areas;
    bool is_target_sw_addr_chain_supplied = (rmap_packet_raw.target_addr_chain.length > 0);
    bool is_reply_sw_addr_chain_supplied = (rmap_packet_raw.reply_addr_chain.length > 0);
    bool is_data_chain_supplied = (rmap_packet_raw.data_chain.array != 0);
    uint32_t index_for_header_CRC = 0;
    uint8_t  temporary_CRC = 0;

    /*SW RMAP target address chain (optional)*/
    if(is_target_sw_addr_chain_supplied)
    {
        if( rmap_packet_raw.target_addr_chain.array == NULL)
            return PREPARE_RMAP_PACKET_ADDR_CHAIN_IS_NULL;
        rmap_packet_ready->data_areas[rmap_packet_ready->count_areas]
                                      = rmap_packet_raw.target_addr_chain.array;
        rmap_packet_ready->data_area_sizes[rmap_packet_ready->count_areas]
                                      = rmap_packet_raw.target_addr_chain.length;
        descr.start_address = rumboot_virt_to_dma(rmap_packet_ready->data_areas[rmap_packet_ready->count_areas]);
        descr.length        = rmap_packet_ready->data_area_sizes[rmap_packet_ready->count_areas];
        descr.act           = HSCB_ACT_TRAN;
        descr.act0          = HSCB_ACT0_NOLAST;
        descr.ie            = HSCB_DESCR_ITRPT_OFF;
        descr.valid         = HSCB_DESCR_VALID;
        descr.change_endian = rmap_packet_raw.change_endian;
        hscb_set_descr_in_mem(descr, (uint32_t)(rmap_packet_ready->array_of_descriptors + rmap_packet_ready->count_areas));
        rmap_packet_ready->count_areas++;
    }

    /*SW RMAP mandatory control chain*/
    if((is_reply_sw_addr_chain_supplied) &&
            ( rmap_packet_raw.reply_addr_chain.array == NULL))
            return PREPARE_RMAP_PACKET_REPLY_ADDR_CHAIN_IS_NULL;

    CLEAR_BITS_BY_MASK(rmap_packet_raw.instruction,HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_mask);
    switch((rmap_packet_raw.reply_addr_chain.length))
    {
        case 0:
            SET_BITS_BY_MASK(rmap_packet_raw.instruction,
                    (HSCB_RMAP_PACKET_REPLY_ADDR_0B << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i));
            break;
        case 4:
            SET_BITS_BY_MASK(rmap_packet_raw.instruction,
                    (HSCB_RMAP_PACKET_REPLY_ADDR_4B << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i));
            break;
        case 8:
            SET_BITS_BY_MASK(rmap_packet_raw.instruction,
                    (HSCB_RMAP_PACKET_REPLY_ADDR_8B << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i));
            break;
        case 12:
            SET_BITS_BY_MASK(rmap_packet_raw.instruction,
                    (HSCB_RMAP_PACKET_REPLY_ADDR_12B << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i));
            break;
        default: return PREPARE_RMAP_PACKET_REPLY_ADDR_CHAIN_WRONG_SIZE;
    }

    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_TARGET_LOGICAL_ADDR_i] = rmap_packet_raw.target_logical_addr;
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_PROTOCOL_ID_i] = HSCB_RMAP_PROTOCOL_ID;
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_INSTRUCTION_i] = rmap_packet_raw.instruction;
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_KEY_i] = rmap_packet_raw.key;

    descr.start_address = rumboot_virt_to_dma(rmap_packet_ready->data_areas[rmap_packet_ready->count_areas]);
    descr.length        = rmap_packet_ready->data_area_sizes[rmap_packet_ready->count_areas];
    descr.act           = HSCB_ACT_TRAN;
    descr.act0          = HSCB_ACT0_NOLAST;
    descr.ie            = HSCB_DESCR_ITRPT_OFF;
    descr.valid         = HSCB_DESCR_VALID;
    descr.change_endian = rmap_packet_raw.change_endian;
    hscb_set_descr_in_mem(descr, (uint32_t)(rmap_packet_ready->array_of_descriptors + rmap_packet_ready->count_areas));
    rmap_packet_ready->count_areas++;

    /*SW RMAP Reply Address Chain (optional)*/

    if(is_reply_sw_addr_chain_supplied)
    {
        rmap_packet_ready->data_areas[rmap_packet_ready->count_areas]
                                      = rmap_packet_raw.reply_addr_chain.array;
        rmap_packet_ready->data_area_sizes[rmap_packet_ready->count_areas]
                                      = rmap_packet_raw.reply_addr_chain.length;
        descr.start_address = rumboot_virt_to_dma(rmap_packet_ready->data_areas[rmap_packet_ready->count_areas]);
        descr.length        = rmap_packet_ready->data_area_sizes[rmap_packet_ready->count_areas];
        descr.act           = HSCB_ACT_TRAN;
        descr.act0          = HSCB_ACT0_NOLAST;
        descr.ie            = HSCB_DESCR_ITRPT_OFF;
        descr.valid         = HSCB_DESCR_VALID;
        descr.change_endian = rmap_packet_raw.change_endian;
        hscb_set_descr_in_mem(descr, (uint32_t)(rmap_packet_ready->array_of_descriptors + rmap_packet_ready->count_areas));
        rmap_packet_ready->count_areas++;
    }

    /*SW RMAP Second control chain with TxID, Initiator Addr, data length and addr etc (mandatory)*/
    if(rmap_packet_raw.data_chain.length >= (1 << 24))
        return PREPARE_RMAP_PACKET_DATA_LEN_OVERFLOW;
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_INITIATOR_LOGICAL_ADDR_i]
                                                                  = rmap_packet_raw.initiator_logical_addr;
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_TRANSACTION_ID_MS_i]
                                                                  =  (uint8_t)((rmap_packet_raw.transaction_id >> 8) & 0xFF);
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_TRANSACTION_ID_LS_i]
                                                                  =  (uint8_t)(rmap_packet_raw.transaction_id & 0xFF);
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_EXT_ADDR_i]
                                                                  =  rmap_packet_raw.ext_addr;
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_ADDR_3_i]
                                                                  =  (uint8_t)((rmap_packet_raw.addr >> 24) & 0xFF);
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_ADDR_2_i]
                                                                  =  (uint8_t)((rmap_packet_raw.addr >> 16) & 0xFF);
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_ADDR_1_i]
                                                                  =  (uint8_t)((rmap_packet_raw.addr >>  8) & 0xFF);
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_ADDR_0_i]
                                                                  =  (uint8_t)((rmap_packet_raw.addr >>  0) & 0xFF);
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_DATA_LEN_2_i]
                                                                  =  (uint8_t)((rmap_packet_raw.data_chain.length >> 16) & 0xFF);
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_DATA_LEN_1_i]
                                                                  =  (uint8_t)((rmap_packet_raw.data_chain.length >>  8) & 0xFF);
    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_DATA_LEN_0_i]
                                                                  =  (uint8_t)((rmap_packet_raw.data_chain.length >>  0) & 0xFF);

    // uint8_t data;
    /*We skip a chain of Target SW addresses if it is supplied*/
    index_for_header_CRC = (is_target_sw_addr_chain_supplied) ? initial_data_area_index + 1 : initial_data_area_index;
    /*We calculate CRC starting with Target Logical Address*/
    temporary_CRC = crc8(temporary_CRC, rmap_packet_ready->data_areas[index_for_header_CRC],
        rmap_packet_ready->data_area_sizes[index_for_header_CRC]);
    // for (int i = 0; i < rmap_packet_ready->data_area_sizes[index_for_header_CRC]; i++) {
    //     data = rmap_packet_ready->data_areas[index_for_header_CRC][i];
    //     temporary_CRC = crc8(temporary_CRC, &data, sizeof(data));
    // }

    /*CRC-8 from Reply Address*/
    if (is_reply_sw_addr_chain_supplied) {
        index_for_header_CRC++;
        temporary_CRC = crc8(temporary_CRC, rmap_packet_ready->data_areas[index_for_header_CRC],
            rmap_packet_ready->data_area_sizes[index_for_header_CRC]);
        // for( int i = 0; i < rmap_packet_ready->data_area_sizes[index_for_header_CRC]; i++) {
        //     data = rmap_packet_ready->data_areas[index_for_header_CRC][i];
        //     temporary_CRC = crc8(temporary_CRC, &data, sizeof(data));
        // }
    }

    /*CRC-8 from Initiator Logical Address up to the Header CRC (not including, so up to (size - 1))*/
    index_for_header_CRC++;
    temporary_CRC = crc8(temporary_CRC, rmap_packet_ready->data_areas[index_for_header_CRC],
        rmap_packet_ready->data_area_sizes[index_for_header_CRC]);
    // for (int i = 0; i < (rmap_packet_ready->data_area_sizes[index_for_header_CRC] - 1); i++) {
    //     data = rmap_packet_ready->data_areas[index_for_header_CRC][i];
    //     temporary_CRC = crc8(temporary_CRC, &data, sizeof(data));
    // }

    rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][HSCB_RMAP_HEADER_CRC8_i] = temporary_CRC;

    descr.start_address = rumboot_virt_to_dma(rmap_packet_ready->data_areas[rmap_packet_ready->count_areas]);
    descr.length        = rmap_packet_ready->data_area_sizes[rmap_packet_ready->count_areas];
    descr.act           = HSCB_ACT_TRAN;
    descr.act0          = (is_data_chain_supplied) ? HSCB_ACT0_NOLAST : HSCB_ACT0_LAST;
    descr.ie            = HSCB_DESCR_ITRPT_OFF;
    descr.valid         = HSCB_DESCR_VALID;
    descr.change_endian = rmap_packet_raw.change_endian;
    hscb_set_descr_in_mem(descr, (uint32_t)(rmap_packet_ready->array_of_descriptors + rmap_packet_ready->count_areas));
    rmap_packet_ready->count_areas++;

    /*for write and rmw commands we must have data if their length is not 0*/
    if(is_data_chain_supplied &&
            (rmap_packet_raw.instruction
                    & (HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_mask
                     | HSCB_RMAP_PACKET_INSTRUCTION_FIELD_VERIFY_DATA_mask)))
    {
        if(rmap_packet_raw.data_chain.array == NULL)
            return PREPARE_RMAP_PACKET_DATA_CHAIN_IS_NULL;

        rmap_packet_ready->data_areas[rmap_packet_ready->count_areas]
                                      = rmap_packet_raw.data_chain.array;
        rmap_packet_ready->data_area_sizes[rmap_packet_ready->count_areas]
                                      = rmap_packet_raw.data_chain.length;
        descr.start_address = rumboot_virt_to_dma(rmap_packet_ready->data_areas[rmap_packet_ready->count_areas]);
        descr.length        = rmap_packet_ready->data_area_sizes[rmap_packet_ready->count_areas];
        descr.act           = HSCB_ACT_TRAN;
        descr.act0          = HSCB_ACT0_NOLAST;
        descr.ie            = HSCB_DESCR_ITRPT_OFF;
        descr.valid         = HSCB_DESCR_VALID;
        descr.change_endian = rmap_packet_raw.change_endian;
        hscb_set_descr_in_mem(descr, (uint32_t)(rmap_packet_ready->array_of_descriptors + rmap_packet_ready->count_areas));

        temporary_CRC = crc8(0, rmap_packet_ready->data_areas[rmap_packet_ready->count_areas],
                                            rmap_packet_ready->data_area_sizes[rmap_packet_ready->count_areas]);
        rmap_packet_ready->count_areas++;

        rmap_packet_ready->data_areas[rmap_packet_ready->count_areas][0] = temporary_CRC;

        descr.start_address = rumboot_virt_to_dma(rmap_packet_ready->data_areas[rmap_packet_ready->count_areas]);
        descr.length        = rmap_packet_ready->data_area_sizes[rmap_packet_ready->count_areas];
        descr.act           = HSCB_ACT_TRAN;
        descr.act0          = HSCB_ACT0_LAST;
        descr.ie            = HSCB_DESCR_ITRPT_OFF;
        descr.valid         = HSCB_DESCR_VALID;
        descr.change_endian = rmap_packet_raw.change_endian;
        hscb_set_descr_in_mem(descr, (uint32_t)(rmap_packet_ready->array_of_descriptors + rmap_packet_ready->count_areas));
        rmap_packet_ready->count_areas++;
    }

    hscb_set_empty_descr_in_mem( (uint32_t)(rmap_packet_ready->array_of_descriptors + rmap_packet_ready->count_areas),
            rmap_packet_raw.change_endian);

    return PREPARE_RMAP_PACKET_OK;
}
