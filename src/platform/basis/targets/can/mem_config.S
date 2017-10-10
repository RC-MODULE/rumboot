#include <platform/tubemacro.incl>
#include "devices/can.h"

.global MEM_CONFIG

MEM_CONFIG:

    PUSH {R1-R2, LR}
    @ R1 - here is can_base_addr
    tube_msg "-- Initialize RAM"
    LDR R2, =0x00ff

    STR R2, [R1, #CCAN_IF1_Com_M]
    LDR R2, =0x0001

    tube_msg "init_loop"

init_loop:
    STR R2, [R1, #CCAN_IF1_Arb_2]       @ want to reset MsgVal in all Mes. Obj.
    STR R2, [R1, #CCAN_IF1_Com_Req]
    ADDS R2, R2, #1
    CMP R2, #0x0021
    BNE init_loop
    tube_msg "-- Initialize OK"

    tube_msg "-"

    tube_msg "-- Set Configuration"
    LDR R2, =0x0041
    STR R2, [R1, #CCAN_CAN_Contr_R]

    LDR R2, =0x1C03                     @0x1C03 - Bit Rate = 1000 kb/s (Fcan = 64 MHz)
    STR R2, [R1, #CCAN_Bit_Time_R]

    POP {R1-R2, LR}
    BX  LR
