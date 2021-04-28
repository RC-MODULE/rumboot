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
    gr0 = 0FFFFFC00h; // Mask

    gr1 = [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DRAR)];
    gr1 = gr0 and gr1;

    gr2 = [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DSAR)];
    gr2 = gr0 and gr2;

    gr2 = gr2+gr1;
    gr3 = GET_NMC_ADDR(DBG_BASE_DEC,0);
    gr3 = gr3-gr2;
    if <>0 goto FAIL;

PASS:
    gr7 = 0;
    goto FINISH;

FAIL:
    gr7 = 0FFFFFFFFh;
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
