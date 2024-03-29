#include <stdint.h>
#include <stdbool.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <regs/fields/emi.h>
#include <arch/common_macros.h>
#include <regs/regs_emi.h>
#include <assert.h>
#include <rumboot/io.h>

uint8_t calc_hamming_ecc(uint32_t data)
{
    static const
    uint32_t   hmat[7] = {HAMMING_MATRIX};
    uint32_t   t,i,m,s; /* (t)emp, (i)ndex, (m)ask, (s)hift */
    uint8_t    result;

    for(result=0,i=0;i<7;i++)
    {
        for(m=0xFFFF,t=hmat[i]&data,s=16;s;s>>=1,m>>=s)
            t = (t & m) ^ (t >> s);
        result |= t << i;
    }
    return result;
}

void emi_get_bank_cfg(uint32_t const emi_dcr_base, emi_bank_num const num_bank, emi_bank_cfg * bn_cfg)
{
    assert( num_bank != emi_bank_all);
    uint32_t val;

    val = dcr_read(emi_dcr_base + EMI_SS0 + num_bank*(EMI_SS1 - EMI_SS0));
    bn_cfg->ssx_cfg.BTYP   = (val & (0x7   << EMI_SSx_BTYP_i))   >> EMI_SSx_BTYP_i;
    bn_cfg->ssx_cfg.PTYP   = (val & (0x1   << EMI_SSx_PTYP_i))   >> EMI_SSx_PTYP_i;
    bn_cfg->ssx_cfg.SRDY   = (val & (0x1   << EMI_SSx_SRDY_i))   >> EMI_SSx_SRDY_i;
    bn_cfg->ssx_cfg.TWR    = (val & (0x1   << EMI_SSx_TWR_i))    >> EMI_SSx_TWR_i;
    bn_cfg->ssx_cfg.SST    = (val & (0x1   << EMI_SSx_SST_i))    >> EMI_SSx_SST_i;
    bn_cfg->ssx_cfg.T_SSOE = (val & (0x1   << EMI_SSx_T_SSOE_i)) >> EMI_SSx_T_SSOE_i;
    bn_cfg->ssx_cfg.T_SOE  = (val & (0x1   << EMI_SSx_T_SOE_i))  >> EMI_SSx_T_SOE_i;
    bn_cfg->ssx_cfg.T_CYC  = (val & (0xF   << EMI_SSx_T_CYC_i))  >> EMI_SSx_T_CYC_i;
    bn_cfg->ssx_cfg.T_RDY  = (val & (0x3FF << EMI_SSx_T_RDY_i))  >> EMI_SSx_T_RDY_i;
    bn_cfg->ssx_cfg.T_DEL  = (val & (0x3   << EMI_SSx_T_DEL_i))  >> EMI_SSx_T_DEL_i;

    val = dcr_read(emi_dcr_base + EMI_SD0 + num_bank*(EMI_SD1 - EMI_SD0));
    bn_cfg->sdx_cfg.CSP    = (val & (0x7 << EMI_SDx_CSP_i))   >> EMI_SDx_CSP_i;
    bn_cfg->sdx_cfg.SDS    = (val & (0x7 << EMI_SDx_SDS_i))   >> EMI_SDx_SDS_i;
    bn_cfg->sdx_cfg.CL     = (val & (0x3 << EMI_SDx_CL_i))    >> EMI_SDx_CL_i;
    bn_cfg->sdx_cfg.T_RDL  = (val & (0x1 << EMI_SDx_T_RDL_i)) >> EMI_SDx_T_RDL_i;
    bn_cfg->sdx_cfg.SI     = (val & (0x1 << EMI_SDx_SI_i))    >> EMI_SDx_SI_i;
    bn_cfg->sdx_cfg.T_RCD  = (val & (0x3 << EMI_SDx_T_RCD_i)) >> EMI_SDx_T_RCD_i;
    bn_cfg->sdx_cfg.T_RAS  = (val & (0x7 << EMI_SDx_T_RAS_i)) >> EMI_SDx_T_RAS_i;
}


