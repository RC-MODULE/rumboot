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

static inline void rumboot_putdump(uint32_t const start_addr, uint32_t const length_in_bytes) {
    asm volatile (
            "mtspr %0, %1 \n\t"
            "mtspr %2, %3 \n\t"
            ::"n"(SPR_SPRG3), "r"(start_addr),
              "n"(SPR_SPRG4), "r"(length_in_bytes)
    );
    test_event(EVENT_TRACE_DUMP);
}


static inline void rumboot_issdump(uint32_t const start_addr, uint32_t const length_in_bytes) {
    asm volatile (
            "mtspr %0, %1 \n\t"
            "mtspr %2, %3 \n\t"
            ::"n"(SPR_SPRG3), "r"(start_addr),
              "n"(SPR_SPRG4), "r"(length_in_bytes)
    );
    test_event(TEST_EVENT_CREATE_ISS_DUMP);
}

#endif /* TRACE_H_ */
