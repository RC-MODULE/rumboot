#include <platform/tubemacro.incl>
#include "devices/can.h"

.global CCAN_LOOPBACK
.global warning

@ ***arm calling convention***
@ there is CAN_BASE in R0
@ there is test_state number in R3

CCAN_LOOPBACK:
    PUSH {R1-R7, LR}

    tube_msg "- CCAN_LOOP_BACK started"

    MOV R1, R0

    BL MEM_CONFIG

    tube_msg "-"

    BL TEST_CONFIG

    tube_msg "-"

@    tube_msg "-- Start Real Real"
@    LDR R2, =0x000e
@    STR R2, [R1, #CCAN_CAN_Contr_R]
@    @ set CCAN_CAN_Contr_R[7] to normal operation?

    tube_msg "-- Start LoopBack"
    LDR R2, =0x0010
    STR R2, [R1, #CCAN_Test_R]

    @LDR R2, =0xa008
    @STR R2, [R1, #CCAN_IF1_Arb_2]
    @LDR R2, =0x0000
    @STR R2, [R1, #CCAN_IF1_Arb_1]
    @LDR R2, =0xdfff
    @STR R2, [R1, #CCAN_IF1_Mask_2]
    @LDR R2, =0xffff
    @STR R2, [R1, #CCAN_IF1_Mask_1]
    @LDR R2, =0x8188
    @STR R2, [R1, #CCAN_IF1_Msg_Con]
    @LDR R2, =0xf2f1
    @STR R2, [R1, #CCAN_IF1_Dat_A_1]
    @LDR R2, =0xf4f3
    @STR R2, [R1, #CCAN_IF1_Dat_A_2]
    @LDR R2, =0xf6f5
    @STR R2, [R1, #CCAN_IF1_Dat_B_1]
    @LDR R2, =0xf8f7
    @STR R2, [R1, #CCAN_IF1_Dat_B_2]
    @LDR R2, =0x00f3
    @STR R2, [R1, #CCAN_IF1_Com_M]
    @
    @LDR R2, =0x8008
    @STR R2, [R1, #CCAN_IF2_Arb_2]
    @LDR R2, =0x0000
    @STR R2, [R1, #CCAN_IF2_Arb_1]
    @LDR R2, =0xdfff
    @STR R2, [R1, #CCAN_IF2_Mask_2]
    @LDR R2, =0xffff
    @STR R2, [R1, #CCAN_IF2_Mask_1]
    @LDR R2, =0x8080
    @STR R2, [R1, #CCAN_IF2_Msg_Con]
    @LDR R2, =0x0000
    @STR R2, [R1, #CCAN_IF2_Dat_A_1]
    @LDR R2, =0x0000
    @STR R2, [R1, #CCAN_IF2_Dat_A_2]
    @LDR R2, =0x0000
    @STR R2, [R1, #CCAN_IF2_Dat_B_1]
    @LDR R2, =0x0000
    @STR R2, [R1, #CCAN_IF2_Dat_B_2]
    @LDR R2, =0x00f3
    @STR R2, [R1, #CCAN_IF2_Com_M]
    @
    @LDR R2, =0x8001
    @STR R2, [R1, #CCAN_IF1_Com_Req]
    @LDR R2, =0x8002
    @STR R2, [R1, #CCAN_IF2_Com_Req]

    LDR R2, =0xa008
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
    LDR R2, =0x8001
    STR R2, [R1, #CCAN_IF1_Com_Req]

    LDR R2, =0x8008
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8080
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

    tube_msg "-- LoopBack OK"

    tube_msg "-"

    tube_msg "-- Wait for transaction"  @Universal Tracker
    LDR R3, =0x0001

repeat_loop:
    BL INT_SEND
    ADDS R3, R3, #1
    CMP R3, #0x0002                     @Number of parcels for sending + 1
    BNE repeat_loop
    tube_msg "-- Transaction OK"

    tube_msg "-"

    tube_msg "-- Read and Compare"
    LDR R2, =0x0073
    STR R2, [R1, #CCAN_IF2_Com_M]

    LDR R2, =0x8001
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R3, [R1, #CCAN_IF2_Dat_A_1]
    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R4, [R1, #CCAN_IF2_Dat_A_1]
    CMP R3, R4
    BNE warning
    tube_msg "--- DATA A1 OK"

    LDR R2, =0x8001
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R3, [R1, #CCAN_IF2_Dat_A_2]
    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R4, [R1, #CCAN_IF2_Dat_A_2]
    CMP R3, R4
    BNE warning
    tube_msg "--- DATA A2 OK"

    LDR R2, =0x8001
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R3, [R1, #CCAN_IF2_Dat_B_1]
    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R4, [R1, #CCAN_IF2_Dat_B_1]
    CMP R3, R4
    BNE warning
    tube_msg "--- DATA B1 OK"

    LDR R2, =0x8001
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R3, [R1, #CCAN_IF2_Dat_B_2]
    LDR R2, =0x8002
    STR R2, [R1, #CCAN_IF2_Com_Req]
    LDR R4, [R1, #CCAN_IF2_Dat_B_2]
    CMP R3, R4
    BNE warning
    tube_msg "--- DATA B2 OK"

    tube_msg "---- NICE :)"
    B cmp_final

warning:
    tube_msg "---- MISMATCH :("
    MOV R0, #1                          @ error
    tube_msg "go to exit"
    B exit

cmp_final:
    tube_msg "-- Read and Compare OK"
    MOV R0, #0
    tube_msg "-"

    tube_msg "- CCAN_LOOP_BACK finished"

exit:
    POP {R1-R7, LR}
    BX  LR
