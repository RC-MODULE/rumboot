#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <arch/mmu.S.h>
#include <platform/devices.h>


.macro init_fpr gpr_data_pointer
    load_addr  \gpr_data_pointer, power_3_fp_data //data location


    lfdu    f0, 8(\gpr_data_pointer)
    lfdu    f1, 8(\gpr_data_pointer)
    lfdu    f2, 8(\gpr_data_pointer)
    lfdu    f3, 8(\gpr_data_pointer)
    lfdu    f4, 8(\gpr_data_pointer)
    lfdu    f5, 8(\gpr_data_pointer)
    lfdu    f6, 8(\gpr_data_pointer)
    lfdu    f7, 8(\gpr_data_pointer)
    lfdu    f8, 8(\gpr_data_pointer)
    lfdu    f9, 8(\gpr_data_pointer)
    lfdu    f10, 8(\gpr_data_pointer)
    lfdu    f11, 8(\gpr_data_pointer)
    lfdu    f12, 8(\gpr_data_pointer)
    lfdu    f13, 8(\gpr_data_pointer)
    lfdu    f14, 8(\gpr_data_pointer)
    lfdu    f15, 8(\gpr_data_pointer)
    lfdu    f16, 8(\gpr_data_pointer)
    lfdu    f17, 8(\gpr_data_pointer)
    lfdu    f18, 8(\gpr_data_pointer)
    lfdu    f19, 8(\gpr_data_pointer)
    lfdu    f20, 8(\gpr_data_pointer)
    lfdu    f21, 8(\gpr_data_pointer)
    lfdu    f22, 8(\gpr_data_pointer)
    lfdu    f23, 8(\gpr_data_pointer)
    lfdu    f24, 8(\gpr_data_pointer)
    lfdu    f25, 8(\gpr_data_pointer)
    lfdu    f26, 8(\gpr_data_pointer)
    lfdu    f27, 8(\gpr_data_pointer)
    lfdu    f28, 8(\gpr_data_pointer)
    lfdu    f29, 8(\gpr_data_pointer)
    lfdu    f30, 8(\gpr_data_pointer)
    lfdu    f31, 8(\gpr_data_pointer)
.endm


tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
    .long   MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )


.global test_oi10_power_endless

.section ".cached_text","ax",@progbits

test_oi10_power_endless:
    rumboot_putstring "test_oi10_power_3: turn on cache\n"
    stwu r1, -32(r1)
    mflr r0
    stw r0, 36(r1)
    stmw r28, 16(r1)
    mfctr r0
    stw r0, 12(r1)
    load_addr   r3, tlb_entries
    load_size   r4, 1
    bl  ppc470s_write_tlb_entries/*(r3, r4) - are parameters*/

#define L1C_LINE_BYTES_N    0x20
test_oi10_power_3:
    rumboot_putstring "test_oi10_power_3 started\n"

fpu_enable:
    mfmsr r0
    ori r0, r0, reg_field(50, 0b1) | reg_field(52, 0b1) | reg_field(55, 0b1)
    mtmsr r0

begin:
    rumboot_putstring "Caching begin\n"
//  Caching code: 32+5 words = 148 bytes = 5 lines +1
//  Caching code: 64+5 words = 276 bytes = 9 lines +1
    load_addr  r4, loop
    isync
.rept 10
    icbt 0, 0, r4
    addi  r4, r4, L1C_LINE_BYTES_N
.endr
    msync
    isync
//  Caching store data location: 16+1 doublewords = 136  bytes = 5 lines
//  Caching store data location: 32+1 doublewords = 264  bytes = 9 lines
    load_addr  r4, power_3_store_fp_data
//.rept 5
    isync
.rept 7
    dcbt 0, r4, 0
    addi  r4, r4, L1C_LINE_BYTES_N
.endr
    msync
    isync
    rumboot_putstring "Caching done\n"

    init_fpr r30

//    load_const  r31, CPU_ITERATIONS_COUNT
//    mtctr       r31

    load_const r29, 0x8 // update index

.align 7
loop:
    fmadd  f9, f1,  f3, f5
    load_addr r8, power_3_store_fp_data 
    fmadd  f10, f0,  f3, f5
    load_addr r9, power_3_store_fp_data_2
    fmadd  f11, f7,  f2, f4
    load_addr r10, power_3_store_fp_data 
    fmadd  f12, f0,  f0, f2

    b       loop
    
    rumboot_putstring "CPU test done\n"
    lwz r0, 12(r1)
    mtctr r0
    lmw r28, 16(r1)
    lwz r0, 36(r1)
    mtlr r0
    addi r1, r1, 32
    blr

test_ok:
    rumboot_putstring "\nTEST OK\n", r15, r16
    test_event      EVENT_OK, r15, r16
    load_const  r3, RESULT_OK

finish:
    mfspr       r4, SPR_SPRG8   /* Restore */
    mtlr        r4              /*   LR    */
    blr


.align 7
power_3_fp_data:
//non-used data
.long         0x00000000
.long         0x00000000
//FPR0..15
.long 0x43b1e587
.long 0x202e6140
.long 0x7f6f55fe
.long 0x074dd10e
.long 0x9c390c38
.long 0x0abe7315
.long 0xb6b6986d
.long 0x2c5c1558
.long 0x44f6a189
.long 0x5dfff3bb
.long 0x35734b6a
.long 0xf0e192e1
.long 0xeb0598d6
.long 0x135aad26
.long 0xc51fc88a
.long 0x3e85fd7d
.long 0xd966b2b2
.long 0xfff12eff
.long 0x5f822fbf
.long 0x4459b988
.long 0x01bc6b03
.long 0x5f2cc3be
.long 0x3cc85979
.long 0x2f41f75e
.long 0x26c6fd4d
.long 0x24097548
.long 0xc963e092
.long 0x3724a36e
.long 0xa4a06449
.long 0x5d2213ba
.long 0x5fa627bf
.long 0x2cd46559

.space 128, 0x0

.align 7
power_3_store_fp_data:

.space 4, 0x0

power_3_store_fp_data_2:

.space 4, 0xffffffff

test_end:
    b test_end
