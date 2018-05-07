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
/* #define UNIDIRECTIONAL_BRIDGE */

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
/* PLB4AHB_0 */
#define PLB4AHB_0_REVID           (PLB4AHB_0_BASE + REVID)
#define PLB4AHB_0_SEAR_ADDRUPPER  (PLB4AHB_0_BASE + SEAR_ADDRUPPER)
#define PLB4AHB_0_SEAR_ADDRLOWER  (PLB4AHB_0_BASE + SEAR_ADDRLOWER)
#define PLB4AHB_0_SESR            (PLB4AHB_0_BASE + SESR)
#define PLB4AHB_0_TOP_ADDR        (PLB4AHB_0_BASE + TOP_ADDR)
#define PLB4AHB_0_BOT_ADDR        (PLB4AHB_0_BASE + BOT_ADDR)
#define PLB4AHB_0_ATTRS           (PLB4AHB_0_BASE + ATTRS)
#define PLB4AHB_0_CONTROL         (PLB4AHB_0_BASE + CONTROL)
#define PLB4AHB_0_ERR_ST          (PLB4AHB_0_BASE + ERR_ST)
#define PLB4AHB_0_ERR_ADDR        (PLB4AHB_0_BASE + ERR_ADDR)
#define PLB4AHB_0_INT_MSK         (PLB4AHB_0_BASE + INT_MSK)

/* PLB4AHB_1 */
#define PLB4AHB_1_REVID           (PLB4AHB_1_BASE + REVID)
#define PLB4AHB_1_SEAR_ADDRUPPER  (PLB4AHB_1_BASE + SEAR_ADDRUPPER)
#define PLB4AHB_1_SEAR_ADDRLOWER  (PLB4AHB_1_BASE + SEAR_ADDRLOWER)
#define PLB4AHB_1_SESR            (PLB4AHB_1_BASE + SESR)
#define PLB4AHB_1_TOP_ADDR        (PLB4AHB_1_BASE + TOP_ADDR)
#define PLB4AHB_1_BOT_ADDR        (PLB4AHB_1_BASE + BOT_ADDR)
#define PLB4AHB_1_ATTRS           (PLB4AHB_1_BASE + ATTRS)
#define PLB4AHB_1_CONTROL         (PLB4AHB_1_BASE + CONTROL)
#define PLB4AHB_1_ERR_ST          (PLB4AHB_1_BASE + ERR_ST)
#define PLB4AHB_1_ERR_ADDR        (PLB4AHB_1_BASE + ERR_ADDR)
#define PLB4AHB_1_INT_MSK         (PLB4AHB_1_BASE + INT_MSK)

/* PLB4AHB_2 */
#define PLB4AHB_2_REVID           (PLB4AHB_2_BASE + REVID)
#define PLB4AHB_2_SEAR_ADDRUPPER  (PLB4AHB_2_BASE + SEAR_ADDRUPPER)
#define PLB4AHB_2_SEAR_ADDRLOWER  (PLB4AHB_2_BASE + SEAR_ADDRLOWER)
#define PLB4AHB_2_SESR            (PLB4AHB_2_BASE + SESR)
#define PLB4AHB_2_TOP_ADDR        (PLB4AHB_2_BASE + TOP_ADDR)
#define PLB4AHB_2_BOT_ADDR        (PLB4AHB_2_BASE + BOT_ADDR)
#define PLB4AHB_2_ATTRS           (PLB4AHB_2_BASE + ATTRS)
#define PLB4AHB_2_CONTROL         (PLB4AHB_2_BASE + CONTROL)
#define PLB4AHB_2_ERR_ST          (PLB4AHB_2_BASE + ERR_ST)
#define PLB4AHB_2_ERR_ADDR        (PLB4AHB_2_BASE + ERR_ADDR)
#define PLB4AHB_2_INT_MSK         (PLB4AHB_2_BASE + INT_MSK)


#endif /* REGS_PLB4AHB_H_ */
