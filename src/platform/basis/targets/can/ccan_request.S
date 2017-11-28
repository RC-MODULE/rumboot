#include <platform/tubemacro.incl>
#include "devices/can.h"

.global CCAN_REQUEST
.global warning

@ ***arm calling convention***
@ there is CAN_BASE in the R0
@ there is test_state number in the R3

CCAN_REQUEST:
    PUSH {R1-R7, LR}

    tube_msg "- CCAN_REQUEST_TRANSACTION started"

    MOV R1, R0

    BL MEM_CONFIG

    tube_msg "-"

    BL TEST_CONFIG

    tube_msg "-"

    tube_msg "-- Transmit Remote Frame"

    LDR R2, =0x8008     @ Dir == 0 receive; transmit Rem Frame
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8188     @ TxRqst == 1
    STR R2, [R1, #CCAN_IF1_Msg_Con]

    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_2]

    LDR R2, =0x00f3     @ WR/RD == 1 from Message Buf Regs to Message object
    STR R2, [R1, #CCAN_IF1_Com_M]

    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF1_Com_Req]

    tube_msg "-- Request OK"

    tube_msg "-"

    tube_msg "-- Wait for transaction"
    LDR R3, =0x0001

repeat_loop:
    BL INT_SEND
    ADDS R3, R3, #1
    CMP R3, #0x0002     @ number of parcel to send
    BNE repeat_loop
    tube_msg "-- Transaction OK"

    tube_msg "-"

    tube_msg "-- Wait for answer"
    LDR R3, =0x0001
a_repeat_loop:
    BL INT_RECEIVE
    ADDS R3, R3, #1
    CMP R3, #0x0002
    BNE a_repeat_loop
    tube_msg "-- Answer OK"

    tube_msg "-"

    tube_msg "-- Read and Compare"
    LDR R2, =0x0073
    STR R2, [R1, #CCAN_IF1_Com_M]

    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF1_Com_Req]
    LDR R3, [R1, #CCAN_IF1_Dat_A_1]
    LDR R4, =0xf2f1
    CMP R3, R4
    BNE warning
    tube_msg "--- DATA A1 OK"

    LDR R2, =0x8002
    LDR R3, [R1, #CCAN_IF1_Dat_A_2]
    LDR R4, =0xf4f3
    CMP R3, R4
    BNE warning
    tube_msg "--- DATA A2 OK"

    LDR R2, =0x8002
    LDR R3, [R1, #CCAN_IF1_Dat_B_1]
    LDR R4, =0xf6f5
    CMP R3, R4
    BNE warning
    tube_msg "--- DATA B1 OK"

    LDR R2, =0x8002
    LDR R3, [R1, #CCAN_IF1_Dat_B_2]
    LDR R4, =0xf8f7
    CMP R3, R4
    BNE warning

    tube_msg "--- DATA B2 OK"
    tube_msg "--- NICE :)"

cmp_final:
    tube_msg "-- Read and Compare OK"
    MOV R0, #0
@    MOV LR, R7
    tube_msg "-"

    tube_msg "- CCAN_REQUEST_TRANSACTION finished"
    B exit

warning:
    tube_msg "---- MISMATCH :("
    MOV R0, #1                          @ error
    tube_msg "go to exit"

exit:
    POP {R1-R7, LR}
    BX  LR @ return
