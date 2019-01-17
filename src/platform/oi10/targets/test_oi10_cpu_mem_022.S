#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>

.section ".text","ax",@progbits

.global working_function

working_function:
//stack
    stwu    r1,-104(r1)
    stw     r31,100(r1)
    stmw    r27, 20(r1)
    mr      r31,r1

    //disable GHR
    mfspr r5, SPR_CCR0
    ori r5, r5, ((1 << IBM_BIT_INDEX(32, 20)))
    mtspr SPR_CCR0, r5

    lwz     r10,  0(r3)
    lwz     r11,  4(r3)
    lwz     r12,  8(r3)
    lwz     r13, 12(r3)
    lwz     r14, 16(r3)
    lwz     r15, 20(r3)
    lwz     r16, 24(r3)
    lwz     r17, 28(r3)
    lwz     r18, 32(r3)
    lwz     r9,  36(r3)
    lwz     r8,  40(r3)
//    lwz     r7,  44(r3)

    addi    r10, r10, -4
    addi    r11, r11, -4
    addi    r12, r12, -4
    addi    r13, r13, -4
    addi    r14, r14, -4
    addi    r15, r15, -4
    addi    r16, r16, -4
    addi    r17, r17, -4
    addi    r18, r18, -4
    addi    r9 , r9 , -4
    addi    r8 , r8 , -4
    li      r6 , 0
    addi    r7 , 0  , -4
    mtctr   r4

    msync
.align 4
loop:
    lwzu    r19,  4(r10)
    xor     r19,    r19, r6
    stw     r19, -4(r10)
    lwz     r20,  0(r1)
    mfctr   r4
    andi.   r5,     r4, 1
    bne     odd
    lwz     r21, 0(r1)
    andi.   r5, r4, 4
    bne     it_is_three_or_two

.align 8
    lwzu    r20,  4(r11)
    xor     r20,    r20, r6
    stw     r20, -4(r11)
    lwzu    r21,  4(r12)
    xor     r21,    r21, r6
    stw     r21, -4(r12)
    andi.   r5, r4, 2
    bne     it_is_two

.align 8
    lwzu    r22,  4(r13)
    lwzu    r23,  4(r14)
    xor     r22,    r22, r6
    xor     r23,    r23, r6
    stw     r22, -4(r13)
    stw     r23, -4(r14)

    lwzu    r24,  4(r15)
    lwzu    r25,  4(r16)
    xor     r24,    r24, r6
    xor     r25,    r25, r6
    stw     r24, -4(r15)
    stw     r25, -4(r16)
    andi.   r5, r4, 8
    bne     odd
//    dcbf    r10,    r7,  0
    dcbf    r11,    r7,  0
    dcbf    r12,    r7,  0
    dcbf    r13,    r7,  0
    dcbf    r14,    r7,  0
    dcbf    r15,    r7,  0
    dcbf    r16,    r7,  0
    dcbf    r17,    r7,  0
    dcbf    r18,    r7,  0
    dcbf    r9 ,    r7,  0
    dcbf    r8 ,    r7,  0

.align 8
odd:
//zero or odd
    lwzu    r26, 4(r17)
    lwzu    r27, 4(r18)
    xor     r26,   r26, r6
    xor     r27,   r27, r6
    stw     r24, -4(r17)
    stw     r25, -4(r18)
    andi.   r5, r4, 2
    bne     it_is_three_or_two

.align 8
    lwzu    r19,  4(r9 )
    xor     r19,    r19, r6
    stw     r19, -4(r9 )
//    lwzu    r21, 4(r7 )
it_is_three_or_two:
    lwzu    r20,  4(r8 )
    xor     r20,    r20, r6
    stw     r20, -4(r8 )
it_is_two:
    bdnz    loop
//stack restore
    addi    r11,r31,104
    lwz     r31,-4(r11)
    lmw     r27,-84(r11)
    mr      r1,r11
    blr
