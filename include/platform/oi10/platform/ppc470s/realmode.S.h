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

/*
	Usage example:
	init_realmode r3, r4
	isync
	b	main
*/

/* You must to synchronize context (isync) after usage. */
/*     init_realmode temp_regA, temp_reg_B  */
.macro init_realmode trA,       trB
	load_const \trB, RMPD_DATA
	mfspr \trA, SPR_RSTCFG
	or    \trA, \trA, \trB
	load_const \trB, MMUCR_REALE
	mtspr SPR_RMPD, \trA
	mfspr \trA, SPR_MMUCR
	or    \trA, \trA, \trB
	mtspr SPR_MMUCR, \trA

.endm

#endif /* REALMODE_S_H_ */
