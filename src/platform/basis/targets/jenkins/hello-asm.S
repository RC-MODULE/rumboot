#include <platform/tubemacro.incl>

.global main
main:
    nop
    nop

      /*tube_msg string should not use '%' Symbol! */
    tube_msg "Hello GNU assembler!"

      /*tube_dec and tube_hex macro gets a Register*/
    LDR R1, =0x00bec001
    tube_hex R1

    MOV R2, #42
    tube_dec R2

    mov r0, #0
    mov pc, lr
