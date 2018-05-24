/*
 * realmode.S.h
 *
 *  Created on: May 24, 2018
 *      Author: d.arsentiev
 */

#ifndef REALMODE_S_H_
#define REALMODE_S_H_

#define MMUCR_REALE 0x80000000

/*                                    LL             */
/*                    |  ERPN  |      11  SSSUUUUUUU */
/*                  --9876543210UWIMGEID--XWRXWR3210 */
#define RMPD_DATA 0b00000000000000101011001011010000

/*     init_realmode temp_regA, temp_reg_B  */
.macro init_realmode trA,       trB
	load_const \trB, RMPD_DATA
	mfspr \trA, SPR_RSTCFG
	or    \trA, \trA, \trB
	load_const \trB, MMUCR_REALE
	mtspr SPR_RMPD, \trA
	mfspr r3, SPR_MMUCR
	or    r3, r3, r4
	mtspr SPR_MMUCR, r3

.endm

#endif /* REALMODE_S_H_ */
