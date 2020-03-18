#include <arch/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>

.section ".text","ax",@progbits

.global working_function
.global working_function_no_cache

working_function:
//stack
    stwu    r1,-160(r1)
    stw     r0,156(r1)
    stmw    r2,36(r1)
    mflr    r31
    mfctr   r30
    mfcr    r29
    mfxer   r28
    stmw    r28,20(r1)

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
    li      r6 , -1
    addi    r7 , 0  , -4
    addi    r4 , r4 , -1
    mtctr   r4

    msync
.align 8
    lwzu    r19,  4(r10)
    xor     r19,    r19, r6
    lwzu    r20,  4(r11)
    xor     r20,    r20, r6
    lwzu    r21,  4(r12)
    xor     r21,    r21, r6
    lwzu    r22,  4(r13)
    xor     r22,    r22, r6
    lwzu    r23,  4(r14)
    xor     r23,    r23, r6
    lwzu    r24,  4(r15)
    xor     r24,    r24, r6
    lwzu    r25,  4(r16)
    xor     r25,    r25, r6
    lwzu    r26,  4(r17)
    xor     r26,    r26, r6
    lwzu    r27,  4(r18)
    xor     r27,    r27, r6
    lwzu    r28,  4(r9)
    xor     r28,    r28, r6
    lwzu    r29,  4(r8)
    xor     r29,    r29, r6


.align 8
loop:
    stw     r19,  0(r10)
    lwzu    r19,  4(r10)
    xor     r19,    r19, r6
    lwz     r30,  0(r1)
//    mfctr   r4
//    andi.   r5,     r4, 1
//    bne     odd
    lwz     r31, 0(r1)
//    andi.   r5, r4, 4
//    bne     it_is_three_or_two

.align 8
    stw     r20,  0(r11)
    lwzu    r20,  4(r11)
    xor     r20,    r20, r6
    stw     r21,  0(r12)
    lwzu    r21,  4(r12)
    xor     r21,    r21, r6
//    andi.   r5, r4, 2
//    bne     it_is_two

.align 8
    stw     r22,  0(r13)
    stw     r23,  0(r14)
    lwzu    r22,  4(r13)
    lwzu    r23,  4(r14)
    xor     r22,    r22, r6
    xor     r23,    r23, r6

    stw     r24,  0(r15)
    stw     r25,  0(r16)
    lwzu    r24,  4(r15)
    lwzu    r25,  4(r16)
    xor     r24,    r24, r6
    xor     r25,    r25, r6
//    andi.   r5, r4, 8
//    bne     odd
////    dcbf    r10,    r7,  0
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
    stw     r26, 0(r17)
    stw     r27, 0(r18)
    lwzu    r26, 4(r17)
    lwzu    r27, 4(r18)
    xor     r26,   r26, r6
    xor     r27,   r27, r6
//    andi.   r5, r4, 2
//    bne     it_is_three_or_two

.align 8
    stw     r28,  0(r9 )
    lwzu    r28,  4(r9 )
    xor     r28,    r28, r6
//    lwzu    r21, 4(r7 )
it_is_three_or_two:
    stw     r29,  0(r8 )
    lwzu    r29,  4(r8 )
    xor     r29,    r29, r6
it_is_two:
    bdnz    loop

    stw    r19,  0(r10)
    stw    r20,  0(r11)
    stw    r21,  0(r12)
    stw    r22,  0(r13)
    stw    r23,  0(r14)
    stw    r24,  0(r15)
    stw    r25,  0(r16)
    stw    r26,  0(r17)
    stw    r27,  0(r18)
    stw    r28,  0(r9)
    stw    r29,  0(r8)

//stack restore
    lmw     r28,20(r1)
    mtxer   r28
    mtcr    r29
    mtctr   r30
    mtlr    r31
    lmw     r2,36(r1)
    lwz     r0,156(r1)
    addi    r1,r1,160
    blr


working_function_no_cache:
//stack
    stwu    r1,-160(r1)
    stw     r0,156(r1)
    stmw    r2,36(r1)
    mflr    r31
    mfctr   r30
    mfcr    r29
    mfxer   r28
    stmw    r28,20(r1)

    //r5 with test_data
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

    addi    r5 , r5 , -4
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
    li      r6 , -1
    addi    r7 , 0  , -4
    mtctr   r4

    msync


.align 8
loop_no_cache:
    lwzu    r7 ,  4(r5)
    xor     r7 ,    r7, r6
    stw     r7 ,  0(r5)
    stwu    r7 ,  4(r10)
    stwu    r7 ,  4(r11)
    stwu    r7 ,  4(r12)
    stwu    r7 ,  4(r13)
    stwu    r7 ,  4(r14)
    stwu    r7 ,  4(r15)
    stwu    r7 ,  4(r16)
    stwu    r7 ,  4(r17)
    stwu    r7 ,  4(r18)
    stwu    r7 ,  4(r9 )
    stwu    r7 ,  4(r8 )
    bdnz    loop_no_cache

    msync

//stack restore
    lmw     r28,20(r1)
    mtxer   r28
    mtcr    r29
    mtctr   r30
    mtlr    r31
    lmw     r2,36(r1)
    lwz     r0,156(r1)
    addi    r1,r1,160
    blr
