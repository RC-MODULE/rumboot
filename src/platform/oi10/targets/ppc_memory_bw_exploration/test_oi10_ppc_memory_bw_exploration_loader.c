#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/platform.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>
#include <arch/ppc_476fp_mmu.h>
#include <devices/ugly/emi.h>
#include <rumboot/memfill.h>

typedef bool func( void );

int main(void) {
    static emi_bank_cfg bank_cfg[6] =
            {
            {    //SS0
                {
                    BTYP_SRAM,
                    PTYP_NO_PAGES,
                    SRDY_EXT_RDY_NOT_USE,
                    TWR_0,
                    SST_Flow_Through,
                    TSSOE_1,
                    TSOE_1,
                #ifndef SLOW_SRAM0_CFG
                    TCYC_3,
                    0, //T_RDY
                    TDEL_1
                #else
                    TCYC_8,
                    0, //T_RDY
                    TDEL_3
                #endif
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
            },
            // append from emi_init_impl, default settings for bank1-bank5
            //init bank1 - SDRAM
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
                },
                // init bank2 - SSRAM, 
                //  SS2
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
                },
                //init bank3 - PIPELINED
                //SS3
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
                },
                //init bank4 - SRAM1
                //SS4
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
                },
                //init bank5 - NOR
                //SS5
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
            }
            };
    //rumboot_printf( "EMI init\n" );
    //emi_init( DCR_EM2_EMI_BASE );
    
    rumboot_printf("Init EMI...\n");
    #ifdef SLOW_SRAM0_CFG
        rumboot_printf("Slow SRAM0 configuration\n");
    #endif
    emi_init_ext_cfg(DCR_EM2_EMI_BASE, &bank_cfg);
    rumboot_printf("Init done.\n");

    rumboot_printf( "Load cache test function in 0x%x\n", SRAM0_BASE );
    rumboot_platform_request_file( "MBIN", SRAM0_BASE );

    rumboot_printf("Call cache test function\n");
    func* test_func = (func*)(SRAM0_BASE);
    if ( !test_func() ) {
        rumboot_printf( "TEST ERROR\n" );
        return 1;
    }

    rumboot_printf( "TEST OK\n" );
    return 0;
}
