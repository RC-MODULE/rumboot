/*
 * hscb.c
 *
 *  Created on: Jul 19, 2018
 *      Author: r.galiulin
 */

#include <devices/hscb.h>

uint32_t hscb_change_endian (uint32_t data_in){
    uint32_t data_out = 0;
    rumboot_puthex(data_in);

    data_out = data_in << 24 & 0xff000000;
    data_out = data_out | (data_in << 8  & 0x00ff0000);
    data_out = data_out | (data_in >> 8  & 0x0000ff00);
    data_out = data_out | (data_in >> 24 & 0x000000ff);

    return data_out;
}

void hscb_convert_to_bytes (uint32_t* data_in, uint8_t* data_out, uint32_t len) {
    int i = 0;

    for (i=0; i<=len; i=i+8){
        data_out[i  ] = data_in[(i+4)/4];
        data_out[i+1] = data_in[(i+4)/4] >> 8 ;
        data_out[i+2] = data_in[(i+4)/4] >> 16;
        data_out[i+3] = data_in[(i+4)/4] >> 24;
        data_out[i+4] = data_in[i/4];
        data_out[i+5] = data_in[i/4] >> 8 ;
        data_out[i+6] = data_in[i/4] >> 16;
        data_out[i+7] = data_in[i/4] >> 24;
        }
}

uint32_t hscb_set_desc (uint32_t sys_addr, uint32_t len, uint32_t desc_addr, uint8_t* data_in, bool act0, bool interrupt, bool valid, bool endian){
    int i = 0;
    uint32_t tmp = 0;
    uint32_t desc_data = 0;
    rumboot_putstring ("Set descriptor ");
    rumboot_puthex (sys_addr);
    if (len == 0) {
        iowrite32(0x01000000, sys_addr); // "Stop" descriptor
    }
    else {
        desc_data = len << 6; // Set length
        desc_data = desc_data | 0x00000020;
        if (act0) desc_data = desc_data | 0x00000008;
        if (interrupt) desc_data = desc_data | 0x00000004;
        if (valid) desc_data = desc_data | 0x00000001;
            if (endian)
                iowrite32(hscb_change_endian(desc_data), sys_addr);
            else
                iowrite32(desc_data, sys_addr);
    }
    sys_addr += 4;

    if (len == 0) {
        iowrite32(0x00000000, sys_addr); // "Stop" descriptor
        iowrite32(0x00000000,sys_addr+4); // "Stop" descriptor
        sys_addr += 8;
        return sys_addr;
    }
    else {
            if (endian)
                iowrite32(hscb_change_endian(desc_addr), sys_addr);
            else
                iowrite32(desc_addr,sys_addr);
    }
    sys_addr += 4;

    tmp = (len / 4 + 2)*4;



        for (i=0; i<=tmp; i++){
            if (i <= len){
                iowrite8(data_in[i], desc_addr++);
            }
            else
                iowrite8(0, desc_addr++);
        }
    return sys_addr;
}

uint32_t hscb_get_desc (uint32_t sys_addr, uint8_t* data_out, uint32_t* len,  bool* act0, bool* interrupt, bool* end, bool* valid, bool endian){
    int i = 0;
    uint32_t desc_addr = 0;
    uint32_t desc_data = 0;
    uint32_t desc_len = 0;
    rumboot_putstring( "Get descriptor " );
    rumboot_puthex (sys_addr);
    if (endian)
        desc_data = hscb_change_endian(ioread32(sys_addr));

    else
        desc_data = ioread32(sys_addr);

    sys_addr += 4;

    if (endian)
        desc_addr = hscb_change_endian(ioread32(sys_addr));
    else
        desc_addr = ioread32(sys_addr);

    sys_addr += 4;
    desc_len = desc_data >> 6;
    *len = desc_len;
    if(desc_data & 0x00000010){
        *act0      = 0;
        *interrupt = 0;
        *end       = 0;
        *valid     = 0;
        if(desc_data & 0x00000008) *act0      = 1;
        if(desc_data & 0x00000004) *interrupt = 1;
        if(desc_data & 0x00000002) *end       = 1;
        if(desc_data & 0x00000001) *valid     = 1;
        for (i=0; i<=desc_len-1; i++){
            data_out[i] = ioread8(desc_addr++);
        }
    }
    else {
        sys_addr = 0xffffffff;
    }
    return sys_addr;
}

