/*
 * regs_dcrarb.h
 *
 *  Created on: May 10, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_DCRARB_H_
#define REGS_DCRARB_H_

/* DCR address space, DCRARB */

#define DCRARB_TADDR  0x0
#define DCRARB_MADDR  0x1
#define DCRARB_MASK   0x2
#define DCRARB_DACR   0x3
#define DCRARB_DAESR  0x4
#define DCRARB_DAEAR  0x5
#define DCRARB_DASMR  0x6
#define DCRARB_REVID  0x7


/* from cpu/units/dcrarb4m_v001/verilog/rtl/DCRARB4M.v */
#define DCRARB_REVID_HARDCODED  0x00000102

/* from */
#define STRAP_ARBFAIRNESS       0
#define STRAP_ARBLEGACYPROTECT  0

#define DCRARB_TADDR_VALUE      0x00000000
#define DCRARB_MADDR_VALUE      0x00000000
#define DCRARB_MASK_VALUE       0x00000000
#define DCRARB_DACR_VALUE       reg_field(1, STRAP_ARBFAIRNESS) |\
								reg_field(0,STRAP_ARBLEGACYPROTECT)
#define DCRARB_DAESR_VALUE      0x00000000
#define DCRARB_DAEAR_VALUE      0x00000000
#define DCRARB_DASMR_VALUE      0x00000000
#define DCRARB_REVID_VALUE      DCRARB_REVID_HARDCODED



#endif /* REGS_DCRARB_H_ */
