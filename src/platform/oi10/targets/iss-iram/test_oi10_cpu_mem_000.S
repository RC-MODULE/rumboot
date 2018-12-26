#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/devices.h>
#include <platform/trace.S.h>



#define TEST_OI10_CPU_MEM_000_ZERO_OFFSET            0x0000
#define TEST_OI10_CPU_MEM_000_ONE_OFFSET             0x0010
#define TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET     0x0020
#define TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET    0x00A0


.macro read_from_mem_zero_clean reg, offset
    .align 4
    lwz             \reg, \offset(r0)
    lwz             \reg, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r0)
    isync
.endm

.macro read_from_mem_zero_clean_with_addr reg, offset, addr
    .align 4
    lwz             \reg, \offset(\addr)
//    stwu            \reg, 4(r6)
    lwz             \reg, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(\addr)
    isync
.endm

.macro read_from_mem_one_clean_with_addr reg, offset, addr
    .align 4
    lwz             \reg, \offset(\addr)
    lwz             \reg, TEST_OI10_CPU_MEM_000_ONE_OFFSET(\addr)
    isync
.endm

.macro clear_regs_with_0
    .align 4
    li r0,0x0
    li r1,0x0
    li r2,0x0
    li r3,0x0
    li r4,0x0
    li r5,0x0
    li r6,0x0
    li r7,0x0
    li r8,0x0
    li r9,0x0
    li r10,0x0
    li r11,0x0
    li r12,0x0
    li r13,0x0
    li r14,0x0
    li r15,0x0
    li r16,0x0
    li r17,0x0
    li r18,0x0
    li r19,0x0
    li r20,0x0
    li r21,0x0
    li r22,0x0
    li r23,0x0
    li r24,0x0
    li r25,0x0
    li r26,0x0
    li r27,0x0
    li r28,0x0
    li r29,0x0
    li r30,0x0
    li r31,0x0
    mtspr SPR_SPRG8, r0
.endm

.macro clear_regs_with_0_xor
    .align 4
    xor r0, r0, r0
    xor r1, r1, r1
    xor r2, r2, r2
    xor r3, r3, r3
    xor r4, r4, r4
    xor r5, r5, r5
    xor r6, r6, r6
    xor r7, r7, r7
    xor r8, r8, r8
    xor r9, r9, r9
    xor r10,r10,r10
    xor r11,r11,r11
    xor r12,r12,r12
    xor r13,r13,r13
    xor r14,r14,r14
    xor r15,r15,r15
    xor r16,r16,r16
    xor r17,r17,r17
    xor r18,r18,r18
    xor r19,r19,r19
    xor r20,r20,r20
    xor r21,r21,r21
    xor r22,r22,r22
    xor r23,r23,r23
    xor r24,r24,r24
    xor r25,r25,r25
    xor r26,r26,r26
    xor r27,r27,r27
    xor r28,r28,r28
    xor r29,r29,r29
    xor r30,r30,r30
    xor r31,r31,r31
.endm

.macro nor_with_itself reg
    nor \reg,\reg,\reg
.endm

.macro clear_regs_with_1
    .align 4
    nor_with_itself r0
    nor_with_itself r1
    nor_with_itself r2
    nor_with_itself r3
    nor_with_itself r4
    nor_with_itself r5
    nor_with_itself r6
    nor_with_itself r7
    nor_with_itself r8
    nor_with_itself r9
    nor_with_itself r10
    nor_with_itself r11
    nor_with_itself r12
    nor_with_itself r13
    nor_with_itself r14
    nor_with_itself r15
    nor_with_itself r16
    nor_with_itself r17
    nor_with_itself r18
    nor_with_itself r19
    nor_with_itself r20
    nor_with_itself r21
    nor_with_itself r22
    nor_with_itself r23
    nor_with_itself r24
    nor_with_itself r25
    nor_with_itself r26
    nor_with_itself r27
    nor_with_itself r28
    nor_with_itself r29
    nor_with_itself r30
    nor_with_itself r31
    mtspr SPR_SPRG8, r0
.endm

.macro xori_reg reg,value
    .align 4
    xori \reg,\reg,\value
    xori \reg,\reg,\value
    .align 4
.endm

.macro xoris_reg reg,value
    .align 4
    xoris \reg,\reg,\value
    xoris \reg,\reg,\value
    .align 4
