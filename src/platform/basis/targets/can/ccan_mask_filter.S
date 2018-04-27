#include <platform/tubemacro.incl>
#include "devices/can.h"

.global CCAN_MASK_FILTER
.global warning

@ ***arm calling convention***
@ there is CAN_BASE in R0
@ there is test_state number in R3

CCAN_MASK_FILTER:
    PUSH {R1-R7, LR}

    tube_hex R0

    tube_msg "- CCAN_MASK_FILTER_TRANSACTION started"

    MOV R1, R0

    tube_hex R1

    BL MEM_CONFIG

    tube_msg "-"

    BL TEST_CONFIG

    tube_msg "-"

    tube_msg "-- Start Storing"
    LDR R2, =0xa008                 @First Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8188
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

    LDR R2, =0xa004                 @Second Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8188
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    LDR R2, =0x0201
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0x0403
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0x0605
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0x0807
    STR R2, [R1, #CCAN_IF1_Dat_B_2]
    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]

    LDR R2, =0xa00c                 @Third Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8188
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    LDR R2, =0x2f1f
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0x4f3f
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0x6f5f
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0x8f7f
    STR R2, [R1, #CCAN_IF1_Dat_B_2]
    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]

    LDR R2, =0xe00c                 @Fourth Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0111
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8188
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

    LDR R2, =0xe00c                 @Fifth Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0110
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8108
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    LDR R2, =0x2b1b
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0x4b3b
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0x6b5b
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0x8b7b
    STR R2, [R1, #CCAN_IF1_Dat_B_2]
    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]

    LDR R2, =0xe00c                 @Sixth Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0111
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8188
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    LDR R2, =0xe2e1
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0xe4e3
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0xe6e5
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0xe8e7
    STR R2, [R1, #CCAN_IF1_Dat_B_2]
    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]

    LDR R2, =0xe00c                 @Seventh Mes
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0110
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8188
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

    LDR R2, =0x8001
    STR R2, [R1, #CCAN_IF1_Com_Req]
@    tube_msg "--- Message 1 stored*"
    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF1_Com_Req]
@    tube_msg "--- Message 2 stored"
    LDR R2, =0x8003
    STR R2, [R1, #CCAN_IF1_Com_Req]
@    tube_msg "--- Message 3 stored"
    LDR R2, =0x8004
    STR R2, [R1, #CCAN_IF1_Com_Req]
@    tube_msg "--- Message 4 stored"
    LDR R2, =0x8005
    STR R2, [R1, #CCAN_IF1_Com_Req]
@    tube_msg "--- Message 5 stored"
    LDR R2, =0x8006
    STR R2, [R1, #CCAN_IF1_Com_Req]
@    tube_msg "--- Message 6 stored"
    LDR R2, =0x8007
    STR R2, [R1, #CCAN_IF1_Com_Req]
@    tube_msg "--- Message 7 stored"

    tube_msg "-- Storing all is OK"

    tube_msg "-"

    tube_msg "-- Wait for transaction"
    LDR R3, =0x0001

repeat_loop:
    BL INT_SEND
    ADDS R3, R3, #1
    CMP R3, #0x0008
    BNE repeat_loop
    tube_msg "-- Transaction OK"

    tube_msg "-"

    MOV R0, #0
    tube_msg "- CCAN_MASK_FILTER_TRANSACTION finished"
    B exit

warning:
    tube_msg "---- MISMATCH :("
    MOV R0, #1                          @ error
    tube_msg "go to exit"

exit:
    POP {R1-R7, LR}
    BX  LR @ return
