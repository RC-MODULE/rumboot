#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>
#include <platform/ppc470s/mmu.S.h>
#include <platform/regs/regs_plb6bc.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/devices.h>
#include <platform/regs/sctl.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c.h>

.section ".text","ax",@progbits

//r3-r6, r12 are used
.global ppc470s_enter_doze_mode_with_EE
ppc470s_enter_doze_mode_with_EE:
    load_const  r3, SCTL_PPC_SLP_CPU_DOZE

//r3-r6, r12 are used
.global ppc470s_enter_sleep_mode_with_EE
ppc470s_enter_sleep_mode_with_EE:
    load_const  r6, (0b1 << ITRPT_XSR_WE_i)\
                  | (0b1 << ITRPT_XSR_EE_i)
    b           ppc470s_enter_sleep_mode_

//r3-r6, r12 are used
.global ppc470s_enter_doze_mode
ppc470s_enter_doze_mode:
    load_const  r3, SCTL_PPC_SLP_CPU_DOZE

//r3-r6, r12 are used
.global ppc470s_enter_sleep_mode
ppc470s_enter_sleep_mode:
    load_const  r6, (0b1 << ITRPT_XSR_WE_i)

ppc470s_enter_sleep_mode_:
    load_const  r4, DCR_SCTL_BASE
    addi        r4, r4, SCTL_PPC_SLP_CTRL

set_ppc0_sleep_mode_:
    mfdcrx      r5, r4      // TODO RMW is point of potential racing
    //set SCTL_PPC_SLP_CTRL[PPC0_SLP_TYPE]
    copy_field( r5, IBM_BIT_INDEX(32, 3), IBM_BIT_INDEX(32, 2), \
                r3, IBM_BIT_INDEX(32, 1) )
    mtdcrx      r4, r5

    cmpwi       r3, SCTL_PPC_SLP_CPU_SLP
    bne         ppc470s_enter_sleep_mode_end_
    load_const  r4, DCR_L2C_BASE
    addi        r3, r4, L2C_L2CDCRAI
    addi        r4, r4, L2C_L2CDCRDI
    b           prepare_l2c_sleep_

prepare_l2c_sleep_:
    isync
    //msync was temporary commented. Msync calls snoopable request. Then "BC_SRSNZ error" is set in P6BC.BC_ERR (Non-zero segment snoopable request)
    msync

    load_const  r5, L2C_L2SLEEPREQ
    mtdcrx      r3, r5                          // prepare L2SLEEPREQ
    load_const  r5, reg_field(31, 0x1)
    mtdcrx      r4, r5                          // write L2SLEEPREQ[SleepRequest]=1

    load_const  r5, L2C_L2SLEEPSTAT
    mtdcrx      r3, r5                          // prepare L2SLEEPSTAT
wait_L2SLEEPSTAT_SnoopIdle_:
    mfdcrx      r5, r4                          // read L2SLEEPSTAT
    andi.       r5, r5, reg_field(31, 0x1)
    beq         wait_L2SLEEPSTAT_SnoopIdle_     // while(0 == L2SLEEPSTAT[SnoopIdle]);

    isync

ppc470s_enter_sleep_mode_end_:
    mfmsr       r3
    or          r3, r3, r6
    mtmsr       r3

    blr
