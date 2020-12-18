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
#include <arch/ppc_476fp_debug_fields.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_timer_fields.h>


typedef enum test_event_code {
    TEC_CHECK_WD_CORE_RESET     = TEST_EVENT_CODE_MIN,
    TEC_CHECK_WD_CHIP_RESET,
    TEC_CHECK_WD_SYSTEM_RESET
} test_event_code;

typedef enum test_check {
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
    spr_write( SPR_TCR, (TIMER_TCR_WP_2pow21_clocks << TIMER_TCR_WP_i)
                      | (reset_type                 << TIMER_TCR_WRC_i)
                      | (0b1                        << TIMER_TCR_WIE_i)
                      | (0b0                        << TIMER_TCR_DIE_i)
                      | (0b0                        << TIMER_TCR_FIE_i) );

    spr_write( SPR_TBL_W, 0 );                          //reset TBL
    spr_write( SPR_TSR_RC, (0b1     << TIMER_TSR_ENW_i)    // clear WD next action
                         | (0b1     << TIMER_TSR_WIS_i) ); // clear WD exception

    spr_write( SPR_TBL_W, FIELD_MASK32(8, (20-8)) );    // speedup WD triggering (2^8 clocks) for TIMER_TCR_WP_2pow21_clocks: TSR[ENW] to 1
    while( !(spr_read(SPR_TBL_R) & (0b1 << 20)) );      // wait trigger TSR[ENW] to 1
    spr_write( SPR_TBL_W, FIELD_MASK32(8, (20-8)) );    // set TBL value that triggers TSR[WIS] to 1
    while( !(spr_read(SPR_TBL_R) & (0b1 << 20)) );      // wait trigger TSR[WIS] to 1
}

static void exception_handler( int const id, char const * const name ) {
    rumboot_printf( "exception_handler\n" );
    if( id != RUMBOOT_IRQ_WATCHDOG_TIMER ) {
        rumboot_printf( "Unexpected exception: %s\n", name );
        exit(1);
    }

    rumboot_platform_perf("generate_wd_reset");
    spr_write( SPR_TBL_W, FIELD_MASK32(8, (20-8)) );    // speedup WD triggering (2^8 clocks) for TIMER_TCR_WP_2pow21_clocks: reset on TSR[ENW] == 1 and TSR[WIS] == 1
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

int main() {
    test_event_send_test_id( "test_oi10_cpu_025_wd");

    if(rumboot_platform_runtime_info->persistent[test_check_magic] != MAGIC ) {
        rumboot_platform_runtime_info->persistent[test_check_magic] = MAGIC;
        rumboot_platform_runtime_info->persistent[last_test_check] = TC_NONE;
    }

    rumboot_irq_set_exception_handler(exception_handler);

    switch( rumboot_platform_runtime_info->persistent[last_test_check] ) {
        case TC_NONE: {
            check_wd_core_reset();
            break;
        }
        case TC_CORE_RESET: {
            TEST_ASSERT( GET_BITS( spr_read(SPR_TSR_RC), TIMER_TSR_WRS_i, TIMER_TSR_WRS_n ) == TIMER_TCR_WRS_Core_reset, "Error. Invalid value in TSR[WRS]" );
            // Under git@git.module.ru:OI10/cpu.git de57269bb7945aaea0b1fc2757708036baf0aa8a
            TEST_ASSERT( GET_BITS( spr_read(SPR_DBSR_RC), DEBUG_DBSR_MRR_i, DEBUG_DBSR_MRR_n ) == 0b10, "Error. Invalid value in DBSR[MRR]" );
            check_wd_chip_reset();
            break;
        }
        case TC_CHIP_RESET: {
            TEST_ASSERT( GET_BITS( spr_read(SPR_TSR_RC), TIMER_TSR_WRS_i, TIMER_TSR_WRS_n ) == TIMER_TCR_WRS_Chip_reset, "Error. Invalid value in TSR[WRS]" );
            TEST_ASSERT( GET_BITS( spr_read(SPR_DBSR_RC), DEBUG_DBSR_MRR_i, DEBUG_DBSR_MRR_n ) == 0b10, "Error. Invalid value in DBSR[MRR]" );
            check_wd_system_reset();
            break;
        }
        case TC_SYSTEM_RESET: {
            TEST_ASSERT( GET_BITS( spr_read(SPR_TSR_RC), TIMER_TSR_WRS_i, TIMER_TSR_WRS_n ) == TIMER_TCR_WRS_System_reset, "Error. Invalid value in TSR[WRS]" );
            TEST_ASSERT( GET_BITS( spr_read(SPR_DBSR_RC), DEBUG_DBSR_MRR_i, DEBUG_DBSR_MRR_n ) == 0b11, "Error. Invalid value in DBSR[MRR]" );
            rumboot_platform_runtime_info->persistent[test_check_magic] = ~MAGIC;
            return 0;
        }
    }

    return 1;
}

