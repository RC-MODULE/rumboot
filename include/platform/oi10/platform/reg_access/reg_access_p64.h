/*
 * reg_access_p64.h
 *
 *  Created on: Apr 9, 2018
 *      Author: v.strukov
 */

#ifndef REG_ACCESS_P64_H_
#define REG_ACCESS_P64_H_


#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/reg_access/macros.h>
#include <platform/regs/regs_p64.h>

REG_READ_DCR( p64, P64_P64CR, 32 )
REG_WRITE_DCR( p64, P64_P64CR, 32 )

REG_READ_DCR( p64, P64_ESR, 32 )
REG_WRITE_DCR( p64, P64_ESR, 32 )

REG_READ_DCR( p64, P64_P64REV, 32 )

REG_READ_DCR( p64, P64_EARH, 32 )

REG_READ_DCR( p64, P64_EARL, 32 )

REG_READ_DCR( p64, P64_ADDRL0, 32 )
REG_WRITE_DCR( p64, P64_ADDRL0, 32 )

REG_READ_DCR( p64, P64_ADDRH0, 32 )
REG_WRITE_DCR( p64, P64_ADDRH0, 32 )

REG_READ_DCR( p64, P64_ADDRL1, 32 )
REG_WRITE_DCR( p64, P64_ADDRL1, 32 )

REG_READ_DCR( p64, P64_ADDRH1, 32 )
REG_WRITE_DCR( p64, P64_ADDRH1, 32 )

REG_WRITE_DCR( p64, P64_TESR, 32 )

#endif /* REG_ACCESS_P64_H_ */
