/*
 *  Created on: Jun 9, 2018
 *      Author: v.strukov
 */

#ifndef TRACE_H_
#define TRACE_H_


#include <platform/test_event_c.h>


static inline __attribute__((always_inline)) void rumboot_putstring_fast( const char * const msg ) {
    RUMBOOT_ATOMIC_BLOCK() {
        spr_write( SPR_SPRG3, msg );
        rumboot_event_arg1_( EVENT_TESTEVENT, EVENT_TRACE_MESSAGE );
    }
}
#define rumboot_putstring( msg ) rumboot_putstring_fast( msg )

static inline __attribute__((always_inline)) void rumboot_puthex( uint32_t const hex ) {
    RUMBOOT_ATOMIC_BLOCK() {
        spr_write( SPR_SPRG3, hex );
        rumboot_event_arg1_( EVENT_TESTEVENT, EVENT_TRACE_HEX );
    }
}

static inline __attribute__((always_inline)) void rumboot_putdump( uint32_t const start_addr, uint32_t const length_in_bytes ) {
    RUMBOOT_ATOMIC_BLOCK() {
        spr_write( SPR_SPRG3, start_addr );
        spr_write( SPR_SPRG4, length_in_bytes );
        rumboot_event_arg1_( EVENT_TESTEVENT, EVENT_TRACE_DUMP );
    }
}

static inline __attribute__((always_inline)) void rumboot_issdump( uint32_t const start_addr, uint32_t const length_in_bytes ) {
    RUMBOOT_ATOMIC_BLOCK() {
        spr_write( SPR_SPRG3, start_addr );
        spr_write( SPR_SPRG4, length_in_bytes );
        rumboot_event_arg1_( EVENT_TESTEVENT, TEST_EVENT_CREATE_ISS_DUMP );
    }
}


#endif /* TRACE_H_ */
