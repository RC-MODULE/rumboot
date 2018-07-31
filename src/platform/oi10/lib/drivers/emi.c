/*
 * emi.c
 *
 *  Created on: Jul 25, 2018
 *      Author: s.chernousov
 */

#include <stdint.h>
#include <stdbool.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/regs/fields/emi.h>
#include <platform/common_macros/common_macros.h>
#include <platform/regs/regs_emi.h>

void emi_get_bank_cfg(uint8_t num_bank, uint32_t emi_base, emi_bank_cfg* bn_cfg)
{
    uint32_t val;

    val = dcr_read(emi_base + EMI_SS0 + num_bank*(EMI_SS1 - EMI_SS0));
    bn_cfg->ssx_cfg.BTYP   = val & 0x7;
    bn_cfg->ssx_cfg.PTYP   = (val & (0x1   << EMI_SSx_PTYP_i))   >> EMI_SSx_PTYP_i;
    bn_cfg->ssx_cfg.SRDY   = (val & (0x1   << EMI_SSx_SRDY_i))   >> EMI_SSx_SRDY_i;
    bn_cfg->ssx_cfg.TWR    = (val & (0x1   << EMI_SSx_TWR_i))    >> EMI_SSx_TWR_i;
    bn_cfg->ssx_cfg.SST    = (val & (0x1   << EMI_SSx_SST_i))    >> EMI_SSx_SST_i;
    bn_cfg->ssx_cfg.T_SSOE = (val & (0x1   << EMI_SSx_T_SSOE_i)) >> EMI_SSx_T_SSOE_i;
    bn_cfg->ssx_cfg.T_SOE  = (val & (0x1   << EMI_SSx_T_SOE_i))  >> EMI_SSx_T_SOE_i;
    bn_cfg->ssx_cfg.T_CYC  = (val & (0xF   << EMI_SSx_T_CYC_i))  >> EMI_SSx_T_CYC_i;
    bn_cfg->ssx_cfg.T_RDY  = (val & (0x3FF << EMI_SSx_T_RDY_i))  >> EMI_SSx_T_RDY_i;
    bn_cfg->ssx_cfg.T_DEL  = (val & (0x3   << EMI_SSx_T_DEL_i))  >> EMI_SSx_T_DEL_i;

    val = dcr_read(emi_base + EMI_SD0 + num_bank*(EMI_SD1 - EMI_SD0));
    bn_cfg->sdx_cfg.CSP    = (val &  0x7);
    bn_cfg->sdx_cfg.SDS    = (val & (0x7 << EMI_SDx_SDS_i))   >> EMI_SDx_SDS_i;
    bn_cfg->sdx_cfg.CL     = (val & (0x3 << EMI_SDx_CL_i))    >> EMI_SDx_CL_i;
    bn_cfg->sdx_cfg.T_RDL  = (val & (0x1 << EMI_SDx_T_RDL_i)) >> EMI_SDx_T_RDL_i;
    bn_cfg->sdx_cfg.SI     = (val & (0x1 << EMI_SDx_SI_i))    >> EMI_SDx_SI_i;
    bn_cfg->sdx_cfg.T_RCD  = (val & (0x3 << EMI_SDx_T_RCD_i)) >> EMI_SDx_T_RCD_i;
    bn_cfg->sdx_cfg.T_RAS  = (val & (0x7 << EMI_SDx_T_RAS_i)) >> EMI_SDx_T_RAS_i;
}


void emi_set_bank_cfg (uint8_t num_bank, uint32_t emi_base, emi_bank_cfg* bn_cfg)
{
    dcr_write (emi_base + EMI_SS0 + num_bank*(EMI_SS1 - EMI_SS0),  //write SSx
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
    dcr_write (emi_base + EMI_SD0 + num_bank*(EMI_SD1 - EMI_SD0), //write SDx
            ((bn_cfg->sdx_cfg.CSP    << EMI_SDx_CSP_i )   |
            ( bn_cfg->sdx_cfg.SDS    << EMI_SDx_SDS_i )   |
            ( bn_cfg->sdx_cfg.CL     << EMI_SDx_CL_i )    |
            ( bn_cfg->sdx_cfg.T_RDL  << EMI_SDx_T_RDL_i ) |
            ( bn_cfg->sdx_cfg.SI     << EMI_SDx_SI_i )    |
            ( bn_cfg->sdx_cfg.T_RCD  << EMI_SDx_T_RCD_i ) |
            ( bn_cfg->sdx_cfg.T_RAS  << EMI_SDx_T_RAS_i ))
            );
}

void emi_init_impl (uint32_t emi_dcr_base, uint32_t plb6mcif2_dcr_base, uint32_t puaba)
{
    rumboot_printf("Init EMI\n");
    //init bridge
    plb6mcif2_simple_init( plb6mcif2_dcr_base,  puaba );

    //init bank0 - SRAM0
    emi_bank_cfg b0_cfg =
    {
       //SS0
       {
           BTYP_SRAM,
           PTYP_NO_PAGES,
           SRDY_EXT_RDY_NOT_USE,
           TWR_0,
           SST_Flow_Through,
           TSSOE_1,
           TSOE_1,
           TCYC_8,
           0, //T_RDY
           TDEL_0
       },
       //SD0
       {
           CSP_256,
           SDS_2M,
           CL_3,
           TRDL_1,
           SI_EXT_INIT,
           TRCD_5,
           TRAS_9
       }
    };
    emi_set_bank_cfg(0, emi_dcr_base, &b0_cfg);

    //init bank1
    //TODO:
    //init bank2
    //TODO:
    //init bank3
    //TODO:
    //init bank4 - SRAM1
    emi_bank_cfg b4_cfg =
    {
       //SS0
       {
           BTYP_SRAM,
           PTYP_NO_PAGES,
           SRDY_EXT_RDY_NOT_USE,
           TWR_0,
           SST_Flow_Through,
           TSSOE_1,
           TSOE_1,
           TCYC_8,
           0, //T_RDY
           TDEL_0
       },
       //SD0
       {
           CSP_256,
           SDS_2M,
           CL_3,
           TRDL_1,
           SI_EXT_INIT,
           TRCD_5,
           TRAS_9
       }
    };
    emi_set_bank_cfg(0, emi_dcr_base, &b4_cfg);

    //init bank5 - NOR
    emi_bank_cfg b5_cfg =
    {
        //SS5
        {
            BTYP_SRAM,
            PTYP_NO_PAGES,
            SRDY_EXT_RDY_NOT_USE,
            TWR_0,
            SST_Flow_Through,
            TSSOE_1,
            TSOE_1,
            TCYC_8,
            0, //T_RDY
            TDEL_0
        },
        //SD5
        {
            CSP_256,
            SDS_2M,
            CL_3,
            TRDL_1,
            SI_EXT_INIT,
            TRCD_5,
            TRAS_9
        }
    };
    emi_set_bank_cfg(5, emi_dcr_base, &b5_cfg);

    dcr_write(emi_dcr_base + EMI_BUSEN, 0x01);
}

void emi_init ()
{
    emi_init_impl (DCR_EM2_EMI_BASE, DCR_EM2_PLB6MCIF2_BASE, 0x00);
}
