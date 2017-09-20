#include <platform/tubemacro.incl>
#include "can_defs.h"

.global TEST_CONFIG

TEST_CONFIG:

    PUSH {R1-R5, LR}

    @ R1 - there is can_base_addr
    @ R3 - ther is test_state number

    tube_msg "-- Send Test Configuration Message"
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Arb_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Arb_1]
    LDR R2, =0xdfff
    STR R2, [R1, #CCAN_IF1_Mask_2]
    LDR R2, =0xffff
    STR R2, [R1, #CCAN_IF1_Mask_1]
    LDR R2, =0x8182
    STR R2, [R1, #CCAN_IF1_Msg_Con]
    MOV R2, R3
    STR R2, [R1, #CCAN_IF1_Dat_A_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_A_2]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_1]
    LDR R2, =0x0000
    STR R2, [R1, #CCAN_IF1_Dat_B_2]

    LDR R2, =0x00f3
    STR R2, [R1, #CCAN_IF1_Com_M]
    LDR R2, =0x8020
    STR R2, [R1, #CCAN_IF1_Com_Req]
    LDR R2, [R1, #CCAN_IF1_Com_Req]
    tube_msg "--- Message 32 was stored"
s_repeat_loop:
    LDR R4, =0x100
s_wait_loop:
    SUBS R4, R4, #1
    CMP R4, #0x0000
    BNE s_wait_loop
    LDR R5, [R1, #CCAN_St_R]    @ interrupt
    CMP R5, #0x0000
    BNE s_cmp_7
    tube_msg "---- State = 0"
    B s_repeat_loop
s_cmp_7:
    tube_hex R5
    CMP R5, #0x0007
    BNE s_cmp_TxOk
    tube_msg "---- State = 7"
    B s_repeat_loop
s_cmp_TxOk:
    CMP R5, #0x0008
    BNE s_cmp_error
    tube_msg "---- State = 8"
    B s_cmp_fin
s_cmp_error:

    tube_msg "---- Error during transaction was found. Error code:"
    POP {R1-R5, LR}
    B warning
s_cmp_fin:
    LDR R5, =0x0007             @ interrupt
    STR R5, [R1, #CCAN_St_R]    @ interrupt
    tube_msg "-- Test Configuration Message was sent"

    POP {R1-R5, LR}
    BX  LR
