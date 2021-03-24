#include <platform/nmc/platform/devices.h>
#include <regs/regs_nmc_dbg.h>

.global __main

__main:
//------------------------------------------------------------------------------
// Section 1 - APB-MUX Check
//------------------------------------------------------------------------------
    // Check that the DAP ROM table is accessible
    // This tests System to Debug APB address translation
    ar0 = GET_NMC_ADDR(DAP_BASE_DEC,0);
    gr0 = [ar0];
    gr1 = 00001003h;
    gr1 = gr0-gr1;
    if <>0 delayed goto FAIL;
    gr7 = 1;

    // Unlock NMCDBG
    gr0 = 1ACCE551h;
    [GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_LAR)] = gr0;

    // Write then read back Debug component register on the
    // Debug APB to check buses
    ar0 = GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_BVR0);
    gr0 = 01234566h;
    [ar0] = gr0;
    gr1 = [ar0];
    gr1 = gr0-gr1;
    if <>0 delayed goto FAIL;
    gr7 = 2;

    gr0 = 0FEDCBA98h;
    [ar0] = gr0;
    gr1 = [ar0];
    gr1 = gr0-gr1;
    if <>0 delayed goto FAIL;
    gr7 = 3;
    
    ar0 = GET_NMC_ADDR(DBG_BASE_DEC,NMC_DBG_DRCR);
    gr0 = 1;
    [ar0] = gr0; // Halt Core! BST will control NMC

    gr0 = 500;
LOOP:
    gr0--;
    if <>0 goto LOOP;

PASS:
    gr7 = 0;
    return;
    
FAIL:
    return;