bool hscb_sw_rst(uint32_t base_addr)
{
    uint32_t time = 0;
    iowrite32(1, base_addr + HSCB_SW_RESET);
    while ((ioread32(base_addr + HSCB_SW_RESET)!=0) && (time++<HSCB_SW_RESET_TIMEOUT));
    if (time<HSCB_SW_RESET_TIMEOUT)
        return true;
    else
        return false;
}

void hscb_set_config(uint32_t base_addr, hscb_cfg_t* cfg)
{
    iowrite32(((cfg->en_hscb << HSCB_SETTINGS_EN_HSCB_i)     & HSCB_SETTINGS_EN_HSCB_mask)     |
              ((cfg->en_hscb << HSCB_SETTINGS_TX_ENDIAN_i)   & HSCB_SETTINGS_TX_ENDIAN_mask)   |
              ((cfg->en_hscb << HSCB_SETTINGS_RX_ENDIAN_i)   & HSCB_SETTINGS_RX_ENDIAN_mask)   |
              ((cfg->en_hscb << HSCB_SETTINGS_TIME_MODE_i)   & HSCB_SETTINGS_TIME_MODE_mask)   |
              ((cfg->en_hscb << HSCB_SETTINGS_LOOP_BACK_i)   & HSCB_SETTINGS_LOOP_BACK_mask)   |
              ((cfg->en_hscb << HSCB_SETTINGS_EN_RMAP_i)     & HSCB_SETTINGS_EN_RMAP_i)        |
              ((cfg->en_hscb << HSCB_SETTINGS_EN_TRIM_CLK_i) & HSCB_SETTINGS_EN_TRIM_CLK_mask) |
              ((cfg->en_hscb << HSCB_SETTINGS_RX_FIX_EN_i)   & HSCB_SETTINGS_RX_FIX_EN_mask)   | (1 << 24),
              base_addr + HSCB_SETTINGS);
}

void hscb_get_config(uint32_t base_addr, hscb_cfg_t* cfg)
{
    uint32_t settings;
    settings = ioread32(base_addr + HSCB_SETTINGS);
    rumboot_printf("HSCB_SETTINGS = 0x%X\n", settings);
    cfg->en_hscb     = (settings & HSCB_SETTINGS_EN_HSCB_mask)      >> HSCB_SETTINGS_EN_HSCB_i     ;
    cfg->tx_endian   = (settings & HSCB_SETTINGS_TX_ENDIAN_mask)    >> HSCB_SETTINGS_TX_ENDIAN_i   ;
    cfg->rx_endian   = (settings & HSCB_SETTINGS_RX_ENDIAN_mask)    >> HSCB_SETTINGS_RX_ENDIAN_i   ;
    cfg->time_mode   = (settings & HSCB_SETTINGS_TIME_MODE_mask)    >> HSCB_SETTINGS_TIME_MODE_i   ;
    cfg->loop_back   = (settings & HSCB_SETTINGS_LOOP_BACK_mask)    >> HSCB_SETTINGS_LOOP_BACK_i   ;
    cfg->en_rmap     = (settings & HSCB_SETTINGS_EN_RMAP_i)         >> HSCB_SETTINGS_EN_RMAP_i     ;
    cfg->en_trim_clk = (settings & HSCB_SETTINGS_EN_TRIM_CLK_mask)  >> HSCB_SETTINGS_EN_TRIM_CLK_i ;
    cfg->rx_fix_en   = (settings & HSCB_SETTINGS_RX_FIX_EN_mask)    >> HSCB_SETTINGS_RX_FIX_EN_i   ;
}

