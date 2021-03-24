#include <platform/nmc/platform/devices.h>
#include <regs/regs_nmc_dbg.h>

.global __main

__main:
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
    return;