void emi_set_bank_cfg (uint32_t const emi_dcr_base, emi_bank_num const num_bank, emi_bank_cfg const * bn_cfg)
{
    assert( num_bank != emi_bank_all);
    dcr_write (emi_dcr_base + EMI_SS0 + num_bank*(EMI_SS1 - EMI_SS0),  //write SSx
            ((bn_cfg->ssx_cfg.BTYP   << EMI_SSx_BTYP_i )  |
            ( bn_cfg->ssx_cfg.PTYP   << EMI_SSx_PTYP_i)   |
            ( bn_cfg->ssx_cfg.SRDY   << EMI_SSx_SRDY_i )  |
            ( bn_cfg->ssx_cfg.TWR    << EMI_SSx_TWR_i )   |
            ( bn_cfg->ssx_cfg.SST    << EMI_SSx_SST_i )   |
            ( bn_cfg->ssx_cfg.T_SSOE << EMI_SSx_T_SSOE_i )|
            ( bn_cfg->ssx_cfg.T_SOE  << EMI_SSx_T_SOE_i ) |
            ( bn_cfg->ssx_cfg.T_CYC  << EMI_SSx_T_CYC_i ) |
            ( bn_cfg->ssx_cfg.T_RDY  << EMI_SSx_T_RDY_i ) |
            ( bn_cfg->ssx_cfg.T_DEL  << EMI_SSx_T_DEL_i ))
            );
    dcr_write (emi_dcr_base + EMI_SD0 + num_bank*(EMI_SD1 - EMI_SD0), //write SDx
            ((bn_cfg->sdx_cfg.CSP    << EMI_SDx_CSP_i )   |
            ( bn_cfg->sdx_cfg.SDS    << EMI_SDx_SDS_i )   |
            ( bn_cfg->sdx_cfg.CL     << EMI_SDx_CL_i )    |
            ( bn_cfg->sdx_cfg.T_RDL  << EMI_SDx_T_RDL_i ) |
            ( bn_cfg->sdx_cfg.SI     << EMI_SDx_SI_i )    |
            ( bn_cfg->sdx_cfg.T_RCD  << EMI_SDx_T_RCD_i ) |
            ( bn_cfg->sdx_cfg.T_RAS  << EMI_SDx_T_RAS_i ))
            );
}

void emi_set_rfc(uint32_t const emi_dcr_base, emi_rfc_cfg const * rfc)
{
    dcr_write(emi_dcr_base + EMI_RFC,
                ( rfc->TRFC << EMI_RFC_TRFC_i ) |
                ( rfc->RP   << EMI_RFC_RP_i   )
             );
}

void emi_get_rfc(uint32_t const emi_dcr_base, emi_rfc_cfg * rfc)
{
    uint32_t val;
    val = dcr_read(emi_dcr_base + EMI_RFC);
    rfc->TRFC = ( val & (0x7    << EMI_RFC_TRFC_i)) >> EMI_RFC_TRFC_i;
    rfc->RP   = ( val & (0x3FFF << EMI_RFC_RP_i))   >> EMI_RFC_RP_i;
}

void emi_hiz_mode_on(uint32_t const emi_dcr_base)
{
    uint32_t val;
    val = dcr_read(emi_dcr_base + EMI_WECR);
    SET_BIT(val, EMI_WECR_OE_DIS_i);
    dcr_write(emi_dcr_base + EMI_WECR, val);
}

void emi_hiz_mode_off(uint32_t const emi_dcr_base)
{
    uint32_t val;
    val = dcr_read(emi_dcr_base + EMI_WECR);
    CLEAR_BIT(val, EMI_WECR_OE_DIS_i);
    dcr_write(emi_dcr_base + EMI_WECR, val);
}

/* (c) a.gurov  */
void emi_hsh_mode_on(uint32_t const emi_dcr_base)
{
    uint32_t val;
    val = dcr_read(emi_dcr_base + EMI_WECR);
    SET_BITS_BY_MASK(val,
            (1 << EMI_WECR_HSH_i)   |
            (1 << EMI_WECR_BWE0_i)  |
            (1 << EMI_WECR_BWE1_i)  |
            (1 << EMI_WECR_BWE2_i)  |
            (1 << EMI_WECR_BWE3_i)
            );
    dcr_write(emi_dcr_base + EMI_WECR, val);
}

