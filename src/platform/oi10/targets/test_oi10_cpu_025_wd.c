/*
 * test_oi10_cpu_025_wd.c
 *
 *  Created on: Mar 27, 2019
 *      Author: m.smolina
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <platform/test_event_c.h>
#include <rumboot/irq.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>

typedef enum test_event_code
{
    TEC_CHECK_WD_CORE_RESET     = TEST_EVENT_CODE_MIN,
    TEC_CHECK_WD_CHIP_RESET,
    TEC_CHECK_WD_SYSTEM_RESET
} test_event_code;


typedef enum test_check
{
    TC_NONE,
    TC_CORE_RESET,
    TC_CHIP_RESET,
    TC_SYSTEM_RESET
} test_check;

enum {
    last_test_check = 0,
    test_check_magic = 1,
};

#define MAGIC 0x7E11012

static void generate_wd_reset( TIMER_TCR_WRC const reset_type )
{
    rumboot_platform_perf("generate_wd_reset");


    spr_write(SPR_TCR,    (TIMER_TCR_WP_2pow21_clocks  << TIMER_TCR_WP_i)
                        | (reset_type                  << TIMER_TCR_WRC_i)
                        | (0b0                         << TIMER_TCR_WIE_i)
                        | (0b0                         << TIMER_TCR_DIE_i)
                        | (0b0                         << TIMER_TCR_FIE_i) );


    spr_write(SPR_TBL_W,0); //reset TBL
    spr_write(SPR_TSR_RC,  (0b1  << TIMER_TSR_ENW_i )     // clear WD next action
                         | (0b1  << TIMER_TSR_WIS_i)     // clear WD exception
                         | (0b11 << TIMER_TSR_WRS_i) );  // clear WD reset status

    spr_write( SPR_TBL_W,(0b1 << 20) ); // set TBL value that triggers TSR[ENW] to 1
    spr_write( SPR_TBL_W,(0b0 << 20) ); // prepare for next trigger
    spr_write( SPR_TBL_W,(0b1 << 20) ); // set TBL value that triggers TSR[WIS] to 1
    spr_write( SPR_TBL_W,(0b0 << 20) ); // prepare for next trigger
    spr_write( SPR_TBL_W,(0b1 << 20) ); // set TBL value that triggers reset when TSR[ENW] == 1 and TSR[WIS] == 1

}


static void check_wd_core_reset() {
    rumboot_printf( "check_wd_core_reset\n" );
    rumboot_platform_runtime_info->persistent[last_test_check]= TC_CORE_RESET;
    msync();

    test_event( TEC_CHECK_WD_CORE_RESET );
    generate_wd_reset( TIMER_TCR_WRC_Core_reset );
}

static void check_wd_chip_reset() {
    rumboot_printf( "check_wd_chip_reset\n" );
    rumboot_platform_runtime_info->persistent[last_test_check] = TC_CHIP_RESET;
    msync();

    test_event( TEC_CHECK_WD_CHIP_RESET );
    generate_wd_reset( TIMER_TCR_WRC_Chip_reset );
}

static void check_wd_system_reset() {
    rumboot_printf( "check_wd_system_reset\n" );
    rumboot_platform_runtime_info->persistent[last_test_check] = TC_SYSTEM_RESET;
    msync();

    test_event( TEC_CHECK_WD_SYSTEM_RESET );
    generate_wd_reset( TIMER_TCR_WRC_System_reset );
}


int main()
{
    test_event_send_test_id( "test_oi10_cpu_025_wd");

    msr_write( msr_read() & ~( (0b1 << ITRPT_XSR_WE_i)
                             | (0b1 << ITRPT_XSR_CE_i)
                             | (0b1 << ITRPT_XSR_EE_i)
                             | (0b1 << ITRPT_XSR_ME_i)
                             | (0b1 << ITRPT_XSR_FE0_i)
                             | (0b1 << ITRPT_XSR_DWE_i)
                             | (0b1 << ITRPT_XSR_DE_i)
                             | (0b1 << ITRPT_XSR_FE1_i)) );

    uint32_t reg = 0;

    if(rumboot_platform_runtime_info->persistent[test_check_magic] != MAGIC )
        {
        rumboot_printf("MAGIC\n");
        rumboot_platform_runtime_info->persistent[last_test_check] = TC_NONE;
        }


    switch( rumboot_platform_runtime_info->persistent[last_test_check] ) {
        case TC_NONE: {
            check_wd_core_reset();
            break;
        }
        case TC_CORE_RESET: {
            reg = spr_read (SPR_DBSR_RC);
            // Under git@git.module.ru:OI10/cpu.git de57269bb7945aaea0b1fc2757708036baf0aa8a
            TEST_ASSERT((reg & (1<<29)) && !((reg & (1<<28))), "Error. Invalid value in DBSR[MRR]");
            check_wd_chip_reset();
            break;
        }
        case TC_CHIP_RESET: {
            reg = spr_read (SPR_DBSR_RC);
            TEST_ASSERT((reg & (1<<29)) && !((reg & (1<<28))), "Error. Invalid value in DBSR[MRR]");
            check_wd_system_reset();
            break;
        }
        case TC_SYSTEM_RESET:
        {
            reg = spr_read (SPR_DBSR_RC);
            TEST_ASSERT((reg & (1<<28)) && (reg & (1<<29)), "Error. Invalid value in DBSR[MRR]");
            rumboot_platform_runtime_info->persistent[test_check_magic] = ~MAGIC;
            return 0;
        }
    }

    return 1;
}