void hscb_set_descr_in_mem_tx(uint32_t sys_addr, uint32_t src_data_addr, uint32_t len)
{
    uint32_t buf;
    hscb_descr_struct_t descr;

    buf = hscb_change_endian(src_data_addr);
    iowrite32( buf, sys_addr + 4);

    descr.start_address = src_data_addr;
    descr.length = len;
    descr.act  = HSCB_ACT_TRAN;
    descr.act0 = HSCB_ACT0_LAST;
    descr.ie = true;
    descr.valid = true;
    buf = hscb_change_endian(((descr.length << HSCB_RD_DESCR_LENGTH_i) & HSCB_RD_DESCR_LENGTH_mask)   |
                             ((descr.act    << HSCB_RD_DESCR_ACT_i)    & HSCB_RD_DESCR_ACT_mask)      |
                             ((descr.act0   << HSCB_RD_DESCR_ACT0_i)   & HSCB_RD_DESCR_ACT0_mask)     |
                             ((descr.ie     << HSCB_RD_DESCR_IE_i)     & HSCB_RD_DESCR_IE_mask)       |
                             ((descr.valid  << HSCB_RD_DESCR_VALID_i)  & HSCB_RD_DESCR_VALID_mask));
    iowrite32( buf, sys_addr);

    rumboot_printf("Preparing descriptor: MEM[0x%x] = 0x%x\n", sys_addr,     ioread32(sys_addr));
    rumboot_printf("Preparing descriptor: MEM[0x%x] = 0x%x\n", sys_addr + 4, ioread32(sys_addr + 4));
}

void hscb_set_descr_in_mem_rx(uint32_t sys_addr, uint32_t src_data_addr, uint32_t len)
{
    uint32_t buf;
    hscb_descr_struct_t descr;

    buf = hscb_change_endian(src_data_addr);
    iowrite32( buf, sys_addr + 4);

    descr.start_address = src_data_addr;
    descr.length = len;
    descr.act  = HSCB_ACT_TRAN;
    descr.ie = true;
    buf = hscb_change_endian(((descr.length << HSCB_RD_DESCR_LENGTH_i) & HSCB_RD_DESCR_LENGTH_mask)   |
                             ((descr.act    << HSCB_RD_DESCR_ACT_i)    & HSCB_RD_DESCR_ACT_mask)      |
                             ((descr.ie     << HSCB_RD_DESCR_IE_i)     & HSCB_RD_DESCR_IE_mask));
    iowrite32( buf, sys_addr);

    rumboot_printf("Preparing descriptor: MEM[0x%x] = 0x%x\n", sys_addr,     ioread32(sys_addr));
    rumboot_printf("Preparing descriptor: MEM[0x%x] = 0x%x\n", sys_addr + 4, ioread32(sys_addr + 4));
}

uint32_t hscb_get_status(uint32_t base_addr)
{
    return ioread32(base_addr + HSCB_STATUS);
}

void hscb_set_irq_mask(uint32_t base_addr, uint32_t mask)
{
    iowrite32(mask, base_addr + HSCB_IRQ_MASK);
}