/* (c) a.gurov  */
void emi_hsh_mode_off(uint32_t const emi_dcr_base)
{
    uint32_t val;
    val = dcr_read(emi_dcr_base + EMI_WECR);
    CLEAR_BITS_BY_MASK(val,
            (1 << EMI_WECR_HSH_i)   |
            (1 << EMI_WECR_BWE0_i)  |
            (1 << EMI_WECR_BWE1_i)  |
            (1 << EMI_WECR_BWE2_i)  |
            (1 << EMI_WECR_BWE3_i)
            );
    dcr_write(emi_dcr_base + EMI_WECR, val);
}

/* (c) a.gurov  */
void emi_ecc_write(uint32_t emi_dcr_base, uint32_t *word_addr, uint8_t  ecc_val)
{
    uint32_t wecr = dcr_read(emi_dcr_base + EMI_WECR);
    if(!GET_BIT(wecr, EMI_WECR_HSH_i)) emi_hsh_mode_on(emi_dcr_base);
    *word_addr = (uint32_t)ecc_val;
    if(!GET_BIT(wecr, EMI_WECR_HSH_i)) emi_hsh_mode_off(emi_dcr_base);
}

/* (c) a.gurov  */
void emi_ecc_read(uint32_t emi_dcr_base, uint32_t *word_addr, uint8_t *ecc_val)
{
    uint32_t wecr = dcr_read(emi_dcr_base + EMI_WECR);
    if(!GET_BIT(wecr, EMI_WECR_HSH_i)) emi_hsh_mode_on(emi_dcr_base);
    *ecc_val = (uint8_t)(*word_addr & ~0xFF);
    if(!GET_BIT(wecr, EMI_WECR_HSH_i)) emi_hsh_mode_off(emi_dcr_base);
}


void emi_enable_ext_rdy(uint32_t const emi_dcr_base, emi_bank_num const num_bank)
{
    emi_bank_cfg bank_cfg;
    assert( num_bank != emi_bank_all);
    emi_get_bank_cfg(emi_dcr_base, num_bank, &bank_cfg);
    bank_cfg.ssx_cfg.SRDY = SRDY_EXT_RDY_USE;
    emi_set_bank_cfg(emi_dcr_base, num_bank, &bank_cfg);
}

void emi_set_trdy(uint32_t const emi_dcr_base, emi_bank_num const num_bank, uint16_t const trdy)
{
    emi_bank_cfg bank_cfg;
    assert( num_bank != emi_bank_all);
    emi_get_bank_cfg(emi_dcr_base, num_bank, &bank_cfg);
    bank_cfg.ssx_cfg.T_RDY = trdy;
    emi_set_bank_cfg(emi_dcr_base, num_bank, &bank_cfg);
}

void emi_set_int_mask(uint32_t const emi_dcr_base, emi_imr_cfg const * mask)
{
    dcr_write(emi_dcr_base + EMI_IMR_SET,
                                            (mask->ME1B0  << EMI_IMR_ME1B0_i ) |
                                            (mask->ME1B0  << EMI_IMR_ME2B0_i ) |
                                            (mask->ME1B1  << EMI_IMR_ME1B1_i ) |
                                            (mask->ME1B1  << EMI_IMR_ME2B1_i ) |
                                            (mask->ME1B2  << EMI_IMR_ME1B2_i ) |
                                            (mask->ME1B2  << EMI_IMR_ME2B2_i ) |
                                            (mask->ME1B3  << EMI_IMR_ME1B3_i ) |
                                            (mask->ME1B3  << EMI_IMR_ME2B3_i ) |
                                            (mask->ME1B4  << EMI_IMR_ME1B4_i ) |
                                            (mask->ME1B4  << EMI_IMR_ME2B4_i ) |
                                            (mask->ME1B5  << EMI_IMR_ME1B5_i ) |
                                            (mask->ME1B5  << EMI_IMR_ME2B5_i ) |
                                            (mask->MRDYR  << EMI_IMR_MRDYR_i ) |
                                            (mask->MRDYW  << EMI_IMR_MRDYW_i ) |
                                            (mask->MERRAR << EMI_IMR_MERRAR_i) |
                                            (mask->MERRAW << EMI_IMR_MERRAW_i) |
                                            (mask->MERRDW << EMI_IMR_MERRDW_i)
            );
}

