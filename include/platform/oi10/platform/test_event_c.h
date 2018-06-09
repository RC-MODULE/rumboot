/*
 *
 *  Created on: Jun 9, 2018
 *      Author: v.strukov
 */

#ifndef TEST_EVENT_C_H_
#define TEST_EVENT_C_H_

#include <platform/test_event_codes.h>
#include <platform/arch/ppc/ppc_476fp_config.h>

static inline void test_event( uint32_t const event_code ) {
    unsigned volatile test_event_opcode = EVENT_TESTEVENT;
    unsigned volatile nop_opcode        = EVENT_NOP;
    asm volatile(
        "mtspr %0, %1 \n\t"
        "mtspr %2, %3 \n\t"
        "mtspr %2, %4 \n\t"
        ::"n"(SPR_SPRG2), "r"(event_code),
         "n"(SPR_SPRG1), "r"(test_event_opcode),
         "r"(nop_opcode)
    );
}


#endif /* TEST_EVENT_C_H_ */
