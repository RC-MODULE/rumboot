#include <platform/nmc/platform/devices.h>
#include <regs/regs_nmc_dbg.h>

.global _dbg_test
.global __main

_dbg_test:
    ar5 = ar7 - 2;
    push ar0,gr0;
    push ar1,gr1;
    push ar2,gr2;
    push ar3,gr3;
    push ar4,gr4;
    push ar6,gr6;   // Will be modified in Debug
    gr0 = DBG_EXIT; // Addr for debug restart. gr0 will be used in Debug
    ar0 = [--ar5];  // data_orig. Will be used and modified in Debug
    ar1 = [--ar5];  // data_copy. Will be used and modified in Debug
    gr1 = [--ar5];  // data_num. Will be used in Debug

    ar3 = ar0; // Backup ar0
    ar4 = ar1; // Backup ar1

    gr7 = 1ACCE551h;
    ar2 = GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_LAR);
    [ar2] = gr7; // Unlock Debug Register
    
    gr7 = 1;
    ar2 = GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DRCR);
    gr2 = pswr;
    [ar2] = gr7; // Entering debug

LOOP_INF:
    goto LOOP_INF;
    
DBG_EXIT:
    pswr = gr2;

    gr3 = [ar3++];
    gr4 = [ar4++];
   
    gr7 = gr3 xor gr4;
    if <>0 goto BAD_CASE;
    gr1--;
    if <>0 goto DBG_EXIT;

GOOD_CASE:
    gr7 = 0000000Fh;
    goto END_OF_FUNC;

BAD_CASE:
    gr7 = 00000001h;

END_OF_FUNC:
    pop ar6,gr6;
    pop ar4,gr4;
    pop ar3,gr3;
    pop ar2,gr2;
    pop ar1,gr1;
    pop ar0,gr0;
    return;

.align 3
data_orig:
.long 055555555h, 046B94BFBh, 01C667E81h, 0FF6ABDDFh, 09E2CFB54h, 0487201BFh, 037CF1CE1h, 037200F47h, 02313DE56h, 0359DAA88h, 0C6756687h, 06561137Bh, 073CF59EAh, 09D853C54h, 01D2D85A1h, 0AAEE655Bh

.align 3
data_copy:
.skip 64

__main:
    // Loops
    gr2 = 4;
    // Loop number in which clearing data_copy array will be skipped (array on next loop will compared by BST and will not write in data_copy)
    gr3 = 2;
NEXT_CHECK:
    ar7 += 4;
    ar1 = ar7 - 1;

    ar0 = data_orig;
    [ar1] = ar0;
    ar1--;
    ar0 = data_copy;
    [ar1] = ar0;
    ar1--;
    gr4 = 11; // Amount of words to be copied from data_orig to data_copy
    [ar1] = gr4;
    call _dbg_test;
    ar1--;
    ar7 = ar1;
    gr0 = 0000000Fh;
    gr7 = gr7-gr0;
    if <>0 goto FAIL;
    
    ar4 = data_copy;
    ar2 = ar4+gr4;
    gr5 = [ar2];
    if <>0 goto FAIL_EXTRA_COPYING;

    gr3--;
    if =0 goto PRE_NEXT_CHECK;
    // Clear data_copy
    gr0 = 0;
    gr1 = gr4;
    ar0 = data_copy;
CLEAR_DATA_COPY:
    [ar0++] = gr0;
    gr1--;
    if <>0 goto CLEAR_DATA_COPY;

PRE_NEXT_CHECK:
    gr2--;
    if <>0 goto NEXT_CHECK;

PASS:
    gr7 = 0;
    return;
FAIL:
    gr7 = 1;
    return;
FAIL_EXTRA_COPYING:
    gr7 = 2;
    return;
