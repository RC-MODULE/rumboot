/*
 *  Created on: Jan 30, 2019
 *      Author: s.chernousov
 */

#ifndef PPC470S_POWER_MODES_H_
#define PPC470S_POWER_MODES_H_

#include <platform/regs/sctl.h>

void ppc470s_enter_sleep_mode( SCTL_PPC_SLP_TYPE const sleep_mode );
void ppc470s_enter_sleep_mode_with_EE( SCTL_PPC_SLP_TYPE const sleep_mode );
void ppc470s_enter_doze_mode();
void ppc470s_enter_doze_mode_with_EE();


#endif /* PPC470S_POWER_MODES_H_ */
