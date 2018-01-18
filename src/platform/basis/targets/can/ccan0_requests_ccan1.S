#include <platform/tubemacro.incl>
#include "devices/can.h"

.global CCAN0_REQUESTS_CCAN1
.global warning

@ ***arm calling convention***
@ there is CAN0_BASE in the R0
@ there is CAN1_BASE in the R3

CCAN0_REQUESTS_CCAN1:
    PUSH {R1-R7, LR}

    tube_msg "- CCAN0_REQUESTS_CCAN1 started"

    @ config CCAN0
    MOV R1, R0
    BL MEM_CONFIG
    LDR R2, =0x000e
    STR R2, [R1, #CCAN_CAN_Contr_R]

    @ config CCAN1
    MOV R1, R3
    BL MEM_CONFIG
    LDR R2, =0x000e
    STR R2, [R1, #CCAN_CAN_Contr_R]

    tube_msg "-- Configuration OK"

    @ ***config CCAN0 to transmit request then to receive remote frame***
    MOV R1, R0

    tube_msg "-- Transmit Remote Frame"

    LDR R2, =0x8008     @ MsgVal ~Xtd ~Dir
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8188     @ NewDat ~RmtEn TxRqst 
    STR R2, [R1, #CCAN_IF1_Msg_Con]

    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_2]

    @ move DataA & DataB from Message Buf Regs to Message object
    LDR R2, =0x00f3     @ WR/RD Mask Arb Control DataA DataB
    STR R2, [R1, #CCAN_IF1_Com_M]

    @ ***config CCAN1 to transmit remote frame in case of receiving request***
    MOV R1, R3

    LDR R2, =0xA008     @ MsgVal ~Xtd Dir
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0X8288     @ NewDat RmtEn ~TxRqst
    STR R2, [R1, #CCAN_IF1_Msg_Con]

    LDR R2, =0xf2f1
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0xf4f3
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0xf6f5
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0xf8f7
    STR R2, [R1, #CCAN_IF1_Dat_B_2]

    @ move DataA & DataB from Message Buf Regs to Message object
    LDR R2, =0x00f3     @ WR/RD Mask Arb Control DataA DataB
    STR R2, [R1, #CCAN_IF1_Com_M]

    @ start CCAN0
    MOV R1, R0
    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF1_Com_Req]

    @ start CCAN1
    MOV R1, R3
    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF1_Com_Req]

    tube_msg "-- Config CCAN0 & CCAN1 OK"

@ ***CCAN0 transmits request to receive remote frame***
    MOV R1, R0
    tube_msg "-- CCAN0: sending request"
    BL INT_SEND
    tube_msg "-- Request: sent"

@ ***CCAN1 receives request to send remote frame***
    MOV R1, R3
    tube_msg "-- CCAN1: Receive request"
    BL INT_RECEIVE
    tube_msg "-- Request received"

@ ***CCAN1 transmites remote frame***
    tube_msg "-- CCAN1: sending remote frame"
    BL INT_SEND
    tube_msg "-- Remote frame: sent"

@ ***CCAN0 receives remote frame***
    MOV R1, R0
    tube_msg "-- CCAN0: Receive remote frame"
    BL INT_RECEIVE
    tube_msg "-- Remote frame: received"

@ ***Read and compare received data (CCAN0)***
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
    STR R2, [R1, #CCAN_IF1_Com_Req]
    LDR R3, [R1, #CCAN_IF1_Dat_A_2]
    LDR R4, =0xf4f3
    CMP R3, R4
    BNE warning
    tube_msg "--- DATA A2 OK"

    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF1_Com_Req]
    LDR R3, [R1, #CCAN_IF1_Dat_B_1]
    LDR R4, =0xf6f5
    CMP R3, R4
    BNE warning
    tube_msg "--- DATA B1 OK"

    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF1_Com_Req]
    LDR R3, [R1, #CCAN_IF1_Dat_B_2]
    LDR R4, =0xf8f7
    CMP R3, R4
    BNE warning

    tube_msg "--- DATA B2 OK"
    tube_msg "--- NICE :)"

cmp_final:
    tube_msg "-- Read and Compare OK"
    MOV R0, #0
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