.endm

.macro xori_reg_spr reg,value,spr
    .align 4
    xori \reg,\reg,\value
    mtspr \spr,\reg
//    mulli \reg,\reg,0x1
    xori \reg,\reg,\value
    .align 4
.endm

.macro xoris_reg_spr reg,value,spr
    .align 4
    xoris \reg,\reg,\value
    mtspr \spr,\reg
//    mulli \reg,\reg,0x1
    xoris \reg,\reg,\value
    .align 4
.endm

.macro mfspr_reg reg,spr
    mfspr \reg,\spr
    mfspr \reg,SPR_SPRG8
.endm



.section ".data","aw",@nobits

.align 4
results:
    .space 4096
#define RESULTS_SIZE 0x680

.section ".data","aw",@progbits
.align 4
event_data:
    .long TEST_EVENT_CREATE_ISS_DUMP
    .long results
    .long RESULTS_SIZE
#define EVENT_DATA_SIZE 3


.global main
.section ".text","ax",@progbits

.global test_oi10_cpu_mem_000_data_zero
.global test_oi10_cpu_mem_000_data_one
.global test_oi10_cpu_mem_000_data_sliding_one
.global test_oi10_cpu_mem_000_data_sliding_zero


.org TEST_OI10_CPU_MEM_000_ZERO_OFFSET
test_oi10_cpu_mem_000_data_zero:
    .long           0x00000000
.org TEST_OI10_CPU_MEM_000_ONE_OFFSET
test_oi10_cpu_mem_000_data_one:
    .long           0xFFFFFFFF
.org TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET
test_oi10_cpu_mem_000_data_sliding_one:
    .long           0x00000001
    .long          (0x00000001 << 1)
    .long          (0x00000001 << 2)
    .long          (0x00000001 << 3)
    .long          (0x00000001 << 4)
    .long          (0x00000001 << 5)
    .long          (0x00000001 << 6)
    .long          (0x00000001 << 7)
    .long          (0x00000001 << 8)
    .long          (0x00000001 << 9)
    .long          (0x00000001 << 10)
    .long          (0x00000001 << 11)
    .long          (0x00000001 << 12)
    .long          (0x00000001 << 13)
    .long          (0x00000001 << 14)
    .long          (0x00000001 << 15)
    .long          (0x00000001 << 16)
    .long          (0x00000001 << 17)
    .long          (0x00000001 << 18)
    .long          (0x00000001 << 19)
    .long          (0x00000001 << 20)
    .long          (0x00000001 << 21)
    .long          (0x00000001 << 22)
    .long          (0x00000001 << 23)
    .long          (0x00000001 << 24)
    .long          (0x00000001 << 25)
    .long          (0x00000001 << 26)
    .long          (0x00000001 << 27)
    .long          (0x00000001 << 28)
    .long          (0x00000001 << 29)
    .long          (0x00000001 << 30)
    .long          (0x00000001 << 31)
.org TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET
test_oi10_cpu_mem_000_data_sliding_zero:
    .long          ~0x00000001
    .long          ~(0x00000001 << 1)
    .long          ~(0x00000001 << 2)
    .long          ~(0x00000001 << 3)
    .long          ~(0x00000001 << 4)
    .long          ~(0x00000001 << 5)
    .long          ~(0x00000001 << 6)
    .long          ~(0x00000001 << 7)
    .long          ~(0x00000001 << 8)
    .long          ~(0x00000001 << 9)
    .long          ~(0x00000001 << 10)
    .long          ~(0x00000001 << 11)
    .long          ~(0x00000001 << 12)
    .long          ~(0x00000001 << 13)
    .long          ~(0x00000001 << 14)
    .long          ~(0x00000001 << 15)
    .long          ~(0x00000001 << 16)
    .long          ~(0x00000001 << 17)
    .long          ~(0x00000001 << 18)
    .long          ~(0x00000001 << 19)
    .long          ~(0x00000001 << 20)
    .long          ~(0x00000001 << 21)
    .long          ~(0x00000001 << 22)
    .long          ~(0x00000001 << 23)
    .long          ~(0x00000001 << 24)
    .long          ~(0x00000001 << 25)
    .long          ~(0x00000001 << 26)
    .long          ~(0x00000001 << 27)
    .long          ~(0x00000001 << 28)
    .long          ~(0x00000001 << 29)
    .long          ~(0x00000001 << 30)
    .long          ~(0x00000001 << 31)



