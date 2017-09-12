#include "basis/tubemacro.incl"
#include "can_defs.h"

.global CCAN_FULL
.global warning

@ ***arm calling convention***
@ there is CAN_BASE in the R0    
@ there is LR in R7 to exit to main.c program after warning
@ there is test_state number in the R3

CCAN_FULL:
    PUSH {R1-R7, LR}

    MOV R7, LR  @ some crunch
    
    tube_msg "- CCAN_FULL_TRANSACTION started"
    
    MOV R1, R0
    
    BL MEM_CONFIG
    
    tube_msg "-"
    
    tube_msg "-- Start FIFO"
    LDR R2, =0xa008                 @ Dir==1 ID==0x8 First Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8288                 @ NewDat==1 RmtEn TxRqst==0 DLC==0x8
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    LDR R2, =0xf2f1
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0xf4f3
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0xf6f5
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0xf8f7
    STR R2, [R1, #CCAN_IF1_Dat_B_2]
    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]
    LDR R2, =0x8001
    STR R2, [R1, #CCAN_IF1_Com_Req]
    tube_msg "--- Message 1 stored"
    LDR R2, =0x8008                 @ Dir==0 ID==0x8 Second Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x0088                 @ NewDat==0 EoB DLC==0x08
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_2]
    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]
    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF1_Com_Req]
    tube_msg "--- Message 2 stored"
    LDR R2, =0xe00c                 @ Xtd, Dir==1 IDmsb==0xC Fourth Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0101                 @ IDlsb==0x0101
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8288                 @ NewDat==1 RmtEn EoB DLC==0x8
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    LDR R2, =0x2e1e
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0x4e3e
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0x6e5e
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0x8e7e
    STR R2, [R1, #CCAN_IF1_Dat_B_2]
    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]
    LDR R2, =0x8004
    STR R2, [R1, #CCAN_IF1_Com_Req]
    tube_msg "--- Message 4 stored"
    LDR R2, =0xc00c                 @ Xtd Dir==0 IDmsb==0xC Fifth Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0101                 @ IDlsb==0x0101
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x0088                 @ NewDat==0 EoB DLC==0x8
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_2]
    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]
    LDR R2, =0x8005
    STR R2, [R1, #CCAN_IF1_Com_Req]
    tube_msg "--- Message 5 stored"
    LDR R2, =0xe00c                 @ Xtd Dir==1 IDmsb==0xC Seventh Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0111                 @ IDlsb==0x0111
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8288                 @ NewDat==1 RmtEn EoB DLC==0x8
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    LDR R2, =0xb2b1
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0xb4b3
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0xb6b5
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0xb8b7
    STR R2, [R1, #CCAN_IF1_Dat_B_2]
    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]
    LDR R2, =0x8007
    STR R2, [R1, #CCAN_IF1_Com_Req]
    tube_msg "--- Message 7 stored"
    LDR R2, =0xc00c                 @ Xtd Dir==0 IDmsb==0xC Eighth Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0111                 @ IDlsb==0x0111
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x0088                 @ NewDat==0 EoB DLC==0x8
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_2]
    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]
    LDR R2, =0x8008
    STR R2, [R1, #CCAN_IF1_Com_Req]
    tube_msg "--- Message 8 stored"

    BL TEST_CONFIG

    tube_msg "-- FIFO OK"
    
    tube_msg "-"

    tube_msg "-- Wait for transaction"      @ INTERRUPT TRACKING ONLY FOR THIS TEST!!
    LDR R6, =0x0001                         @ message/request identifier
    LDR R3, =0x0001                         @ cycle for 3 requests
repeat_loop:
    LDR R4, =0x25                           @ wait cycle
wait_loop:
    SUBS R4, R4, #1
    CMP R4, #0x0000
    BNE wait_loop
    LDR R5, [R1, #CCAN_St_R]                @ interrupt state
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
    CMP R6, #0x0001
    BNE msg_rec
    ADDS R6, R6, #1
    tube_msg "--- Request was received"
    B req_rec
msg_rec:
    ADDS R6, R6, #1
    tube_hex R6
    tube_msg "--- Message was received"
req_rec:
    LDR R5, =0x0007             @ clear interrupt state
    STR R5, [R1, #CCAN_St_R]    @ clear interrupt state
    B repeat_loop
cmp_error:
    tube_msg "--- Error during transaction was found. Error code:"
    tube_hex R5
    B warning                   @ "warning" procedure is described in can-loopback.S
cmp_fin:
    LDR R5, =0x0007             @ clear interrupt state
    STR R5, [R1, #CCAN_St_R]    @ clear interrupt state
    tube_msg "--- Message was sent"
    LDR R6, =0x0001
    ADDS R3, R3, #1
    CMP R3, #0x0004             @ Number of parcels for sending + 1
    BNE repeat_loop

@ Receive the last message
    LDR R3, =0x0001

l_repeat_loop:
    BL INT_RECEIVE
    ADDS R3, R3, #1
    CMP R3, #0x0002
    BNE l_repeat_loop
    tube_msg "-- Transaction OK"
    
    tube_msg "-"
    
    tube_msg "-- Checking"
    LDR R2, =0x0073
    STR R2, [R1, #CCAN_IF2_Com_M]
    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R3, [R1, #CCAN_IF2_Dat_A_1]
    LDR R4, =0x2f1f
    CMP R3, R4
    BNE warning_1
    tube_msg "--- Request 1 is OK"
    B cmp2
warning_1:
    tube_msg "--- Request 1 is not OK"
    B warning
cmp2:
    LDR R2, =0x8005
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R3, [R1, #CCAN_IF2_Dat_A_2]
    LDR R4, =0xe4e3
    CMP R3, R4
    BNE warning_2
    tube_msg "--- Request 2 is OK"
    B cmp3
warning_2:
    tube_msg "--- Request 2 is not OK"
    B warning
cmp3:
    LDR R2, =0x8008
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R3, [R1, #CCAN_IF2_Dat_B_1]
    LDR R4, =0x6b5b
    CMP R3, R4
    BNE warning_3
    tube_msg "--- Request 3 is OK"
    B cmp4
warning_3:
    tube_msg "--- Request 3 is not OK"
    B warning
cmp4:
    tube_msg "-- Checking OK"
    MOV R0, #0
@    MOV LR, R7
    tube_msg "-"
    
    tube_msg "- CCAN_FULL_TRANSACTION finished"
    B exit

warning:
    tube_msg "---- MISMATCH :("
    MOV R0, #1                          @ error
    MOV LR, R7
    tube_msg "go to exit"

exit:    
    POP {R1-R7, LR}
    BX  LR @ return
