#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <regs/regs_emi.h>
#include <platform/devices.h>


int main()
{
    rumboot_printf("Initializing PLB6MCIF2 bridge...\n");
    plb6mcif2_simple_init( DCR_EM2_PLB6MCIF2_BASE,  0x00);
    rumboot_printf("Configuring EMI banks...\n");
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
           TDEL_1
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
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, emi_b0_sram0, &b0_cfg);

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
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, emi_b1_sdram, &b1_cfg);

    static const emi_rfc_cfg emi_rfc =
    {
            TRFC_7,
            0b11110011110011,//RP
    };
    emi_set_rfc(DCR_EM2_EMI_BASE, &emi_rfc);

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
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, emi_b2_ssram, &b2_cfg);

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
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, emi_b3_pipelined, &b3_cfg);

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
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, emi_b4_sram1, &b4_cfg);

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
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, emi_b5_nor, &b5_cfg);
    dcr_write(DCR_EM2_EMI_BASE + EMI_FLCNTRL, 0x17);
    emi_set_ecc (DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_off);
    dcr_write(DCR_EM2_EMI_BASE + EMI_BUSEN, 0x01);

#if defined(CMAKE_BUILD_TYPE_DEBUG) && !defined(RUMBOOT_NOINIT)
    iowrite32(0x00000000, SDRAM_BASE);
    iowrite32(0x00000000, SDRAM_BASE + 4);
    iowrite32(0x00000000, SDRAM_BASE + 8);
    iowrite32(0x00000000, SDRAM_BASE + 12);
    rumboot_putstring( "Touching SDRAM: WARNING!!! WRITING 0x00000000 to SDRAM_BASE!!!\n" );
#endif
    rumboot_printf("Clearing SRAM to prevent speculation faults...\n");
    rumboot_memfill8((void*)SRAM0_BASE, 0x10000, 0x00, 0x00); //workaround (init first 64KB in SRAM0)

    #ifdef KEEP_TLB
        rumboot_printf("HACK: NOT calling platform exit\n");
        /* HACK: For NMC booting we have to keep populated TLBs in RAM */
        rumboot_platform_perf(NULL);
		longjmp(rumboot_platform_runtime.exit_trampoline, 256);        
    #endif
    rumboot_printf("All done, have fun.\n");
    return 0;
}
