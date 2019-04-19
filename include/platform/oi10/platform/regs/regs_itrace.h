/*
 * regs_itrace.h
 *
 *  Created on: May 14, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_ITRACE_H_
#define REGS_ITRACE_H_

#include <platform/regs/fields/itrace.h>


#define ITC0_TC         	0x00
#define ITC0_TS         	0x01
#define ITC0_TLA1       	0x02
#define ITC0_TLA2       	0x03
#define ITC0_WCNT       	0x04
#define ITC0_CTO        	0x05
#define ITC0_C1MH       	0x06
#define ITC0_C1ML       	0x07
#define ITC0_C2MH       	0x08
#define ITC0_C2ML       	0x09
#define ITC0_ITAI       	0x0A
#define ITC0_ITAD       	0x0B

/* default values */
#define ITC0_TC_DEFAULT		0x00
#define ITC0_TS_DEFAULT		0x00
#define ITC0_WCNT_DEFAULT	0x00
#define ITC0_CTO_DEFAULT	0x00
#define ITC0_C1MH_DEFAULT	0x00
#define ITC0_C1ML_DEFAULT	0x00 /* Reserved */
#define ITC0_C2MH_DEFAULT	0x00
#define ITC0_C2ML_DEFAULT	0x00 /* Reserved */

/* Masks */
#define ITC0_TC_MSK         0xFFE00000
#define ITC0_TS_MSK         0xFE000000
#define ITC0_C1MH_MSK       0xFFFF3FFF
#define ITC0_C2MH_MSK       0xFFFF3FFF


#endif /* REGS_ITRACE_H_ */