main:
    full_prologue
    mtctr       r1  // save stack ptr

    test_event_send_test_id "test_iss_base"

    load_addr   r31, results
    mtspr SPR_VRSAVE, r31

    clear_regs_with_1


    rumboot_putstring "Checking LWZ ...\n"
//    test_event EVENT_GPRS_CLEARED_START_CHECK_LWZ


    .align 4
    lis             r31, test_oi10_cpu_mem_000_data_zero@h
    ori             r31, r31, test_oi10_cpu_mem_000_data_zero@l

    .align 4
    lwz             r0 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r1 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r2 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r3 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r4 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r5 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r6 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r7 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r8 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r9 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r10, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r11, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r12, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r13, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r14, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r15, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r16, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r17, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r18, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r19, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r20, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r21, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r22, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r23, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r24, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r25, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r26, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r27, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r28, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r29, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r30, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r31, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)

    isync

    mtspr SPR_SPRG8, r31
    mfspr r31, SPR_VRSAVE
    stmw r0, 0(r31)
    mfspr r30, SPR_SPRG8
    stw r30, 124(r31)
    addi r31, r31, 128
    mtspr SPR_VRSAVE, r31
    li r31, 0
    mtspr SPR_SPRG8, r31


//    clear_regs_with_0

    lis             r7, test_oi10_cpu_mem_000_data_zero@h
    ori             r7, r7, test_oi10_cpu_mem_000_data_zero@l

    read_from_mem_zero_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET       ), r7
    read_from_mem_zero_clean_with_addr r1 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET +  0x4), r7
    read_from_mem_zero_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET +  0x8), r7
    read_from_mem_zero_clean_with_addr r2 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET +  0xC), r7
    read_from_mem_zero_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x10), r7
    read_from_mem_zero_clean_with_addr r4 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x14), r7
    read_from_mem_zero_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x18), r7
    read_from_mem_zero_clean_with_addr r8 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x1C), r7
    read_from_mem_zero_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x20), r7
    read_from_mem_zero_clean_with_addr r16, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x24), r7
    read_from_mem_zero_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x28), r7
    read_from_mem_zero_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x2C), r7
    read_from_mem_zero_clean_with_addr r30, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x30), r7
    read_from_mem_zero_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x34), r7
    read_from_mem_zero_clean_with_addr r29, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x38), r7
    read_from_mem_zero_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x3C), r7
    read_from_mem_zero_clean_with_addr r27, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x40), r7
    read_from_mem_zero_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x44), r7
    read_from_mem_zero_clean_with_addr r23, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x48), r7
    read_from_mem_zero_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x4C), r7
    read_from_mem_zero_clean_with_addr r15, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x50), r7
    read_from_mem_zero_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x54), r7
    read_from_mem_zero_clean_with_addr r1 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x58), r7
    read_from_mem_zero_clean_with_addr r2 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x5C), r7
    read_from_mem_zero_clean_with_addr r4 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x60), r7
    read_from_mem_zero_clean_with_addr r8 , (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x64), r7
    read_from_mem_zero_clean_with_addr r16, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x68), r7
    read_from_mem_zero_clean_with_addr r30, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x6C), r7
    read_from_mem_zero_clean_with_addr r29, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x70), r7
    read_from_mem_zero_clean_with_addr r27, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x74), r7
    read_from_mem_zero_clean_with_addr r23, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x78), r7
    read_from_mem_zero_clean_with_addr r15, (TEST_OI10_CPU_MEM_000_SLIDING_ONE_OFFSET + 0x7C), r7


    mtspr SPR_SPRG8, r31
    mfspr r31, SPR_VRSAVE
    stmw r0, 0(r31)
    mfspr r30, SPR_SPRG8
    stw r30, 124(r31)
    addi r31, r31, 128
    mtspr SPR_VRSAVE, r31
    li r31, 0
    mtspr SPR_SPRG8, r31


    .align 4
    lis             r31, test_oi10_cpu_mem_000_data_zero@h
    ori             r31, r31, test_oi10_cpu_mem_000_data_zero@l

    .align 4
    lwz             r0 , TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r1 , TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r2 , TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r3 , TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r4 , TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r5 , TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r6 , TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r7 , TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r8 , TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r9 , TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r10, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r11, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r12, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r13, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r14, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r15, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r16, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r17, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r18, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r19, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r20, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r21, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r22, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r23, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r24, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r25, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r26, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r27, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r28, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r29, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r30, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)
    lwz             r31, TEST_OI10_CPU_MEM_000_ONE_OFFSET(r31)

    isync


    mtspr SPR_SPRG8, r31
    mfspr r31, SPR_VRSAVE
    stmw r0, 0(r31)
    mfspr r30, SPR_SPRG8
    stw r30, 124(r31)
    addi r31, r31, 128
    mtspr SPR_VRSAVE, r31
    li r31, 0
    mtspr SPR_SPRG8, r31


    lis             r7, test_oi10_cpu_mem_000_data_zero@h
    ori             r7, r7, test_oi10_cpu_mem_000_data_zero@l

    read_from_mem_one_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET       ), r7
    read_from_mem_one_clean_with_addr r1 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET +  0x4), r7
    read_from_mem_one_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET +  0x8), r7
    read_from_mem_one_clean_with_addr r2 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET +  0xC), r7
    read_from_mem_one_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x10), r7
    read_from_mem_one_clean_with_addr r4 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x14), r7
    read_from_mem_one_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x18), r7
    read_from_mem_one_clean_with_addr r8 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x1C), r7
    read_from_mem_one_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x20), r7
    read_from_mem_one_clean_with_addr r16, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x24), r7
    read_from_mem_one_clean_with_addr r0 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x28), r7
    read_from_mem_one_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x2C), r7
    read_from_mem_one_clean_with_addr r30, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x30), r7
    read_from_mem_one_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x34), r7
    read_from_mem_one_clean_with_addr r29, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x38), r7
    read_from_mem_one_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x3C), r7
    read_from_mem_one_clean_with_addr r27, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x40), r7
    read_from_mem_one_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x44), r7
    read_from_mem_one_clean_with_addr r23, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x48), r7
    read_from_mem_one_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x4C), r7
    read_from_mem_one_clean_with_addr r15, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x50), r7
    read_from_mem_one_clean_with_addr r31, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x54), r7
    read_from_mem_one_clean_with_addr r1 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x58), r7
    read_from_mem_one_clean_with_addr r2 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x5C), r7
    read_from_mem_one_clean_with_addr r4 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x60), r7
    read_from_mem_one_clean_with_addr r8 , (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x64), r7
    read_from_mem_one_clean_with_addr r16, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x68), r7
    read_from_mem_one_clean_with_addr r30, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x6C), r7
    read_from_mem_one_clean_with_addr r29, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x70), r7
    read_from_mem_one_clean_with_addr r27, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x74), r7
    read_from_mem_one_clean_with_addr r23, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x78), r7
    read_from_mem_one_clean_with_addr r15, (TEST_OI10_CPU_MEM_000_SLIDING_ZERO_OFFSET + 0x7C), r7
    isync


    mtspr SPR_SPRG8, r31
    mfspr r31, SPR_VRSAVE
    stmw r0, 0(r31)
    mfspr r30, SPR_SPRG8
    stw r30, 124(r31)
    addi r31, r31, 128
    mtspr SPR_VRSAVE, r31
    li r31, 0
    mtspr SPR_SPRG8, r31


    .align 4
    lis             r31, test_oi10_cpu_mem_000_data_zero@h
    ori             r31, r31, test_oi10_cpu_mem_000_data_zero@l


    .align 4
    lwz             r0 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r1 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r2 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r3 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r4 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r5 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r6 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r7 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r8 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r9 , TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r10, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r11, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r12, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r13, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r14, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r15, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r16, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r17, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r18, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r19, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r20, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r21, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r22, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r23, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r24, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r25, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r26, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r27, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r28, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r29, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r30, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)
    lwz             r31, TEST_OI10_CPU_MEM_000_ZERO_OFFSET(r31)

    isync


    mtspr SPR_SPRG8, r31
    mfspr r31, SPR_VRSAVE
    stmw r0, 0(r31)
    mfspr r30, SPR_SPRG8
    stw r30, 124(r31)
    addi r31, r31, 128
    mtspr SPR_VRSAVE, r31
    li r31, 0
    mtspr SPR_SPRG8, r31

    clear_regs_with_1



    rumboot_putstring "Checking MAC ...\n"
