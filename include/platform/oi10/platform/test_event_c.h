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

#ifdef RUMBOOT_PRINTF_ACCEL
static __attribute__((no_instrument_function)) void test_event_do_deliver(enum sys_event_code event, uint32_t stack_pointer)
{
    uint32_t const event_data[] = { event, stack_pointer };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_data, ARRAY_SIZE(event_data) );
}

static inline __attribute__((no_instrument_function)) void test_event_deliver(enum sys_event_code event, uint32_t stack_pointer)
{
    test_event_do_deliver(event, stack_pointer);
}

#endif

/**
 * The function fills memory sequentially with val that is incremented each time by incr s
 * tarting from address pointed by addr until all sz bytes are filled.
 * The function is intended for usage only with a model for providing anti-x workaround.
 * If RUMBOOT_PRINTF_ACCEL is not defined, the function is nop.
 *
 * @param  addr start address
 * @param  sz   count of bytes
 * @param  val  starting value
 * @param  incr increment
 * @return      pointer to the start of the filled area
 */
static inline __attribute__((no_instrument_function))
void rumboot_memfill8_modelling(void *addr, size_t sz, uint8_t val, int8_t incr)
{
#ifdef RUMBOOT_PRINTF_ACCEL
    rumboot_memfill8(addr, sz, val, incr);
#endif
    return;
}


#endif /* TEST_EVENT_C_H_ */
