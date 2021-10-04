#include <platform/test_event_asm.h>
//#include <platform/test_assert.S.h>
//#include <platform/arch/ppc/test_macro_asm.S.h>
//#include <platform/ppc470s/mmu.S.h>
#include <platform/devices.h>


#define ITERATION_COUNT     0X8
#define ITERATION_COUNT_3   0x3


//tlb_entries:
/*          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
//    .long   MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )


.global test_oi10_power_1_7

.section ".text.test_oi10_power_1_7","ax",@progbits

test_oi10_power_1_7:

//stack
    stwu    r1,-160(r1)
    stw     r0,156(r1)
    stmw    r2,36(r1)
    mflr    r31
    mfctr   r30
    mfcr    r29
    mfxer   r28
    stmw    r28,20(r1)
    
    
    //rumboot_putstring "test_oi10_power_1_7: turn on cache\n"
    //load_addr   r3, tlb_entries
    load_size   r4, 1
    //bl          ppc470s_write_tlb_entries /*(r3, r4) - are parameters*/


#define L1C_LINE_BYTES_N    0x20
test_oi10_power_7:
    //rumboot_putstring "test_oi10_power_1_7 started\n"

fpu_enable:
    mfmsr r0
    ori r0, r0, reg_field(50, 0b1) | reg_field(52, 0b1) | reg_field(55, 0b1)
    mtmsr r0

begin:
//    rumboot_putstring "Caching begin\n"
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
    load_addr  r4, C_matrix
//.rept 5
    isync
.rept 7
    dcbt 0, r4, 0
    addi  r4, r4, L1C_LINE_BYTES_N
.endr
    msync
    isync
//    rumboot_putstring "Caching done\n"
    
    //8x8 matrix multiplication: A*B=C
    //rumboot_putstring "Start max power 1_7 (Matrix multiplication)\n"
    

    load_addr r30, A_matrix //A matrix pointer
    load_addr r28, B_matrix //B matrix pointer
    load_addr r26, C_matrix //C matrix pointer
    
    load_const r29, 0x4 // update index
    load_const r27, 0x4 // update index
    load_const r25, 0x4 // update index    

    
    // load A00, B00..07
    lfsux f0,  r30, r29 // A00
    lfsux f8,  r28, r27 // B00
    lfsux f9,  r28, r27 // B01
    lfsux f10, r28, r27 // B02
    lfsux f11, r28, r27 // B03
    lfsux f12, r28, r27 // B04
    lfsux f13, r28, r27 // B05
    lfsux f14, r28, r27 // B06
    lfsux f15, r28, r27 // B07
    
    
    load_const  r31, ITERATION_COUNT // strings num - x
    mtctr       r31   

.align 5
loop: 
 load_const  r21, ITERATION_COUNT_3 
 load_const  r20, 0x0

metka: // columns num - y
    lfsux f1, r30, r29       // load Ax(y+1)
     fmadd f24, f0,  f8, f24 // f24 = Axy*By0 + f24
    lfsux f16,  r28, r27      // load B(y+1)0
     fmadd f25, f0,  f9, f25 // f25 = Axy*By1 + f25
    lfsux f17,  r28, r27      // load B(y+1)1
     fmadd f26, f0, f10, f26 // f26 = Axy*By2 + f26
    lfsux f18, r28, r27      // load B(y+1)2
     fmadd f27, f0, f11, f27 // f27 = Axy*By3 + f27
    lfsux f19, r28, r27      // load B(y+1)3
     fmadd f28, f0, f12, f28 // f28 = Axy*By4 + f28
    lfsux f20, r28, r27      // load B(y+1)4
     fmadd f29, f0, f13, f29 // f29 = Axy*By5 + f29
    lfsux f21, r28, r27      // load B(y+1)5
     fmadd f30, f0, f14, f30 // f30 = Axy*By6 + f30
    lfsux f22, r28, r27      // load B(y+1)6
     fmadd f31, f0, f15, f31 // f31 = Axy*By7 + f31
    lfsux f23, r28, r27      // load B(y+1)7    

    lfsux f0, r30, r29       // load Ax(y+1)
     fmadd f24, f1,  f16, f24 // f24 = Axy*By0 + f24
    lfsux f8,  r28, r27      // load B(y+1)0
     fmadd f25, f1,  f17, f25 // f25 = Axy*By1 + f25
    lfsux f9,  r28, r27      // load B(y+1)1
     fmadd f26, f1, f18, f26 // f26 = Axy*By2 + f26
    lfsux f10, r28, r27      // load B(y+1)2
     fmadd f27, f1, f19, f27 // f27 = Axy*By3 + f27
    lfsux f11, r28, r27      // load B(y+1)3
     fmadd f28, f1, f20, f28 // f28 = Axy*By4 + f28
    lfsux f12, r28, r27      // load B(y+1)4
     fmadd f29, f1, f21, f29 // f29 = Axy*By5 + f29
    lfsux f13, r28, r27      // load B(y+1)5
     fmadd f30, f1, f22, f30 // f30 = Axy*By6 + f30
    lfsux f14, r28, r27      // load B(y+1)6
     fmadd f31, f1, f23, f31 // f31 = Axy*By7 + f31
    lfsux f15, r28, r27      // load B(y+1)7     
    
    subi r21, r21, 0x1
    cmpw cr5, r21, r20
    bgt  cr5, metka


     fmadd f24, f0,  f8, f24 // f24 = Axy*By0 + f24
     fmadd f25, f0,  f9, f25 // f25 = Axy*By1 + f25
     fmadd f26, f0, f10, f26 // f26 = Axy*By2 + f26
     fmadd f27, f0, f11, f27 // f27 = Axy*By3 + f27
     fmadd f28, f0, f12, f28 // f28 = Axy*By4 + f28
     fmadd f29, f0, f13, f29 // f29 = Axy*By5 + f29
     fmadd f30, f0, f14, f30 // f30 = Axy*By6 + f30
     fmadd f31, f0, f15, f31 // f31 = Axy*By7 + f31
     
     lfsux f0, r30, r29       // load Ax(y+1)

   
    stfsux f24, r26, r25     // store Cx0
    stfsux f25, r26, r25     // store Cx1
    stfsux f26, r26, r25     // store Cx2
    stfsux f27, r26, r25     // store Cx3
    stfsux f28, r26, r25     // store Cx4
    stfsux f29, r26, r25     // store Cx5
    stfsux f30, r26, r25     // store Cx6
    stfsux f31, r26, r25     // store Cx7    

    bdnz+       loop
    
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
    


