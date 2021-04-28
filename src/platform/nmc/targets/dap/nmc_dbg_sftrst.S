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

    // Check whether NMC was in Debug
    ar0 = GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_BVR1);
    gr0 = [ar0];
    gr1 = 0000DEB0h; // Value which indicates that NMC was in Debug
    gr0 = gr0-gr1;
    if =0 goto Finish;
    
    // Writing BKPT by 0 addr
    gr0 = 0;
    [NMC_DBG_BVR0] = gr0;

    // Enable BKPT
    gr0 = 00010000h;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSCRset)] = gr0;

    // NMC to Debug
    gr0 = 1;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DRCR)] = gr0;

Loop:
    goto Loop;

Finish:
    gr7 = 0;
    pop ar6,gr6;
    pop ar5,gr5;
    pop ar4,gr4;
    pop ar3,gr3;
    pop ar2,gr2;
    pop ar1,gr1;
    pop ar0,gr0;
    return;