void emi_get_int_mask(uint32_t const emi_dcr_base, emi_imr_cfg * mask)
{
    uint32_t rd;
    rd = dcr_read(emi_dcr_base + EMI_IMR);
    mask->ME1B0  = (rd & (1 << EMI_IMR_ME1B0_i )) >> EMI_IMR_ME1B0_i;
    mask->ME1B0  = (rd & (1 << EMI_IMR_ME2B0_i )) >> EMI_IMR_ME2B0_i;
    mask->ME1B1  = (rd & (1 << EMI_IMR_ME1B1_i )) >> EMI_IMR_ME1B1_i;
    mask->ME1B1  = (rd & (1 << EMI_IMR_ME2B1_i )) >> EMI_IMR_ME2B1_i;
    mask->ME1B2  = (rd & (1 << EMI_IMR_ME1B2_i )) >> EMI_IMR_ME1B2_i;
    mask->ME1B2  = (rd & (1 << EMI_IMR_ME2B2_i )) >> EMI_IMR_ME2B2_i;
    mask->ME1B3  = (rd & (1 << EMI_IMR_ME1B3_i )) >> EMI_IMR_ME1B3_i;
    mask->ME1B3  = (rd & (1 << EMI_IMR_ME2B3_i )) >> EMI_IMR_ME2B3_i;
    mask->ME1B4  = (rd & (1 << EMI_IMR_ME1B4_i )) >> EMI_IMR_ME1B4_i;
    mask->ME1B4  = (rd & (1 << EMI_IMR_ME2B4_i )) >> EMI_IMR_ME2B4_i;
    mask->ME1B5  = (rd & (1 << EMI_IMR_ME1B5_i )) >> EMI_IMR_ME1B5_i;
    mask->ME1B5  = (rd & (1 << EMI_IMR_ME2B5_i )) >> EMI_IMR_ME2B5_i;
    mask->MRDYR  = (rd & (1 << EMI_IMR_MRDYR_i )) >> EMI_IMR_MRDYR_i;
    mask->MRDYW  = (rd & (1 << EMI_IMR_MRDYW_i )) >> EMI_IMR_MRDYW_i;
    mask->MERRAR = (rd & (1 << EMI_IMR_MERRAR_i)) >> EMI_IMR_MERRAR_i;
    mask->MERRAW = (rd & (1 << EMI_IMR_MERRAW_i)) >> EMI_IMR_MERRAW_i;
    mask->MERRDW = (rd & (1 << EMI_IMR_MERRDW_i)) >> EMI_IMR_MERRDW_i;
}

