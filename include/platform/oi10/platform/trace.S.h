#ifndef TRACE_S_H_
#define TRACE_S_H_

#include <platform/oi10/platform/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_event_codes.h>

//Trace infrastructure convention
//SPR_SPRG0-3 stack vars
//SPR_SPRG6 - tube for string data
//SPR_SPRG7 - tube for hex data
//SPR_SPRG8 - tube for data size available

/*
.global trace_msg_words

.macro trace_msg text
    b 3f

.align 2, 0
1:
    .string "\text"
.align 2, 0
2:
    //end of string

3:
    //do push
    mtspr       SPR_SPRG0, r3
    mtspr       SPR_SPRG1, r4
    mtspr       SPR_SPRG2, r10
    mflr        r10
    mtspr       SPR_SPRG3, r10

    load_addr   r3, 1b           //load string ptr
    load_size   r4, ((2b-1b)/4)  //load total words count

    bl          trace_msg_words

    //do pop
    mfspr       r10, SPR_SPRG3
    mtlr        r10
    mfspr       r10, SPR_SPRG2
    mfspr       r4, SPR_SPRG1
    mfspr       r3, SPR_SPRG0
.endm
*/

//SPR_SPRG3 - message pointer
//SPR_SPRG4 - words count
.macro trace_msg text
    b 3f

1:
    .ascii "\text"
2:
    //end of string
.align 2, 0
3:
    load_addr   r12, 1b           //load string ptr
    mtspr SPR_SPRG3, r12

    load_size   r12, (2b-1b)      //load total words count
    mtspr SPR_SPRG4, r12

    test_event EVENT_TRACE_MESSAGE
.endm


//SPR_SPRG3 - address
//SPR_SPRG4 - dump length
.macro trace_dump address, length

    load_addr   r12, \address            //load string ptr
    mtspr SPR_SPRG3, r12

    load_size   r12, \length             //load total words count
    mtspr SPR_SPRG4, r12

    test_event EVENT_TRACE_DUMP
.endm

//SPR_SPRG3 - data
.macro trace_hex rS
    mtspr SPR_SPRG3, \rS
    test_event EVENT_TRACE_HEX
.endm

#endif /* TRACE_S_H_ */
