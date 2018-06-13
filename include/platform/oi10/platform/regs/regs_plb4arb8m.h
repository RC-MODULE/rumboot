/*
 * regs_plb4arb8m.h
 *
 *  Created on: May 4, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_PLB4ARB8M_H_
#define REGS_PLB4ARB8M_H_

#define PREV				0x2
#define PACR				0x3
#define PESR_RC				0x4
#define PESR_W				0x5
#define PEARL				0x6
#define PEARH				0x7

/* units/cpu/units/ppc_chip/verilog/rtl/PPC470S_TOP111323221111.v */
#define PACR_RESET_VAL		0x9B000000
#define PREV_RESET_VAL		0x00000301
// #define PACR_RESET_VAL		0x0B000000
#define PESR_RESET_VAL		0x00000000
#define PEARL_RESET_VAL		0x00000000
#define PEARH_RESET_VAL		0x00000000


#endif /* REGS_PLB4ARB8M_H_ */