void hscb_set_timings(uint32_t base_addr, hscb_timings_cfg_t* cfg)
{
    iowrite32(((cfg->freq      << HSCB_TRANS_CLK_FREQ_i)      & HSCB_TRANS_CLK_FREQ_mask)      |
              ((cfg->init_freq << HSCB_TRANS_CLK_INIT_FREQ_i) & HSCB_TRANS_CLK_INIT_FREQ_mask) |
              ((cfg->limit     << HSCB_TRANS_CLK_LIMIT_i)     & HSCB_TRANS_CLK_LIMIT_mask),
              base_addr + HSCB_TRANS_CLK);
    iowrite32(((cfg->time_64      << HSCB_TIMINGS_TIME_64_i)      & HSCB_TIMINGS_TIME_64_mask)      |
              ((cfg->time_128     << HSCB_TIMINGS_TIME_128_i)     & HSCB_TIMINGS_TIME_128_i)        |
              ((cfg->silence_time << HSCB_TIMINGS_SILENCE_TIME_i) & HSCB_TIMINGS_SILENCE_TIME_mask),
              base_addr + HSCB_TIMINGS);
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
    iowrite32( ( settings->rw_desc_int       << HSCB_RWDMA_SETTINGS_DESC_INT_i )          |
               ( settings->rw_desc_end       << HSCB_RWDMA_SETTINGS_DESC_END_i )          |
               ( settings->rw_bad_desc       << HSCB_RWDMA_SETTINGS_BAD_DESC_i )          |
               ( settings->rw_axi_error      << HSCB_RWDMA_SETTINGS_RW_AXI_ERROR_i )      |
               ( settings->rw_buf_full       << HSCB_RWDMA_SETTINGS_RW_BUF_FULL_i )       |
               ( settings->rw_buf_empty      << HSCB_RWDMA_SETTINGS_RW_BUF_EMPTY_i )      |
               ( settings->en_rwdma          << HSCB_RWDMA_SETTINGS_EN_RWDMA_i )          |
               ( settings->en_rwdma_desc_tbl << HSCB_RWDMA_SETTINGS_EN_RWDMA_DESC_TBL_i ) |
               ( settings->rwdma_long_len    << HSCB_RWDMA_SETTINGS_RWDMA_LONG_LEN_i )    |
               ( settings->cancel_rwdma      << HSCB_RWDMA_SETTINGS_CANCEL_RWDMA_i ),
                base_addr + HSCB_RDMA_SETTINGS         );
}

void hscb_get_rdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings)
{
    uint32_t val;
    val = ioread32(base_addr+ HSCB_RDMA_SETTINGS);
    settings->rw_desc_int       = ( val & HSCB_RWDMA_SETTINGS_DESC_INT_mask )          >> HSCB_RWDMA_SETTINGS_DESC_INT_i;
    settings->rw_desc_end       = ( val & HSCB_RWDMA_SETTINGS_DESC_END_mask )          >> HSCB_RWDMA_SETTINGS_DESC_END_i;
    settings->rw_bad_desc       = ( val & HSCB_RWDMA_SETTINGS_BAD_DESC_mask )          >> HSCB_RWDMA_SETTINGS_BAD_DESC_i;
    settings->rw_axi_error      = ( val & HSCB_RWDMA_SETTINGS_RW_AXI_ERROR_mask )      >> HSCB_RWDMA_SETTINGS_RW_AXI_ERROR_i;
    settings->rw_buf_full       = ( val & HSCB_RWDMA_SETTINGS_RW_BUF_FULL_mask )       >> HSCB_RWDMA_SETTINGS_RW_BUF_FULL_i;
    settings->rw_buf_empty      = ( val & HSCB_RWDMA_SETTINGS_RW_BUF_EMPTY_mask )      >> HSCB_RWDMA_SETTINGS_RW_BUF_EMPTY_i;
    settings->en_rwdma          = ( val & HSCB_RWDMA_SETTINGS_EN_RWDMA_mask )          >> HSCB_RWDMA_SETTINGS_EN_RWDMA_i;
    settings->en_rwdma_desc_tbl = ( val & HSCB_RWDMA_SETTINGS_EN_RWDMA_DESC_TBL_mask ) >> HSCB_RWDMA_SETTINGS_EN_RWDMA_DESC_TBL_i;
    settings->rwdma_long_len    = ( val & HSCB_RWDMA_SETTINGS_RWDMA_LONG_LEN_mask )    >> HSCB_RWDMA_SETTINGS_RWDMA_LONG_LEN_i;
    settings->cancel_rwdma      = ( val & HSCB_RWDMA_SETTINGS_CANCEL_RWDMA_mask )      >> HSCB_RWDMA_SETTINGS_CANCEL_RWDMA_i;
}

