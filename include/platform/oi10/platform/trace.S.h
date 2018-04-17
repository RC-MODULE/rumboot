#ifndef TRACE_S_H_
#define TRACE_S_H_

#include <platform/oi10/platform/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_event_codes.h>

.macro _putchar reg_data, reg_ctrl
    load_const  \reg_ctrl, 2;
    mtspr       SPR_SPRG2, \reg_data
    load_const  \reg_data, 0
    mtspr       SPR_SPRG1, r12
    mtspr       SPR_SPRG1, \reg_data
.endm

.macro putchar char
    load_const  r13, \char; /* test stdout */
    _putchar r13, r12
.endm


.macro trace_msg text
    b 3f

1:
    .ascii "\text"
2:
    //end of string
.align 2, 0
3:
    load_addr   r14, 1b           //load string ptr
    addi        r14, r14, -1      //because of lbzu
    load_addr   r15, 2b
    load_const  r12, 2;
4:
    lbzu  r13, 1(r14) /* test stdout */
    _putchar r13, r12
    cmplw cr0, r14, r15
    blt 4b

.endm



#endif /* TRACE_S_H_ */
