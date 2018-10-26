/*
 *
 *  Created on: Jun 9, 2018
 *      Author: v.strukov
 */

#ifndef TEST_EVENT_C_H_
#define TEST_EVENT_C_H_


#include <platform/test_event_codes.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>


static inline __attribute__((always_inline)) void test_event( uint32_t const event_code ) {
    uint32_t const event_data[] = { event_code };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_data, ARRAY_SIZE(event_data) );
}

static inline __attribute__((always_inline)) void test_event_send_test_id( char const * const test_id ) {
    uint32_t const event_data[] = { TEST_EVENT_NEW_TEST_STARTED, (uint32_t)test_id };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_data, ARRAY_SIZE(event_data) );
}


#endif /* TEST_EVENT_C_H_ */