void hscb_set_wdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings)
{
    iowrite32( ( settings->rw_desc_int       << HSCB_RWDMA_SETTINGS_DESC_INT_i )          |
               ( settings->rw_desc_end       << HSCB_RWDMA_SETTINGS_DESC_END_i )          |
               ( settings->rw_bad_desc       << HSCB_RWDMA_SETTINGS_BAD_DESC_i )          |
               ( settings->rw_axi_error      << HSCB_RWDMA_SETTINGS_RW_AXI_ERROR_i )      |
               ( settings->rw_buf_full       << HSCB_RWDMA_SETTINGS_RW_BUF_FULL_i )       |
               ( settings->rw_buf_empty      << HSCB_RWDMA_SETTINGS_RW_BUF_EMPTY_i )      |
               ( settings->en_rwdma          << HSCB_RWDMA_SETTINGS_EN_RWDMA_i )          |
               ( settings->en_rwdma_desc_tbl << HSCB_RWDMA_SETTINGS_EN_RWDMA_DESC_TBL_i ) |
               ( settings->rwdma_long_len    << HSCB_RWDMA_SETTINGS_RWDMA_LONG_LEN_i )    |
               ( settings->cancel_rwdma      << HSCB_RWDMA_SETTINGS_CANCEL_RWDMA_i ),
                base_addr + HSCB_WDMA_SETTINGS         );
}

void hscb_get_wdma_settings(uint32_t base_addr, hscb_rwdma_settings_t* settings)
{
    uint32_t val;
    val = ioread32(base_addr+ HSCB_WDMA_SETTINGS);
    settings->rw_desc_int       = ( val & HSCB_RWDMA_SETTINGS_DESC_INT_mask )          >> HSCB_RWDMA_SETTINGS_DESC_INT_i;
    settings->rw_desc_end       = ( val & HSCB_RWDMA_SETTINGS_DESC_END_mask )          >> HSCB_RWDMA_SETTINGS_DESC_END_i;
    settings->rw_bad_desc       = ( val & HSCB_RWDMA_SETTINGS_BAD_DESC_mask )          >> HSCB_RWDMA_SETTINGS_BAD_DESC_i;
    settings->rw_axi_error      = ( val & HSCB_RWDMA_SETTINGS_RW_AXI_ERROR_mask )      >> HSCB_RWDMA_SETTINGS_RW_AXI_ERROR_i;
    settings->rw_buf_full       = ( val & HSCB_RWDMA_SETTINGS_RW_BUF_FULL_mask )       >> HSCB_RWDMA_SETTINGS_RW_BUF_FULL_i;
    settings->rw_buf_empty      = ( val & HSCB_RWDMA_SETTINGS_RW_BUF_EMPTY_mask )      >> HSCB_RWDMA_SETTINGS_RW_BUF_EMPTY_i;
    settings->en_rwdma          = ( val & HSCB_RWDMA_SETTINGS_EN_RWDMA_mask )          >> HSCB_RWDMA_SETTINGS_EN_RWDMA_i;
    settings->en_rwdma_desc_tbl = ( val & HSCB_RWDMA_SETTINGS_EN_RWDMA_DESC_TBL_mask ) >> HSCB_RWDMA_SETTINGS_EN_RWDMA_DESC_TBL_i;
    settings->rwdma_long_len    = ( val & HSCB_RWDMA_SETTINGS_RWDMA_LONG_LEN_mask )    >> HSCB_RWDMA_SETTINGS_RWDMA_LONG_LEN_i;
    settings->cancel_rwdma      = ( val & HSCB_RWDMA_SETTINGS_CANCEL_RWDMA_mask )      >> HSCB_RWDMA_SETTINGS_CANCEL_RWDMA_i;
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
    hscb_set_descr_in_mem_tx(desc_addr, src_data_addr, len);

    rumboot_printf("Setting RDMA_TBL_SIZE and RDMA_SYS_ADDR\n");
    hscb_set_rdma_tbl_size(base_addr, 8);
    hscb_set_rdma_sys_addr(base_addr, rumboot_virt_to_dma((uint32_t *) desc_addr));
}

