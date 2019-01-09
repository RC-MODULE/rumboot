#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>

.section ".text","ax",@progbits

.global working_function

working_function:
//stack
    stwu    r1,-16(r1)
    stw     r31,12(r1)
    mr      r31,r1

    lwz     r10,  0(r3)
    lwz     r11,  4(r3)
    lwz     r12,  8(r3)
    lwz     r13, 12(r3)
    lwz     r14, 16(r3)
    lwz     r15, 20(r3)
    lwz     r16, 24(r3)
    lwz     r17, 28(r3)
    lwz     r18, 32(r3)
    addi    r10, r10, -4
    addi    r11, r11, -4
    addi    r12, r12, -4
    addi    r13, r13, -4
    addi    r14, r14, -4
    addi    r15, r15, -4
    addi    r16, r16, -4
    addi    r17, r17, -4
    addi    r18, r18, -4
    mtctr   r4

    msync
.align 4
loop:
    lwz     r19, 4(r10)
    lwz     r20, 4(r11)
    lwz     r21, 4(r12)
    lwz     r22, 4(r13)
    lwz     r23, 4(r14)
    lwz     r24, 4(r15)
    lwz     r25, 4(r16)
    lwz     r26, 4(r17)
    lwz     r27, 4(r18)
    bdnz    loop
//stack restore
    addi    r11,r31,16
    lwz     r31,-4(r11)
    mr      r1,r11
    blr
