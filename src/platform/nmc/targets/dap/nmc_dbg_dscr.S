#include <platform/nmc/platform/devices.h>
#include <regs/regs_nmc_dbg.h>

.global __main

__main:
    ar5 = ar7 - 2;
    push ar0,gr0;
    push ar1,gr1;
    push ar2,gr2;
    push ar3,gr3;
    push ar4,gr4;
    push ar5,gr5;
    push ar6,gr6;
    // Unlock NMCDBG
    gr0 = 1ACCE551h;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_LAR)] = gr0;

    ar2 = GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_BVR0); // Address of even BVR
    ar3 = GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_BVR1); // Address of odd BVR
    
    gr0 = 00020000h; // Init set value for enable BCRx (0)
    gr1 = 00010000h; // Init reset value for disanle BCRx (1)
    
    goto SKIP_REINIT;

LOOP:
    // Clear old BVRs
    gr2 = 0;
    [ar2] = gr2;
    [ar3] = gr2;

    // Check last loop
    gr2 = ar3;
    gr3 = 0FFh;
    gr2 = gr2 and gr3;
    gr3 = 04Eh;
    gr2 = gr2-gr3;
    if =0 goto PASS;

    // Change ARs to next BVR addresses
    ar2 = ar2+4;
    ar3 = ar3+4;

SKIP_REINIT:
    // Write BVR(2n) and BVR(2n+1)
    ar0 = false_breakpoint;
    [ar2] = ar0;
    ar0 = true_breakpoint;
    [ar3] = ar0;

    gr4 = 2; // Downcounter of Debug entries
    gr6 = 0; // This value will be changed in Debug to 00DEB110h value
    ar0 = DBG_EXIT; // Return Debug address

    // Set BCR(2n+1) and reset BCR(2n)
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSCRset)] = gr0;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSCRreset)] = gr1;

    // Wait writing
wait_wr_dscr_1:
    gr2 = [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSCR)];
    gr3 = gr0 or gr1;
    gr2 = gr2 and gr3;
    gr3 = gr0;
    gr2-gr3;
    if <>0 goto wait_wr_dscr_1;

start_test:
    gr7 = 90ABCDEFh;
false_breakpoint:
    gr7 = 6F543210h;
true_breakpoint:
    gr7 = 0000DEADh;

DBG_EXIT:
    // Check whether NMC was in Debug
    gr5 = 00DEB110h;

    gr6-gr5;
    if =0 goto NEXT_CHECK;
    gr5 = 0000DEADh;
    gr6-gr5;
    if =0 goto ITREN_ERROR;
    goto DBGIN_ERROR;

NEXT_CHECK:
    // Check which breakpoint caused Debug mode. If gr7==90ABCDEFh then false_breakpoint
    gr5 = 6F543210h;
    gr7-gr5;
    if <>0 goto BCR_ERROR;

    gr4--;
    if =0 goto LOOP;

    // Write BVR(2n) and BVR(2n+1) (vice versa)
    ar0 = false_breakpoint;
    [ar3] = ar0;
    ar0 = true_breakpoint;
    [ar2] = ar0;

    ar0 = DBG_EXIT; // Return Debug address
    gr6 = 0; // This value will be changed in Debug to 00DEB110h value

    // Set BCR(2n) and reset BCR(2n+1) (vice versa)
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSCRset)] = gr1;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSCRreset)] = gr0;

    // Wait writing
wait_wr_dscr_2:
    gr2 = [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSCR)];
    gr3 = gr0 or gr1;
    gr2 = gr2 and gr3;
    gr3 = gr1;
    gr2-gr3;
    if <>0 goto wait_wr_dscr_2;

    // Set BCR set and reset for next loop
    gr0 = gr0<<2;
    gr1 = gr1<<2;

    goto start_test;

PASS:
    gr7 = 0;
    goto FINISH;
    
ITREN_ERROR:
    gr7 = 1;
    goto FINISH;

DBGIN_ERROR:
    gr7 = 2;
    goto FINISH;

BCR_ERROR:
    gr7 = 3;
    goto FINISH;

FINISH:
    pop ar6,gr6;
    pop ar5,gr5;
    pop ar4,gr4;
    pop ar3,gr3;
    pop ar2,gr2;
    pop ar1,gr1;
    pop ar0,gr0;
    return;
