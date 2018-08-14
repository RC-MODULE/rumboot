/*
 * test_oi10_sys_009.S
 *
 *  Created on: Aug 14, 2018
 *      Author: a.tarasov
 */
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>


.global main

.section ".text","ax",@progbits

main:
    mflr        r3              /* Save */
    mtspr       SPR_SPRG8,  r3  /*  LR  */
    test_event_send_test_id "test_oi10_sys_009"
infine_loop:
    b           infine_loop


    test_error:
    rumboot_putstring "\nTEST ERROR\n", r15, r16, r17
    test_event      EVENT_ERROR, r15, r16, r17
    load_const  r3, RESULT_ERROR
    b finish

test_ok:
    rumboot_putstring "\nTEST OK\n", r15, r16, r17
    test_event      EVENT_OK, r15, r16, r17
    load_const  r3, RESULT_OK

finish:
    mfspr       r4, SPR_SPRG8   /* Restore */
    mtlr        r4              /*   LR    */
    blr
