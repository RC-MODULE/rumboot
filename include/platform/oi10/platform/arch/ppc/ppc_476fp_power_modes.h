/*
 *  Created on: Jan 30, 2019
 *      Author: s.chernousov
 */

#ifndef PPC470S_POWER_MODES_H_
#define PPC470S_POWER_MODES_H_

#include <platform/devices.h>
#include <platform/regs/sctl.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c.h>
#include <arch/ppc_476fp_config.h>

void ppc470s_enter_sleep_mode( SCTL_PPC_SLP_TYPE const sleep_mode );
void ppc470s_enter_sleep_mode_with_EE( SCTL_PPC_SLP_TYPE const sleep_mode );
void ppc470s_enter_doze_mode();
void ppc470s_enter_doze_mode_with_EE();

void ppc470s_exit_doze_mode_on_noncritical_interrupt()
    {   spr_write(SPR_SRR1, spr_read(SPR_SRR1) & ~(1 << IBM_BIT_INDEX(64, 45)));} //xSRR[WE] = 0

void ppc470s_exit_doze_mode_on_critical_interrupt()
    {   spr_write(SPR_CSRR1, spr_read(SPR_CSRR1) & ~(1 << IBM_BIT_INDEX(64, 45)));} //xSRR[WE] = 0

void ppc470s_exit_sleep_mode_on_noncritical_interrupt()
    {
        ppc470s_exit_doze_mode_on_noncritical_interrupt();
        l2c_l2_write( DCR_L2C_BASE, L2C_L2SLEEPREQ, 0);
        //We must wait while L2SLEEPSTAT[SnoopIdle] == 1. See L2Cache core databook
        while(l2c_l2_read( DCR_L2C_BASE, L2C_L2SLEEPSTAT) & reg_field(31, 1)); //while L2SLEEPSTAT[SnoopIdle]
        isync();
    }

void ppc470s_exit_sleep_mode_on_critical_interrupt()
    {
        ppc470s_exit_doze_mode_on_critical_interrupt();
        l2c_l2_write( DCR_L2C_BASE, L2C_L2SLEEPREQ, 0 );
        //We must wait while L2SLEEPSTAT[SnoopIdle] == 1. See L2Cache core databook
        while(l2c_l2_read( DCR_L2C_BASE, L2C_L2SLEEPSTAT) & reg_field(31, 1)); //while L2SLEEPSTAT[SnoopIdle]
        isync();
    }

#endif /* PPC470S_POWER_MODES_H_ */
