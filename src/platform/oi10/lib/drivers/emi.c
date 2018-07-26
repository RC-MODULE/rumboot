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

void emi_init_bank (uint8_t num_bank, uint32_t emi_base, emi_init_bank_cfg bn_cfg)
{
    dcr_write (emi_base + EMI_SS0 + num_bank*(EMI_SS1 - EMI_SS0),  //write SSx
            ((bn_cfg.ssx_cfg.BTYP   << EMI_SSx_BTYP_i )  |
            ( bn_cfg.ssx_cfg.PTYP   << EMI_SSx_PTYP_i)   |
            ( bn_cfg.ssx_cfg.SRDY   << EMI_SSx_SRDY_i )  |
            ( bn_cfg.ssx_cfg.TWR    << EMI_SSx_TWR_i )   |
            ( bn_cfg.ssx_cfg.SST    << EMI_SSx_SST_i )   |
            ( bn_cfg.ssx_cfg.T_SSOE << EMI_SSx_T_SSOE_i )|
            ( bn_cfg.ssx_cfg.T_SOE  << EMI_SSx_T_SOE_i ) |
            ( bn_cfg.ssx_cfg.T_CYC  << EMI_SSx_T_CYC_i ) |
            ( bn_cfg.ssx_cfg.T_RDY  << EMI_SSx_T_RDY_i ) |
            ( bn_cfg.ssx_cfg.T_DEL  << EMI_SSx_T_DEL_i ) |
            ( bn_cfg.ssx_cfg.UWEB   << EMI_SSx_UWEB_i ))
            );
    dcr_write (emi_base + EMI_SD0 + num_bank*(EMI_SD1 - EMI_SD0), //write SDx
            ((bn_cfg.sdx_cfg.CSP    << EMI_SDx_CSP_i )   |
            ( bn_cfg.sdx_cfg.SDS    << EMI_SDx_SDS_i )   |
            ( bn_cfg.sdx_cfg.CL     << EMI_SDx_CL_i )    |
            ( bn_cfg.sdx_cfg.T_RDL  << EMI_SDx_CSP_i )   |
            ( bn_cfg.sdx_cfg.SI     << EMI_SDx_SI_i )    |
            ( bn_cfg.sdx_cfg.T_RCD  << EMI_SDx_T_RCD_i ) |
            ( bn_cfg.sdx_cfg.T_RAS  << EMI_SDx_T_RAS_i ))
            );
}

void emi_init_impl (uint32_t emi_dcr_base, uint32_t plb6mcif2_dcr_base, uint32_t puaba)
{
    rumboot_printf("Init EMI\n");
    //init bridge
    plb6mcif2_simple_init( plb6mcif2_dcr_base,  puaba );

    //init bank0 - SRAM0
    emi_init_bank_cfg b0_cfg =
    {
       //SS0
       {
           0, //BTYP
           0, //PTYP
           0, //SRDY
           0, //TWR
           0, //SST
           0, //T_SOE
           0b1001, //TCYC == 8
           0, //T_RDY
           0, //T_DEL
           0 //UWEB
       },
       //SD0
       {0, 0, 0, 0, 0, 0, 0}
    };
    emi_init_bank(0, emi_dcr_base, b0_cfg);

    //init bank1
    //TODO:
    //init bank2
    //TODO:
    //init bank3
    //TODO:
    //init bank4
    //TODO:

    //init bank5 - NOR
    emi_init_bank_cfg b5_cfg =
    {
      //SS5
      {
          0, //BTYP
          0, //PTYP
          0, //SRDY
          0, //TWR
          0, //SST
          0, //T_SOE
          0b1001, //TCYC == 8
          0, //T_RDY
          0, //T_DEL
          0 //UWEB
      },
      //SD5
      {0, 0, 0, 0, 0, 0, 0}
    };
    emi_init_bank(5, emi_dcr_base, b5_cfg);

    dcr_write(emi_dcr_base + EMI_BUSEN, 0x01);
}

void emi_init ()
{
    emi_init_impl (DCR_EM2_EMI_BASE, DCR_EM2_PLB6MCIF2_BASE, 0x00);
}
