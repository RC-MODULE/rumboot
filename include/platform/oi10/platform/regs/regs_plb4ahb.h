/*
 * regs_plb4ahb.h
 *
 *  Created on: May 4, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_PLB4AHB_H_
#define REGS_PLB4AHB_H_

/* DCR address space */


/* see cpu/units/plb4xahb_v001/verilog/rtl_dcr_addr/PLB4XAHB.defines.v */
#define UNIDIRECTIONAL_BRIDGE

/* PLB2AHB */
#define REVID                   0x0
#define SEAR_ADDRUPPER          0x1
#define SEAR_ADDRLOWER          0x2
#define SESR                    0x3
#define TOP_ADDR                0x4
#define BOT_ADDR                0x5
#define ATTRS                   0x6

#ifdef UNIDIRECTIONAL_BRIDGE
/* AHB2PLB (uni-directional) */
#define CONTROL                   0x1
#define ERR_ST                    0x2
#define ERR_ADDR                  0x3
#define INT_MSK                   0x4
#else
/* AHB2PLB (bi-directional) */
#define CONTROL                   0x7
#define ERR_ST                    0x8
#define ERR_ADDR                  0x9
#define INT_MSK                   0xA
#endif

/* RESET VALUES */
#define PLB4XAHB_DCR_REVID 0x00000320
/* from cpu/units/plb4xahb_v001/verilog/rtl_dcr_addr/PLB4XAHB.defines.v */

#define REVID_VALUE               PLB4XAHB_DCR_REVID
#define SEAR_ADDRUPPER_VALUE      0x00000000
#define SEAR_ADDRLOWER_VALUE      0x00000000
#define SESR_VALUE                0x00000000
#define TOP_ADDR_VALUE            0x00000000
#define BOT_ADDR_VALUE            0x00000000
#define ATTRS_VALUE               0x00000000
#define CONTROL_VALUE             0x00000004
#define ERR_ST_VALUE              0x00000000
#define ERR_ADDR_VALUE            0x00000000
#define INT_MSK_VALUE             0x00000000


#endif /* REGS_PLB4AHB_H_ */
