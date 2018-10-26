/*
 *  Created on: Jun 9, 2018
 *      Author: v.strukov
 */

#ifndef TRACE_H_
#define TRACE_H_


#include <platform/test_event_c.h>


static inline __attribute__((always_inline)) void rumboot_putstring_fast( const char * const msg ) {
    uint32_t const event_data[] = { EVENT_TRACE_MESSAGE, (uint32_t)msg };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_data, ARRAY_SIZE(event_data) );
}
#define rumboot_putstring( msg ) rumboot_putstring_fast( msg )

static inline __attribute__((always_inline)) void rumboot_puthex( uint32_t const hex ) {
    uint32_t const event_data[] = { EVENT_TRACE_HEX, hex };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_data, ARRAY_SIZE(event_data) );
}

static inline __attribute__((always_inline)) void rumboot_putdump( uint32_t const start_addr, uint32_t const length_in_bytes ) {
    uint32_t const event_data[] = { EVENT_TRACE_DUMP, start_addr, length_in_bytes };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_data, ARRAY_SIZE(event_data) );
}

static inline __attribute__((always_inline)) void rumboot_issdump( uint32_t const start_addr, uint32_t const length_in_bytes ) {
    uint32_t const event_data[] = { TEST_EVENT_CREATE_ISS_DUMP, start_addr, length_in_bytes };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_data, ARRAY_SIZE(event_data) );
}


#endif /* TRACE_H_ */
