/*
 * regs_srammc2plb4.h
 *
 *  Created on: May 4, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_SRAMMC2PLB4_H_
#define REGS_SRAMMC2PLB4_H_

#define SRAMMC2PLB4_0_BASE DCR_SRAMMC2PLB4_0_BASE
#define SRAMMC2PLB4_1_BASE DCR_SRAMMC2PLB4_1_BASE

#define SRAMMC2PLB4_0_SB0CR		(SRAMMC2PLB4_0_BASE | SB0CR)
#define SRAMMC2PLB4_0_SB1CR		(SRAMMC2PLB4_0_BASE | SB1CR)
#define SRAMMC2PLB4_0_SB2CR		(SRAMMC2PLB4_0_BASE | SB2CR)
#define SRAMMC2PLB4_0_SB3CR		(SRAMMC2PLB4_0_BASE | SB3CR)
#define SRAMMC2PLB4_0_BEAR		(SRAMMC2PLB4_0_BASE | BEAR)
#define SRAMMC2PLB4_0_BESR0		(SRAMMC2PLB4_0_BASE | BESR0)
#define SRAMMC2PLB4_0_BESR1		(SRAMMC2PLB4_0_BASE | BESR1)
#define SRAMMC2PLB4_0_PMEG		(SRAMMC2PLB4_0_BASE | PMEG)
#define SRAMMC2PLB4_0_CID		(SRAMMC2PLB4_0_BASE | CID)
#define SRAMMC2PLB4_0_REVID		(SRAMMC2PLB4_0_BASE | REVID)
#define SRAMMC2PLB4_0_DPC		(SRAMMC2PLB4_0_BASE | DPC)

#define SRAMMC2PLB4_1_SB0CR		(SRAMMC2PLB4_1_BASE | SB0CR)
#define SRAMMC2PLB4_1_SB1CR		(SRAMMC2PLB4_1_BASE | SB1CR)
#define SRAMMC2PLB4_1_SB2CR		(SRAMMC2PLB4_1_BASE | SB2CR)
#define SRAMMC2PLB4_1_SB3CR		(SRAMMC2PLB4_1_BASE | SB3CR)
#define SRAMMC2PLB4_1_BEAR		(SRAMMC2PLB4_1_BASE | BEAR)
#define SRAMMC2PLB4_1_BESR0		(SRAMMC2PLB4_1_BASE | BESR0)
#define SRAMMC2PLB4_1_BESR1		(SRAMMC2PLB4_1_BASE | BESR1)
#define SRAMMC2PLB4_1_PMEG		(SRAMMC2PLB4_1_BASE | PMEG)
#define SRAMMC2PLB4_1_CID		(SRAMMC2PLB4_1_BASE | CID)
#define SRAMMC2PLB4_1_REVID		(SRAMMC2PLB4_1_BASE | REVID)
#define SRAMMC2PLB4_1_DPC		(SRAMMC2PLB4_1_BASE | DPC)

//next values from sys_ctrl.v ALTCR_PGM
#define SRAMMC2PLB4_0_SB0CR_RESET_VAL			0xFFFC098F
#define SRAMMC2PLB4_0_SB1CR_RESET_VAL			0xFFFD098F
#define SRAMMC2PLB4_0_SB2CR_RESET_VAL			0xFFFE098F
#define SRAMMC2PLB4_0_SB3CR_RESET_VAL			0xFFFF098F

#define SRAMMC2PLB4_1_SB0CR_RESET_VAL         0x00040980
#define SRAMMC2PLB4_1_SB1CR_RESET_VAL         0x00050980
#define SRAMMC2PLB4_1_SB2CR_RESET_VAL         0x00060980
#define SRAMMC2PLB4_1_SB3CR_RESET_VAL         0x00070980


#endif /* REGS_SRAMMC2PLB4_H_ */
