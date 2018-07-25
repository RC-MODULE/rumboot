/*
 * reg_access_emi.h
 *
 *  Created on: Jul 25, 2018
 *      Author: s.chernousov
 */

#ifndef REG_ACCESS_EMI_H_
#define REG_ACCESS_EMI_H_

#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/reg_access/macros.h>
#include <platform/regs/regs_emi.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>

REG_READ_DCR( emi, EMI_SS0, 32 )
REG_WRITE_DCR( emi, EMI_SS0, 32 )

REG_READ_DCR( emi, EMI_SS1, 32 )
REG_WRITE_DCR( emi, EMI_SS1, 32 )

REG_READ_DCR( emi, EMI_SS2, 32 )
REG_WRITE_DCR( emi, EMI_SS2, 32 )

REG_READ_DCR( emi, EMI_SS3, 32 )
REG_WRITE_DCR( emi, EMI_SS3, 32 )

REG_READ_DCR( emi, EMI_SS4, 32 )
REG_WRITE_DCR( emi, EMI_SS4, 32 )

REG_READ_DCR( emi, EMI_SS5, 32 )
REG_WRITE_DCR( emi, EMI_SS5, 32 )

REG_READ_DCR( emi, EMI_SD0, 32 )
REG_WRITE_DCR( emi, EMI_SD0, 32 )

REG_READ_DCR( emi, EMI_SD1, 32 )
REG_WRITE_DCR( emi, EMI_SD1, 32 )

REG_READ_DCR( emi, EMI_SD2, 32 )
REG_WRITE_DCR( emi, EMI_SD2, 32 )

REG_READ_DCR( emi, EMI_SD3, 32 )
REG_WRITE_DCR( emi, EMI_SD3, 32 )

REG_READ_DCR( emi, EMI_SD4, 32 )
REG_WRITE_DCR( emi, EMI_SD4, 32 )

REG_READ_DCR( emi, EMI_SD5, 32 )
REG_WRITE_DCR( emi, EMI_SD5, 32 )

REG_READ_DCR( emi, EMI_RFC, 32 )
REG_WRITE_DCR( emi, EMI_RFC, 32 )

REG_READ_DCR( emi, EMI_HSTR, 32 )
REG_WRITE_DCR( emi, EMI_HSTR, 32 )

REG_READ_DCR( emi, EMI_ECNT20, 32 )
REG_WRITE_DCR( emi, EMI_ECNT20, 32 )

REG_READ_DCR( emi, EMI_ECNT53, 32 )
REG_WRITE_DCR( emi, EMI_ECNT53, 32 )

REG_READ_DCR( emi, EMI_H1ADR, 32 )
REG_WRITE_DCR( emi, EMI_H1ADR, 32 )

REG_READ_DCR( emi, EMI_H2ADR, 32 )
REG_WRITE_DCR( emi, EMI_H2ADR, 32 )

REG_READ_DCR( emi, EMI_RREQADR, 32 )
REG_WRITE_DCR( emi, EMI_RREQADR, 32 )

REG_READ_DCR( emi, EMI_WREQADR, 32 )
REG_WRITE_DCR( emi, EMI_WREQADR, 32 )

REG_READ_DCR( emi, EMI_WDADR, 32 )
REG_WRITE_DCR( emi, EMI_WDADR, 32 )

REG_READ_DCR( emi, EMI_BUSEN, 32 )
REG_WRITE_DCR( emi, EMI_BUSEN, 32 )

REG_READ_DCR( emi, EMI_WECR, 32 )
REG_WRITE_DCR( emi, EMI_WECR, 32 )

REG_READ_DCR( emi, EMI_IMR, 32 )
REG_WRITE_DCR( emi, EMI_IMR, 32 )

REG_READ_DCR( emi, EMI_IMR_SET, 32 )
REG_WRITE_DCR( emi, EMI_IMR_SET, 32 )

REG_READ_DCR( emi, EMI_IMR_RST, 32 )
REG_WRITE_DCR( emi, EMI_IMR_RST, 32 )

REG_READ_DCR( emi, EMI_IRR, 32 )
REG_WRITE_DCR( emi, EMI_IRR, 32 )

REG_READ_DCR( emi, EMI_IRR_RST, 32 )
REG_WRITE_DCR( emi, EMI_IRR_RST, 32 )

#endif /* REG_ACCESS_EMI_H_ */
