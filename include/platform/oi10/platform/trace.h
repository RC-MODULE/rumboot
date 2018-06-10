/*
 *  Created on: Jun 9, 2018
 *      Author: v.strukov
 */

#ifndef TRACE_H_
#define TRACE_H_

#include <platform/test_event_c.h>

static inline void rumboot_putstring_fast(const char *msg){
    asm volatile (
            "mtspr %0, %1 \n\t"
            ::"n"(SPR_SPRG3), "r"(msg)
    );
    test_event(EVENT_TRACE_MESSAGE);
}
#define rumboot_putstring(msg) rumboot_putstring_fast(msg)
static inline void rumboot_puthex(uint32_t const hex) {
    asm volatile (
            "mtspr %0, %1 \n\t"
            ::"n"(SPR_SPRG3), "r"(hex)
    );
    test_event(EVENT_TRACE_HEX);
}

#endif /* TRACE_H_ */
