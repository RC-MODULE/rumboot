/*
 * hscb.c
 *
 *  Created on: Jul 19, 2018
 *      Author: r.galiulin
 */


#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <devices/hscb.h>
#include <regs/regs_hscb.h>

#include <platform/trace.h>


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
                iowrite32(change_endian(desc_data), sys_addr);
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
                iowrite32(change_endian(desc_addr), sys_addr);
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
        desc_data = change_endian(ioread32(sys_addr));

    else
        desc_data = ioread32(sys_addr);

    sys_addr += 4;

    if (endian)
        desc_addr = change_endian(ioread32(sys_addr));
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

bool hscb_reset(uint32_t base_addr)
{

    return true;
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
              ((cfg->en_hscb << HSCB_SETTINGS_RX_FIX_EN_i)   & HSCB_SETTINGS_RX_FIX_EN_mask),
              base_addr + HSCB_SETTINGS);
}

void hscb_get_config(uint32_t base_addr, hscb_cfg_t* cfg)
{
    uint32_t settings;
    settings = ioread32(base_addr + HSCB_SETTINGS);
    cfg->en_hscb     = (settings & HSCB_SETTINGS_EN_HSCB_mask)      >> HSCB_SETTINGS_EN_HSCB_i     ;
    cfg->tx_endian   = (settings & HSCB_SETTINGS_TX_ENDIAN_mask)    >> HSCB_SETTINGS_TX_ENDIAN_i   ;
    cfg->rx_endian   = (settings & HSCB_SETTINGS_RX_ENDIAN_mask)    >> HSCB_SETTINGS_RX_ENDIAN_i   ;
    cfg->time_mode   = (settings & HSCB_SETTINGS_TIME_MODE_mask)    >> HSCB_SETTINGS_TIME_MODE_i   ;
    cfg->loop_back   = (settings & HSCB_SETTINGS_LOOP_BACK_mask)    >> HSCB_SETTINGS_LOOP_BACK_i   ;
    cfg->en_rmap     = (settings & HSCB_SETTINGS_EN_RMAP_i)         >> HSCB_SETTINGS_EN_RMAP_i     ;
    cfg->en_trim_clk = (settings & HSCB_SETTINGS_EN_TRIM_CLK_mask)  >> HSCB_SETTINGS_EN_TRIM_CLK_i ;
    cfg->rx_fix_en   = (settings & HSCB_SETTINGS_RX_FIX_EN_mask)    >> HSCB_SETTINGS_RX_FIX_EN_i   ;
}


void hscb_set_rd_descr_in_mem(uint32_t sys_addr, hscb_descr_struct_t* descr)
{
    iowrite32( ( descr->start_address                            & HSCB_RD_DESCR_START_ADDRESS_mask), sys_addr + 4);
    iowrite32( ((descr->length << HSCB_RD_DESCR_START_ADDRESS_i) & HSCB_RD_DESCR_START_ADDRESS_mask) |
               ((descr->act    << HSCB_RD_DESCR_ACT_i)           & HSCB_RD_DESCR_ACT_mask)           |
               ((descr->act0   << HSCB_RD_DESCR_ACT0_i)          & HSCB_RD_DESCR_ACT0_mask)          |
               ((descr->ie     << HSCB_RD_DESCR_IE_i)            & HSCB_RD_DESCR_IE_mask)            |
               ((descr->err    << HSCB_RD_DESCR_ERR_i)           & HSCB_RD_DESCR_ERR_mask)           |
               ((descr->valid  << HSCB_RD_DESCR_VALID_i)         & HSCB_RD_DESCR_VALID_mask),
               sys_addr);
}