void emi_get_irr(uint32_t const emi_dcr_base, emi_irr_cfg * irr)
{
    uint32_t rd;
    rd = dcr_read(emi_dcr_base + EMI_IRR);
    irr->IE1B0  = (rd & (1 << EMI_IRR_IE1B0_i )) >> EMI_IRR_IE1B0_i;
    irr->IE1B0  = (rd & (1 << EMI_IRR_IE2B0_i )) >> EMI_IRR_IE2B0_i;
    irr->IE1B1  = (rd & (1 << EMI_IRR_IE1B1_i )) >> EMI_IRR_IE1B1_i;
    irr->IE1B1  = (rd & (1 << EMI_IRR_IE2B1_i )) >> EMI_IRR_IE2B1_i;
    irr->IE1B2  = (rd & (1 << EMI_IRR_IE1B2_i )) >> EMI_IRR_IE1B2_i;
    irr->IE1B2  = (rd & (1 << EMI_IRR_IE2B2_i )) >> EMI_IRR_IE2B2_i;
    irr->IE1B3  = (rd & (1 << EMI_IRR_IE1B3_i )) >> EMI_IRR_IE1B3_i;
    irr->IE1B3  = (rd & (1 << EMI_IRR_IE2B3_i )) >> EMI_IRR_IE2B3_i;
    irr->IE1B4  = (rd & (1 << EMI_IRR_IE1B4_i )) >> EMI_IRR_IE1B4_i;
    irr->IE1B4  = (rd & (1 << EMI_IRR_IE2B4_i )) >> EMI_IRR_IE2B4_i;
    irr->IE1B5  = (rd & (1 << EMI_IRR_IE1B5_i )) >> EMI_IRR_IE1B5_i;
    irr->IE1B5  = (rd & (1 << EMI_IRR_IE2B5_i )) >> EMI_IRR_IE2B5_i;
    irr->IRDYR  = (rd & (1 << EMI_IRR_IRDYR_i )) >> EMI_IRR_IRDYR_i;
    irr->IRDYW  = (rd & (1 << EMI_IRR_IRDYW_i )) >> EMI_IRR_IRDYW_i;
    irr->IERRAR = (rd & (1 << EMI_IRR_IERRAR_i)) >> EMI_IRR_IERRAR_i;
    irr->IERRAW = (rd & (1 << EMI_IRR_IERRAW_i)) >> EMI_IRR_IERRAW_i;
    irr->IERRDW = (rd & (1 << EMI_IRR_IERRDW_i)) >> EMI_IRR_IERRDW_i;
}

void emi_clear_irr(uint32_t const emi_dcr_base, emi_irr_cfg const * irr)
{
    dcr_write(emi_dcr_base + EMI_IRR_RST,
                                            (irr->IE1B0  << EMI_IRR_IE1B0_i ) |
                                            (irr->IE1B0  << EMI_IRR_IE2B0_i ) |
                                            (irr->IE1B1  << EMI_IRR_IE1B1_i ) |
                                            (irr->IE1B1  << EMI_IRR_IE2B1_i ) |
                                            (irr->IE1B2  << EMI_IRR_IE1B2_i ) |
                                            (irr->IE1B2  << EMI_IRR_IE2B2_i ) |
                                            (irr->IE1B3  << EMI_IRR_IE1B3_i ) |
                                            (irr->IE1B3  << EMI_IRR_IE2B3_i ) |
                                            (irr->IE1B4  << EMI_IRR_IE1B4_i ) |
                                            (irr->IE1B4  << EMI_IRR_IE2B4_i ) |
                                            (irr->IE1B5  << EMI_IRR_IE1B5_i ) |
                                            (irr->IE1B5  << EMI_IRR_IE2B5_i ) |
                                            (irr->IRDYR  << EMI_IRR_IRDYR_i ) |
                                            (irr->IRDYW  << EMI_IRR_IRDYW_i ) |
                                            (irr->IERRAR << EMI_IRR_IERRAR_i) |
                                            (irr->IERRAW << EMI_IRR_IERRAW_i) |
                                            (irr->IERRDW << EMI_IRR_IERRDW_i)
             );
}

void emi_set_ecc (uint32_t const emi_dcr_base, emi_bank_num const num_bank, emi_ecc_status const ecc_stat)
{
    if (num_bank == emi_bank_all )
    {
        if (ecc_stat == emi_ecc_off)
            dcr_write (emi_dcr_base + EMI_HSTSR, 0x00);
        else
            dcr_write (emi_dcr_base + EMI_HSTSR, 0x3F);
    }
    else
    {
        uint32_t tmp = dcr_read (emi_dcr_base + EMI_HSTSR);
        if (ecc_stat == emi_ecc_off)
        {
            tmp &= ~(1 << num_bank);
            dcr_write (emi_dcr_base + EMI_HSTSR, tmp);
        }
        else
        {
            tmp |= 1 << num_bank;
            dcr_write (emi_dcr_base + EMI_HSTSR, tmp);
        }
    }
}
