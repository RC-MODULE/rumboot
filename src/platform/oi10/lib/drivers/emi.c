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
#include <devices/ugly/plb6mcif2.h>
#include <devices/ugly/emi.h>
#include <regs/fields/emi.h>
#include <arch/common_macros.h>
#include <regs/regs_emi.h>
#include <assert.h>
#include <rumboot/io.h>

static emi_bank_cfg const *bank_config_cache[6];
emi_bank_cfg const *emi_get_bank_cfg_cached(emi_bank_num num_bank)
{
    return bank_config_cache[num_bank];
}

void emi_init_impl (uint32_t const emi_dcr_base, uint32_t const plb6mcif2_dcr_base, uint32_t const puaba)
{
    //init bridge
    plb6mcif2_simple_init( plb6mcif2_dcr_base,  puaba );

    //init bank0 - SRAM0
    static emi_bank_cfg const b0_cfg =
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
           TCYC_2,
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
    emi_set_bank_cfg(emi_dcr_base, emi_b0_sram0, &b0_cfg);

    //init bank1 - SDRAM
    //setting parameters by comment:
    //(https://jira.module.ru/jira/browse/OI10-116?focusedCommentId=43530&page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel#comment-43530)
    static emi_bank_cfg const b1_cfg =
    {
       //SS1
       {
           BTYP_SDRAM,
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
       //SD1
       {
           CSP_2048,
           SDS_64M,
           CL_3,
           TRDL_2,
           SI_CPU_INIT,
           TRCD_2,
           TRAS_5
       }
    };
    emi_set_bank_cfg(emi_dcr_base, emi_b1_sdram, &b1_cfg);

    static const emi_rfc_cfg emi_rfc =
    {
            TRFC_7,
            0b11110011110011,//RP
    };
    emi_set_rfc(emi_dcr_base, &emi_rfc);

    //init bank2 - SSRAM
    static emi_bank_cfg const b2_cfg =
    {
       //SS2
       {
           BTYP_SSRAM,
           PTYP_NO_PAGES,
           SRDY_EXT_RDY_NOT_USE,
           TWR_0,
           SST_Pipelined,
           TSSOE_2,
           TSOE_1,
           TCYC_8,
           0, //T_RDY
           TDEL_0
       },
       //SD2
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
    emi_set_bank_cfg(emi_dcr_base, emi_b2_ssram, &b2_cfg);

    //init bank3 - PIPELINED
    static emi_bank_cfg const b3_cfg =
    {
       //SS3
       {
           BTYP_PIPERDY,
           PTYP_NO_PAGES,
           SRDY_EXT_RDY_NOT_USE,
           TWR_0,
           SST_Pipelined,
           TSSOE_1,
           TSOE_1,
           TCYC_8,
           0, //T_RDY
           TDEL_0
       },
       //SD3
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
    emi_set_bank_cfg(emi_dcr_base, emi_b3_pipelined, &b3_cfg);

    //init bank4 - SRAM1
    static emi_bank_cfg const b4_cfg =
    {
       //SS4
       {
           BTYP_SRAM,
           PTYP_NO_PAGES,
           SRDY_EXT_RDY_NOT_USE,
           TWR_0,
           SST_Flow_Through,
           TSSOE_1,
           TSOE_1,
           TCYC_2,
           0, //T_RDY
           TDEL_0
       },
       //SD4
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
    emi_set_bank_cfg(emi_dcr_base, emi_b4_sram1, &b4_cfg);

    //init bank5 - NOR
     static emi_bank_cfg const b5_cfg =
    {
        //SS5
        {
            BTYP_NOR,
            PTYP_NO_PAGES,
            SRDY_EXT_RDY_NOT_USE,
            TWR_0,
            SST_Flow_Through,
            TSSOE_1,
            TSOE_1,
            #ifdef CMAKE_BUILD_TYPE_DEBUG
            TCYC_8,
            #else
            TCYC_12,
            #endif
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
    emi_set_bank_cfg(emi_dcr_base, emi_b5_nor, &b5_cfg);
    dcr_write(DCR_EM2_EMI_BASE + EMI_FLCNTRL, 0x17);
    emi_set_ecc (emi_dcr_base, emi_bank_all, emi_ecc_off);
    dcr_write(emi_dcr_base + EMI_BUSEN, 0x01);

    /* Current config */
    bank_config_cache[0] = &b0_cfg;
    bank_config_cache[1] = &b1_cfg;
    bank_config_cache[2] = &b2_cfg;
    bank_config_cache[3] = &b3_cfg;
    bank_config_cache[4] = &b4_cfg;
    bank_config_cache[5] = &b5_cfg;
    msync();
}


void emi_init_impl_sram40ns (uint32_t const emi_dcr_base, uint32_t const plb6mcif2_dcr_base, uint32_t const puaba)
{
    //init bridge
    plb6mcif2_simple_init( plb6mcif2_dcr_base,  puaba );

    //init bank0 - SRAM0
    static emi_bank_cfg const b0_cfg =
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
           TCYC_4,
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
    emi_set_bank_cfg(emi_dcr_base, emi_b0_sram0, &b0_cfg);

    //init bank1 - SDRAM
    //setting parameters by comment:
    //(https://jira.module.ru/jira/browse/OI10-116?focusedCommentId=43530&page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel#comment-43530)
    static emi_bank_cfg const b1_cfg =
    {
       //SS1
       {
           BTYP_SDRAM,
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
       //SD1
       {
           CSP_2048,
           SDS_64M,
           CL_3,
           TRDL_2,
           SI_CPU_INIT,
           TRCD_2,
           TRAS_5
       }
    };
    emi_set_bank_cfg(emi_dcr_base, emi_b1_sdram, &b1_cfg);

    static const emi_rfc_cfg emi_rfc =
    {
            TRFC_7,
            0b11110011110011,//RP
    };
    emi_set_rfc(emi_dcr_base, &emi_rfc);

    //init bank2 - SSRAM
    static emi_bank_cfg const b2_cfg =
    {
       //SS2
       {
           BTYP_SSRAM,
           PTYP_NO_PAGES,
           SRDY_EXT_RDY_NOT_USE,
           TWR_0,
           SST_Pipelined,
           TSSOE_2,
           TSOE_1,
           TCYC_8,
           0, //T_RDY
           TDEL_0
       },
       //SD2
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
    emi_set_bank_cfg(emi_dcr_base, emi_b2_ssram, &b2_cfg);

    //init bank3 - PIPELINED
    static emi_bank_cfg const b3_cfg =
    {
       //SS3
       {
           BTYP_PIPERDY,
           PTYP_NO_PAGES,
           SRDY_EXT_RDY_NOT_USE,
           TWR_0,
           SST_Pipelined,
           TSSOE_1,
           TSOE_1,
           TCYC_8,
           0, //T_RDY
           TDEL_0
       },
       //SD3
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
    emi_set_bank_cfg(emi_dcr_base, emi_b3_pipelined, &b3_cfg);

    //init bank4 - SRAM1
    static emi_bank_cfg const b4_cfg =
    {
       //SS4
       {
           BTYP_SRAM,
           PTYP_NO_PAGES,
           SRDY_EXT_RDY_NOT_USE,
           TWR_0,
           SST_Flow_Through,
           TSSOE_1,
           TSOE_1,
           TCYC_2,
           0, //T_RDY
           TDEL_0
       },
       //SD4
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
    emi_set_bank_cfg(emi_dcr_base, emi_b4_sram1, &b4_cfg);

    //init bank5 - NOR
     static emi_bank_cfg const b5_cfg =
    {
        //SS5
        {
            BTYP_NOR,
            PTYP_NO_PAGES,
            SRDY_EXT_RDY_NOT_USE,
            TWR_0,
            SST_Flow_Through,
            TSSOE_1,
            TSOE_1,
            #ifdef CMAKE_BUILD_TYPE_DEBUG
            TCYC_8,
            #else
            TCYC_12,
            #endif
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
    emi_set_bank_cfg(emi_dcr_base, emi_b5_nor, &b5_cfg);
    dcr_write(DCR_EM2_EMI_BASE + EMI_FLCNTRL, 0x17);
    emi_set_ecc (emi_dcr_base, emi_bank_all, emi_ecc_off);
    dcr_write(emi_dcr_base + EMI_BUSEN, 0x01);

    /* Current config */
    bank_config_cache[0] = &b0_cfg;
    bank_config_cache[1] = &b1_cfg;
    bank_config_cache[2] = &b2_cfg;
    bank_config_cache[3] = &b3_cfg;
    bank_config_cache[4] = &b4_cfg;
    bank_config_cache[5] = &b5_cfg;
    msync();
}


void emi_init (uint32_t const emi_dcr_base)
{
    emi_init_impl (emi_dcr_base, DCR_EM2_PLB6MCIF2_BASE, 0x00);
#ifdef CMAKE_BUILD_TYPE_DEBUG
    iowrite32(0x00000000, SDRAM_BASE); // touch sdram to prevent warnings in simulation console
    rumboot_putstring( "Touching SDRAM: WARNING!!! WRITING 0x00000000 to SDRAM_BASE!!!\n" );
#endif
}

void emi_init_sram40ns (uint32_t const emi_dcr_base)
{
    emi_init_impl_sram40ns (emi_dcr_base, DCR_EM2_PLB6MCIF2_BASE, 0x00);
#ifdef CMAKE_BUILD_TYPE_DEBUG
    iowrite32(0x00000000, SDRAM_BASE); // touch sdram to prevent warnings in simulation console
    rumboot_putstring( "Touching SDRAM: WARNING!!! WRITING 0x00000000 to SDRAM_BASE!!!\n" );
#endif
}

void emi_init_impl_ext_cfg (uint32_t const emi_dcr_base, uint32_t const plb6mcif2_dcr_base, uint32_t const puaba, emi_bank_cfg* cfg )
{
    //init bridge
    plb6mcif2_simple_init( plb6mcif2_dcr_base,  puaba );

    //init bank0 - SRAM0
    emi_set_bank_cfg(emi_dcr_base, emi_b0_sram0, cfg + 0);

    //init bank1 - SDRAM
    emi_set_bank_cfg(emi_dcr_base, emi_b1_sdram, cfg + 1);

    emi_rfc_cfg emi_rfc =
    {
            TRFC_7,
            0b11110011110011,//RP
    };
    emi_set_rfc(emi_dcr_base, &emi_rfc);

    //init bank2 - SSRAM
    emi_set_bank_cfg(emi_dcr_base, emi_b2_ssram, cfg + 2);

    //init bank3 - PIPELINED
    emi_set_bank_cfg(emi_dcr_base, emi_b3_pipelined, cfg + 3);

    //init bank4 - SRAM1
    emi_set_bank_cfg(emi_dcr_base, emi_b4_sram1, cfg + 4);

    //init bank5 - NOR
    emi_set_bank_cfg(emi_dcr_base, emi_b5_nor, cfg + 5);
    dcr_write(DCR_EM2_EMI_BASE + EMI_FLCNTRL, 0x17);

    emi_set_ecc (emi_dcr_base, emi_bank_all, emi_ecc_off);
    dcr_write(emi_dcr_base + EMI_BUSEN, 0x01);

    /* Current config */
    for (int i=0; i<6; i++)
    {
        bank_config_cache[i] = cfg + i;
    }
}

void prepare_default_emi_cfg(emi_bank_cfg* cfg)
{
    //bank0 - SRAM0
    (cfg + 0)->ssx_cfg.BTYP   = BTYP_SRAM;
    (cfg + 0)->ssx_cfg.PTYP   = PTYP_NO_PAGES;
    (cfg + 0)->ssx_cfg.SRDY   = SRDY_EXT_RDY_NOT_USE;
    (cfg + 0)->ssx_cfg.TWR    = TWR_0;
    (cfg + 0)->ssx_cfg.SST    = SST_Flow_Through;
    (cfg + 0)->ssx_cfg.T_SSOE = TSSOE_1;
    (cfg + 0)->ssx_cfg.T_SOE  = TSOE_1;
    (cfg + 0)->ssx_cfg.T_CYC  = TCYC_2;
    (cfg + 0)->ssx_cfg.T_RDY  = 0;
    (cfg + 0)->ssx_cfg.T_DEL  = TDEL_0;

    (cfg + 0)->sdx_cfg.CSP    = CSP_256;
    (cfg + 0)->sdx_cfg.SDS    = SDS_2M;
    (cfg + 0)->sdx_cfg.CL     = CL_3;
    (cfg + 0)->sdx_cfg.T_RDL  = TRDL_1;
    (cfg + 0)->sdx_cfg.SI     = SI_EXT_INIT;
    (cfg + 0)->sdx_cfg.T_RCD  = TRCD_5;
    (cfg + 0)->sdx_cfg.T_RAS  = TRAS_9;

    //bank1 - SDRAM
   (cfg + 1)->ssx_cfg.BTYP   = BTYP_SDRAM;
   (cfg + 1)->ssx_cfg.PTYP   = PTYP_NO_PAGES;
   (cfg + 1)->ssx_cfg.SRDY   = SRDY_EXT_RDY_NOT_USE;
   (cfg + 1)->ssx_cfg.TWR    = TWR_0;
   (cfg + 1)->ssx_cfg.SST    = SST_Flow_Through;
   (cfg + 1)->ssx_cfg.T_SSOE = TSSOE_1;
   (cfg + 1)->ssx_cfg.T_SOE  = TSOE_1;
   (cfg + 1)->ssx_cfg.T_CYC  = TCYC_8;
   (cfg + 1)->ssx_cfg.T_RDY  = 0;
   (cfg + 1)->ssx_cfg.T_DEL  = TDEL_0;

   (cfg + 1)->sdx_cfg.CSP    = CSP_2048;
   (cfg + 1)->sdx_cfg.SDS    = SDS_64M;
   (cfg + 1)->sdx_cfg.CL     = CL_3;
   (cfg + 1)->sdx_cfg.T_RDL  = TRDL_2;
   (cfg + 1)->sdx_cfg.SI     = SI_CPU_INIT;
   (cfg + 1)->sdx_cfg.T_RCD  = TRCD_2;
   (cfg + 1)->sdx_cfg.T_RAS  = TRAS_5;

    //bank2 - SSRAM
    (cfg + 2)->ssx_cfg.BTYP   = BTYP_SSRAM;
    (cfg + 2)->ssx_cfg.PTYP   = PTYP_NO_PAGES;
    (cfg + 2)->ssx_cfg.SRDY   = SRDY_EXT_RDY_NOT_USE;
    (cfg + 2)->ssx_cfg.TWR    = TWR_0;
    (cfg + 2)->ssx_cfg.SST    = SST_Pipelined;
    (cfg + 2)->ssx_cfg.T_SSOE = TSSOE_2;
    (cfg + 2)->ssx_cfg.T_SOE  = TSOE_1;
    (cfg + 2)->ssx_cfg.T_CYC  = TCYC_8;
    (cfg + 2)->ssx_cfg.T_RDY  = 0;
    (cfg + 2)->ssx_cfg.T_DEL  = TDEL_0;

    (cfg + 2)->sdx_cfg.CSP    = CSP_256;
    (cfg + 2)->sdx_cfg.SDS    = SDS_2M;
    (cfg + 2)->sdx_cfg.CL     = CL_3;
    (cfg + 2)->sdx_cfg.T_RDL  = TRDL_1;
    (cfg + 2)->sdx_cfg.SI     = SI_EXT_INIT;
    (cfg + 2)->sdx_cfg.T_RCD  = TRCD_5;
    (cfg + 2)->sdx_cfg.T_RAS  = TRAS_9;

    //bank3 - PIPELINED
    (cfg + 3)->ssx_cfg.BTYP   = BTYP_PIPERDY;
    (cfg + 3)->ssx_cfg.PTYP   = PTYP_NO_PAGES;
    (cfg + 3)->ssx_cfg.SRDY   = SRDY_EXT_RDY_NOT_USE;
    (cfg + 3)->ssx_cfg.TWR    = TWR_0;
    (cfg + 3)->ssx_cfg.SST    = SST_Pipelined;
    (cfg + 3)->ssx_cfg.T_SSOE = TSSOE_1;
    (cfg + 3)->ssx_cfg.T_SOE  = TSOE_1;
    (cfg + 3)->ssx_cfg.T_CYC  = TCYC_8;
    (cfg + 3)->ssx_cfg.T_RDY  = 0;
    (cfg + 3)->ssx_cfg.T_DEL  = TDEL_0;

    (cfg + 3)->sdx_cfg.CSP    = CSP_256;
    (cfg + 3)->sdx_cfg.SDS    = SDS_2M;
    (cfg + 3)->sdx_cfg.CL     = CL_3;
    (cfg + 3)->sdx_cfg.T_RDL  = TRDL_1;
    (cfg + 3)->sdx_cfg.SI     = SI_EXT_INIT;
    (cfg + 3)->sdx_cfg.T_RCD  = TRCD_5;
    (cfg + 3)->sdx_cfg.T_RAS  = TRAS_9;

    //bank4 - SRAM1
    (cfg + 4)->ssx_cfg.BTYP   = BTYP_SRAM;
    (cfg + 4)->ssx_cfg.PTYP   = PTYP_NO_PAGES;
    (cfg + 4)->ssx_cfg.SRDY   = SRDY_EXT_RDY_NOT_USE;
    (cfg + 4)->ssx_cfg.TWR    = TWR_0;
    (cfg + 4)->ssx_cfg.SST    = SST_Flow_Through;
    (cfg + 4)->ssx_cfg.T_SSOE = TSSOE_1;
    (cfg + 4)->ssx_cfg.T_SOE  = TSOE_1;
    (cfg + 4)->ssx_cfg.T_CYC  = TCYC_2;
    (cfg + 4)->ssx_cfg.T_RDY  = 0;
    (cfg + 4)->ssx_cfg.T_DEL  = TDEL_0;

    (cfg + 4)->sdx_cfg.CSP    = CSP_256;
    (cfg + 4)->sdx_cfg.SDS    = SDS_2M;
    (cfg + 4)->sdx_cfg.CL     = CL_3;
    (cfg + 4)->sdx_cfg.T_RDL  = TRDL_1;
    (cfg + 4)->sdx_cfg.SI     = SI_EXT_INIT;
    (cfg + 4)->sdx_cfg.T_RCD  = TRCD_5;
    (cfg + 4)->sdx_cfg.T_RAS  = TRAS_9;

    //bank5 - NOR
    (cfg + 5)->ssx_cfg.BTYP   = BTYP_NOR;
    (cfg + 5)->ssx_cfg.PTYP   = PTYP_NO_PAGES;
    (cfg + 5)->ssx_cfg.SRDY   = SRDY_EXT_RDY_NOT_USE;
    (cfg + 5)->ssx_cfg.TWR    = TWR_0;
    (cfg + 5)->ssx_cfg.SST    = SST_Flow_Through;
    (cfg + 5)->ssx_cfg.T_SSOE = TSSOE_1;
    (cfg + 5)->ssx_cfg.T_SOE  = TSOE_1;
    (cfg + 5)->ssx_cfg.T_CYC  = TCYC_8;
    (cfg + 5)->ssx_cfg.T_RDY  = 0;
    (cfg + 5)->ssx_cfg.T_DEL  = TDEL_0;

    (cfg + 5)->sdx_cfg.CSP    =  CSP_256;
    (cfg + 5)->sdx_cfg.SDS    =  SDS_2M;
    (cfg + 5)->sdx_cfg.CL     =  CL_3;
    (cfg + 5)->sdx_cfg.T_RDL  =  TRDL_1;
    (cfg + 5)->sdx_cfg.SI     =  SI_EXT_INIT;
    (cfg + 5)->sdx_cfg.T_RCD  =  TRCD_5;
    (cfg + 5)->sdx_cfg.T_RAS  =  TRAS_9;
}


void emi_init_ext_cfg (uint32_t const emi_dcr_base, emi_bank_cfg* cfg )
{
    emi_init_impl_ext_cfg (emi_dcr_base, DCR_EM2_PLB6MCIF2_BASE, 0x00, cfg);
#if defined(CMAKE_BUILD_TYPE_DEBUG) && !defined(RUMBOOT_NOINIT)
    iowrite32(0x00000000, SDRAM_BASE); // touch sdram to prevent warnings in simulation console
    iowrite32(0x00000000, SDRAM_BASE + 4);
    iowrite32(0x00000000, SDRAM_BASE + 8);
    iowrite32(0x00000000, SDRAM_BASE + 12);
    rumboot_putstring( "Touching SDRAM: WARNING!!! WRITING 0x00000000 to SDRAM_BASE!!!\n" );
#endif
}