void hscb_configure_for_receive(uint32_t base_addr, uint32_t dst_data_addr, uint32_t len, uint32_t desc_addr)
{
    rumboot_printf("Setting receive descriptor to addr 0x%X\n", desc_addr);
    hscb_set_descr_in_mem_rx(desc_addr, dst_data_addr, len);

    rumboot_printf("Setting WDMA_TBL_SIZE and WDMA_SYS_ADDR\n");
    hscb_set_wdma_tbl_size(base_addr, 8);
    hscb_set_wdma_sys_addr(base_addr, rumboot_virt_to_dma((uint32_t *) desc_addr));
}

void hscb_enable(uint32_t base_addr)
{
    hscb_cfg_t hscb_cfg;
    rumboot_printf("Enable HSCB\n");
    hscb_get_config(base_addr, &hscb_cfg);
    hscb_cfg.en_hscb = true;
    hscb_set_config(base_addr, &hscb_cfg);
}

void hscb_run_rdma(uint32_t base_addr)
{
    hscb_rwdma_settings_t rdma_settings;
    rumboot_printf("Run RDMA\n");
    hscb_get_rdma_settings(base_addr, &rdma_settings);
    rdma_settings.rw_desc_int = true;
    rdma_settings.rw_desc_end = true;
    rdma_settings.rw_bad_desc = true;
    rdma_settings.en_rwdma = true;
    rdma_settings.en_rwdma_desc_tbl = true;
    hscb_set_rdma_settings(base_addr, &rdma_settings);
}

void hscb_run_wdma(uint32_t base_addr)
{
    hscb_rwdma_settings_t wdma_settings;
    rumboot_printf("Run WDMA\n");
    hscb_get_wdma_settings(base_addr, &wdma_settings);
    wdma_settings.rw_desc_int = true;
    wdma_settings.rw_desc_end = true;
    wdma_settings.rw_bad_desc = true;
    wdma_settings.en_rwdma = true;
    wdma_settings.en_rwdma_desc_tbl = true;
    hscb_set_rdma_settings(base_addr, &wdma_settings);
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
    timings.freq = 1;
    hscb_set_timings(base_addr, &timings);
}

bool hscb_transmit(uint32_t base_addr, uint32_t src_data_addr, uint32_t len, uint32_t desc_addr)
{
    uint8_t hscb_idx = GET_HSCB_IDX_BY_ADDR(base_addr);

    rumboot_printf("HSCB%d(0x%x) will transmit %d bytes from address 0x%X\n", hscb_idx, base_addr, len, src_data_addr);
    hscb_configure_for_transmit(base_addr, src_data_addr, len, desc_addr);
    //hscb_set_max_speed(base_addr);
    hscb_enable(base_addr);

    rumboot_printf("Waiting ACTIVE_LINK status\n");
    if (!hscb_wait_status(base_addr, HSCB_STATUS_ACTIVE_LINK_mask)) return false;

    hscb_run_rdma(base_addr);

    return true;
}

bool hscb_receive(uint32_t base_addr, uint32_t dst_data_addr, uint32_t len, uint32_t desc_addr)
{
    uint8_t hscb_idx = GET_HSCB_IDX_BY_ADDR(base_addr);

    rumboot_printf("HSCB%d(0x%x) will receive %d bytes to address 0x%X\n", hscb_idx, base_addr, len, dst_data_addr);

    hscb_configure_for_transmit(base_addr, dst_data_addr, len, desc_addr);
    //hscb_set_max_speed(base_addr);
    hscb_enable(base_addr);

    rumboot_printf("Waiting ACTIVE_LINK status\n");
    if (!hscb_wait_status(base_addr, HSCB_STATUS_ACTIVE_LINK_mask)) return false;

    hscb_run_rdma(base_addr);

    return true;
}
