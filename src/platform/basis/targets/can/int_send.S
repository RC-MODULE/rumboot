#include <platform/tubemacro.incl>
#include "devices/can.h"

.global INT_SEND

INT_SEND:

    PUSH {R1-R5, LR}

    @ R1 - here is can_base_addr

repeat_loop:
    LDR R4, =0x255
wait_loop:
    SUBS R4, R4, #1
    CMP R4, #0x0000
    BNE wait_loop
    LDR R5, [R1, #CCAN_St_R]    @ interrupt
    CMP R5, #0x0000
    BNE cmp_7
    tube_msg "--- State = 0"
    B repeat_loop
cmp_7:
    CMP R5, #0x0007
    BNE cmp_TxOk
    tube_msg "--- State = 7"
    B repeat_loop
cmp_TxOk:
    CMP R5, #0x0008
    BNE cmp_RxOk
    tube_msg "--- State = 8"
    B cmp_fin
cmp_RxOk:
    CMP R5, #0x0010
    BNE cmp_error
    tube_msg "--- State = 10"
    tube_msg "--- Message/Request was received"
    LDR R5, =0x0007             @ interrupt
    STR R5, [R1, #CCAN_St_R]    @ interrupt
    B repeat_loop
cmp_error:
    tube_msg "--- Error during transaction was found. Error code:"
    tube_hex R5
    POP {R1-R5, LR}
    B warning
cmp_fin:
    LDR R5, =0x0007             @ interrupt
    STR R5, [R1, #CCAN_St_R]    @ interrupt
    tube_msg "--- Message/Request was sent"

    POP {R1-R5, LR}
    BX  LR
