#include <platform/nmc/platform/devices.h>
#include <regs/regs_nmc_dbg.h>

#define IB_goto 0
#define IB_delayed 2

// Shared with BST
#define BKPT_ARRAY 00010000h
#define BKPT_ARRAY_PTR 000100FFh

.global __main

__main:
    // Unlock NMCDBG
    gr0 = 1ACCE551h;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_LAR)] = gr0;
    
    // Writing BKPT
    ar0 = bkpt_0;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_BVR0)] = ar0;

    // Enable BKPT and DCC Mode
    gr0 = 00010000h;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSCRset)] = gr0;

    ar0 = Success;
    gr0 = 0;
    gr1 = 0;
// Store by NEXT_BKPT_ADDR addr addresses of breakpoints
    [GET_NMC_ADDR(DBG_BASE_DEC,BKPT_ARRAY_PTR)] = gr0;
    ar1 = BKPT_ARRAY;
////////////////////
//  gr2 = IB_goto; 
    gr2 = IB_delayed;
////////////////////
    gr5 = gr2;
    // 1st goto
    gr2 = bkpt_1;
    gr2 = gr2 + gr5;
    [ar1++] = gr2;
    
    // 2nd goto
    gr2 = bkpt_2;
    gr2 = gr2 + gr5;
    [ar1++] = gr2;
    
    // 3rd goto
    gr2 = bkpt_3;
    gr2 = gr2 + gr5;
    [ar1++] = gr2;
    
    // 4th goto 
    gr2 = bkpt_4;
    gr2 = gr2 + gr5;
    [ar1++] = gr2;
    
    // 5th goto
    gr2 = bkpt_5;
    gr2 = gr2 + gr5;
    [ar1++] = gr2;
    
    // 6th goto 
    gr2 = bkpt_6;
    gr2 = gr2 + gr5;
    [ar1++] = gr2;

    // Last BVR value write
    gr2 = bkpt_1;
    [ar1++] = gr2;
    [ar1++] = gr2;
    [ar1++] = gr2;
    gr5 = 0;

    goto test_start;
test_start:
    gr4 = 0; // gr4 will eq. 0 if NMC will no in Debug state (it is DBG entry counter)
    // Loop for first dbg state
bkpt_0:
    .rept 10;
        gr7 = gr7; // Align address
    .endr;

label_1: // Long goto by even addr
    ar6 = label_2;
    gr7 = 0; // Amount of not nop delayed command
    .align 3
    gr7 = gr7; 
    gr7 = gr7;
bkpt_1:
    goto Success;
    gr0++; // This command will be executed if error

label_2: // Short goto by even addr
    ar6 = label_3;
    gr7 = 0;
    .align 3
    gr7 = gr7;
    gr7 = gr7;
bkpt_2:
    goto ar0;
    gr0++;

label_3: // Short goto by odd addr
    ar6 = label_4;
    gr7 = 0;
bkpt_3:
    .align 3
    gr7 = gr7;
    goto ar0;
    gr0++;

label_4: // Delayed long goto by even addr
    ar6 = label_5;
    gr7 = 2;
    .align 3
    gr7 = gr7;
    gr7 = gr7;
bkpt_4:
    delayed goto Success;
    gr1++; 
    gr1++;

    gr1++; // This command will be executed if error

label_5: // Delayed short goto by even addr
    ar6 = label_6;
    gr7 = 3;
    .align 3
    gr7 = gr7;
    gr7 = gr7;
bkpt_5:
    delayed goto ar0;
    gr1++;
    gr1++;
    gr1++;

    gr1++; // This command will be executed if error

label_6: // Delayed short goto by odd addr
    ar6 = label_7;
    gr7 = 2;
bkpt_6:
    .align 3
    gr7 = gr7;
    delayed goto ar0;
    gr1++;
    gr1++;

    gr1++; // This command will be executed if error

label_7: // New breakpoint by return address
    ar6 = bkpt1_1;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_BVR0)] = ar6;
    ar6 = bkpt2_1;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_BVR1)] = ar6;
    gr7 = 00030000h;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSCRset)] = gr7; // Set both BVR enable
// Wait for writing
loopBrpEn:
    gr7 = [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSCRset)];
    gr3 = 00030000h;
    gr7 = gr7 and gr3;
    gr7-gr3;
    if <>0 goto loopBrpEn;

    ar6 = Normal;
    gr7 = 4;
bkpt1_1:
    .align 3
    gr1++;
    gr1++;
bkpt2_1:
    .align 3
    gr1++;
    gr1++;

    gr7 = gr7;
    gr7 = gr7;

    goto Success;

    gr7 = gr7;
    gr7 = gr7;
    gr5 = 5;
Success:
    gr0 = 511CCE55h;
    gr0++; // ...56h
    gr0++; // ...57h
    gr0++; // ...58h
    gr0++; // ...59h
    gr0++; // ...5Ah
    gr0++; // ...5Bh
    gr0++; // ...5Ch
    gr0 = gr0 - gr5;
    gr6 = 511CCE5Ch;
    gr6 - gr0;
    if <>0 goto Error;

    gr7 - gr1;
    if <>0 goto Error; // Checking number of executed instructions after "delayed goto". This number should be equal gr7!

    gr0 = 0;
    gr1 = 0;
    gr2 = 0;
    gr5 = 0;
    
    goto ar6;

Normal:
    gr7 = 9; // Expected amount of DBG entries
    gr4 = gr4 - gr7;
    if <>0 goto WrongEnt;
    gr7 = 0;
    goto Finish;
    
Error:
    gr7 = 1;
    goto Finish;

WrongEnt:
    gr7 = 2;
    goto Finish;

Finish:
    return;
