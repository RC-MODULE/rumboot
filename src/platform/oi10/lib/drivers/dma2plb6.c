#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/regs/regs_dma2plb6.h>
#include <platform/devices/dma2plb6.h>


/*
 * wait copy
 */
bool wait_dma2plb6_mcpy(dma2plb6_setup_info const * const setup_info, channel_status * const status)
{
    do{
        *status = dma2plb6_ch_get_status(setup_info->base_addr, setup_info->channel);
        if(status->error_detected)
            return false;
    }
    while(status->busy | !status->terminalcnt_reached);
    return true;
}

/*
 * copy
 */
bool dma2plb6_single_copy(dma2plb6_setup_info * const setup_info, channel_status * const status)
{
    dma2plb6_mcpy(setup_info);
    return wait_dma2plb6_mcpy(setup_info, status);
}

/*
 * copy for memory coherent requests
 */
bool dma2plb6_single_copy_coherency_required(dma2plb6_setup_info const * const setup_info, channel_status * const status)
{
    dma2plb6_mcpy_coherency_required(setup_info);
    return wait_dma2plb6_mcpy(setup_info, status);
}

/*
 * returns status of channel
 */
channel_status dma2plb6_ch_get_status(uint32_t const base_address, DmaChannel const channel)
{
    channel_status status;
    uint32_t sr = dcr_read(base_address + PLB6_DMA_SR);
    status.terminalcnt_reached = ((sr >> IBM_BIT_INDEX(32,CS)) >> IBM_BIT_INDEX(4,channel)) & 0x1;
    status.error_detected = (sr >> IBM_BIT_INDEX(32,RI0)) & 0x1;
    status.spec_error_status = (sr >> IBM_BIT_INDEX(32,RS0)) & 0x7;
    status.busy = ((sr >> IBM_BIT_INDEX(32,CB)) >> IBM_BIT_INDEX(4,channel)) & 0x1;
    status.sc_gth_status = ((sr >> IBM_BIT_INDEX(32,SG)) >> IBM_BIT_INDEX(4,channel)) & 0x1;
    return status;
}

/*
 * clear SR[CSn]
 *       SR[RIn]
 */
void dma2plb6_clear_interrupt(uint32_t const base_addr, DmaChannel const channel)
{
    dcr_write(base_addr + PLB6_DMA_SR, (1 << IBM_BIT_INDEX(32,CS)) << IBM_BIT_INDEX(4,channel));
}

void dma2plb6_clear_status_reg(uint32_t const base_addr)
{
    uint32_t reg = dcr_read (base_addr + PLB6_DMA_SR);
    dcr_write(base_addr + PLB6_DMA_SR, reg);
}

/*
 * initializes memory-to-memory single transfer for channel (setup_info->channel=0,1,2 or 3)
 * this function is not applicable for requests to coherent slaves
 * but for compatibility reasons it is left here
 */
void dma2plb6_mcpy(dma2plb6_setup_info * const setup_info)
{
    setup_info->snp_mode = snp_mode_off;
    dma2plb6_mcpy_coherency_required(setup_info);
}

/*
 * initializes memory-to-memory single transfer for channel (setup_info->channel=0,1,2 or 3)
 */
void dma2plb6_mcpy_coherency_required(dma2plb6_setup_info const * const setup_info)
{
    dma2plb6_disable_channel(setup_info->base_addr,setup_info->channel);
    dcr_write(get_addr(setup_info->channel,PLB6_DMA_SAH,setup_info->base_addr),(setup_info->source_adr >> 32));
    dcr_write(get_addr(setup_info->channel,PLB6_DMA_SAL,setup_info->base_addr),setup_info->source_adr & 0xFFFFFFFF);
    dcr_write(get_addr(setup_info->channel,PLB6_DMA_DAH,setup_info->base_addr),(setup_info->dest_adr >> 32));
    dcr_write(get_addr(setup_info->channel,PLB6_DMA_DAL,setup_info->base_addr),setup_info->dest_adr & 0xFFFFFFFF);
    //count < 0x000FFFFF
    dcr_write(get_addr(setup_info->channel,PLB6_DMA_CTC,setup_info->base_addr),setup_info->count);
    if(setup_info->striding_info.striding != striding_none){
        dcr_write(get_addr(setup_info->channel,PLB6_DMA_DSTR,setup_info->base_addr),
                setup_info->striding_info.sbs << IBM_BIT_INDEX(32,31) | setup_info->striding_info.tbs << IBM_BIT_INDEX(32,15));
    }
    //clear CS[x] and RIx in SR
    dcr_write(setup_info->base_addr + PLB6_DMA_SR, (1 << IBM_BIT_INDEX(32,(RI0 * (setup_info->channel+1)))) |
                                                   ((1 << IBM_BIT_INDEX(32,CS))) << IBM_BIT_INDEX(4, setup_info->channel));
    uint32_t cr = 0;
    //int mode, int enable
    cr |= setup_info->int_mode << IBM_BIT_INDEX(32,CIM);
    cr |= setup_info->tc_int_enable << IBM_BIT_INDEX(32,TCIE);
    cr |= setup_info->err_int_enable << IBM_BIT_INDEX(32,EIE);
    //transfer options
    cr |= setup_info->transfer_width << IBM_BIT_INDEX(32,PW);
    cr |= setup_info->striding_info.striding << IBM_BIT_INDEX(32,DSD);//setup DSD and DSS
    cr |= 1 << IBM_BIT_INDEX(32,TCE);//Enable terminal count
    cr |= setup_info->priority << IBM_BIT_INDEX(32,CP);
    cr |= setup_info->rw_transfer_size << IBM_BIT_INDEX(32,PF);
    cr |= setup_info->snp_mode << IBM_BIT_INDEX(32,DSC);
    dcr_write(get_addr(setup_info->channel,PLB6_DMA_CR,setup_info->base_addr),cr);
    //enable channel
    cr |= 1 << IBM_BIT_INDEX(32,CE);
    dcr_write(get_addr(setup_info->channel,PLB6_DMA_CR,setup_info->base_addr),cr);
}

