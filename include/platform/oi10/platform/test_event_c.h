/*
 *
 *  Created on: Jun 9, 2018
 *      Author: v.strukov
 */

#ifndef TEST_EVENT_C_H_
#define TEST_EVENT_C_H_


#include <platform/test_event_codes.h>
#include <rumboot/platform.h>
#include <rumboot/irq.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>


static inline __attribute__((always_inline)) void rumboot_event_arg1_( enum rumboot_simulation_event const event, uint32_t const arg0 ) {
    spr_write( SPR_SPRG2, arg0 );
    spr_write( SPR_SPRG1, event );
    spr_write( SPR_SPRG1, EVENT_NOP );
}

static inline __attribute__((always_inline)) void test_event( uint32_t const event_code ) {
    RUMBOOT_ATOMIC_BLOCK() {
        rumboot_event_arg1_( EVENT_TESTEVENT, event_code );
    }
}

static inline __attribute__((always_inline)) void test_event_send_test_id( char const * const test_id ) {
    RUMBOOT_ATOMIC_BLOCK() {
        spr_write( SPR_SPRG3, test_id );
        rumboot_event_arg1_( EVENT_TESTEVENT, TEST_EVENT_NEW_TEST_STARTED );
    }
}


#endif /* TEST_EVENT_C_H_ */