//    test_event EVENT_GPRS_CLEARED_START_CHECK_MAC
    msync
    isync
    clear_regs_with_0

    li r1,1
    li r2,1
    li r4,1

    mfspr r31, SPR_VRSAVE

    maclhw r5,r2,r5

    stw r5, 0(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 4(r31)
    stw r5, 8(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 12(r31)
    stw r5, 16(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 20(r31)
    stw r5, 24(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 28(r31)
    stw r5, 32(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 36(r31)
    stw r5, 40(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 44(r31)
    stw r5, 48(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 52(r31)
    stw r5, 56(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 60(r31)
    stw r5, 64(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 68(r31)
    stw r5, 72(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 76(r31)
    stw r5, 80(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 84(r31)
    stw r5, 88(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 92(r31)
    stw r5, 96(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 100(r31)
    stw r5, 104(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 108(r31)
    stw r5, 112(r31)

    maclhw r2,r1,r2
    maclhw r5,r2,r5

    stw r2, 116(r31)
    stw r5, 120(r31)



    .align 4
    ori r6, r6,0x8000 // r6 == 8000
    .align 4
    maclhwu r2, r1,r2 // r2 == 10000
    maclhw r5,r2,r5

    stw r2, 124(r31)
    stw r5, 128(r31)



    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0

    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0

    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0

    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0

    maclhwu r1, r1,r4 //r1 == 2
    maclhw r5,r2,r5

    stw r1, 132(r31)
    stw r5, 136(r31)

    maclhwu r2, r1,r6 //r2 == 20000
    maclhw r5,r2,r5

    stw r2, 140(r31)
    stw r5, 144(r31)

    maclhwu r1, r1,r4 //r1 == 4
    maclhw r5,r2,r5

    stw r1, 148(r31)
    stw r5, 152(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 156(r31)
    stw r5, 160(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 164(r31)
    stw r5, 168(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 172(r31)
    stw r5, 176(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 180(r31)
    stw r5, 184(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 188(r31)
    stw r5, 192(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 196(r31)
    stw r5, 200(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 204(r31)
    stw r5, 208(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 212(r31)
    stw r5, 216(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 220(r31)
    stw r5, 224(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 228(r31)
    stw r5, 232(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 236(r31)
    stw r5, 240(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 244(r31)
    stw r5, 248(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 252(r31)
    stw r5, 256(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 260(r31)
    stw r5, 264(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 268(r31)
    stw r5, 272(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 276(r31)
    stw r5, 280(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 284(r31)
    stw r5, 288(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 292(r31)
    stw r5, 296(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 300(r31)
    stw r5, 304(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 308(r31)
    stw r5, 312(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 316(r31)
    stw r5, 320(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 324(r31)
    stw r5, 328(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 332(r31)
    stw r5, 336(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 340(r31)
    stw r5, 344(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 348(r31)
    stw r5, 352(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 356(r31)
    stw r5, 360(r31)

    maclhwu r2, r1,r6
    maclhw r5,r2,r5

    stw r2, 364(r31)
    stw r5, 368(r31)

    maclhwu r1, r1,r4
    maclhw r5,r2,r5

    stw r1, 372(r31)
    stw r5, 376(r31)

    maclhwu r2, r1,r6

    stw r2, 380(r31)


    .align 4
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0

    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0

    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0

    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0


    addi r31, r31, 384
    mtspr SPR_VRSAVE, r31



    rumboot_putstring "Checking STW ...\n"
//    test_event EVENT_GPRS_CLEARED_START_CHECK_STW

    mfspr r3, SPR_VRSAVE
    mr r7, r3
    subi r3, r3, 4
    addi r7, r7, 4


    .align 4
    li r1,1
    li r2,1
    li r4,1
    stwu    r2, 4(r3) //0
    isync

    //mullw  r5,r1,r2
    maclhw r5,r2,r5
    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //1
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //2
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //3
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //4
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //5
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //6
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //7
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //8
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //9
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //10
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //11
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //12
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //13
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //14
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    maclhw r2,r1,r2
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //15
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 4
    addi r7, r7, 4

    ori r6, r6,0x8000 // r6 == 8000
    maclhwu r2, r1,r2 // r2 == 10000
    maclhw r5,r2,r5
    stwu    r2, 4(r3) //16
    stwu    r5, 4(r7) //
    isync

    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0

    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0

    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0

    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    xori 0,0,0
    //isync
    //r1 == 1
    //r2 == 8000
    //r4 == 1
    //r5 == 0
    //r6 == 0



    mr r8, r7
    subi r8, r8, 4
    addi r7, r7, 4
    addi r3, r3, 12

    stwu    r1, 4(r8)

    maclhwu r1, r1,r4 //r1 == 2
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6 //r2 == 20000
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4 //r1 == 4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4 //r1 == 4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6 //r6 == 40000
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r3, r3, 12
    addi r7, r7, 4

    maclhwu r1, r1,r4
    maclhw r5,r2,r5
    stwu    r1, 4(r8)
    stwu    r5, 4(r7) //
    isync

    addi r8, r8, 12
    addi r7, r7, 4

    maclhwu r2, r1,r6
    maclhw r5,r2,r5
//    stwu    r2, 4(r3)
    stwu    r5, 4(r7) //
    isync

    addi r7, r7, 4

    //to initialize 4(r3) memory word
    load_const r2, 0x00
    stw r2, 4(r3)

    mtspr SPR_VRSAVE, r7

    clear_regs_with_0



    rumboot_putstring "Checking XORI MTFSPR ...\n"
//    test_event EVENT_GPRS_CLEARED_START_CHECK_XORI_MTFSPR
    msync
    isync
    .align 4
    //iterations 0-7
    xori_reg_spr r0, 0x0001, SPR_SPRG0
    xori_reg_spr r1, 0x0002, SPR_SPRG1
    xori_reg_spr r0, 0x0004, SPR_SPRG2
    xori_reg_spr r2, 0x0008, SPR_SPRG3
    xori_reg_spr r0, 0x0010, SPR_SPRG4
    xori_reg_spr r4, 0x0020, SPR_SPRG5
    xori_reg_spr r0, 0x0040, SPR_SPRG6
    xori_reg_spr r8, 0x0080, SPR_SPRG7


    mfspr r31, SPR_VRSAVE

    //iterations 8-15
    mfspr_reg r0, SPR_SPRG7
    mfspr_reg r1, SPR_SPRG6

    stw r0, 0(r31)
    stw r1, 4(r31)

    mfspr_reg r0, SPR_SPRG5
    mfspr_reg r2, SPR_SPRG4

    stw r0, 8(r31)
    stw r2, 12(r31)

    mfspr_reg r0, SPR_SPRG3
    mfspr_reg r4, SPR_SPRG2

    stw r0, 16(r31)
    stw r4, 20(r31)

    mfspr_reg r0, SPR_SPRG1
    mfspr_reg r8, SPR_SPRG0

    stw r0, 24(r31)
    stw r8, 28(r31)


    mr r1, r31


    //iterations 16-23
    xori_reg_spr r0,  0x0100, SPR_SPRG0
    xori_reg_spr r16, 0x0200, SPR_SPRG1
    xori_reg_spr r0,  0x0400, SPR_SPRG2
    xori_reg_spr r31, 0x0800, SPR_SPRG3
    xori_reg_spr r30, 0x1000, SPR_SPRG4
    xori_reg_spr r31, 0x2000, SPR_SPRG5
    xori_reg_spr r29, 0x4000, SPR_SPRG6
    xori_reg_spr r31, 0x8000, SPR_SPRG7

    //iterations 24-31
    mfspr_reg r0,  SPR_SPRG7
    mfspr_reg r16, SPR_SPRG6

    stw r0, 32(r1)
    stw r16, 36(r1)

    mfspr_reg r0,  SPR_SPRG5
    mfspr_reg r31, SPR_SPRG4

    stw r0, 40(r1)
    stw r31, 44(r1)

    mfspr_reg r30, SPR_SPRG3
    mfspr_reg r31, SPR_SPRG2

    stw r30, 48(r1)
    stw r31, 52(r1)

    mfspr_reg r29, SPR_SPRG1
    mfspr_reg r31, SPR_SPRG0

    stw r29, 56(r1)
    stw r31, 60(r1)


    mr r3, r1


    //iterations 32-39
    xoris_reg_spr r27, 0x0001, SPR_SPRG0
    xoris_reg_spr r31, 0x0002, SPR_SPRG1
    xoris_reg_spr r23, 0x0004, SPR_SPRG2
    xoris_reg_spr r31, 0x0008, SPR_SPRG3
    xoris_reg_spr r15, 0x0010, SPR_SPRG4
    xoris_reg_spr r31, 0x0020, SPR_SPRG5
    xoris_reg_spr r1, 0x0040, SPR_SPRG6
    xoris_reg_spr r2, 0x0080, SPR_SPRG7

    //iterations 40-47
    mfspr_reg r27, SPR_SPRG7
    mfspr_reg r31, SPR_SPRG6

    stw r27, 64(r3)
    stw r31, 68(r3)

    mfspr_reg r23, SPR_SPRG5
    mfspr_reg r31, SPR_SPRG4

    stw r23, 72(r3)
    stw r31, 76(r3)

    mfspr_reg r15, SPR_SPRG3
    mfspr_reg r31, SPR_SPRG2

    stw r15, 80(r3)
    stw r31, 84(r3)

    mfspr_reg r1 , SPR_SPRG1
    mfspr_reg r2 , SPR_SPRG0

    stw r1, 88(r3)
    stw r2, 92(r3)


    //iterations 48-55
    xoris_reg_spr r4,  0x0100, SPR_SPRG0
    xoris_reg_spr r8,  0x0200, SPR_SPRG1
    xoris_reg_spr r16, 0x0400, SPR_SPRG2
    xoris_reg_spr r30, 0x0800, SPR_SPRG3
    xoris_reg_spr r29, 0x1000, SPR_SPRG4
    xoris_reg_spr r27, 0x2000, SPR_SPRG5
    xoris_reg_spr r23, 0x4000, SPR_SPRG6
    xoris_reg_spr r15, 0x8000, SPR_SPRG7

    //iterations 56-63
    mfspr_reg r4 , SPR_SPRG7
    mfspr_reg r8 , SPR_SPRG6
    mfspr_reg r16, SPR_SPRG5
    mfspr_reg r30, SPR_SPRG4
    mfspr_reg r29, SPR_SPRG3
    mfspr_reg r27, SPR_SPRG2
    mfspr_reg r23, SPR_SPRG1
    mfspr_reg r15, SPR_SPRG0

    stw r4, 96(r3)
    stw r8, 100(r3)
    stw r16, 104(r3)
    stw r30, 108(r3)
    stw r29, 112(r3)
    stw r27, 116(r3)
    stw r23, 120(r3)
    stw r15, 124(r3)


    mtspr SPR_VRSAVE, r3

    clear_regs_with_1

    mfspr r3, SPR_VRSAVE


    //iterations 0-7
    xori_reg_spr r0, 0x0001, SPR_SPRG0
    xori_reg_spr r1, 0x0002, SPR_SPRG1
    xori_reg_spr r0, 0x0004, SPR_SPRG2
    xori_reg_spr r2, 0x0008, SPR_SPRG3
    xori_reg_spr r0, 0x0010, SPR_SPRG4
    xori_reg_spr r4, 0x0020, SPR_SPRG5
    xori_reg_spr r0, 0x0040, SPR_SPRG6
    xori_reg_spr r8, 0x0080, SPR_SPRG7

    //iterations 8-15
    mfspr_reg r0, SPR_SPRG7
    mfspr_reg r1, SPR_SPRG6

    stw r0, 128(r3)
    stw r1, 132(r3)

    mfspr_reg r0, SPR_SPRG5
    mfspr_reg r2, SPR_SPRG4

    stw r0, 136(r3)
    stw r2, 140(r3)

    mfspr_reg r0, SPR_SPRG3
    mfspr_reg r4, SPR_SPRG2

    stw r0, 144(r3)
    stw r4, 148(r3)

    mfspr_reg r0, SPR_SPRG1
    mfspr_reg r8, SPR_SPRG0

    stw r0, 152(r3)
    stw r8, 156(r3)


    //iterations 16-23
    xori_reg_spr r0 , 0x0100, SPR_SPRG0
    xori_reg_spr r16, 0x0200, SPR_SPRG1
    xori_reg_spr r0 , 0x0400, SPR_SPRG2
    xori_reg_spr r31, 0x0800, SPR_SPRG3
    xori_reg_spr r30, 0x1000, SPR_SPRG4
    xori_reg_spr r31, 0x2000, SPR_SPRG5
    xori_reg_spr r29, 0x4000, SPR_SPRG6
    xori_reg_spr r31, 0x8000, SPR_SPRG7

    //iterations 24-31
    mfspr_reg r0 , SPR_SPRG7
    mfspr_reg r16, SPR_SPRG6

    stw r0, 160(r3)
    stw r16, 164(r3)

    mfspr_reg r0 , SPR_SPRG5
    mfspr_reg r31, SPR_SPRG4

    stw r0, 168(r3)
    stw r31, 172(r3)

    mfspr_reg r30, SPR_SPRG3
    mfspr_reg r31, SPR_SPRG2

    stw r30, 176(r3)
    stw r31, 180(r3)

    mfspr_reg r29, SPR_SPRG1
    mfspr_reg r31, SPR_SPRG0

    stw r29, 184(r3)
    stw r31, 188(r3)


    //iterations 32-39
    xoris_reg_spr r27, 0x0001, SPR_SPRG0
    xoris_reg_spr r31, 0x0002, SPR_SPRG1
    xoris_reg_spr r23, 0x0004, SPR_SPRG2
    xoris_reg_spr r31, 0x0008, SPR_SPRG3
    xoris_reg_spr r15, 0x0010, SPR_SPRG4
    xoris_reg_spr r31, 0x0020, SPR_SPRG5
    xoris_reg_spr r1 , 0x0040, SPR_SPRG6
    xoris_reg_spr r2 , 0x0080, SPR_SPRG7

    //iterations 40-47
    mfspr_reg r27, SPR_SPRG7
    mfspr_reg r31, SPR_SPRG6

    stw r27, 192(r3)
    stw r31, 196(r3)

    mfspr_reg r23, SPR_SPRG5
    mfspr_reg r31, SPR_SPRG4

    stw r23, 200(r3)
    stw r31, 204(r3)

    mfspr_reg r15, SPR_SPRG3
    mfspr_reg r31, SPR_SPRG2

    stw r15, 208(r3)
    stw r31, 212(r3)

    mfspr_reg r1 , SPR_SPRG1
    mfspr_reg r2 , SPR_SPRG0

    stw r1, 216(r3)
    stw r2, 220(r3)


    //iterations 48-55
    xoris_reg_spr r4, 0x0100, SPR_SPRG0
    xoris_reg_spr r8, 0x0200, SPR_SPRG1
    xoris_reg_spr r16, 0x0400, SPR_SPRG2
    xoris_reg_spr r30, 0x0800, SPR_SPRG3
    xoris_reg_spr r29, 0x1000, SPR_SPRG4
    xoris_reg_spr r27, 0x2000, SPR_SPRG5
    xoris_reg_spr r23, 0x4000, SPR_SPRG6
    xoris_reg_spr r15, 0x8000, SPR_SPRG7

    //iterations 56-63
    mfspr_reg r4 , SPR_SPRG7
    mfspr_reg r8 , SPR_SPRG6
    mfspr_reg r16, SPR_SPRG5
    mfspr_reg r30, SPR_SPRG4
    mfspr_reg r29, SPR_SPRG3
    mfspr_reg r27, SPR_SPRG2
    mfspr_reg r23, SPR_SPRG1
    mfspr_reg r15, SPR_SPRG0

    stw r4, 224(r3)
    stw r8, 228(r3)
    stw r16, 232(r3)
    stw r30, 236(r3)
    stw r29, 240(r3)
    stw r27, 244(r3)
    stw r23, 248(r3)
    stw r15, 252(r3)


    clear_regs_with_0

    msync

    gtube_init

    mfctr       r1  // restore stack ptr

    load_const  r3, EVENT_TESTEVENT
    load_addr   r4, event_data
    load_const  r5, EVENT_DATA_SIZE
    bl          rumboot_platform_event_raise


test_ok:
    rumboot_putstring "TEST OK\n"
    load_const  r3, RESULT_OK
    b finish

test_error:
    gtube_init
    rumboot_putstring "TEST ERROR\n"
    load_const  r3, RESULT_ERROR

finish:
    full_epilogue
    blr
