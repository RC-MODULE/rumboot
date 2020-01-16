#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/trace.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/devices/emi.h>
#include <rumboot/memfill.h>


typedef bool func( void );
static emi_bank_cfg bank_cfg =
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
        TCYC_3,
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


int main(void) {

    #if 1
    rumboot_printf("Setup cache\n");
    spr_write(SPR_CCR2, (CTRL_CCR2_DSTG_disabled << CTRL_CCR2_DSTG_i)\
                        | (0b0 << CTRL_CCR2_DLFPD_i)    \
                        | (0b1 << CTRL_CCR2_DSTI_i)     \
                        | (0b0 << CTRL_CCR2_PMUD_i)     \
                        | (0b0 << CTRL_CCR2_DCSTGW_i)   \
                        | (0   << CTRL_CCR2_STGCTR_i)   \
                        | (0b0 << CTRL_CCR2_DISTG_i)    \
                        | (0b0 << CTRL_CCR2_SPC5C1_i)   \
                        | (0b0 << CTRL_CCR2_MCDTO_i     )
            );

    dcr_write(0x80000600, 0xC10);
    dcr_write(0x80000604, 0x1);


    uint32_t wac = dcr_read(0xC10);
    rumboot_printf("wac: %x\n", wac);
   // wac |= 1<<31 | 1 << 14 | 1 << 13;
    rumboot_printf("wac: %x\n", wac);
    dcr_write(wac, 0xC10);
    #endif

    rumboot_printf( "EMI init\n" );
//    emi_init( DCR_EM2_EMI_BASE );
    emi_init_ext_cfg(DCR_EM2_EMI_BASE, &bank_cfg);

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