void dma2plb6_mcpy_init(dma2plb6_setup_info const * const setup_info)
{
    dma2plb6_disable_channel(setup_info->base_addr, setup_info->channel);

    dcr_write(get_addr(setup_info->channel, PLB6_DMA_SAH,setup_info->base_addr), (setup_info->source_adr >> 32));
    dcr_write(get_addr(setup_info->channel, PLB6_DMA_SAL,setup_info->base_addr), setup_info->source_adr & 0xFFFFFFFF);
    dcr_write(get_addr(setup_info->channel, PLB6_DMA_DAH,setup_info->base_addr), (setup_info->dest_adr >> 32));
    dcr_write(get_addr(setup_info->channel, PLB6_DMA_DAL,setup_info->base_addr), setup_info->dest_adr & 0xFFFFFFFF);

    //count < 0x000FFFFF
    dcr_write(get_addr(setup_info->channel, PLB6_DMA_CTC, setup_info->base_addr), setup_info->count);

    if(setup_info->striding_info.striding != striding_none){
        dcr_write(get_addr(setup_info->channel, PLB6_DMA_DSTR, setup_info->base_addr),
                setup_info->striding_info.sbs << IBM_BIT_INDEX(32,31) | setup_info->striding_info.tbs << IBM_BIT_INDEX(32,15));
    }

    //clear CS[x] and RIx in SR
    dcr_write(setup_info->base_addr + PLB6_DMA_SR, (1 << IBM_BIT_INDEX(32,(RI0 * (setup_info->channel+1)))) |
                                                       ((1 << IBM_BIT_INDEX(32,CS))) << IBM_BIT_INDEX(4, setup_info->channel));

    uint32_t cr = 0;
    //int mode, int enable
    cr |= setup_info->int_mode << IBM_BIT_INDEX(32,CIM);
    cr |= setup_info->tc_int_enable << IBM_BIT_INDEX(32,TCIE);
    cr |= setup_info->err_int_enable << IBM_BIT_INDEX(32,EIE);
    //transfer options
    cr |= setup_info->transfer_width << IBM_BIT_INDEX(32,PW);
    cr |= setup_info->striding_info.striding << IBM_BIT_INDEX(32,DSD);//setup DSD and DSS
    cr |= 1 << IBM_BIT_INDEX(32,TCE);//Enable terminal count
    cr |= setup_info->priority << IBM_BIT_INDEX(32,CP);
    cr |= setup_info->rw_transfer_size << IBM_BIT_INDEX(32,PF);
    cr |= setup_info->snp_mode << IBM_BIT_INDEX(32,DSC);
    dcr_write(get_addr(setup_info->channel, PLB6_DMA_CR, setup_info->base_addr), cr);
}

void dma2plb6_enable_o_slv_err_interrupt(uint32_t const base_addr)
{
    dcr_write(base_addr + PLB6_DMA_OPTIONS, dcr_read(base_addr + PLB6_DMA_OPTIONS) | (1 << DMA_OPTIONS_SL_INT_i));
}

void dma2plb6_disable_o_slv_err_interrupt(uint32_t const base_addr)
{
    dcr_write(base_addr + PLB6_DMA_OPTIONS, dcr_read(base_addr + PLB6_DMA_OPTIONS) & ~(1 << DMA_OPTIONS_SL_INT_i));
}

void dma2plb6_enable_channel(uint32_t const base_addr, DmaChannel const channel)
{
    uint32_t cr_value = dcr_read(get_addr(channel, PLB6_DMA_CR, base_addr));
    dcr_write(get_addr(channel, PLB6_DMA_CR, base_addr), (cr_value | 1 << IBM_BIT_INDEX(32, CE)));
}

void dma2plb6_enable_all_channels(uint32_t const base_addr)
{
    uint32_t cr_value_0 = dcr_read(get_addr(channel0, PLB6_DMA_CR, base_addr));
    uint32_t cr_value_1 = dcr_read(get_addr(channel1, PLB6_DMA_CR, base_addr));
    uint32_t cr_value_2 = dcr_read(get_addr(channel2, PLB6_DMA_CR, base_addr));
    uint32_t cr_value_3 = dcr_read(get_addr(channel3, PLB6_DMA_CR, base_addr));

    dcr_write(get_addr(channel0, PLB6_DMA_CR, base_addr), (cr_value_0 | 1 << IBM_BIT_INDEX(32, CE)));
    dcr_write(get_addr(channel1, PLB6_DMA_CR, base_addr), (cr_value_1 | 1 << IBM_BIT_INDEX(32, CE)));
    dcr_write(get_addr(channel2, PLB6_DMA_CR, base_addr), (cr_value_2 | 1 << IBM_BIT_INDEX(32, CE)));
    dcr_write(get_addr(channel3, PLB6_DMA_CR, base_addr), (cr_value_3 | 1 << IBM_BIT_INDEX(32, CE)));
}

void dma2plb6_disable_channel(uint32_t const base_addr, DmaChannel const channel)
{
    dcr_write(get_addr(channel,PLB6_DMA_CR,base_addr),0x00000000);
}

void dma2plb6_disable_all_channel(uint32_t const base_addr)
{
    dma2plb6_disable_channel (base_addr, channel0);
    dma2plb6_disable_channel (base_addr, channel1);
    dma2plb6_disable_channel (base_addr, channel2);
    dma2plb6_disable_channel (base_addr, channel3);
}
