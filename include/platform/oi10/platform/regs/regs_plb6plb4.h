/*
 * regs_plb6plb4.h
 *
 *  Created on: May 4, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_PLB6PLB4_H_
#define REGS_PLB6PLB4_H_

#define P64CR                       0x00
#define ESR                         0x01
#define P64REV                      0x02
#define EARH                        0x03
#define EARL                        0x04
#define ADDRL0                      0x05
#define ADDRH0                      0x06
#define ADDRL1                      0x07
#define ADDRH1                      0x08
#define TESR                        0x09


#define PLB6PLB4_WATERMARK_VALUE    0x00000108
/*
from /dut_hlb_cpu_v1.02/trunk/cpu/units/plb6plb4_v001/verilog/rtl/PLB6PLB4.defines.v
from http://svn.module.ru/r42_mm7705/mm7705/trunk/toplevel/verilog/rtl/v1/mpw7705_const.vh
"STRAP_" +define */
#define STRAP_PLB6PLB4_0_ADDRL0     0x000003FF
#define STRAP_PLB6PLB4_0_ADDRH0     0x000003FF
#define STRAP_PLB6PLB4_1_ADDRL0     0x00000020
#define STRAP_PLB6PLB4_1_ADDRH0     0x00000020
#define STRAP_PLB6PLB41_I_ADDRL0    0x00000012
#define STRAP_PLB6PLB41_I_ADDRH0    0x00000012
#define STRAP_PLB6PLB42_I_ADDRL0    0x00000013
#define STRAP_PLB6PLB42_I_ADDRH0    0x00000013


#define P64CR_0_RESET_VAL           0x00000203  /* due configuration */
#define P64CR_1_RESET_VAL           0x00000203
#define ESR_RESET_VAL               0x00000000
#define P64REV_RESET_VAL            PLB6PLB4_WATERMARK_VALUE
#define EARH_RESET_VAL              0x00000000
#define EARL_RESET_VAL              0x00000000
#define PLB6PLB4_0_ADDRL0_RESET_VAL STRAP_PLB6PLB4_0_ADDRL0
#define PLB6PLB4_0_ADDRH0_RESET_VAL	STRAP_PLB6PLB4_0_ADDRH0

#define PLB6PLB4_1_ADDRL0_RESET_VAL	STRAP_PLB6PLB4_1_ADDRL0
#define PLB6PLB4_1_ADDRH0_RESET_VAL	STRAP_PLB6PLB4_1_ADDRH0

#define PLB6PLB4_2_ADDRL0_RESET_VAL	STRAP_PLB6PLB41_I_ADDRL0
#define PLB6PLB4_2_ADDRH0_RESET_VAL	STRAP_PLB6PLB41_I_ADDRH0

#define PLB6PLB4_3_ADDRL0_RESET_VAL	STRAP_PLB6PLB42_I_ADDRL0
#define PLB6PLB4_3_ADDRH0_RESET_VAL	STRAP_PLB6PLB42_I_ADDRH0
#define ADDRL1_RESET_VAL            0x00000000
#define ADDRH1_RESET_VAL            0x00000000
#define TESR_RESET_VAL              0x00000000

#endif /* REGS_PLB6PLB4_H_ */