.align 5
A_matrix:
//non_used data
.long 0x00000000
//Matrix A
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
.long 0xb1e58743
.long 0x2e614020
.long 0x6f55fe7f
.long 0x4dd10e07
.long 0x390c389c
.long 0xbe73150a
.long 0xb6986db6
.long 0x5c15582c
.long 0x58743b1e
.long 0x140202e6
.long 0x5fe7f6f5
.long 0x10e074dd
.long 0xc389c390
.long 0x3150abe7
.long 0x86db6b69
.long 0x5582c5c1
.long 0x743b1e58
.long 0x0202e614
.long 0xe7f6f55f
.long 0xe074dd10
.long 0x89c390c3
.long 0x50abe731
.long 0xdb6b6986
.long 0x82c5c155
.long 0x6a18944f
.long 0xff3bb5df
.long 0x34b6a357
.long 0x192e1f0e
.long 0x598d6eb0
.long 0xaad26135
.long 0xfc88ac51
.long 0x5fd7d3e8

.align 5
B_matrix:
//non_used data
.long 0x00000000
//Matrix B
.long 0x43e587b1
.long 0x2061402e
.long 0x7f55fe6f
.long 0x07d10e4d
.long 0x9c0c3839
.long 0x0a7315be
.long 0xb6986db6
.long 0x2c15585c
.long 0x44a189f6
.long 0x5df3bbff
.long 0x354b6a73
.long 0xf092e1e1
.long 0xeb98d605
.long 0x13ad265a
.long 0xc5c88a1f
.long 0x3efd7d85
.long 0xd9b2b266
.long 0xff2efff1
.long 0x5f2fbf82
.long 0x44b98859
.long 0x016b03bc
.long 0x5fc3be2c
.long 0x3c5979c8
.long 0x2ff75e41
.long 0x26fd4dc6
.long 0x24754809
.long 0xc9e09263
.long 0x37a36e24
.long 0xa46449a0
.long 0x5d13ba22
.long 0x5f27bfa6
.long 0x2c6559d4
.long 0xb18743e5
.long 0x2e402061
.long 0x6ffe7f55
.long 0x4d0e07d1
.long 0x39389c0c
.long 0xbe150a73
.long 0xb66db698
.long 0x5c582c15
.long 0x583b1e74
.long 0x1402e602
.long 0x5ff6f5e7
.long 0x1074dde0
.long 0xc3c39089
.long 0x31abe750
.long 0x866b69db
.long 0x55c5c182
.long 0x741e583b
.long 0x02e61402
.long 0xe7f55ff6
.long 0xe0dd1074
.long 0x8990c3c3
.long 0x50e731ab
.long 0xdb69866b
.long 0x82c155c5
.long 0x6a944f18
.long 0xffb5df3b
.long 0x34a357b6
.long 0x191f0e2e
.long 0x596eb08d
.long 0xaa6135d2
.long 0xfcac5188
.long 0x5fd3e8d7

.space 520, 0x0

.align 7
C_matrix:

.space 256, 0x0

