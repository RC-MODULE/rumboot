/*
 *  Created on: Jun 9, 2018
 *      Author: v.strukov
 */

#ifndef TRACE_H_
#define TRACE_H_

#include <platform/test_event_c.h>
/*#define rumboot_putstring( msg ) \
MACRO_BEGIN\
    static char const trace_message[] = msg; \
    SPR_SPRG3_write((uint32_t)&trace_message); \
    SPR_SPRG4_write((sizeof(trace_message)/sizeof(char))); \
    test_event(EVENT_TRACE_MESSAGE); \
MACRO_END
*/
//TODO refactor it, make a spr_read, spr_write macros or function if possible
/*static inline void rumboot_putstring(char *msg){
    asm volatile (
            "mtspr %0, %1 \n\t"
            ::"n"(SPR_SPRG3), "r"(msg)
    );
    test_event(EVENT_TRACE_MESSAGE);
}*/

static inline void rumboot_puthex(uint32_t const hex) {
    asm volatile (
            "mtspr %0, %1 \n\t"
            ::"n"(SPR_SPRG3), "r"(hex)
    );
    test_event(EVENT_TRACE_HEX);
}

#endif /* TRACE_H_ */
